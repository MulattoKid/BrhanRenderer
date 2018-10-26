#include "MemoryPool.h"

MemoryPool::MemoryPool(const size_t bytes, const size_t n_threads)
{
	num_threads = n_threads;
	pool_size = bytes;
	const size_t num_chars_needed = (bytes + 8 - 1) / 8; //8 bits fit in one char -> 8 bools in one char

	pools = new char*[num_threads];
	availibilities = new unsigned char*[num_threads];
	search_starts = new size_t[num_threads];
	for (size_t t = 0; t < num_threads; t++)
	{
		pools[t] = new char[bytes];

		availibilities[t] = new unsigned char[num_chars_needed];
		for (size_t i = 0; i < num_chars_needed; i++)
		{
			availibilities[t][i] = 0b11111111;
		}
		//Set the needed bits in the last byte (char)
		const size_t num_exceeding_bits = bytes - ((bytes / 8) * 8);
		for (size_t i = 0; i < num_exceeding_bits; i++)
		{
			unsigned char mask = 0b00000001 << (num_exceeding_bits - i - 1);
			availibilities[t][num_chars_needed - 1] &= mask;
		}
		
		search_starts[t] = 0;
	}
}

MemoryPool::~MemoryPool()
{
	for (size_t t = 0; t < num_threads; t++)
	{
		delete[] availibilities[t];
		delete[] pools[t];
	}
	delete[] search_starts;
	delete[] availibilities;
	delete[] pools;
}

void* MemoryPool::Allocate(const size_t obj_size, const size_t thread_num)
{
	size_t offset = search_starts[thread_num];
	bool range_is_available = false;
	bool passed_available = false;
	while (!range_is_available && offset <= pool_size)
	{
		size_t availibilities_location = offset / 8;
		size_t availibilities_bit = offset % 8;
		unsigned char mask = 0b00000001 << availibilities_bit;
		while (!(availibilities[thread_num][availibilities_location] & mask) && offset <= pool_size)
		{
			offset++;
			availibilities_location = offset / 8;
			availibilities_bit = offset % 8;
			mask = 0b00000001 << availibilities_bit;
		}

		range_is_available = true;
		for (size_t i = 1; i < obj_size; i++)
		{
			availibilities_location = (offset + i) / 8;
			availibilities_bit = (offset + 1) % 8;
			mask = 0b00000001 << availibilities_bit;
			if (offset + i >= pool_size || !(availibilities[thread_num][availibilities_location] & mask))
			{
				range_is_available = false;
				passed_available = true;
				offset += obj_size;
				break;
			}
		}
	}

	if (!range_is_available)
	{
		return NULL;
	}

	if (!passed_available || offset == 0)
	{
		search_starts[thread_num] = offset + obj_size;
	}

	size_t availibilities_location = 0;
	size_t availibilities_bit = 0;
	unsigned char mask = 0;
	for (size_t i = 0; i < obj_size; i++)
	{
		availibilities_location = (offset + i) / 8;
		availibilities_bit = (offset + i) % 8;
		mask = 0b00000001 << availibilities_bit;
		mask ^= 0b11111111; //XOR
		availibilities[thread_num][availibilities_location] &= mask;
	}

	return (void*)(pools[thread_num] + offset);
}

void* MemoryPool::Free(const void* obj_ptr, const size_t obj_size, const size_t thread_num)
{
	size_t offset = (size_t)((char*)(obj_ptr) - pools[thread_num]);
	size_t availibilities_location = 0;
	size_t availibilities_bit = 0;
	unsigned char mask = 0;
	for (size_t i = 0; i < obj_size; i++)
	{
		availibilities_location = (offset + i) / 8;
		availibilities_bit = (offset + i) % 8;
		mask = 0b00000001 << availibilities_bit;
		availibilities[thread_num][availibilities_location] |= mask;
	}

	if (offset < search_starts[thread_num])
	{
		search_starts[thread_num] = offset;
	}

	return NULL;
}

void MemoryPool::Free(const size_t thread_num)
{
	for (size_t i = 0; i < pool_size / 8; i++)
	{
		availibilities[thread_num][i] = 0b11111111;
	}

	search_starts[thread_num] = 0;
}

void MemoryPool::Free()
{
	for (size_t t = 0; t < num_threads; t++)
	{
		for (size_t i = 0; i < pool_size / 8; i++)
		{
			availibilities[t][i] = 0b11111111;
		}

		search_starts[t] = 0;
	}
}