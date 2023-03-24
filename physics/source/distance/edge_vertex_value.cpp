/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'acgipc'                        *
 ******************************************************************************/
#include "aphysics/ccd/distances/edge_vertex_value.hpp"

#include <math.h>

double edge_vertex_value(double a_0, double a_1, double a_2, double b_0,
                         double b_1, double b_2, double c_0, double c_1,
                         double c_2, double d_h) {
  const double x0 = a_0 - b_0;
  const double x1 = a_1 - b_1;
  const double x2 = a_2 - b_2;
  const double x3 = ((x0) * (x0)) + ((x1) * (x1)) + ((x2) * (x2));
  const double x4 = a_1 - c_1;
  const double x5 = a_0 - c_0;
  const double x6 = a_2 - c_2;
  const double x7 = ((x0 * x4 - x1 * x5) * (x0 * x4 - x1 * x5))
                    + ((x0 * x6 - x2 * x5) * (x0 * x6 - x2 * x5))
                    + ((x1 * x6 - x2 * x4) * (x1 * x6 - x2 * x4));
  const double x8 = ((a_0) * (a_0));
  const double x9 = ((a_1) * (a_1));
  const double x10 = ((a_2) * (a_2));
  const double x11 = ((b_0) * (b_0));
  const double x12 = ((b_1) * (b_1));
  const double x13 = ((b_2) * (b_2));
  const double x14 = 2 * a_0;
  const double x15 = b_0 * x14;
  const double x16 = 2 * a_1;
  const double x17 = b_1 * x16;
  const double x18 = 2 * a_2;
  const double x19 = b_2 * x18;
  const double x20 = a_1 * x15;
  const double x21 = c_0 * x14;
  const double x22 = a_1 * x21;
  const double x23 = a_2 * b_2;
  const double x24 = a_2 * c_2;
  const double x25 = b_1 * c_1;
  const double x26 = b_2 * c_2;
  const double x27 = c_1 * x16;
  const double x28 = b_0 * c_0;
  const double x29 = c_2 * x18;
  const double x30 = 2 * x28;
  const double x31 = ((c_1) * (c_1));
  const double x32 = ((c_2) * (c_2));
  const double x33 = ((c_0) * (c_0));
  const double x34 = 2 * x8;
  const double x35 = 2 * x9;
  const double x36 = 2 * x10;

  double edge_vertex_value_result;
  edge_vertex_value_result
      = (-2 * log(d_h)
         + log((-b_1 * x20 + b_1 * x22 + c_1 * x20 - c_1 * x22 + x10 * x11
                + x10 * x12 + x10 * x31 + x10 * x33 - x11 * x27 - x11 * x29
                + x11 * x31 + x11 * x32 + x11 * x9 - x12 * x21 - x12 * x29
                + x12 * x32 + x12 * x33 + x12 * x8 - x13 * x21 - x13 * x27
                + x13 * x31 + x13 * x33 + x13 * x8 + x13 * x9 - x15 * x23
                + x15 * x24 + x15 * x25 + x15 * x26 - x15 * x31 - x15 * x32
                - x17 * x23 + x17 * x24 + x17 * x26 + x17 * x28 - x17 * x32
                - x17 * x33 + x19 * x25 + x19 * x28 - x19 * x31 - x19 * x33
                + x21 * x23 - x21 * x24 + x21 * x25 + x21 * x26 + x23 * x27
                - x24 * x27 - 2 * x25 * x26 + x25 * x29 - x25 * x30 - x25 * x34
                - x25 * x36 + x26 * x27 - x26 * x30 - x26 * x34 - x26 * x35
                + x27 * x28 + x28 * x29 - x28 * x35 - x28 * x36 + x31 * x8
                + x32 * x8 + x32 * x9 + x33 * x9)
               / (x10 + x11 + x12 + x13 - x15 - x17 - x19 + x8 + x9)))
        * ((1.0 / 2.0) * ((d_h) * (d_h)) - d_h * sqrt(x7) / sqrt(x3)
           + (1.0 / 2.0) * x7 / x3);
  return edge_vertex_value_result;
}

void edge_vertex_grad(double a_0, double a_1, double a_2, double b_0,
                      double b_1, double b_2, double c_0, double c_1,
                      double c_2, double d_h, double *out_5905569529076117828) {
  const double x0 = ((a_0) * (a_0));
  const double x1 = ((a_1) * (a_1));
  const double x2 = ((a_2) * (a_2));
  const double x3 = ((b_0) * (b_0));
  const double x4 = ((b_1) * (b_1));
  const double x5 = ((b_2) * (b_2));
  const double x6 = 2 * a_0;
  const double x7 = b_0 * x6;
  const double x8 = 2 * b_1;
  const double x9 = a_1 * x8;
  const double x10 = 2 * b_2;
  const double x11 = a_2 * x10;
  const double x12 = x0 + x1 - x11 + x2 + x3 + x4 + x5 - x7 - x9;
  const double x13 = 1.0 / x12;
  const double x14 = b_0 * x9;
  const double x15 = 2 * c_1;
  const double x16 = a_1 * x15;
  const double x17 = b_0 * x16;
  const double x18 = c_0 * x9;
  const double x19 = c_0 * x16;
  const double x20 = b_0 * x11;
  const double x21 = 2 * c_2;
  const double x22 = a_2 * x21;
  const double x23 = b_0 * x22;
  const double x24 = c_0 * x11;
  const double x25 = c_0 * x22;
  const double x26 = b_1 * x15;
  const double x27 = b_0 * x26;
  const double x28 = b_2 * x21;
  const double x29 = b_0 * x28;
  const double x30 = c_0 * x26;
  const double x31 = c_0 * x28;
  const double x32 = b_2 * x9;
  const double x33 = c_2 * x9;
  const double x34 = b_2 * x16;
  const double x35 = c_2 * x16;
  const double x36 = b_2 * c_2;
  const double x37 = b_2 * x26;
  const double x38 = c_2 * x26;
  const double x39 = ((c_1) * (c_1));
  const double x40 = ((c_2) * (c_2));
  const double x41 = ((c_0) * (c_0));
  const double x42 = x4 * x6;
  const double x43 = x5 * x6;
  const double x44 = x0 * x15;
  const double x45 = x0 * x21;
  const double x46 = x15 * x3;
  const double x47 = x15 * x5;
  const double x48 = 2 * b_0;
  const double x49 = x1 * x48;
  const double x50 = x1 * x21;
  const double x51 = x21 * x3;
  const double x52 = x21 * x4;
  const double x53 = x2 * x48;
  const double x54 = x15 * x2;
  const double x55
      = -a_0 * x14 + a_0 * x17 + a_0 * x18 - a_0 * x19 - a_0 * x20 + a_0 * x23
        + a_0 * x24 - a_0 * x25 + a_0 * x27 + a_0 * x29 + a_0 * x30 + a_0 * x31
        - a_1 * x46 - a_1 * x47 - a_2 * x32 + a_2 * x33 + a_2 * x34 - a_2 * x35
        + a_2 * x37 + a_2 * x38 - a_2 * x51 - a_2 * x52 + b_0 * x19 + b_0 * x25
        - b_1 * x44 - b_1 * x54 - b_2 * x45 - b_2 * x50 + c_0 * x14 + c_0 * x20
        - c_0 * x27 - c_0 * x29 - c_0 * x42 - c_0 * x43 - c_0 * x49 - c_0 * x53
        + x0 * x39 + x0 * x4 + x0 * x40 + x0 * x5 + x1 * x3 + x1 * x40
        + x1 * x41 + x1 * x5 - x11 * x39 - x11 * x41 + x16 * x36 + x2 * x3
        + x2 * x39 + x2 * x4 + x2 * x41 - x26 * x36 + x3 * x39 + x3 * x40
        + x36 * x9 + x39 * x5 - x39 * x7 + x4 * x40 + x4 * x41 - x40 * x7
        - x40 * x9 + x41 * x5 - x41 * x9;
  const double x56 = -2 * log(d_h) + log(x13 * x55);
  const double x57 = a_0 - b_0;
  const double x58 = a_1 - b_1;
  const double x59 = a_2 - b_2;
  const double x60 = ((x57) * (x57)) + ((x58) * (x58)) + ((x59) * (x59));
  const double x61 = 1.0 / x60;
  const double x62 = -x15;
  const double x63 = x62 + x8;
  const double x64 = a_1 - c_1;
  const double x65 = a_0 - c_0;
  const double x66 = x57 * x64 - x58 * x65;
  const double x67 = x63 * x66;
  const double x68 = -x21;
  const double x69 = x10 + x68;
  const double x70 = a_2 - c_2;
  const double x71 = x57 * x70 - x59 * x65;
  const double x72 = x69 * x71;
  const double x73 = -x48 + x6;
  const double x74 = -x73;
  const double x75 = pow(x60, -2);
  const double x76 = x58 * x70 - x59 * x64;
  const double x77 = ((x66) * (x66)) + ((x71) * (x71)) + ((x76) * (x76));
  const double x78 = sqrt(x77);
  const double x79 = d_h * x78;
  const double x80 = x79 / pow(x60, 3.0 / 2.0);
  const double x81 = pow(x60, -1.0 / 2.0);
  const double x82 = d_h * x81 / x78;
  const double x83 = x39 * x48;
  const double x84 = x40 * x48;
  const double x85 = 2 * c_0;
  const double x86 = x4 * x85;
  const double x87 = x5 * x85;
  const double x88 = x39 * x6;
  const double x89 = x40 * x6;
  const double x90 = 4 * a_0;
  const double x91 = b_1 * c_1;
  const double x92 = x18 + x24;
  const double x93 = x17 + x23;
  const double x94 = x55 / ((x12) * (x12));
  const double x95
      = ((1.0 / 2.0) * ((d_h) * (d_h)) + (1.0 / 2.0) * x61 * x77 - x79 * x81)
        / x55;
  const double x96 = x12 * x95;
  const double x97 = -x85;
  const double x98 = -x48 - x97;
  const double x99 = x66 * x98;
  const double x100 = x69 * x76;
  const double x101 = 2 * a_1;
  const double x102 = x101 - x8;
  const double x103 = -x102;
  const double x104 = x41 * x8;
  const double x105 = x40 * x8;
  const double x106 = x101 * x3;
  const double x107 = x101 * x5;
  const double x108 = x101 * x41;
  const double x109 = x101 * x40;
  const double x110 = 4 * a_1;
  const double x111 = b_0 * c_0;
  const double x112 = a_0 * b_0;
  const double x113 = x112 * x8;
  const double x114 = a_0 * c_0;
  const double x115 = x114 * x15;
  const double x116 = a_2 * x8;
  const double x117 = b_2 * x116;
  const double x118 = a_2 * x15;
  const double x119 = c_2 * x118;
  const double x120 = x111 * x8;
  const double x121 = x111 * x15;
  const double x122 = x36 * x8;
  const double x123 = x15 * x36;
  const double x124 = b_2 * x118 + x112 * x15;
  const double x125 = c_2 * x116 + x114 * x8;
  const double x126 = x71 * x98;
  const double x127 = -x63 * x76;
  const double x128 = 2 * a_2;
  const double x129 = -2 * b_2 + x128;
  const double x130 = -x129;
  const double x131 = x10 * x41;
  const double x132 = x10 * x39;
  const double x133 = x128 * x3;
  const double x134 = x128 * x4;
  const double x135 = x128 * x41;
  const double x136 = x128 * x39;
  const double x137 = 4 * a_2;
  const double x138 = x10 * x112;
  const double x139 = x114 * x21;
  const double x140 = x10 * x111;
  const double x141 = x111 * x21;
  const double x142 = x112 * x21 + x33;
  const double x143 = x10 * x114 + x34;
  const double x144 = x101 + x62;
  const double x145 = -x144 * x66;
  const double x146 = -x128 - x68;
  const double x147 = x146 * x71;
  const double x148 = x1 * x85;
  const double x149 = x2 * x85;
  const double x150 = c_1 * x110;
  const double x151 = c_2 * x137;
  const double x152 = a_0 * x9;
  const double x153 = a_0 * x11;
  const double x154 = a_0 * x16;
  const double x155 = a_0 * x22;
  const double x156 = a_0 * x26 + a_0 * x28;
  const double x157 = x6 + x97;
  const double x158 = x157 * x66;
  const double x159 = x146 * x76;
  const double x160 = x0 * x8;
  const double x161 = x2 * x8;
  const double x162 = c_0 * x90;
  const double x163 = a_1 * x7;
  const double x164 = a_1 * x11;
  const double x165 = a_1 * c_0;
  const double x166 = x165 * x6;
  const double x167 = a_1 * x22;
  const double x168 = a_1 * x28 + x165 * x48;
  const double x169 = x157 * x71;
  const double x170 = x144 * x76;
  const double x171 = x0 * x10;
  const double x172 = x1 * x10;
  const double x173 = a_2 * x7;
  const double x174 = a_2 * x9;
  const double x175 = a_2 * c_0;
  const double x176 = x175 * x6;
  const double x177 = a_2 * x16;
  const double x178 = a_2 * x26 + x175 * x48;
  const double x179 = x102 * x66;
  const double x180 = x129 * x71;
  const double x181 = b_1 * x110;
  const double x182 = b_2 * x137;
  const double x183 = x66 * x74;
  const double x184 = x129 * x76;
  const double x185 = b_0 * x90;
  const double x186 = x71 * x74;
  const double x187 = x103 * x76;

  out_5905569529076117828[0]
      = x56
            * (x57 * x80 + (1.0 / 2.0) * x61 * (x67 + x72)
               + (1.0 / 2.0) * x74 * x75 * x77
               - x82 * ((1.0 / 2.0) * x67 + (1.0 / 2.0) * x72))
        + x96
              * (x13
                     * (-x14 - x19 - x20 - x25 + x27 + x29 + x30 + x31
                        - x36 * x90 + x42 + x43 - x83 - x84 - x86 - x87 + x88
                        + x89 - x90 * x91 + x92 + x93)
                 + x74 * x94);
  out_5905569529076117828[1]
      = x56
            * ((1.0 / 2.0) * x103 * x75 * x77 + x58 * x80
               + (1.0 / 2.0) * x61 * (x100 + x99)
               - x82 * ((1.0 / 2.0) * x100 + (1.0 / 2.0) * x99))
        + x96
              * (x103 * x94
                 + x13
                       * (-x104 - x105 + x106 + x107 + x108 + x109 - x110 * x111
                          - x110 * x36 - x113 - x115 - x117 - x119 + x120 + x121
                          + x122 + x123 + x124 + x125 - x46 - x47));
  out_5905569529076117828[2]
      = x56
            * ((1.0 / 2.0) * x130 * x75 * x77 + x59 * x80
               + (1.0 / 2.0) * x61 * (x126 + x127)
               - x82 * ((1.0 / 2.0) * x126 + (1.0 / 2.0) * x127))
        + x96
              * (x13
                     * (-x111 * x137 - x131 - x132 + x133 + x134 + x135 + x136
                        - x137 * x91 - x138 - x139 + x140 + x141 + x142 + x143
                        - x32 - x35 + x37 + x38 - x51 - x52)
                 + x130 * x94);
  out_5905569529076117828[3]
      = x56
            * (-x57 * x80 + (1.0 / 2.0) * x61 * (x145 + x147)
               + (1.0 / 2.0) * x73 * x75 * x77
               - x82 * ((1.0 / 2.0) * x145 + (1.0 / 2.0) * x147))
        + x96
              * (x13
                     * (-b_0 * x150 - b_0 * x151 - x148 - x149 - x152 - x153
                        + x154 + x155 + x156 + x19 + x25 - x30 - x31 + x49 + x53
                        + x83 + x84 - x88 - x89 + x92)
                 + x73 * x94);
  out_5905569529076117828[4]
      = x56
            * ((1.0 / 2.0) * x102 * x75 * x77 - x58 * x80
               + (1.0 / 2.0) * x61 * (x158 + x159)
               - x82 * ((1.0 / 2.0) * x158 + (1.0 / 2.0) * x159))
        + x96
              * (x102 * x94
                 + x13
                       * (-b_1 * x151 - b_1 * x162 + x104 + x105 - x108 - x109
                          + x115 + x119 - x121 - x123 + x124 + x160 + x161
                          - x163 - x164 + x166 + x167 + x168 - x44 - x54));
  out_5905569529076117828[5]
      = x56
            * ((1.0 / 2.0) * x129 * x75 * x77 - x59 * x80
               + (1.0 / 2.0) * x61 * (x169 + x170)
               - x82 * ((1.0 / 2.0) * x169 + (1.0 / 2.0) * x170))
        + x96
              * (x129 * x94
                 + x13
                       * (-b_2 * x150 - b_2 * x162 + x131 + x132 - x135 - x136
                          + x139 - x141 + x142 + x171 + x172 - x173 - x174
                          + x176 + x177 + x178 + x35 - x38 - x45 - x50));
  out_5905569529076117828[6]
      = x56
            * ((1.0 / 2.0) * x61 * (x179 + x180)
               - x82 * ((1.0 / 2.0) * x179 + (1.0 / 2.0) * x180))
        + x95
              * (-c_0 * x181 - c_0 * x182 + x14 + x148 + x149 + x152 + x153
                 - x154 - x155 + x156 + x20 - x27 - x29 - x42 - x43 - x49 - x53
                 + x86 + x87 + x93);
  out_5905569529076117828[7]
      = x56
            * ((1.0 / 2.0) * x61 * (x183 + x184)
               - x82 * ((1.0 / 2.0) * x183 + (1.0 / 2.0) * x184))
        + x95
              * (-c_1 * x182 - c_1 * x185 - x106 - x107 + x113 + x117 - x120
                 - x122 + x125 - x160 - x161 + x163 + x164 - x166 - x167 + x168
                 + x44 + x46 + x47 + x54);
  out_5905569529076117828[8]
      = x56
            * ((1.0 / 2.0) * x61 * (x186 + x187)
               - x82 * ((1.0 / 2.0) * x186 + (1.0 / 2.0) * x187))
        + x95
              * (-c_2 * x181 - c_2 * x185 - x133 - x134 + x138 - x140 + x143
                 - x171 - x172 + x173 + x174 - x176 - x177 + x178 + x32 - x37
                 + x45 + x50 + x51 + x52);
}

