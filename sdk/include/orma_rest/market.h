#pragma once

// Bento includes
#include <bento_memory/common.h>
#include <bento_collection/dynamic_string.h>

// External includes
#include <vector>
#include <map>

namespace orma
{
	struct TOffer
	{
		uint32_t sellerId;
		uint32_t price;
		uint32_t amount;
	};

	struct TRequest
	{
		uint32_t buyerId;
		uint32_t price;
		uint32_t amount;
	};

	struct TObject
	{
		uint64_t guid;
		uint32_t databaseId;
		uint32_t refineLevel;
		uint32_t cards[4];
		std::vector<TOffer> offers;
		std::vector<TRequest> requests;
	};

	struct TArticle
	{
		uint64_t guid;
		uint32_t price;
		uint32_t amount;
	};

	enum class TMerchantType
	{
		Seller,
		Buyer,
		Invalid
	};

	struct TShop
	{
		ALLOCATOR_BASED;
		TShop(bento::IAllocator& allocator)
		: name(allocator)
		, owner (allocator)
		, map(allocator)
		, locX(UINT32_MAX)
		, locY(UINT32_MAX)
		, type(TMerchantType::Invalid)
		, articles(allocator)
		{
		}

		bento::DynamicString name;
		bento::DynamicString owner;
		bento::DynamicString map;
		uint32_t locX;
		uint32_t locY;
		TMerchantType type;
		bento::Vector<TArticle> articles;
	};

	struct TMarket
	{
		ALLOCATOR_BASED;
		TMarket(bento::IAllocator& allocator)
		: shops(allocator)
		{
		}
		bento::Vector<TShop> shops;
		std::map<uint64_t, TObject> items;
	};
}