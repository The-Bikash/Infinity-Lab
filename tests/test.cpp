#include "LinearAlgebra/matrix.h"
#include "Number/complex.h"
#include "Number/Int.h"

int main(){

    alpha::UnsignedInt x = "31326491237223472098430294820000000000000000000000000000000000000000074320977249082038432040820420840320193712694";

    x <<= 1000000;

    alpha::print(x);

}