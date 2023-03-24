/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'acgipc'                        *
 ******************************************************************************/
#include "aphysics/ccd/distances/vertex_vertex_value.hpp"

#include <math.h>

double vertex_vertex_value(double a_0, double a_1, double a_2, double b_0,
                           double b_1, double b_2, double d_h) {
  const double x0 = ((a_0 - b_0) * (a_0 - b_0)) + ((a_1 - b_1) * (a_1 - b_1))
                    + ((a_2 - b_2) * (a_2 - b_2));

  double vertex_vertex_value_result;
  vertex_vertex_value_result
      = -1.0 / 2.0
        * (2 * log(d_h)
           - log(((a_0) * (a_0)) - 2 * a_0 * b_0 + ((a_1) * (a_1))
                 - 2 * a_1 * b_1 + ((a_2) * (a_2)) - 2 * a_2 * b_2
                 + ((b_0) * (b_0)) + ((b_1) * (b_1)) + ((b_2) * (b_2))))
        * (((d_h) * (d_h)) - 2 * d_h * sqrt(x0) + x0);
  return vertex_vertex_value_result;
}

void vertex_vertex_grad(double a_0, double a_1, double a_2, double b_0,
                        double b_1, double b_2, double d_h,
                        double *out_8244818615857473380) {
  const double x0 = 2 * b_0;
  const double x1 = 2 * b_1;
  const double x2 = 2 * b_2;
  const double x3 = ((a_0) * (a_0)) - a_0 * x0 + ((a_1) * (a_1)) - a_1 * x1
                    + ((a_2) * (a_2)) - a_2 * x2 + ((b_0) * (b_0))
                    + ((b_1) * (b_1)) + ((b_2) * (b_2));
  const double x4 = -log(d_h) + (1.0 / 2.0) * log(x3);
  const double x5 = 2 * a_0;
  const double x6 = a_0 - b_0;
  const double x7 = a_1 - b_1;
  const double x8 = a_2 - b_2;
  const double x9 = ((x6) * (x6)) + ((x7) * (x7)) + ((x8) * (x8));
  const double x10 = sqrt(x9);
  const double x11 = 2 * d_h;
  const double x12 = x11 / x10;
  const double x13 = -x0 + x5;
  const double x14 = (1.0 / 2.0) * (((d_h) * (d_h)) - x10 * x11 + x9) / x3;
  const double x15 = 2 * a_1;
  const double x16 = -x1 + x15;
  const double x17 = 2 * a_2;
  const double x18 = x17 - x2;

  out_8244818615857473380[0] = x13 * x14 + x4 * (-x0 - x12 * x6 + x5);
  out_8244818615857473380[1] = x14 * x16 + x4 * (-x1 - x12 * x7 + x15);
  out_8244818615857473380[2] = x14 * x18 + x4 * (-x12 * x8 + x17 - x2);
  out_8244818615857473380[3] = -x13 * x14 + x4 * (x12 * x6 - x13);
  out_8244818615857473380[4] = -x14 * x16 + x4 * (x12 * x7 - x16);
  out_8244818615857473380[5] = -x14 * x18 + x4 * (x12 * x8 - x18);
}

