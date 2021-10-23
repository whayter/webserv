const frisby = require('frisby');

describe('webserv', function() {

var baseUri = 'http://localhost:80/';

// FileReader(""); read index.php and compare content to see if it match response body

///////////////////////////////////////////////////////////////////////////
/// GET //////////////////////////////////////////////////////////////////  

it ('GET /favicon.ico', function () {
  return frisby
  .get(baseUri + 'favicon.ico')
  .expect('status', 200)
});

it ('GET localhost:80/', function () {
  return frisby
  .get(baseUri)
  .expect('status', 200)
  .expectNot('bodyContains', '<?php')
});

it ('GET localhost:80/index.php', function () {
  return frisby
  .get(baseUri + 'index.php')
  .expect('status', 200)
  .expect('header', 'date')
  .expect('header', 'content-type', "text/html; charset=UTF-8")
  .expect('bodyContains', 'Webserv')
  .expect('bodyContains', 'CGI params')
  .expectNot('bodyContains', '<?php')
});

it ('GET localhost:80/NotExist', function () {
  return frisby
  .get(baseUri + '/NotExist')
  .expect('status', 404)
  .expect('bodyContains', 'Seems like your page doesn\'t exist anymore')
});

it ('GET localhost:80/autoindex', function () {
  return frisby
  .get(baseUri + 'autoindex')
  .expect('status', 200)
  .expect('header', 'date')
  .expect('header', 'content-type', "text/html")
  .expect('bodyContains', 'teletubbies')
});

it ('GET localhost:80/bad_cgi', function () {
  return frisby
  .get(baseUri + 'bad_cgi')
  .expect('status', 500)
});

it ('GET localhost:81', function () {
  return frisby
  .get('http://localhost:81')
  .expect('status', 200)
  .expect('bodyContains', '<?php')
});

it ('GET localhost:82', function () {
  return frisby
  .get('http://localhost:82/')
  .expect('status', 200)
  .expect('bodyContains', 'This project is here to make you write your HTTP server.')
});

it ('GET localhost:82/text', function () {
  return frisby
  .get('http://localhost:82/text')
  .expect('status', 204)
  .expect('bodyContains', 'this is a return directive with text')
});

it ('GET localhost:82/uri', function () {
  return frisby
  .get('http://localhost:82/uri')
  .expect('status', 200)
  .expect('bodyContains', 'Copyright The Closure Library Authors')
});

it ('GET localhost:83', function () {
  return frisby
  .get('http://localhost:83')
  .expect('status', 200)
  .expect('bodyContains', 'index of /')
  .expect('bodyContains', 'Makefile')
});

////////////////////////////////////////////////////////////////////////////
/// POST //////////////////////////////////////////////////////////////////  

it ('POST localhost:80/ with body size > max_body_size', function () {
  return frisby
  .setup({
    request: {
      body: "x".repeat(1025)
    }
  })
  .post(baseUri + 'test.html')
  .expect('status', 413)
});

it ('POST localhost:80/ with body size >> max_body_size', function () {
  return frisby
  .setup({
    request: {
      body: "x".repeat(10250)
    }
  })
  .post(baseUri + 'test.html')
  .expect('status', 413)
});

it ('POST localhost:80/ with body size = max_body_size', function () {
  return frisby
  .setup({
    request: {
      body: "x".repeat(1024)
    }
  })
  .post(baseUri + 'testPostMethod.html')
  .expect('status', 200)
});

//////////////////////////////////////////////////////////////////////////////
/// DELETE //////////////////////////////////////////////////////////////////  

it ('DELETE http://localhost:80/testPostMethod.html', function () {
  return frisby
    .del('http://localhost:80/testPostMethod.html')
    .expect('status', 405);
});

it ('DELETE http://localhost:84/testPostMethod.html', function () {
  return frisby
    .del('http://localhost:84/testPostMethod.html')
    .expect('status', 200);
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
// test not implemented Method
// etc...
      
});
