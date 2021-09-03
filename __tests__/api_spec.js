const frisby = require('frisby');

describe('Role api test suite', function() {

	// GET

	FileReader("");

	it ('GET should return a status of 200', function () {
		return frisby
		  .get('http://localhost')
		  .expect('status', 200);
	  });
	 

	// POST

	// it ('POST should return a status of 201 Created', function () {
	// return frisby
	// 	.post('http://localhost', {
	// 	title: 'My New Blog Post',
	// 	content: '<p>A cool blog post!</p>'
	// 	})
	// 	.expect('status', 201);
	// });


	// DELETE

	// it ('DELETE should return a status of 204 No Content', function () {
	// 	return frisby
	// 	  .del('http://localhost')
	// 	  .expect('status', 204);
	// });



});