void vertex_vertex_hessian(double a_0, double a_1, double a_2, double b_0,
                           double b_1, double b_2, double d_h,
                           double *out_8361029790532263509) {
  const double x0 = 2 * a_0;
  const double x1 = 2 * b_0;
  const double x2 = a_0 - b_0;
  const double x3 = ((x2) * (x2));
  const double x4 = a_1 - b_1;
  const double x5 = ((x4) * (x4));
  const double x6 = a_2 - b_2;
  const double x7 = ((x6) * (x6));
  const double x8 = x3 + x5 + x7;
  const double x9 = sqrt(x8);
  const double x10 = 2 * d_h;
  const double x11 = x10 / x9;
  const double x12 = x0 - x1 - x11 * x2;
  const double x13 = 2 * b_1;
  const double x14 = 2 * b_2;
  const double x15 = ((a_0) * (a_0)) - a_0 * x1 + ((a_1) * (a_1)) - a_1 * x13
                     + ((a_2) * (a_2)) - a_2 * x14 + ((b_0) * (b_0))
                     + ((b_1) * (b_1)) + ((b_2) * (b_2));
  const double x16 = 1.0 / x15;
  const double x17 = x0 - x1;
  const double x18 = x16 * x17;
  const double x19 = ((d_h) * (d_h)) - x10 * x9 + x8;
  const double x20 = x16 * x19;
  const double x21 = -log(d_h) + (1.0 / 2.0) * log(x15);
  const double x22 = -x2;
  const double x23 = x10 / pow(x8, 3.0 / 2.0);
  const double x24 = x2 * x23;
  const double x25 = x11 - 2;
  const double x26 = -x17;
  const double x27 = (1.0 / 2.0) * x19 / ((x15) * (x15));
  const double x28 = x17 * x27;
  const double x29 = x20 + x21 * (-x22 * x24 - x25) + x26 * x28;
  const double x30 = -x4;
  const double x31 = x21 * x24;
  const double x32 = 2 * a_1;
  const double x33 = -x11 * x4 - x13 + x32;
  const double x34 = (1.0 / 2.0) * x18;
  const double x35 = -x13 + x32;
  const double x36 = x16 * x35;
  const double x37 = (1.0 / 2.0) * x12;
  const double x38 = -x35;
  const double x39 = x28 * x38 - x30 * x31 + x33 * x34 + x36 * x37;
  const double x40 = -x6;
  const double x41 = 2 * a_2;
  const double x42 = -x11 * x6 - x14 + x41;
  const double x43 = -x14 + x41;
  const double x44 = x16 * x43;
  const double x45 = -x43;
  const double x46 = x28 * x45 - x31 * x40 + x34 * x42 + x37 * x44;
  const double x47 = x16 * x26;
  const double x48 = -x11 * x22 - x17;
  const double x49 = -x20;
  const double x50 = 2 - x11;
  const double x51 = ((x17) * (x17)) * x27 + x21 * (-x23 * x3 - x50) + x34 * x48
                     + x37 * x47 + x49;
  const double x52 = x16 * x38;
  const double x53 = -x11 * x30 - x35;
  const double x54 = x28 * x35 - x31 * x4;
  const double x55 = x34 * x53 + x37 * x52 + x54;
  const double x56 = x16 * x45;
  const double x57 = -x11 * x40 - x43;
  const double x58 = x28 * x43 - x31 * x6;
  const double x59 = x34 * x57 + x37 * x56 + x58;
  const double x60 = x23 * x4;
  const double x61 = x27 * x35;
  const double x62 = x20 + x21 * (-x25 - x30 * x60) + x38 * x61;
  const double x63 = x21 * x60;
  const double x64 = (1.0 / 2.0) * x36;
  const double x65 = (1.0 / 2.0) * x33;
  const double x66 = -x40 * x63 + x42 * x64 + x44 * x65 + x45 * x61;
  const double x67 = x47 * x65 + x48 * x64 + x54;
  const double x68 = x21 * (-x23 * x5 - x50) + x27 * ((x35) * (x35)) + x49
                     + x52 * x65 + x53 * x64;
  const double x69 = x43 * x61 - x6 * x63;
  const double x70 = x56 * x65 + x57 * x64 + x69;
  const double x71 = x23 * x6;
  const double x72 = x27 * x43;
  const double x73 = x20 + x21 * (-x25 - x40 * x71) + x45 * x72;
  const double x74 = (1.0 / 2.0) * x42;
  const double x75 = (1.0 / 2.0) * x44;
  const double x76 = x47 * x74 + x48 * x75 + x58;
  const double x77 = x52 * x74 + x53 * x75 + x69;
  const double x78 = x21 * (-x23 * x7 - x50) + x27 * ((x43) * (x43)) + x49
                     + x56 * x74 + x57 * x75;
  const double x79 = (1.0 / 2.0) * x47;
  const double x80 = (1.0 / 2.0) * x48;
  const double x81 = -x22 * x63 + x26 * x61 + x52 * x80 + x53 * x79;
  const double x82 = x21 * x71;
  const double x83 = -x22 * x82 + x26 * x72 + x56 * x80 + x57 * x79;
  const double x84 = -x30 * x82 + x38 * x72 + (1.0 / 2.0) * x52 * x57
                     + (1.0 / 2.0) * x53 * x56;

  out_8361029790532263509[0] = x12 * x18 + x29;
  out_8361029790532263509[1] = x39;
  out_8361029790532263509[2] = x46;
  out_8361029790532263509[3] = x51;
  out_8361029790532263509[4] = x55;
  out_8361029790532263509[5] = x59;
  out_8361029790532263509[6] = x39;
  out_8361029790532263509[7] = x33 * x36 + x62;
  out_8361029790532263509[8] = x66;
  out_8361029790532263509[9] = x67;
  out_8361029790532263509[10] = x68;
  out_8361029790532263509[11] = x70;
  out_8361029790532263509[12] = x46;
  out_8361029790532263509[13] = x66;
  out_8361029790532263509[14] = x42 * x44 + x73;
  out_8361029790532263509[15] = x76;
  out_8361029790532263509[16] = x77;
  out_8361029790532263509[17] = x78;
  out_8361029790532263509[18] = x51;
  out_8361029790532263509[19] = x67;
  out_8361029790532263509[20] = x76;
  out_8361029790532263509[21] = x29 + x47 * x48;
  out_8361029790532263509[22] = x81;
  out_8361029790532263509[23] = x83;
  out_8361029790532263509[24] = x55;
  out_8361029790532263509[25] = x68;
  out_8361029790532263509[26] = x77;
  out_8361029790532263509[27] = x81;
  out_8361029790532263509[28] = x52 * x53 + x62;
  out_8361029790532263509[29] = x84;
  out_8361029790532263509[30] = x59;
  out_8361029790532263509[31] = x70;
  out_8361029790532263509[32] = x78;
  out_8361029790532263509[33] = x83;
  out_8361029790532263509[34] = x84;
  out_8361029790532263509[35] = x56 * x57 + x73;
}
float vertex_vertex_value(float a_0, float a_1, float a_2, float b_0, float b_1,
                          float b_2, float d_h) {
  const float x0 = ((a_0 - b_0) * (a_0 - b_0)) + ((a_1 - b_1) * (a_1 - b_1))
                   + ((a_2 - b_2) * (a_2 - b_2));

  float vertex_vertex_value_result;
  vertex_vertex_value_result
      = -1.0 / 2.0
        * (2 * log(d_h)
           - log(((a_0) * (a_0)) - 2 * a_0 * b_0 + ((a_1) * (a_1))
                 - 2 * a_1 * b_1 + ((a_2) * (a_2)) - 2 * a_2 * b_2
                 + ((b_0) * (b_0)) + ((b_1) * (b_1)) + ((b_2) * (b_2))))
        * (((d_h) * (d_h)) - 2 * d_h * sqrt(x0) + x0);
  return vertex_vertex_value_result;
}

