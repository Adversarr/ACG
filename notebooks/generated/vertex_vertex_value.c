/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                  This file is part of 'ACG_3RDPARTY_IPC'                   *
 ******************************************************************************/
#include "vertex_vertex_value.h"
#include <math.h>

double vertex_vertex_value(double a_0, double a_1, double a_2, double b_0, double b_1, double b_2, double d_h) {

   double vertex_vertex_value_result;
   vertex_vertex_value_result = -pow(d_h - sqrt(pow(a_0 - b_0, 2) + pow(a_1 - b_1, 2) + pow(a_2 - b_2, 2)), 2)*log(sqrt(pow(a_0, 2) - 2*a_0*b_0 + pow(a_1, 2) - 2*a_1*b_1 + pow(a_2, 2) - 2*a_2*b_2 + pow(b_0, 2) + pow(b_1, 2) + pow(b_2, 2))/d_h);
   return vertex_vertex_value_result;

}

void vertex_vertex_grad(double a_0, double a_1, double a_2, double b_0, double b_1, double b_2, double d_h, double *out_2356872058512998853) {
   const double x0 = a_0 - b_0;
   const double x1 = a_1 - b_1;
   const double x2 = a_2 - b_2;
   const double x3 = sqrt(pow(x0, 2) + pow(x1, 2) + pow(x2, 2));
   const double x4 = d_h - x3;
   const double x5 = pow(a_0, 2) - 2*a_0*b_0 + pow(a_1, 2) - 2*a_1*b_1 + pow(a_2, 2) - 2*a_2*b_2 + pow(b_0, 2) + pow(b_1, 2) + pow(b_2, 2);
   const double x6 = pow(x4, 2)/x5;
   const double x7 = 1.0/x3;
   const double x8 = log(sqrt(x5)/d_h);
   const double x9 = -x0;
   const double x10 = -x1;
   const double x11 = -x2;

   out_2356872058512998853[0] = 2*x0*x4*x7*x8 - x0*x6;
   out_2356872058512998853[1] = 2*x1*x4*x7*x8 - x1*x6;
   out_2356872058512998853[2] = 2*x2*x4*x7*x8 - x2*x6;
   out_2356872058512998853[3] = 2*x4*x7*x8*x9 - x6*x9;
   out_2356872058512998853[4] = 2*x10*x4*x7*x8 - x10*x6;
   out_2356872058512998853[5] = 2*x11*x4*x7*x8 - x11*x6;

}

