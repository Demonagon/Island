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


#ifndef __ML_MAT3__
#define __ML_MAT3__


#include <ml/ml_define.h>


void	mlMat3_Set		(mlMat3 _out, double _a, double _b, double _c, double _d, double _e, double _f, double _g, double _h, double _i);
																												/* Fill the _out matrix with |_a _b _c| */
																												/* 							 |_d _e _f| */
																												/* 							 |_g _h _i| */

void	mlMat3_Zero		(mlMat3 _out);																			/* Fill the _out matrix with zeros */
void	mlMat3_Identity	(mlMat3 _out);																			/* Copy the identity matrix into _out */
void	mlMat3_Copy		(mlMat3 _out, const mlMat3 _in);														/* Copy the matrix _in into _out */


void	mlMat3_FromCols	(mlMat3 _out, const mlVec3 _col1, const mlVec3 _col2, const mlVec3 _col3);				/* Copy the 3 columns vectors into _out */
void	mlMat3_FromRows	(mlMat3 _out, const mlVec3 _row1, const mlVec3 _row2, const mlVec3 _row3);				/* Copy the 3 rows vectors into _out */

void	mlMat3_Add		(mlMat3 _out, const mlMat3 _in, double _k);												/* _out = _in + _k */
void	mlMat3_Sub		(mlMat3 _out, const mlMat3 _in, double _k);												/* _out = _in - _k */
void	mlMat3_Scale	(mlMat3 _out, const mlMat3 _in, double _k);												/* _out = _in * _k */
void	mlMat3_Neg		(mlMat3 _out, const mlMat3 _in);														/* _out = -_out */

void	mlMat3_AddMat	(mlMat3 _out, const mlMat3 _in1, const mlMat3 _in2);									/* _out = _in1 + _in2 */
void	mlMat3_SubMat	(mlMat3 _out, const mlMat3 _in1, const mlMat3 _in2);									/* _out = _in1 - _in2 */
void	mlMat3_MultMat	(mlMat3 _out, const mlMat3 _in1, const mlMat3 _in2);									/* _out = _in1 * _in2 */

void	mlMat3_MultVec	(mlVec3 _out, const mlMat3 _in1, const mlVec3 _in2);									/* _out = _in1 * _in2 (column vector) */

double	mlMat3_Det		(const mlMat3 _in);																		/* Compute the determinant of _in */
void	mlMat3_Inv		(mlMat3 _out, const mlMat3 _in);														/* _out = _in^-1 */
void	mlMat3_Transpose(mlMat3 _out, const mlMat3 _in);														/* _out = _in' */

void	mlMat3_Print	(const mlMat3 _in);																		/* Print the _in matrix */


#endif
