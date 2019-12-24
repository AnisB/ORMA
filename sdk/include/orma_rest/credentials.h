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
		: id (allocator)
		, password (allocator)
		{
		}
		bento::DynamicString id;
		bento::DynamicString password;
	};
}