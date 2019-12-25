#pragma once

// Bento includes
#include <bento_memory/common.h>

// SDK includes
#include "orma_rest/credentials.h"
#include "orma_rest/curl_api.h"
#include "orma_rest/ping_response.h"
#include "orma_rest/market.h"

namespace orma
{
	class TSession
	{
		ALLOCATOR_BASED;
	public:
		// Cst & Dst
		TSession(bento::IAllocator& allocator);
		~TSession();

		void init();
		void terminate();

		// Interactions
		void ping(TPingResponse& pingResponse);
		void market_list(const TCredentials& credentials, TMarket& market);

	private:
		curl::CurlInstance* _curlInstance;
		curl::CurlMessage* _curlMessage;
	public:
		bento::IAllocator& _allocator;
	};
}