/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'acgipc'                        *
 ******************************************************************************/
#include <aphysics/collision/distances/vvd_value.hpp>
#include <math.h>

double vvd_value(double a_0, double a_1, double a_2, double b_0, double b_1, double b_2) {

   double vvd_value_result;
   vvd_value_result = sqrt(pow(a_0 - b_0, 2) + pow(a_1 - b_1, 2) + pow(a_2 - b_2, 2));
   return vvd_value_result;

}

void vvd_grad(double a_0, double a_1, double a_2, double b_0, double b_1, double b_2, double *out_8800465547854922746) {
   const double x0 = a_0 - b_0;
   const double x1 = a_1 - b_1;
   const double x2 = a_2 - b_2;
   const double x3 = pow(pow(x0, 2) + pow(x1, 2) + pow(x2, 2), -1.0/2.0);

   out_8800465547854922746[0] = x0*x3;
   out_8800465547854922746[1] = x1*x3;
   out_8800465547854922746[2] = x2*x3;
   out_8800465547854922746[3] = -x0*x3;
   out_8800465547854922746[4] = -x1*x3;
   out_8800465547854922746[5] = -x2*x3;

}
