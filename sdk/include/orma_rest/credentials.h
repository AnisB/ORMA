#pragma once

// Bento includes
#include <bento_memory/common.h>
#include <bento_collection/dynamic_string.h>

namespace orma
{
	struct TCredentials
	{
		ALLOCATOR_BASED;
		TCredentials(bento::IAllocator& allocator)
		: token(allocator)
		{
		}
		bento::DynamicString token;
	};
}