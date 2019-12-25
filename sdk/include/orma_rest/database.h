#pragma once

// Bento includes
#include <bento_memory/common.h>
#include <bento_collection/dynamic_string.h>

// External includes
#include <vector>
#include <map>

namespace orma
{
	struct TDatabaseObject
	{
		ALLOCATOR_BASED;
		TDatabaseObject(bento::IAllocator& allocator)
		: name(allocator)
		, databaseId(UINT32_MAX)
		{
		}

		uint32_t databaseId;
		bento::DynamicString name;
	};

	struct TDatabase
	{
		ALLOCATOR_BASED;
		TDatabase(bento::IAllocator& allocator)
		: objects(allocator)
		{
		}
		bento::Vector<TDatabaseObject> objects;
		std::map<uint32_t, uint32_t> objectReference;
	};
}