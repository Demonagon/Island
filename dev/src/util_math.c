#include "util_math.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void randomInitSeed(int seed) {
	printf("Random seed %d\n", seed);
	srand(seed);
}

void randomInit(void) {
	randomInitSeed( time(NULL) );
}

double rand_double_a_b(double a, double b)
{
	return ( rand()/(double)RAND_MAX ) * (b-a) + a;
}

int rand_int_a_b(int a, int b)
{
	return rand()%(b-a+1) +a;
}

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

Complex complexCreate(double a, double b) {
	return (Complex) { .a = a, .b = b };
}

Complex complexCreatePolar(double angle, double norm) {
	return complexCreate( cos(angle) * norm, sin(angle) * norm );
}

Complex complexAdd(Complex a, Complex b) {
	return complexCreate(a.a + b.a, a.b + b.b);
}

Complex complexRemove(Complex a, Complex b) {
	return complexCreate(a.a - b.a, a.b - b.b);
}

Complex complexMultiply(Complex a, Complex b) {
	return complexCreate(a.a * b.a - a.b * b.b, a.b * b.a + a.a * b.b);
}

Complex complexDivide(Complex a, Complex b) {
	double alpha = a.a * b.a + a.b * b.b;
	double beta = a.b * b.a - a.a * b.b;
	double gamma = b.a * b.a + b.b * b.b;
	return complexCreate(alpha / gamma, beta / gamma);
}

Complex complexDot(Complex a, Complex b) {
	return complexCreate(a.a * b.a, a.b * b.b);
}

double complexNorm(Complex c) {
	return sqrt(c.a * c.a + c.b * c.b);
}

Complex complexScale(Complex c, double scale) {
	return complexCreate(c.a * scale, c.b * scale);
}

Complex complexNormalize(Complex c) {
	return complexScale(c, 1 / complexNorm(c) );
}

double complexAngle(Complex c) {
	return ( !c.a && !c.b ) ? 0 : atan2(c.b, c.a);
}

void complexPrint(Complex c) {
	printf("(%3lf, %3lf)", c.a, c.b);
}

char isPointInCircle(Complex point, Complex centre, double radius) {
	return complexNorm( complexRemove(centre, point) ) <= radius;
}

double randomAngle(void) {
	return rand_double_a_b(0, MATH_2_PI);
}

int flipCoin(void) {
	return rand() % 2;
}