void vertex_vertex_hessian(double a_0, double a_1, double a_2, double b_0, double b_1, double b_2, double d_h, double *out_3076791349377008312) {
   const double x0 = a_0 - b_0;
   const double x1 = pow(x0, 2);
   const double x2 = a_1 - b_1;
   const double x3 = pow(x2, 2);
   const double x4 = a_2 - b_2;
   const double x5 = pow(x4, 2);
   const double x6 = x1 + x3 + x5;
   const double x7 = 2*a_0;
   const double x8 = 2*a_1;
   const double x9 = 2*a_2;
   const double x10 = pow(a_0, 2) + pow(a_1, 2) + pow(a_2, 2) + pow(b_0, 2) - b_0*x7 + pow(b_1, 2) - b_1*x8 + pow(b_2, 2) - b_2*x9;
   const double x11 = log(sqrt(x10)/d_h);
   const double x12 = 2*x11;
   const double x13 = x12/x6;
   const double x14 = -2*b_0 + x7;
   const double x15 = sqrt(x6);
   const double x16 = d_h - x15;
   const double x17 = pow(x16, 2);
   const double x18 = x17/pow(x10, 2);
   const double x19 = x0*x18;
   const double x20 = 1.0/x15;
   const double x21 = 1.0/x10;
   const double x22 = -x0;
   const double x23 = pow(x6, -3.0/2.0);
   const double x24 = x17*x21;
   const double x25 = x16*x20;
   const double x26 = x12*x25;
   const double x27 = x24 - x26;
   const double x28 = x0*x13;
   const double x29 = -2*b_1 + x8;
   const double x30 = -x29;
   const double x31 = -x2;
   const double x32 = 2*x0*x11*x16*x23*x31 + 4*x0*x16*x2*x20*x21 - x19*x30 - x2*x28;
   const double x33 = -2*b_2 + x9;
   const double x34 = -x33;
   const double x35 = -x4;
   const double x36 = 2*x0*x11*x16*x23*x35 + 4*x0*x16*x20*x21*x4 - x19*x34 - x28*x4;
   const double x37 = x12*x16*x23;
   const double x38 = 4*x21*x25;
   const double x39 = x0*x38;
   const double x40 = x1*x37 - x14*x19 - x22*x28 + x22*x39 + x27;
   const double x41 = x0*x37;
   const double x42 = x2*x41;
   const double x43 = -x19*x29 - x28*x31 + x31*x39 + x42;
   const double x44 = x4*x41;
   const double x45 = -x19*x33 - x28*x35 + x35*x39 + x44;
   const double x46 = x18*x2;
   const double x47 = x13*x2;
   const double x48 = 2*x11*x16*x2*x23*x35 + 4*x16*x2*x20*x21*x4 - x34*x46 - x4*x47;
   const double x49 = x2*x38;
   const double x50 = -x14*x46 - x22*x47 + x22*x49 + x42;
   const double x51 = x27 - x29*x46 + x3*x37 - x31*x47 + x31*x49;
   const double x52 = x2*x37;
   const double x53 = x4*x52;
   const double x54 = -x33*x46 - x35*x47 + x35*x49 + x53;
   const double x55 = x18*x4;
   const double x56 = x13*x4;
   const double x57 = x38*x4;
   const double x58 = -x14*x55 - x22*x56 + x22*x57 + x44;
   const double x59 = -x29*x55 - x31*x56 + x31*x57 + x53;
   const double x60 = x35*x4;
   const double x61 = x27 - x33*x55 - x35*x56 + x37*x5 + x38*x60;
   const double x62 = pow(x22, 2);
   const double x63 = x18*x22;
   const double x64 = -x24 + x26;
   const double x65 = x13*x22;
   const double x66 = x22*x38;
   const double x67 = x22*x52 - x29*x63 - x31*x65 + x31*x66;
   const double x68 = x37*x4;
   const double x69 = x22*x68 - x33*x63 - x35*x65 + x35*x66;
   const double x70 = pow(x31, 2);
   const double x71 = x18*x31;
   const double x72 = x31*x35;
   const double x73 = -x13*x72 + x31*x68 - x33*x71 + x38*x72;
   const double x74 = pow(x35, 2);

   out_3076791349377008312[0] = 2*x0*x11*x16*x22*x23 - x1*x13 + 4*x1*x16*x20*x21 + x14*x19 - x27;
   out_3076791349377008312[1] = x32;
   out_3076791349377008312[2] = x36;
   out_3076791349377008312[3] = x40;
   out_3076791349377008312[4] = x43;
   out_3076791349377008312[5] = x45;
   out_3076791349377008312[6] = x32;
   out_3076791349377008312[7] = 2*x11*x16*x2*x23*x31 - x13*x3 + 4*x16*x20*x21*x3 - x27 - x30*x46;
   out_3076791349377008312[8] = x48;
   out_3076791349377008312[9] = x50;
   out_3076791349377008312[10] = x51;
   out_3076791349377008312[11] = x54;
   out_3076791349377008312[12] = x36;
   out_3076791349377008312[13] = x48;
   out_3076791349377008312[14] = 2*x11*x16*x23*x35*x4 - x13*x5 + 4*x16*x20*x21*x5 - x27 - x34*x55;
   out_3076791349377008312[15] = x58;
   out_3076791349377008312[16] = x59;
   out_3076791349377008312[17] = x61;
   out_3076791349377008312[18] = x40;
   out_3076791349377008312[19] = x50;
   out_3076791349377008312[20] = x58;
   out_3076791349377008312[21] = -x13*x62 - x14*x63 + x22*x41 + x38*x62 + x64;
   out_3076791349377008312[22] = x67;
   out_3076791349377008312[23] = x69;
   out_3076791349377008312[24] = x43;
   out_3076791349377008312[25] = x51;
   out_3076791349377008312[26] = x59;
   out_3076791349377008312[27] = x67;
   out_3076791349377008312[28] = -x13*x70 - x29*x71 + x31*x52 + x38*x70 + x64;
   out_3076791349377008312[29] = x73;
   out_3076791349377008312[30] = x45;
   out_3076791349377008312[31] = x54;
   out_3076791349377008312[32] = x61;
   out_3076791349377008312[33] = x69;
   out_3076791349377008312[34] = x73;
   out_3076791349377008312[35] = -x13*x74 - x18*x33*x35 + x37*x60 + x38*x74 + x64;

}
