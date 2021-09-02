const frisby = require('frisby');

describe('Role api test suite', function() {
	// var baseURL = 'http://docker.local:8088/ajax/users/roles/';
	/**---This case test the api response and check the result----*/
  
	// frisby.create('Get all roles')
	// 	  .get(baseURL + 'roles')
	// 	  .expectStatus(200)
	// 	  .expectHeader('Content-Type', 'application/json')
	// 	  .afterJSON(function (response) {          
	// 		  expect(response.body.method).toBe('roles');
	// 		  expect(response.body.success).toBe(true);          
	// 		  expect(response.errors).toBe(false);
	// 		  expect(response.body.roles).toBeDefined();
	// 	  })     
	// 	  .toss()

	it ('should return a status of 200', function () {
		return frisby
		  .get('http://localhost')
		  .
		  .expect('status', 200);
	  });

  });