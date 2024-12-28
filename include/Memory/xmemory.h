#pragma once
#ifndef __XMEMORY__
#define __XMEMORY__

#include "../Core/core.h"
namespace alpha { 
	void* allocate(_SizeType& _Size);
	void* reallocate(void* _Ptr, _SizeType _Size, _SizeType& _NewSize);
	void destroy(void* _Ptr, _SizeType _Size);
}

#endif