/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'acgipc'                        *
 ******************************************************************************/

#ifndef ACGIPC__EDGE_VERTEX_VALUE__H
#define ACGIPC__EDGE_VERTEX_VALUE__H

double edge_vertex_value(double a_0, double a_1, double a_2, double b_0,
                         double b_1, double b_2, double c_0, double c_1,
                         double c_2, double d_h);
void edge_vertex_grad(double a_0, double a_1, double a_2, double b_0,
                      double b_1, double b_2, double c_0, double c_1,
                      double c_2, double d_h, double *out_5905569529076117828);
void edge_vertex_hessian(double a_0, double a_1, double a_2, double b_0,
                         double b_1, double b_2, double c_0, double c_1,
                         double c_2, double d_h,
                         double *out_2823277736450422735);

float edge_vertex_value(float a_0, float a_1, float a_2, float b_0, float b_1,
                        float b_2, float c_0, float c_1, float c_2, float d_h);
void edge_vertex_grad(float a_0, float a_1, float a_2, float b_0, float b_1,
                      float b_2, float c_0, float c_1, float c_2, float d_h,
                      float *out_5905569529076117828);
void edge_vertex_hessian(float a_0, float a_1, float a_2, float b_0, float b_1,
                         float b_2, float c_0, float c_1, float c_2, float d_h,
                         float *out_2823277736450422735);

#endif
