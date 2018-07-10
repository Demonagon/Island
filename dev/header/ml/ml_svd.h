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


#ifndef __ML_SVD__
#define __ML_SVD__


#include <ml/ml_define.h>


int mlSVD2x2(mlMat2 _a, mlMat2 _U, mlVec2 _S, mlMat2 _V, int _iterations);	/* Compute the singular value decomposition of a 2x2 matrix */
int mlSVD3x3(mlMat3 _a, mlMat3 _U, mlVec3 _S, mlMat3 _V, int _iterations);	/* Compute the singular value decomposition of a 3x3 matrix */
int mlSVD4x4(mlMat4 _a, mlMat4 _U, mlVec4 _S, mlMat4 _V, int _iterations);	/* Compute the singular value decomposition of a 4x4 matrix */


#endif

