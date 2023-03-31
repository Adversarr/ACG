/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'acgipc'                        *
 ******************************************************************************/

#ifndef ACGIPC__EDGE_EDGE_VALUE__H
#define ACGIPC__EDGE_EDGE_VALUE__H

double edge_edge_value(double a_0, double a_1, double a_2, double b_0,
                       double b_1, double b_2, double c_0, double c_1,
                       double c_2, double d_0, double d_1, double d_2,
                       double d_h);
void edge_edge_grad(double a_0, double a_1, double a_2, double b_0, double b_1,
                    double b_2, double c_0, double c_1, double c_2, double d_0,
                    double d_1, double d_2, double d_h,
                    double *out_7057165182775666242);
void edge_edge_hessian(double a_0, double a_1, double a_2, double b_0,
                       double b_1, double b_2, double c_0, double c_1,
                       double c_2, double d_0, double d_1, double d_2,
                       double d_h, double *out_3598486270598774998);

float edge_edge_value(float a_0, float a_1, float a_2, float b_0, float b_1,
                      float b_2, float c_0, float c_1, float c_2, float d_0,
                      float d_1, float d_2, float d_h);
void edge_edge_grad(float a_0, float a_1, float a_2, float b_0, float b_1,
                    float b_2, float c_0, float c_1, float c_2, float d_0,
                    float d_1, float d_2, float d_h,
                    float *out_7057165182775666242);
void edge_edge_hessian(float a_0, float a_1, float a_2, float b_0, float b_1,
                       float b_2, float c_0, float c_1, float c_2, float d_0,
                       float d_1, float d_2, float d_h,
                       float *out_3598486270598774998);

#endif
