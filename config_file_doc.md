Syntax:	 autoindex on | off;
Default: autoindex off;
Context: server, location
http://nginx.org/en/docs/http/ngx_http_autoindex_module.html

Syntax:	client_max_body_size size;
Default: client_max_body_size 1m;
Context: server, location
http://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size
http://nginx.org/en/docs/syntax.html


Syntax:	root path;
Default: root html;
Context: server, location
http://nginx.org/en/docs/http/ngx_http_core_module.html#root
http://nginx.org/en/docs/http/ngx_http_core_module.html#alias

Syntax:	index file;
Default:	
index index.html;
Context: server, location
http://nginx.org/en/docs/http/ngx_http_index_module.html

Syntax:	error_page code ... uri;
Default: —
Context: server, location
http://nginx.org/en/docs/http/ngx_http_core_module.html#error_page


Syntax:	server { ... }
Default:	—
Context: main
https://nginx.org/en/docs/http/ngx_http_core_module.html#server

Syntax:  **listen** address[:port];
**listen** port;
Default: listen *:80;
Context: server
http://nginx.org/en/docs/http/ngx_http_core_module.html#listen
http://nginx.org/en/docs/http/request_processing.html

Syntax:	server_name name;
Default: server_name "";
Context: server
https://nginx.org/en/docs/http/ngx_http_core_module.html#server_name
http://nginx.org/en/docs/http/server_names.html

Syntax:	**location** uri { ... }
**location** ext .extention { ... }
Default:	—
Context: server
https://nginx.org/en/docs/http/ngx_http_core_module.html#location


Syntax:	limit_except method ...;
Default:	—
Context:	location
https://nginx.org/en/docs/http/ngx_http_core_module.html#limit_except

Syntax:	**return** code [text];
**return** code URL;
<!-- **return** URL; -->
Default: —
Context: server, location
http://nginx.org/en/docs/http/ngx_http_rewrite_module.html#return
https://developer.mozilla.org/en-US/docs/Web/HTTP/Redirections




Syntax:	cgi_exec binary_path;
Default:	—
Context:	location


Syntax: upload_store <location>;
Default:	—
Context:	server, location
https://www.nginx.com/resources/wiki/modules/upload/


<!-- impl in parsing but useless -->
Syntax:	cgi_param parameter value [if_not_empty];
Default:	—
Context:	location

<!-- //////////////// Not impl ////////////// -->
Syntax: upload_pass <location>;
Default:	—
Context:	server, location
https://www.nginx.com/resources/wiki/modules/upload/

Syntax:	fastcgi_pass address;
Default:	—
Context:	location
http://nginx.org/en/docs/http/ngx_http_fastcgi_module.html#fastcgi_pass

Syntax:	fastcgi_param parameter value [if_not_empty];
Default:	—
Context:	location
http://nginx.org/en/docs/http/ngx_http_fastcgi_module.html#fastcgi_param

Syntax:	default_type mime-type;
Default: default_type text/plain;
Context: http, server, location
https://nginx.org/en/docs/http/ngx_http_core_module.html#default_type

Syntax:	types { ... }
Default:	
types {
    text/html  html;
    image/gif  gif;
    image/jpeg jpg;
}
Context:	server, location
https://nginx.org/en/docs/http/ngx_http_core_module.html#types