void vertex_vertex_grad(float a_0, float a_1, float a_2, float b_0, float b_1,
                        float b_2, float d_h, float *out_8244818615857473380) {
  const float x0 = 2 * b_0;
  const float x1 = 2 * b_1;
  const float x2 = 2 * b_2;
  const float x3 = ((a_0) * (a_0)) - a_0 * x0 + ((a_1) * (a_1)) - a_1 * x1
                   + ((a_2) * (a_2)) - a_2 * x2 + ((b_0) * (b_0))
                   + ((b_1) * (b_1)) + ((b_2) * (b_2));
  const float x4 = -log(d_h) + (1.0 / 2.0) * log(x3);
  const float x5 = 2 * a_0;
  const float x6 = a_0 - b_0;
  const float x7 = a_1 - b_1;
  const float x8 = a_2 - b_2;
  const float x9 = ((x6) * (x6)) + ((x7) * (x7)) + ((x8) * (x8));
  const float x10 = sqrt(x9);
  const float x11 = 2 * d_h;
  const float x12 = x11 / x10;
  const float x13 = -x0 + x5;
  const float x14 = (1.0 / 2.0) * (((d_h) * (d_h)) - x10 * x11 + x9) / x3;
  const float x15 = 2 * a_1;
  const float x16 = -x1 + x15;
  const float x17 = 2 * a_2;
  const float x18 = x17 - x2;

  out_8244818615857473380[0] = x13 * x14 + x4 * (-x0 - x12 * x6 + x5);
  out_8244818615857473380[1] = x14 * x16 + x4 * (-x1 - x12 * x7 + x15);
  out_8244818615857473380[2] = x14 * x18 + x4 * (-x12 * x8 + x17 - x2);
  out_8244818615857473380[3] = -x13 * x14 + x4 * (x12 * x6 - x13);
  out_8244818615857473380[4] = -x14 * x16 + x4 * (x12 * x7 - x16);
  out_8244818615857473380[5] = -x14 * x18 + x4 * (x12 * x8 - x18);
}

