#include "Number/complex.h"
#include "Number/Int.h"

#include <cstdio>
#include <ctime>

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


int main(){

    alpha::UnsignedInt x = "12823643286483264d9237432974991257392152642861023482749264891326589632896498263538642986502023658365926393275934574236583296598326392592394293563256234632532685345324";
    alpha::print(x, "\n");
    time_start();
    x <<= 10000000000;
    time_end();
    time_start();
    x >>= 10000000000;
    time_end();
    time_start();
    x <<= 10000000000;
    time_end();
    time_start();
    x >>= 10000000000;
    time_end();
    alpha::print(x, "\n");

}