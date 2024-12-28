#pragma once 
#ifndef __CORE__
#define __CORE__


constexpr const unsigned int uint32_max = -1;
constexpr const unsigned long long uint64_max = -1;

#if _DEBUG
	constexpr auto const _debug = true;
#else
	constexpr auto const _debug = false;
#endif
using _SizeType = unsigned long long;
#endif