void edge_vertex_hessian(double a_0, double a_1, double a_2, double b_0,
                         double b_1, double b_2, double c_0, double c_1,
                         double c_2, double d_h,
                         double *out_2823277736450422735) {
  const double x0 = 2 * a_0;
  const double x1 = 2 * b_0;
  const double x2 = x0 - x1;
  const double x3 = b_0 * x0;
  const double x4 = -x3;
  const double x5 = ((a_0) * (a_0));
  const double x6 = ((a_1) * (a_1));
  const double x7 = ((a_2) * (a_2));
  const double x8 = ((b_0) * (b_0));
  const double x9 = ((b_1) * (b_1));
  const double x10 = ((b_2) * (b_2));
  const double x11 = 2 * b_1;
  const double x12 = a_1 * x11;
  const double x13 = -x12;
  const double x14 = 2 * b_2;
  const double x15 = a_2 * x14;
  const double x16 = -x15;
  const double x17 = x13 + x16;
  const double x18 = x10 + x17 + x4 + x5 + x6 + x7 + x8 + x9;
  const double x19 = 1.0 / x18;
  const double x20 = ((c_1) * (c_1));
  const double x21 = x1 * x20;
  const double x22 = ((c_2) * (c_2));
  const double x23 = x1 * x22;
  const double x24 = 2 * x9;
  const double x25 = c_0 * x24;
  const double x26 = 2 * x10;
  const double x27 = c_0 * x26;
  const double x28 = x0 * x9;
  const double x29 = x0 * x10;
  const double x30 = x0 * x20;
  const double x31 = x0 * x22;
  const double x32 = 4 * a_0;
  const double x33 = c_1 * x32;
  const double x34 = c_2 * x32;
  const double x35 = b_0 * x12;
  const double x36 = 2 * c_1;
  const double x37 = c_0 * x36;
  const double x38 = a_1 * x37;
  const double x39 = b_0 * x15;
  const double x40 = 2 * c_2;
  const double x41 = c_0 * x40;
  const double x42 = a_2 * x41;
  const double x43 = b_0 * x36;
  const double x44 = b_1 * x43;
  const double x45 = b_0 * x40;
  const double x46 = b_2 * x45;
  const double x47 = b_1 * x37;
  const double x48 = b_2 * x41;
  const double x49 = c_0 * x12;
  const double x50 = c_0 * x15;
  const double x51 = x49 + x50;
  const double x52 = a_1 * x43;
  const double x53 = a_2 * x45;
  const double x54 = x52 + x53;
  const double x55 = -b_1 * x33 - b_2 * x34 - x21 - x23 - x25 - x27 + x28 + x29
                     + x30 + x31 - x35 - x38 - x39 - x42 + x44 + x46 + x47 + x48
                     + x51 + x54;
  const double x56 = -x2;
  const double x57 = b_2 * x12;
  const double x58 = c_2 * x12;
  const double x59 = a_1 * x36;
  const double x60 = b_2 * x59;
  const double x61 = c_2 * x59;
  const double x62 = b_1 * x36;
  const double x63 = b_2 * x62;
  const double x64 = c_2 * x62;
  const double x65 = ((c_0) * (c_0));
  const double x66 = b_2 * x40;
  const double x67 = x36 * x8;
  const double x68 = x10 * x36;
  const double x69 = x1 * x6;
  const double x70 = x40 * x8;
  const double x71 = x40 * x9;
  const double x72 = x1 * x7;
  const double x73
      = -a_0 * x35 - a_0 * x38 - a_0 * x39 - a_0 * x42 + a_0 * x44 + a_0 * x46
        + a_0 * x47 + a_0 * x48 + a_0 * x49 + a_0 * x50 + a_0 * x52 + a_0 * x53
        - a_1 * x67 - a_1 * x68 - a_2 * x57 + a_2 * x58 + a_2 * x60 - a_2 * x61
        + a_2 * x63 + a_2 * x64 - a_2 * x70 - a_2 * x71 + b_0 * x38 + b_0 * x42
        - b_0 * x47 - b_0 * x48 + b_2 * x61 - c_0 * x28 - c_0 * x29 + c_0 * x35
        + c_0 * x39 - c_0 * x69 - c_0 * x72 + c_2 * x57 - c_2 * x63 + x10 * x20
        + x10 * x5 + x10 * x6 + x10 * x65 - x12 * x22 - x12 * x65 - x15 * x20
        - x15 * x65 - x20 * x3 + x20 * x5 + x20 * x7 + x20 * x8 - x22 * x3
        + x22 * x5 + x22 * x6 + x22 * x8 + x22 * x9 - x5 * x62 - x5 * x66
        + x5 * x9 + x6 * x65 - x6 * x66 + x6 * x8 - x62 * x7 + x65 * x7
        + x65 * x9 + x7 * x8 + x7 * x9;
  const double x74 = pow(x18, -2);
  const double x75 = x73 * x74;
  const double x76 = x19 * x55 + x56 * x75;
  const double x77 = 1.0 / x73;
  const double x78 = a_0 - b_0;
  const double x79 = -c_1;
  const double x80 = a_1 + x79;
  const double x81 = x78 * x80;
  const double x82 = -c_0;
  const double x83 = a_0 + x82;
  const double x84 = a_1 - b_1;
  const double x85 = x83 * x84;
  const double x86 = x81 - x85;
  const double x87 = -c_2;
  const double x88 = a_2 + x87;
  const double x89 = x78 * x88;
  const double x90 = a_2 - b_2;
  const double x91 = x83 * x90;
  const double x92 = x89 - x91;
  const double x93 = x84 * x88;
  const double x94 = x80 * x90;
  const double x95 = x93 - x94;
  const double x96 = ((x86) * (x86)) + ((x92) * (x92)) + ((x95) * (x95));
  const double x97 = ((x90) * (x90));
  const double x98 = ((x84) * (x84)) + x97;
  const double x99 = ((x78) * (x78)) + x98;
  const double x100 = 1.0 / x99;
  const double x101 = pow(x99, -1.0 / 2.0);
  const double x102 = sqrt(x96);
  const double x103 = d_h * x102;
  const double x104
      = (1.0 / 2.0) * ((d_h) * (d_h)) + (1.0 / 2.0) * x100 * x96 - x101 * x103;
  const double x105 = x104 * x77;
  const double x106 = x105 * x76;
  const double x107 = x18 * x76;
  const double x108 = x104 / ((x73) * (x73));
  const double x109 = x107 * x108;
  const double x110 = -x36;
  const double x111 = x11 + x110;
  const double x112 = x111 * x86;
  const double x113 = -x40;
  const double x114 = x113 + x14;
  const double x115 = x114 * x92;
  const double x116 = x112 + x115;
  const double x117 = pow(x99, -2);
  const double x118 = -x78;
  const double x119 = pow(x99, -3.0 / 2.0);
  const double x120 = x103 * x119;
  const double x121 = (1.0 / 2.0) * x112 + (1.0 / 2.0) * x115;
  const double x122 = 1.0 / x102;
  const double x123 = d_h * x101;
  const double x124 = x122 * x123;
  const double x125
      = x77
        * ((1.0 / 2.0) * x100 * x116 + (1.0 / 2.0) * x117 * x56 * x96
           - x118 * x120 - x121 * x124);
  const double x126 = -2 * log(d_h) + log(x19 * x73);
  const double x127 = b_1 + x79;
  const double x128 = b_2 + x87;
  const double x129 = x114 * x128;
  const double x130 = ((x128) * (x128));
  const double x131 = -4 * b_0 + x32;
  const double x132 = -x131;
  const double x133 = pow(x99, -3);
  const double x134 = 3 * a_0 - 3 * b_0;
  const double x135 = x103 / pow(x99, 5.0 / 2.0);
  const double x136 = x118 * x135;
  const double x137 = x123 / pow(x96, 3.0 / 2.0);
  const double x138 = x121 * x137;
  const double x139 = d_h * x119 * x122;
  const double x140 = x118 * x139;
  const double x141 = x117 * x96;
  const double x142 = -x120 + x141;
  const double x143 = 4 * b_1;
  const double x144 = c_1 * x143;
  const double x145 = -x144;
  const double x146 = 4 * b_2;
  const double x147 = -c_2 * x146;
  const double x148 = 2 * x20;
  const double x149 = 2 * x22;
  const double x150 = x148 + x149;
  const double x151 = x24 + x26;
  const double x152 = x56 * x74;
  const double x153 = 2 * x75;
  const double x154 = -x153;
  const double x155 = x73 / pow(x18, 3);
  const double x156 = x105 * x18;
  const double x157 = 2 * a_1;
  const double x158 = -x11 + x157;
  const double x159 = x11 * x65;
  const double x160 = x11 * x22;
  const double x161 = x157 * x8;
  const double x162 = a_1 * x26;
  const double x163 = x157 * x65;
  const double x164 = a_1 * x149;
  const double x165 = 4 * b_0;
  const double x166 = c_0 * x165;
  const double x167 = 4 * a_1;
  const double x168 = c_2 * x167;
  const double x169 = b_0 * x11;
  const double x170 = a_0 * x169;
  const double x171 = a_0 * x37;
  const double x172 = b_2 * x11;
  const double x173 = a_2 * x172;
  const double x174 = c_2 * x36;
  const double x175 = a_2 * x174;
  const double x176 = c_0 * x169;
  const double x177 = b_0 * x37;
  const double x178 = c_2 * x172;
  const double x179 = b_2 * x174;
  const double x180 = b_2 * x36;
  const double x181 = a_0 * x43 + a_2 * x180;
  const double x182 = c_0 * x11;
  const double x183 = c_2 * x11;
  const double x184 = a_0 * x182 + a_2 * x183;
  const double x185 = -a_1 * x166 - b_2 * x168 - x159 - x160 + x161 + x162
                      + x163 + x164 - x170 - x171 - x173 - x175 + x176 + x177
                      + x178 + x179 + x181 + x184 - x67 - x68;
  const double x186 = -x185;
  const double x187 = -x43;
  const double x188 = -x182;
  const double x189 = -x158;
  const double x190 = x55 * x74;
  const double x191 = -4 * b_1 + x167;
  const double x192 = -x191;
  const double x193 = x155 * x56;
  const double x194 = 2 * c_0;
  const double x195 = -x194;
  const double x196 = -x1 - x195;
  const double x197 = x196 * x86;
  const double x198 = x114 * x95;
  const double x199 = x197 + x198;
  const double x200 = -x84;
  const double x201 = (1.0 / 2.0) * x197 + (1.0 / 2.0) * x198;
  const double x202
      = x77
        * ((1.0 / 2.0) * x100 * x199 + (1.0 / 2.0) * x117 * x189 * x96
           - x120 * x200 - x124 * x201);
  const double x203 = x185 * x19 + x189 * x75;
  const double x204 = x125 * x18;
  const double x205 = -b_0 - x82;
  const double x206 = x124 * x205;
  const double x207 = 3 * a_1 - 3 * b_1;
  const double x208 = -x207;
  const double x209 = x121 * x139;
  const double x210 = -x201;
  const double x211
      = x106 * x158 + x107 * x202 + x109 * x186
        + x126
              * ((1.0 / 2.0) * x100 * x111 * x205
                 + (1.0 / 2.0) * x116 * x117 * x189
                 + (1.0 / 2.0) * x117 * x199 * x56 - x127 * x206
                 + (1.0 / 2.0) * x133 * x192 * x56 * x96 - x136 * x208
                 - x138 * x210 - x140 * x201 - x200 * x209)
        + x156
              * (x152 * x185 + x189 * x190 + x19 * (-x169 - x187 - x188 - x37)
                 + x192 * x193)
        + x203 * x204;
  const double x212 = 2 * a_2;
  const double x213 = -2 * b_2 + x212;
  const double x214 = x14 * x65;
  const double x215 = x14 * x20;
  const double x216 = x212 * x8;
  const double x217 = a_2 * x24;
  const double x218 = x212 * x65;
  const double x219 = a_2 * x148;
  const double x220 = b_0 * x14;
  const double x221 = a_0 * x220;
  const double x222 = a_0 * x41;
  const double x223 = c_0 * x220;
  const double x224 = b_0 * x41;
  const double x225 = a_0 * x45 + x58;
  const double x226 = c_0 * x14;
  const double x227 = a_0 * x226 + x60;
  const double x228 = -a_2 * x144 - a_2 * x166 - x214 - x215 + x216 + x217
                      + x218 + x219 - x221 - x222 + x223 + x224 + x225 + x227
                      - x57 - x61 + x63 + x64 - x70 - x71;
  const double x229 = -x228;
  const double x230 = -x45;
  const double x231 = -x226;
  const double x232 = -x213;
  const double x233 = 4 * a_2;
  const double x234 = -4 * b_2 + x233;
  const double x235 = -x234;
  const double x236 = x196 * x92;
  const double x237 = -x111;
  const double x238 = x237 * x95;
  const double x239 = x236 + x238;
  const double x240 = -x90;
  const double x241 = (1.0 / 2.0) * x236 + (1.0 / 2.0) * x238;
  const double x242
      = x77
        * ((1.0 / 2.0) * x100 * x239 + (1.0 / 2.0) * x117 * x232 * x96
           - x120 * x240 - x124 * x241);
  const double x243 = x19 * x228 + x232 * x75;
  const double x244 = 3 * a_2 - 3 * b_2;
  const double x245 = -x244;
  const double x246 = -x241;
  const double x247
      = x106 * x213 + x107 * x242 + x109 * x229
        + x126
              * ((1.0 / 2.0) * x100 * x114 * x205
                 + (1.0 / 2.0) * x116 * x117 * x232
                 + (1.0 / 2.0) * x117 * x239 * x56 - x128 * x206
                 + (1.0 / 2.0) * x133 * x235 * x56 * x96 - x136 * x245
                 - x138 * x246 - x140 * x241 - x209 * x240)
        + x156
              * (x152 * x228 + x19 * (-x220 - x230 - x231 - x41) + x190 * x232
                 + x193 * x235)
        + x204 * x243;
  const double x248 = x194 * x6;
  const double x249 = x194 * x7;
  const double x250 = c_1 * x165;
  const double x251 = c_2 * x165;
  const double x252 = a_0 * x12;
  const double x253 = a_0 * x15;
  const double x254 = a_0 * x59;
  const double x255 = a_2 * x40;
  const double x256 = a_0 * x255;
  const double x257 = a_0 * x62 + a_0 * x66;
  const double x258 = -a_1 * x250 - a_2 * x251 + x21 + x23 - x248 - x249 - x252
                      - x253 + x254 + x256 + x257 - x30 - x31 + x38 + x42 - x47
                      - x48 + x51 + x69 + x72;
  const double x259 = -x258;
  const double x260 = x110 + x157;
  const double x261 = -x260;
  const double x262 = x261 * x86;
  const double x263 = -x113 - x212;
  const double x264 = x263 * x92;
  const double x265 = x262 + x264;
  const double x266 = (1.0 / 2.0) * x262 + (1.0 / 2.0) * x264;
  const double x267
      = x77
        * ((1.0 / 2.0) * x100 * x265 + (1.0 / 2.0) * x117 * x2 * x96
           - x120 * x78 - x124 * x266);
  const double x268 = x19 * x258 + x2 * x75;
  const double x269 = -x80;
  const double x270 = -x88;
  const double x271 = x114 * x270;
  const double x272 = x128 * x270;
  const double x273 = -x266;
  const double x274 = x120 - x141;
  const double x275 = -x255;
  const double x276 = x15 + x275;
  const double x277 = x276 - x66;
  const double x278 = -x59;
  const double x279 = x12 + x278;
  const double x280 = x279 - x62;
  const double x281
      = x106 * x56 + x107 * x267 + x109 * x259
        + x126
              * ((1.0 / 2.0) * x100 * (x111 * x269 + x271)
                 + (1.0 / 2.0) * x116 * x117 * x2
                 + (1.0 / 2.0) * x117 * x265 * x56 - x124 * (x127 * x269 + x272)
                 + (1.0 / 2.0) * x131 * x133 * x56 * x96 - x134 * x136
                 - x138 * x273 - x140 * x266 - x209 * x78 - x274)
        + x156
              * (x131 * x193 + x152 * x258 + x153 + x19 * (-x150 - x277 - x280)
                 + x190 * x2)
        + x204 * x268;
  const double x282 = x36 * x5;
  const double x283 = x36 * x7;
  const double x284 = x11 * x5;
  const double x285 = x11 * x7;
  const double x286 = b_1 * x32;
  const double x287 = c_2 * x143;
  const double x288 = a_1 * x3;
  const double x289 = a_1 * x15;
  const double x290 = c_0 * x0;
  const double x291 = a_1 * x290;
  const double x292 = a_1 * x255;
  const double x293 = a_1 * x1;
  const double x294 = a_1 * x66 + c_0 * x293;
  const double x295 = -a_2 * x287 - c_0 * x286 + x159 + x160 - x163 - x164
                      + x171 + x175 - x177 - x179 + x181 - x282 - x283 + x284
                      + x285 - x288 - x289 + x291 + x292 + x294;
  const double x296 = -x295;
  const double x297 = -x293;
  const double x298 = c_0 * x143;
  const double x299 = -x298 + x43;
  const double x300 = a_1 * x194;
  const double x301 = x300 - x33;
  const double x302 = x0 + x195;
  const double x303 = x302 * x86;
  const double x304 = x263 * x95;
  const double x305 = x303 + x304;
  const double x306 = (1.0 / 2.0) * x303 + (1.0 / 2.0) * x304;
  const double x307
      = x77
        * ((1.0 / 2.0) * x100 * x305 + (1.0 / 2.0) * x117 * x158 * x96
           - x120 * x84 - x124 * x306);
  const double x308 = x158 * x75 + x19 * x295;
  const double x309 = 2 * x85;
  const double x310 = -x309;
  const double x311 = 2 * x81;
  const double x312 = x310 + x311;
  const double x313 = -x306;
  const double x314
      = x106 * x189 + x107 * x307 + x109 * x296
        + x126
              * ((1.0 / 2.0) * x100 * (x111 * x83 + x312)
                 + (1.0 / 2.0) * x116 * x117 * x158
                 + (1.0 / 2.0) * x117 * x305 * x56 - x124 * (x127 * x83 + x86)
                 + (1.0 / 2.0) * x133 * x191 * x56 * x96 - x136 * x207
                 - x138 * x313 - x140 * x306 - x209 * x84)
        + x156
              * (x152 * x295 + x158 * x190
                 + x19 * (x286 + x297 + x299 + x301 + x37) + x191 * x193)
        + x204 * x308;
  const double x315 = x40 * x5;
  const double x316 = x40 * x6;
  const double x317 = x14 * x5;
  const double x318 = x14 * x6;
  const double x319 = b_2 * x32;
  const double x320 = b_2 * x167;
  const double x321 = a_2 * x3;
  const double x322 = a_2 * x12;
  const double x323 = a_2 * x290;
  const double x324 = a_2 * x59;
  const double x325 = a_2 * x1;
  const double x326 = a_2 * x62 + c_0 * x325;
  const double x327 = -c_0 * x319 - c_1 * x320 + x214 + x215 - x218 - x219
                      + x222 - x224 + x225 - x315 - x316 + x317 + x318 - x321
                      - x322 + x323 + x324 + x326 + x61 - x64;
  const double x328 = -x327;
  const double x329 = -x325;
  const double x330 = c_0 * x146;
  const double x331 = -x330 + x45;
  const double x332 = a_2 * x194;
  const double x333 = x332 - x34;
  const double x334 = x302 * x92;
  const double x335 = x260 * x95;
  const double x336 = x334 + x335;
  const double x337 = (1.0 / 2.0) * x334 + (1.0 / 2.0) * x335;
  const double x338
      = x77
        * ((1.0 / 2.0) * x100 * x336 + (1.0 / 2.0) * x117 * x213 * x96
           - x120 * x90 - x124 * x337);
  const double x339 = x19 * x327 + x213 * x75;
  const double x340 = 2 * x91;
  const double x341 = -x340;
  const double x342 = 2 * x89;
  const double x343 = x341 + x342;
  const double x344 = -x337;
  const double x345
      = x106 * x232 + x107 * x338 + x109 * x328
        + x126
              * ((1.0 / 2.0) * x100 * (x114 * x83 + x343)
                 + (1.0 / 2.0) * x116 * x117 * x213
                 + (1.0 / 2.0) * x117 * x336 * x56 - x124 * (x128 * x83 + x92)
                 + (1.0 / 2.0) * x133 * x234 * x56 * x96 - x136 * x244
                 - x138 * x344 - x140 * x337 - x209 * x90)
        + x156
              * (x152 * x327 + x19 * (x319 + x329 + x331 + x333 + x41)
                 + x190 * x213 + x193 * x234)
        + x204 * x339;
  const double x346 = x62 + x66;
  const double x347 = -x26 + x276;
  const double x348 = -x24 + x279;
  const double x349 = c_0 * x167;
  const double x350 = c_0 * x233;
  const double x351 = -b_1 * x349 - b_2 * x350 + x248 + x249 + x25 + x252 + x253
                      - x254 - x256 + x257 + x27 - x28 - x29 + x35 + x39 - x44
                      - x46 + x54 - x69 - x72;
  const double x352 = x114 * x90;
  const double x353 = x158 * x86;
  const double x354 = x213 * x92;
  const double x355 = x353 + x354;
  const double x356 = x128 * x90;
  const double x357 = (1.0 / 2.0) * x353 + (1.0 / 2.0) * x354;
  const double x358 = -x357;
  const double x359 = (1.0 / 2.0) * x100 * x355 - x124 * x357;
  const double x360 = x359 * x77;
  const double x361 = -x351;
  const double x362
      = x107 * x360 + x109 * x361 + x125 * x351
        + x126
              * ((1.0 / 2.0) * x100 * (x111 * x84 + x352)
                 + (1.0 / 2.0) * x117 * x355 * x56 - x124 * (x127 * x84 + x356)
                 - x138 * x358 - x140 * x357)
        + x156 * (x152 * x351 + x19 * (x346 + x347 + x348));
  const double x363 = -x300;
  const double x364 = x182 - x250;
  const double x365 = -x286 + x293;
  const double x366 = c_1 * x233;
  const double x367 = -b_0 * x33 - b_2 * x366 - x161 - x162 + x170 + x173 - x176
                      - x178 + x184 + x282 + x283 - x284 - x285 + x288 + x289
                      - x291 - x292 + x294 + x67 + x68;
  const double x368 = x309 - x311;
  const double x369 = x56 * x86;
  const double x370 = x213 * x95;
  const double x371 = x369 + x370;
  const double x372 = -x81 + x85;
  const double x373 = (1.0 / 2.0) * x369 + (1.0 / 2.0) * x370;
  const double x374 = -x373;
  const double x375 = (1.0 / 2.0) * x100 * x371 - x124 * x373;
  const double x376 = x375 * x77;
  const double x377 = -x367;
  const double x378
      = x107 * x376 + x109 * x377 + x125 * x367
        + x126
              * ((1.0 / 2.0) * x100 * (x111 * x118 + x368)
                 + (1.0 / 2.0) * x117 * x371 * x56 - x124 * (x118 * x127 + x372)
                 - x138 * x374 - x140 * x373)
        + x156 * (x152 * x367 + x19 * (x169 + x33 + x363 + x364 + x365));
  const double x379 = -x332;
  const double x380 = x226 - x251;
  const double x381 = -x319 + x325;
  const double x382 = -b_0 * x34 - b_1 * x168 - x216 - x217 + x221 - x223 + x227
                      + x315 + x316 - x317 - x318 + x321 + x322 - x323 - x324
                      + x326 + x57 - x63 + x70 + x71;
  const double x383 = x340 - x342;
  const double x384 = x56 * x92;
  const double x385 = x189 * x95;
  const double x386 = x384 + x385;
  const double x387 = -x89 + x91;
  const double x388 = (1.0 / 2.0) * x384 + (1.0 / 2.0) * x385;
  const double x389 = -x388;
  const double x390 = (1.0 / 2.0) * x100 * x386 - x124 * x388;
  const double x391 = x390 * x77;
  const double x392 = -x382;
  const double x393 = x108 * x392;
  const double x394
      = x107 * x391 + x107 * x393 + x125 * x382
        + x126
              * ((1.0 / 2.0) * x100 * (x114 * x118 + x383)
                 + (1.0 / 2.0) * x117 * x386 * x56 - x124 * (x118 * x128 + x387)
                 - x138 * x389 - x140 * x388)
        + x156 * (x152 * x382 + x19 * (x220 + x34 + x379 + x380 + x381));
  const double x395 = x105 * x203;
  const double x396 = x18 * x203;
  const double x397 = x108 * x396;
  const double x398 = x196 * x205;
  const double x399 = ((x205) * (x205));
  const double x400 = x135 * x200;
  const double x401 = x137 * x201;
  const double x402 = x139 * x200;
  const double x403 = -x166;
  const double x404 = 2 * x65;
  const double x405 = x149 + x404;
  const double x406 = 2 * x8;
  const double x407 = x26 + x406;
  const double x408 = x189 * x74;
  const double x409 = x155 * x189;
  const double x410 = -x183;
  const double x411 = -x180;
  const double x412 = x185 * x74;
  const double x413 = x18 * x202;
  const double x414 = -x127;
  const double x415 = x139 * x201;
  const double x416
      = x126
            * ((1.0 / 2.0) * x100 * x114 * x414
               + (1.0 / 2.0) * x117 * x189 * x239
               + (1.0 / 2.0) * x117 * x199 * x232 - x124 * x128 * x414
               + (1.0 / 2.0) * x133 * x189 * x235 * x96 - x240 * x415
               - x241 * x402 - x245 * x400 - x246 * x401)
        + x156
              * (x19 * (-x172 - x174 - x410 - x411) + x228 * x408 + x232 * x412
                 + x235 * x409)
        + x213 * x395 + x229 * x397 + x242 * x396 + x243 * x413;
  const double x417 = a_0 * x11;
  const double x418 = -x417;
  const double x419 = a_1 * x165;
  const double x420 = a_0 * x36;
  const double x421 = -x349 + x420;
  const double x422
      = x126
            * ((1.0 / 2.0) * x100 * (x196 * x269 + x368)
               + (1.0 / 2.0) * x117 * x189 * x265
               + (1.0 / 2.0) * x117 * x199 * x2 - x124 * (x205 * x269 + x372)
               + (1.0 / 2.0) * x131 * x133 * x189 * x96 - x134 * x400
               - x266 * x402 - x273 * x401 - x415 * x78)
        + x156
              * (x131 * x409 + x19 * (x364 + x37 + x418 + x419 + x421)
                 + x2 * x412 + x258 * x408)
        + x259 * x397 + x267 * x396 + x268 * x413 + x395 * x56;
  const double x423 = x196 * x83;
  const double x424 = x205 * x83;
  const double x425 = c_0 * x1;
  const double x426 = -x290;
  const double x427 = x3 + x426;
  const double x428 = -x425 + x427;
  const double x429
      = x126
            * ((1.0 / 2.0) * x100 * (x271 + x423)
               + (1.0 / 2.0) * x117 * x158 * x199
               + (1.0 / 2.0) * x117 * x189 * x305 - x124 * (x272 + x424)
               + (1.0 / 2.0) * x133 * x189 * x191 * x96 - x207 * x400 - x274
               - x306 * x402 - x313 * x401 - x415 * x84)
        + x156
              * (x153 + x158 * x412 + x19 * (-x277 - x405 - x428) + x191 * x409
                 + x295 * x408)
        + x189 * x395 + x296 * x397 + x307 * x396 + x308 * x413;
  const double x430 = a_2 * x11;
  const double x431 = -x430;
  const double x432 = c_1 * x146;
  const double x433 = x183 - x432;
  const double x434 = a_2 * x36;
  const double x435 = -x168 + x434;
  const double x436 = 2 * x94;
  const double x437 = -x436;
  const double x438 = 2 * x93;
  const double x439 = x437 + x438;
  const double x440
      = x126
            * ((1.0 / 2.0) * x100 * (x114 * x80 + x439)
               + (1.0 / 2.0) * x117 * x189 * x336
               + (1.0 / 2.0) * x117 * x199 * x213 - x124 * (x128 * x80 + x95)
               + (1.0 / 2.0) * x133 * x189 * x234 * x96 - x244 * x400
               - x337 * x402 - x344 * x401 - x415 * x90)
        + x156
              * (x19 * (x174 + x320 + x431 + x433 + x435) + x213 * x412
                 + x234 * x409 + x327 * x408)
        + x232 * x395 + x328 * x397 + x338 * x396 + x339 * x413;
  const double x441 = -x420;
  const double x442 = x417 - x419;
  const double x443
      = x126
            * ((1.0 / 2.0) * x100 * (x196 * x84 + x312)
               + (1.0 / 2.0) * x117 * x189 * x355 - x124 * (x205 * x84 + x86)
               - x357 * x402 - x358 * x401)
        + x156 * (x19 * (x169 + x299 + x349 + x441 + x442) + x351 * x408)
        + x202 * x351 + x360 * x396 + x361 * x397;
  const double x444 = x425 + x66;
  const double x445 = -x406 + x427;
  const double x446 = x118 * x196;
  const double x447 = x118 * x205;
  const double x448 = x126
                          * ((1.0 / 2.0) * x100 * (x352 + x446)
                             + (1.0 / 2.0) * x117 * x189 * x371
                             - x124 * (x356 + x447) - x373 * x402 - x374 * x401)
                      + x156 * (x19 * (x347 + x444 + x445) + x367 * x408)
                      + x202 * x367 + x376 * x396 + x377 * x397;
  const double x449 = -x434;
  const double x450 = x180 - x287;
  const double x451 = -x320 + x430;
  const double x452 = x436 - x438;
  const double x453 = -x93 + x94;
  const double x454
      = x126
            * ((1.0 / 2.0) * x100 * (x114 * x200 + x452)
               + (1.0 / 2.0) * x117 * x189 * x386 - x124 * (x128 * x200 + x453)
               - x388 * x402 - x389 * x401)
        + x156 * (x19 * (x168 + x172 + x449 + x450 + x451) + x382 * x408)
        + x202 * x382 + x391 * x396 + x393 * x396;
  const double x455 = x105 * x243;
  const double x456 = x18 * x243;
  const double x457 = x108 * x456;
  const double x458 = x18 * x242;
  const double x459 = x135 * x240;
  const double x460 = x137 * x241;
  const double x461 = x139 * x241;
  const double x462 = x148 + x404;
  const double x463 = x24 + x406;
  const double x464 = x232 * x74;
  const double x465 = x155 * x232;
  const double x466 = a_0 * x14;
  const double x467 = -x466;
  const double x468 = a_2 * x165;
  const double x469 = a_0 * x40;
  const double x470 = -x350 + x469;
  const double x471 = x228 * x74;
  const double x472 = x139 * x240;
  const double x473
      = x126
            * ((1.0 / 2.0) * x100 * (x196 * x270 + x383)
               + (1.0 / 2.0) * x117 * x2 * x239
               + (1.0 / 2.0) * x117 * x232 * x265 - x124 * (x205 * x270 + x387)
               + (1.0 / 2.0) * x131 * x133 * x232 * x96 - x134 * x459
               - x266 * x472 - x273 * x460 - x461 * x78)
        + x156
              * (x131 * x465 + x19 * (x380 + x41 + x467 + x468 + x470)
                 + x2 * x471 + x258 * x464)
        + x259 * x457 + x267 * x456 + x268 * x458 + x455 * x56;
  const double x474 = a_1 * x14;
  const double x475 = -x474;
  const double x476 = a_2 * x143;
  const double x477 = a_1 * x40;
  const double x478 = -x366 + x477;
  const double x479
      = x126
            * ((1.0 / 2.0) * x100 * (x237 * x270 + x452)
               + (1.0 / 2.0) * x117 * x158 * x239
               + (1.0 / 2.0) * x117 * x232 * x305 - x124 * (x270 * x414 + x453)
               + (1.0 / 2.0) * x133 * x191 * x232 * x96 - x207 * x459
               - x306 * x472 - x313 * x460 - x461 * x84)
        + x156
              * (x158 * x471 + x19 * (x174 + x450 + x475 + x476 + x478)
                 + x191 * x465 + x295 * x464)
        + x189 * x455 + x296 * x457 + x307 * x456 + x308 * x458;
  const double x480
      = x126
            * ((1.0 / 2.0) * x100 * (x237 * x80 + x423)
               + (1.0 / 2.0) * x117 * x213 * x239
               + (1.0 / 2.0) * x117 * x232 * x336 - x124 * (x414 * x80 + x424)
               + (1.0 / 2.0) * x133 * x232 * x234 * x96 - x244 * x459 - x274
               - x337 * x472 - x344 * x460 - x461 * x90)
        + x156
              * (x153 + x19 * (-x280 - x428 - x462) + x213 * x471 + x234 * x465
                 + x327 * x464)
        + x232 * x455 + x328 * x457 + x338 * x456 + x339 * x458;
  const double x481 = -x469;
  const double x482 = x466 - x468;
  const double x483
      = x126
            * ((1.0 / 2.0) * x100 * (x196 * x90 + x343)
               + (1.0 / 2.0) * x117 * x232 * x355 - x124 * (x205 * x90 + x92)
               - x357 * x472 - x358 * x460)
        + x156 * (x19 * (x220 + x331 + x350 + x481 + x482) + x351 * x464)
        + x242 * x351 + x360 * x456 + x361 * x457;
  const double x484 = -x477;
  const double x485 = x474 - x476;
  const double x486
      = x126
            * ((1.0 / 2.0) * x100 * (x237 * x90 + x439)
               + (1.0 / 2.0) * x117 * x232 * x371 - x124 * (x414 * x90 + x95)
               - x373 * x472 - x374 * x460)
        + x156 * (x19 * (x172 + x366 + x433 + x484 + x485) + x367 * x464)
        + x242 * x367 + x376 * x456 + x377 * x457;
  const double x487 = x425 + x62;
  const double x488
      = x126
            * ((1.0 / 2.0) * x100 * (x200 * x237 + x446)
               + (1.0 / 2.0) * x117 * x232 * x386 - x124 * (x200 * x414 + x447)
               - x388 * x472 - x389 * x460)
        + x156 * (x19 * (x348 + x445 + x487) + x382 * x464) + x242 * x382
        + x391 * x456 + x393 * x456;
  const double x489 = x105 * x268;
  const double x490 = x18 * x268;
  const double x491 = x108 * x490;
  const double x492 = x263 * x270;
  const double x493 = ((x270) * (x270));
  const double x494 = x135 * x78;
  const double x495 = x137 * x266;
  const double x496 = x139 * x78;
  const double x497 = 2 * x7;
  const double x498 = -c_2 * x233 + x497;
  const double x499 = 2 * x6;
  const double x500 = -c_1 * x167 + x499;
  const double x501 = x2 * x74;
  const double x502 = x155 * x2;
  const double x503 = a_1 * x0;
  const double x504 = x258 * x74;
  const double x505 = x18 * x267;
  const double x506 = x124 * x83;
  const double x507 = x139 * x266;
  const double x508
      = x126
            * ((1.0 / 2.0) * x100 * x261 * x83
               + (1.0 / 2.0) * x117 * x158 * x265
               + (1.0 / 2.0) * x117 * x2 * x305
               + (1.0 / 2.0) * x133 * x191 * x2 * x96 - x207 * x494
               - x269 * x506 - x306 * x496 - x313 * x495 - x507 * x84)
        + x156
              * (x158 * x504 + x19 * (-x363 - x37 - x441 - x503) + x191 * x502
                 + x295 * x501)
        + x189 * x489 + x296 * x491 + x307 * x490 + x308 * x505;
  const double x509 = a_2 * x0;
  const double x510
      = x126
            * ((1.0 / 2.0) * x100 * x263 * x83 + (1.0 / 2.0) * x117 * x2 * x336
               + (1.0 / 2.0) * x117 * x213 * x265
               + (1.0 / 2.0) * x133 * x2 * x234 * x96 - x244 * x494
               - x270 * x506 - x337 * x496 - x344 * x495 - x507 * x90)
        + x156
              * (x19 * (-x379 - x41 - x481 - x509) + x213 * x504 + x234 * x502
                 + x327 * x501)
        + x232 * x489 + x328 * x491 + x338 * x490 + x339 * x505;
  const double x511 = x275 + x497;
  const double x512 = x278 + x499;
  const double x513 = x263 * x90;
  const double x514 = x270 * x90;
  const double x515
      = x126
            * ((1.0 / 2.0) * x100 * (x261 * x84 + x513)
               + (1.0 / 2.0) * x117 * x2 * x355 - x124 * (x269 * x84 + x514)
               - x357 * x496 - x358 * x495)
        + x156 * (x19 * (-x17 - x346 - x511 - x512) + x351 * x501) + x267 * x351
        + x360 * x490 + x361 * x491;
  const double x516
      = x126
            * ((1.0 / 2.0) * x100 * (x118 * x261 + x312)
               + (1.0 / 2.0) * x117 * x2 * x371 - x124 * (x118 * x269 + x86)
               - x373 * x496 - x374 * x495)
        + x156 * (x19 * (x188 + x250 + x301 + x442 + x503) + x367 * x501)
        + x267 * x367 + x376 * x490 + x377 * x491;
  const double x517
      = x126
            * ((1.0 / 2.0) * x100 * (x118 * x263 + x343)
               + (1.0 / 2.0) * x117 * x2 * x386 - x124 * (x118 * x270 + x92)
               - x388 * x496 - x389 * x495)
        + x156 * (x19 * (x231 + x251 + x333 + x482 + x509) + x382 * x501)
        + x267 * x382 + x391 * x490 + x393 * x490;
  const double x518 = x105 * x308;
  const double x519 = x18 * x308;
  const double x520 = x108 * x519;
  const double x521 = x302 * x83;
  const double x522 = ((x83) * (x83));
  const double x523 = x135 * x84;
  const double x524 = x137 * x306;
  const double x525 = x139 * x84;
  const double x526 = 2 * x5;
  const double x527 = -c_0 * x32 + x526;
  const double x528 = x158 * x74;
  const double x529 = x155 * x158;
  const double x530 = a_2 * x157;
  const double x531 = x213 * x74;
  const double x532 = x18 * x339;
  const double x533 = x139 * x90;
  const double x534
      = x126
            * ((1.0 / 2.0) * x100 * x263 * x80
               + (1.0 / 2.0) * x117 * x158 * x336
               + (1.0 / 2.0) * x117 * x213 * x305 - x124 * x270 * x80
               + (1.0 / 2.0) * x133 * x158 * x234 * x96 - x244 * x523
               - x306 * x533 - x337 * x525 - x344 * x524)
        + x156
              * (x19 * (-x174 - x449 - x484 - x530) + x234 * x529 + x295 * x531
                 + x327 * x528)
        + x232 * x518 + x307 * x532 + x328 * x520 + x338 * x519;
  const double x535
      = x126
            * ((1.0 / 2.0) * x100 * (x302 * x84 + x368)
               + (1.0 / 2.0) * x117 * x158 * x355 - x124 * (-x310 - x81)
               - x357 * x525 - x358 * x524)
        + x156 * (x19 * (x187 + x298 + x365 + x421 + x503) + x351 * x528)
        + x307 * x351 + x360 * x519 + x361 * x520;
  const double x536 = x4 + x426 + x526;
  const double x537 = x118 * x302;
  const double x538 = x118 * x83;
  const double x539 = x126
                          * ((1.0 / 2.0) * x100 * (x513 + x537)
                             + (1.0 / 2.0) * x117 * x158 * x371
                             - x124 * (x514 + x538) - x373 * x525 - x374 * x524)
                      + x156 * (x19 * (-x16 - x444 - x511 - x536) + x367 * x528)
                      + x307 * x367 + x376 * x519 + x377 * x520;
  const double x540
      = x126
            * ((1.0 / 2.0) * x100 * (x200 * x263 + x439)
               + (1.0 / 2.0) * x117 * x158 * x386 - x124 * (x200 * x270 + x95)
               - x388 * x525 - x389 * x524)
        + x156 * (x19 * (x287 + x411 + x435 + x485 + x530) + x382 * x528)
        + x307 * x382 + x391 * x519 + x393 * x519;
  const double x541 = x108 * x532;
  const double x542 = x137 * x337;
  const double x543
      = x126
            * ((1.0 / 2.0) * x100 * (x302 * x90 + x383)
               + (1.0 / 2.0) * x117 * x213 * x355 - x124 * (-x341 - x89)
               - x357 * x533 - x358 * x542)
        + x156 * (x19 * (x230 + x330 + x381 + x470 + x509) + x351 * x531)
        + x338 * x351 + x360 * x532 + x361 * x541;
  const double x544
      = x126
            * ((1.0 / 2.0) * x100 * (x260 * x90 + x452)
               + (1.0 / 2.0) * x117 * x213 * x371 - x124 * (-x437 - x93)
               - x373 * x533 - x374 * x542)
        + x156 * (x19 * (x410 + x432 + x451 + x478 + x530) + x367 * x531)
        + x338 * x367 + x376 * x532 + x377 * x541;
  const double x545
      = x126
            * ((1.0 / 2.0) * x100 * (x200 * x260 + x537)
               + (1.0 / 2.0) * x117 * x213 * x386 - x124 * (x200 * x80 + x538)
               - x388 * x533 - x389 * x542)
        + x156 * (x19 * (-x13 - x487 - x512 - x536) + x382 * x531) + x338 * x382
        + x391 * x532 + x393 * x532;
  const double x546 = -b_2 * x233 + x497;
  const double x547 = -b_1 * x167 + x499;
  const double x548 = x351 * x77;
  const double x549 = x213 * x90;
  const double x550 = x137 * x357;
  const double x551 = x108 * x351;
  const double x552 = x118 * x124;
  const double x553
      = x105 * (-x169 - x297 - x418 - x503)
        + x126 * ((1.0 / 2.0) * x100 * x118 * x158 - x374 * x550 - x552 * x84)
        + x360 * x367 + x375 * x548 + x377 * x551;
  const double x554
      = x105 * (-x220 - x329 - x467 - x509)
        + x126 * ((1.0 / 2.0) * x100 * x118 * x213 - x389 * x550 - x552 * x90)
        + x360 * x382 + x390 * x548 + x392 * x551;
  const double x555 = -b_0 * x32 + x526;
  const double x556 = x118 * x56;
  const double x557 = ((x118) * (x118));
  const double x558 = x137 * x373;
  const double x559 = x108 * x367;
  const double x560 = x105 * (-x172 - x431 - x475 - x530)
                      + x126
                            * ((1.0 / 2.0) * x100 * x200 * x213
                               - x124 * x200 * x90 - x389 * x558)
                      + x367 * x391 + x376 * x382 + x392 * x559;

  out_2823277736450422735[0]
      = x106 * x2 + 2 * x107 * x125 - x109 * x55
        + x126
              * ((1.0 / 2.0) * x100 * (x111 * x127 + x129) + x116 * x117 * x56
                 + x121 * x138 - 2 * x121 * x140
                 - x124 * (((x127) * (x127)) + x130)
                 + (1.0 / 2.0) * x132 * x133 * x56 * x96 + x134 * x136 - x142)
        + x156
              * (x132 * x155 * x56 + 2 * x152 * x55 + x154
                 + x19 * (x145 + x147 + x150 + x151));
  out_2823277736450422735[1] = x211;
  out_2823277736450422735[2] = x247;
  out_2823277736450422735[3] = x281;
  out_2823277736450422735[4] = x314;
  out_2823277736450422735[5] = x345;
  out_2823277736450422735[6] = x362;
  out_2823277736450422735[7] = x378;
  out_2823277736450422735[8] = x394;
  out_2823277736450422735[9] = x211;
  out_2823277736450422735[10]
      = x126
            * ((1.0 / 2.0) * x100 * (x129 + x398) + x117 * x189 * x199
               - x124 * (x130 + x399) + (1.0 / 2.0) * x133 * x189 * x192 * x96
               - x142 - 2 * x201 * x402 - x208 * x400 - x210 * x401)
        + x156
              * (x154 + 2 * x185 * x408 + x19 * (x147 + x403 + x405 + x407)
                 + x192 * x409)
        + x158 * x395 + x186 * x397 + 2 * x202 * x396;
  out_2823277736450422735[11] = x416;
  out_2823277736450422735[12] = x422;
  out_2823277736450422735[13] = x429;
  out_2823277736450422735[14] = x440;
  out_2823277736450422735[15] = x443;
  out_2823277736450422735[16] = x448;
  out_2823277736450422735[17] = x454;
  out_2823277736450422735[18] = x247;
  out_2823277736450422735[19] = x416;
  out_2823277736450422735[20]
      = x126
            * ((1.0 / 2.0) * x100 * (x237 * x414 + x398) + x117 * x232 * x239
               - x124 * (x399 + ((x414) * (x414)))
               + (1.0 / 2.0) * x133 * x232 * x235 * x96 - x142 - 2 * x240 * x461
               - x245 * x459 - x246 * x460)
        + x156
              * (x154 + x19 * (x145 + x403 + x462 + x463) + 2 * x228 * x464
                 + x235 * x465)
        + x213 * x455 + x229 * x457 + 2 * x243 * x458;
  out_2823277736450422735[21] = x473;
  out_2823277736450422735[22] = x479;
  out_2823277736450422735[23] = x480;
  out_2823277736450422735[24] = x483;
  out_2823277736450422735[25] = x486;
  out_2823277736450422735[26] = x488;
  out_2823277736450422735[27] = x281;
  out_2823277736450422735[28] = x422;
  out_2823277736450422735[29] = x473;
  out_2823277736450422735[30]
      = x126
            * ((1.0 / 2.0) * x100 * (x261 * x269 + x492) + x117 * x2 * x265
               - x124 * (((x269) * (x269)) + x493)
               + (1.0 / 2.0) * x131 * x133 * x2 * x96 - x134 * x494 - x142
               - 2 * x266 * x496 - x273 * x495)
        + x156
              * (x131 * x502 + x154 + x19 * (x150 + x498 + x500)
                 + 2 * x258 * x501)
        + x259 * x491 + 2 * x267 * x490 + x489 * x56;
  out_2823277736450422735[31] = x508;
  out_2823277736450422735[32] = x510;
  out_2823277736450422735[33] = x515;
  out_2823277736450422735[34] = x516;
  out_2823277736450422735[35] = x517;
  out_2823277736450422735[36] = x314;
  out_2823277736450422735[37] = x429;
  out_2823277736450422735[38] = x479;
  out_2823277736450422735[39] = x508;
  out_2823277736450422735[40]
      = x126
            * ((1.0 / 2.0) * x100 * (x492 + x521) + x117 * x158 * x305
               - x124 * (x493 + x522) + (1.0 / 2.0) * x133 * x158 * x191 * x96
               - x142 - x207 * x523 - 2 * x306 * x525 - x313 * x524)
        + x156
              * (x154 + x19 * (x405 + x498 + x527) + x191 * x529
                 + 2 * x295 * x528)
        + x189 * x518 + x296 * x520 + 2 * x307 * x519;
  out_2823277736450422735[41] = x534;
  out_2823277736450422735[42] = x535;
  out_2823277736450422735[43] = x539;
  out_2823277736450422735[44] = x540;
  out_2823277736450422735[45] = x345;
  out_2823277736450422735[46] = x440;
  out_2823277736450422735[47] = x480;
  out_2823277736450422735[48] = x510;
  out_2823277736450422735[49] = x534;
  out_2823277736450422735[50]
      = x105 * x232 * x339
        + x126
              * ((1.0 / 2.0) * x100 * (x260 * x80 + x521) + x117 * x213 * x336
                 - x124 * (x522 + ((x80) * (x80)))
                 + (1.0 / 2.0) * x133 * x213 * x234 * x96 - x135 * x244 * x90
                 - x142 - 2 * x337 * x533 - x344 * x542)
        + x156
              * (x154 + x155 * x213 * x234 + x19 * (x462 + x500 + x527)
                 + 2 * x327 * x531)
        + x328 * x541 + 2 * x338 * x532;
  out_2823277736450422735[51] = x543;
  out_2823277736450422735[52] = x544;
  out_2823277736450422735[53] = x545;
  out_2823277736450422735[54] = x362;
  out_2823277736450422735[55] = x443;
  out_2823277736450422735[56] = x483;
  out_2823277736450422735[57] = x515;
  out_2823277736450422735[58] = x535;
  out_2823277736450422735[59] = x543;
  out_2823277736450422735[60]
      = x105 * (x151 + x546 + x547)
        + x126
              * ((1.0 / 2.0) * x100 * (x158 * x84 + x549) - x124 * x98
                 - x358 * x550)
        + 2 * x359 * x548 + x361 * x551;
  out_2823277736450422735[61] = x553;
  out_2823277736450422735[62] = x554;
  out_2823277736450422735[63] = x378;
  out_2823277736450422735[64] = x448;
  out_2823277736450422735[65] = x486;
  out_2823277736450422735[66] = x516;
  out_2823277736450422735[67] = x539;
  out_2823277736450422735[68] = x544;
  out_2823277736450422735[69] = x553;
  out_2823277736450422735[70] = x105 * (x407 + x546 + x555)
                                + x126
                                      * ((1.0 / 2.0) * x100 * (x549 + x556)
                                         - x124 * (x557 + x97) - x374 * x558)
                                + 2 * x367 * x376 + x377 * x559;
  out_2823277736450422735[71] = x560;
  out_2823277736450422735[72] = x394;
  out_2823277736450422735[73] = x454;
  out_2823277736450422735[74] = x488;
  out_2823277736450422735[75] = x517;
  out_2823277736450422735[76] = x540;
  out_2823277736450422735[77] = x545;
  out_2823277736450422735[78] = x554;
  out_2823277736450422735[79] = x560;
  out_2823277736450422735[80]
      = x105 * (x463 + x547 + x555)
        + x126
              * ((1.0 / 2.0) * x100 * (x189 * x200 + x556)
                 - x124 * (((x200) * (x200)) + x557) - x137 * x388 * x389)
        + 2 * x382 * x391 + x382 * x393;
}
float edge_vertex_value(float a_0, float a_1, float a_2, float b_0, float b_1,
                        float b_2, float c_0, float c_1, float c_2, float d_h) {
  const float x0 = a_0 - b_0;
  const float x1 = a_1 - b_1;
  const float x2 = a_2 - b_2;
  const float x3 = ((x0) * (x0)) + ((x1) * (x1)) + ((x2) * (x2));
  const float x4 = a_1 - c_1;
  const float x5 = a_0 - c_0;
  const float x6 = a_2 - c_2;
  const float x7 = ((x0 * x4 - x1 * x5) * (x0 * x4 - x1 * x5))
                   + ((x0 * x6 - x2 * x5) * (x0 * x6 - x2 * x5))
                   + ((x1 * x6 - x2 * x4) * (x1 * x6 - x2 * x4));
  const float x8 = ((a_0) * (a_0));
  const float x9 = ((a_1) * (a_1));
  const float x10 = ((a_2) * (a_2));
  const float x11 = ((b_0) * (b_0));
  const float x12 = ((b_1) * (b_1));
  const float x13 = ((b_2) * (b_2));
  const float x14 = 2 * a_0;
  const float x15 = b_0 * x14;
  const float x16 = 2 * a_1;
  const float x17 = b_1 * x16;
  const float x18 = 2 * a_2;
  const float x19 = b_2 * x18;
  const float x20 = a_1 * x15;
  const float x21 = c_0 * x14;
  const float x22 = a_1 * x21;
  const float x23 = a_2 * b_2;
  const float x24 = a_2 * c_2;
  const float x25 = b_1 * c_1;
  const float x26 = b_2 * c_2;
  const float x27 = c_1 * x16;
  const float x28 = b_0 * c_0;
  const float x29 = c_2 * x18;
  const float x30 = 2 * x28;
  const float x31 = ((c_1) * (c_1));
  const float x32 = ((c_2) * (c_2));
  const float x33 = ((c_0) * (c_0));
  const float x34 = 2 * x8;
  const float x35 = 2 * x9;
  const float x36 = 2 * x10;

  float edge_vertex_value_result;
  edge_vertex_value_result
      = (-2 * log(d_h)
         + log((-b_1 * x20 + b_1 * x22 + c_1 * x20 - c_1 * x22 + x10 * x11
                + x10 * x12 + x10 * x31 + x10 * x33 - x11 * x27 - x11 * x29
                + x11 * x31 + x11 * x32 + x11 * x9 - x12 * x21 - x12 * x29
                + x12 * x32 + x12 * x33 + x12 * x8 - x13 * x21 - x13 * x27
                + x13 * x31 + x13 * x33 + x13 * x8 + x13 * x9 - x15 * x23
                + x15 * x24 + x15 * x25 + x15 * x26 - x15 * x31 - x15 * x32
                - x17 * x23 + x17 * x24 + x17 * x26 + x17 * x28 - x17 * x32
                - x17 * x33 + x19 * x25 + x19 * x28 - x19 * x31 - x19 * x33
                + x21 * x23 - x21 * x24 + x21 * x25 + x21 * x26 + x23 * x27
                - x24 * x27 - 2 * x25 * x26 + x25 * x29 - x25 * x30 - x25 * x34
                - x25 * x36 + x26 * x27 - x26 * x30 - x26 * x34 - x26 * x35
                + x27 * x28 + x28 * x29 - x28 * x35 - x28 * x36 + x31 * x8
                + x32 * x8 + x32 * x9 + x33 * x9)
               / (x10 + x11 + x12 + x13 - x15 - x17 - x19 + x8 + x9)))
        * ((1.0 / 2.0) * ((d_h) * (d_h)) - d_h * sqrt(x7) / sqrt(x3)
           + (1.0 / 2.0) * x7 / x3);
  return edge_vertex_value_result;
}

