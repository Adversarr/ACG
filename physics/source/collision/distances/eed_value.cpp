/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'acgipc'                        *
 ******************************************************************************/
#include <aphysics/collision/distances/eed_value.hpp>
#include <math.h>

double eed_value(double a_0, double a_1, double a_2, double b_0, double b_1, double b_2, double c_0, double c_1, double c_2, double d_0, double d_1, double d_2) {
   const double x0 = a_0 - b_0;
   const double x1 = c_1 - d_1;
   const double x2 = a_1 - b_1;
   const double x3 = c_0 - d_0;
   const double x4 = x0*x1 - x2*x3;
   const double x5 = c_2 - d_2;
   const double x6 = a_2 - b_2;
   const double x7 = x0*x5 - x3*x6;
   const double x8 = -x1*x6 + x2*x5;

   double eed_value_result;
   eed_value_result = (-x4*(a_2 - c_2) + x7*(a_1 - c_1) - x8*(a_0 - c_0))/sqrt(pow(x4, 2) + pow(x7, 2) + pow(x8, 2));
   return eed_value_result;

}

void eed_grad(double a_0, double a_1, double a_2, double b_0, double b_1, double b_2, double c_0, double c_1, double c_2, double d_0, double d_1, double d_2, double *out_4484199130847538343) {
   const double x0 = a_1 - c_1;
   const double x1 = c_2 - d_2;
   const double x2 = a_2 - c_2;
   const double x3 = -x2;
   const double x4 = c_1 - d_1;
   const double x5 = a_2 - b_2;
   const double x6 = x4*x5;
   const double x7 = a_1 - b_1;
   const double x8 = x1*x7;
   const double x9 = a_0 - b_0;
   const double x10 = x4*x9;
   const double x11 = c_0 - d_0;
   const double x12 = x11*x7;
   const double x13 = x10 - x12;
   const double x14 = x1*x9;
   const double x15 = x11*x5;
   const double x16 = x14 - x15;
   const double x17 = -x6 + x8;
   const double x18 = pow(x13, 2) + pow(x16, 2) + pow(x17, 2);
   const double x19 = pow(x18, -1.0/2.0);
   const double x20 = 2*c_1 - 2*d_1;
   const double x21 = (1.0/2.0)*x13;
   const double x22 = 2*c_2 - 2*d_2;
   const double x23 = (1.0/2.0)*x22;
   const double x24 = a_0 - c_0;
   const double x25 = (x0*x16 - x13*x2 - x17*x24)/pow(x18, 3.0/2.0);
   const double x26 = -x24;
   const double x27 = -x11;
   const double x28 = 2*c_0 - 2*d_0;
   const double x29 = -x28;
   const double x30 = -x4;
   const double x31 = (1.0/2.0)*x16;
   const double x32 = -x20;
   const double x33 = (1.0/2.0)*x17;
   const double x34 = -x1;
   const double x35 = -x22;
   const double x36 = -x5;
   const double x37 = -x7;
   const double x38 = 2*a_1 - 2*b_1;
   const double x39 = -x38;
   const double x40 = 2*a_2 - 2*b_2;
   const double x41 = -x40;
   const double x42 = 2*a_0 - 2*b_0;
   const double x43 = -x9;
   const double x44 = -x42;

   out_4484199130847538343[0] = x19*(x0*x1 + x3*x4 + x6 - x8) + x25*(-x16*x23 - x20*x21);
   out_4484199130847538343[1] = x19*(x1*x26 + x16 + x27*x3) + x25*(-x17*x23 - x21*x29);
   out_4484199130847538343[2] = x19*(x0*x27 - x10 + x12 + x26*x30) + x25*(-x29*x31 - x32*x33);
   out_4484199130847538343[3] = x19*(x0*x34 + x3*x30) + x25*(-x21*x32 - x31*x35);
   out_4484199130847538343[4] = x19*(x11*x3 + x26*x34) + x25*(-x21*x28 - x33*x35);
   out_4484199130847538343[5] = x19*(x0*x11 + x26*x4) + x25*(-x20*x33 - x28*x31);
   out_4484199130847538343[6] = x19*(x0*x36 + x17 + x3*x37) + x25*(-x21*x39 - x31*x41);
   out_4484199130847538343[7] = x19*(-x14 + x15 + x26*x36 + x3*x9) + x25*(-x21*x42 - x33*x41);
   out_4484199130847538343[8] = x19*(x0*x9 + x13 + x26*x7) + x25*(-x31*x42 - x33*x38);
   out_4484199130847538343[9] = x19*(x0*x5 + x3*x7) + x25*(-x21*x38 - x31*x40);
   out_4484199130847538343[10] = x19*(x26*x5 + x3*x43) + x25*(-x21*x44 - x33*x40);
   out_4484199130847538343[11] = x19*(x0*x43 + x26*x37) + x25*(-x31*x44 - x33*x39);

}
