/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'acgipc'                        *
 ******************************************************************************/
#include "evd_value.h"
#include <math.h>

double evd_value(double a_0, double a_1, double a_2, double b_0, double b_1, double b_2, double c_0, double c_1, double c_2) {
   const double x0 = a_0 - b_0;
   const double x1 = a_1 - b_1;
   const double x2 = a_2 - b_2;
   const double x3 = a_1 - c_1;
   const double x4 = a_0 - c_0;
   const double x5 = a_2 - c_2;

   double evd_value_result;
   evd_value_result = sqrt(pow(x0*x3 - x1*x4, 2) + pow(x0*x5 - x2*x4, 2) + pow(x1*x5 - x2*x3, 2))/sqrt(pow(x0, 2) + pow(x1, 2) + pow(x2, 2));
   return evd_value_result;

}

void evd_grad(double a_0, double a_1, double a_2, double b_0, double b_1, double b_2, double c_0, double c_1, double c_2, double *out_6809069808720211477) {
   const double x0 = a_0 - b_0;
   const double x1 = a_1 - b_1;
   const double x2 = a_2 - b_2;
   const double x3 = pow(x0, 2) + pow(x1, 2) + pow(x2, 2);
   const double x4 = a_1 - c_1;
   const double x5 = a_0 - c_0;
   const double x6 = x0*x4 - x1*x5;
   const double x7 = a_2 - c_2;
   const double x8 = x0*x7 - x2*x5;
   const double x9 = x1*x7 - x2*x4;
   const double x10 = sqrt(pow(x6, 2) + pow(x8, 2) + pow(x9, 2));
   const double x11 = x10/pow(x3, 3.0/2.0);
   const double x12 = 2*b_1;
   const double x13 = -2*c_1;
   const double x14 = x12 + x13;
   const double x15 = (1.0/2.0)*x6;
   const double x16 = 2*b_2;
   const double x17 = -2*c_2;
   const double x18 = (1.0/2.0)*x16 + (1.0/2.0)*x17;
   const double x19 = 1/(x10*sqrt(x3));
   const double x20 = 2*b_0;
   const double x21 = -2*c_0;
   const double x22 = -x20 - x21;
   const double x23 = (1.0/2.0)*x8;
   const double x24 = (1.0/2.0)*x9;
   const double x25 = 2*a_1;
   const double x26 = x13 + x25;
   const double x27 = 2*a_2;
   const double x28 = -x17 - x27;
   const double x29 = 2*a_0;
   const double x30 = x21 + x29;
   const double x31 = -x12 + x25;
   const double x32 = -x16 + x27;
   const double x33 = x20 - x29;

   out_6809069808720211477[0] = -x0*x11 + x19*(x14*x15 + x18*x8);
   out_6809069808720211477[1] = -x1*x11 + x19*(x15*x22 + x18*x9);
   out_6809069808720211477[2] = -x11*x2 + x19*(-x14*x24 + x22*x23);
   out_6809069808720211477[3] = x0*x11 + x19*(-x15*x26 + x23*x28);
   out_6809069808720211477[4] = x1*x11 + x19*(x15*x30 + x24*x28);
   out_6809069808720211477[5] = x11*x2 + x19*(x23*x30 + x24*x26);
   out_6809069808720211477[6] = x19*(x15*x31 + x23*x32);
   out_6809069808720211477[7] = x19*(x15*x33 + x24*x32);
   out_6809069808720211477[8] = x19*(x23*x33 - x24*x31);

}
