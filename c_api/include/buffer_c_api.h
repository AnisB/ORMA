#pragma once

// Internal includes
#include "types_c_api.h"

// External includes
#include <stdint.h>

extern "C"
{
	// Create a buffer with a given size and value
	C_API_EXPORT OrmaBufferObject* orma_create_buffer(OrmaAllocatorObject* alloc, uint32_t size, unsigned char* input_buffer);

	// Get the size of the buffer object
	C_API_EXPORT uint32_t orma_buffer_size(OrmaBufferObject* buffer);

	// Copy the buffer into a pointer
	C_API_EXPORT void orma_buffer_get_data(OrmaBufferObject* buffer, unsigned char* output_buffer);

	// Destroy a previously created buffer
	C_API_EXPORT void orma_destroy_buffer(OrmaBufferObject* buffer);
}