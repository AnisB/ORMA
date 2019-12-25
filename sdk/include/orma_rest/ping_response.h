#pragma once

// Bento includes
#include <bento_memory/common.h>
#include <bento_collection/dynamic_string.h>

namespace orma
{
	struct TPingResponse
	{
		ALLOCATOR_BASED;
		TPingResponse(bento::IAllocator& allocator)
		: validity(false)
		, apiVersion(0)
		, timeStamp(allocator, "INVALID")
		{
		}
		bool validity;
		uint32_t apiVersion;
		bento::DynamicString timeStamp;
	};
}