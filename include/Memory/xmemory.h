#pragma once
#ifndef __XMEMORY__
#define __XMEMORY__

namespace alpha { 
	void* allocate(size_t& _Size);
	void* reallocate(void* _Ptr, size_t _Size, size_t& _NewSize);
	void destroy(void* _Ptr, size_t _Size);
}

#endif