#pragma once

// Bento includes
#include <bento_memory/common.h>

// SDK includes
#include "orma_rest/credentials.h"
#include "orma_rest/curl_api.h"

namespace orma
{
	class TSession
	{
		ALLOCATOR_BASED;
	public:
		// Cst & Dst
		TSession(bento::IAllocator& allocator);
		~TSession();

		void init(const TCredentials& credentials);
		void terminate();

		// Interactions
		void ping(bento::DynamicString& str);

	private:
		curl::CurlInstance* _curlInstance;
		curl::CurlMessage* _curlMessage;
	public:
		bento::IAllocator& _allocator;
	};
}