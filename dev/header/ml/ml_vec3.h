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


#ifndef __ML_VEC3__
#define __ML_VEC3__


#include <ml/ml_define.h>


void	mlVec3_Set		(mlVec3 _out, double _x, double _y, double _z);					/* Fill the _out vector with |_x _y _z| */
void	mlVec3_Zero		(mlVec3 _out);													/* Fill the _out vector with zeros */
void	mlVec3_Copy		(mlVec3 _out, const mlVec3 _in);								/* Copy the _in vector into _out */

void	mlVec3_Add		(mlVec3 _out, const mlVec3 _in, double _k);						/* _out = _in + _k */
void	mlVec3_Sub		(mlVec3 _out, const mlVec3 _in, double _k);						/* _out = _in - _k */
void	mlVec3_Scale	(mlVec3 _out, const mlVec3 _in, double _k);						/* _out = _in * _k */
void	mlVec3_Neg		(mlVec3 _out, const mlVec3 _in);								/* _out = -_in */

void	mlVec3_AddVec	(mlVec3 _out, const mlVec3 _in1, const mlVec3 _in2);			/* _out = _in1 + _in2 */
void	mlVec3_SubVec	(mlVec3 _out, const mlVec3 _in1, const mlVec3 _in2);			/* _out = _in1 - _in2 */

void	mlVec3_MultMat	(mlVec3 _out, const mlVec3 _in1, const mlMat3 _in2);			/* _out = _in1 (row vector) * _in2 */

void	mlVec3_Normalize(mlVec3 _out, const mlVec3 _in);								/* _out = _in / ||_in|| */
double	mlVec3_Norm		(const mlVec3 _in);												/* Return ||_in|| */
double	mlVec3_Norm2	(const mlVec3 _in);												/* Return ||_in|| * ||_in|| */
double	mlVec3_Dot		(const mlVec3 _in1, const mlVec3 _in2);							/* Return _in1 . _in2 */
void	mlVec3_Cross	(mlVec3 _out, const mlVec3 _in1, const mlVec3 _in2);			/* _out = _in1 x _in2 */

void	mlVec3_Print	(const mlVec3 _in);												/* Print the _in vector */


#endif