void edge_vertex_grad(float a_0, float a_1, float a_2, float b_0, float b_1,
                      float b_2, float c_0, float c_1, float c_2, float d_h,
                      float *out_5905569529076117828) {
  const float x0 = ((a_0) * (a_0));
  const float x1 = ((a_1) * (a_1));
  const float x2 = ((a_2) * (a_2));
  const float x3 = ((b_0) * (b_0));
  const float x4 = ((b_1) * (b_1));
  const float x5 = ((b_2) * (b_2));
  const float x6 = 2 * a_0;
  const float x7 = b_0 * x6;
  const float x8 = 2 * b_1;
  const float x9 = a_1 * x8;
  const float x10 = 2 * b_2;
  const float x11 = a_2 * x10;
  const float x12 = x0 + x1 - x11 + x2 + x3 + x4 + x5 - x7 - x9;
  const float x13 = 1.0 / x12;
  const float x14 = b_0 * x9;
  const float x15 = 2 * c_1;
  const float x16 = a_1 * x15;
  const float x17 = b_0 * x16;
  const float x18 = c_0 * x9;
  const float x19 = c_0 * x16;
  const float x20 = b_0 * x11;
  const float x21 = 2 * c_2;
  const float x22 = a_2 * x21;
  const float x23 = b_0 * x22;
  const float x24 = c_0 * x11;
  const float x25 = c_0 * x22;
  const float x26 = b_1 * x15;
  const float x27 = b_0 * x26;
  const float x28 = b_2 * x21;
  const float x29 = b_0 * x28;
  const float x30 = c_0 * x26;
  const float x31 = c_0 * x28;
  const float x32 = b_2 * x9;
  const float x33 = c_2 * x9;
  const float x34 = b_2 * x16;
  const float x35 = c_2 * x16;
  const float x36 = b_2 * c_2;
  const float x37 = b_2 * x26;
  const float x38 = c_2 * x26;
  const float x39 = ((c_1) * (c_1));
  const float x40 = ((c_2) * (c_2));
  const float x41 = ((c_0) * (c_0));
  const float x42 = x4 * x6;
  const float x43 = x5 * x6;
  const float x44 = x0 * x15;
  const float x45 = x0 * x21;
  const float x46 = x15 * x3;
  const float x47 = x15 * x5;
  const float x48 = 2 * b_0;
  const float x49 = x1 * x48;
  const float x50 = x1 * x21;
  const float x51 = x21 * x3;
  const float x52 = x21 * x4;
  const float x53 = x2 * x48;
  const float x54 = x15 * x2;
  const float x55
      = -a_0 * x14 + a_0 * x17 + a_0 * x18 - a_0 * x19 - a_0 * x20 + a_0 * x23
        + a_0 * x24 - a_0 * x25 + a_0 * x27 + a_0 * x29 + a_0 * x30 + a_0 * x31
        - a_1 * x46 - a_1 * x47 - a_2 * x32 + a_2 * x33 + a_2 * x34 - a_2 * x35
        + a_2 * x37 + a_2 * x38 - a_2 * x51 - a_2 * x52 + b_0 * x19 + b_0 * x25
        - b_1 * x44 - b_1 * x54 - b_2 * x45 - b_2 * x50 + c_0 * x14 + c_0 * x20
        - c_0 * x27 - c_0 * x29 - c_0 * x42 - c_0 * x43 - c_0 * x49 - c_0 * x53
        + x0 * x39 + x0 * x4 + x0 * x40 + x0 * x5 + x1 * x3 + x1 * x40
        + x1 * x41 + x1 * x5 - x11 * x39 - x11 * x41 + x16 * x36 + x2 * x3
        + x2 * x39 + x2 * x4 + x2 * x41 - x26 * x36 + x3 * x39 + x3 * x40
        + x36 * x9 + x39 * x5 - x39 * x7 + x4 * x40 + x4 * x41 - x40 * x7
        - x40 * x9 + x41 * x5 - x41 * x9;
  const float x56 = -2 * log(d_h) + log(x13 * x55);
  const float x57 = a_0 - b_0;
  const float x58 = a_1 - b_1;
  const float x59 = a_2 - b_2;
  const float x60 = ((x57) * (x57)) + ((x58) * (x58)) + ((x59) * (x59));
  const float x61 = 1.0 / x60;
  const float x62 = -x15;
  const float x63 = x62 + x8;
  const float x64 = a_1 - c_1;
  const float x65 = a_0 - c_0;
  const float x66 = x57 * x64 - x58 * x65;
  const float x67 = x63 * x66;
  const float x68 = -x21;
  const float x69 = x10 + x68;
  const float x70 = a_2 - c_2;
  const float x71 = x57 * x70 - x59 * x65;
  const float x72 = x69 * x71;
  const float x73 = -x48 + x6;
  const float x74 = -x73;
  const float x75 = pow(x60, -2);
  const float x76 = x58 * x70 - x59 * x64;
  const float x77 = ((x66) * (x66)) + ((x71) * (x71)) + ((x76) * (x76));
  const float x78 = sqrt(x77);
  const float x79 = d_h * x78;
  const float x80 = x79 / pow(x60, 3.0 / 2.0);
  const float x81 = pow(x60, -1.0 / 2.0);
  const float x82 = d_h * x81 / x78;
  const float x83 = x39 * x48;
  const float x84 = x40 * x48;
  const float x85 = 2 * c_0;
  const float x86 = x4 * x85;
  const float x87 = x5 * x85;
  const float x88 = x39 * x6;
  const float x89 = x40 * x6;
  const float x90 = 4 * a_0;
  const float x91 = b_1 * c_1;
  const float x92 = x18 + x24;
  const float x93 = x17 + x23;
  const float x94 = x55 / ((x12) * (x12));
  const float x95
      = ((1.0 / 2.0) * ((d_h) * (d_h)) + (1.0 / 2.0) * x61 * x77 - x79 * x81)
        / x55;
  const float x96 = x12 * x95;
  const float x97 = -x85;
  const float x98 = -x48 - x97;
  const float x99 = x66 * x98;
  const float x100 = x69 * x76;
  const float x101 = 2 * a_1;
  const float x102 = x101 - x8;
  const float x103 = -x102;
  const float x104 = x41 * x8;
  const float x105 = x40 * x8;
  const float x106 = x101 * x3;
  const float x107 = x101 * x5;
  const float x108 = x101 * x41;
  const float x109 = x101 * x40;
  const float x110 = 4 * a_1;
  const float x111 = b_0 * c_0;
  const float x112 = a_0 * b_0;
  const float x113 = x112 * x8;
  const float x114 = a_0 * c_0;
  const float x115 = x114 * x15;
  const float x116 = a_2 * x8;
  const float x117 = b_2 * x116;
  const float x118 = a_2 * x15;
  const float x119 = c_2 * x118;
  const float x120 = x111 * x8;
  const float x121 = x111 * x15;
  const float x122 = x36 * x8;
  const float x123 = x15 * x36;
  const float x124 = b_2 * x118 + x112 * x15;
  const float x125 = c_2 * x116 + x114 * x8;
  const float x126 = x71 * x98;
  const float x127 = -x63 * x76;
  const float x128 = 2 * a_2;
  const float x129 = -2 * b_2 + x128;
  const float x130 = -x129;
  const float x131 = x10 * x41;
  const float x132 = x10 * x39;
  const float x133 = x128 * x3;
  const float x134 = x128 * x4;
  const float x135 = x128 * x41;
  const float x136 = x128 * x39;
  const float x137 = 4 * a_2;
  const float x138 = x10 * x112;
  const float x139 = x114 * x21;
  const float x140 = x10 * x111;
  const float x141 = x111 * x21;
  const float x142 = x112 * x21 + x33;
  const float x143 = x10 * x114 + x34;
  const float x144 = x101 + x62;
  const float x145 = -x144 * x66;
  const float x146 = -x128 - x68;
  const float x147 = x146 * x71;
  const float x148 = x1 * x85;
  const float x149 = x2 * x85;
  const float x150 = c_1 * x110;
  const float x151 = c_2 * x137;
  const float x152 = a_0 * x9;
  const float x153 = a_0 * x11;
  const float x154 = a_0 * x16;
  const float x155 = a_0 * x22;
  const float x156 = a_0 * x26 + a_0 * x28;
  const float x157 = x6 + x97;
  const float x158 = x157 * x66;
  const float x159 = x146 * x76;
  const float x160 = x0 * x8;
  const float x161 = x2 * x8;
  const float x162 = c_0 * x90;
  const float x163 = a_1 * x7;
  const float x164 = a_1 * x11;
  const float x165 = a_1 * c_0;
  const float x166 = x165 * x6;
  const float x167 = a_1 * x22;
  const float x168 = a_1 * x28 + x165 * x48;
  const float x169 = x157 * x71;
  const float x170 = x144 * x76;
  const float x171 = x0 * x10;
  const float x172 = x1 * x10;
  const float x173 = a_2 * x7;
  const float x174 = a_2 * x9;
  const float x175 = a_2 * c_0;
  const float x176 = x175 * x6;
  const float x177 = a_2 * x16;
  const float x178 = a_2 * x26 + x175 * x48;
  const float x179 = x102 * x66;
  const float x180 = x129 * x71;
  const float x181 = b_1 * x110;
  const float x182 = b_2 * x137;
  const float x183 = x66 * x74;
  const float x184 = x129 * x76;
  const float x185 = b_0 * x90;
  const float x186 = x71 * x74;
  const float x187 = x103 * x76;

  out_5905569529076117828[0]
      = x56
            * (x57 * x80 + (1.0 / 2.0) * x61 * (x67 + x72)
               + (1.0 / 2.0) * x74 * x75 * x77
               - x82 * ((1.0 / 2.0) * x67 + (1.0 / 2.0) * x72))
        + x96
              * (x13
                     * (-x14 - x19 - x20 - x25 + x27 + x29 + x30 + x31
                        - x36 * x90 + x42 + x43 - x83 - x84 - x86 - x87 + x88
                        + x89 - x90 * x91 + x92 + x93)
                 + x74 * x94);
  out_5905569529076117828[1]
      = x56
            * ((1.0 / 2.0) * x103 * x75 * x77 + x58 * x80
               + (1.0 / 2.0) * x61 * (x100 + x99)
               - x82 * ((1.0 / 2.0) * x100 + (1.0 / 2.0) * x99))
        + x96
              * (x103 * x94
                 + x13
                       * (-x104 - x105 + x106 + x107 + x108 + x109 - x110 * x111
                          - x110 * x36 - x113 - x115 - x117 - x119 + x120 + x121
                          + x122 + x123 + x124 + x125 - x46 - x47));
  out_5905569529076117828[2]
      = x56
            * ((1.0 / 2.0) * x130 * x75 * x77 + x59 * x80
               + (1.0 / 2.0) * x61 * (x126 + x127)
               - x82 * ((1.0 / 2.0) * x126 + (1.0 / 2.0) * x127))
        + x96
              * (x13
                     * (-x111 * x137 - x131 - x132 + x133 + x134 + x135 + x136
                        - x137 * x91 - x138 - x139 + x140 + x141 + x142 + x143
                        - x32 - x35 + x37 + x38 - x51 - x52)
                 + x130 * x94);
  out_5905569529076117828[3]
      = x56
            * (-x57 * x80 + (1.0 / 2.0) * x61 * (x145 + x147)
               + (1.0 / 2.0) * x73 * x75 * x77
               - x82 * ((1.0 / 2.0) * x145 + (1.0 / 2.0) * x147))
        + x96
              * (x13
                     * (-b_0 * x150 - b_0 * x151 - x148 - x149 - x152 - x153
                        + x154 + x155 + x156 + x19 + x25 - x30 - x31 + x49 + x53
                        + x83 + x84 - x88 - x89 + x92)
                 + x73 * x94);
  out_5905569529076117828[4]
      = x56
            * ((1.0 / 2.0) * x102 * x75 * x77 - x58 * x80
               + (1.0 / 2.0) * x61 * (x158 + x159)
               - x82 * ((1.0 / 2.0) * x158 + (1.0 / 2.0) * x159))
        + x96
              * (x102 * x94
                 + x13
                       * (-b_1 * x151 - b_1 * x162 + x104 + x105 - x108 - x109
                          + x115 + x119 - x121 - x123 + x124 + x160 + x161
                          - x163 - x164 + x166 + x167 + x168 - x44 - x54));
  out_5905569529076117828[5]
      = x56
            * ((1.0 / 2.0) * x129 * x75 * x77 - x59 * x80
               + (1.0 / 2.0) * x61 * (x169 + x170)
               - x82 * ((1.0 / 2.0) * x169 + (1.0 / 2.0) * x170))
        + x96
              * (x129 * x94
                 + x13
                       * (-b_2 * x150 - b_2 * x162 + x131 + x132 - x135 - x136
                          + x139 - x141 + x142 + x171 + x172 - x173 - x174
                          + x176 + x177 + x178 + x35 - x38 - x45 - x50));
  out_5905569529076117828[6]
      = x56
            * ((1.0 / 2.0) * x61 * (x179 + x180)
               - x82 * ((1.0 / 2.0) * x179 + (1.0 / 2.0) * x180))
        + x95
              * (-c_0 * x181 - c_0 * x182 + x14 + x148 + x149 + x152 + x153
                 - x154 - x155 + x156 + x20 - x27 - x29 - x42 - x43 - x49 - x53
                 + x86 + x87 + x93);
  out_5905569529076117828[7]
      = x56
            * ((1.0 / 2.0) * x61 * (x183 + x184)
               - x82 * ((1.0 / 2.0) * x183 + (1.0 / 2.0) * x184))
        + x95
              * (-c_1 * x182 - c_1 * x185 - x106 - x107 + x113 + x117 - x120
                 - x122 + x125 - x160 - x161 + x163 + x164 - x166 - x167 + x168
                 + x44 + x46 + x47 + x54);
  out_5905569529076117828[8]
      = x56
            * ((1.0 / 2.0) * x61 * (x186 + x187)
               - x82 * ((1.0 / 2.0) * x186 + (1.0 / 2.0) * x187))
        + x95
              * (-c_2 * x181 - c_2 * x185 - x133 - x134 + x138 - x140 + x143
                 - x171 - x172 + x173 + x174 - x176 - x177 + x178 + x32 - x37
                 + x45 + x50 + x51 + x52);
}

