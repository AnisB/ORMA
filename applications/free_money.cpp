// SDK includes
#include <bento_memory/system_allocator.h>
#include <bento_base/log.h>
#include <orma_rest/session.h>

// External includes
#include <string>

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

	// Do a ping request
	orma::TMarket market(systemAllocator);
	session.market_list(credentials, market);
	
	// Log the result
	bento::ILogger* logger = bento::default_logger();
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

	// Terminate the session
	session.terminate();

	// We are done!
	return 0;
}