#ifndef UTIL_MATH__H
#define UTIL_MATH__H

#define MATH_PI   3.14159265359
#define MATH_2_PI 6.28318530718
#define MATH_PI_2 9.86960440109

#define SQRT_2 1.41421356237

/*Modifié par Mathieu le 22/07/2018
Ajout de fonction de calculs utilisés dans la heightmap
*/

/**
* Par Pacôme le 04/07/2018
* Ce fichier est là pour contenir toute fonction mathématique générale que nous
* souhaiterions utiliser et qui n'est pas contenue dans la libraire de math de
* C. Par exemple, des fonctions d'interpolation...
* Les angles sont en radians.
*/

// renvoie un rand double entre a et b inclus
double rand_double_a_b(double a, double b);


// renvoie un rand entier entre a et b inclus
int rand_int_a_b(int a, int b);

/**
* Prend une entrée sur [0, 1] et renvoie une valeur sur [0, 1]. Le plot de
* cette fonction ralentit à l'approche de 0 et de 1 ; sert beaucoup pour les
* animations.
* f(x) = 6x^5 - 15x^4 + 10x^3
*/

double smootherInterpolation(double x);

/**
* f(x) = x^2
*/

double accelerationInterpolation(double x);

/**
* f(x) = 1 - (1-x)^2
*/

double decelerationInterpolation(double x);

/**
* Nombre complexe pour toute manipulation de coordonnées dans le plan.
*/

typedef struct ComplexNumber {
	double a;
	double b;
} ComplexNumber;

/**
* Petit raccourcis pour raccourcir le nom
*/

typedef ComplexNumber Complex;

Complex complexCreate(double a, double b);
Complex complexCreatePolar(double angle, double norm);
Complex complexAdd(Complex a, Complex b);
Complex complexRemove(Complex a, Complex b);
Complex complexMultiply(Complex a, Complex b);
Complex complexDivide(Complex a, Complex b);
Complex complexDot(Complex a, Complex b);
double  complexNorm(Complex c);
Complex complexScale(Complex c, double scale);
Complex complexNormalize(Complex c);
double  complexAngle(Complex c);
void complexPrint(Complex c);

char isPointInCircle(Complex point, Complex centre, double radius);

#endif
