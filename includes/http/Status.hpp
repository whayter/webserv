#ifndef STATUS_HPP
#define STATUS_HPP

#include "http.hpp"

#include <string>

namespace http {

struct Status {

public:

	enum StatusEnum
	{
		None							= 0, 
		Continue						= 100, 
		SwitchingProtocols				= 101, 
		Processing						= 102, 
		EarlyHints						= 103, 
		OK								= 200, 
		Created							= 201, 
		Accepted						= 202, 
		NonAuthoritativeInformation		= 203, 
		NoContent						= 204, 
		ResetContent					= 205, 
		PartialContent					= 206, 
		MultiStatus						= 207, 
		AlreadyReported					= 208, 
		IMUsed							= 226, 
		MultipleChoices					= 300, 
		MovedPermanently				= 301, 
		Found							= 302, 
		SeeOther						= 303, 
		NotModified						= 304, 
		UseProxy						= 305, 						
		TemporaryRedirect				= 307, 
		PermanentRedirect				= 308, 
		BadRequest						= 400, 
		Unauthorized					= 401, 
		PaymentRequired					= 402, 
		Forbidden						= 403, 
		NotFound						= 404, 
		MethodNotAllowed				= 405, 
		NotAcceptable					= 406, 
		ProxyAuthenticationRequired		= 407, 
		RequestTimeout					= 408, 
		Conflict						= 409, 
		Gone							= 410, 
		LengthRequired					= 411, 
		PreconditionFailed				= 412, 
		PayloadTooLarge					= 413, 
		URITooLong						= 414, 
		UnsupportedMediaType			= 415, 
		RangeNotSatisfiable				= 416, 
		ExpectationFailed				= 417, 
		ImATeapot						= 418, 
		UnprocessableEntity				= 422, 
		Locked							= 423, 
		FailedDependency				= 424, 
		UpgradeRequired					= 426, 
		PreconditionRequired			= 428, 
		TooManyRequests					= 429, 
		RequestHeaderFieldsTooLarge		= 431, 
		UnavailableForLegalReasons		= 451, 
		InternalServerError				= 500, 
		NotImplemented					= 501, 
		BadGateway						= 502, 
		ServiceUnavailable				= 503, 
		GatewayTimeout					= 504, 
		HTTPVersionNotSupported			= 505, 
		VariantAlsoNegotiates			= 506, 
		InsufficientStorage				= 507, 
		LoopDetected					= 508, 
		NotExtended						= 510, 
		NetworkAuthenticationRequired	= 511,

		EndOfInput	= 999 // look away from here
	};

	Status();
	Status(StatusEnum e);
	Status(unsigned int e);
	~Status();

	void setValue(StatusEnum e);
	void setValue(int e);

	int	getValue();

	std::string getDefinition();
	
	friend bool	operator==(const Status& lhs, const Status& rhs)
	{
		return lhs._e == rhs._e;
	}
	friend bool operator!=(const Status& lhs, const Status& rhs)
	{
		return lhs._e != rhs._e;
	}
	friend bool operator>=(const Status& lhs, const Status& rhs)
	{
		return lhs._e >= rhs._e;
	}
	friend bool operator<=(const Status& lhs, const Status& rhs)
	{
		return lhs._e <= rhs._e;
	}
	friend bool operator<(const Status& lhs, const Status& rhs)
	{
		return lhs._e < rhs._e;
	}
	friend bool operator>(const Status& lhs, const Status& rhs)
	{
		return lhs._e > rhs._e;
	}

private:

	StatusEnum _e;

}; /* struct Status */

bool		isInformational(Status statusCode);
bool		isSuccessful(Status statusCode);
bool 		isRedirection(Status statusCode);
bool 		isClientError(Status statusCode);
bool 		isServerError(Status statusCode);
bool 		isError(Status statusCode);

} /* namespace http */

#endif /* STATUS_HPP */