void vertex_vertex_hessian(float a_0, float a_1, float a_2, float b_0,
                           float b_1, float b_2, float d_h,
                           float *out_8361029790532263509) {
  const float x0 = 2 * a_0;
  const float x1 = 2 * b_0;
  const float x2 = a_0 - b_0;
  const float x3 = ((x2) * (x2));
  const float x4 = a_1 - b_1;
  const float x5 = ((x4) * (x4));
  const float x6 = a_2 - b_2;
  const float x7 = ((x6) * (x6));
  const float x8 = x3 + x5 + x7;
  const float x9 = sqrt(x8);
  const float x10 = 2 * d_h;
  const float x11 = x10 / x9;
  const float x12 = x0 - x1 - x11 * x2;
  const float x13 = 2 * b_1;
  const float x14 = 2 * b_2;
  const float x15 = ((a_0) * (a_0)) - a_0 * x1 + ((a_1) * (a_1)) - a_1 * x13
                    + ((a_2) * (a_2)) - a_2 * x14 + ((b_0) * (b_0))
                    + ((b_1) * (b_1)) + ((b_2) * (b_2));
  const float x16 = 1.0 / x15;
  const float x17 = x0 - x1;
  const float x18 = x16 * x17;
  const float x19 = ((d_h) * (d_h)) - x10 * x9 + x8;
  const float x20 = x16 * x19;
  const float x21 = -log(d_h) + (1.0 / 2.0) * log(x15);
  const float x22 = -x2;
  const float x23 = x10 / pow(x8, 3.0 / 2.0);
  const float x24 = x2 * x23;
  const float x25 = x11 - 2;
  const float x26 = -x17;
  const float x27 = (1.0 / 2.0) * x19 / ((x15) * (x15));
  const float x28 = x17 * x27;
  const float x29 = x20 + x21 * (-x22 * x24 - x25) + x26 * x28;
  const float x30 = -x4;
  const float x31 = x21 * x24;
  const float x32 = 2 * a_1;
  const float x33 = -x11 * x4 - x13 + x32;
  const float x34 = (1.0 / 2.0) * x18;
  const float x35 = -x13 + x32;
  const float x36 = x16 * x35;
  const float x37 = (1.0 / 2.0) * x12;
  const float x38 = -x35;
  const float x39 = x28 * x38 - x30 * x31 + x33 * x34 + x36 * x37;
  const float x40 = -x6;
  const float x41 = 2 * a_2;
  const float x42 = -x11 * x6 - x14 + x41;
  const float x43 = -x14 + x41;
  const float x44 = x16 * x43;
  const float x45 = -x43;
  const float x46 = x28 * x45 - x31 * x40 + x34 * x42 + x37 * x44;
  const float x47 = x16 * x26;
  const float x48 = -x11 * x22 - x17;
  const float x49 = -x20;
  const float x50 = 2 - x11;
  const float x51 = ((x17) * (x17)) * x27 + x21 * (-x23 * x3 - x50) + x34 * x48
                    + x37 * x47 + x49;
  const float x52 = x16 * x38;
  const float x53 = -x11 * x30 - x35;
  const float x54 = x28 * x35 - x31 * x4;
  const float x55 = x34 * x53 + x37 * x52 + x54;
  const float x56 = x16 * x45;
  const float x57 = -x11 * x40 - x43;
  const float x58 = x28 * x43 - x31 * x6;
  const float x59 = x34 * x57 + x37 * x56 + x58;
  const float x60 = x23 * x4;
  const float x61 = x27 * x35;
  const float x62 = x20 + x21 * (-x25 - x30 * x60) + x38 * x61;
  const float x63 = x21 * x60;
  const float x64 = (1.0 / 2.0) * x36;
  const float x65 = (1.0 / 2.0) * x33;
  const float x66 = -x40 * x63 + x42 * x64 + x44 * x65 + x45 * x61;
  const float x67 = x47 * x65 + x48 * x64 + x54;
  const float x68 = x21 * (-x23 * x5 - x50) + x27 * ((x35) * (x35)) + x49
                    + x52 * x65 + x53 * x64;
  const float x69 = x43 * x61 - x6 * x63;
  const float x70 = x56 * x65 + x57 * x64 + x69;
  const float x71 = x23 * x6;
  const float x72 = x27 * x43;
  const float x73 = x20 + x21 * (-x25 - x40 * x71) + x45 * x72;
  const float x74 = (1.0 / 2.0) * x42;
  const float x75 = (1.0 / 2.0) * x44;
  const float x76 = x47 * x74 + x48 * x75 + x58;
  const float x77 = x52 * x74 + x53 * x75 + x69;
  const float x78 = x21 * (-x23 * x7 - x50) + x27 * ((x43) * (x43)) + x49
                    + x56 * x74 + x57 * x75;
  const float x79 = (1.0 / 2.0) * x47;
  const float x80 = (1.0 / 2.0) * x48;
  const float x81 = -x22 * x63 + x26 * x61 + x52 * x80 + x53 * x79;
  const float x82 = x21 * x71;
  const float x83 = -x22 * x82 + x26 * x72 + x56 * x80 + x57 * x79;
  const float x84 = -x30 * x82 + x38 * x72 + (1.0 / 2.0) * x52 * x57
                    + (1.0 / 2.0) * x53 * x56;

  out_8361029790532263509[0] = x12 * x18 + x29;
  out_8361029790532263509[1] = x39;
  out_8361029790532263509[2] = x46;
  out_8361029790532263509[3] = x51;
  out_8361029790532263509[4] = x55;
  out_8361029790532263509[5] = x59;
  out_8361029790532263509[6] = x39;
  out_8361029790532263509[7] = x33 * x36 + x62;
  out_8361029790532263509[8] = x66;
  out_8361029790532263509[9] = x67;
  out_8361029790532263509[10] = x68;
  out_8361029790532263509[11] = x70;
  out_8361029790532263509[12] = x46;
  out_8361029790532263509[13] = x66;
  out_8361029790532263509[14] = x42 * x44 + x73;
  out_8361029790532263509[15] = x76;
  out_8361029790532263509[16] = x77;
  out_8361029790532263509[17] = x78;
  out_8361029790532263509[18] = x51;
  out_8361029790532263509[19] = x67;
  out_8361029790532263509[20] = x76;
  out_8361029790532263509[21] = x29 + x47 * x48;
  out_8361029790532263509[22] = x81;
  out_8361029790532263509[23] = x83;
  out_8361029790532263509[24] = x55;
  out_8361029790532263509[25] = x68;
  out_8361029790532263509[26] = x77;
  out_8361029790532263509[27] = x81;
  out_8361029790532263509[28] = x52 * x53 + x62;
  out_8361029790532263509[29] = x84;
  out_8361029790532263509[30] = x59;
  out_8361029790532263509[31] = x70;
  out_8361029790532263509[32] = x78;
  out_8361029790532263509[33] = x83;
  out_8361029790532263509[34] = x84;
  out_8361029790532263509[35] = x56 * x57 + x73;
}
