[![C/C++ CI](https://github.com/Working-From-Home/webserv/actions/workflows/ci.yml/badge.svg)](https://github.com/Working-From-Home/webserv/actions/workflows/ci.yml)

## 42 - webserv

#### firsts things first, install php-cgi
###### linux
- sudo apt install php-cgi
###### macos
 - idk, try with brew bro.

In order to make python and ruby cgi works, you must make files executable.

## Documentation

[Build a simple http server from scratch (C)](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)

[RFC 7230 (message syntax and routing)](https://www.rfc-editor.org/rfc/pdfrfc/rfc7230.txt.pdf)

[RFC 7231 (semantics and content)](https://www.rfc-editor.org/rfc/pdfrfc/rfc7231.txt.pdf)

[RFC 7232 (conditional requests)](https://www.rfc-editor.org/rfc/pdfrfc/rfc7232.txt.pdf)

[RFC 7233 (range requests)](https://www.rfc-editor.org/rfc/pdfrfc/rfc7233.txt.pdf)

[RFC 7234 (catching)](https://www.rfc-editor.org/rfc/pdfrfc/rfc7234.txt.pdf)

[RFC 7235 (authentication)](https://www.rfc-editor.org/rfc/pdfrfc/rfc7235.txt.pdf)

[The Common Gateway Interface (CGI) Version 1.1 ](https://datatracker.ietf.org/doc/html/rfc3875)



## generated uml diagram webserv :
![image2](https://raw.githubusercontent.com/Working-From-Home/webserv/uml/docs/diagrams/out/webserv.svg)

## Refs :

- [Design pattern helper](https://refactoring.guru/design-patterns)
- [Design pattern helper 2](https://sourcemaking.com/design_patterns)

#### http server + sockets, etc...


- [Good server example, using builder and singleton pattern](https://users.cs.jmu.edu/bernstdh/web/common/lectures/slides_http-server-example_java.php)
	- [design improved](https://users.cs.jmu.edu/bernstdh/web/common/lectures/slides_http-servlet-example_java.php) 
- [Socket programming tuto (java)](https://www.infoworld.com/article/2853780/socket-programming-for-scalable-systems.html)

- [Cours prog reseau c++ - Tcp - Premiers pas en tant que server](https://bousk.developpez.com/cours/reseau-c++/TCP/04-serveur-premiers-pas)
- [Cours prog reseau c++ - Tcp - Un premier serveur](https://bousk.developpez.com/cours/reseau-c++/TCP/08-premier-serveur-mini-serveur/)
- [Writing C++ HTTP server from scratch (1)-thread pool](https://www.programmersought.com/article/51864013425/)
- https://codereview.stackexchange.com/questions/217783/simple-http-server
- [Let’s Build A Web Server (Python)](https://ruslanspivak.com/lsbaws-part1/)
- [Developing Flexible and High-performance Web Servers with Frameworks and Patterns](https://www.dre.vanderbilt.edu/~schmidt/PDF/computing-surveys.pdf)
- http://manpagesfr.free.fr/man/man7/ip.7.html

- [socket - Address family](https://www.ibm.com/docs/en/i/7.4?topic=family-af-inet-address)
- [Basic socket design](https://www.ibm.com/docs/en/i/7.4?topic=programming-basic-socket-design)
- [Advanced socket concepts - Non blocking I/O](https://www.ibm.com/docs/en/i/7.4?topic=concepts-nonblocking-io)
- [Async prog, blocking and non blocking I/O](https://luminousmen.com/post/asynchronous-programming-blocking-and-non-blocking)

![image](https://www.ibm.com/docs/en/ssw_ibm_i_74/rzab6/rxab6502.gif)

#### Parsing http request

- [Writing http parser](https://fr.slideshare.net/fukamachi/writing-a-fast-http-parser)
- [Http Message Format](https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages)
- [Http Requests](https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages#http_requests)
- [The anatomy of http request](https://betterprogramming.pub/the-anatomy-of-an-http-request-728a469ecba9)
- [Design and Evaluation of HTTP Protocol Parsers](https://hal.inria.fr/hal-01497010/document)
- [Builder design pattern in modern c](https://dzone.com/articles/builder-design-pattern-in-modern-c)
- https://perishablepress.com/stop-using-unsafe-characters-in-urls/#unreserved-characters

- [The normal procedure for parsing an HTTP message is to read the
   start-line into a structure, read each header field into a hash table
   by field name until the empty line, and then use the parsed data to
   determine if a message body is expected.  If a message body has been
   indicated, then it is read as a stream until an amount of octets
   equal to the message body length is read or the connection is closed.](https://datatracker.ietf.org/doc/html/rfc7230#page-20)
- [Field value component (allowed char)](https://datatracker.ietf.org/doc/html/rfc7230#section-3.2.6)
- https://stackoverflow.com/questions/47687379/what-characters-are-allowed-in-http-header-values

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
- https://www.nginx.com/wp-content/uploads/2018/12/NGINX-Conf-2018-slides_Hulegaard-config-analysis.pdf

- [Scoped enum in c++98](https://stackoverflow.com/questions/61269537/how-to-implement-scoped-enum-in-c98-and-can-be-used-like-enum-class-in-c11)

- [scanner class example](https://link.springer.com/content/pdf/bbm%3A978-0-387-79422-8%2F1.pdf)
	- [entire book (link above is missing explanations)](https://1lib.fr/book/540032/438c1d)
- [std::variant an union...](https://www.bfilipek.com/2018/06/variant.html)
- [union and variant guideline](https://www.modernescpp.com/index.php/c-core-guidelines-rules-for-unions)
- [c++98 - union restricted for complex type like string...and variant not in c++98...](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2544.pdf)
- [type punning using reinterpret cast undefined ?](https://stackoverflow.com/questions/53995657/is-reinterpret-cast-type-punning-actually-undefined-behavior)

- [scanner - how to know if next-char is eof](https://stackoverflow.com/questions/6283632/how-to-know-if-the-next-character-is-eof-in-c)
- https://www.cs.colostate.edu/~mstrout/CS553Fall06/slides/lecture02a-scanparse.pdf
- [for lexer parser example from father of c++](http://index-of.co.uk/Programming/The%20C++%20Programming.Language.4th.Edition.Jun.2013%5BA4%5D.pdf)
- [How to parse http req - example lib poco](https://stackoverflow.com/questions/15179409/how-to-parse-http-request-in-c)

- [Youtubev](https://www.youtube.com/watch?v=KRbzEaUmog8&list=PLKUb7MEve0TjHQSKUWChAWyJPCpYMRovO&index=54)
- [Youtube2 - LL(1)](https://www.youtube.com/watch?v=qYvCFUEseQI&list=PLKUb7MEve0TjHQSKUWChAWyJPCpYMRovO&index=53&t=1s)
- [Youtube3 - LL(1) recursive decent parser stategy](https://www.youtube.com/watch?v=qYvCFUEseQI&list=PLKUb7MEve0TjHQSKUWChAWyJPCpYMRovO&index=53&t=1s)

- [looking for a clear definition of what a tokenizer parser and lexers are](https://stackoverflow.com/questions/380455/looking-for-a-clear-definition-of-what-a-tokenizer-parser-and-lexers-are)

#### structure file interface, header...
- https://softwareengineering.stackexchange.com/questions/323738/how-can-i-effectively-use-one-file-per-class-in-c
- https://stackoverflow.com/questions/27102956/structuring-c-application-directory-and-folders
- https://www.fluentcpp.com/2017/09/22/make-pimpl-using-unique_ptr/
- 
#### Google c++ style guide
- https://google.github.io/styleguide/cppguide.html

#### Sockets
- https://www.youtube.com/watch?v=YwHErWJIh6Y

#### FCGI
- http://www.chiark.greenend.org.uk/doc/libfcgi0ldbl/fastcgi-prog-guide/ch1intro.htm#9432

#### Http decision tree
- https://news.ycombinator.com/item?id=12067809
- https://github.com/for-GET/http-decision-diagram
![image2](https://camo.githubusercontent.com/d09839bf7ae593fa403793326a9af335e9392d622f89ea3ee13b889c02ece2fc/68747470733a2f2f7261776769746875622e636f6d2f666f722d4745542f687474702d6465636973696f6e2d6469616772616d2f6d61737465722f6874747064642e706e67)

#### Directory Traversal Attacks
- https://www.acunetix.com/websitesecurity/directory-traversal/


- https://www.joelonsoftware.com/2000/04/06/things-you-should-never-do-part-i/


#### valgrind
- https://valgrind.org/docs/manual/faq.html#faq.reports

#### filesystem
- https://en.cppreference.com/w/cpp/header/filesystem
- https://www.cppstories.com/2017/08/cpp17-details-filesystem/
- https://stackoverflow.com/questions/40899267/how-similar-are-boost-filesystem-and-the-c-standard-filesystem-library
- http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1030r1.pdf
- http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0218r0.html

#### pimpl
- https://www.fluentcpp.com/2017/09/22/make-pimpl-using-unique_ptr/


- https://blog.spacepatroldelta.com/a?ID=01500-34f01d9e-2d48-4ea4-a26d-0a3deabfdb4b

####  Http connections
https://www.w3.org/Protocols/rfc2616/rfc2616-sec8.html

#### transfer encoding
- https://httpwg.org/specs/rfc7230.html#transfer.codings
- https://httpwg.org/specs/rfc7230.html#chunked.encoding
- https://httpwg.org/specs/rfc7230.html#decoding.chunked

##### other
- https://stackoverflow.com/questions/59261547/handle-ctrlc-in-client-server-socket-in-c-programing


### cgi
- https://helpx.adobe.com/coldfusion/cfml-reference/reserved-words-and-variables/cgi-environment-cgi-scope-variables/cgi-server-variables.html
- http://www.faqs.org/rfcs/rfc3875.html
- https://www.php.net/manual/fr/reserved.variables.server.php

#### multipart
- https://stackoverflow.com/questions/913626/what-should-a-multipart-http-request-with-multiple-files-look-like
- https://www.w3.org/Protocols/rfc1341/7_2_Multipart.html
- https://developer.mozilla.org/fr/docs/Web/HTML/Element/Input/file
- https://www.w3.org/TR/html401/interact/forms.html#h-17.13.4.2

#### reactor and c10k problem
- https://dzone.com/articles/understanding-reactor-pattern-thread-based-and-eve
- http://www.kegel.com/c10k.html
- https://hila.sh/2019/12/28/reactor.html
- https://raw.githubusercontent.com/swn73/books/master/Packt.Nginx.HTTP.Server.3rd.Edition.pdf
- https://www.adamtornhill.com/Patterns%20in%20C%205,%20REACTOR.pdf

#### nginx
- https://raw.githubusercontent.com/swn73/books/master/Packt.Nginx.HTTP.Server.3rd.Edition.pdf

#### http header value  ',' (list syntax) vs ';'
- https://stackoverflow.com/a/38406581/5374043
- https://github.com/bnoordhuis/mozilla-central/blob/master/netwerk/protocol/http/nsHttpHeaderArray.h#L185
