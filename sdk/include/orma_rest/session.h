#pragma once

// Bento includes
#include <bento_memory/common.h>

// SDK includes
#include "orma_rest/credentials.h"

namespace orma
{
	class TSession
	{
		ALLOCATOR_BASED;
	public:
		// Cst & Dst
		TSession();
		~TSession();

		// Identification & drop
		void connect(const TCredentials& credentials);
		void disconnect();
	};
}