/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'acgipc'                        *
 ******************************************************************************/

#ifndef ACGIPC__VERTEX_VERTEX_VALUE__H
#define ACGIPC__VERTEX_VERTEX_VALUE__H

double vertex_vertex_value(double a_0, double a_1, double a_2, double b_0,
                           double b_1, double b_2, double d_h);
void vertex_vertex_grad(double a_0, double a_1, double a_2, double b_0,
                        double b_1, double b_2, double d_h,
                        double *out_8244818615857473380);
void vertex_vertex_hessian(double a_0, double a_1, double a_2, double b_0,
                           double b_1, double b_2, double d_h,
                           double *out_8361029790532263509);

float vertex_vertex_value(float a_0, float a_1, float a_2, float b_0, float b_1,
                          float b_2, float d_h);
void vertex_vertex_grad(float a_0, float a_1, float a_2, float b_0, float b_1,
                        float b_2, float d_h, float *out_8244818615857473380);
void vertex_vertex_hessian(float a_0, float a_1, float a_2, float b_0,
                           float b_1, float b_2, float d_h,
                           float *out_8361029790532263509);
#endif
