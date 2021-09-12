const frisby = require('frisby');

// install node.js
// npm install --save-dev frisby
// npm install --save-dev jest
// run tests:
//    jest
// Or use Jest Orta extention from vscode (orta.vscode-jest)

describe('Role api test suite', function() {

  var baseUri = 'http://localhost:80/';

/////////////////////////////////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////  
// GET

// FileReader(""); read index.php and compare content to see if it match response body 

it ('GET should return a status of 200', function () {
  return frisby
  .get(baseUri + 'index.php')
  .expect('status', 200)
  .expect('header', 'content-length', '582')
  .expect('header', 'date')
  .expect('bodyContains', 'Webserv - home')
  // .inspectBody()
});

// 262144 max char macos (getconf ARG_MAX) 
var max_len_uri = 8000; // choose our random max len uri 7980
it ('GET with max uri len should return 404 not found', function () {
  return frisby
  .get(baseUri + "x".repeat(max_len_uri- baseUri.length + 3))
  .expect('status', 404)
});

it ('GET with max uri len + 1 should return 414 URI Too Long', function () {
  return frisby
  .get(baseUri + "x".repeat(max_len_uri - baseUri.length + 4))
  .expect('status', 414)
});

// test on folders, (autoindex or not, index or not (so 403 or 404 ?))
// test bad http protocol version
// test not implemented Method
// etc...


/////////////////////////////////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////  
// POST

// var max_body_size = 1000 * 1000;
// it ('POST: with content less than max body size should return 201 Success Created', function () {
//   return frisby
//   .setup({
//     request: {
//       body: "x".repeat(max_body_size)
//     }
//   })
//   .post(baseUri + 'index.php')
//   .expect('status', 201)
// });

// it ('POST: should return 413 Request Entity Too Large', function () {
//   return frisby
//   .setup({
//     request: {
//       body: "x".repeat(max_body_size + 1)
//     }
//   })
//   .post(baseUri + 'index.php')
//   .expect('status', 413)

// });


// it ('POST should return a status of 201 Created', function () {
  // return frisby
	// 	.post('http://localhost', {
    // 	title: 'My New Blog Post',
    // 	content: '<p>A cool blog post!</p>'
    // 	})
    // 	.expect('status', 201);
    // });

    

/////////////////////////////////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////  
// DELETE
    
    // it ('DELETE should return a status of 204 No Content', function () {
      // 	return frisby
      // 	  .del('http://localhost')
      // 	  .expect('status', 204);
      // });
      
      
      
});