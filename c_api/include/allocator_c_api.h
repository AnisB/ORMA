#pragma once

#include "types_c_api.h"

extern "C"
{
	// Function to create a new orma allocator
	C_API_EXPORT OrmaAllocatorObject* orma_create_allocator();

	// Function to destroy a orma allocator
	C_API_EXPORT void orma_destroy_allocator(OrmaAllocatorObject* allocator);
}