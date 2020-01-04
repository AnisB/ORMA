#include "orma_rest/database.h"

// Bento includes
#include <bento_base/stream.h>

namespace orma
{
	void build_database_index(const TDatabase& database, TDatabaseIndex& outIndex)
	{
		// Fill the items
		uint32_t numItems = database.objects.size();
		for (uint32_t itemIdx = 0; itemIdx < numItems; ++itemIdx)
		{
			// Fetch the current shop to process
			const TDatabaseObject& currentObject = database.objects[itemIdx];

			// Add it to the reference map
			outIndex.objectReference[currentObject.databaseId] = itemIdx;
		}
	}
}

namespace bento
{
	const uint32_t DATABASE_OBJECT_SOURCE_VERSION = 1;
	void pack_type(bento::Vector<char>& buffer, const orma::TDatabaseObject& databaseObject)
	{
		bento::pack_bytes(buffer, DATABASE_OBJECT_SOURCE_VERSION);
		bento::pack_bytes(buffer, databaseObject.databaseId);
		bento::pack_type(buffer, databaseObject.name);
		bento::pack_bytes(buffer, databaseObject.npcPrice);
	}

	bool unpack_type(const char*& stream, orma::TDatabaseObject& databaseObject)
	{
		// Read the version
		uint32_t data_stream_version;
		bento::unpack_bytes(stream, data_stream_version);

		// Stop if this does not match the current version
		if (data_stream_version != bento::DATABASE_OBJECT_SOURCE_VERSION) return false;
		bento::unpack_bytes(stream, databaseObject.databaseId);
		bento::unpack_type(stream, databaseObject.name);
		bento::unpack_bytes(stream, databaseObject.npcPrice);
		return true;
	}

	const uint32_t DATABASE_SOURCE_VERSION = 1;
	void pack_type(bento::Vector<char>& buffer, const orma::TDatabase& database)
	{
		bento::pack_bytes(buffer, DATABASE_SOURCE_VERSION);
		bento::pack_vector_types(buffer, database.objects);
	}

	bool unpack_type(const char*& stream, orma::TDatabase& database)
	{
		// Read the version
		uint32_t data_stream_version;
		bento::unpack_bytes(stream, data_stream_version);

		// Stop if this does not match the current version
		if (data_stream_version != bento::DATABASE_SOURCE_VERSION) return false;
		bento::unpack_vector_types(stream, database.objects);
		return true;
	}
}