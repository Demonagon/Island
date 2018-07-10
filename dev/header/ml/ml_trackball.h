/********************************************************************************
* ___  ___        _    _             _      _  _                                *
* |  \/  |       | |  | |           | |    (_)| |                               *
* | .  . |  __ _ | |_ | |__   ___   | |     _ | |__   _ __   __ _  _ __  _   _  *
* | |\/| | / _` || __|| '_ \ / __|  | |    | || '_ \ | '__| / _` || '__|| | | | *
* | |  | || (_| || |_ | | | |\__ \  | |____| || |_) || |   | (_| || |   | |_| | *
* \_|  |_/ \__,_| \__||_| |_||___/  \_____/|_||_.__/ |_|    \__,_||_|    \__, | *
*                                                                        __/ |  *
*  Copyright (c) 2010                                                   |___/   *
*  Author: Dimitri KUDELSKI                                                     *
*  Contact: dimitri [dot] kudelski [at] gmail [dot] com                         *
*                                                                               *
*                                                                               *
* "THE (extended) BEER-WARE LICENSE" (Revision 42.0815):                        *
* As long as you retain this notice you can do whatever you want with this      *
* stuff. If we meet some day, and you think this stuff is worth it , you can    *
* buy me some beers in return.                                                  *
*                                                                               *
*********************************************************************************/    


#ifndef __ML_TRACKBALL__
#define __ML_TRACKBALL__


#include <ml/ml_define.h>


void 		mlTbInit(int _w, int _h);													/* Initialize the trackball with the dimensions of the current window */

void 		mlTbClick	(int _x, int _y);												/* Record a simple mouse click event at the point (x, y) */
void 		mlTbMotion	(int _x, int _y);												/* Rotate the object according to the new coordinates (x, y) */
void 		mlTbRelease	(int _x, int _y);												/* Record a simple mouse release event at the point (x, y) */
void 		mlTbZoom	(double _zoom);													/* Just a zoom... */

double * 	mlTbGetTransformation();													/* Return a pointer to the transformation matrix (to send to the glMultMatrixd(...) procedure) */


#endif
