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


#include <ml/ml_util.h>
#include <ml/ml_vec2.h>
#include <ml/ml_vec3.h>
#include <ml/ml_vec4.h>
#include <math.h>


double mlDistance2(mlVec2 _a, mlVec2 _b)
{
	mlVec2 v;
	
	mlVec2_SubVec(v, _b, _a);
	return mlVec2_Norm(v);
}


double mlDistance3(mlVec3 _a, mlVec3 _b)
{
	mlVec3 v;
	
	mlVec3_SubVec(v, _b, _a);
	return mlVec3_Norm(v);
}


double mlDistance4(mlVec4 _a, mlVec4 _b)
{
	mlVec4 v;
	
	mlVec4_SubVec(v, _b, _a);
	return mlVec4_Norm(v);
}


double mlMax2(double _a, double _b)
{
	return (_a > _b) ? _a : _b;
}


double mlMax3(double _a, double _b, double _c)
{
	return mlMax2(mlMax2(_a, _b), _c);
}


double mlMin2(double _a, double _b)
{
	return (_a < _b) ? _a : _b;
}


double mlMin3(double _a, double _b, double _c)
{
	return mlMin2(mlMin2(_a, _b), _c);
}


void mlSwap(double * _a, double * _b)
{
	double t = *_b;
	*_b = *_a;
	*_a = t;
}


double mlSquare(double _x)
{
	return _x * _x;
}


double mlCube(double _x)
{
	return _x * _x * _x;
}


int mlSign(double _x)
{
	return (_x > 0) ? 1 : -1;
}


int mlNearZero(double _x)
{
	return fabs(_x) < mlEps;
}


int mlNearValue(double _x, double _a)
{
	return mlNearZero(_x - _a);
}


double mlClamp(double _x, double _a, double _b)
{
	return (_x < _a) ? _a : ((_x > _b) ? _b : _x);
}


double mlDegToRad(double _x)
{
	return _x * mlPi / 180.0;
}


double mlRadToDeg(double _x)
{
	return _x * 180.0 / mlPi;
}

