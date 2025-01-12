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
    alpha::UnsignedInt y = "294273439247262983294629374924693274297973495395739475349753459347593493497395380379308305307309530733980373073073053053053470534753405834053907";
    alpha::print(x, "\n");

    x <<= 10000;
    y = x - y;

    for(int i = 0; i < 10000000; ++i) {
        x -= y;
        x += y;
    }
    x >>= 10000;

    alpha::print(x);
}