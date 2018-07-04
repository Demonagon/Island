#ifndef UTIL_MATH__H
#define UTIL_MATH__H

/**
* Par Pacôme le 04/07/2018
* Ce fichier est là pour contenir toute fonction mathématique générale que nous
* souhaiterions utiliser et qui n'est pas contenue dans la libraire de math de
* C. Par exemple, des fonctions d'interpolation...
*/

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

#endif
