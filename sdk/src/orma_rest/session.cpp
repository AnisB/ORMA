// Bento includes
#include <bento_base/hash.h>
#include <bento_base/log.h>

// SDK includes
#include "orma_rest/session.h"
#include "orma_rest/curl_api.h"

// External includes
#include <json.hpp>

namespace orma
{
	TSession::TSession(bento::IAllocator& allocator)
	: _allocator(allocator)
	, _curlInstance(nullptr)
	, _curlMessage(nullptr)
	{

	}

	TSession::~TSession()
	{

	}

	void TSession::init()
	{
		// Create a curl instance for our communications
		_curlInstance = curl::create_instance();
		// Create a curl message for our communications
		_curlMessage = curl::create_message(_allocator);
	}

	void TSession::ping(TPingResponse& pingResponse)
	{
		const char* res = curl::request(_curlInstance, "https://api.originsro.org/api/v1/ping", nullptr, 0, nullptr, nullptr, _curlMessage);
		
		bool success = false;
		if (res == nullptr)
		{
			// Parse the json answer
			auto pingJsonResponse = nlohmann::json::parse(curl::message_raw_data(_curlMessage));
			
			// If message exits and is pong
			if (!pingJsonResponse["message"].is_null() && pingJsonResponse["message"] == "pong")
			{
				pingResponse.validity = true;
				pingResponse.apiVersion = pingJsonResponse["version"].get<uint32_t>();
				pingResponse.timeStamp = pingJsonResponse["generation_timestamp"].get<std::string>().c_str();
				success = true;
			}
		}
		
		if (!success)
		{
			pingResponse.validity = false;
			pingResponse.apiVersion = UINT32_MAX;
			pingResponse.timeStamp = "INVALID";
		}
	}

	bool TOfferComparator(const TOffer& o1, const TOffer& o2)
	{
		return o1.price < o2.price;
	}

	bool TRequestComparator(const TRequest& r1, const TRequest& r2)
	{
		return r1.price > r2.price;
	}

