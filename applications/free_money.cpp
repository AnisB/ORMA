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

	bool read_database(const char* target_path, bento::TAssetDatabase& output_database)
	{
		// Get the file's directory and filename
		const bento::DynamicString& db_directory = bento::path::directory(target_path, *bento::common_allocator());
		const bento::DynamicString& filename = bento::path::filename(target_path, *bento::common_allocator());

		// Create the serializer
		bento::DiskSerializer serializer(*bento::common_allocator(), db_directory.c_str());

		// Read the file
		bool db_read = serializer.read_binary(output_database, filename.c_str(), database_file_extension);

		// return the result
		return db_read;
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

	// Define the path of the database
	bento::DynamicString databseLocation(systemAllocator);
	databseLocation = "C:/Temp/asset_database.adb";

	// Deserialize the database
	bento::TAssetDatabase assetDatabase(systemAllocator);
	orma::read_database(databseLocation.c_str(), assetDatabase);

	// Declare the database we need to fill
	orma::TDatabase database(systemAllocator);
	assetDatabase.unpack_asset_to_type("ragnarok_database.database", database);

	// Build the index for faster access
	orma::TDatabaseIndex databaseIndex;
	orma::build_database_index(database, databaseIndex);

	// Grab the market's content
	orma::TMarket market(systemAllocator);
	bool requestResult = session.market_list(credentials, market);

	// Leave right away if we failed
	if (!requestResult)
	{
		bento::ILogger* logger = bento::default_logger();
		logger->log(bento::LogLevel::info, "FAILTURE", "Market request failed");
		session.terminate();
		return - 1;

	}
	// Log the result
	bento::ILogger* logger = bento::default_logger();

	logger->log(bento::LogLevel::info, "SELLER < BUYER", "");
	logger->new_line();
	for (auto& itemIT : market.items)
	{
		// Fetch the current shop
		const orma::TObject& currentItem = itemIT.second;

		// Is the item sold and wanted?
		if (currentItem.offers.size() != 0 && currentItem.requests.size() != 0)
		{
			// Fill the shop identification data
			if (currentItem.offers[0].price < currentItem.requests[0].price)
			{
				const auto& iterator = databaseIndex.objectReference.find(currentItem.databaseId);
				if (iterator != databaseIndex.objectReference.end())
				{
					uint32_t objectDatabaseIndex = iterator->second;
					orma::TDatabaseObject& currentObject = database.objects[objectDatabaseIndex];
					logger->log(bento::LogLevel::info, "ITEM_DATABASE_NAME", currentObject.name.c_str());
				}
				else
				{
					logger->log(bento::LogLevel::info, "ITEM_DATABASE_NAME", "UNKOWN");
				}

				logger->log(bento::LogLevel::info, "ITEM_DATABASE_ID", std::to_string(currentItem.databaseId).c_str());
				uint32_t priceDifference = currentItem.requests[0].price - currentItem.offers[0].price;
				uint32_t minItems = min(currentItem.requests[0].amount, currentItem.offers[0].amount);
				logger->log(bento::LogLevel::info, "COST", std::to_string(currentItem.offers[0].price * minItems).c_str());
				logger->log(bento::LogLevel::info, "BONUS_PER_ITEM", std::to_string(priceDifference).c_str());
				logger->log(bento::LogLevel::info, "BONUS", std::to_string(priceDifference * minItems).c_str());
				logger->new_line();
			}
		}
	}

	logger->log(bento::LogLevel::info, "SELLER < NPC PRICE MAJORATED", "");
	logger->new_line();
	for (auto& itemIT : market.items)
	{
		// Fetch the current shop
		const orma::TObject& currentItem = itemIT.second;

		// Is the item sold and wanted?
		if (currentItem.offers.size() != 0)
		{
			const auto& iterator = databaseIndex.objectReference.find(currentItem.databaseId);
			if (iterator != databaseIndex.objectReference.end())
			{
				uint32_t objectDatabaseIndex = iterator->second;
				orma::TDatabaseObject& currentObject = database.objects[objectDatabaseIndex];

				uint32_t majoratedPrice = (uint32_t)(currentObject.npcPrice * 1.24f * 0.5f);

				// Fetch the number of sellers
				uint32_t numOffers = (uint32_t)currentItem.offers.size();

				for (uint32_t offerIdx = 0; offerIdx < numOffers; ++offerIdx)
				{
					// Fill the shop identification data
					if (currentItem.offers[offerIdx].price < majoratedPrice)
					{
						const orma::TShop& currentShop = market.shops[currentItem.offers[offerIdx].sellerId];

						uint32_t priceDifference = majoratedPrice - currentItem.offers[offerIdx].price;
						uint32_t minItems = currentItem.offers[offerIdx].amount;

						logger->log(bento::LogLevel::info, "ITEM_DATABASE_NAME", currentObject.name.c_str());
						logger->log(bento::LogLevel::info, "ITEM_DATABASE_ID", std::to_string(currentItem.databaseId).c_str());
						logger->log(bento::LogLevel::info, "COST", std::to_string(currentItem.offers[offerIdx].price * minItems).c_str());
						logger->log(bento::LogLevel::info, "MAP", currentShop.map.c_str());
						logger->log(bento::LogLevel::info, "VENDOR_PRICE", std::to_string(currentItem.offers[offerIdx].price).c_str());
						logger->log(bento::LogLevel::info, "MAJORATED_PRICE", std::to_string(majoratedPrice).c_str());
						logger->log(bento::LogLevel::info, "BONUS_PER_ITEM", std::to_string(priceDifference).c_str());
						logger->log(bento::LogLevel::info, "BONUS", std::to_string(priceDifference * minItems).c_str());

						logger->new_line();
					}
					else
					{
						break;
					}
				}
			}
		}
	}


	// Terminate the session
	session.terminate();

	// We are done!
	return 0;
}