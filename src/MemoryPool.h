#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <stddef.h>

struct MemoryPool
{
	size_t num_threads;
	size_t pool_size;
	char** pools;
	unsigned char** availibilities;
	size_t* search_starts; //First empty chunk where the search for the next one starts

	MemoryPool(const size_t bytes, const size_t n_threads = 1);
	~MemoryPool();
	void* Allocate(const size_t obj_size, const size_t thread_num = 0);
	void* Free(const void* obj_ptr, const size_t obj_size, const size_t thread_num = 0);
	void  Free(const size_t thread_num);
	void  Free();
};

#endif