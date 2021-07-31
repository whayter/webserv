# Webserv

## Documentation

yeye
ok

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

- [ ] choose the [port and host](http://nginx.org/en/docs/http/ngx_http_core_module.html#listen) of each "[server](https://nginx.org/en/docs/http/ngx_http_core_module.html#server)"
- [ ] setup the [server_names](http://nginx.org/en/docs/http/server_names.html) or not
- [ ] The first server for a host:port will be the default for this host:port (meaning it will answer to all request that doesn’t belong to an other server)
- [ ] setup default [error pages](http://nginx.org/en/docs/http/ngx_http_core_module.html#error_page)
- [ ] limit [client body size](http://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size)
- [ ] [setup routes](http://nginx.org/en/docs/http/ngx_http_core_module.html#location) with one or multiple of the following rules/configuration (routes wont be using regexp):
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
- [Design pattern helper 2](https://sourcemaking.com/design_patterns)

#### http server + sockets, etc...

- [Good server example, using builder and singleton pattern](https://users.cs.jmu.edu/bernstdh/web/common/lectures/slides_http-server-example_java.php)
- [Socket programming tuto (java)](https://www.infoworld.com/article/2853780/socket-programming-for-scalable-systems.html)
- [Cours prog reseau c++ - Tcp - Un premier serveur](https://bousk.developpez.com/cours/reseau-c++/TCP/08-premier-serveur-mini-serveur/)
- [Writing C++ HTTP server from scratch (1)-thread pool](https://www.programmersought.com/article/51864013425/)
- https://codereview.stackexchange.com/questions/217783/simple-http-server
- [Developing Flexible and High-performance Web Servers with Frameworks and Patterns](https://www.dre.vanderbilt.edu/~schmidt/PDF/computing-surveys.pdf)

#### Parsing http request

- [Writing http parser](https://fr.slideshare.net/fukamachi/writing-a-fast-http-parser)

#### namespaces
- [Namespace naming convention](https://stackoverflow.com/questions/228783/what-are-the-rules-about-using-an-underscore-in-a-c-identifier)
- [how to use namespace to separate interface from implementation](https://softwareengineering.stackexchange.com/questions/233672/how-to-use-namespaces-to-separate-interface-from-implementation-in-c/399588)
- [The problems with Avoiding Smurf Naming classes with namespaces](https://softwareengineering.stackexchange.com/questions/191929/the-problems-with-avoiding-smurf-naming-classes-with-namespaces?noredirect=1&lq=1)
- [Multiple classes with the same name, but different namespaces?](https://softwareengineering.stackexchange.com/questions/251760/multiple-classes-with-the-same-name-but-different-namespaces?rq=1)

#### Nginx doc
- [nginx doc](http://nginx.org/en/)
- [Creating NGINX Plus and NGINX Configuration Files](https://docs.nginx.com/nginx/admin-guide/basic-functionality/managing-configuration-files/)
	- [server block](https://nginx.org/en/docs/http/ngx_http_core_module.html#server)
- [How nginx process request](https://nginx.org/en/docs/http/request_processing.html)
- [all nginx config directives](https://nginx.org/en/docs/http/ngx_http_core_module.html)

#### Sync fork

- [Syncing a fork](https://docs.github.com/en/github/collaborating-with-pull-requests/working-with-forks/syncing-a-fork)

#### config
- https://stackoverflow.com/questions/16041623/the-best-way-to-handle-config-in-large-c-project
- https://www.guru99.com/compiler-design-lexical-analysis.html#2
- https://stackoverflow.com/questions/14954721/what-is-the-difference-between-a-token-and-a-lexeme
- https://tlevesque.developpez.com/tutoriels/csharp/parser-texte-csharp-sprache/#Lno-III
- https://rosettacode.org/wiki/Compiler/lexical_analyzer
- https://visualstudiomagazine.com/articles/2014/06/01/compiler-basics-part-2.aspx
