# Webserv

## Documentation

[Build a simple http server from scratch (C)](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)

[RFC 7230 (message syntax and routing)](https://www.rfc-editor.org/rfc/pdfrfc/rfc7230.txt.pdf)

[RFC 7231 (semantics and content)](https://www.rfc-editor.org/rfc/pdfrfc/rfc7231.txt.pdf)

[RFC 7232 (conditional requests)](https://www.rfc-editor.org/rfc/pdfrfc/rfc7232.txt.pdf)

[RFC 7233 (range requests)](https://www.rfc-editor.org/rfc/pdfrfc/rfc7233.txt.pdf)

[RFC 7234 (catching)](https://www.rfc-editor.org/rfc/pdfrfc/rfc7234.txt.pdf)

[RFC 7235 (authentication)](https://www.rfc-editor.org/rfc/pdfrfc/rfc7235.txt.pdf)

## To do list / check list from subject:

#### General 

- [ ] The C++ standard must be C++ 98. Your project must compile with it.
- [ ] No external library, no Boost, etc...
- [ ] It must be non-blocking and use only 1 poll (or equivalent) for all the IO between the client and the server (listens includes).
- [ ] poll (or equivalent) should check read and write at the same time.
- [ ] Your server should never block and the client should be bounce properly if necessary.
- [ ] You should never do a read operation or a write operation without going through poll (or equivalent).
- [ ] Checking the value of errno is strictly forbidden after a read or a write operation.
- [ ] A request to your server should never hang forever.
- [ ] You server should have default error pages if none are provided.
- [ ] Your program should not leak and should never crash, (even when out of memory if all the initialization is done)
- [ ] You can’t use fork for something else than CGI (like php or python etc...)
- [ ] You can’t execve another webserver...
- [ ] Your program should have a config file in argument or use a default path.
- [ ] You don’t need to use poll (or equivalent) before reading your config file.
- [ ] You should be able to serve a fully static website.
- [ ] Client should be able to upload files.
- [ ] Your HTTP response status codes must be accurate.
- [ ] You need at least GET, POST, and DELETE methods.
- [ ] Stress tests your server it must stay available at all cost.

#### Config file should be able to :

- [ ] choose the port and host of each "server"
- [ ] setup the server_names or not
- [ ] The first server for a host:port will be the default for this host:port (meaning it will answer to all request that doesn’t belong to an other server)
- [ ] setup default error pages
- [ ] limit client body size
- [ ] setup routes with one or multiple of the following rules/configuration (routes wont be using regexp):
	- [ ] define a list of accepted HTTP Methods for the route
	- [ ] define an HTTP redirection.
	- [ ] define a directory or a file from where the file should be search (for example if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet)
	- [ ] turn on or off directory listing
	- [ ] default file to answer if the request is a directory
	- [ ] execute CGI based on certain file extension (for example .php)
		- [ ] You wonder what a CGI is ?
		- [ ] Because you won’t call the CGI directly use the full path as PATH_INFO
		- [ ] Just remembers that for chunked request, your server needs to unchunked it and the CGI will expect EOF as end of the body.
		- [ ] Same things for the output of the CGI. if no content_length is returned from the CGI, EOF will mean the end of the returned data.
		- [ ] Your program should call the cgi with the file requested as first argument
		- [ ] The cgi should be run in the correct directory for relativ path file access
		- [ ] Your server should work with one CGI (php-cgi, python...)
	- [ ] make the route able to accept uploaded files and configure where it should be saved
- [ ] You must provide some configuration files and default basic files to test/demonstrate every feature is working during eval


## Our todo list :
- [ ] ?

## Refs :

- [Design pattern helper](https://refactoring.guru/design-patterns)
- [Namespace naming convention](https://stackoverflow.com/questions/228783/what-are-the-rules-about-using-an-underscore-in-a-c-identifier)
