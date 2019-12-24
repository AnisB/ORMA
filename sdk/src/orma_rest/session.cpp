#include "orma_rest/session.h"
#include "orma_rest/curl_api.h"

namespace orma
{
	TSession::TSession(bento::IAllocator& allocator)
	: _allocator(allocator)
	{

	}

	TSession::~TSession()
	{

	}

	void TSession::init(const TCredentials&)
	{
		// Create a curl instance for our communications
		_curlInstance = curl::create_instance();
		// Create a curl message for our communications
		_curlMessage = curl::create_message(_allocator);
	}

	void TSession::ping(bento::DynamicString& message)
	{
		const char* res = curl::request(_curlInstance, "https://api.originsro.org/api/v1/ping", nullptr, 0, nullptr, nullptr, _curlMessage);
		if (res == nullptr)
			message = curl::message_raw_data(_curlMessage);
		else
			message = res;
	}

	void TSession::terminate()
	{
		curl::destroy_instance(_curlInstance);
		curl::destroy_message(_curlMessage);
	}
}
