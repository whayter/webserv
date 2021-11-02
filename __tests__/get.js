const frisby = require('frisby');

describe('webserv', function() {

var baseUri = 'http://localhost:8080/';

let timeout = 10000;
jest.setTimeout(timeout);

// FileReader(""); read index.php and compare content to see if it match response body

//////////////////////////////////////////////////////////////////////////////
/// GET /////////////////////////////////////////////////////////////////////  

it ('GET /favicon.ico', function () {
  return frisby
  .get(baseUri + 'favicon.ico')
  .expect('status', 200)
});

it ('GET localhost:8080/', function () {
  return frisby
  .get(baseUri)
  .expect('status', 200)
  .expectNot('bodyContains', '<?php')
});

it ('GET localhost:8080/index.php', function () {
  return frisby
  .get(baseUri + 'index.php')
  .expect('status', 200)
  .expect('header', 'date')
  .expect('header', 'content-type', "text/html; charset=UTF-8")
  .expect('bodyContains', 'Webserv')
  .expect('bodyContains', 'CGI params')
  .expectNot('bodyContains', '<?php')
});

it ('GET localhost:8080/cgiParams.php?query=working', function () {
  return frisby
  .get(baseUri + 'cgiParams.php?query=working')
  .expect('status', 200)
  .expect('header', 'date')
  .expect('header', 'content-type', "text/html; charset=UTF-8")
  .expect('bodyContains', 'Webserv')
  .expect('bodyContains', 'CGI params')
  .expectNot('bodyContains', '<?php')
  .expect('bodyContains', 'srv_one')
  .expect('bodyContains', 'query=working')
});

it ('GET localhost:8080/cgiParams.php?webserv=done', function () {
  return frisby
  .get(baseUri + 'cgiParams.php?webserv=done')
  .expect('status', 200)
  .expect('header', 'date')
  .expect('header', 'content-type', "text/html; charset=UTF-8")
  .expect('bodyContains', 'Webserv')
  .expect('bodyContains', 'CGI params')
  .expectNot('bodyContains', '<?php')
  .expect('bodyContains', 'srv_one')
  .expect('bodyContains', 'webserv=done')
});

it ('GET localhost:8080/subject.html', function () {
  return frisby
  .get(baseUri + '/subject.html')
  .expect('status', 200)
  .expect('header', 'content-type', "text/html")
  .expect('bodyContains', 'Mandatory part')
});

it ('GET localhost:8080/NotExist', function () {
  return frisby
  .get(baseUri + '/NotExist')
  .expect('status', 404)
  .expect('bodyContains', 'Seems like your page doesn\'t exist anymore')
});

it ('GET localhost:8080/emptyFile.html', function () {
  return frisby
  .get(baseUri + '/emptyFile.html')
  .expect('status', 200)
  .expect('header', 'content-type', "text/html")
  .expect('header', 'content-length', "0")
});

it ('GET localhost:8080/emptyFile.php', function () {
  return frisby
  .get(baseUri + '/emptyFile.php')
  .expect('status', 200)
  .expect('header', 'content-type', "text/html; charset=UTF-8")
  .expect('header', 'content-length', "0")
});

it ('GET localhost:8080/autoindex', function () {
  return frisby
  .get(baseUri + 'autoindex')
  .expect('status', 200)
  .expect('header', 'date')
  .expect('header', 'content-type', "text/html")
  .expect('bodyContains', 'teletubbies')
});

it ('GET localhost:8080/bad_cgi', function () {
  return frisby
  .get(baseUri + 'bad_cgi')
  .expect('status', 500)
});

it ('GET localhost:8081', function () {
  return frisby
  .get('http://localhost:8081')
  .expect('status', 200)
  .expect('bodyContains', '<?php')
});

it ('GET localhost:8082', function () {
  return frisby
  .get('http://localhost:8082/')
  .expect('status', 200)
  .expect('bodyContains', 'This project is here to make you write your HTTP server.')
});

it ('GET localhost:8082/uri', function () {
  return frisby
  .get('http://localhost:8082/uri')
  .expect('status', 200)
  .expect('bodyContains', 'Copyright The Closure Library Authors')
});

it ('GET localhost:8083', function () {
  return frisby
  .get('http://localhost:8083')
  .expect('status', 200)
  .expect('bodyContains', 'index of /')
  .expect('bodyContains', 'Makefile')
});

//////////////////////////////////////////////////////////////////////////////
/// POST ////////////////////////////////////////////////////////////////////

let oneM = 1024 * 1024

it ('POST localhost:8080/ with body size bigger than max_body_size', function () {
  return frisby.timeout(timeout)
  .setup({
    request: {
      body: "x".repeat(oneM + 1)
    }
  }, timeout)
  .post(baseUri + 'uploads/test.html')
  .expect('status', 413)
});

it ('POST localhost:8080/ with body size way bigger than max_body_size', function () {
  return frisby.timeout(timeout)
  .setup({
    request: {
      body: "x".repeat(oneM * 2)
    }
  }, timeout)
  .post(baseUri + 'uploads/test.html')
  .expect('status', 413)
});

it ('POST localhost:8080/ with body size = max_body_size', function () {
  return frisby.timeout(timeout)
  .setup({
    request: {
      body: "x".repeat(oneM)
    }
  })
  .post(baseUri + 'uploads/testPostMethod.html')
  .expect('status', 201)
});

it ('POST localhost:8080/cgiParams.php', function () {
  return frisby.timeout(timeout)
  .setup({
    request: {
      body: "winston"
    }
  })
  .post(baseUri + 'cgiParams.php')
  .expect('status', 200)
  .expect('bodyContains', 'POST')
  .expect('bodyContains', 'winston')
});

//////////////////////////////////////////////////////////////////////////////
/// DELETE //////////////////////////////////////////////////////////////////  

it ('DELETE http://localhost:8080/testPostMethod.html', function () {
  return frisby
    .del('http://localhost:8080/testPostMethod.html')
    .expect('status', 405);
});

it ('DELETE http://localhost:8080/uploads/testPostMethod.html', function () {
  return frisby
    .del('http://localhost:8080/uploads/testPostMethod.html')
    .expect('status', 200)
    .expect('bodyContains', 'File deleted')
});

it ('DELETE http://localhost:8080/uploads/notExist.html', function () {
  return frisby
    .del('http://localhost:8080/uploads/notExist.html')
    .expect('status', 404)
});

//////////////////////////////////////////////////////////////////////////////
/// PUT /////////////////////////////////////////////////////////////////////

it ('PUT http://localhost:8080 should return Not implemented 501', function () {
  return frisby
  .put('http://localhost:8080')
  .expect('status', 501);
});



// 262144 max char macos (getconf ARG_MAX) 
// var max_len_uri = 8000; // choose our random max len uri 7980
// it ('GET with max uri len should return 404 not found', function () {
//   return frisby
//   .get(baseUri + "x".repeat(max_len_uri- baseUri.length + 3))
//   .expect('status', 404)
// });

// it ('GET with max uri len + 1 should return 414 URI Too Long', function () {
//   return frisby
//   .get(baseUri + "x".repeat(max_len_uri - baseUri.length + 4))
//   .expect('status', 414)
// });

// test on folders, (autoindex or not, index or not (so 403 or 404 ?))
// test bad http protocol version
// etc...
      
});
