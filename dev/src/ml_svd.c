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


#include <ml/ml_svd.h>
#include <ml/ml_mat2.h>
#include <ml/ml_mat3.h>
#include <ml/ml_mat4.h>
#include <ml/ml_util.h>
#include <math.h>


double mlPythagore(double _x, double _y)
{
	double absx = fabs(_x);
	double absy = fabs(_y);

	if(absx > absy)
		return absx * sqrt(1.0 + mlSquare(absy / absx));
	else
		return mlNearZero(absy) ? 0.0 : absy * sqrt(1.0 + mlSquare(absx / absy));
}


double mlSameSign(double _x, double _y)
{
	return _y > 0.0 ? fabs(_x) : -fabs(_x);
}


int __mlSVDNxN__(double * _A, int _n, double * _U, double * _S, double * _V, int _iterations)
{
	int    flag;
	int    i, its, j, jj, k, l, nm;
	double anorm, c, f, g, h, s, scale, x, y, z;
	mlVec3 rv1;

	l = 0;

	for(i = 0; i < _n * _n; ++i)
		_U[i] = _A[i];

	g = scale = anorm = 0.0;
	for(i = 0; i < _n; ++i)
	{
		l = i + 2;

		rv1[i] = scale * g;
		g	   =
		scale  =
		s	   = 0.0;

		if(i < _n)
		{
			for(k = i; k < _n; ++k)
				scale += fabs(_U[k * _n + i]);

			if(scale > mlEps)
			{
				for(k = i; k < _n; ++k)
				{
					_U[k * _n + i] /= scale;
					s += _U[k * _n + i] * _U[k * _n + i];
				}

				f = _U[i * _n + i];
				g = -mlSameSign(sqrt(s), f);
				h = f * g - s;

				_U[i * _n + i] = f - g;

				for(j = l - 1; j < _n; ++j)
				{
					for(s = 0.0, k = i; k < _n; ++k)
						s += _U[k * _n + i] * _U[k * _n + j];

					f = s / h;

					for(k = i; k < _n; ++k)
						_U[k * _n + j] += f * _U[k * _n + i];
				}

				for(k = i; k < _n; ++k)
					_U[k * _n + i] *= scale;
			}
		}

		_S[i] = scale * g;
		g	  =
		scale =
		s	  = 0.0;

		if(i + 1 <= _n && i != _n)
		{
			for(k = l - 1; k < _n; ++k)
				scale += fabs(_U[i * _n + k]);

			if(scale > mlEps)
			{
				for(k = l - 1; k < _n; ++k)
				{
					_U[i * _n + k] /= scale;
					s += _U[i * _n + k] * _U[i * _n + k];
				}

				f = _U[i * _n + l - 1];
				g = -mlSameSign(sqrt(s), f);
				h = f * g - s;

				_U[i * _n + l - 1] = f - g;

				for(k = l - 1; k < _n; ++k)
					rv1[k] = _U[i * _n + k] / h;

				for(j = l - 1; j < _n; ++j)
				{
					for(s = 0.0, k = l - 1; k < _n; ++k)
						s += _U[j * _n + k] * _U[i * _n + k];

					for(k = l - 1; k < _n; ++k)
						_U[j * _n + k] += s * rv1[k];
				}

				for(k = l - 1; k < _n; ++k)
					_U[i * _n + k] *= scale;
			}
		}

		anorm = mlMax2(anorm, fabs(_S[i]) + fabs(rv1[i]));
	}

	for(i = _n - 1; i >= 0; --i)
	{
		if(i < _n - 1)
		{
			if(mlNearZero(g) == 0)
			{
				for(j = l; j < _n; ++j)
					_V[j * _n + i] = _U[i * _n + j] / (_U[i * _n + l] * g);

				for(j = l; j < _n; ++j)
				{
					for(s = 0.0, k = l; k < _n; ++k)
						s += _U[i * _n + k] * _V[k * _n + j];

					for(k = l; k < _n; ++k)
						_V[k * _n + j] += s * _V[k * _n + i];
				}
			}

			for(j = l; j < _n; ++j)
			{
				_V[i * _n + j] =
				_V[j * _n + i] = 0.0;
			}
		}

		_V[i * _n + i] = 1.0;

		g = rv1[i];
		l = i;
	}

	for(i = mlMin2(_n, _n) - 1; i >= 0; --i)
	{
		l = i + 1;
		g = _S[i];

		for(j = l; j < _n; ++j)
			_U[i * _n + j] = 0.0;

		if(mlNearZero(g) == 0)
		{
			g = 1.0 / g;

			for(j = l; j < _n; ++j)
			{
				for(s = 0.0, k = l; k < _n; ++k)
					s += _U[k * _n + i] * _U[k * _n + j];

				f = (s / _U[i * _n + i]) * g;

				for(k = i; k < _n; ++k)
					_U[k * _n + j] += f * _U[k * _n + i];
			}

			for(j = i; j < _n; ++j)
				_U[j * _n + i] *= g;
		}
		else
			for(j = i; j < _n; ++j)
				_U[j * _n + i] = 0.0;

		++_U[i * _n + i];
	}

	for(k = _n - 1; k >= 0; --k)
	{
		for(its = 0; its < _iterations; ++its)
		{
			flag = 1;

			for(l = k; l >= 0; --l)
			{
				nm = l - 1;

				if(fabs(rv1[l]) + anorm == anorm)
				{
					flag = 0;
					break;
				}

				if(fabs(_S[nm]) + anorm == anorm)
					break;
			}

			if(flag)
			{
				c = 0.0;
				s = 1.0;

				for(i = l - 1; i < k + 1; ++i)
				{
					f	   = s * rv1[i];
					rv1[i] = c * rv1[i];

					if(fabs(f) + anorm == anorm)
						break;

					g = _S[i];
					h = mlPythagore(f, g);

					_S[i] = h;

					h = 1.0 / h;
					c = g * h;
					s = - f * h;

					for(j = 0; j < _n; ++j)
					{
						y = _U[j * _n + nm];
						z = _U[j * _n + i];

						_U[j * _n + nm] = y * c + z * s;
						_U[j * _n + i] = z * c - y * s;
					}
				}
			}

			z = _S[k];
			if(l == k)
			{
				if(z < 0.0)
				{
					_S[k] = - z;

					for(j = 0; j < _n; ++j)
						_V[j * _n + k] = -_V[j * _n + k];
				}

				break;
			}

			if(its == _iterations - 1)
				return -1;

			x  = _S[l];
			nm = k - 1;
			y  = _S[nm];

			g = rv1[nm];
			h = rv1[k];
			f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);

			g  = mlPythagore(f, 1.0);
			f  = ((x - z) * (x + z) + h * ((y / (f + mlSameSign(g, f))) - h)) / x;
			c  = s = 1.0;

			for(j = l; j <= nm; ++j)
			{
				i = j + 1;

				g = rv1[i];
				y = _S[i];
				h = s * g;

				g = c * g;
				z = mlPythagore(f, h);

				rv1[j] = z;

				c = f / z;
				s = h / z;

				f  = x * c + g * s;
				g  = g * c - x * s;
				h  = y * s;
				y *= c;

				for(jj = 0; jj < _n; ++jj)
				{
					x = _V[jj * _n + j];
					z = _V[jj * _n + i];

					_V[jj * _n + j] = x * c + z * s;
					_V[jj * _n + i] = z * c - x * s;
				}

				z = mlPythagore(f, h);

				_S[j] = z;

				if(z > mlEps)
				{
					z = 1.0 / z;
					c = f * z;
					s = h * z;
				}

				f = c * g + s * y;
				x = c * y - s * g;

				for(jj = 0; jj < _n; ++jj)
				{
					y = _U[jj * _n + j];
					z = _U[jj * _n + i];

					_U[jj * _n + j] = y * c + z * s;
					_U[jj * _n + i] = z * c - y * s;
				}
			}

			rv1[l] = 0.0;
			rv1[k] = f;
			_S[k]  = x;
		}
	}

	return 0;
}


int mlSVD2x2(mlMat2 _A, mlMat2 _U, mlVec2 _S, mlMat2 _V, int _iterations)
{
	return __mlSVDNxN__(_A, 2, _U, _S, _V, _iterations);
}


int mlSVD3x3(mlMat3 _A, mlMat3 _U, mlVec3 _S, mlMat3 _V, int _iterations)
{
	return __mlSVDNxN__(_A, 3, _U, _S, _V, _iterations);
}


int mlSVD4x4(mlMat4 _A, mlMat4 _U, mlVec4 _S, mlMat4 _V, int _iterations)
{
	return __mlSVDNxN__(_A, 4, _U, _S, _V, _iterations);
}

