// SDK includes
#include <bento_memory/system_allocator.h>
#include <bento_base/log.h>
#include <bento_resources/asset_database.h>
#include <bento_tools/disk_serializer.h>

#include <orma_rest/session.h>

// External includes
#include <string>

namespace orma
{
	const char* database_file_extension = "adb";
	const char* database_debug_file_extension = "json";

	bool write_database(const char* target_path, const bento::TAssetDatabase& target_database)
	{
		// Get the file's directory and filename
		const bento::DynamicString& db_directory = bento::path::directory(target_path, *bento::common_allocator());
		const bento::DynamicString& filename = bento::path::filename(target_path, *bento::common_allocator());

		// Create the serializer
		bento::DiskSerializer serializer(*bento::common_allocator(), db_directory.c_str());

		// Write the file
		bool db_written = serializer.write_binary(target_database, filename.c_str(), database_file_extension);

		// return the result
		return db_written;
	}

	bool write_database_debug(const char* target_path, const bento::TAssetDatabase& target_database)
	{
		const bento::DynamicString& db_directory = bento::path::directory(target_path, *bento::common_allocator());
		const bento::DynamicString& filename = bento::path::filename(target_path, *bento::common_allocator());

		bento::DiskSerializer serializer(*bento::common_allocator(), db_directory.c_str());

		// Write the file
		bool db_debug_written = serializer.write_string(target_database, filename.c_str(), database_debug_file_extension);

		// return the result
		return db_debug_written;
	}
}

int main(int argc, char** argv)
{
	// Check that the id is there
	if (argc != 2)
		return -1;

	// Create an allocator for our application
	bento::SystemAllocator systemAllocator;

	// Create a session
	orma::TSession session(systemAllocator);

	// Init the session
	session.init();

	// Declare our credentials
	orma::TCredentials credentials(systemAllocator);
	credentials.token = argv[1];

	// Grab the market's content
	orma::TDatabase database(systemAllocator);
	bool requestResult = session.database_list(credentials, database);

	// Leave right away if we failed
	if (!requestResult)
	{
		bento::ILogger* logger = bento::default_logger();
		logger->log(bento::LogLevel::info, "FAILTURE", "Database request failed");
		session.terminate();
		return -1;
	}

	// Terminate the session
	session.terminate();

	// Now we need to serialized the database to an asset database
	bento::TAssetDatabase assetDatabase(systemAllocator);
	bento::Vector<char> data(systemAllocator);
	bento::pack_type(data, database);
	assetDatabase.insert_asset("ragnarok_database.database", ".", 0, data);

	// Write the database and the debug file to the target_file
	bento::DynamicString output_database_path(*bento::common_allocator());
	output_database_path = "C:/Temp/asset_database";
	orma::write_database(output_database_path.c_str(), assetDatabase);
	orma::write_database_debug(output_database_path.c_str(), assetDatabase);

	// We are done!
	return 0;
}