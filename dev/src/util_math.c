#include "util_math.h"
#include <math.h>

double smootherInterpolation(double x) {
	if( x <= 0 ) return 0;
	if( x >= 1 ) return 1;
	return 6 * pow(x, 5) - 15 * pow(x, 4) + 10 * pow(x, 3);
}

double accelerationInterpolation(double x) {
	if( x <= 0 ) return 0;
	if( x >= 1 ) return 1;
	return x*x;
}

double decelerationInterpolation(double x) {
	if( x <= 0 ) return 0;
	if( x >= 1 ) return 1;
	return 2*x - x*x;
}