void edge_vertex_hessian(float a_0, float a_1, float a_2, float b_0, float b_1,
                         float b_2, float c_0, float c_1, float c_2, float d_h,
                         float *out_2823277736450422735) {
  const float x0 = 2 * a_0;
  const float x1 = 2 * b_0;
  const float x2 = x0 - x1;
  const float x3 = b_0 * x0;
  const float x4 = -x3;
  const float x5 = ((a_0) * (a_0));
  const float x6 = ((a_1) * (a_1));
  const float x7 = ((a_2) * (a_2));
  const float x8 = ((b_0) * (b_0));
  const float x9 = ((b_1) * (b_1));
  const float x10 = ((b_2) * (b_2));
  const float x11 = 2 * b_1;
  const float x12 = a_1 * x11;
  const float x13 = -x12;
  const float x14 = 2 * b_2;
  const float x15 = a_2 * x14;
  const float x16 = -x15;
  const float x17 = x13 + x16;
  const float x18 = x10 + x17 + x4 + x5 + x6 + x7 + x8 + x9;
  const float x19 = 1.0 / x18;
  const float x20 = ((c_1) * (c_1));
  const float x21 = x1 * x20;
  const float x22 = ((c_2) * (c_2));
  const float x23 = x1 * x22;
  const float x24 = 2 * x9;
  const float x25 = c_0 * x24;
  const float x26 = 2 * x10;
  const float x27 = c_0 * x26;
  const float x28 = x0 * x9;
  const float x29 = x0 * x10;
  const float x30 = x0 * x20;
  const float x31 = x0 * x22;
  const float x32 = 4 * a_0;
  const float x33 = c_1 * x32;
  const float x34 = c_2 * x32;
  const float x35 = b_0 * x12;
  const float x36 = 2 * c_1;
  const float x37 = c_0 * x36;
  const float x38 = a_1 * x37;
  const float x39 = b_0 * x15;
  const float x40 = 2 * c_2;
  const float x41 = c_0 * x40;
  const float x42 = a_2 * x41;
  const float x43 = b_0 * x36;
  const float x44 = b_1 * x43;
  const float x45 = b_0 * x40;
  const float x46 = b_2 * x45;
  const float x47 = b_1 * x37;
  const float x48 = b_2 * x41;
  const float x49 = c_0 * x12;
  const float x50 = c_0 * x15;
  const float x51 = x49 + x50;
  const float x52 = a_1 * x43;
  const float x53 = a_2 * x45;
  const float x54 = x52 + x53;
  const float x55 = -b_1 * x33 - b_2 * x34 - x21 - x23 - x25 - x27 + x28 + x29
                    + x30 + x31 - x35 - x38 - x39 - x42 + x44 + x46 + x47 + x48
                    + x51 + x54;
  const float x56 = -x2;
  const float x57 = b_2 * x12;
  const float x58 = c_2 * x12;
  const float x59 = a_1 * x36;
  const float x60 = b_2 * x59;
  const float x61 = c_2 * x59;
  const float x62 = b_1 * x36;
  const float x63 = b_2 * x62;
  const float x64 = c_2 * x62;
  const float x65 = ((c_0) * (c_0));
  const float x66 = b_2 * x40;
  const float x67 = x36 * x8;
  const float x68 = x10 * x36;
  const float x69 = x1 * x6;
  const float x70 = x40 * x8;
  const float x71 = x40 * x9;
  const float x72 = x1 * x7;
  const float x73
      = -a_0 * x35 - a_0 * x38 - a_0 * x39 - a_0 * x42 + a_0 * x44 + a_0 * x46
        + a_0 * x47 + a_0 * x48 + a_0 * x49 + a_0 * x50 + a_0 * x52 + a_0 * x53
        - a_1 * x67 - a_1 * x68 - a_2 * x57 + a_2 * x58 + a_2 * x60 - a_2 * x61
        + a_2 * x63 + a_2 * x64 - a_2 * x70 - a_2 * x71 + b_0 * x38 + b_0 * x42
        - b_0 * x47 - b_0 * x48 + b_2 * x61 - c_0 * x28 - c_0 * x29 + c_0 * x35
        + c_0 * x39 - c_0 * x69 - c_0 * x72 + c_2 * x57 - c_2 * x63 + x10 * x20
        + x10 * x5 + x10 * x6 + x10 * x65 - x12 * x22 - x12 * x65 - x15 * x20
        - x15 * x65 - x20 * x3 + x20 * x5 + x20 * x7 + x20 * x8 - x22 * x3
        + x22 * x5 + x22 * x6 + x22 * x8 + x22 * x9 - x5 * x62 - x5 * x66
        + x5 * x9 + x6 * x65 - x6 * x66 + x6 * x8 - x62 * x7 + x65 * x7
        + x65 * x9 + x7 * x8 + x7 * x9;
  const float x74 = pow(x18, -2);
  const float x75 = x73 * x74;
  const float x76 = x19 * x55 + x56 * x75;
  const float x77 = 1.0 / x73;
  const float x78 = a_0 - b_0;
  const float x79 = -c_1;
  const float x80 = a_1 + x79;
  const float x81 = x78 * x80;
  const float x82 = -c_0;
  const float x83 = a_0 + x82;
  const float x84 = a_1 - b_1;
  const float x85 = x83 * x84;
  const float x86 = x81 - x85;
  const float x87 = -c_2;
  const float x88 = a_2 + x87;
  const float x89 = x78 * x88;
  const float x90 = a_2 - b_2;
  const float x91 = x83 * x90;
  const float x92 = x89 - x91;
  const float x93 = x84 * x88;
  const float x94 = x80 * x90;
  const float x95 = x93 - x94;
  const float x96 = ((x86) * (x86)) + ((x92) * (x92)) + ((x95) * (x95));
  const float x97 = ((x90) * (x90));
  const float x98 = ((x84) * (x84)) + x97;
  const float x99 = ((x78) * (x78)) + x98;
  const float x100 = 1.0 / x99;
  const float x101 = pow(x99, -1.0 / 2.0);
  const float x102 = sqrt(x96);
  const float x103 = d_h * x102;
  const float x104
      = (1.0 / 2.0) * ((d_h) * (d_h)) + (1.0 / 2.0) * x100 * x96 - x101 * x103;
  const float x105 = x104 * x77;
  const float x106 = x105 * x76;
  const float x107 = x18 * x76;
  const float x108 = x104 / ((x73) * (x73));
  const float x109 = x107 * x108;
  const float x110 = -x36;
  const float x111 = x11 + x110;
  const float x112 = x111 * x86;
  const float x113 = -x40;
  const float x114 = x113 + x14;
  const float x115 = x114 * x92;
  const float x116 = x112 + x115;
  const float x117 = pow(x99, -2);
  const float x118 = -x78;
  const float x119 = pow(x99, -3.0 / 2.0);
  const float x120 = x103 * x119;
  const float x121 = (1.0 / 2.0) * x112 + (1.0 / 2.0) * x115;
  const float x122 = 1.0 / x102;
  const float x123 = d_h * x101;
  const float x124 = x122 * x123;
  const float x125
      = x77
        * ((1.0 / 2.0) * x100 * x116 + (1.0 / 2.0) * x117 * x56 * x96
           - x118 * x120 - x121 * x124);
  const float x126 = -2 * log(d_h) + log(x19 * x73);
  const float x127 = b_1 + x79;
  const float x128 = b_2 + x87;
  const float x129 = x114 * x128;
  const float x130 = ((x128) * (x128));
  const float x131 = -4 * b_0 + x32;
  const float x132 = -x131;
  const float x133 = pow(x99, -3);
  const float x134 = 3 * a_0 - 3 * b_0;
  const float x135 = x103 / pow(x99, 5.0 / 2.0);
  const float x136 = x118 * x135;
  const float x137 = x123 / pow(x96, 3.0 / 2.0);
  const float x138 = x121 * x137;
  const float x139 = d_h * x119 * x122;
  const float x140 = x118 * x139;
  const float x141 = x117 * x96;
  const float x142 = -x120 + x141;
  const float x143 = 4 * b_1;
  const float x144 = c_1 * x143;
  const float x145 = -x144;
  const float x146 = 4 * b_2;
  const float x147 = -c_2 * x146;
  const float x148 = 2 * x20;
  const float x149 = 2 * x22;
  const float x150 = x148 + x149;
  const float x151 = x24 + x26;
  const float x152 = x56 * x74;
  const float x153 = 2 * x75;
  const float x154 = -x153;
  const float x155 = x73 / pow(x18, 3);
  const float x156 = x105 * x18;
  const float x157 = 2 * a_1;
  const float x158 = -x11 + x157;
  const float x159 = x11 * x65;
  const float x160 = x11 * x22;
  const float x161 = x157 * x8;
  const float x162 = a_1 * x26;
  const float x163 = x157 * x65;
  const float x164 = a_1 * x149;
  const float x165 = 4 * b_0;
  const float x166 = c_0 * x165;
  const float x167 = 4 * a_1;
  const float x168 = c_2 * x167;
  const float x169 = b_0 * x11;
  const float x170 = a_0 * x169;
  const float x171 = a_0 * x37;
  const float x172 = b_2 * x11;
  const float x173 = a_2 * x172;
  const float x174 = c_2 * x36;
  const float x175 = a_2 * x174;
  const float x176 = c_0 * x169;
  const float x177 = b_0 * x37;
  const float x178 = c_2 * x172;
  const float x179 = b_2 * x174;
  const float x180 = b_2 * x36;
  const float x181 = a_0 * x43 + a_2 * x180;
  const float x182 = c_0 * x11;
  const float x183 = c_2 * x11;
  const float x184 = a_0 * x182 + a_2 * x183;
  const float x185 = -a_1 * x166 - b_2 * x168 - x159 - x160 + x161 + x162 + x163
                     + x164 - x170 - x171 - x173 - x175 + x176 + x177 + x178
                     + x179 + x181 + x184 - x67 - x68;
  const float x186 = -x185;
  const float x187 = -x43;
  const float x188 = -x182;
  const float x189 = -x158;
  const float x190 = x55 * x74;
  const float x191 = -4 * b_1 + x167;
  const float x192 = -x191;
  const float x193 = x155 * x56;
  const float x194 = 2 * c_0;
  const float x195 = -x194;
  const float x196 = -x1 - x195;
  const float x197 = x196 * x86;
  const float x198 = x114 * x95;
  const float x199 = x197 + x198;
  const float x200 = -x84;
  const float x201 = (1.0 / 2.0) * x197 + (1.0 / 2.0) * x198;
  const float x202
      = x77
        * ((1.0 / 2.0) * x100 * x199 + (1.0 / 2.0) * x117 * x189 * x96
           - x120 * x200 - x124 * x201);
  const float x203 = x185 * x19 + x189 * x75;
  const float x204 = x125 * x18;
  const float x205 = -b_0 - x82;
  const float x206 = x124 * x205;
  const float x207 = 3 * a_1 - 3 * b_1;
  const float x208 = -x207;
  const float x209 = x121 * x139;
  const float x210 = -x201;
  const float x211
      = x106 * x158 + x107 * x202 + x109 * x186
        + x126
              * ((1.0 / 2.0) * x100 * x111 * x205
                 + (1.0 / 2.0) * x116 * x117 * x189
                 + (1.0 / 2.0) * x117 * x199 * x56 - x127 * x206
                 + (1.0 / 2.0) * x133 * x192 * x56 * x96 - x136 * x208
                 - x138 * x210 - x140 * x201 - x200 * x209)
        + x156
              * (x152 * x185 + x189 * x190 + x19 * (-x169 - x187 - x188 - x37)
                 + x192 * x193)
        + x203 * x204;
  const float x212 = 2 * a_2;
  const float x213 = -2 * b_2 + x212;
  const float x214 = x14 * x65;
  const float x215 = x14 * x20;
  const float x216 = x212 * x8;
  const float x217 = a_2 * x24;
  const float x218 = x212 * x65;
  const float x219 = a_2 * x148;
  const float x220 = b_0 * x14;
  const float x221 = a_0 * x220;
  const float x222 = a_0 * x41;
  const float x223 = c_0 * x220;
  const float x224 = b_0 * x41;
  const float x225 = a_0 * x45 + x58;
  const float x226 = c_0 * x14;
  const float x227 = a_0 * x226 + x60;
  const float x228 = -a_2 * x144 - a_2 * x166 - x214 - x215 + x216 + x217 + x218
                     + x219 - x221 - x222 + x223 + x224 + x225 + x227 - x57
                     - x61 + x63 + x64 - x70 - x71;
  const float x229 = -x228;
  const float x230 = -x45;
  const float x231 = -x226;
  const float x232 = -x213;
  const float x233 = 4 * a_2;
  const float x234 = -4 * b_2 + x233;
  const float x235 = -x234;
  const float x236 = x196 * x92;
  const float x237 = -x111;
  const float x238 = x237 * x95;
  const float x239 = x236 + x238;
  const float x240 = -x90;
  const float x241 = (1.0 / 2.0) * x236 + (1.0 / 2.0) * x238;
  const float x242
      = x77
        * ((1.0 / 2.0) * x100 * x239 + (1.0 / 2.0) * x117 * x232 * x96
           - x120 * x240 - x124 * x241);
  const float x243 = x19 * x228 + x232 * x75;
  const float x244 = 3 * a_2 - 3 * b_2;
  const float x245 = -x244;
  const float x246 = -x241;
  const float x247
      = x106 * x213 + x107 * x242 + x109 * x229
        + x126
              * ((1.0 / 2.0) * x100 * x114 * x205
                 + (1.0 / 2.0) * x116 * x117 * x232
                 + (1.0 / 2.0) * x117 * x239 * x56 - x128 * x206
                 + (1.0 / 2.0) * x133 * x235 * x56 * x96 - x136 * x245
                 - x138 * x246 - x140 * x241 - x209 * x240)
        + x156
              * (x152 * x228 + x19 * (-x220 - x230 - x231 - x41) + x190 * x232
                 + x193 * x235)
        + x204 * x243;
  const float x248 = x194 * x6;
  const float x249 = x194 * x7;
  const float x250 = c_1 * x165;
  const float x251 = c_2 * x165;
  const float x252 = a_0 * x12;
  const float x253 = a_0 * x15;
  const float x254 = a_0 * x59;
  const float x255 = a_2 * x40;
  const float x256 = a_0 * x255;
  const float x257 = a_0 * x62 + a_0 * x66;
  const float x258 = -a_1 * x250 - a_2 * x251 + x21 + x23 - x248 - x249 - x252
                     - x253 + x254 + x256 + x257 - x30 - x31 + x38 + x42 - x47
                     - x48 + x51 + x69 + x72;
  const float x259 = -x258;
  const float x260 = x110 + x157;
  const float x261 = -x260;
  const float x262 = x261 * x86;
  const float x263 = -x113 - x212;
  const float x264 = x263 * x92;
  const float x265 = x262 + x264;
  const float x266 = (1.0 / 2.0) * x262 + (1.0 / 2.0) * x264;
  const float x267
      = x77
        * ((1.0 / 2.0) * x100 * x265 + (1.0 / 2.0) * x117 * x2 * x96
           - x120 * x78 - x124 * x266);
  const float x268 = x19 * x258 + x2 * x75;
  const float x269 = -x80;
  const float x270 = -x88;
  const float x271 = x114 * x270;
  const float x272 = x128 * x270;
  const float x273 = -x266;
  const float x274 = x120 - x141;
  const float x275 = -x255;
  const float x276 = x15 + x275;
  const float x277 = x276 - x66;
  const float x278 = -x59;
  const float x279 = x12 + x278;
  const float x280 = x279 - x62;
  const float x281
      = x106 * x56 + x107 * x267 + x109 * x259
        + x126
              * ((1.0 / 2.0) * x100 * (x111 * x269 + x271)
                 + (1.0 / 2.0) * x116 * x117 * x2
                 + (1.0 / 2.0) * x117 * x265 * x56 - x124 * (x127 * x269 + x272)
                 + (1.0 / 2.0) * x131 * x133 * x56 * x96 - x134 * x136
                 - x138 * x273 - x140 * x266 - x209 * x78 - x274)
        + x156
              * (x131 * x193 + x152 * x258 + x153 + x19 * (-x150 - x277 - x280)
                 + x190 * x2)
        + x204 * x268;
  const float x282 = x36 * x5;
  const float x283 = x36 * x7;
  const float x284 = x11 * x5;
  const float x285 = x11 * x7;
  const float x286 = b_1 * x32;
  const float x287 = c_2 * x143;
  const float x288 = a_1 * x3;
  const float x289 = a_1 * x15;
  const float x290 = c_0 * x0;
  const float x291 = a_1 * x290;
  const float x292 = a_1 * x255;
  const float x293 = a_1 * x1;
  const float x294 = a_1 * x66 + c_0 * x293;
  const float x295 = -a_2 * x287 - c_0 * x286 + x159 + x160 - x163 - x164 + x171
                     + x175 - x177 - x179 + x181 - x282 - x283 + x284 + x285
                     - x288 - x289 + x291 + x292 + x294;
  const float x296 = -x295;
  const float x297 = -x293;
  const float x298 = c_0 * x143;
  const float x299 = -x298 + x43;
  const float x300 = a_1 * x194;
  const float x301 = x300 - x33;
  const float x302 = x0 + x195;
  const float x303 = x302 * x86;
  const float x304 = x263 * x95;
  const float x305 = x303 + x304;
  const float x306 = (1.0 / 2.0) * x303 + (1.0 / 2.0) * x304;
  const float x307
      = x77
        * ((1.0 / 2.0) * x100 * x305 + (1.0 / 2.0) * x117 * x158 * x96
           - x120 * x84 - x124 * x306);
  const float x308 = x158 * x75 + x19 * x295;
  const float x309 = 2 * x85;
  const float x310 = -x309;
  const float x311 = 2 * x81;
  const float x312 = x310 + x311;
  const float x313 = -x306;
  const float x314
      = x106 * x189 + x107 * x307 + x109 * x296
        + x126
              * ((1.0 / 2.0) * x100 * (x111 * x83 + x312)
                 + (1.0 / 2.0) * x116 * x117 * x158
                 + (1.0 / 2.0) * x117 * x305 * x56 - x124 * (x127 * x83 + x86)
                 + (1.0 / 2.0) * x133 * x191 * x56 * x96 - x136 * x207
                 - x138 * x313 - x140 * x306 - x209 * x84)
        + x156
              * (x152 * x295 + x158 * x190
                 + x19 * (x286 + x297 + x299 + x301 + x37) + x191 * x193)
        + x204 * x308;
  const float x315 = x40 * x5;
  const float x316 = x40 * x6;
  const float x317 = x14 * x5;
  const float x318 = x14 * x6;
  const float x319 = b_2 * x32;
  const float x320 = b_2 * x167;
  const float x321 = a_2 * x3;
  const float x322 = a_2 * x12;
  const float x323 = a_2 * x290;
  const float x324 = a_2 * x59;
  const float x325 = a_2 * x1;
  const float x326 = a_2 * x62 + c_0 * x325;
  const float x327 = -c_0 * x319 - c_1 * x320 + x214 + x215 - x218 - x219 + x222
                     - x224 + x225 - x315 - x316 + x317 + x318 - x321 - x322
                     + x323 + x324 + x326 + x61 - x64;
  const float x328 = -x327;
  const float x329 = -x325;
  const float x330 = c_0 * x146;
  const float x331 = -x330 + x45;
  const float x332 = a_2 * x194;
  const float x333 = x332 - x34;
  const float x334 = x302 * x92;
  const float x335 = x260 * x95;
  const float x336 = x334 + x335;
  const float x337 = (1.0 / 2.0) * x334 + (1.0 / 2.0) * x335;
  const float x338
      = x77
        * ((1.0 / 2.0) * x100 * x336 + (1.0 / 2.0) * x117 * x213 * x96
           - x120 * x90 - x124 * x337);
  const float x339 = x19 * x327 + x213 * x75;
  const float x340 = 2 * x91;
  const float x341 = -x340;
  const float x342 = 2 * x89;
  const float x343 = x341 + x342;
  const float x344 = -x337;
  const float x345
      = x106 * x232 + x107 * x338 + x109 * x328
        + x126
              * ((1.0 / 2.0) * x100 * (x114 * x83 + x343)
                 + (1.0 / 2.0) * x116 * x117 * x213
                 + (1.0 / 2.0) * x117 * x336 * x56 - x124 * (x128 * x83 + x92)
                 + (1.0 / 2.0) * x133 * x234 * x56 * x96 - x136 * x244
                 - x138 * x344 - x140 * x337 - x209 * x90)
        + x156
              * (x152 * x327 + x19 * (x319 + x329 + x331 + x333 + x41)
                 + x190 * x213 + x193 * x234)
        + x204 * x339;
  const float x346 = x62 + x66;
  const float x347 = -x26 + x276;
  const float x348 = -x24 + x279;
  const float x349 = c_0 * x167;
  const float x350 = c_0 * x233;
  const float x351 = -b_1 * x349 - b_2 * x350 + x248 + x249 + x25 + x252 + x253
                     - x254 - x256 + x257 + x27 - x28 - x29 + x35 + x39 - x44
                     - x46 + x54 - x69 - x72;
  const float x352 = x114 * x90;
  const float x353 = x158 * x86;
  const float x354 = x213 * x92;
  const float x355 = x353 + x354;
  const float x356 = x128 * x90;
  const float x357 = (1.0 / 2.0) * x353 + (1.0 / 2.0) * x354;
  const float x358 = -x357;
  const float x359 = (1.0 / 2.0) * x100 * x355 - x124 * x357;
  const float x360 = x359 * x77;
  const float x361 = -x351;
  const float x362
      = x107 * x360 + x109 * x361 + x125 * x351
        + x126
              * ((1.0 / 2.0) * x100 * (x111 * x84 + x352)
                 + (1.0 / 2.0) * x117 * x355 * x56 - x124 * (x127 * x84 + x356)
                 - x138 * x358 - x140 * x357)
        + x156 * (x152 * x351 + x19 * (x346 + x347 + x348));
  const float x363 = -x300;
  const float x364 = x182 - x250;
  const float x365 = -x286 + x293;
  const float x366 = c_1 * x233;
  const float x367 = -b_0 * x33 - b_2 * x366 - x161 - x162 + x170 + x173 - x176
                     - x178 + x184 + x282 + x283 - x284 - x285 + x288 + x289
                     - x291 - x292 + x294 + x67 + x68;
  const float x368 = x309 - x311;
  const float x369 = x56 * x86;
  const float x370 = x213 * x95;
  const float x371 = x369 + x370;
  const float x372 = -x81 + x85;
  const float x373 = (1.0 / 2.0) * x369 + (1.0 / 2.0) * x370;
  const float x374 = -x373;
  const float x375 = (1.0 / 2.0) * x100 * x371 - x124 * x373;
  const float x376 = x375 * x77;
  const float x377 = -x367;
  const float x378
      = x107 * x376 + x109 * x377 + x125 * x367
        + x126
              * ((1.0 / 2.0) * x100 * (x111 * x118 + x368)
                 + (1.0 / 2.0) * x117 * x371 * x56 - x124 * (x118 * x127 + x372)
                 - x138 * x374 - x140 * x373)
        + x156 * (x152 * x367 + x19 * (x169 + x33 + x363 + x364 + x365));
  const float x379 = -x332;
  const float x380 = x226 - x251;
  const float x381 = -x319 + x325;
  const float x382 = -b_0 * x34 - b_1 * x168 - x216 - x217 + x221 - x223 + x227
                     + x315 + x316 - x317 - x318 + x321 + x322 - x323 - x324
                     + x326 + x57 - x63 + x70 + x71;
  const float x383 = x340 - x342;
  const float x384 = x56 * x92;
  const float x385 = x189 * x95;
  const float x386 = x384 + x385;
  const float x387 = -x89 + x91;
  const float x388 = (1.0 / 2.0) * x384 + (1.0 / 2.0) * x385;
  const float x389 = -x388;
  const float x390 = (1.0 / 2.0) * x100 * x386 - x124 * x388;
  const float x391 = x390 * x77;
  const float x392 = -x382;
  const float x393 = x108 * x392;
  const float x394
      = x107 * x391 + x107 * x393 + x125 * x382
        + x126
              * ((1.0 / 2.0) * x100 * (x114 * x118 + x383)
                 + (1.0 / 2.0) * x117 * x386 * x56 - x124 * (x118 * x128 + x387)
                 - x138 * x389 - x140 * x388)
        + x156 * (x152 * x382 + x19 * (x220 + x34 + x379 + x380 + x381));
  const float x395 = x105 * x203;
  const float x396 = x18 * x203;
  const float x397 = x108 * x396;
  const float x398 = x196 * x205;
  const float x399 = ((x205) * (x205));
  const float x400 = x135 * x200;
  const float x401 = x137 * x201;
  const float x402 = x139 * x200;
  const float x403 = -x166;
  const float x404 = 2 * x65;
  const float x405 = x149 + x404;
  const float x406 = 2 * x8;
  const float x407 = x26 + x406;
  const float x408 = x189 * x74;
  const float x409 = x155 * x189;
  const float x410 = -x183;
  const float x411 = -x180;
  const float x412 = x185 * x74;
  const float x413 = x18 * x202;
  const float x414 = -x127;
  const float x415 = x139 * x201;
  const float x416
      = x126
            * ((1.0 / 2.0) * x100 * x114 * x414
               + (1.0 / 2.0) * x117 * x189 * x239
               + (1.0 / 2.0) * x117 * x199 * x232 - x124 * x128 * x414
               + (1.0 / 2.0) * x133 * x189 * x235 * x96 - x240 * x415
               - x241 * x402 - x245 * x400 - x246 * x401)
        + x156
              * (x19 * (-x172 - x174 - x410 - x411) + x228 * x408 + x232 * x412
                 + x235 * x409)
        + x213 * x395 + x229 * x397 + x242 * x396 + x243 * x413;
  const float x417 = a_0 * x11;
  const float x418 = -x417;
  const float x419 = a_1 * x165;
  const float x420 = a_0 * x36;
  const float x421 = -x349 + x420;
  const float x422
      = x126
            * ((1.0 / 2.0) * x100 * (x196 * x269 + x368)
               + (1.0 / 2.0) * x117 * x189 * x265
               + (1.0 / 2.0) * x117 * x199 * x2 - x124 * (x205 * x269 + x372)
               + (1.0 / 2.0) * x131 * x133 * x189 * x96 - x134 * x400
               - x266 * x402 - x273 * x401 - x415 * x78)
        + x156
              * (x131 * x409 + x19 * (x364 + x37 + x418 + x419 + x421)
                 + x2 * x412 + x258 * x408)
        + x259 * x397 + x267 * x396 + x268 * x413 + x395 * x56;
  const float x423 = x196 * x83;
  const float x424 = x205 * x83;
  const float x425 = c_0 * x1;
  const float x426 = -x290;
  const float x427 = x3 + x426;
  const float x428 = -x425 + x427;
  const float x429
      = x126
            * ((1.0 / 2.0) * x100 * (x271 + x423)
               + (1.0 / 2.0) * x117 * x158 * x199
               + (1.0 / 2.0) * x117 * x189 * x305 - x124 * (x272 + x424)
               + (1.0 / 2.0) * x133 * x189 * x191 * x96 - x207 * x400 - x274
               - x306 * x402 - x313 * x401 - x415 * x84)
        + x156
              * (x153 + x158 * x412 + x19 * (-x277 - x405 - x428) + x191 * x409
                 + x295 * x408)
        + x189 * x395 + x296 * x397 + x307 * x396 + x308 * x413;
  const float x430 = a_2 * x11;
  const float x431 = -x430;
  const float x432 = c_1 * x146;
  const float x433 = x183 - x432;
  const float x434 = a_2 * x36;
  const float x435 = -x168 + x434;
  const float x436 = 2 * x94;
  const float x437 = -x436;
  const float x438 = 2 * x93;
  const float x439 = x437 + x438;
  const float x440
      = x126
            * ((1.0 / 2.0) * x100 * (x114 * x80 + x439)
               + (1.0 / 2.0) * x117 * x189 * x336
               + (1.0 / 2.0) * x117 * x199 * x213 - x124 * (x128 * x80 + x95)
               + (1.0 / 2.0) * x133 * x189 * x234 * x96 - x244 * x400
               - x337 * x402 - x344 * x401 - x415 * x90)
        + x156
              * (x19 * (x174 + x320 + x431 + x433 + x435) + x213 * x412
                 + x234 * x409 + x327 * x408)
        + x232 * x395 + x328 * x397 + x338 * x396 + x339 * x413;
  const float x441 = -x420;
  const float x442 = x417 - x419;
  const float x443
      = x126
            * ((1.0 / 2.0) * x100 * (x196 * x84 + x312)
               + (1.0 / 2.0) * x117 * x189 * x355 - x124 * (x205 * x84 + x86)
               - x357 * x402 - x358 * x401)
        + x156 * (x19 * (x169 + x299 + x349 + x441 + x442) + x351 * x408)
        + x202 * x351 + x360 * x396 + x361 * x397;
  const float x444 = x425 + x66;
  const float x445 = -x406 + x427;
  const float x446 = x118 * x196;
  const float x447 = x118 * x205;
  const float x448 = x126
                         * ((1.0 / 2.0) * x100 * (x352 + x446)
                            + (1.0 / 2.0) * x117 * x189 * x371
                            - x124 * (x356 + x447) - x373 * x402 - x374 * x401)
                     + x156 * (x19 * (x347 + x444 + x445) + x367 * x408)
                     + x202 * x367 + x376 * x396 + x377 * x397;
  const float x449 = -x434;
  const float x450 = x180 - x287;
  const float x451 = -x320 + x430;
  const float x452 = x436 - x438;
  const float x453 = -x93 + x94;
  const float x454
      = x126
            * ((1.0 / 2.0) * x100 * (x114 * x200 + x452)
               + (1.0 / 2.0) * x117 * x189 * x386 - x124 * (x128 * x200 + x453)
               - x388 * x402 - x389 * x401)
        + x156 * (x19 * (x168 + x172 + x449 + x450 + x451) + x382 * x408)
        + x202 * x382 + x391 * x396 + x393 * x396;
  const float x455 = x105 * x243;
  const float x456 = x18 * x243;
  const float x457 = x108 * x456;
  const float x458 = x18 * x242;
  const float x459 = x135 * x240;
  const float x460 = x137 * x241;
  const float x461 = x139 * x241;
  const float x462 = x148 + x404;
  const float x463 = x24 + x406;
  const float x464 = x232 * x74;
  const float x465 = x155 * x232;
  const float x466 = a_0 * x14;
  const float x467 = -x466;
  const float x468 = a_2 * x165;
  const float x469 = a_0 * x40;
  const float x470 = -x350 + x469;
  const float x471 = x228 * x74;
  const float x472 = x139 * x240;
  const float x473
      = x126
            * ((1.0 / 2.0) * x100 * (x196 * x270 + x383)
               + (1.0 / 2.0) * x117 * x2 * x239
               + (1.0 / 2.0) * x117 * x232 * x265 - x124 * (x205 * x270 + x387)
               + (1.0 / 2.0) * x131 * x133 * x232 * x96 - x134 * x459
               - x266 * x472 - x273 * x460 - x461 * x78)
        + x156
              * (x131 * x465 + x19 * (x380 + x41 + x467 + x468 + x470)
                 + x2 * x471 + x258 * x464)
        + x259 * x457 + x267 * x456 + x268 * x458 + x455 * x56;
  const float x474 = a_1 * x14;
  const float x475 = -x474;
  const float x476 = a_2 * x143;
  const float x477 = a_1 * x40;
  const float x478 = -x366 + x477;
  const float x479
      = x126
            * ((1.0 / 2.0) * x100 * (x237 * x270 + x452)
               + (1.0 / 2.0) * x117 * x158 * x239
               + (1.0 / 2.0) * x117 * x232 * x305 - x124 * (x270 * x414 + x453)
               + (1.0 / 2.0) * x133 * x191 * x232 * x96 - x207 * x459
               - x306 * x472 - x313 * x460 - x461 * x84)
        + x156
              * (x158 * x471 + x19 * (x174 + x450 + x475 + x476 + x478)
                 + x191 * x465 + x295 * x464)
        + x189 * x455 + x296 * x457 + x307 * x456 + x308 * x458;
  const float x480
      = x126
            * ((1.0 / 2.0) * x100 * (x237 * x80 + x423)
               + (1.0 / 2.0) * x117 * x213 * x239
               + (1.0 / 2.0) * x117 * x232 * x336 - x124 * (x414 * x80 + x424)
               + (1.0 / 2.0) * x133 * x232 * x234 * x96 - x244 * x459 - x274
               - x337 * x472 - x344 * x460 - x461 * x90)
        + x156
              * (x153 + x19 * (-x280 - x428 - x462) + x213 * x471 + x234 * x465
                 + x327 * x464)
        + x232 * x455 + x328 * x457 + x338 * x456 + x339 * x458;
  const float x481 = -x469;
  const float x482 = x466 - x468;
  const float x483
      = x126
            * ((1.0 / 2.0) * x100 * (x196 * x90 + x343)
               + (1.0 / 2.0) * x117 * x232 * x355 - x124 * (x205 * x90 + x92)
               - x357 * x472 - x358 * x460)
        + x156 * (x19 * (x220 + x331 + x350 + x481 + x482) + x351 * x464)
        + x242 * x351 + x360 * x456 + x361 * x457;
  const float x484 = -x477;
  const float x485 = x474 - x476;
  const float x486
      = x126
            * ((1.0 / 2.0) * x100 * (x237 * x90 + x439)
               + (1.0 / 2.0) * x117 * x232 * x371 - x124 * (x414 * x90 + x95)
               - x373 * x472 - x374 * x460)
        + x156 * (x19 * (x172 + x366 + x433 + x484 + x485) + x367 * x464)
        + x242 * x367 + x376 * x456 + x377 * x457;
  const float x487 = x425 + x62;
  const float x488
      = x126
            * ((1.0 / 2.0) * x100 * (x200 * x237 + x446)
               + (1.0 / 2.0) * x117 * x232 * x386 - x124 * (x200 * x414 + x447)
               - x388 * x472 - x389 * x460)
        + x156 * (x19 * (x348 + x445 + x487) + x382 * x464) + x242 * x382
        + x391 * x456 + x393 * x456;
  const float x489 = x105 * x268;
  const float x490 = x18 * x268;
  const float x491 = x108 * x490;
  const float x492 = x263 * x270;
  const float x493 = ((x270) * (x270));
  const float x494 = x135 * x78;
  const float x495 = x137 * x266;
  const float x496 = x139 * x78;
  const float x497 = 2 * x7;
  const float x498 = -c_2 * x233 + x497;
  const float x499 = 2 * x6;
  const float x500 = -c_1 * x167 + x499;
  const float x501 = x2 * x74;
  const float x502 = x155 * x2;
  const float x503 = a_1 * x0;
  const float x504 = x258 * x74;
  const float x505 = x18 * x267;
  const float x506 = x124 * x83;
  const float x507 = x139 * x266;
  const float x508
      = x126
            * ((1.0 / 2.0) * x100 * x261 * x83
               + (1.0 / 2.0) * x117 * x158 * x265
               + (1.0 / 2.0) * x117 * x2 * x305
               + (1.0 / 2.0) * x133 * x191 * x2 * x96 - x207 * x494
               - x269 * x506 - x306 * x496 - x313 * x495 - x507 * x84)
        + x156
              * (x158 * x504 + x19 * (-x363 - x37 - x441 - x503) + x191 * x502
                 + x295 * x501)
        + x189 * x489 + x296 * x491 + x307 * x490 + x308 * x505;
  const float x509 = a_2 * x0;
  const float x510
      = x126
            * ((1.0 / 2.0) * x100 * x263 * x83 + (1.0 / 2.0) * x117 * x2 * x336
               + (1.0 / 2.0) * x117 * x213 * x265
               + (1.0 / 2.0) * x133 * x2 * x234 * x96 - x244 * x494
               - x270 * x506 - x337 * x496 - x344 * x495 - x507 * x90)
        + x156
              * (x19 * (-x379 - x41 - x481 - x509) + x213 * x504 + x234 * x502
                 + x327 * x501)
        + x232 * x489 + x328 * x491 + x338 * x490 + x339 * x505;
  const float x511 = x275 + x497;
  const float x512 = x278 + x499;
  const float x513 = x263 * x90;
  const float x514 = x270 * x90;
  const float x515
      = x126
            * ((1.0 / 2.0) * x100 * (x261 * x84 + x513)
               + (1.0 / 2.0) * x117 * x2 * x355 - x124 * (x269 * x84 + x514)
               - x357 * x496 - x358 * x495)
        + x156 * (x19 * (-x17 - x346 - x511 - x512) + x351 * x501) + x267 * x351
        + x360 * x490 + x361 * x491;
  const float x516
      = x126
            * ((1.0 / 2.0) * x100 * (x118 * x261 + x312)
               + (1.0 / 2.0) * x117 * x2 * x371 - x124 * (x118 * x269 + x86)
               - x373 * x496 - x374 * x495)
        + x156 * (x19 * (x188 + x250 + x301 + x442 + x503) + x367 * x501)
        + x267 * x367 + x376 * x490 + x377 * x491;
  const float x517
      = x126
            * ((1.0 / 2.0) * x100 * (x118 * x263 + x343)
               + (1.0 / 2.0) * x117 * x2 * x386 - x124 * (x118 * x270 + x92)
               - x388 * x496 - x389 * x495)
        + x156 * (x19 * (x231 + x251 + x333 + x482 + x509) + x382 * x501)
        + x267 * x382 + x391 * x490 + x393 * x490;
  const float x518 = x105 * x308;
  const float x519 = x18 * x308;
  const float x520 = x108 * x519;
  const float x521 = x302 * x83;
  const float x522 = ((x83) * (x83));
  const float x523 = x135 * x84;
  const float x524 = x137 * x306;
  const float x525 = x139 * x84;
  const float x526 = 2 * x5;
  const float x527 = -c_0 * x32 + x526;
  const float x528 = x158 * x74;
  const float x529 = x155 * x158;
  const float x530 = a_2 * x157;
  const float x531 = x213 * x74;
  const float x532 = x18 * x339;
  const float x533 = x139 * x90;
  const float x534
      = x126
            * ((1.0 / 2.0) * x100 * x263 * x80
               + (1.0 / 2.0) * x117 * x158 * x336
               + (1.0 / 2.0) * x117 * x213 * x305 - x124 * x270 * x80
               + (1.0 / 2.0) * x133 * x158 * x234 * x96 - x244 * x523
               - x306 * x533 - x337 * x525 - x344 * x524)
        + x156
              * (x19 * (-x174 - x449 - x484 - x530) + x234 * x529 + x295 * x531
                 + x327 * x528)
        + x232 * x518 + x307 * x532 + x328 * x520 + x338 * x519;
  const float x535
      = x126
            * ((1.0 / 2.0) * x100 * (x302 * x84 + x368)
               + (1.0 / 2.0) * x117 * x158 * x355 - x124 * (-x310 - x81)
               - x357 * x525 - x358 * x524)
        + x156 * (x19 * (x187 + x298 + x365 + x421 + x503) + x351 * x528)
        + x307 * x351 + x360 * x519 + x361 * x520;
  const float x536 = x4 + x426 + x526;
  const float x537 = x118 * x302;
  const float x538 = x118 * x83;
  const float x539 = x126
                         * ((1.0 / 2.0) * x100 * (x513 + x537)
                            + (1.0 / 2.0) * x117 * x158 * x371
                            - x124 * (x514 + x538) - x373 * x525 - x374 * x524)
                     + x156 * (x19 * (-x16 - x444 - x511 - x536) + x367 * x528)
                     + x307 * x367 + x376 * x519 + x377 * x520;
  const float x540
      = x126
            * ((1.0 / 2.0) * x100 * (x200 * x263 + x439)
               + (1.0 / 2.0) * x117 * x158 * x386 - x124 * (x200 * x270 + x95)
               - x388 * x525 - x389 * x524)
        + x156 * (x19 * (x287 + x411 + x435 + x485 + x530) + x382 * x528)
        + x307 * x382 + x391 * x519 + x393 * x519;
  const float x541 = x108 * x532;
  const float x542 = x137 * x337;
  const float x543
      = x126
            * ((1.0 / 2.0) * x100 * (x302 * x90 + x383)
               + (1.0 / 2.0) * x117 * x213 * x355 - x124 * (-x341 - x89)
               - x357 * x533 - x358 * x542)
        + x156 * (x19 * (x230 + x330 + x381 + x470 + x509) + x351 * x531)
        + x338 * x351 + x360 * x532 + x361 * x541;
  const float x544
      = x126
            * ((1.0 / 2.0) * x100 * (x260 * x90 + x452)
               + (1.0 / 2.0) * x117 * x213 * x371 - x124 * (-x437 - x93)
               - x373 * x533 - x374 * x542)
        + x156 * (x19 * (x410 + x432 + x451 + x478 + x530) + x367 * x531)
        + x338 * x367 + x376 * x532 + x377 * x541;
  const float x545
      = x126
            * ((1.0 / 2.0) * x100 * (x200 * x260 + x537)
               + (1.0 / 2.0) * x117 * x213 * x386 - x124 * (x200 * x80 + x538)
               - x388 * x533 - x389 * x542)
        + x156 * (x19 * (-x13 - x487 - x512 - x536) + x382 * x531) + x338 * x382
        + x391 * x532 + x393 * x532;
  const float x546 = -b_2 * x233 + x497;
  const float x547 = -b_1 * x167 + x499;
  const float x548 = x351 * x77;
  const float x549 = x213 * x90;
  const float x550 = x137 * x357;
  const float x551 = x108 * x351;
  const float x552 = x118 * x124;
  const float x553
      = x105 * (-x169 - x297 - x418 - x503)
        + x126 * ((1.0 / 2.0) * x100 * x118 * x158 - x374 * x550 - x552 * x84)
        + x360 * x367 + x375 * x548 + x377 * x551;
  const float x554
      = x105 * (-x220 - x329 - x467 - x509)
        + x126 * ((1.0 / 2.0) * x100 * x118 * x213 - x389 * x550 - x552 * x90)
        + x360 * x382 + x390 * x548 + x392 * x551;
  const float x555 = -b_0 * x32 + x526;
  const float x556 = x118 * x56;
  const float x557 = ((x118) * (x118));
  const float x558 = x137 * x373;
  const float x559 = x108 * x367;
  const float x560 = x105 * (-x172 - x431 - x475 - x530)
                     + x126
                           * ((1.0 / 2.0) * x100 * x200 * x213
                              - x124 * x200 * x90 - x389 * x558)
                     + x367 * x391 + x376 * x382 + x392 * x559;

  out_2823277736450422735[0]
      = x106 * x2 + 2 * x107 * x125 - x109 * x55
        + x126
              * ((1.0 / 2.0) * x100 * (x111 * x127 + x129) + x116 * x117 * x56
                 + x121 * x138 - 2 * x121 * x140
                 - x124 * (((x127) * (x127)) + x130)
                 + (1.0 / 2.0) * x132 * x133 * x56 * x96 + x134 * x136 - x142)
        + x156
              * (x132 * x155 * x56 + 2 * x152 * x55 + x154
                 + x19 * (x145 + x147 + x150 + x151));
  out_2823277736450422735[1] = x211;
  out_2823277736450422735[2] = x247;
  out_2823277736450422735[3] = x281;
  out_2823277736450422735[4] = x314;
  out_2823277736450422735[5] = x345;
  out_2823277736450422735[6] = x362;
  out_2823277736450422735[7] = x378;
  out_2823277736450422735[8] = x394;
  out_2823277736450422735[9] = x211;
  out_2823277736450422735[10]
      = x126
            * ((1.0 / 2.0) * x100 * (x129 + x398) + x117 * x189 * x199
               - x124 * (x130 + x399) + (1.0 / 2.0) * x133 * x189 * x192 * x96
               - x142 - 2 * x201 * x402 - x208 * x400 - x210 * x401)
        + x156
              * (x154 + 2 * x185 * x408 + x19 * (x147 + x403 + x405 + x407)
                 + x192 * x409)
        + x158 * x395 + x186 * x397 + 2 * x202 * x396;
  out_2823277736450422735[11] = x416;
  out_2823277736450422735[12] = x422;
  out_2823277736450422735[13] = x429;
  out_2823277736450422735[14] = x440;
  out_2823277736450422735[15] = x443;
  out_2823277736450422735[16] = x448;
  out_2823277736450422735[17] = x454;
  out_2823277736450422735[18] = x247;
  out_2823277736450422735[19] = x416;
  out_2823277736450422735[20]
      = x126
            * ((1.0 / 2.0) * x100 * (x237 * x414 + x398) + x117 * x232 * x239
               - x124 * (x399 + ((x414) * (x414)))
               + (1.0 / 2.0) * x133 * x232 * x235 * x96 - x142 - 2 * x240 * x461
               - x245 * x459 - x246 * x460)
        + x156
              * (x154 + x19 * (x145 + x403 + x462 + x463) + 2 * x228 * x464
                 + x235 * x465)
        + x213 * x455 + x229 * x457 + 2 * x243 * x458;
  out_2823277736450422735[21] = x473;
  out_2823277736450422735[22] = x479;
  out_2823277736450422735[23] = x480;
  out_2823277736450422735[24] = x483;
  out_2823277736450422735[25] = x486;
  out_2823277736450422735[26] = x488;
  out_2823277736450422735[27] = x281;
  out_2823277736450422735[28] = x422;
  out_2823277736450422735[29] = x473;
  out_2823277736450422735[30]
      = x126
            * ((1.0 / 2.0) * x100 * (x261 * x269 + x492) + x117 * x2 * x265
               - x124 * (((x269) * (x269)) + x493)
               + (1.0 / 2.0) * x131 * x133 * x2 * x96 - x134 * x494 - x142
               - 2 * x266 * x496 - x273 * x495)
        + x156
              * (x131 * x502 + x154 + x19 * (x150 + x498 + x500)
                 + 2 * x258 * x501)
        + x259 * x491 + 2 * x267 * x490 + x489 * x56;
  out_2823277736450422735[31] = x508;
  out_2823277736450422735[32] = x510;
  out_2823277736450422735[33] = x515;
  out_2823277736450422735[34] = x516;
  out_2823277736450422735[35] = x517;
  out_2823277736450422735[36] = x314;
  out_2823277736450422735[37] = x429;
  out_2823277736450422735[38] = x479;
  out_2823277736450422735[39] = x508;
  out_2823277736450422735[40]
      = x126
            * ((1.0 / 2.0) * x100 * (x492 + x521) + x117 * x158 * x305
               - x124 * (x493 + x522) + (1.0 / 2.0) * x133 * x158 * x191 * x96
               - x142 - x207 * x523 - 2 * x306 * x525 - x313 * x524)
        + x156
              * (x154 + x19 * (x405 + x498 + x527) + x191 * x529
                 + 2 * x295 * x528)
        + x189 * x518 + x296 * x520 + 2 * x307 * x519;
  out_2823277736450422735[41] = x534;
  out_2823277736450422735[42] = x535;
  out_2823277736450422735[43] = x539;
  out_2823277736450422735[44] = x540;
  out_2823277736450422735[45] = x345;
  out_2823277736450422735[46] = x440;
  out_2823277736450422735[47] = x480;
  out_2823277736450422735[48] = x510;
  out_2823277736450422735[49] = x534;
  out_2823277736450422735[50]
      = x105 * x232 * x339
        + x126
              * ((1.0 / 2.0) * x100 * (x260 * x80 + x521) + x117 * x213 * x336
                 - x124 * (x522 + ((x80) * (x80)))
                 + (1.0 / 2.0) * x133 * x213 * x234 * x96 - x135 * x244 * x90
                 - x142 - 2 * x337 * x533 - x344 * x542)
        + x156
              * (x154 + x155 * x213 * x234 + x19 * (x462 + x500 + x527)
                 + 2 * x327 * x531)
        + x328 * x541 + 2 * x338 * x532;
  out_2823277736450422735[51] = x543;
  out_2823277736450422735[52] = x544;
  out_2823277736450422735[53] = x545;
  out_2823277736450422735[54] = x362;
  out_2823277736450422735[55] = x443;
  out_2823277736450422735[56] = x483;
  out_2823277736450422735[57] = x515;
  out_2823277736450422735[58] = x535;
  out_2823277736450422735[59] = x543;
  out_2823277736450422735[60]
      = x105 * (x151 + x546 + x547)
        + x126
              * ((1.0 / 2.0) * x100 * (x158 * x84 + x549) - x124 * x98
                 - x358 * x550)
        + 2 * x359 * x548 + x361 * x551;
  out_2823277736450422735[61] = x553;
  out_2823277736450422735[62] = x554;
  out_2823277736450422735[63] = x378;
  out_2823277736450422735[64] = x448;
  out_2823277736450422735[65] = x486;
  out_2823277736450422735[66] = x516;
  out_2823277736450422735[67] = x539;
  out_2823277736450422735[68] = x544;
  out_2823277736450422735[69] = x553;
  out_2823277736450422735[70] = x105 * (x407 + x546 + x555)
                                + x126
                                      * ((1.0 / 2.0) * x100 * (x549 + x556)
                                         - x124 * (x557 + x97) - x374 * x558)
                                + 2 * x367 * x376 + x377 * x559;
  out_2823277736450422735[71] = x560;
  out_2823277736450422735[72] = x394;
  out_2823277736450422735[73] = x454;
  out_2823277736450422735[74] = x488;
  out_2823277736450422735[75] = x517;
  out_2823277736450422735[76] = x540;
  out_2823277736450422735[77] = x545;
  out_2823277736450422735[78] = x554;
  out_2823277736450422735[79] = x560;
  out_2823277736450422735[80]
      = x105 * (x463 + x547 + x555)
        + x126
              * ((1.0 / 2.0) * x100 * (x189 * x200 + x556)
                 - x124 * (((x200) * (x200)) + x557) - x137 * x388 * x389)
        + 2 * x382 * x391 + x382 * x393;
}
