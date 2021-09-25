#ifndef CGI_HPP
#define CGI_HPP

#include "ServerConfig.hpp"
#include "utility.hpp"
#include "filesystem.h"
#include "Request.hpp"

#include <unistd.h>
#include <sys/wait.h>

#include <iostream>
#include <string>

#include <cstdlib>

#include <istream>

namespace fs = ft::filesystem;

extern char** environ;

void setEnvironment(http::Request& request, ServerBlock& sblock, fs::path path)
{
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 0);
	setenv("SERVER_NAME", sblock.getServerName().c_str(), 0);
	setenv("SERVER_SOFTWARE", "webserv/1.0", 0);
	setenv("SERVER_PROTOCOL", "http/1.1", 0);
	setenv("HTTP_ACCEPT", request.getHeader("Accept").c_str(), 0);
	setenv("HTTP_ACCEPT_CHARSET", request.getHeader("Accept-Charset").c_str(), 0);
	setenv("HTTP_ACCEPT_ENCODING", request.getHeader("Accept-Encoding").c_str(), 0);
	setenv("HTTP_ACCEPT_LANGUAGE", request.getHeader("Accept-Language").c_str(), 0);
	setenv("HTTP_CONNECTION", request.getHeader("Connection").c_str(), 0);
	setenv("HTTP_HOST", request.getUri().getHost().c_str(), 0);
	setenv("HTTPS", "", 0);
	setenv("REQUEST_METHOD", request.getMethod().c_str(), 0);
	setenv("REQUEST_SCHEME", request.getUri().getScheme().c_str(), 0);
	setenv("REQUEST_URI", request.getUri().getPath().c_str(), 0);
	setenv("QUERY_STRING", request.getUri().getQuery().c_str(), 0);
	setenv("DOCUMENT_ROOT", sblock.getRoot().c_str(), 0);
	// j'ai rajoute ca juste pour desactiver l'erreur cgi sous linux:
	setenv("REDIRECT_STATUS", "", 1);
	// <p>This PHP CGI binary was compiled with force-cgi-redirect enabled.  This
	// means that a page will only be served up if the REDIRECT_STATUS CGI variable is
	// set, e.g. via an Apache Action directive.</p>

	setenv("SCRIPT_FILENAME", path.c_str(), 0);

	setenv("CONTENT_TYPE", request.getHeader("Content-type").c_str(), 0);
	setenv("CONTENT_LENGTH", request.getHeader("Content-Length").c_str(), 0);
}

void unsetEnvironment()
{
	unsetenv("GATEWAY_INTERFACE");
	unsetenv("SERVER_NAME");
	unsetenv("SERVER_SOFTWARE");
	unsetenv("SERVER_PROTOCOL");
	unsetenv("HTTP_ACCEPT");
	unsetenv("HTTP_ACCEPT_CHARSET");
	unsetenv("HTTP_ACCEPT_ENCODING");
	unsetenv("HTTP_ACCEPT_LANGUAGE");
	unsetenv("HTTP_CONNECTION");
	unsetenv("HTTP_HOST");
	unsetenv("HTTPS");
	unsetenv("REQUEST_METHOD");
	unsetenv("REQUEST_SCHEME");
	unsetenv("REQUEST_URI");
	unsetenv("QUERY_STRING");
	unsetenv("DOCUMENT_ROOT");
	unsetenv("SCRIPT_FILENAME");
	unsetenv("CONTENT_TYPE");
	unsetenv("CONTENT_LENGTH");
}

static void replacePipeEnd(int oldFd, int newFd)
{
	if (oldFd != newFd)
	{
		dup2(oldFd, newFd);
		close(oldFd);
	}
}

std::vector<unsigned char> getCgiResponse(std::string cgiExecPath)
{	
	std::vector<unsigned char> r;
	int childToParent[2];
	int parentToChild[2];
	pipe(childToParent);
	pipe(parentToChild);	
	pid_t pid = fork();
	if (pid == 0)										// child process
	{
		close(childToParent[0]);						// close reading end of childToParent
		close(parentToChild[1]);						// close writing end of parentToChild	
		replacePipeEnd(parentToChild[0], 0);			// replace stdin with incoming pipe
		replacePipeEnd(childToParent[1], 1);			// replace stdout with outgoing pipe

		// read data from parent here (if method=POST)

		char* arg = 0;
		execve(cgiExecPath.c_str(), &arg, environ);				// exec cgi binary
	}
	else if (pid > 0)									// main process
	{
		close(parentToChild[0]);						// close reading end of parentToChild
		close(childToParent[1]);						// close writing end of childToParent

		// send data to child here (if method=POST)
		
		close(parentToChild[1]);						// close writing end of parentToChild
		waitpid(pid, NULL, 0);							// wait for child process to end.
		replacePipeEnd(childToParent[0], 0);			// replace stdin with incoming pipe

		char buffer[4096];							// tmp buf size

		size_t nbytes;
		while ((nbytes = read(STDIN_FILENO, buffer, 4096)) > 0)
			for (size_t i = 0; i < nbytes; i++)
				r.push_back(buffer[i]);
	}
	return r;
}

#endif