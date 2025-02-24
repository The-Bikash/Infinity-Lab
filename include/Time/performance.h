#pragma once
#ifndef __ALPHA_TIME__
#define __ALPHA_TIME__

#include <ctime>
#include <cstdio>

auto start = clock();
auto end = clock();
double elapsed_time;
int result; 

inline auto current_time() {
	return clock();
}

inline void time_start() {
	start = clock();
}

inline void time_end() {
	end = clock();
	elapsed_time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
	printf("Time Taken %f. miliseconds\n", elapsed_time);
	/*elapsed_time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
	printf("Time Taken %f. miliseconds\n", elapsed_time);*/
}

#endif



