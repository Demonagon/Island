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


#ifndef __ML_DEFINE__
#define __ML_DEFINE__


static const double mlEps = 1.0e-09;				/* Definition of epsilon */
static const double mlPi  = 3.14159265358979323846;	/* Definition of PI */
static const double mlPi2 = 1.57079632679489661923;	/* Definition of PI / 2 */

typedef double mlVec2[ 2];							/* Definition of 2D vector as an array of 2 doubles */
typedef double mlVec3[ 3];							/* Definition of 3D vector as an array of 3 doubles */
typedef double mlVec4[ 4];							/* Definition of 4D vector as an array of 4 doubles */

typedef double mlMat2[ 4];							/* Definition of 2D matrix as an array of  4 doubles */
typedef double mlMat3[ 9];							/* Definition of 3D matrix as an array of  9 doubles */
typedef double mlMat4[16];							/* Definition of 4D matrix as an array of 16 doubles */


#endif

