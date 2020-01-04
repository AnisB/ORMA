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
		uint32_t npcPrice;
	};

	struct TDatabase
	{
		ALLOCATOR_BASED;
		TDatabase(bento::IAllocator& allocator)
		: objects(allocator)
		{
		}
		bento::Vector<TDatabaseObject> objects;
	};

	struct TDatabaseIndex
	{
		std::map<uint32_t, uint32_t> objectReference;
	};

	void build_database_index(const TDatabase& database, TDatabaseIndex& outIndex);
}

namespace bento
{
	void pack_type(bento::Vector<char>& buffer, const orma::TDatabaseObject& databaseObject);
	bool unpack_type(const char*& stream, orma::TDatabaseObject& databaseObject);
	void pack_type(bento::Vector<char>& buffer, const orma::TDatabase& database);
	bool unpack_type(const char*& stream, orma::TDatabase& database);
}