	void TSession::market_list(const TCredentials& credentials, TMarket& market)
	{
		// Create the parameter
		bento::DynamicString auth(_allocator);
		auth += "x-api-key:";
		auth += credentials.token;

		const char* authData[1];
		authData[0] = auth.c_str();
		const char* res = curl::request(_curlInstance, "https://api.originsro.org/api/v1/market/list", authData, 1, nullptr, nullptr, _curlMessage);
		
		bool success = false;
		if (res == nullptr)
		{	
			try
			{
				// Parse the json answer
				auto marketJsonResponse = nlohmann::json::parse(curl::message_raw_data(_curlMessage));

				// Fetch the number of shops
				uint32_t numShops = (uint32_t)marketJsonResponse["shops"].size();
				market.shops.resize(numShops);

				// Fill the shops
				for (uint32_t shopIdx = 0; shopIdx < numShops; ++shopIdx)
				{
					// Fetch the current shop to process
					TShop& currentShop = market.shops[shopIdx];
					auto shopJson = marketJsonResponse["shops"][shopIdx];

					// Fill the shop identification data
					currentShop.name = shopJson["title"].get<std::string>().c_str();
					currentShop.owner = shopJson["owner"].get<std::string>().c_str();
					currentShop.map = shopJson["location"]["map"].get<std::string>().c_str();
					currentShop.locX = shopJson["location"]["x"].get<uint32_t>();
					currentShop.locY = shopJson["location"]["y"].get<uint32_t>();
					currentShop.type = (shopJson["type"] == "V") ? TMerchantType::Seller : TMerchantType::Buyer;

					// Fetch the number of items to process
					uint32_t numArticles = (uint32_t)shopJson["items"].size();
					currentShop.articles.resize(numArticles);
					for (uint32_t articleIdx = 0; articleIdx < numArticles; ++articleIdx)
					{
						// Grab the current json item
						auto itemJson = shopJson["items"][articleIdx];

						// Grab the current element to fill
						TArticle& currentArticle = currentShop.articles[articleIdx];

						// Let's first generate its guid
						uint32_t hashArray[6] = { UINT32_MAX };
						hashArray[0] = itemJson["item_id"].get<uint32_t>();
						if (itemJson["refine"] != nullptr)
							hashArray[1] = itemJson["refine"].get<uint32_t>();
						if (itemJson["cards"] != nullptr)
						{
							if (itemJson["cards"][0] != nullptr)
								hashArray[2] = itemJson["cards"][0].get<uint32_t>();
							if (itemJson["cards"][1] != nullptr)
								hashArray[3] = itemJson["cards"][1].get<uint32_t>();
							if (itemJson["cards"][2] != nullptr)
								hashArray[4] = itemJson["cards"][2].get<uint32_t>();
							if (itemJson["cards"][3] != nullptr)
								hashArray[5] = itemJson["cards"][3].get<uint32_t>();
						}

						// Hash to generate the guid
						uint64_t guid = bento::murmur_hash_64(hashArray, sizeof(hashArray), 666);

						// Fill the current article
						currentArticle.guid = guid;
						currentArticle.price = itemJson["price"].get<uint32_t>();
						currentArticle.amount = itemJson["amount"].get<uint32_t>();

						// Try to fetch the elemnt or initialize it
						TObject* currentObject = nullptr;
						auto itemIt = market.items.find(guid);
						if (itemIt != market.items.end())
						{
							currentObject = &(itemIt->second);
						}
						else
						{
							currentObject = &market.items[guid];
							currentObject->guid = guid;
							currentObject->databaseId = hashArray[0];
							currentObject->refineLevel = hashArray[1];
							currentObject->cards[0] = hashArray[2];
							currentObject->cards[1] = hashArray[3];
							currentObject->cards[2] = hashArray[4];
							currentObject->cards[3] = hashArray[5];
						}
						if (currentShop.type == TMerchantType::Seller)
						{
							TOffer offer;
							offer.sellerId = shopIdx;
							offer.price = currentArticle.price;
							offer.amount = currentArticle.amount;
							currentObject->offers.push_back(offer);
						}
						else
						{
							TRequest request;
							request.buyerId = shopIdx;
							request.price = currentArticle.price;
							request.amount = currentArticle.amount;
							currentObject->requests.push_back(request);
						}
					}
				}

				for (auto& objectIt : market.items)
				{
					TObject& currentItem = objectIt.second;
					std::sort(currentItem.offers.begin(), currentItem.offers.end(), TOfferComparator);
					std::sort(currentItem.requests.begin(), currentItem.requests.end(), TRequestComparator);
				}
			}
			catch (nlohmann::json::exception&)
			{
				bento::ILogger* logger = bento::default_logger();
				logger->log(bento::LogLevel::error, "MARKET", "Request failed");
			}

			// It worked out, perfect.
			success = true;
		}
		
		if (!success)
		{
			market.shops.clear();
		}
	}

	void TSession::database_list(const TCredentials& credentials, TDatabase& database)
	{
		// Create the parameter
		bento::DynamicString auth(_allocator);
		auth += "x-api-key:";
		auth += credentials.token;

		const char* authData[1];
		authData[0] = auth.c_str();
		const char* res = curl::request(_curlInstance, "https://api.originsro.org/api/v1/items/list", authData, 1, nullptr, nullptr, _curlMessage);

		bool success = false;
		if (res == nullptr)
		{
			try
			{
				// Parse the json answer
				auto databaseJsonResponse = nlohmann::json::parse(curl::message_raw_data(_curlMessage));

				// Fetch the number of shops
				uint32_t numItems = (uint32_t)databaseJsonResponse["items"].size();
				database.objects.resize(numItems);

				// Fill the items
				for (uint32_t itemIdx = 0; itemIdx < numItems; ++itemIdx)
				{
					// Fetch the current shop to process
					TDatabaseObject& currentObject = database.objects[itemIdx];
					auto itemJson = databaseJsonResponse["items"][itemIdx];

					// Fill the shop identification data
					currentObject.name = itemJson["name"].get<std::string>().c_str();
					currentObject.databaseId = itemJson["item_id"].get<uint32_t>();

					// Add it to the reference map
					database.objectReference[currentObject.databaseId] = itemIdx;
				}
			}
			catch (nlohmann::json::exception&)
			{
				bento::ILogger* logger = bento::default_logger();
				logger->log(bento::LogLevel::error, "DATABASE", "Request failed");
			}

			// It worked out, perfect.
			success = true;
		}

		if (!success)
		{
			database.objects.clear();
			database.objectReference.clear();
		}
	}

	void TSession::terminate()
	{
		curl::destroy_instance(_curlInstance);
		curl::destroy_message(_curlMessage);
	}
}
