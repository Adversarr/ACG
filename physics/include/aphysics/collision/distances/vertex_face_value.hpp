/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'acgipc'                        *
 ******************************************************************************/

#ifndef ACGIPC__VERTEX_FACE_VALUE__H
#define ACGIPC__VERTEX_FACE_VALUE__H

double vertex_face_value(double a_0, double a_1, double a_2, double b_0,
                         double b_1, double b_2, double c_0, double c_1,
                         double c_2, double d_0, double d_1, double d_2,
                         double d_h);
void vertex_face_grad(double a_0, double a_1, double a_2, double b_0,
                      double b_1, double b_2, double c_0, double c_1,
                      double c_2, double d_0, double d_1, double d_2,
                      double d_h, double *out_6543693310212660619);
void vertex_face_hessian(double a_0, double a_1, double a_2, double b_0,
                         double b_1, double b_2, double c_0, double c_1,
                         double c_2, double d_0, double d_1, double d_2,
                         double d_h, double *out_3216172810630303951);

float vertex_face_value(float a_0, float a_1, float a_2, float b_0, float b_1,
                        float b_2, float c_0, float c_1, float c_2, float d_0,
                        float d_1, float d_2, float d_h);
void vertex_face_grad(float a_0, float a_1, float a_2, float b_0, float b_1,
                      float b_2, float c_0, float c_1, float c_2, float d_0,
                      float d_1, float d_2, float d_h,
                      float *out_6543693310212660619);
void vertex_face_hessian(float a_0, float a_1, float a_2, float b_0, float b_1,
                         float b_2, float c_0, float c_1, float c_2, float d_0,
                         float d_1, float d_2, float d_h,
                         float *out_3216172810630303951);

#endif
