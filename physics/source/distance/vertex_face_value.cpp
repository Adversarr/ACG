/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'acgipc'                        *
 ******************************************************************************/
#include "aphysics/ccd/distances/vertex_face_value.hpp"

#include <math.h>

double vertex_face_value(double a_0, double a_1, double a_2, double b_0,
                         double b_1, double b_2, double c_0, double c_1,
                         double c_2, double d_0, double d_1, double d_2,
                         double d_h) {
  const double x0 = a_0 - b_0;
  const double x1 = a_1 - c_1;
  const double x2 = a_0 - c_0;
  const double x3 = a_1 - b_1;
  const double x4 = x0 * x1 - x2 * x3;
  const double x5 = a_2 - c_2;
  const double x6 = a_2 - b_2;
  const double x7 = x0 * x5 - x2 * x6;
  const double x8 = -x1 * x6 + x3 * x5;
  const double x9 = ((x4) * (x4)) + ((x7) * (x7)) + ((x8) * (x8));
  const double x10 = x4 * (a_2 - d_2) - x7 * (a_1 - d_1) + x8 * (a_0 - d_0);
  const double x11 = a_0 * b_1;
  const double x12 = a_0 * b_2;
  const double x13 = c_1 * d_2;
  const double x14 = a_1 * b_0;
  const double x15 = a_1 * b_2;
  const double x16 = c_2 * d_0;
  const double x17 = a_2 * b_0;
  const double x18 = a_2 * b_1;
  const double x19 = c_0 * d_1;
  const double x20 = c_2 * d_1;
  const double x21 = c_0 * d_2;
  const double x22 = c_1 * d_0;
  const double x23 = 2 * x11;
  const double x24 = 2 * a_0;
  const double x25 = c_1 * x14;
  const double x26 = 2 * a_1;
  const double x27 = c_0 * x24;
  const double x28 = 2 * x12;
  const double x29 = c_2 * x17;
  const double x30 = 2 * a_2;
  const double x31 = a_2 * c_2;
  const double x32 = c_1 * x23;
  const double x33 = c_2 * x28;
  const double x34 = 2 * x15;
  const double x35 = c_2 * x18;
  const double x36 = c_1 * x26;
  const double x37 = 2 * c_0;
  const double x38 = c_2 * x34;
  const double x39 = 2 * c_1;
  const double x40 = b_1 * c_1;
  const double x41 = b_0 * c_0;
  const double x42 = 2 * x41;
  const double x43 = b_2 * c_2;
  const double x44 = ((a_0) * (a_0));
  const double x45 = ((b_1) * (b_1));
  const double x46 = ((b_2) * (b_2));
  const double x47 = ((c_1) * (c_1));
  const double x48 = ((c_2) * (c_2));
  const double x49 = ((a_1) * (a_1));
  const double x50 = ((b_0) * (b_0));
  const double x51 = ((c_0) * (c_0));
  const double x52 = ((a_2) * (a_2));
  const double x53 = b_0 * x24;
  const double x54 = 2 * x44;
  const double x55 = b_1 * x26;
  const double x56 = 2 * x49;
  const double x57 = c_2 * x30;
  const double x58 = b_2 * x30;
  const double x59 = 2 * x52;

  double vertex_face_value_result;
  vertex_face_value_result
      = (-2 * log(d_h)
         + log(((a_0 * x13 - a_0 * x20 + a_1 * x16 - a_1 * x21 + a_2 * x19
                 - a_2 * x22 - b_0 * x13 + b_0 * x20 - b_1 * x16 + b_1 * x21
                 - b_2 * x19 + b_2 * x22 + c_0 * x15 - c_0 * x18 - c_1 * x12
                 + c_1 * x17 + c_2 * x11 - c_2 * x14 - d_0 * x15 + d_0 * x18
                 + d_1 * x12 - d_1 * x17 - d_2 * x11 + d_2 * x14)
                * (a_0 * x13 - a_0 * x20 + a_1 * x16 - a_1 * x21 + a_2 * x19
                   - a_2 * x22 - b_0 * x13 + b_0 * x20 - b_1 * x16 + b_1 * x21
                   - b_2 * x19 + b_2 * x22 + c_0 * x15 - c_0 * x18 - c_1 * x12
                   + c_1 * x17 + c_2 * x11 - c_2 * x14 - d_0 * x15 + d_0 * x18
                   + d_1 * x12 - d_1 * x17 - d_2 * x11 + d_2 * x14))
               / (-a_1 * c_1 * x27 + b_0 * x32 + b_0 * x33 + b_1 * x14 * x37
                  + b_1 * x38 + b_2 * x17 * x37 + b_2 * x18 * x39
                  + c_0 * x11 * x26 + c_0 * x12 * x30 + c_0 * x32 + c_0 * x33
                  + c_1 * x15 * x30 + c_1 * x38 - x14 * x23 - x17 * x28
                  - x18 * x34 + x24 * x25 + x24 * x29 + x25 * x37 + x26 * x35
                  - x27 * x31 - x27 * x45 - x27 * x46 + x29 * x37 - x31 * x36
                  + x35 * x39 - x36 * x46 - x36 * x50 - x40 * x42
                  - 2 * x40 * x43 - x40 * x54 - x40 * x59 - x41 * x56
                  - x41 * x59 - x42 * x43 - x43 * x54 - x43 * x56 + x44 * x45
                  + x44 * x46 + x44 * x47 + x44 * x48 + x45 * x48 + x45 * x51
                  + x45 * x52 - x45 * x57 + x46 * x47 + x46 * x49 + x46 * x51
                  + x47 * x50 + x47 * x52 - x47 * x53 - x47 * x58 + x48 * x49
                  + x48 * x50 - x48 * x53 - x48 * x55 + x49 * x50 + x49 * x51
                  + x50 * x52 - x50 * x57 + x51 * x52 - x51 * x55 - x51 * x58)))
        * ((1.0 / 2.0) * ((d_h) * (d_h)) + d_h * x10 / sqrt(x9)
           + (1.0 / 2.0) * ((x10) * (x10)) / x9);
  return vertex_face_value_result;
}

void vertex_face_grad(double a_0, double a_1, double a_2, double b_0,
                      double b_1, double b_2, double c_0, double c_1,
                      double c_2, double d_0, double d_1, double d_2,
                      double d_h, double *out_6543693310212660619) {
  const double x0 = a_0 * b_1;
  const double x1 = a_0 * b_2;
  const double x2 = c_1 * d_2;
  const double x3 = a_1 * b_0;
  const double x4 = a_1 * b_2;
  const double x5 = c_2 * d_0;
  const double x6 = a_2 * b_0;
  const double x7 = a_2 * b_1;
  const double x8 = c_0 * d_1;
  const double x9 = c_2 * d_1;
  const double x10 = c_0 * d_2;
  const double x11 = c_1 * d_0;
  const double x12 = a_0 * x2 - a_0 * x9 - a_1 * x10 + a_1 * x5 - a_2 * x11
                     + a_2 * x8 - b_0 * x2 + b_0 * x9 + b_1 * x10 - b_1 * x5
                     + b_2 * x11 - b_2 * x8 + c_0 * x4 - c_0 * x7 - c_1 * x1
                     + c_1 * x6 + c_2 * x0 - c_2 * x3 - d_0 * x4 + d_0 * x7
                     + d_1 * x1 - d_1 * x6 - d_2 * x0 + d_2 * x3;
  const double x13 = ((x12) * (x12));
  const double x14 = 2 * b_1;
  const double x15 = a_0 * x14;
  const double x16 = 2 * c_1;
  const double x17 = a_0 * x16;
  const double x18 = c_0 * x14;
  const double x19 = a_1 * x18;
  const double x20 = c_0 * x17;
  const double x21 = 2 * b_2;
  const double x22 = a_0 * x21;
  const double x23 = 2 * c_2;
  const double x24 = a_0 * x23;
  const double x25 = c_0 * x21;
  const double x26 = a_2 * x25;
  const double x27 = c_0 * x24;
  const double x28 = b_0 * x16;
  const double x29 = b_0 * x23;
  const double x30 = c_0 * x16;
  const double x31 = c_0 * x23;
  const double x32 = a_2 * x14;
  const double x33 = c_2 * x14;
  const double x34 = a_2 * x33;
  const double x35 = a_2 * x16;
  const double x36 = c_2 * x35;
  const double x37 = x16 * x3;
  const double x38 = x16 * x4;
  const double x39 = x23 * x6;
  const double x40 = b_2 * x16;
  const double x41 = c_2 * x16;
  const double x42 = b_1 * x28;
  const double x43 = b_2 * x29;
  const double x44 = c_2 * x40;
  const double x45 = ((a_0) * (a_0));
  const double x46 = ((b_1) * (b_1));
  const double x47 = ((b_2) * (b_2));
  const double x48 = ((c_1) * (c_1));
  const double x49 = ((c_2) * (c_2));
  const double x50 = ((a_1) * (a_1));
  const double x51 = ((b_0) * (b_0));
  const double x52 = ((c_0) * (c_0));
  const double x53 = ((a_2) * (a_2));
  const double x54 = 2 * b_0;
  const double x55 = x48 * x54;
  const double x56 = x49 * x54;
  const double x57 = 2 * c_0;
  const double x58 = x46 * x57;
  const double x59 = x47 * x57;
  const double x60 = x16 * x45;
  const double x61 = x23 * x45;
  const double x62 = x16 * x51;
  const double x63 = x14 * x52;
  const double x64 = x14 * x49;
  const double x65 = x16 * x47;
  const double x66 = x50 * x54;
  const double x67 = x23 * x50;
  const double x68 = x23 * x51;
  const double x69 = x23 * x46;
  const double x70 = x21 * x52;
  const double x71 = x21 * x48;
  const double x72 = x53 * x54;
  const double x73 = x16 * x53;
  const double x74
      = a_0 * x19 + a_0 * x26 - a_0 * x55 - a_0 * x56 - a_0 * x58 - a_0 * x59
        - a_1 * x20 + a_1 * x34 - a_1 * x36 - a_1 * x62 - a_1 * x63 - a_1 * x64
        - a_1 * x65 - a_2 * x27 - a_2 * x68 - a_2 * x69 - a_2 * x70 - a_2 * x71
        - b_1 * x44 - b_1 * x60 - b_1 * x73 - b_2 * x61 - b_2 * x67 + c_0 * x37
        + c_0 * x39 - c_0 * x42 - c_0 * x43 - c_0 * x66 - c_0 * x72 + c_2 * x38
        + x0 * x28 + x0 * x30 + x1 * x29 + x1 * x31 - x15 * x3 + x17 * x3
        + x18 * x3 - x22 * x6 + x24 * x6 + x25 * x6 - x32 * x4 + x33 * x4
        + x35 * x4 + x40 * x7 + x41 * x7 + x45 * x46 + x45 * x47 + x45 * x48
        + x45 * x49 + x46 * x49 + x46 * x52 + x46 * x53 + x47 * x48 + x47 * x50
        + x47 * x52 + x48 * x51 + x48 * x53 + x49 * x50 + x49 * x51 + x50 * x51
        + x50 * x52 + x51 * x53 + x52 * x53;
  const double x75 = 1.0 / x74;
  const double x76 = -2 * log(d_h) + log(x13 * x75);
  const double x77 = -c_1;
  const double x78 = a_1 + x77;
  const double x79 = a_2 - b_2;
  const double x80 = x78 * x79;
  const double x81 = a_1 - d_1;
  const double x82 = -c_2;
  const double x83 = b_2 + x82;
  const double x84 = a_2 - d_2;
  const double x85 = -x84;
  const double x86 = b_1 + x77;
  const double x87 = a_1 - b_1;
  const double x88 = a_2 + x82;
  const double x89 = x87 * x88;
  const double x90 = a_0 - b_0;
  const double x91 = x78 * x90;
  const double x92 = -c_0;
  const double x93 = a_0 + x92;
  const double x94 = x87 * x93;
  const double x95 = x91 - x94;
  const double x96 = x88 * x90;
  const double x97 = x79 * x93;
  const double x98 = x96 - x97;
  const double x99 = -x80 + x89;
  const double x100 = ((x95) * (x95)) + ((x98) * (x98)) + ((x99) * (x99));
  const double x101 = d_h / sqrt(x100);
  const double x102 = 2 * x84;
  const double x103 = -x81;
  const double x104 = 2 * x83;
  const double x105 = -2 * x80 + 2 * x89;
  const double x106 = 1.0 / x100;
  const double x107 = a_0 - d_0;
  const double x108 = x107 * x99 - x81 * x98 + x84 * x95;
  const double x109 = -x16;
  const double x110 = x109 + x14;
  const double x111 = x110 * x95;
  const double x112 = -x23;
  const double x113 = x112 + x21;
  const double x114 = x113 * x98;
  const double x115 = -x108;
  const double x116 = d_h * x115 / pow(x100, 3.0 / 2.0);
  const double x117 = pow(x100, -2);
  const double x118 = ((x108) * (x108));
  const double x119 = d_2 * x14;
  const double x120 = d_1 * x21;
  const double x121 = -d_1 * x23 + d_2 * x16;
  const double x122 = x33 - x40;
  const double x123 = x12 * x75;
  const double x124 = 2 * a_0;
  const double x125 = x124 * x46;
  const double x126 = x124 * x47;
  const double x127 = x124 * x48;
  const double x128 = x124 * x49;
  const double x129 = x14 * x3;
  const double x130 = a_1 * x30;
  const double x131 = x21 * x6;
  const double x132 = a_2 * x31;
  const double x133 = b_1 * x30;
  const double x134 = b_2 * x31;
  const double x135 = x19 + x26;
  const double x136 = x37 + x39;
  const double x137 = x13 / ((x74) * (x74));
  const double x138
      = (1.0 / 2.0) * ((d_h) * (d_h)) - x101 * x115 + (1.0 / 2.0) * x106 * x118;
  const double x139 = x138 * x74 / x13;
  const double x140 = -x107;
  const double x141 = -b_0 - x92;
  const double x142 = 2 * x96;
  const double x143 = 2 * x97;
  const double x144 = -x57;
  const double x145 = -x144 - x54;
  const double x146 = x145 * x95;
  const double x147 = x113 * x99;
  const double x148 = d_2 * x54;
  const double x149 = d_0 * x21;
  const double x150 = -d_0 * x23 + d_2 * x57;
  const double x151 = -x25 + x29;
  const double x152 = 2 * a_1;
  const double x153 = x152 * x51;
  const double x154 = x152 * x47;
  const double x155 = x152 * x52;
  const double x156 = x152 * x49;
  const double x157 = b_0 * x15;
  const double x158 = b_2 * x32;
  const double x159 = b_0 * x18;
  const double x160 = c_0 * x28;
  const double x161 = b_2 * x33;
  const double x162 = a_0 * x28 + a_2 * x40;
  const double x163 = a_0 * x18 + x34;
  const double x164 = -x86;
  const double x165 = 2 * x107;
  const double x166 = 2 * x103;
  const double x167 = 2 * x91 - 2 * x94;
  const double x168 = x145 * x98;
  const double x169 = -x110 * x99;
  const double x170 = d_1 * x54;
  const double x171 = d_0 * x14;
  const double x172 = -d_0 * x16 + d_1 * x57;
  const double x173 = -x18 + x28;
  const double x174 = 2 * a_2;
  const double x175 = x174 * x51;
  const double x176 = x174 * x46;
  const double x177 = x174 * x52;
  const double x178 = x174 * x48;
  const double x179 = b_0 * x22;
  const double x180 = x14 * x4;
  const double x181 = a_1 * x41;
  const double x182 = b_0 * x25;
  const double x183 = c_0 * x29;
  const double x184 = b_1 * x40;
  const double x185 = b_1 * x41;
  const double x186 = a_0 * x29 + a_1 * x33;
  const double x187 = a_0 * x25 + x38;
  const double x188 = -x78;
  const double x189 = -x88;
  const double x190 = x109 + x152;
  const double x191 = -x190 * x95;
  const double x192 = -x112 - x174;
  const double x193 = x192 * x98;
  const double x194 = a_1 * x23;
  const double x195 = d_1 * x174 - d_2 * x152;
  const double x196 = x50 * x57;
  const double x197 = x53 * x57;
  const double x198 = a_1 * x15;
  const double x199 = a_2 * x22;
  const double x200 = a_1 * x17;
  const double x201 = a_2 * x24;
  const double x202 = x0 * x16 + x1 * x23;
  const double x203 = x124 + x144;
  const double x204 = x203 * x95;
  const double x205 = x192 * x99;
  const double x206 = a_2 * x57;
  const double x207 = d_0 * x174 - d_2 * x124;
  const double x208 = x14 * x45;
  const double x209 = x14 * x53;
  const double x210 = a_1 * x54;
  const double x211 = a_0 * x210;
  const double x212 = a_1 * x21;
  const double x213 = a_2 * x212;
  const double x214 = a_1 * x57;
  const double x215 = a_0 * x214;
  const double x216 = a_2 * x194;
  const double x217 = c_0 * x210 + x23 * x4;
  const double x218 = x203 * x98;
  const double x219 = x190 * x99;
  const double x220 = d_0 * x152 - d_1 * x124;
  const double x221 = x21 * x45;
  const double x222 = x21 * x50;
  const double x223 = a_2 * x54;
  const double x224 = a_0 * x223;
  const double x225 = a_1 * x32;
  const double x226 = a_0 * x206;
  const double x227 = a_1 * x35;
  const double x228 = c_0 * x223 + x16 * x7;
  const double x229 = -x14 + x152;
  const double x230 = x229 * x95;
  const double x231 = x174 - x21;
  const double x232 = x231 * x98;
  const double x233 = x212 - x32;
  const double x234 = -x90;
  const double x235 = -x124 + x54;
  const double x236 = x235 * x95;
  const double x237 = x231 * x99;
  const double x238 = x22 - x223;
  const double x239 = -x87;
  const double x240 = x235 * x98;
  const double x241 = -x229 * x99;
  const double x242 = x15 - x210;
  const double x243 = x138 / x12;

  out_6543693310212660619[0]
      = x139
            * (x123 * (-x119 + x120 + x121 + x122)
               + x137
                     * (4 * a_0 * b_1 * c_1 + 4 * a_0 * b_2 * c_2 - x125 - x126
                        - x127 - x128 + x129 + x130 + x131 + x132 - x133 - x134
                        - x135 - x136 - x42 - x43 + x55 + x56 + x58 + x59))
        + x76
              * (-x101 * (x80 + x81 * x83 + x85 * x86 - x89)
                 + (1.0 / 2.0) * x106 * x108 * (x102 * x86 + x103 * x104 + x105)
                 - x116 * (-1.0 / 2.0 * x111 - 1.0 / 2.0 * x114)
                 + (1.0 / 2.0) * x117 * x118 * (-x111 - x114));
  out_6543693310212660619[1]
      = x139
            * (x123 * (x148 - x149 - x150 - x151)
               + x137
                     * (4 * a_1 * b_0 * c_0 + 4 * a_1 * b_2 * c_2 - x153 - x154
                        - x155 - x156 + x157 + x158 - x159 - x160 - x161 - x162
                        - x163 + x20 + x36 - x44 + x62 + x63 + x64 + x65))
        + x76
              * (-x101 * (x140 * x83 + x141 * x85 + x98)
                 + (1.0 / 2.0) * x106 * x108
                       * (x102 * x141 + x104 * x107 - x142 + x143)
                 - x116 * (-1.0 / 2.0 * x146 - 1.0 / 2.0 * x147)
                 + (1.0 / 2.0) * x117 * x118 * (-x146 - x147));
  out_6543693310212660619[2]
      = x139
            * (x123 * (-x170 + x171 + x172 + x173)
               + x137
                     * (4 * a_2 * b_0 * c_0 + 4 * a_2 * b_1 * c_1 - x175 - x176
                        - x177 - x178 + x179 + x180 + x181 - x182 - x183 - x184
                        - x185 - x186 - x187 + x27 + x68 + x69 + x70 + x71))
        + x76
              * (-x101 * (x140 * x164 + x141 * x81 - x91 + x94)
                 + (1.0 / 2.0) * x106 * x108
                       * (x141 * x166 + x164 * x165 + x167)
                 - x116 * (-1.0 / 2.0 * x168 - 1.0 / 2.0 * x169)
                 + (1.0 / 2.0) * x117 * x118 * (-x168 - x169));
  out_6543693310212660619[3]
      = x139
            * (x123 * (-x121 - x194 - x195 + x35)
               + x137
                     * (4 * a_1 * b_0 * c_1 + 4 * a_2 * b_0 * c_2 + x127 + x128
                        - x130 - x132 + x133 + x134 - x135 + x196 + x197 + x198
                        + x199 - x200 - x201 - x202 - x55 - x56 - x66 - x72))
        + x76
              * (-x101 * (x188 * x85 + x189 * x81)
                 + (1.0 / 2.0) * x106 * x108 * (x102 * x188 + x166 * x189)
                 - x116 * (-1.0 / 2.0 * x191 - 1.0 / 2.0 * x193)
                 + (1.0 / 2.0) * x117 * x118 * (-x191 - x193));
  out_6543693310212660619[4]
      = x139
            * (x123 * (x150 - x206 + x207 + x24)
               + x137
                     * (4 * a_0 * b_1 * c_0 + 4 * a_2 * b_1 * c_2 + x155 + x156
                        + x160 - x162 - x20 - x208 - x209 + x211 + x213 - x215
                        - x216 - x217 - x36 + x44 + x60 - x63 - x64 + x73))
        + x76
              * (-x101 * (x140 * x189 + x85 * x93)
                 + (1.0 / 2.0) * x106 * x108 * (x102 * x93 + x165 * x189)
                 - x116 * (-1.0 / 2.0 * x204 - 1.0 / 2.0 * x205)
                 + (1.0 / 2.0) * x117 * x118 * (-x204 - x205));
  out_6543693310212660619[5]
      = x139
            * (x123 * (-x17 - x172 + x214 - x220)
               + x137
                     * (4 * a_0 * b_2 * c_0 + 4 * a_1 * b_2 * c_1 + x177 + x178
                        - x181 + x183 + x185 - x186 - x221 - x222 + x224 + x225
                        - x226 - x227 - x228 - x27 + x61 + x67 - x70 - x71))
        + x76
              * (-x101 * (x140 * x78 + x81 * x93)
                 + (1.0 / 2.0) * x106 * x108 * (x165 * x78 + x166 * x93)
                 - x116 * (-1.0 / 2.0 * x218 - 1.0 / 2.0 * x219)
                 + (1.0 / 2.0) * x117 * x118 * (-x218 - x219));
  out_6543693310212660619[6]
      = x139
            * (x123 * (x119 - x120 + x195 + x233)
               + x137
                     * (4 * a_1 * b_1 * c_0 + 4 * a_2 * b_2 * c_0 + x125 + x126
                        - x129 - x131 - x136 - x196 - x197 - x198 - x199 + x200
                        + x201 - x202 + x42 + x43 - x58 - x59 + x66 + x72))
        + x76
              * (-x101 * (x79 * x81 + x85 * x87)
                 + (1.0 / 2.0) * x106 * x108 * (x102 * x87 + x166 * x79)
                 - x116 * (-1.0 / 2.0 * x230 - 1.0 / 2.0 * x232)
                 + (1.0 / 2.0) * x117 * x118 * (-x230 - x232));
  out_6543693310212660619[7]
      = x139
            * (x123 * (-x148 + x149 - x207 - x238)
               + x137
                     * (4 * a_0 * b_0 * c_1 + 4 * a_2 * b_2 * c_1 + x153 + x154
                        - x157 - x158 + x159 + x161 - x163 + x208 + x209 - x211
                        - x213 + x215 + x216 - x217 - x60 - x62 - x65 - x73))
        + x76
              * (-x101 * (x140 * x79 + x234 * x85)
                 + (1.0 / 2.0) * x106 * x108 * (x102 * x234 + x165 * x79)
                 - x116 * (-1.0 / 2.0 * x236 - 1.0 / 2.0 * x237)
                 + (1.0 / 2.0) * x117 * x118 * (-x236 - x237));
  out_6543693310212660619[8]
      = x139
            * (x123 * (x170 - x171 + x220 + x242)
               + x137
                     * (4 * a_0 * b_0 * c_2 + 4 * a_1 * b_1 * c_2 + x175 + x176
                        - x179 - x180 + x182 + x184 - x187 + x221 + x222 - x224
                        - x225 + x226 + x227 - x228 - x61 - x67 - x68 - x69))
        + x76
              * (-x101 * (x140 * x239 + x234 * x81)
                 + (1.0 / 2.0) * x106 * x108 * (x165 * x239 + x166 * x234)
                 - x116 * (-1.0 / 2.0 * x240 - 1.0 / 2.0 * x241)
                 + (1.0 / 2.0) * x117 * x118 * (-x240 - x241));
  out_6543693310212660619[9]
      = x243 * (-x122 + x194 - x233 - x35)
        + x76 * (-x101 * x99 - 1.0 / 2.0 * x105 * x106 * x108);
  out_6543693310212660619[10]
      = x243 * (x151 + x206 + x238 - x24)
        + x76 * (x101 * x98 + (1.0 / 2.0) * x106 * x108 * (x142 - x143));
  out_6543693310212660619[11]
      = x243 * (x17 - x173 - x214 - x242)
        + x76 * (-x101 * x95 - 1.0 / 2.0 * x106 * x108 * x167);
}

void vertex_face_hessian(double a_0, double a_1, double a_2, double b_0,
                         double b_1, double b_2, double c_0, double c_1,
                         double c_2, double d_0, double d_1, double d_2,
                         double d_h, double *out_3216172810630303951) {
  const double x0 = ((c_1) * (c_1));
  const double x1 = 2 * x0;
  const double x2 = b_0 * x1;
  const double x3 = ((c_2) * (c_2));
  const double x4 = 2 * x3;
  const double x5 = b_0 * x4;
  const double x6 = ((b_1) * (b_1));
  const double x7 = 2 * x6;
  const double x8 = c_0 * x7;
  const double x9 = ((b_2) * (b_2));
  const double x10 = 2 * x9;
  const double x11 = c_0 * x10;
  const double x12 = a_0 * x7;
  const double x13 = a_0 * x10;
  const double x14 = a_0 * x1;
  const double x15 = a_0 * x4;
  const double x16 = a_0 * c_1;
  const double x17 = 4 * b_1;
  const double x18 = x16 * x17;
  const double x19 = a_0 * c_2;
  const double x20 = 4 * b_2;
  const double x21 = x19 * x20;
  const double x22 = 2 * b_1;
  const double x23 = a_1 * b_0;
  const double x24 = x22 * x23;
  const double x25 = 2 * c_1;
  const double x26 = a_1 * c_0;
  const double x27 = x25 * x26;
  const double x28 = 2 * b_2;
  const double x29 = a_2 * b_0;
  const double x30 = x28 * x29;
  const double x31 = 2 * c_2;
  const double x32 = a_2 * c_0;
  const double x33 = x31 * x32;
  const double x34 = b_0 * c_1;
  const double x35 = x22 * x34;
  const double x36 = b_0 * c_2;
  const double x37 = x28 * x36;
  const double x38 = c_0 * x22;
  const double x39 = c_1 * x38;
  const double x40 = c_0 * x28;
  const double x41 = c_2 * x40;
  const double x42 = x22 * x26 + x28 * x32;
  const double x43 = x23 * x25 + x29 * x31;
  const double x44 = -x11 + x12 + x13 + x14 + x15 - x18 - x2 - x21 - x24 - x27
                     - x30 - x33 + x35 + x37 + x39 + x41 + x42 + x43 - x5 - x8;
  const double x45 = b_1 * c_2;
  const double x46 = b_2 * d_1;
  const double x47 = c_1 * d_2;
  const double x48 = b_0 * d_2;
  const double x49 = b_2 * c_0;
  const double x50 = c_2 * d_0;
  const double x51 = b_1 * d_0;
  const double x52 = c_0 * d_1;
  const double x53 = c_2 * d_1;
  const double x54 = b_1 * d_2;
  const double x55 = b_2 * c_1;
  const double x56 = b_2 * d_0;
  const double x57 = c_0 * d_2;
  const double x58 = b_0 * d_1;
  const double x59 = b_1 * c_0;
  const double x60 = c_1 * d_0;
  const double x61 = a_0 * x45 + a_0 * x46 + a_0 * x47 - a_0 * x53 - a_0 * x54
                     - a_0 * x55 - a_1 * x36 + a_1 * x48 + a_1 * x49 + a_1 * x50
                     - a_1 * x56 - a_1 * x57 + a_2 * x34 + a_2 * x51 + a_2 * x52
                     - a_2 * x58 - a_2 * x59 - a_2 * x60 - b_0 * x47 + b_0 * x53
                     - c_0 * x46 + c_0 * x54 - d_0 * x45 + d_0 * x55;
  const double x62 = a_0 * x22;
  const double x63 = a_0 * x25;
  const double x64 = a_0 * x28;
  const double x65 = a_0 * x31;
  const double x66 = a_1 * b_2;
  const double x67 = a_2 * x22;
  const double x68 = a_1 * c_2;
  const double x69 = a_2 * x25;
  const double x70 = b_0 * x22;
  const double x71 = b_0 * x25;
  const double x72 = b_2 * x22;
  const double x73 = b_2 * x25;
  const double x74 = b_0 * x28;
  const double x75 = b_0 * x31;
  const double x76 = a_2 * c_1;
  const double x77 = c_2 * x22;
  const double x78 = ((a_0) * (a_0));
  const double x79 = ((a_1) * (a_1));
  const double x80 = ((b_0) * (b_0));
  const double x81 = ((c_0) * (c_0));
  const double x82 = ((a_2) * (a_2));
  const double x83 = c_1 * x22;
  const double x84 = c_2 * x28;
  const double x85 = x25 * x80;
  const double x86 = x22 * x81;
  const double x87 = x22 * x3;
  const double x88 = x25 * x9;
  const double x89 = 2 * b_0;
  const double x90 = x79 * x89;
  const double x91 = x31 * x80;
  const double x92 = x31 * x6;
  const double x93 = x28 * x81;
  const double x94 = x0 * x28;
  const double x95 = x82 * x89;
  const double x96
      = -a_1 * x85 - a_1 * x86 - a_1 * x87 - a_1 * x88 - a_2 * x91 - a_2 * x92
        - a_2 * x93 - a_2 * x94 - b_0 * x14 - b_0 * x15 - c_0 * x12 - c_0 * x13
        - c_0 * x90 - c_0 * x95 + x0 * x78 + x0 * x80 + x0 * x82 + x0 * x9
        + x16 * x38 + x19 * x40 - x23 * x62 + x23 * x63 + x26 * x62 - x26 * x63
        + x26 * x70 + x26 * x71 - x29 * x64 + x29 * x65 + x3 * x6 + x3 * x78
        + x3 * x79 + x3 * x80 + x32 * x64 - x32 * x65 + x32 * x74 + x32 * x75
        - x34 * x38 + x34 * x62 - x36 * x40 + x36 * x64 + x55 * x67 - x55 * x77
        + x6 * x78 + x6 * x81 + x6 * x82 - x66 * x67 + x66 * x69 + x67 * x68
        - x68 * x69 + x68 * x72 + x68 * x73 + x76 * x77 - x78 * x83 - x78 * x84
        + x78 * x9 + x79 * x80 + x79 * x81 - x79 * x84 + x79 * x9 + x80 * x82
        + x81 * x82 + x81 * x9 - x82 * x83;
  const double x97 = 1.0 / x96;
  const double x98 = d_2 * x22;
  const double x99 = d_1 * x28;
  const double x100 = -d_1 * x31 + d_2 * x25;
  const double x101 = -x73;
  const double x102 = x101 + x77;
  const double x103 = x100 + x102 - x98 + x99;
  const double x104 = x103 * x97;
  const double x105 = -x44;
  const double x106 = ((x61) * (x61));
  const double x107 = pow(x96, -2);
  const double x108 = x106 * x107;
  const double x109 = x104 * x61 + x105 * x108;
  const double x110 = 1.0 / x106;
  const double x111 = -d_0;
  const double x112 = a_0 + x111;
  const double x113 = a_1 - b_1;
  const double x114 = -c_2;
  const double x115 = a_2 + x114;
  const double x116 = x113 * x115;
  const double x117 = -c_1;
  const double x118 = a_1 + x117;
  const double x119 = a_2 - b_2;
  const double x120 = x118 * x119;
  const double x121 = x116 - x120;
  const double x122 = -d_2;
  const double x123 = a_2 + x122;
  const double x124 = a_0 - b_0;
  const double x125 = x118 * x124;
  const double x126 = -c_0;
  const double x127 = a_0 + x126;
  const double x128 = x113 * x127;
  const double x129 = x125 - x128;
  const double x130 = -d_1;
  const double x131 = a_1 + x130;
  const double x132 = x115 * x124;
  const double x133 = x119 * x127;
  const double x134 = x132 - x133;
  const double x135 = x112 * x121 + x123 * x129 - x131 * x134;
  const double x136 = ((x135) * (x135));
  const double x137 = ((x121) * (x121)) + ((x129) * (x129)) + ((x134) * (x134));
  const double x138 = 1.0 / x137;
  const double x139 = (1.0 / 2.0) * x138;
  const double x140 = -x135;
  const double x141 = d_h / sqrt(x137);
  const double x142 = (1.0 / 2.0) * ((d_h) * (d_h)) + x136 * x139 - x140 * x141;
  const double x143 = x110 * x142;
  const double x144 = x109 * x143;
  const double x145 = x142 * x96 / pow(x61, 3);
  const double x146 = x109 * x145;
  const double x147 = -2 * log(d_h) + log(x106 * x97);
  const double x148 = b_1 + x117;
  const double x149 = x123 * x148;
  const double x150 = -x131;
  const double x151 = b_2 + x114;
  const double x152 = x150 * x151;
  const double x153 = 2 * x116;
  const double x154 = 2 * x120;
  const double x155 = x153 - x154;
  const double x156 = 2 * x149 + 2 * x152 + x155;
  const double x157 = x139 * x156;
  const double x158 = -x25;
  const double x159 = x158 + x22;
  const double x160 = x129 * x159;
  const double x161 = -x31;
  const double x162 = x161 + x28;
  const double x163 = x134 * x162;
  const double x164 = -1.0 / 2.0 * x160 - 1.0 / 2.0 * x163;
  const double x165 = -x123;
  const double x166 = -x116 + x120;
  const double x167 = x131 * x151 + x148 * x165 + x166;
  const double x168 = d_h / pow(x137, 3.0 / 2.0);
  const double x169 = x167 * x168;
  const double x170 = -x148;
  const double x171 = x148 * x170;
  const double x172 = -x151;
  const double x173 = x151 * x172;
  const double x174 = x140 * x168;
  const double x175 = -x159;
  const double x176 = -x162;
  const double x177 = x151 * x176;
  const double x178 = pow(x137, -2);
  const double x179 = (1.0 / 2.0) * x136;
  const double x180 = x178 * x179;
  const double x181 = -x160 - x163;
  const double x182 = x135 * x178;
  const double x183 = x156 * x182;
  const double x184 = x179 / pow(x137, 3);
  const double x185 = x181 * x184;
  const double x186 = d_h * x140 / pow(x137, 5.0 / 2.0);
  const double x187 = x164 * x186;
  const double x188 = (1.0 / 2.0) * x135 * x138 * x156
                      + (1.0 / 2.0) * x136 * x178 * x181 - x141 * x167
                      - x164 * x174;
  const double x189 = x110 * x96;
  const double x190 = x109 * x189;
  const double x191 = x47 - x53;
  const double x192 = x45 - x55;
  const double x193 = -4 * b_1 * c_1;
  const double x194 = -4 * b_2 * c_2;
  const double x195 = x1 + x4;
  const double x196 = x10 + x7;
  const double x197 = x107 * x61;
  const double x198 = x105 * x197;
  const double x199 = 4 * b_0;
  const double x200 = x0 * x199;
  const double x201 = x199 * x3;
  const double x202 = 4 * c_0;
  const double x203 = x202 * x6;
  const double x204 = x202 * x9;
  const double x205 = 4 * a_0;
  const double x206 = x205 * x6;
  const double x207 = x205 * x9;
  const double x208 = x0 * x205;
  const double x209 = x205 * x3;
  const double x210 = x17 * x23;
  const double x211 = 4 * c_1;
  const double x212 = x211 * x26;
  const double x213 = x20 * x29;
  const double x214 = 4 * c_2;
  const double x215 = x214 * x32;
  const double x216 = x17 * x34;
  const double x217 = x20 * x36;
  const double x218 = x211 * x59;
  const double x219 = x214 * x49;
  const double x220 = 4 * a_1;
  const double x221 = x220 * x59;
  const double x222 = 4 * a_2;
  const double x223 = x222 * x49;
  const double x224 = x221 + x223;
  const double x225 = x220 * x34;
  const double x226 = x222 * x36;
  const double x227 = x225 + x226;
  const double x228 = x106 / pow(x96, 3);
  const double x229 = x105 * x228;
  const double x230 = x143 * x96;
  const double x231 = 2 * a_1;
  const double x232 = x231 * x80;
  const double x233 = a_1 * x10;
  const double x234 = x231 * x81;
  const double x235 = a_1 * x4;
  const double x236 = x199 * x26;
  const double x237 = x20 * x68;
  const double x238 = a_0 * x70;
  const double x239 = c_0 * x25;
  const double x240 = a_0 * x239;
  const double x241 = b_2 * x67;
  const double x242 = c_2 * x69;
  const double x243 = b_0 * x38;
  const double x244 = c_0 * x71;
  const double x245 = b_2 * x77;
  const double x246 = c_2 * x73;
  const double x247 = a_0 * x71 + a_2 * x73;
  const double x248 = a_0 * x38 + a_2 * x77;
  const double x249 = x232 + x233 + x234 + x235 - x236 - x237 - x238 - x240
                      - x241 - x242 + x243 + x244 + x245 + x246 + x247 + x248
                      - x85 - x86 - x87 - x88;
  const double x250 = d_2 * x89;
  const double x251 = d_0 * x28;
  const double x252 = 2 * c_0;
  const double x253 = -d_0 * x31 + d_2 * x252;
  const double x254 = -x40;
  const double x255 = x254 + x75;
  const double x256 = -x250 + x251 + x253 + x255;
  const double x257 = -x256;
  const double x258 = x61 * x97;
  const double x259 = -x249;
  const double x260 = x108 * x259 + x257 * x258;
  const double x261 = x188 * x189;
  const double x262 = -x112;
  const double x263 = b_0 + x126;
  const double x264 = -x263;
  const double x265 = x134 + x151 * x262 + x165 * x264;
  const double x266 = x112 * x151;
  const double x267 = x123 * x264;
  const double x268 = 2 * x132;
  const double x269 = 2 * x133;
  const double x270 = -x268 + x269;
  const double x271 = 2 * x266 + 2 * x267 + x270;
  const double x272 = -x252;
  const double x273 = x272 + x89;
  const double x274 = -x273;
  const double x275 = x129 * x274;
  const double x276 = x121 * x162;
  const double x277 = -1.0 / 2.0 * x275 - 1.0 / 2.0 * x276;
  const double x278 = -x275 - x276;
  const double x279 = (1.0 / 2.0) * x135 * x138 * x271
                      + (1.0 / 2.0) * x136 * x178 * x278 - x141 * x265
                      - x174 * x277;
  const double x280 = -x132 + x133;
  const double x281 = x266 + x267 + x280;
  const double x282 = x174 * x264;
  const double x283 = x180 * x264;
  const double x284 = x164 * x168;
  const double x285 = (1.0 / 2.0) * x183;
  const double x286 = x182 * x271;
  const double x287 = (1.0 / 2.0) * x181;
  const double x288 = -2 * x275 - 2 * x276;
  const double x289 = -3.0 / 2.0 * x275 - 3.0 / 2.0 * x276;
  const double x290 = -x50 + x57;
  const double x291 = x36 - x49;
  const double x292 = -x290 - x291 + x48 - x56;
  const double x293 = -x71;
  const double x294 = -x38;
  const double x295 = x103 * x197;
  const double x296 = x17 * x81;
  const double x297 = x17 * x3;
  const double x298 = x211 * x80;
  const double x299 = x211 * x9;
  const double x300 = x220 * x80;
  const double x301 = x220 * x9;
  const double x302 = x220 * x81;
  const double x303 = x220 * x3;
  const double x304 = a_0 * b_1;
  const double x305 = x199 * x304;
  const double x306 = x16 * x202;
  const double x307 = a_2 * b_1;
  const double x308 = x20 * x307;
  const double x309 = x214 * x76;
  const double x310 = x199 * x59;
  const double x311 = x202 * x34;
  const double x312 = x20 * x45;
  const double x313 = x214 * x55;
  const double x314 = x205 * x34;
  const double x315 = x222 * x55;
  const double x316 = x314 + x315;
  const double x317 = x205 * x59;
  const double x318 = x222 * x45;
  const double x319 = x317 + x318;
  const double x320 = 8 * a_1 * b_0 * c_0 + 8 * a_1 * b_2 * c_2 + x296 + x297
                      + x298 + x299 - x300 - x301 - x302 - x303 + x305 + x306
                      + x308 + x309 - x310 - x311 - x312 - x313 - x316 - x319;
  const double x321 = x144 * x249 + x146 * x256
                      + x147
                            * (x157 * x281 - x169 * x277 - x170 * x282
                               + x175 * x283 + x185 * x288 - x187 * x289
                               - x265 * x284 + x278 * x285 + x286 * x287)
                      + x190 * x279
                      + x230
                            * (x104 * x292 + x108 * (x239 + x293 + x294 + x70)
                               + x198 * x257 + x229 * x320 + x259 * x295)
                      + x260 * x261;
  const double x322 = 2 * a_2;
  const double x323 = x322 * x80;
  const double x324 = a_2 * x7;
  const double x325 = x322 * x81;
  const double x326 = a_2 * x1;
  const double x327 = x199 * x32;
  const double x328 = x17 * x76;
  const double x329 = a_0 * x74;
  const double x330 = c_0 * x31;
  const double x331 = a_0 * x330;
  const double x332 = x22 * x66;
  const double x333 = x25 * x68;
  const double x334 = b_0 * x40;
  const double x335 = c_0 * x75;
  const double x336 = x22 * x55;
  const double x337 = c_1 * x77;
  const double x338 = a_0 * x75 + x22 * x68;
  const double x339 = a_0 * x40 + x25 * x66;
  const double x340 = x323 + x324 + x325 + x326 - x327 - x328 - x329 - x331
                      - x332 - x333 + x334 + x335 + x336 + x337 + x338 + x339
                      - x91 - x92 - x93 - x94;
  const double x341 = d_1 * x89;
  const double x342 = d_0 * x22;
  const double x343 = -d_0 * x25 + d_1 * x252;
  const double x344 = x294 + x71;
  const double x345 = -x341 + x342 + x343 + x344;
  const double x346 = -x345;
  const double x347 = -x340;
  const double x348 = x108 * x347 + x258 * x345;
  const double x349 = -x125 + x128;
  const double x350 = x131 * x264 + x170 * x262 + x349;
  const double x351 = x112 * x170;
  const double x352 = x150 * x264;
  const double x353 = 2 * x125;
  const double x354 = 2 * x128;
  const double x355 = x353 - x354;
  const double x356 = 2 * x351 + 2 * x352 + x355;
  const double x357 = x134 * x274;
  const double x358 = x121 * x175;
  const double x359 = -1.0 / 2.0 * x357 - 1.0 / 2.0 * x358;
  const double x360 = -x357 - x358;
  const double x361 = (1.0 / 2.0) * x135 * x138 * x356
                      + (1.0 / 2.0) * x136 * x178 * x360 - x141 * x350
                      - x174 * x359;
  const double x362 = x129 + x351 + x352;
  const double x363 = x182 * x356;
  const double x364 = -2 * x357 - 2 * x358;
  const double x365 = -3.0 / 2.0 * x357 - 3.0 / 2.0 * x358;
  const double x366 = x52 - x60;
  const double x367 = x34 - x59;
  const double x368 = x366 + x367 + x51 - x58;
  const double x369 = -x75;
  const double x370 = x20 * x81;
  const double x371 = x0 * x20;
  const double x372 = x214 * x80;
  const double x373 = x214 * x6;
  const double x374 = x222 * x80;
  const double x375 = x222 * x6;
  const double x376 = x222 * x81;
  const double x377 = x0 * x222;
  const double x378 = a_0 * b_2;
  const double x379 = x199 * x378;
  const double x380 = x19 * x202;
  const double x381 = x17 * x66;
  const double x382 = x211 * x68;
  const double x383 = x199 * x49;
  const double x384 = x202 * x36;
  const double x385 = x17 * x55;
  const double x386 = x211 * x45;
  const double x387 = x205 * x36;
  const double x388 = x220 * x45;
  const double x389 = x387 + x388;
  const double x390 = x205 * x49;
  const double x391 = x220 * x55;
  const double x392 = x390 + x391;
  const double x393 = 8 * a_2 * b_0 * c_0 + 8 * a_2 * b_1 * c_1 + x370 + x371
                      + x372 + x373 - x374 - x375 - x376 - x377 + x379 + x380
                      + x381 + x382 - x383 - x384 - x385 - x386 - x389 - x392;
  const double x394 = x144 * x340 + x146 * x346
                      + x147
                            * (x157 * x362 - x169 * x359 - x172 * x282
                               + x176 * x283 + x185 * x364 - x187 * x365
                               - x284 * x350 + x285 * x360 + x287 * x363)
                      + x190 * x361
                      + x230
                            * (x104 * x368 + x108 * (x254 + x330 + x369 + x74)
                               + x198 * x345 + x229 * x393 + x295 * x347)
                      + x261 * x348;
  const double x395 = x252 * x79;
  const double x396 = x252 * x82;
  const double x397 = a_1 * x22;
  const double x398 = a_0 * x397;
  const double x399 = a_2 * x28;
  const double x400 = a_0 * x399;
  const double x401 = a_1 * x25;
  const double x402 = a_0 * x401;
  const double x403 = a_2 * x31;
  const double x404 = a_0 * x403;
  const double x405 = x16 * x22 + x19 * x28;
  const double x406 = -x14 - x15 + x2 - x225 - x226 + x27 + x33 - x39 - x395
                      - x396 - x398 - x400 + x402 + x404 + x405 - x41 + x42 + x5
                      + x90 + x95;
  const double x407 = -x69;
  const double x408 = a_1 * x31;
  const double x409 = d_1 * x322 - d_2 * x231;
  const double x410 = x100 + x407 + x408 + x409;
  const double x411 = -x118;
  const double x412 = -x115;
  const double x413 = x131 * x412 + x165 * x411;
  const double x414 = x123 * x411;
  const double x415 = x150 * x412;
  const double x416 = 2 * x414 + 2 * x415;
  const double x417 = x158 + x231;
  const double x418 = -x417;
  const double x419 = x129 * x418;
  const double x420 = x161 + x322;
  const double x421 = -x420;
  const double x422 = x134 * x421;
  const double x423 = -1.0 / 2.0 * x419 - 1.0 / 2.0 * x422;
  const double x424 = -x419 - x422;
  const double x425 = (1.0 / 2.0) * x135 * x138 * x416
                      + (1.0 / 2.0) * x136 * x178 * x424 - x141 * x413
                      - x174 * x423;
  const double x426 = -x410;
  const double x427 = -x406;
  const double x428 = x108 * x427 + x258 * x426;
  const double x429 = x414 + x415;
  const double x430 = x172 * x412;
  const double x431 = x176 * x412;
  const double x432 = x182 * x416;
  const double x433 = -2 * x419 - 2 * x422;
  const double x434 = -3.0 / 2.0 * x419 - 3.0 / 2.0 * x422;
  const double x435 = -a_1 * d_2 + a_2 * d_1;
  const double x436 = -x191 - x435 - x68 + x76;
  const double x437 = -x403;
  const double x438 = x399 + x437;
  const double x439 = x438 - x84;
  const double x440 = -x401;
  const double x441 = x397 + x440;
  const double x442 = x441 - x83;
  const double x443 = x202 * x79;
  const double x444 = x202 * x82;
  const double x445 = x199 * x79;
  const double x446 = x199 * x82;
  const double x447 = x220 * x304;
  const double x448 = x222 * x378;
  const double x449 = x16 * x220;
  const double x450 = x19 * x222;
  const double x451 = x18 + x21;
  const double x452 = 8 * a_1 * b_0 * c_1 + 8 * a_2 * b_0 * c_2 - x200 - x201
                      + x208 + x209 - x212 - x215 + x218 + x219 - x224 + x443
                      + x444 - x445 - x446 + x447 + x448 - x449 - x450 - x451;
  const double x453
      = x144 * x406 + x146 * x410
        + x147
              * (x157 * x429 - x169 * x423 - x174 * (x170 * x411 + x430)
                 + x180 * (x175 * x411 + x431) + x185 * x433 - x187 * x434
                 - x284 * x413 + x285 * x424 + x287 * x432)
        + x190 * x425
        + x230
              * (x104 * x436 + x108 * (x195 + x439 + x442) + x198 * x426
                 + x229 * x452 + x295 * x427)
        + x261 * x428;
  const double x454 = x25 * x78;
  const double x455 = x25 * x82;
  const double x456 = x22 * x78;
  const double x457 = x22 * x82;
  const double x458 = a_1 * x89;
  const double x459 = a_0 * x458;
  const double x460 = a_1 * x399;
  const double x461 = a_1 * x252;
  const double x462 = a_0 * x461;
  const double x463 = a_2 * x408;
  const double x464 = x26 * x89 + x28 * x68;
  const double x465 = -x234 - x235 + x240 + x242 - x244 - x246 + x247 - x317
                      - x318 - x454 - x455 + x456 + x457 - x459 - x460 + x462
                      + x463 + x464 + x86 + x87;
  const double x466 = a_2 * x252;
  const double x467 = -x466;
  const double x468 = 2 * d_2;
  const double x469 = -a_0 * x468 + d_0 * x322;
  const double x470 = x253 + x467 + x469 + x65;
  const double x471 = -x470;
  const double x472 = x127 * x165 + x262 * x412;
  const double x473 = x123 * x127;
  const double x474 = x112 * x412;
  const double x475 = 2 * x473 + 2 * x474;
  const double x476 = 2 * a_0;
  const double x477 = x272 + x476;
  const double x478 = x129 * x477;
  const double x479 = x121 * x421;
  const double x480 = -1.0 / 2.0 * x478 - 1.0 / 2.0 * x479;
  const double x481 = -x478 - x479;
  const double x482 = (1.0 / 2.0) * x135 * x138 * x475
                      + (1.0 / 2.0) * x136 * x178 * x481 - x141 * x472
                      - x174 * x480;
  const double x483 = -x465;
  const double x484 = x108 * x483 + x258 * x470;
  const double x485 = c_2 + x122;
  const double x486 = x473 + x474;
  const double x487 = -x468;
  const double x488 = x31 + x487;
  const double x489 = x135 * x139;
  const double x490 = -x353 + x354;
  const double x491 = x182 * x475;
  const double x492 = -2 * x478 - 2 * x479;
  const double x493 = -3.0 / 2.0 * x478 - 3.0 / 2.0 * x479;
  const double x494 = -a_0 * d_2 + a_2 * d_0;
  const double x495 = x19 + x290 - x32 + x494;
  const double x496 = -x458;
  const double x497 = 4 * x304;
  const double x498 = 4 * x59;
  const double x499 = -x498 + x71;
  const double x500 = 4 * x16;
  const double x501 = x461 - x500;
  const double x502 = x211 * x78;
  const double x503 = x211 * x82;
  const double x504 = x17 * x78;
  const double x505 = x17 * x82;
  const double x506 = x205 * x23;
  const double x507 = x222 * x66;
  const double x508 = x205 * x26;
  const double x509 = x222 * x68;
  const double x510 = x236 + x237;
  const double x511 = 8 * a_0 * b_1 * c_0 + 8 * a_2 * b_1 * c_2 - x296 - x297
                      + x302 + x303 - x306 - x309 + x311 + x313 - x316 + x502
                      + x503 - x504 - x505 + x506 + x507 - x508 - x509 - x510;
  const double x512
      = x144 * x465 + x146 * x471
        + x147
              * (x141 * x485 + x157 * x486 - x169 * x480
                 - x174 * (x127 * x170 + x349) + x180 * (x127 * x175 + x490)
                 + x185 * x492 - x187 * x493 - x284 * x472 + x285 * x481
                 + x287 * x491 + x488 * x489)
        + x190 * x482
        + x230
              * (x104 * x495 + x108 * (-x239 - x496 - x497 - x499 - x501)
                 + x198 * x470 + x229 * x511 + x258 * x488 + x295 * x483)
        + x261 * x484;
  const double x513 = x31 * x78;
  const double x514 = x31 * x79;
  const double x515 = x28 * x78;
  const double x516 = x28 * x79;
  const double x517 = a_2 * x89;
  const double x518 = a_0 * x517;
  const double x519 = a_2 * x397;
  const double x520 = a_0 * x466;
  const double x521 = a_1 * x69;
  const double x522 = x22 * x76 + x32 * x89;
  const double x523 = -x325 - x326 + x331 + x333 - x335 - x337 + x338 - x390
                      - x391 - x513 - x514 + x515 + x516 - x518 - x519 + x520
                      + x521 + x522 + x93 + x94;
  const double x524 = -x461;
  const double x525 = d_0 * x231 - d_1 * x476;
  const double x526 = x343 + x524 + x525 + x63;
  const double x527 = x118 * x262 + x127 * x131;
  const double x528 = x127 * x150;
  const double x529 = x112 * x118;
  const double x530 = 2 * x528 + 2 * x529;
  const double x531 = x134 * x477;
  const double x532 = x121 * x417;
  const double x533 = -1.0 / 2.0 * x531 - 1.0 / 2.0 * x532;
  const double x534 = -x531 - x532;
  const double x535 = (1.0 / 2.0) * x135 * x138 * x530
                      + (1.0 / 2.0) * x136 * x178 * x534 - x141 * x527
                      - x174 * x533;
  const double x536 = -x526;
  const double x537 = -x523;
  const double x538 = x108 * x537 + x258 * x536;
  const double x539 = c_1 + x130;
  const double x540 = x528 + x529;
  const double x541 = -2 * d_1;
  const double x542 = x25 + x541;
  const double x543 = -x542;
  const double x544 = x182 * x530;
  const double x545 = -2 * x531 - 2 * x532;
  const double x546 = -3.0 / 2.0 * x531 - 3.0 / 2.0 * x532;
  const double x547 = -a_0 * d_1 + a_1 * d_0;
  const double x548 = -x16 + x26 - x366 - x547;
  const double x549 = -x517;
  const double x550 = 4 * x378;
  const double x551 = 4 * x49;
  const double x552 = -x551 + x75;
  const double x553 = 4 * x19;
  const double x554 = x466 - x553;
  const double x555 = x214 * x78;
  const double x556 = x214 * x79;
  const double x557 = x20 * x78;
  const double x558 = x20 * x79;
  const double x559 = x205 * x29;
  const double x560 = x220 * x307;
  const double x561 = x205 * x32;
  const double x562 = x220 * x76;
  const double x563 = x327 + x328;
  const double x564 = 8 * a_0 * b_2 * c_0 + 8 * a_1 * b_2 * c_1 - x370 - x371
                      + x376 + x377 - x380 - x382 + x384 + x386 - x389 + x555
                      + x556 - x557 - x558 + x559 + x560 - x561 - x562 - x563;
  const double x565
      = x144 * x523 + x146 * x526
        + x147
              * (-x141 * x539 + x157 * x540 - x169 * x533
                 - x174 * (x127 * x172 + x280) + x180 * (x127 * x176 + x270)
                 + x185 * x545 - x187 * x546 - x284 * x527 + x285 * x534
                 + x287 * x544 + x489 * x543)
        + x190 * x535
        + x230
              * (x104 * x548 + x108 * (-x330 - x549 - x550 - x552 - x554)
                 + x198 * x536 + x229 * x564 + x258 * x543 + x295 * x537)
        + x261 * x538;
  const double x566 = x11 - x12 - x13 - x221 - x223 + x24 + x30 - x35 - x37
                      + x395 + x396 + x398 + x400 - x402 - x404 + x405 + x43
                      + x8 - x90 - x95;
  const double x567 = -x67;
  const double x568 = a_1 * x28;
  const double x569 = x567 + x568;
  const double x570 = x409 + x569 + x98 - x99;
  const double x571 = -x570;
  const double x572 = x113 * x165 + x119 * x131;
  const double x573 = x119 * x150;
  const double x574 = x113 * x123;
  const double x575 = 2 * x573 + 2 * x574;
  const double x576 = -x22 + x231;
  const double x577 = x129 * x576;
  const double x578 = -x28 + x322;
  const double x579 = x134 * x578;
  const double x580 = -1.0 / 2.0 * x577 - 1.0 / 2.0 * x579;
  const double x581 = -x577 - x579;
  const double x582 = (1.0 / 2.0) * x135 * x138 * x575
                      + (1.0 / 2.0) * x136 * x178 * x581 - x141 * x572
                      - x174 * x580;
  const double x583 = -x566;
  const double x584 = x108 * x583 + x258 * x570;
  const double x585 = x573 + x574;
  const double x586 = x119 * x172;
  const double x587 = x119 * x176;
  const double x588 = x182 * x575;
  const double x589 = -2 * x577 - 2 * x579;
  const double x590 = -3.0 / 2.0 * x577 - 3.0 / 2.0 * x579;
  const double x591 = -x307 + x66;
  const double x592 = x435 - x46 + x54 + x591;
  const double x593 = x83 + x84;
  const double x594 = -x10 + x438;
  const double x595 = x441 - x7;
  const double x596 = 8 * a_1 * b_1 * c_0 + 8 * a_2 * b_2 * c_0 - x203 - x204
                      + x206 + x207 - x210 - x213 + x216 + x217 - x227 - x443
                      - x444 + x445 + x446 - x447 - x448 + x449 + x450 - x451;
  const double x597
      = x144 * x566 + x146 * x571
        + x147
              * (x157 * x585 - x169 * x580 - x174 * (x113 * x170 + x586)
                 + x180 * (x113 * x175 + x587) + x185 * x589 - x187 * x590
                 - x284 * x572 + x285 * x581 + x287 * x588)
        + x190 * x582
        + x230
              * (x104 * x592 + x108 * (-x593 - x594 - x595) + x198 * x570
                 + x229 * x596 + x295 * x583)
        + x261 * x584;
  const double x598 = -x232 - x233 + x238 + x241 - x243 - x245 + x248 - x314
                      - x315 + x454 + x455 - x456 - x457 + x459 + x460 - x462
                      - x463 + x464 + x85 + x88;
  const double x599 = x549 + x64;
  const double x600 = x250 - x251 + x469 + x599;
  const double x601 = -x124;
  const double x602 = x119 * x262 + x165 * x601;
  const double x603 = x123 * x601;
  const double x604 = x112 * x119;
  const double x605 = 2 * x603 + 2 * x604;
  const double x606 = x476 - x89;
  const double x607 = -x606;
  const double x608 = x129 * x607;
  const double x609 = x121 * x578;
  const double x610 = -1.0 / 2.0 * x608 - 1.0 / 2.0 * x609;
  const double x611 = -x608 - x609;
  const double x612 = (1.0 / 2.0) * x135 * x138 * x605
                      + (1.0 / 2.0) * x136 * x178 * x611 - x141 * x602
                      - x174 * x610;
  const double x613 = -x600;
  const double x614 = -x598;
  const double x615 = x108 * x614 + x258 * x613;
  const double x616 = b_2 + x122;
  const double x617 = x603 + x604;
  const double x618 = x28 + x487;
  const double x619 = -x618;
  const double x620 = x182 * x605;
  const double x621 = -2 * x608 - 2 * x609;
  const double x622 = -3.0 / 2.0 * x608 - 3.0 / 2.0 * x609;
  const double x623 = -x29 + x378;
  const double x624 = -x48 - x494 + x56 - x623;
  const double x625 = 4 * x34;
  const double x626 = x38 - x625;
  const double x627 = x458 - x497;
  const double x628 = 8 * a_0 * b_0 * c_1 + 8 * a_2 * b_2 * c_1 - x298 - x299
                      + x300 + x301 - x305 - x308 + x310 + x312 - x319 - x502
                      - x503 + x504 + x505 - x506 - x507 + x508 + x509 - x510;
  const double x629
      = x144 * x598 + x146 * x600
        + x147
              * (-x141 * x616 + x157 * x617 - x169 * x610
                 - x174 * (x129 + x170 * x601) + x180 * (x175 * x601 + x355)
                 + x185 * x621 - x187 * x622 - x284 * x602 + x285 * x611
                 + x287 * x620 + x489 * x619)
        + x190 * x612
        + x230
              * (x104 * x624 + x108 * (-x500 - x524 - x626 - x627 - x70)
                 + x198 * x613 + x229 * x628 + x258 * x619 + x295 * x614)
        + x261 * x615;
  const double x630 = -x323 - x324 + x329 + x332 - x334 - x336 + x339 - x387
                      - x388 + x513 + x514 - x515 - x516 + x518 + x519 - x520
                      - x521 + x522 + x91 + x92;
  const double x631 = x496 + x62;
  const double x632 = x341 - x342 + x525 + x631;
  const double x633 = -x632;
  const double x634 = -x113;
  const double x635 = x131 * x601 + x262 * x634;
  const double x636 = x150 * x601;
  const double x637 = x112 * x634;
  const double x638 = 2 * x636 + 2 * x637;
  const double x639 = x134 * x607;
  const double x640 = -x576;
  const double x641 = x121 * x640;
  const double x642 = -1.0 / 2.0 * x639 - 1.0 / 2.0 * x641;
  const double x643 = -x639 - x641;
  const double x644 = (1.0 / 2.0) * x135 * x138 * x638
                      + (1.0 / 2.0) * x136 * x178 * x643 - x141 * x635
                      - x174 * x642;
  const double x645 = -x630;
  const double x646 = x108 * x645 + x258 * x632;
  const double x647 = b_1 + x130;
  const double x648 = x636 + x637;
  const double x649 = x22 + x541;
  const double x650 = x268 - x269;
  const double x651 = x182 * x638;
  const double x652 = -2 * x639 - 2 * x641;
  const double x653 = -3.0 / 2.0 * x639 - 3.0 / 2.0 * x641;
  const double x654 = -x23 + x304;
  const double x655 = -x51 + x547 + x58 + x654;
  const double x656 = 4 * x36;
  const double x657 = x40 - x656;
  const double x658 = x517 - x550;
  const double x659 = 8 * a_0 * b_0 * c_2 + 8 * a_1 * b_1 * c_2 - x372 - x373
                      + x374 + x375 - x379 - x381 + x383 + x385 - x392 - x555
                      - x556 + x557 + x558 - x559 - x560 + x561 + x562 - x563;
  const double x660
      = x144 * x630 + x146 * x633
        + x147
              * (x141 * x647 + x157 * x648 - x169 * x642
                 - x174 * (x134 + x172 * x601) + x180 * (x176 * x601 + x650)
                 + x185 * x652 - x187 * x653 - x284 * x635 + x285 * x643
                 + x287 * x651 + x489 * x649)
        + x190 * x644
        + x230
              * (x104 * x655 + x108 * (-x467 - x553 - x657 - x658 - x74)
                 + x198 * x632 + x229 * x659 + x258 * x649 + x295 * x645)
        + x261 * x646;
  const double x661 = -x408;
  const double x662 = x102 + x569 + x661 + x69;
  const double x663 = -x662;
  const double x664 = 1.0 / x61;
  const double x665 = x188 * x664;
  const double x666 = -x121;
  const double x667 = -x155;
  const double x668 = x182 * x287;
  const double x669 = x192 + x591 - x68 + x76;
  const double x670 = -x669;
  const double x671 = -x121 * x141 + (1.0 / 2.0) * x135 * x138 * x667;
  const double x672
      = x146 * x662 + x147 * (-x121 * x284 + x157 * x666 + x667 * x668)
        + x190 * x671 + x230 * (x104 * x670 + x198 * x663) + x663 * x665;
  const double x673 = -x65;
  const double x674 = x255 + x466 + x599 + x673;
  const double x675 = -x134;
  const double x676 = (1.0 / 2.0) * x135 * x138 * x650 - x141 * x675;
  const double x677 = -x674;
  const double x678 = -x19 + x291 + x32 + x623;
  const double x679 = x146 * x677
                      + x147
                            * (x134 * x157 - x141 * x172 + x162 * x489
                               - x284 * x675 + x650 * x668)
                      + x190 * x676
                      + x230 * (x104 * x678 + x162 * x258 + x198 * x674)
                      + x665 * x674;
  const double x680 = -x63;
  const double x681 = x344 + x461 + x631 + x680;
  const double x682 = -x681;
  const double x683 = -x129;
  const double x684 = -x355;
  const double x685 = -x129 * x141 + (1.0 / 2.0) * x135 * x138 * x684;
  const double x686 = -x16 + x26 + x367 + x654;
  const double x687 = -x686;
  const double x688 = x146 * x681
                      + x147
                            * (-x129 * x284 - x141 * x148 + x157 * x683
                               + x175 * x489 + x668 * x684)
                      + x190 * x685
                      + x230 * (x104 * x687 + x175 * x258 + x198 * x682)
                      + x665 * x682;
  const double x689 = x143 * x260;
  const double x690 = x145 * x260;
  const double x691 = x139 * x271;
  const double x692 = x168 * x277;
  const double x693 = x263 * x264;
  const double x694 = x264 * x273;
  const double x695 = x184 * x278;
  const double x696 = x186 * x277;
  const double x697 = x189 * x260;
  const double x698 = x257 * x97;
  const double x699 = -4 * b_0 * c_0;
  const double x700 = 2 * x81;
  const double x701 = x4 + x700;
  const double x702 = 2 * x80;
  const double x703 = x10 + x702;
  const double x704 = x197 * x257;
  const double x705 = x228 * x259;
  const double x706 = x189 * x279;
  const double x707 = x168 * x265;
  const double x708 = (1.0 / 2.0) * x278;
  const double x709 = (1.0 / 2.0) * x286;
  const double x710 = -x77;
  const double x711 = c_2 * x25;
  const double x712 = x197 * x259;
  const double x713
      = x147
            * (-x170 * x172 * x174 + x170 * x176 * x180 - x350 * x692
               - x359 * x707 + x360 * x709 + x362 * x691 + x363 * x708
               + x364 * x695 - x365 * x696)
        + x230
              * (x108 * (x101 + x710 + x711 + x72) + x345 * x712 + x347 * x704
                 + x368 * x698 + x393 * x705)
        + x340 * x689 + x346 * x690 + x348 * x706 + x361 * x697;
  const double x714 = -x488;
  const double x715 = -x62;
  const double x716 = 4 * x23;
  const double x717 = 4 * x26;
  const double x718 = x63 - x717;
  const double x719
      = x147
            * (-x141 * x485 - x174 * (x129 + x263 * x411)
               + x180 * (x273 * x411 + x355) - x413 * x692 - x423 * x707
               + x424 * x709 + x429 * x691 + x432 * x708 + x433 * x695
               - x434 * x696 + x489 * x714)
        + x230
              * (x108 * (-x239 - x626 - x715 - x716 - x718) + x258 * x714
                 + x426 * x712 + x427 * x704 + x436 * x698 + x452 * x705)
        + x406 * x689 + x410 * x690 + x425 * x697 + x428 * x706;
  const double x720 = x127 * x263;
  const double x721 = x127 * x273;
  const double x722 = c_0 * x89;
  const double x723 = -a_0 * x252;
  const double x724 = a_0 * x89;
  const double x725 = x723 + x724;
  const double x726 = -x722 + x725;
  const double x727
      = x147
            * (-x174 * (x430 + x720) + x180 * (x431 + x721) - x472 * x692
               - x480 * x707 + x481 * x709 + x486 * x691 + x491 * x708
               + x492 * x695 - x493 * x696)
        + x230
              * (x108 * (x439 + x701 + x726) + x470 * x712 + x483 * x704
                 + x495 * x698 + x511 * x705)
        + x465 * x689 + x471 * x690 + x482 * x697 + x484 * x706;
  const double x728 = c_0 + x111;
  const double x729 = -2 * d_0;
  const double x730 = x252 + x729;
  const double x731 = -x153 + x154;
  const double x732 = 4 * x66;
  const double x733 = 4 * x55;
  const double x734 = -x733 + x77;
  const double x735 = 4 * x68;
  const double x736 = x69 - x735;
  const double x737
      = x147
            * (x141 * x728 - x174 * (x118 * x172 + x166)
               + x180 * (x118 * x176 + x731) + x489 * x730 - x527 * x692
               - x533 * x707 + x534 * x709 + x540 * x691 + x544 * x708
               + x545 * x695 - x546 * x696)
        + x230
              * (x108 * (-x567 - x711 - x732 - x734 - x736) + x258 * x730
                 + x536 * x712 + x537 * x704 + x548 * x698 + x564 * x705)
        + x523 * x689 + x526 * x690 + x535 * x697 + x538 * x706;
  const double x738 = x62 - x716;
  const double x739
      = x147
            * (x141 * x616 - x174 * (x113 * x263 + x349)
               + x180 * (x113 * x273 + x490) + x489 * x618 - x572 * x692
               - x580 * x707 + x581 * x709 + x585 * x691 + x588 * x708
               + x589 * x695 - x590 * x696)
        + x230
              * (x108 * (-x499 - x680 - x70 - x717 - x738) + x258 * x618
                 + x570 * x712 + x583 * x704 + x592 * x698 + x596 * x705)
        + x566 * x689 + x571 * x690 + x582 * x697 + x584 * x706;
  const double x740 = x263 * x601;
  const double x741 = x273 * x601;
  const double x742 = x722 + x84;
  const double x743 = -x702 + x725;
  const double x744
      = x147
            * (-x174 * (x586 + x740) + x180 * (x587 + x741) - x602 * x692
               - x610 * x707 + x611 * x709 + x617 * x691 + x620 * x708
               + x621 * x695 - x622 * x696)
        + x230
              * (x108 * (-x594 - x742 - x743) + x613 * x712 + x614 * x704
                 + x624 * x698 + x628 * x705)
        + x598 * x689 + x600 * x690 + x612 * x697 + x615 * x706;
  const double x745 = b_0 + x111;
  const double x746 = x729 + x89;
  const double x747 = -x746;
  const double x748 = 4 * x45;
  const double x749 = x73 - x748;
  const double x750 = x67 - x732;
  const double x751
      = x147
            * (-x141 * x745 - x174 * (x121 + x172 * x634)
               + x180 * (x155 + x176 * x634) + x489 * x747 - x635 * x692
               - x642 * x707 + x643 * x709 + x648 * x691 + x651 * x708
               + x652 * x695 - x653 * x696)
        + x230
              * (x108 * (-x407 - x72 - x735 - x749 - x750) + x258 * x747
                 + x632 * x712 + x645 * x704 + x655 * x698 + x659 * x705)
        + x630 * x689 + x633 * x690 + x644 * x697 + x646 * x706;
  const double x752 = x279 * x664;
  const double x753 = x182 * x708;
  const double x754 = x147
                          * (-x121 * x692 - x141 * x151 + x176 * x489
                             + x666 * x691 + x667 * x753)
                      + x230 * (x176 * x258 + x663 * x712 + x670 * x698)
                      + x662 * x690 + x663 * x752 + x671 * x697;
  const double x755 = x147 * (x134 * x691 + x650 * x753 - x675 * x692)
                      + x230 * (x674 * x712 + x678 * x698) + x674 * x752
                      + x676 * x697 + x677 * x690;
  const double x756 = x147
                          * (-x129 * x692 - x141 * x264 + x273 * x489
                             + x683 * x691 + x684 * x753)
                      + x230 * (x258 * x273 + x682 * x712 + x687 * x698)
                      + x681 * x690 + x682 * x752 + x685 * x697;
  const double x757 = x143 * x348;
  const double x758 = x145 * x348;
  const double x759 = x139 * x356;
  const double x760 = x168 * x359;
  const double x761 = x184 * x360;
  const double x762 = x186 * x359;
  const double x763 = x189 * x348;
  const double x764 = x345 * x97;
  const double x765 = x1 + x700;
  const double x766 = x7 + x702;
  const double x767 = x197 * x347;
  const double x768 = x228 * x347;
  const double x769 = x189 * x361;
  const double x770 = x168 * x350;
  const double x771 = (1.0 / 2.0) * x360;
  const double x772 = (1.0 / 2.0) * x363;
  const double x773 = -x64;
  const double x774 = 4 * x29;
  const double x775 = 4 * x32;
  const double x776 = x65 - x775;
  const double x777 = x197 * x345;
  const double x778
      = x147
            * (x141 * x539 - x174 * (x134 + x263 * x412)
               + x180 * (x273 * x412 + x650) - x413 * x760 - x423 * x770
               + x424 * x772 + x429 * x759 + x432 * x771 + x433 * x761
               - x434 * x762 + x489 * x542)
        + x230
              * (x108 * (-x330 - x657 - x773 - x774 - x776) + x258 * x542
                 + x426 * x767 + x427 * x777 + x436 * x764 + x452 * x768)
        + x406 * x757 + x410 * x758 + x425 * x763 + x428 * x769;
  const double x779 = -x730;
  const double x780 = -x568;
  const double x781 = 4 * x307;
  const double x782 = 4 * x76;
  const double x783 = x408 - x782;
  const double x784
      = x147
            * (-x141 * x728 - x174 * (x121 + x148 * x412)
               + x180 * (x155 + x159 * x412) - x472 * x760 - x480 * x770
               + x481 * x772 + x486 * x759 + x489 * x779 + x491 * x771
               + x492 * x761 - x493 * x762)
        + x230
              * (x108 * (-x711 - x749 - x780 - x781 - x783) + x258 * x779
                 + x470 * x767 + x483 * x777 + x495 * x764 + x511 * x768)
        + x465 * x757 + x471 * x758 + x482 * x763 + x484 * x769;
  const double x785
      = x147
            * (-x174 * (x118 * x148 + x720) + x180 * (x118 * x159 + x721)
               - x527 * x760 - x533 * x770 + x534 * x772 + x540 * x759
               + x544 * x771 + x545 * x761 - x546 * x762)
        + x230
              * (x108 * (x442 + x726 + x765) + x536 * x767 + x537 * x777
                 + x548 * x764 + x564 * x768)
        + x523 * x757 + x526 * x758 + x535 * x763 + x538 * x769;
  const double x786 = -x649;
  const double x787 = x64 - x774;
  const double x788
      = x147
            * (-x141 * x647 - x174 * (x119 * x263 + x280)
               + x180 * (x119 * x273 + x270) + x489 * x786 - x572 * x760
               - x580 * x770 + x581 * x772 + x585 * x759 + x588 * x771
               + x589 * x761 - x590 * x762)
        + x230
              * (x108 * (-x552 - x673 - x74 - x775 - x787) + x258 * x786
                 + x570 * x767 + x583 * x777 + x592 * x764 + x596 * x768)
        + x566 * x757 + x571 * x758 + x582 * x763 + x584 * x769;
  const double x789 = x568 - x781;
  const double x790
      = x147
            * (x141 * x745 - x174 * (x119 * x148 + x166)
               + x180 * (x119 * x159 + x731) + x489 * x746 - x602 * x760
               - x610 * x770 + x611 * x772 + x617 * x759 + x620 * x771
               + x621 * x761 - x622 * x762)
        + x230
              * (x108 * (-x661 - x72 - x734 - x782 - x789) + x258 * x746
                 + x613 * x767 + x614 * x777 + x624 * x764 + x628 * x768)
        + x598 * x757 + x600 * x758 + x612 * x763 + x615 * x769;
  const double x791 = x722 + x83;
  const double x792
      = x147
            * (-x174 * (x148 * x634 + x740) + x180 * (x159 * x634 + x741)
               - x635 * x760 - x642 * x770 + x643 * x772 + x648 * x759
               + x651 * x771 + x652 * x761 - x653 * x762)
        + x230
              * (x108 * (-x595 - x743 - x791) + x632 * x767 + x645 * x777
                 + x655 * x764 + x659 * x768)
        + x630 * x757 + x633 * x758 + x644 * x763 + x646 * x769;
  const double x793 = x361 * x664;
  const double x794 = x182 * x771;
  const double x795 = x147
                          * (-x121 * x760 - x141 * x170 + x159 * x489
                             + x666 * x759 + x667 * x794)
                      + x230 * (x159 * x258 + x663 * x767 + x670 * x764)
                      + x662 * x758 + x663 * x793 + x671 * x763;
  const double x796 = x147
                          * (x134 * x759 - x141 * x263 + x274 * x489
                             + x650 * x794 - x675 * x760)
                      + x230 * (x258 * x274 + x674 * x767 + x678 * x764)
                      + x674 * x793 + x676 * x763 + x677 * x758;
  const double x797 = x147 * (-x129 * x760 + x683 * x759 + x684 * x794)
                      + x230 * (x682 * x767 + x687 * x764) + x681 * x758
                      + x682 * x793 + x685 * x763;
  const double x798 = x143 * x428;
  const double x799 = x145 * x428;
  const double x800 = x139 * x416;
  const double x801 = x168 * x423;
  const double x802 = x118 * x411;
  const double x803 = x115 * x412;
  const double x804 = x412 * x420;
  const double x805 = x184 * x424;
  const double x806 = x186 * x423;
  const double x807 = x189 * x428;
  const double x808 = x426 * x97;
  const double x809 = 2 * x82;
  const double x810 = -4 * a_2 * c_2 + x809;
  const double x811 = 2 * x79;
  const double x812 = -4 * a_1 * c_1 + x811;
  const double x813 = x197 * x427;
  const double x814 = x228 * x427;
  const double x815 = x189 * x425;
  const double x816 = x168 * x413;
  const double x817 = x127 * x174;
  const double x818 = x127 * x180;
  const double x819 = (1.0 / 2.0) * x424;
  const double x820 = (1.0 / 2.0) * x432;
  const double x821 = a_0 * x231;
  const double x822 = x197 * x426;
  const double x823 = x147
                          * (-x118 * x817 + x417 * x818 - x472 * x801
                             - x480 * x816 + x481 * x820 + x486 * x800
                             + x491 * x819 + x492 * x805 - x493 * x806)
                      + x230
                            * (x108 * (x239 + x524 + x680 + x821) + x470 * x813
                               + x483 * x822 + x495 * x808 + x511 * x814)
                      + x465 * x798 + x471 * x799 + x482 * x807 + x484 * x815;
  const double x824 = a_0 * x322;
  const double x825 = x147
                          * (-x115 * x817 + x420 * x818 - x527 * x801
                             - x533 * x816 + x534 * x820 + x540 * x800
                             + x544 * x819 + x545 * x805 - x546 * x806)
                      + x230
                            * (x108 * (x330 + x467 + x673 + x824) + x536 * x813
                               + x537 * x822 + x548 * x808 + x564 * x814)
                      + x523 * x798 + x526 * x799 + x535 * x807 + x538 * x815;
  const double x826 = x115 * x119;
  const double x827 = x119 * x420;
  const double x828 = -x399 + x437 + x809;
  const double x829 = -x397 + x440 + x811;
  const double x830
      = x147
            * (-x174 * (x113 * x118 + x826) + x180 * (x113 * x417 + x827)
               - x572 * x801 - x580 * x816 + x581 * x820 + x585 * x800
               + x588 * x819 + x589 * x805 - x590 * x806)
        + x230
              * (x108 * (x593 + x828 + x829) + x570 * x813 + x583 * x822
                 + x592 * x808 + x596 * x814)
        + x566 * x798 + x571 * x799 + x582 * x807 + x584 * x815;
  const double x831 = x322 + x487;
  const double x832
      = x147
            * (-x141 * x165 - x174 * (x118 * x601 + x349)
               + x180 * (x417 * x601 + x490) + x489 * x831 - x602 * x801
               - x610 * x816 + x611 * x820 + x617 * x800 + x620 * x819
               + x621 * x805 - x622 * x806)
        + x230
              * (x108 * (-x294 - x501 - x625 - x738 - x821) + x258 * x831
                 + x613 * x813 + x614 * x822 + x624 * x808 + x628 * x814)
        + x598 * x798 + x600 * x799 + x612 * x807 + x615 * x815;
  const double x833 = x231 + x541;
  const double x834 = -x833;
  const double x835
      = x147
            * (-x131 * x141 - x174 * (x115 * x601 + x280)
               + x180 * (x270 + x420 * x601) + x489 * x834 - x635 * x801
               - x642 * x816 + x643 * x820 + x648 * x800 + x651 * x819
               + x652 * x805 - x653 * x806)
        + x230
              * (x108 * (-x254 - x554 - x656 - x787 - x824) + x258 * x834
                 + x632 * x813 + x645 * x822 + x655 * x808 + x659 * x814)
        + x630 * x798 + x633 * x799 + x644 * x807 + x646 * x815;
  const double x836 = x425 * x664;
  const double x837 = x182 * x819;
  const double x838 = x147 * (-x121 * x801 + x666 * x800 + x667 * x837)
                      + x230 * (x663 * x813 + x670 * x808) + x662 * x799
                      + x663 * x836 + x671 * x807;
  const double x839 = x147
                          * (-x115 * x141 + x134 * x800 + x421 * x489
                             + x650 * x837 - x675 * x801)
                      + x230 * (x258 * x421 + x674 * x813 + x678 * x808)
                      + x674 * x836 + x676 * x807 + x677 * x799;
  const double x840 = x147
                          * (-x129 * x801 - x141 * x411 + x417 * x489
                             + x683 * x800 + x684 * x837)
                      + x230 * (x258 * x417 + x682 * x813 + x687 * x808)
                      + x681 * x799 + x682 * x836 + x685 * x807;
  const double x841 = x143 * x484;
  const double x842 = x145 * x484;
  const double x843 = x139 * x475;
  const double x844 = x168 * x480;
  const double x845 = -x127;
  const double x846 = x127 * x845;
  const double x847 = -x477;
  const double x848 = x127 * x847;
  const double x849 = x184 * x481;
  const double x850 = x186 * x480;
  const double x851 = x189 * x484;
  const double x852 = x470 * x97;
  const double x853 = 2 * x78;
  const double x854 = -4 * a_0 * c_0 + x853;
  const double x855 = x197 * x483;
  const double x856 = x228 * x483;
  const double x857 = x189 * x482;
  const double x858 = x168 * x472;
  const double x859 = (1.0 / 2.0) * x481;
  const double x860 = (1.0 / 2.0) * x491;
  const double x861 = a_2 * x231;
  const double x862 = x197 * x470;
  const double x863
      = x147
            * (-x115 * x118 * x174 + x118 * x180 * x420 - x527 * x844
               - x533 * x858 + x534 * x860 + x540 * x843 + x544 * x859
               + x545 * x849 - x546 * x850)
        + x230
              * (x108 * (x407 + x661 + x711 + x861) + x536 * x855 + x537 * x862
                 + x548 * x852 + x564 * x856)
        + x523 * x841 + x526 * x842 + x535 * x851 + x538 * x857;
  const double x864 = -x831;
  const double x865
      = x147
            * (-x123 * x141 - x174 * (x113 * x845 + x129)
               + x180 * (x113 * x847 + x355) + x489 * x864 - x572 * x844
               - x580 * x858 + x581 * x860 + x585 * x843 + x588 * x859
               + x589 * x849 - x590 * x850)
        + x230
              * (x108 * (-x293 - x498 - x627 - x718 - x821) + x258 * x864
                 + x570 * x855 + x583 * x862 + x592 * x852 + x596 * x856)
        + x566 * x841 + x571 * x842 + x582 * x851 + x584 * x857;
  const double x866 = x601 * x845;
  const double x867 = x601 * x847;
  const double x868 = x723 - x724 + x853;
  const double x869
      = x147
            * (-x174 * (x826 + x866) + x180 * (x827 + x867) - x602 * x844
               - x610 * x858 + x611 * x860 + x617 * x843 + x620 * x859
               + x621 * x849 - x622 * x850)
        + x230
              * (x108 * (x742 + x828 + x868) + x613 * x855 + x614 * x862
                 + x624 * x852 + x628 * x856)
        + x598 * x841 + x600 * x842 + x612 * x851 + x615 * x857;
  const double x870 = x476 + x729;
  const double x871
      = x147
            * (-x141 * x262 - x174 * (x115 * x634 + x166)
               + x180 * (x420 * x634 + x731) + x489 * x870 - x635 * x844
               - x642 * x858 + x643 * x860 + x648 * x843 + x651 * x859
               + x652 * x849 - x653 * x850)
        + x230
              * (x108 * (-x101 - x736 - x748 - x789 - x861) + x258 * x870
                 + x632 * x855 + x645 * x862 + x655 * x852 + x659 * x856)
        + x630 * x841 + x633 * x842 + x644 * x851 + x646 * x857;
  const double x872 = x482 * x664;
  const double x873 = x182 * x859;
  const double x874 = x147
                          * (-x121 * x844 - x141 * x412 + x420 * x489
                             + x666 * x843 + x667 * x873)
                      + x230 * (x258 * x420 + x663 * x855 + x670 * x852)
                      + x662 * x842 + x663 * x872 + x671 * x851;
  const double x875 = x147 * (x134 * x843 + x650 * x873 - x675 * x844)
                      + x230 * (x674 * x855 + x678 * x852) + x674 * x872
                      + x676 * x851 + x677 * x842;
  const double x876 = x147
                          * (-x127 * x141 - x129 * x844 + x489 * x847
                             + x683 * x843 + x684 * x873)
                      + x230 * (x258 * x847 + x682 * x855 + x687 * x852)
                      + x681 * x842 + x682 * x872 + x685 * x851;
  const double x877 = x143 * x538;
  const double x878 = x145 * x538;
  const double x879 = x139 * x530;
  const double x880 = x168 * x527;
  const double x881 = x184 * x534;
  const double x882 = x186 * x533;
  const double x883 = x189 * x538;
  const double x884 = x536 * x97;
  const double x885 = x197 * x536;
  const double x886 = x228 * x537;
  const double x887 = x189 * x535;
  const double x888 = x168 * x533;
  const double x889 = (1.0 / 2.0) * x544;
  const double x890 = (1.0 / 2.0) * x534;
  const double x891 = x197 * x537;
  const double x892
      = x147
            * (-x141 * x150 - x174 * (x119 * x845 + x134)
               + x180 * (x119 * x847 + x650) + x489 * x833 - x572 * x888
               - x580 * x880 + x581 * x889 + x585 * x879 + x588 * x890
               + x589 * x881 - x590 * x882)
        + x230
              * (x108 * (-x369 - x551 - x658 - x776 - x824) + x258 * x833
                 + x570 * x891 + x583 * x885 + x592 * x884 + x596 * x886)
        + x566 * x877 + x571 * x878 + x582 * x883 + x584 * x887;
  const double x893 = -x870;
  const double x894
      = x147
            * (-x112 * x141 - x174 * (x119 * x411 + x121)
               + x180 * (x119 * x418 + x155) + x489 * x893 - x602 * x888
               - x610 * x880 + x611 * x889 + x617 * x879 + x620 * x890
               + x621 * x881 - x622 * x882)
        + x230
              * (x108 * (-x710 - x733 - x750 - x783 - x861) + x258 * x893
                 + x613 * x891 + x614 * x885 + x624 * x884 + x628 * x886)
        + x598 * x877 + x600 * x878 + x612 * x883 + x615 * x887;
  const double x895
      = x147
            * (-x174 * (x411 * x634 + x866) + x180 * (x418 * x634 + x867)
               - x635 * x888 - x642 * x880 + x643 * x889 + x648 * x879
               + x651 * x890 + x652 * x881 - x653 * x882)
        + x230
              * (x108 * (x791 + x829 + x868) + x632 * x891 + x645 * x885
                 + x655 * x884 + x659 * x886)
        + x630 * x877 + x633 * x878 + x644 * x883 + x646 * x887;
  const double x896 = x535 * x664;
  const double x897 = x182 * x890;
  const double x898 = x147
                          * (-x118 * x141 - x121 * x888 + x418 * x489
                             + x666 * x879 + x667 * x897)
                      + x230 * (x258 * x418 + x663 * x891 + x670 * x884)
                      + x662 * x878 + x663 * x896 + x671 * x883;
  const double x899 = x147
                          * (x134 * x879 - x141 * x845 + x477 * x489
                             + x650 * x897 - x675 * x888)
                      + x230 * (x258 * x477 + x674 * x891 + x678 * x884)
                      + x674 * x896 + x676 * x883 + x677 * x878;
  const double x900 = x147 * (-x129 * x888 + x683 * x879 + x684 * x897)
                      + x230 * (x682 * x891 + x687 * x884) + x681 * x878
                      + x682 * x896 + x685 * x883;
  const double x901 = x143 * x584;
  const double x902 = x145 * x584;
  const double x903 = x139 * x575;
  const double x904 = x168 * x572;
  const double x905 = x113 * x634;
  const double x906 = -x119;
  const double x907 = x119 * x906;
  const double x908 = -x578;
  const double x909 = x119 * x908;
  const double x910 = x184 * x581;
  const double x911 = x186 * x580;
  const double x912 = x189 * x584;
  const double x913 = x570 * x97;
  const double x914 = -4 * a_2 * b_2 + x809;
  const double x915 = -4 * a_1 * b_1 + x811;
  const double x916 = x197 * x583;
  const double x917 = x228 * x583;
  const double x918 = x189 * x582;
  const double x919 = x168 * x580;
  const double x920 = x174 * x601;
  const double x921 = x180 * x601;
  const double x922 = (1.0 / 2.0) * x588;
  const double x923 = (1.0 / 2.0) * x581;
  const double x924 = x197 * x570;
  const double x925 = x147
                          * (-x602 * x919 - x610 * x904 + x611 * x922
                             + x617 * x903 + x620 * x923 + x621 * x910
                             - x622 * x911 - x634 * x920 + x640 * x921)
                      + x230
                            * (x108 * (x496 + x70 + x715 + x821) + x613 * x916
                               + x614 * x924 + x624 * x913 + x628 * x917)
                      + x598 * x901 + x600 * x902 + x612 * x912 + x615 * x918;
  const double x926 = x147
                          * (-x635 * x919 - x642 * x904 + x643 * x922
                             + x648 * x903 + x651 * x923 + x652 * x910
                             - x653 * x911 - x906 * x920 + x908 * x921)
                      + x230
                            * (x108 * (x549 + x74 + x773 + x824) + x632 * x916
                               + x645 * x924 + x655 * x913 + x659 * x917)
                      + x630 * x901 + x633 * x902 + x644 * x912 + x646 * x918;
  const double x927 = x582 * x664;
  const double x928 = x182 * x923;
  const double x929 = x147 * (-x121 * x919 + x666 * x903 + x667 * x928)
                      + x230 * (x663 * x916 + x670 * x913) + x662 * x902
                      + x663 * x927 + x671 * x912;
  const double x930 = x147
                          * (x134 * x903 - x141 * x906 + x489 * x578
                             + x650 * x928 - x675 * x919)
                      + x230 * (x258 * x578 + x674 * x916 + x678 * x913)
                      + x674 * x927 + x676 * x912 + x677 * x902;
  const double x931 = x147
                          * (-x113 * x141 - x129 * x919 + x489 * x640
                             + x683 * x903 + x684 * x928)
                      + x230 * (x258 * x640 + x682 * x916 + x687 * x913)
                      + x681 * x902 + x682 * x927 + x685 * x912;
  const double x932 = x143 * x615;
  const double x933 = x145 * x615;
  const double x934 = x139 * x605;
  const double x935 = x168 * x610;
  const double x936 = x124 * x601;
  const double x937 = x601 * x606;
  const double x938 = x184 * x611;
  const double x939 = x186 * x610;
  const double x940 = x189 * x615;
  const double x941 = x613 * x97;
  const double x942 = -4 * a_0 * b_0 + x853;
  const double x943 = x197 * x614;
  const double x944 = x228 * x614;
  const double x945 = x189 * x646;
  const double x946 = x168 * x642;
  const double x947 = (1.0 / 2.0) * x611;
  const double x948 = (1.0 / 2.0) * x643;
  const double x949 = x197 * x645;
  const double x950
      = x147
            * (-x174 * x634 * x906 + x180 * x634 * x908 - x602 * x946
               + x620 * x948 - x635 * x935 + x648 * x934 + x651 * x947
               + x652 * x938 - x653 * x939)
        + x230
              * (x108 * (x567 + x72 + x780 + x861) + x613 * x949 + x632 * x943
                 + x655 * x941 + x659 * x944)
        + x612 * x945 + x630 * x932 + x633 * x933 + x644 * x940;
  const double x951 = x612 * x664;
  const double x952 = x182 * x947;
  const double x953 = x147
                          * (-x119 * x141 - x121 * x935 + x489 * x908
                             + x666 * x934 + x667 * x952)
                      + x230 * (x258 * x908 + x663 * x943 + x670 * x941)
                      + x662 * x933 + x663 * x951 + x671 * x940;
  const double x954 = x147 * (x134 * x934 + x650 * x952 - x675 * x935)
                      + x230 * (x674 * x943 + x678 * x941) + x674 * x951
                      + x676 * x940 + x677 * x933;
  const double x955 = x147
                          * (-x129 * x935 - x141 * x601 + x489 * x606
                             + x683 * x934 + x684 * x952)
                      + x230 * (x258 * x606 + x682 * x943 + x687 * x941)
                      + x681 * x933 + x682 * x951 + x685 * x940;
  const double x956 = x145 * x646;
  const double x957 = x139 * x638;
  const double x958 = x632 * x97;
  const double x959 = x644 * x664;
  const double x960 = x182 * x948;
  const double x961 = x147
                          * (-x121 * x946 - x141 * x634 + x489 * x576
                             + x666 * x957 + x667 * x960)
                      + x230 * (x258 * x576 + x663 * x949 + x670 * x958)
                      + x662 * x956 + x663 * x959 + x671 * x945;
  const double x962 = x147
                          * (-x124 * x141 + x134 * x957 + x489 * x607
                             + x650 * x960 - x675 * x946)
                      + x230 * (x258 * x607 + x674 * x949 + x678 * x958)
                      + x674 * x959 + x676 * x945 + x677 * x956;
  const double x963 = x147 * (-x129 * x946 + x683 * x957 + x684 * x960)
                      + x230 * (x682 * x949 + x687 * x958) + x681 * x956
                      + x682 * x959 + x685 * x945;
  const double x964 = x663 * x664;
  const double x965 = x143 * x663;
  const double x966 = x139 * x147;
  const double x967 = x667 * x966;
  const double x968 = x664 * x671;
  const double x969 = -x678;
  const double x970 = x134 * x967 + x674 * x968 + x676 * x964 + x965 * x969;
  const double x971 = x682 * x968 + x683 * x967 + x685 * x964 + x686 * x965;
  const double x972 = x664 * x674;
  const double x973 = x143 * x674;
  const double x974 = x650 * x966;
  const double x975 = x664 * x682;
  const double x976 = x676 * x975 + x683 * x974 + x685 * x972 + x686 * x973;

  out_3216172810630303951[0]
      = -x103 * x146 + x144 * x44
        + x147
              * (x157 * (x121 + x149 + x152) - 2 * x164 * x169
                 - x174 * (x171 + x173) + x180 * (x148 * x175 + x177)
                 + x181 * x183 + x185 * (-2 * x160 - 2 * x163)
                 - x187 * (-3.0 / 2.0 * x160 - 3.0 / 2.0 * x163))
        + 2 * x188 * x190
        + x230
              * (2 * x103 * x198 + x104 * (x191 + x192 + x46 - x54)
                 + x108 * (-x193 - x194 - x195 - x196)
                 + x229
                       * (8 * a_0 * b_1 * c_1 + 8 * a_0 * b_2 * c_2 + x200
                          + x201 + x203 + x204 - x206 - x207 - x208 - x209
                          + x210 + x212 + x213 + x215 - x216 - x217 - x218
                          - x219 - x224 - x227));
  out_3216172810630303951[1] = x321;
  out_3216172810630303951[2] = x394;
  out_3216172810630303951[3] = x453;
  out_3216172810630303951[4] = x512;
  out_3216172810630303951[5] = x565;
  out_3216172810630303951[6] = x597;
  out_3216172810630303951[7] = x629;
  out_3216172810630303951[8] = x660;
  out_3216172810630303951[9] = x672;
  out_3216172810630303951[10] = x679;
  out_3216172810630303951[11] = x688;
  out_3216172810630303951[12] = x321;
  out_3216172810630303951[13]
      = x147
            * (-x174 * (x173 + x693) + x180 * (x177 + x694) - 2 * x265 * x692
               + x278 * x286 + x281 * x691 + x288 * x695 - x289 * x696)
        + x230
              * (x108 * (-x194 - x699 - x701 - x703) + 2 * x259 * x704
                 + x292 * x698 + x320 * x705)
        + x249 * x689 + x256 * x690 + 2 * x279 * x697;
  out_3216172810630303951[14] = x713;
  out_3216172810630303951[15] = x719;
  out_3216172810630303951[16] = x727;
  out_3216172810630303951[17] = x737;
  out_3216172810630303951[18] = x739;
  out_3216172810630303951[19] = x744;
  out_3216172810630303951[20] = x751;
  out_3216172810630303951[21] = x754;
  out_3216172810630303951[22] = x755;
  out_3216172810630303951[23] = x756;
  out_3216172810630303951[24] = x394;
  out_3216172810630303951[25] = x713;
  out_3216172810630303951[26]
      = x147
            * (-x174 * (x171 + x693) + x180 * (x159 * x170 + x694)
               - 2 * x350 * x760 + x360 * x363 + x362 * x759 + x364 * x761
               - x365 * x762)
        + x230
              * (x108 * (-x193 - x699 - x765 - x766) + 2 * x345 * x767
                 + x368 * x764 + x393 * x768)
        + x340 * x757 + x346 * x758 + 2 * x361 * x763;
  out_3216172810630303951[27] = x778;
  out_3216172810630303951[28] = x784;
  out_3216172810630303951[29] = x785;
  out_3216172810630303951[30] = x788;
  out_3216172810630303951[31] = x790;
  out_3216172810630303951[32] = x792;
  out_3216172810630303951[33] = x795;
  out_3216172810630303951[34] = x796;
  out_3216172810630303951[35] = x797;
  out_3216172810630303951[36] = x453;
  out_3216172810630303951[37] = x719;
  out_3216172810630303951[38] = x778;
  out_3216172810630303951[39]
      = x147
            * (-x174 * (x802 + x803) + x180 * (x411 * x417 + x804)
               - 2 * x413 * x801 + x424 * x432 + x429 * x800 + x433 * x805
               - x434 * x806)
        + x230
              * (x108 * (-x195 - x810 - x812) + 2 * x426 * x813 + x436 * x808
                 + x452 * x814)
        + x406 * x798 + x410 * x799 + 2 * x425 * x807;
  out_3216172810630303951[40] = x823;
  out_3216172810630303951[41] = x825;
  out_3216172810630303951[42] = x830;
  out_3216172810630303951[43] = x832;
  out_3216172810630303951[44] = x835;
  out_3216172810630303951[45] = x838;
  out_3216172810630303951[46] = x839;
  out_3216172810630303951[47] = x840;
  out_3216172810630303951[48] = x512;
  out_3216172810630303951[49] = x727;
  out_3216172810630303951[50] = x784;
  out_3216172810630303951[51] = x823;
  out_3216172810630303951[52]
      = x147
            * (-x174 * (x803 + x846) + x180 * (x804 + x848) - 2 * x472 * x844
               + x481 * x491 + x486 * x843 + x492 * x849 - x493 * x850)
        + x230
              * (x108 * (-x701 - x810 - x854) + 2 * x470 * x855 + x495 * x852
                 + x511 * x856)
        + x465 * x841 + x471 * x842 + 2 * x482 * x851;
  out_3216172810630303951[53] = x863;
  out_3216172810630303951[54] = x865;
  out_3216172810630303951[55] = x869;
  out_3216172810630303951[56] = x871;
  out_3216172810630303951[57] = x874;
  out_3216172810630303951[58] = x875;
  out_3216172810630303951[59] = x876;
  out_3216172810630303951[60] = x565;
  out_3216172810630303951[61] = x737;
  out_3216172810630303951[62] = x785;
  out_3216172810630303951[63] = x825;
  out_3216172810630303951[64] = x863;
  out_3216172810630303951[65]
      = x147
            * (-x174 * (x802 + x846) + x180 * (x118 * x418 + x848)
               - 2 * x533 * x880 + x534 * x544 + x540 * x879 + x545 * x881
               - x546 * x882)
        + x230
              * (x108 * (-x765 - x812 - x854) + 2 * x537 * x885 + x548 * x884
                 + x564 * x886)
        + x523 * x877 + x526 * x878 + 2 * x535 * x883;
  out_3216172810630303951[66] = x892;
  out_3216172810630303951[67] = x894;
  out_3216172810630303951[68] = x895;
  out_3216172810630303951[69] = x898;
  out_3216172810630303951[70] = x899;
  out_3216172810630303951[71] = x900;
  out_3216172810630303951[72] = x597;
  out_3216172810630303951[73] = x739;
  out_3216172810630303951[74] = x788;
  out_3216172810630303951[75] = x830;
  out_3216172810630303951[76] = x865;
  out_3216172810630303951[77] = x892;
  out_3216172810630303951[78]
      = x147
            * (-x174 * (x905 + x907) + x180 * (x113 * x640 + x909)
               - 2 * x580 * x904 + x581 * x588 + x585 * x903 + x589 * x910
               - x590 * x911)
        + x230
              * (x108 * (-x196 - x914 - x915) + 2 * x570 * x916 + x592 * x913
                 + x596 * x917)
        + x566 * x901 + x571 * x902 + 2 * x582 * x912;
  out_3216172810630303951[79] = x925;
  out_3216172810630303951[80] = x926;
  out_3216172810630303951[81] = x929;
  out_3216172810630303951[82] = x930;
  out_3216172810630303951[83] = x931;
  out_3216172810630303951[84] = x629;
  out_3216172810630303951[85] = x744;
  out_3216172810630303951[86] = x790;
  out_3216172810630303951[87] = x832;
  out_3216172810630303951[88] = x869;
  out_3216172810630303951[89] = x894;
  out_3216172810630303951[90] = x925;
  out_3216172810630303951[91]
      = x147
            * (-x174 * (x907 + x936) + x180 * (x909 + x937) - 2 * x602 * x935
               + x611 * x620 + x617 * x934 + x621 * x938 - x622 * x939)
        + x230
              * (x108 * (-x703 - x914 - x942) + 2 * x613 * x943 + x624 * x941
                 + x628 * x944)
        + x598 * x932 + x600 * x933 + 2 * x612 * x940;
  out_3216172810630303951[92] = x950;
  out_3216172810630303951[93] = x953;
  out_3216172810630303951[94] = x954;
  out_3216172810630303951[95] = x955;
  out_3216172810630303951[96] = x660;
  out_3216172810630303951[97] = x751;
  out_3216172810630303951[98] = x792;
  out_3216172810630303951[99] = x835;
  out_3216172810630303951[100] = x871;
  out_3216172810630303951[101] = x895;
  out_3216172810630303951[102] = x926;
  out_3216172810630303951[103] = x950;
  out_3216172810630303951[104]
      = x143 * x630 * x646
        + x147
              * (-x174 * (x905 + x936) + x180 * (x576 * x634 + x937)
                 + x184 * x643 * x652 - x186 * x642 * x653 - 2 * x635 * x946
                 + x643 * x651 + x648 * x957)
        + x230
              * (x108 * (-x766 - x915 - x942) + x228 * x645 * x659
                 + 2 * x632 * x949 + x655 * x958)
        + x633 * x956 + 2 * x644 * x945;
  out_3216172810630303951[105] = x961;
  out_3216172810630303951[106] = x962;
  out_3216172810630303951[107] = x963;
  out_3216172810630303951[108] = x672;
  out_3216172810630303951[109] = x754;
  out_3216172810630303951[110] = x795;
  out_3216172810630303951[111] = x838;
  out_3216172810630303951[112] = x874;
  out_3216172810630303951[113] = x898;
  out_3216172810630303951[114] = x929;
  out_3216172810630303951[115] = x953;
  out_3216172810630303951[116] = x961;
  out_3216172810630303951[117] = x666 * x967 + x669 * x965 + 2 * x671 * x964;
  out_3216172810630303951[118] = x970;
  out_3216172810630303951[119] = x971;
  out_3216172810630303951[120] = x679;
  out_3216172810630303951[121] = x755;
  out_3216172810630303951[122] = x796;
  out_3216172810630303951[123] = x839;
  out_3216172810630303951[124] = x875;
  out_3216172810630303951[125] = x899;
  out_3216172810630303951[126] = x930;
  out_3216172810630303951[127] = x954;
  out_3216172810630303951[128] = x962;
  out_3216172810630303951[129] = x970;
  out_3216172810630303951[130] = x134 * x974 + 2 * x676 * x972 + x969 * x973;
  out_3216172810630303951[131] = x976;
  out_3216172810630303951[132] = x688;
  out_3216172810630303951[133] = x756;
  out_3216172810630303951[134] = x797;
  out_3216172810630303951[135] = x840;
  out_3216172810630303951[136] = x876;
  out_3216172810630303951[137] = x900;
  out_3216172810630303951[138] = x931;
  out_3216172810630303951[139] = x955;
  out_3216172810630303951[140] = x963;
  out_3216172810630303951[141] = x971;
  out_3216172810630303951[142] = x976;
  out_3216172810630303951[143]
      = x143 * x682 * x686 + x683 * x684 * x966 + 2 * x685 * x975;
}
float vertex_face_value(float a_0, float a_1, float a_2, float b_0, float b_1,
                        float b_2, float c_0, float c_1, float c_2, float d_0,
                        float d_1, float d_2, float d_h) {
  const float x0 = a_0 - b_0;
  const float x1 = a_1 - c_1;
  const float x2 = a_0 - c_0;
  const float x3 = a_1 - b_1;
  const float x4 = x0 * x1 - x2 * x3;
  const float x5 = a_2 - c_2;
  const float x6 = a_2 - b_2;
  const float x7 = x0 * x5 - x2 * x6;
  const float x8 = -x1 * x6 + x3 * x5;
  const float x9 = ((x4) * (x4)) + ((x7) * (x7)) + ((x8) * (x8));
  const float x10 = x4 * (a_2 - d_2) - x7 * (a_1 - d_1) + x8 * (a_0 - d_0);
  const float x11 = a_0 * b_1;
  const float x12 = a_0 * b_2;
  const float x13 = c_1 * d_2;
  const float x14 = a_1 * b_0;
  const float x15 = a_1 * b_2;
  const float x16 = c_2 * d_0;
  const float x17 = a_2 * b_0;
  const float x18 = a_2 * b_1;
  const float x19 = c_0 * d_1;
  const float x20 = c_2 * d_1;
  const float x21 = c_0 * d_2;
  const float x22 = c_1 * d_0;
  const float x23 = 2 * x11;
  const float x24 = 2 * a_0;
  const float x25 = c_1 * x14;
  const float x26 = 2 * a_1;
  const float x27 = c_0 * x24;
  const float x28 = 2 * x12;
  const float x29 = c_2 * x17;
  const float x30 = 2 * a_2;
  const float x31 = a_2 * c_2;
  const float x32 = c_1 * x23;
  const float x33 = c_2 * x28;
  const float x34 = 2 * x15;
  const float x35 = c_2 * x18;
  const float x36 = c_1 * x26;
  const float x37 = 2 * c_0;
  const float x38 = c_2 * x34;
  const float x39 = 2 * c_1;
  const float x40 = b_1 * c_1;
  const float x41 = b_0 * c_0;
  const float x42 = 2 * x41;
  const float x43 = b_2 * c_2;
  const float x44 = ((a_0) * (a_0));
  const float x45 = ((b_1) * (b_1));
  const float x46 = ((b_2) * (b_2));
  const float x47 = ((c_1) * (c_1));
  const float x48 = ((c_2) * (c_2));
  const float x49 = ((a_1) * (a_1));
  const float x50 = ((b_0) * (b_0));
  const float x51 = ((c_0) * (c_0));
  const float x52 = ((a_2) * (a_2));
  const float x53 = b_0 * x24;
  const float x54 = 2 * x44;
  const float x55 = b_1 * x26;
  const float x56 = 2 * x49;
  const float x57 = c_2 * x30;
  const float x58 = b_2 * x30;
  const float x59 = 2 * x52;

  float vertex_face_value_result;
  vertex_face_value_result
      = (-2 * log(d_h)
         + log(((a_0 * x13 - a_0 * x20 + a_1 * x16 - a_1 * x21 + a_2 * x19
                 - a_2 * x22 - b_0 * x13 + b_0 * x20 - b_1 * x16 + b_1 * x21
                 - b_2 * x19 + b_2 * x22 + c_0 * x15 - c_0 * x18 - c_1 * x12
                 + c_1 * x17 + c_2 * x11 - c_2 * x14 - d_0 * x15 + d_0 * x18
                 + d_1 * x12 - d_1 * x17 - d_2 * x11 + d_2 * x14)
                * (a_0 * x13 - a_0 * x20 + a_1 * x16 - a_1 * x21 + a_2 * x19
                   - a_2 * x22 - b_0 * x13 + b_0 * x20 - b_1 * x16 + b_1 * x21
                   - b_2 * x19 + b_2 * x22 + c_0 * x15 - c_0 * x18 - c_1 * x12
                   + c_1 * x17 + c_2 * x11 - c_2 * x14 - d_0 * x15 + d_0 * x18
                   + d_1 * x12 - d_1 * x17 - d_2 * x11 + d_2 * x14))
               / (-a_1 * c_1 * x27 + b_0 * x32 + b_0 * x33 + b_1 * x14 * x37
                  + b_1 * x38 + b_2 * x17 * x37 + b_2 * x18 * x39
                  + c_0 * x11 * x26 + c_0 * x12 * x30 + c_0 * x32 + c_0 * x33
                  + c_1 * x15 * x30 + c_1 * x38 - x14 * x23 - x17 * x28
                  - x18 * x34 + x24 * x25 + x24 * x29 + x25 * x37 + x26 * x35
                  - x27 * x31 - x27 * x45 - x27 * x46 + x29 * x37 - x31 * x36
                  + x35 * x39 - x36 * x46 - x36 * x50 - x40 * x42
                  - 2 * x40 * x43 - x40 * x54 - x40 * x59 - x41 * x56
                  - x41 * x59 - x42 * x43 - x43 * x54 - x43 * x56 + x44 * x45
                  + x44 * x46 + x44 * x47 + x44 * x48 + x45 * x48 + x45 * x51
                  + x45 * x52 - x45 * x57 + x46 * x47 + x46 * x49 + x46 * x51
                  + x47 * x50 + x47 * x52 - x47 * x53 - x47 * x58 + x48 * x49
                  + x48 * x50 - x48 * x53 - x48 * x55 + x49 * x50 + x49 * x51
                  + x50 * x52 - x50 * x57 + x51 * x52 - x51 * x55 - x51 * x58)))
        * ((1.0 / 2.0) * ((d_h) * (d_h)) + d_h * x10 / sqrt(x9)
           + (1.0 / 2.0) * ((x10) * (x10)) / x9);
  return vertex_face_value_result;
}

void vertex_face_grad(float a_0, float a_1, float a_2, float b_0, float b_1,
                      float b_2, float c_0, float c_1, float c_2, float d_0,
                      float d_1, float d_2, float d_h,
                      float *out_6543693310212660619) {
  const float x0 = a_0 * b_1;
  const float x1 = a_0 * b_2;
  const float x2 = c_1 * d_2;
  const float x3 = a_1 * b_0;
  const float x4 = a_1 * b_2;
  const float x5 = c_2 * d_0;
  const float x6 = a_2 * b_0;
  const float x7 = a_2 * b_1;
  const float x8 = c_0 * d_1;
  const float x9 = c_2 * d_1;
  const float x10 = c_0 * d_2;
  const float x11 = c_1 * d_0;
  const float x12 = a_0 * x2 - a_0 * x9 - a_1 * x10 + a_1 * x5 - a_2 * x11
                    + a_2 * x8 - b_0 * x2 + b_0 * x9 + b_1 * x10 - b_1 * x5
                    + b_2 * x11 - b_2 * x8 + c_0 * x4 - c_0 * x7 - c_1 * x1
                    + c_1 * x6 + c_2 * x0 - c_2 * x3 - d_0 * x4 + d_0 * x7
                    + d_1 * x1 - d_1 * x6 - d_2 * x0 + d_2 * x3;
  const float x13 = ((x12) * (x12));
  const float x14 = 2 * b_1;
  const float x15 = a_0 * x14;
  const float x16 = 2 * c_1;
  const float x17 = a_0 * x16;
  const float x18 = c_0 * x14;
  const float x19 = a_1 * x18;
  const float x20 = c_0 * x17;
  const float x21 = 2 * b_2;
  const float x22 = a_0 * x21;
  const float x23 = 2 * c_2;
  const float x24 = a_0 * x23;
  const float x25 = c_0 * x21;
  const float x26 = a_2 * x25;
  const float x27 = c_0 * x24;
  const float x28 = b_0 * x16;
  const float x29 = b_0 * x23;
  const float x30 = c_0 * x16;
  const float x31 = c_0 * x23;
  const float x32 = a_2 * x14;
  const float x33 = c_2 * x14;
  const float x34 = a_2 * x33;
  const float x35 = a_2 * x16;
  const float x36 = c_2 * x35;
  const float x37 = x16 * x3;
  const float x38 = x16 * x4;
  const float x39 = x23 * x6;
  const float x40 = b_2 * x16;
  const float x41 = c_2 * x16;
  const float x42 = b_1 * x28;
  const float x43 = b_2 * x29;
  const float x44 = c_2 * x40;
  const float x45 = ((a_0) * (a_0));
  const float x46 = ((b_1) * (b_1));
  const float x47 = ((b_2) * (b_2));
  const float x48 = ((c_1) * (c_1));
  const float x49 = ((c_2) * (c_2));
  const float x50 = ((a_1) * (a_1));
  const float x51 = ((b_0) * (b_0));
  const float x52 = ((c_0) * (c_0));
  const float x53 = ((a_2) * (a_2));
  const float x54 = 2 * b_0;
  const float x55 = x48 * x54;
  const float x56 = x49 * x54;
  const float x57 = 2 * c_0;
  const float x58 = x46 * x57;
  const float x59 = x47 * x57;
  const float x60 = x16 * x45;
  const float x61 = x23 * x45;
  const float x62 = x16 * x51;
  const float x63 = x14 * x52;
  const float x64 = x14 * x49;
  const float x65 = x16 * x47;
  const float x66 = x50 * x54;
  const float x67 = x23 * x50;
  const float x68 = x23 * x51;
  const float x69 = x23 * x46;
  const float x70 = x21 * x52;
  const float x71 = x21 * x48;
  const float x72 = x53 * x54;
  const float x73 = x16 * x53;
  const float x74
      = a_0 * x19 + a_0 * x26 - a_0 * x55 - a_0 * x56 - a_0 * x58 - a_0 * x59
        - a_1 * x20 + a_1 * x34 - a_1 * x36 - a_1 * x62 - a_1 * x63 - a_1 * x64
        - a_1 * x65 - a_2 * x27 - a_2 * x68 - a_2 * x69 - a_2 * x70 - a_2 * x71
        - b_1 * x44 - b_1 * x60 - b_1 * x73 - b_2 * x61 - b_2 * x67 + c_0 * x37
        + c_0 * x39 - c_0 * x42 - c_0 * x43 - c_0 * x66 - c_0 * x72 + c_2 * x38
        + x0 * x28 + x0 * x30 + x1 * x29 + x1 * x31 - x15 * x3 + x17 * x3
        + x18 * x3 - x22 * x6 + x24 * x6 + x25 * x6 - x32 * x4 + x33 * x4
        + x35 * x4 + x40 * x7 + x41 * x7 + x45 * x46 + x45 * x47 + x45 * x48
        + x45 * x49 + x46 * x49 + x46 * x52 + x46 * x53 + x47 * x48 + x47 * x50
        + x47 * x52 + x48 * x51 + x48 * x53 + x49 * x50 + x49 * x51 + x50 * x51
        + x50 * x52 + x51 * x53 + x52 * x53;
  const float x75 = 1.0 / x74;
  const float x76 = -2 * log(d_h) + log(x13 * x75);
  const float x77 = -c_1;
  const float x78 = a_1 + x77;
  const float x79 = a_2 - b_2;
  const float x80 = x78 * x79;
  const float x81 = a_1 - d_1;
  const float x82 = -c_2;
  const float x83 = b_2 + x82;
  const float x84 = a_2 - d_2;
  const float x85 = -x84;
  const float x86 = b_1 + x77;
  const float x87 = a_1 - b_1;
  const float x88 = a_2 + x82;
  const float x89 = x87 * x88;
  const float x90 = a_0 - b_0;
  const float x91 = x78 * x90;
  const float x92 = -c_0;
  const float x93 = a_0 + x92;
  const float x94 = x87 * x93;
  const float x95 = x91 - x94;
  const float x96 = x88 * x90;
  const float x97 = x79 * x93;
  const float x98 = x96 - x97;
  const float x99 = -x80 + x89;
  const float x100 = ((x95) * (x95)) + ((x98) * (x98)) + ((x99) * (x99));
  const float x101 = d_h / sqrt(x100);
  const float x102 = 2 * x84;
  const float x103 = -x81;
  const float x104 = 2 * x83;
  const float x105 = -2 * x80 + 2 * x89;
  const float x106 = 1.0 / x100;
  const float x107 = a_0 - d_0;
  const float x108 = x107 * x99 - x81 * x98 + x84 * x95;
  const float x109 = -x16;
  const float x110 = x109 + x14;
  const float x111 = x110 * x95;
  const float x112 = -x23;
  const float x113 = x112 + x21;
  const float x114 = x113 * x98;
  const float x115 = -x108;
  const float x116 = d_h * x115 / pow(x100, 3.0 / 2.0);
  const float x117 = pow(x100, -2);
  const float x118 = ((x108) * (x108));
  const float x119 = d_2 * x14;
  const float x120 = d_1 * x21;
  const float x121 = -d_1 * x23 + d_2 * x16;
  const float x122 = x33 - x40;
  const float x123 = x12 * x75;
  const float x124 = 2 * a_0;
  const float x125 = x124 * x46;
  const float x126 = x124 * x47;
  const float x127 = x124 * x48;
  const float x128 = x124 * x49;
  const float x129 = x14 * x3;
  const float x130 = a_1 * x30;
  const float x131 = x21 * x6;
  const float x132 = a_2 * x31;
  const float x133 = b_1 * x30;
  const float x134 = b_2 * x31;
  const float x135 = x19 + x26;
  const float x136 = x37 + x39;
  const float x137 = x13 / ((x74) * (x74));
  const float x138
      = (1.0 / 2.0) * ((d_h) * (d_h)) - x101 * x115 + (1.0 / 2.0) * x106 * x118;
  const float x139 = x138 * x74 / x13;
  const float x140 = -x107;
  const float x141 = -b_0 - x92;
  const float x142 = 2 * x96;
  const float x143 = 2 * x97;
  const float x144 = -x57;
  const float x145 = -x144 - x54;
  const float x146 = x145 * x95;
  const float x147 = x113 * x99;
  const float x148 = d_2 * x54;
  const float x149 = d_0 * x21;
  const float x150 = -d_0 * x23 + d_2 * x57;
  const float x151 = -x25 + x29;
  const float x152 = 2 * a_1;
  const float x153 = x152 * x51;
  const float x154 = x152 * x47;
  const float x155 = x152 * x52;
  const float x156 = x152 * x49;
  const float x157 = b_0 * x15;
  const float x158 = b_2 * x32;
  const float x159 = b_0 * x18;
  const float x160 = c_0 * x28;
  const float x161 = b_2 * x33;
  const float x162 = a_0 * x28 + a_2 * x40;
  const float x163 = a_0 * x18 + x34;
  const float x164 = -x86;
  const float x165 = 2 * x107;
  const float x166 = 2 * x103;
  const float x167 = 2 * x91 - 2 * x94;
  const float x168 = x145 * x98;
  const float x169 = -x110 * x99;
  const float x170 = d_1 * x54;
  const float x171 = d_0 * x14;
  const float x172 = -d_0 * x16 + d_1 * x57;
  const float x173 = -x18 + x28;
  const float x174 = 2 * a_2;
  const float x175 = x174 * x51;
  const float x176 = x174 * x46;
  const float x177 = x174 * x52;
  const float x178 = x174 * x48;
  const float x179 = b_0 * x22;
  const float x180 = x14 * x4;
  const float x181 = a_1 * x41;
  const float x182 = b_0 * x25;
  const float x183 = c_0 * x29;
  const float x184 = b_1 * x40;
  const float x185 = b_1 * x41;
  const float x186 = a_0 * x29 + a_1 * x33;
  const float x187 = a_0 * x25 + x38;
  const float x188 = -x78;
  const float x189 = -x88;
  const float x190 = x109 + x152;
  const float x191 = -x190 * x95;
  const float x192 = -x112 - x174;
  const float x193 = x192 * x98;
  const float x194 = a_1 * x23;
  const float x195 = d_1 * x174 - d_2 * x152;
  const float x196 = x50 * x57;
  const float x197 = x53 * x57;
  const float x198 = a_1 * x15;
  const float x199 = a_2 * x22;
  const float x200 = a_1 * x17;
  const float x201 = a_2 * x24;
  const float x202 = x0 * x16 + x1 * x23;
  const float x203 = x124 + x144;
  const float x204 = x203 * x95;
  const float x205 = x192 * x99;
  const float x206 = a_2 * x57;
  const float x207 = d_0 * x174 - d_2 * x124;
  const float x208 = x14 * x45;
  const float x209 = x14 * x53;
  const float x210 = a_1 * x54;
  const float x211 = a_0 * x210;
  const float x212 = a_1 * x21;
  const float x213 = a_2 * x212;
  const float x214 = a_1 * x57;
  const float x215 = a_0 * x214;
  const float x216 = a_2 * x194;
  const float x217 = c_0 * x210 + x23 * x4;
  const float x218 = x203 * x98;
  const float x219 = x190 * x99;
  const float x220 = d_0 * x152 - d_1 * x124;
  const float x221 = x21 * x45;
  const float x222 = x21 * x50;
  const float x223 = a_2 * x54;
  const float x224 = a_0 * x223;
  const float x225 = a_1 * x32;
  const float x226 = a_0 * x206;
  const float x227 = a_1 * x35;
  const float x228 = c_0 * x223 + x16 * x7;
  const float x229 = -x14 + x152;
  const float x230 = x229 * x95;
  const float x231 = x174 - x21;
  const float x232 = x231 * x98;
  const float x233 = x212 - x32;
  const float x234 = -x90;
  const float x235 = -x124 + x54;
  const float x236 = x235 * x95;
  const float x237 = x231 * x99;
  const float x238 = x22 - x223;
  const float x239 = -x87;
  const float x240 = x235 * x98;
  const float x241 = -x229 * x99;
  const float x242 = x15 - x210;
  const float x243 = x138 / x12;

  out_6543693310212660619[0]
      = x139
            * (x123 * (-x119 + x120 + x121 + x122)
               + x137
                     * (4 * a_0 * b_1 * c_1 + 4 * a_0 * b_2 * c_2 - x125 - x126
                        - x127 - x128 + x129 + x130 + x131 + x132 - x133 - x134
                        - x135 - x136 - x42 - x43 + x55 + x56 + x58 + x59))
        + x76
              * (-x101 * (x80 + x81 * x83 + x85 * x86 - x89)
                 + (1.0 / 2.0) * x106 * x108 * (x102 * x86 + x103 * x104 + x105)
                 - x116 * (-1.0 / 2.0 * x111 - 1.0 / 2.0 * x114)
                 + (1.0 / 2.0) * x117 * x118 * (-x111 - x114));
  out_6543693310212660619[1]
      = x139
            * (x123 * (x148 - x149 - x150 - x151)
               + x137
                     * (4 * a_1 * b_0 * c_0 + 4 * a_1 * b_2 * c_2 - x153 - x154
                        - x155 - x156 + x157 + x158 - x159 - x160 - x161 - x162
                        - x163 + x20 + x36 - x44 + x62 + x63 + x64 + x65))
        + x76
              * (-x101 * (x140 * x83 + x141 * x85 + x98)
                 + (1.0 / 2.0) * x106 * x108
                       * (x102 * x141 + x104 * x107 - x142 + x143)
                 - x116 * (-1.0 / 2.0 * x146 - 1.0 / 2.0 * x147)
                 + (1.0 / 2.0) * x117 * x118 * (-x146 - x147));
  out_6543693310212660619[2]
      = x139
            * (x123 * (-x170 + x171 + x172 + x173)
               + x137
                     * (4 * a_2 * b_0 * c_0 + 4 * a_2 * b_1 * c_1 - x175 - x176
                        - x177 - x178 + x179 + x180 + x181 - x182 - x183 - x184
                        - x185 - x186 - x187 + x27 + x68 + x69 + x70 + x71))
        + x76
              * (-x101 * (x140 * x164 + x141 * x81 - x91 + x94)
                 + (1.0 / 2.0) * x106 * x108
                       * (x141 * x166 + x164 * x165 + x167)
                 - x116 * (-1.0 / 2.0 * x168 - 1.0 / 2.0 * x169)
                 + (1.0 / 2.0) * x117 * x118 * (-x168 - x169));
  out_6543693310212660619[3]
      = x139
            * (x123 * (-x121 - x194 - x195 + x35)
               + x137
                     * (4 * a_1 * b_0 * c_1 + 4 * a_2 * b_0 * c_2 + x127 + x128
                        - x130 - x132 + x133 + x134 - x135 + x196 + x197 + x198
                        + x199 - x200 - x201 - x202 - x55 - x56 - x66 - x72))
        + x76
              * (-x101 * (x188 * x85 + x189 * x81)
                 + (1.0 / 2.0) * x106 * x108 * (x102 * x188 + x166 * x189)
                 - x116 * (-1.0 / 2.0 * x191 - 1.0 / 2.0 * x193)
                 + (1.0 / 2.0) * x117 * x118 * (-x191 - x193));
  out_6543693310212660619[4]
      = x139
            * (x123 * (x150 - x206 + x207 + x24)
               + x137
                     * (4 * a_0 * b_1 * c_0 + 4 * a_2 * b_1 * c_2 + x155 + x156
                        + x160 - x162 - x20 - x208 - x209 + x211 + x213 - x215
                        - x216 - x217 - x36 + x44 + x60 - x63 - x64 + x73))
        + x76
              * (-x101 * (x140 * x189 + x85 * x93)
                 + (1.0 / 2.0) * x106 * x108 * (x102 * x93 + x165 * x189)
                 - x116 * (-1.0 / 2.0 * x204 - 1.0 / 2.0 * x205)
                 + (1.0 / 2.0) * x117 * x118 * (-x204 - x205));
  out_6543693310212660619[5]
      = x139
            * (x123 * (-x17 - x172 + x214 - x220)
               + x137
                     * (4 * a_0 * b_2 * c_0 + 4 * a_1 * b_2 * c_1 + x177 + x178
                        - x181 + x183 + x185 - x186 - x221 - x222 + x224 + x225
                        - x226 - x227 - x228 - x27 + x61 + x67 - x70 - x71))
        + x76
              * (-x101 * (x140 * x78 + x81 * x93)
                 + (1.0 / 2.0) * x106 * x108 * (x165 * x78 + x166 * x93)
                 - x116 * (-1.0 / 2.0 * x218 - 1.0 / 2.0 * x219)
                 + (1.0 / 2.0) * x117 * x118 * (-x218 - x219));
  out_6543693310212660619[6]
      = x139
            * (x123 * (x119 - x120 + x195 + x233)
               + x137
                     * (4 * a_1 * b_1 * c_0 + 4 * a_2 * b_2 * c_0 + x125 + x126
                        - x129 - x131 - x136 - x196 - x197 - x198 - x199 + x200
                        + x201 - x202 + x42 + x43 - x58 - x59 + x66 + x72))
        + x76
              * (-x101 * (x79 * x81 + x85 * x87)
                 + (1.0 / 2.0) * x106 * x108 * (x102 * x87 + x166 * x79)
                 - x116 * (-1.0 / 2.0 * x230 - 1.0 / 2.0 * x232)
                 + (1.0 / 2.0) * x117 * x118 * (-x230 - x232));
  out_6543693310212660619[7]
      = x139
            * (x123 * (-x148 + x149 - x207 - x238)
               + x137
                     * (4 * a_0 * b_0 * c_1 + 4 * a_2 * b_2 * c_1 + x153 + x154
                        - x157 - x158 + x159 + x161 - x163 + x208 + x209 - x211
                        - x213 + x215 + x216 - x217 - x60 - x62 - x65 - x73))
        + x76
              * (-x101 * (x140 * x79 + x234 * x85)
                 + (1.0 / 2.0) * x106 * x108 * (x102 * x234 + x165 * x79)
                 - x116 * (-1.0 / 2.0 * x236 - 1.0 / 2.0 * x237)
                 + (1.0 / 2.0) * x117 * x118 * (-x236 - x237));
  out_6543693310212660619[8]
      = x139
            * (x123 * (x170 - x171 + x220 + x242)
               + x137
                     * (4 * a_0 * b_0 * c_2 + 4 * a_1 * b_1 * c_2 + x175 + x176
                        - x179 - x180 + x182 + x184 - x187 + x221 + x222 - x224
                        - x225 + x226 + x227 - x228 - x61 - x67 - x68 - x69))
        + x76
              * (-x101 * (x140 * x239 + x234 * x81)
                 + (1.0 / 2.0) * x106 * x108 * (x165 * x239 + x166 * x234)
                 - x116 * (-1.0 / 2.0 * x240 - 1.0 / 2.0 * x241)
                 + (1.0 / 2.0) * x117 * x118 * (-x240 - x241));
  out_6543693310212660619[9]
      = x243 * (-x122 + x194 - x233 - x35)
        + x76 * (-x101 * x99 - 1.0 / 2.0 * x105 * x106 * x108);
  out_6543693310212660619[10]
      = x243 * (x151 + x206 + x238 - x24)
        + x76 * (x101 * x98 + (1.0 / 2.0) * x106 * x108 * (x142 - x143));
  out_6543693310212660619[11]
      = x243 * (x17 - x173 - x214 - x242)
        + x76 * (-x101 * x95 - 1.0 / 2.0 * x106 * x108 * x167);
}

void vertex_face_hessian(float a_0, float a_1, float a_2, float b_0, float b_1,
                         float b_2, float c_0, float c_1, float c_2, float d_0,
                         float d_1, float d_2, float d_h,
                         float *out_3216172810630303951) {
  const float x0 = ((c_1) * (c_1));
  const float x1 = 2 * x0;
  const float x2 = b_0 * x1;
  const float x3 = ((c_2) * (c_2));
  const float x4 = 2 * x3;
  const float x5 = b_0 * x4;
  const float x6 = ((b_1) * (b_1));
  const float x7 = 2 * x6;
  const float x8 = c_0 * x7;
  const float x9 = ((b_2) * (b_2));
  const float x10 = 2 * x9;
  const float x11 = c_0 * x10;
  const float x12 = a_0 * x7;
  const float x13 = a_0 * x10;
  const float x14 = a_0 * x1;
  const float x15 = a_0 * x4;
  const float x16 = a_0 * c_1;
  const float x17 = 4 * b_1;
  const float x18 = x16 * x17;
  const float x19 = a_0 * c_2;
  const float x20 = 4 * b_2;
  const float x21 = x19 * x20;
  const float x22 = 2 * b_1;
  const float x23 = a_1 * b_0;
  const float x24 = x22 * x23;
  const float x25 = 2 * c_1;
  const float x26 = a_1 * c_0;
  const float x27 = x25 * x26;
  const float x28 = 2 * b_2;
  const float x29 = a_2 * b_0;
  const float x30 = x28 * x29;
  const float x31 = 2 * c_2;
  const float x32 = a_2 * c_0;
  const float x33 = x31 * x32;
  const float x34 = b_0 * c_1;
  const float x35 = x22 * x34;
  const float x36 = b_0 * c_2;
  const float x37 = x28 * x36;
  const float x38 = c_0 * x22;
  const float x39 = c_1 * x38;
  const float x40 = c_0 * x28;
  const float x41 = c_2 * x40;
  const float x42 = x22 * x26 + x28 * x32;
  const float x43 = x23 * x25 + x29 * x31;
  const float x44 = -x11 + x12 + x13 + x14 + x15 - x18 - x2 - x21 - x24 - x27
                    - x30 - x33 + x35 + x37 + x39 + x41 + x42 + x43 - x5 - x8;
  const float x45 = b_1 * c_2;
  const float x46 = b_2 * d_1;
  const float x47 = c_1 * d_2;
  const float x48 = b_0 * d_2;
  const float x49 = b_2 * c_0;
  const float x50 = c_2 * d_0;
  const float x51 = b_1 * d_0;
  const float x52 = c_0 * d_1;
  const float x53 = c_2 * d_1;
  const float x54 = b_1 * d_2;
  const float x55 = b_2 * c_1;
  const float x56 = b_2 * d_0;
  const float x57 = c_0 * d_2;
  const float x58 = b_0 * d_1;
  const float x59 = b_1 * c_0;
  const float x60 = c_1 * d_0;
  const float x61 = a_0 * x45 + a_0 * x46 + a_0 * x47 - a_0 * x53 - a_0 * x54
                    - a_0 * x55 - a_1 * x36 + a_1 * x48 + a_1 * x49 + a_1 * x50
                    - a_1 * x56 - a_1 * x57 + a_2 * x34 + a_2 * x51 + a_2 * x52
                    - a_2 * x58 - a_2 * x59 - a_2 * x60 - b_0 * x47 + b_0 * x53
                    - c_0 * x46 + c_0 * x54 - d_0 * x45 + d_0 * x55;
  const float x62 = a_0 * x22;
  const float x63 = a_0 * x25;
  const float x64 = a_0 * x28;
  const float x65 = a_0 * x31;
  const float x66 = a_1 * b_2;
  const float x67 = a_2 * x22;
  const float x68 = a_1 * c_2;
  const float x69 = a_2 * x25;
  const float x70 = b_0 * x22;
  const float x71 = b_0 * x25;
  const float x72 = b_2 * x22;
  const float x73 = b_2 * x25;
  const float x74 = b_0 * x28;
  const float x75 = b_0 * x31;
  const float x76 = a_2 * c_1;
  const float x77 = c_2 * x22;
  const float x78 = ((a_0) * (a_0));
  const float x79 = ((a_1) * (a_1));
  const float x80 = ((b_0) * (b_0));
  const float x81 = ((c_0) * (c_0));
  const float x82 = ((a_2) * (a_2));
  const float x83 = c_1 * x22;
  const float x84 = c_2 * x28;
  const float x85 = x25 * x80;
  const float x86 = x22 * x81;
  const float x87 = x22 * x3;
  const float x88 = x25 * x9;
  const float x89 = 2 * b_0;
  const float x90 = x79 * x89;
  const float x91 = x31 * x80;
  const float x92 = x31 * x6;
  const float x93 = x28 * x81;
  const float x94 = x0 * x28;
  const float x95 = x82 * x89;
  const float x96
      = -a_1 * x85 - a_1 * x86 - a_1 * x87 - a_1 * x88 - a_2 * x91 - a_2 * x92
        - a_2 * x93 - a_2 * x94 - b_0 * x14 - b_0 * x15 - c_0 * x12 - c_0 * x13
        - c_0 * x90 - c_0 * x95 + x0 * x78 + x0 * x80 + x0 * x82 + x0 * x9
        + x16 * x38 + x19 * x40 - x23 * x62 + x23 * x63 + x26 * x62 - x26 * x63
        + x26 * x70 + x26 * x71 - x29 * x64 + x29 * x65 + x3 * x6 + x3 * x78
        + x3 * x79 + x3 * x80 + x32 * x64 - x32 * x65 + x32 * x74 + x32 * x75
        - x34 * x38 + x34 * x62 - x36 * x40 + x36 * x64 + x55 * x67 - x55 * x77
        + x6 * x78 + x6 * x81 + x6 * x82 - x66 * x67 + x66 * x69 + x67 * x68
        - x68 * x69 + x68 * x72 + x68 * x73 + x76 * x77 - x78 * x83 - x78 * x84
        + x78 * x9 + x79 * x80 + x79 * x81 - x79 * x84 + x79 * x9 + x80 * x82
        + x81 * x82 + x81 * x9 - x82 * x83;
  const float x97 = 1.0 / x96;
  const float x98 = d_2 * x22;
  const float x99 = d_1 * x28;
  const float x100 = -d_1 * x31 + d_2 * x25;
  const float x101 = -x73;
  const float x102 = x101 + x77;
  const float x103 = x100 + x102 - x98 + x99;
  const float x104 = x103 * x97;
  const float x105 = -x44;
  const float x106 = ((x61) * (x61));
  const float x107 = pow(x96, -2);
  const float x108 = x106 * x107;
  const float x109 = x104 * x61 + x105 * x108;
  const float x110 = 1.0 / x106;
  const float x111 = -d_0;
  const float x112 = a_0 + x111;
  const float x113 = a_1 - b_1;
  const float x114 = -c_2;
  const float x115 = a_2 + x114;
  const float x116 = x113 * x115;
  const float x117 = -c_1;
  const float x118 = a_1 + x117;
  const float x119 = a_2 - b_2;
  const float x120 = x118 * x119;
  const float x121 = x116 - x120;
  const float x122 = -d_2;
  const float x123 = a_2 + x122;
  const float x124 = a_0 - b_0;
  const float x125 = x118 * x124;
  const float x126 = -c_0;
  const float x127 = a_0 + x126;
  const float x128 = x113 * x127;
  const float x129 = x125 - x128;
  const float x130 = -d_1;
  const float x131 = a_1 + x130;
  const float x132 = x115 * x124;
  const float x133 = x119 * x127;
  const float x134 = x132 - x133;
  const float x135 = x112 * x121 + x123 * x129 - x131 * x134;
  const float x136 = ((x135) * (x135));
  const float x137 = ((x121) * (x121)) + ((x129) * (x129)) + ((x134) * (x134));
  const float x138 = 1.0 / x137;
  const float x139 = (1.0 / 2.0) * x138;
  const float x140 = -x135;
  const float x141 = d_h / sqrt(x137);
  const float x142 = (1.0 / 2.0) * ((d_h) * (d_h)) + x136 * x139 - x140 * x141;
  const float x143 = x110 * x142;
  const float x144 = x109 * x143;
  const float x145 = x142 * x96 / pow(x61, 3);
  const float x146 = x109 * x145;
  const float x147 = -2 * log(d_h) + log(x106 * x97);
  const float x148 = b_1 + x117;
  const float x149 = x123 * x148;
  const float x150 = -x131;
  const float x151 = b_2 + x114;
  const float x152 = x150 * x151;
  const float x153 = 2 * x116;
  const float x154 = 2 * x120;
  const float x155 = x153 - x154;
  const float x156 = 2 * x149 + 2 * x152 + x155;
  const float x157 = x139 * x156;
  const float x158 = -x25;
  const float x159 = x158 + x22;
  const float x160 = x129 * x159;
  const float x161 = -x31;
  const float x162 = x161 + x28;
  const float x163 = x134 * x162;
  const float x164 = -1.0 / 2.0 * x160 - 1.0 / 2.0 * x163;
  const float x165 = -x123;
  const float x166 = -x116 + x120;
  const float x167 = x131 * x151 + x148 * x165 + x166;
  const float x168 = d_h / pow(x137, 3.0 / 2.0);
  const float x169 = x167 * x168;
  const float x170 = -x148;
  const float x171 = x148 * x170;
  const float x172 = -x151;
  const float x173 = x151 * x172;
  const float x174 = x140 * x168;
  const float x175 = -x159;
  const float x176 = -x162;
  const float x177 = x151 * x176;
  const float x178 = pow(x137, -2);
  const float x179 = (1.0 / 2.0) * x136;
  const float x180 = x178 * x179;
  const float x181 = -x160 - x163;
  const float x182 = x135 * x178;
  const float x183 = x156 * x182;
  const float x184 = x179 / pow(x137, 3);
  const float x185 = x181 * x184;
  const float x186 = d_h * x140 / pow(x137, 5.0 / 2.0);
  const float x187 = x164 * x186;
  const float x188 = (1.0 / 2.0) * x135 * x138 * x156
                     + (1.0 / 2.0) * x136 * x178 * x181 - x141 * x167
                     - x164 * x174;
  const float x189 = x110 * x96;
  const float x190 = x109 * x189;
  const float x191 = x47 - x53;
  const float x192 = x45 - x55;
  const float x193 = -4 * b_1 * c_1;
  const float x194 = -4 * b_2 * c_2;
  const float x195 = x1 + x4;
  const float x196 = x10 + x7;
  const float x197 = x107 * x61;
  const float x198 = x105 * x197;
  const float x199 = 4 * b_0;
  const float x200 = x0 * x199;
  const float x201 = x199 * x3;
  const float x202 = 4 * c_0;
  const float x203 = x202 * x6;
  const float x204 = x202 * x9;
  const float x205 = 4 * a_0;
  const float x206 = x205 * x6;
  const float x207 = x205 * x9;
  const float x208 = x0 * x205;
  const float x209 = x205 * x3;
  const float x210 = x17 * x23;
  const float x211 = 4 * c_1;
  const float x212 = x211 * x26;
  const float x213 = x20 * x29;
  const float x214 = 4 * c_2;
  const float x215 = x214 * x32;
  const float x216 = x17 * x34;
  const float x217 = x20 * x36;
  const float x218 = x211 * x59;
  const float x219 = x214 * x49;
  const float x220 = 4 * a_1;
  const float x221 = x220 * x59;
  const float x222 = 4 * a_2;
  const float x223 = x222 * x49;
  const float x224 = x221 + x223;
  const float x225 = x220 * x34;
  const float x226 = x222 * x36;
  const float x227 = x225 + x226;
  const float x228 = x106 / pow(x96, 3);
  const float x229 = x105 * x228;
  const float x230 = x143 * x96;
  const float x231 = 2 * a_1;
  const float x232 = x231 * x80;
  const float x233 = a_1 * x10;
  const float x234 = x231 * x81;
  const float x235 = a_1 * x4;
  const float x236 = x199 * x26;
  const float x237 = x20 * x68;
  const float x238 = a_0 * x70;
  const float x239 = c_0 * x25;
  const float x240 = a_0 * x239;
  const float x241 = b_2 * x67;
  const float x242 = c_2 * x69;
  const float x243 = b_0 * x38;
  const float x244 = c_0 * x71;
  const float x245 = b_2 * x77;
  const float x246 = c_2 * x73;
  const float x247 = a_0 * x71 + a_2 * x73;
  const float x248 = a_0 * x38 + a_2 * x77;
  const float x249 = x232 + x233 + x234 + x235 - x236 - x237 - x238 - x240
                     - x241 - x242 + x243 + x244 + x245 + x246 + x247 + x248
                     - x85 - x86 - x87 - x88;
  const float x250 = d_2 * x89;
  const float x251 = d_0 * x28;
  const float x252 = 2 * c_0;
  const float x253 = -d_0 * x31 + d_2 * x252;
  const float x254 = -x40;
  const float x255 = x254 + x75;
  const float x256 = -x250 + x251 + x253 + x255;
  const float x257 = -x256;
  const float x258 = x61 * x97;
  const float x259 = -x249;
  const float x260 = x108 * x259 + x257 * x258;
  const float x261 = x188 * x189;
  const float x262 = -x112;
  const float x263 = b_0 + x126;
  const float x264 = -x263;
  const float x265 = x134 + x151 * x262 + x165 * x264;
  const float x266 = x112 * x151;
  const float x267 = x123 * x264;
  const float x268 = 2 * x132;
  const float x269 = 2 * x133;
  const float x270 = -x268 + x269;
  const float x271 = 2 * x266 + 2 * x267 + x270;
  const float x272 = -x252;
  const float x273 = x272 + x89;
  const float x274 = -x273;
  const float x275 = x129 * x274;
  const float x276 = x121 * x162;
  const float x277 = -1.0 / 2.0 * x275 - 1.0 / 2.0 * x276;
  const float x278 = -x275 - x276;
  const float x279 = (1.0 / 2.0) * x135 * x138 * x271
                     + (1.0 / 2.0) * x136 * x178 * x278 - x141 * x265
                     - x174 * x277;
  const float x280 = -x132 + x133;
  const float x281 = x266 + x267 + x280;
  const float x282 = x174 * x264;
  const float x283 = x180 * x264;
  const float x284 = x164 * x168;
  const float x285 = (1.0 / 2.0) * x183;
  const float x286 = x182 * x271;
  const float x287 = (1.0 / 2.0) * x181;
  const float x288 = -2 * x275 - 2 * x276;
  const float x289 = -3.0 / 2.0 * x275 - 3.0 / 2.0 * x276;
  const float x290 = -x50 + x57;
  const float x291 = x36 - x49;
  const float x292 = -x290 - x291 + x48 - x56;
  const float x293 = -x71;
  const float x294 = -x38;
  const float x295 = x103 * x197;
  const float x296 = x17 * x81;
  const float x297 = x17 * x3;
  const float x298 = x211 * x80;
  const float x299 = x211 * x9;
  const float x300 = x220 * x80;
  const float x301 = x220 * x9;
  const float x302 = x220 * x81;
  const float x303 = x220 * x3;
  const float x304 = a_0 * b_1;
  const float x305 = x199 * x304;
  const float x306 = x16 * x202;
  const float x307 = a_2 * b_1;
  const float x308 = x20 * x307;
  const float x309 = x214 * x76;
  const float x310 = x199 * x59;
  const float x311 = x202 * x34;
  const float x312 = x20 * x45;
  const float x313 = x214 * x55;
  const float x314 = x205 * x34;
  const float x315 = x222 * x55;
  const float x316 = x314 + x315;
  const float x317 = x205 * x59;
  const float x318 = x222 * x45;
  const float x319 = x317 + x318;
  const float x320 = 8 * a_1 * b_0 * c_0 + 8 * a_1 * b_2 * c_2 + x296 + x297
                     + x298 + x299 - x300 - x301 - x302 - x303 + x305 + x306
                     + x308 + x309 - x310 - x311 - x312 - x313 - x316 - x319;
  const float x321 = x144 * x249 + x146 * x256
                     + x147
                           * (x157 * x281 - x169 * x277 - x170 * x282
                              + x175 * x283 + x185 * x288 - x187 * x289
                              - x265 * x284 + x278 * x285 + x286 * x287)
                     + x190 * x279
                     + x230
                           * (x104 * x292 + x108 * (x239 + x293 + x294 + x70)
                              + x198 * x257 + x229 * x320 + x259 * x295)
                     + x260 * x261;
  const float x322 = 2 * a_2;
  const float x323 = x322 * x80;
  const float x324 = a_2 * x7;
  const float x325 = x322 * x81;
  const float x326 = a_2 * x1;
  const float x327 = x199 * x32;
  const float x328 = x17 * x76;
  const float x329 = a_0 * x74;
  const float x330 = c_0 * x31;
  const float x331 = a_0 * x330;
  const float x332 = x22 * x66;
  const float x333 = x25 * x68;
  const float x334 = b_0 * x40;
  const float x335 = c_0 * x75;
  const float x336 = x22 * x55;
  const float x337 = c_1 * x77;
  const float x338 = a_0 * x75 + x22 * x68;
  const float x339 = a_0 * x40 + x25 * x66;
  const float x340 = x323 + x324 + x325 + x326 - x327 - x328 - x329 - x331
                     - x332 - x333 + x334 + x335 + x336 + x337 + x338 + x339
                     - x91 - x92 - x93 - x94;
  const float x341 = d_1 * x89;
  const float x342 = d_0 * x22;
  const float x343 = -d_0 * x25 + d_1 * x252;
  const float x344 = x294 + x71;
  const float x345 = -x341 + x342 + x343 + x344;
  const float x346 = -x345;
  const float x347 = -x340;
  const float x348 = x108 * x347 + x258 * x345;
  const float x349 = -x125 + x128;
  const float x350 = x131 * x264 + x170 * x262 + x349;
  const float x351 = x112 * x170;
  const float x352 = x150 * x264;
  const float x353 = 2 * x125;
  const float x354 = 2 * x128;
  const float x355 = x353 - x354;
  const float x356 = 2 * x351 + 2 * x352 + x355;
  const float x357 = x134 * x274;
  const float x358 = x121 * x175;
  const float x359 = -1.0 / 2.0 * x357 - 1.0 / 2.0 * x358;
  const float x360 = -x357 - x358;
  const float x361 = (1.0 / 2.0) * x135 * x138 * x356
                     + (1.0 / 2.0) * x136 * x178 * x360 - x141 * x350
                     - x174 * x359;
  const float x362 = x129 + x351 + x352;
  const float x363 = x182 * x356;
  const float x364 = -2 * x357 - 2 * x358;
  const float x365 = -3.0 / 2.0 * x357 - 3.0 / 2.0 * x358;
  const float x366 = x52 - x60;
  const float x367 = x34 - x59;
  const float x368 = x366 + x367 + x51 - x58;
  const float x369 = -x75;
  const float x370 = x20 * x81;
  const float x371 = x0 * x20;
  const float x372 = x214 * x80;
  const float x373 = x214 * x6;
  const float x374 = x222 * x80;
  const float x375 = x222 * x6;
  const float x376 = x222 * x81;
  const float x377 = x0 * x222;
  const float x378 = a_0 * b_2;
  const float x379 = x199 * x378;
  const float x380 = x19 * x202;
  const float x381 = x17 * x66;
  const float x382 = x211 * x68;
  const float x383 = x199 * x49;
  const float x384 = x202 * x36;
  const float x385 = x17 * x55;
  const float x386 = x211 * x45;
  const float x387 = x205 * x36;
  const float x388 = x220 * x45;
  const float x389 = x387 + x388;
  const float x390 = x205 * x49;
  const float x391 = x220 * x55;
  const float x392 = x390 + x391;
  const float x393 = 8 * a_2 * b_0 * c_0 + 8 * a_2 * b_1 * c_1 + x370 + x371
                     + x372 + x373 - x374 - x375 - x376 - x377 + x379 + x380
                     + x381 + x382 - x383 - x384 - x385 - x386 - x389 - x392;
  const float x394 = x144 * x340 + x146 * x346
                     + x147
                           * (x157 * x362 - x169 * x359 - x172 * x282
                              + x176 * x283 + x185 * x364 - x187 * x365
                              - x284 * x350 + x285 * x360 + x287 * x363)
                     + x190 * x361
                     + x230
                           * (x104 * x368 + x108 * (x254 + x330 + x369 + x74)
                              + x198 * x345 + x229 * x393 + x295 * x347)
                     + x261 * x348;
  const float x395 = x252 * x79;
  const float x396 = x252 * x82;
  const float x397 = a_1 * x22;
  const float x398 = a_0 * x397;
  const float x399 = a_2 * x28;
  const float x400 = a_0 * x399;
  const float x401 = a_1 * x25;
  const float x402 = a_0 * x401;
  const float x403 = a_2 * x31;
  const float x404 = a_0 * x403;
  const float x405 = x16 * x22 + x19 * x28;
  const float x406 = -x14 - x15 + x2 - x225 - x226 + x27 + x33 - x39 - x395
                     - x396 - x398 - x400 + x402 + x404 + x405 - x41 + x42 + x5
                     + x90 + x95;
  const float x407 = -x69;
  const float x408 = a_1 * x31;
  const float x409 = d_1 * x322 - d_2 * x231;
  const float x410 = x100 + x407 + x408 + x409;
  const float x411 = -x118;
  const float x412 = -x115;
  const float x413 = x131 * x412 + x165 * x411;
  const float x414 = x123 * x411;
  const float x415 = x150 * x412;
  const float x416 = 2 * x414 + 2 * x415;
  const float x417 = x158 + x231;
  const float x418 = -x417;
  const float x419 = x129 * x418;
  const float x420 = x161 + x322;
  const float x421 = -x420;
  const float x422 = x134 * x421;
  const float x423 = -1.0 / 2.0 * x419 - 1.0 / 2.0 * x422;
  const float x424 = -x419 - x422;
  const float x425 = (1.0 / 2.0) * x135 * x138 * x416
                     + (1.0 / 2.0) * x136 * x178 * x424 - x141 * x413
                     - x174 * x423;
  const float x426 = -x410;
  const float x427 = -x406;
  const float x428 = x108 * x427 + x258 * x426;
  const float x429 = x414 + x415;
  const float x430 = x172 * x412;
  const float x431 = x176 * x412;
  const float x432 = x182 * x416;
  const float x433 = -2 * x419 - 2 * x422;
  const float x434 = -3.0 / 2.0 * x419 - 3.0 / 2.0 * x422;
  const float x435 = -a_1 * d_2 + a_2 * d_1;
  const float x436 = -x191 - x435 - x68 + x76;
  const float x437 = -x403;
  const float x438 = x399 + x437;
  const float x439 = x438 - x84;
  const float x440 = -x401;
  const float x441 = x397 + x440;
  const float x442 = x441 - x83;
  const float x443 = x202 * x79;
  const float x444 = x202 * x82;
  const float x445 = x199 * x79;
  const float x446 = x199 * x82;
  const float x447 = x220 * x304;
  const float x448 = x222 * x378;
  const float x449 = x16 * x220;
  const float x450 = x19 * x222;
  const float x451 = x18 + x21;
  const float x452 = 8 * a_1 * b_0 * c_1 + 8 * a_2 * b_0 * c_2 - x200 - x201
                     + x208 + x209 - x212 - x215 + x218 + x219 - x224 + x443
                     + x444 - x445 - x446 + x447 + x448 - x449 - x450 - x451;
  const float x453
      = x144 * x406 + x146 * x410
        + x147
              * (x157 * x429 - x169 * x423 - x174 * (x170 * x411 + x430)
                 + x180 * (x175 * x411 + x431) + x185 * x433 - x187 * x434
                 - x284 * x413 + x285 * x424 + x287 * x432)
        + x190 * x425
        + x230
              * (x104 * x436 + x108 * (x195 + x439 + x442) + x198 * x426
                 + x229 * x452 + x295 * x427)
        + x261 * x428;
  const float x454 = x25 * x78;
  const float x455 = x25 * x82;
  const float x456 = x22 * x78;
  const float x457 = x22 * x82;
  const float x458 = a_1 * x89;
  const float x459 = a_0 * x458;
  const float x460 = a_1 * x399;
  const float x461 = a_1 * x252;
  const float x462 = a_0 * x461;
  const float x463 = a_2 * x408;
  const float x464 = x26 * x89 + x28 * x68;
  const float x465 = -x234 - x235 + x240 + x242 - x244 - x246 + x247 - x317
                     - x318 - x454 - x455 + x456 + x457 - x459 - x460 + x462
                     + x463 + x464 + x86 + x87;
  const float x466 = a_2 * x252;
  const float x467 = -x466;
  const float x468 = 2 * d_2;
  const float x469 = -a_0 * x468 + d_0 * x322;
  const float x470 = x253 + x467 + x469 + x65;
  const float x471 = -x470;
  const float x472 = x127 * x165 + x262 * x412;
  const float x473 = x123 * x127;
  const float x474 = x112 * x412;
  const float x475 = 2 * x473 + 2 * x474;
  const float x476 = 2 * a_0;
  const float x477 = x272 + x476;
  const float x478 = x129 * x477;
  const float x479 = x121 * x421;
  const float x480 = -1.0 / 2.0 * x478 - 1.0 / 2.0 * x479;
  const float x481 = -x478 - x479;
  const float x482 = (1.0 / 2.0) * x135 * x138 * x475
                     + (1.0 / 2.0) * x136 * x178 * x481 - x141 * x472
                     - x174 * x480;
  const float x483 = -x465;
  const float x484 = x108 * x483 + x258 * x470;
  const float x485 = c_2 + x122;
  const float x486 = x473 + x474;
  const float x487 = -x468;
  const float x488 = x31 + x487;
  const float x489 = x135 * x139;
  const float x490 = -x353 + x354;
  const float x491 = x182 * x475;
  const float x492 = -2 * x478 - 2 * x479;
  const float x493 = -3.0 / 2.0 * x478 - 3.0 / 2.0 * x479;
  const float x494 = -a_0 * d_2 + a_2 * d_0;
  const float x495 = x19 + x290 - x32 + x494;
  const float x496 = -x458;
  const float x497 = 4 * x304;
  const float x498 = 4 * x59;
  const float x499 = -x498 + x71;
  const float x500 = 4 * x16;
  const float x501 = x461 - x500;
  const float x502 = x211 * x78;
  const float x503 = x211 * x82;
  const float x504 = x17 * x78;
  const float x505 = x17 * x82;
  const float x506 = x205 * x23;
  const float x507 = x222 * x66;
  const float x508 = x205 * x26;
  const float x509 = x222 * x68;
  const float x510 = x236 + x237;
  const float x511 = 8 * a_0 * b_1 * c_0 + 8 * a_2 * b_1 * c_2 - x296 - x297
                     + x302 + x303 - x306 - x309 + x311 + x313 - x316 + x502
                     + x503 - x504 - x505 + x506 + x507 - x508 - x509 - x510;
  const float x512
      = x144 * x465 + x146 * x471
        + x147
              * (x141 * x485 + x157 * x486 - x169 * x480
                 - x174 * (x127 * x170 + x349) + x180 * (x127 * x175 + x490)
                 + x185 * x492 - x187 * x493 - x284 * x472 + x285 * x481
                 + x287 * x491 + x488 * x489)
        + x190 * x482
        + x230
              * (x104 * x495 + x108 * (-x239 - x496 - x497 - x499 - x501)
                 + x198 * x470 + x229 * x511 + x258 * x488 + x295 * x483)
        + x261 * x484;
  const float x513 = x31 * x78;
  const float x514 = x31 * x79;
  const float x515 = x28 * x78;
  const float x516 = x28 * x79;
  const float x517 = a_2 * x89;
  const float x518 = a_0 * x517;
  const float x519 = a_2 * x397;
  const float x520 = a_0 * x466;
  const float x521 = a_1 * x69;
  const float x522 = x22 * x76 + x32 * x89;
  const float x523 = -x325 - x326 + x331 + x333 - x335 - x337 + x338 - x390
                     - x391 - x513 - x514 + x515 + x516 - x518 - x519 + x520
                     + x521 + x522 + x93 + x94;
  const float x524 = -x461;
  const float x525 = d_0 * x231 - d_1 * x476;
  const float x526 = x343 + x524 + x525 + x63;
  const float x527 = x118 * x262 + x127 * x131;
  const float x528 = x127 * x150;
  const float x529 = x112 * x118;
  const float x530 = 2 * x528 + 2 * x529;
  const float x531 = x134 * x477;
  const float x532 = x121 * x417;
  const float x533 = -1.0 / 2.0 * x531 - 1.0 / 2.0 * x532;
  const float x534 = -x531 - x532;
  const float x535 = (1.0 / 2.0) * x135 * x138 * x530
                     + (1.0 / 2.0) * x136 * x178 * x534 - x141 * x527
                     - x174 * x533;
  const float x536 = -x526;
  const float x537 = -x523;
  const float x538 = x108 * x537 + x258 * x536;
  const float x539 = c_1 + x130;
  const float x540 = x528 + x529;
  const float x541 = -2 * d_1;
  const float x542 = x25 + x541;
  const float x543 = -x542;
  const float x544 = x182 * x530;
  const float x545 = -2 * x531 - 2 * x532;
  const float x546 = -3.0 / 2.0 * x531 - 3.0 / 2.0 * x532;
  const float x547 = -a_0 * d_1 + a_1 * d_0;
  const float x548 = -x16 + x26 - x366 - x547;
  const float x549 = -x517;
  const float x550 = 4 * x378;
  const float x551 = 4 * x49;
  const float x552 = -x551 + x75;
  const float x553 = 4 * x19;
  const float x554 = x466 - x553;
  const float x555 = x214 * x78;
  const float x556 = x214 * x79;
  const float x557 = x20 * x78;
  const float x558 = x20 * x79;
  const float x559 = x205 * x29;
  const float x560 = x220 * x307;
  const float x561 = x205 * x32;
  const float x562 = x220 * x76;
  const float x563 = x327 + x328;
  const float x564 = 8 * a_0 * b_2 * c_0 + 8 * a_1 * b_2 * c_1 - x370 - x371
                     + x376 + x377 - x380 - x382 + x384 + x386 - x389 + x555
                     + x556 - x557 - x558 + x559 + x560 - x561 - x562 - x563;
  const float x565
      = x144 * x523 + x146 * x526
        + x147
              * (-x141 * x539 + x157 * x540 - x169 * x533
                 - x174 * (x127 * x172 + x280) + x180 * (x127 * x176 + x270)
                 + x185 * x545 - x187 * x546 - x284 * x527 + x285 * x534
                 + x287 * x544 + x489 * x543)
        + x190 * x535
        + x230
              * (x104 * x548 + x108 * (-x330 - x549 - x550 - x552 - x554)
                 + x198 * x536 + x229 * x564 + x258 * x543 + x295 * x537)
        + x261 * x538;
  const float x566 = x11 - x12 - x13 - x221 - x223 + x24 + x30 - x35 - x37
                     + x395 + x396 + x398 + x400 - x402 - x404 + x405 + x43 + x8
                     - x90 - x95;
  const float x567 = -x67;
  const float x568 = a_1 * x28;
  const float x569 = x567 + x568;
  const float x570 = x409 + x569 + x98 - x99;
  const float x571 = -x570;
  const float x572 = x113 * x165 + x119 * x131;
  const float x573 = x119 * x150;
  const float x574 = x113 * x123;
  const float x575 = 2 * x573 + 2 * x574;
  const float x576 = -x22 + x231;
  const float x577 = x129 * x576;
  const float x578 = -x28 + x322;
  const float x579 = x134 * x578;
  const float x580 = -1.0 / 2.0 * x577 - 1.0 / 2.0 * x579;
  const float x581 = -x577 - x579;
  const float x582 = (1.0 / 2.0) * x135 * x138 * x575
                     + (1.0 / 2.0) * x136 * x178 * x581 - x141 * x572
                     - x174 * x580;
  const float x583 = -x566;
  const float x584 = x108 * x583 + x258 * x570;
  const float x585 = x573 + x574;
  const float x586 = x119 * x172;
  const float x587 = x119 * x176;
  const float x588 = x182 * x575;
  const float x589 = -2 * x577 - 2 * x579;
  const float x590 = -3.0 / 2.0 * x577 - 3.0 / 2.0 * x579;
  const float x591 = -x307 + x66;
  const float x592 = x435 - x46 + x54 + x591;
  const float x593 = x83 + x84;
  const float x594 = -x10 + x438;
  const float x595 = x441 - x7;
  const float x596 = 8 * a_1 * b_1 * c_0 + 8 * a_2 * b_2 * c_0 - x203 - x204
                     + x206 + x207 - x210 - x213 + x216 + x217 - x227 - x443
                     - x444 + x445 + x446 - x447 - x448 + x449 + x450 - x451;
  const float x597
      = x144 * x566 + x146 * x571
        + x147
              * (x157 * x585 - x169 * x580 - x174 * (x113 * x170 + x586)
                 + x180 * (x113 * x175 + x587) + x185 * x589 - x187 * x590
                 - x284 * x572 + x285 * x581 + x287 * x588)
        + x190 * x582
        + x230
              * (x104 * x592 + x108 * (-x593 - x594 - x595) + x198 * x570
                 + x229 * x596 + x295 * x583)
        + x261 * x584;
  const float x598 = -x232 - x233 + x238 + x241 - x243 - x245 + x248 - x314
                     - x315 + x454 + x455 - x456 - x457 + x459 + x460 - x462
                     - x463 + x464 + x85 + x88;
  const float x599 = x549 + x64;
  const float x600 = x250 - x251 + x469 + x599;
  const float x601 = -x124;
  const float x602 = x119 * x262 + x165 * x601;
  const float x603 = x123 * x601;
  const float x604 = x112 * x119;
  const float x605 = 2 * x603 + 2 * x604;
  const float x606 = x476 - x89;
  const float x607 = -x606;
  const float x608 = x129 * x607;
  const float x609 = x121 * x578;
  const float x610 = -1.0 / 2.0 * x608 - 1.0 / 2.0 * x609;
  const float x611 = -x608 - x609;
  const float x612 = (1.0 / 2.0) * x135 * x138 * x605
                     + (1.0 / 2.0) * x136 * x178 * x611 - x141 * x602
                     - x174 * x610;
  const float x613 = -x600;
  const float x614 = -x598;
  const float x615 = x108 * x614 + x258 * x613;
  const float x616 = b_2 + x122;
  const float x617 = x603 + x604;
  const float x618 = x28 + x487;
  const float x619 = -x618;
  const float x620 = x182 * x605;
  const float x621 = -2 * x608 - 2 * x609;
  const float x622 = -3.0 / 2.0 * x608 - 3.0 / 2.0 * x609;
  const float x623 = -x29 + x378;
  const float x624 = -x48 - x494 + x56 - x623;
  const float x625 = 4 * x34;
  const float x626 = x38 - x625;
  const float x627 = x458 - x497;
  const float x628 = 8 * a_0 * b_0 * c_1 + 8 * a_2 * b_2 * c_1 - x298 - x299
                     + x300 + x301 - x305 - x308 + x310 + x312 - x319 - x502
                     - x503 + x504 + x505 - x506 - x507 + x508 + x509 - x510;
  const float x629
      = x144 * x598 + x146 * x600
        + x147
              * (-x141 * x616 + x157 * x617 - x169 * x610
                 - x174 * (x129 + x170 * x601) + x180 * (x175 * x601 + x355)
                 + x185 * x621 - x187 * x622 - x284 * x602 + x285 * x611
                 + x287 * x620 + x489 * x619)
        + x190 * x612
        + x230
              * (x104 * x624 + x108 * (-x500 - x524 - x626 - x627 - x70)
                 + x198 * x613 + x229 * x628 + x258 * x619 + x295 * x614)
        + x261 * x615;
  const float x630 = -x323 - x324 + x329 + x332 - x334 - x336 + x339 - x387
                     - x388 + x513 + x514 - x515 - x516 + x518 + x519 - x520
                     - x521 + x522 + x91 + x92;
  const float x631 = x496 + x62;
  const float x632 = x341 - x342 + x525 + x631;
  const float x633 = -x632;
  const float x634 = -x113;
  const float x635 = x131 * x601 + x262 * x634;
  const float x636 = x150 * x601;
  const float x637 = x112 * x634;
  const float x638 = 2 * x636 + 2 * x637;
  const float x639 = x134 * x607;
  const float x640 = -x576;
  const float x641 = x121 * x640;
  const float x642 = -1.0 / 2.0 * x639 - 1.0 / 2.0 * x641;
  const float x643 = -x639 - x641;
  const float x644 = (1.0 / 2.0) * x135 * x138 * x638
                     + (1.0 / 2.0) * x136 * x178 * x643 - x141 * x635
                     - x174 * x642;
  const float x645 = -x630;
  const float x646 = x108 * x645 + x258 * x632;
  const float x647 = b_1 + x130;
  const float x648 = x636 + x637;
  const float x649 = x22 + x541;
  const float x650 = x268 - x269;
  const float x651 = x182 * x638;
  const float x652 = -2 * x639 - 2 * x641;
  const float x653 = -3.0 / 2.0 * x639 - 3.0 / 2.0 * x641;
  const float x654 = -x23 + x304;
  const float x655 = -x51 + x547 + x58 + x654;
  const float x656 = 4 * x36;
  const float x657 = x40 - x656;
  const float x658 = x517 - x550;
  const float x659 = 8 * a_0 * b_0 * c_2 + 8 * a_1 * b_1 * c_2 - x372 - x373
                     + x374 + x375 - x379 - x381 + x383 + x385 - x392 - x555
                     - x556 + x557 + x558 - x559 - x560 + x561 + x562 - x563;
  const float x660
      = x144 * x630 + x146 * x633
        + x147
              * (x141 * x647 + x157 * x648 - x169 * x642
                 - x174 * (x134 + x172 * x601) + x180 * (x176 * x601 + x650)
                 + x185 * x652 - x187 * x653 - x284 * x635 + x285 * x643
                 + x287 * x651 + x489 * x649)
        + x190 * x644
        + x230
              * (x104 * x655 + x108 * (-x467 - x553 - x657 - x658 - x74)
                 + x198 * x632 + x229 * x659 + x258 * x649 + x295 * x645)
        + x261 * x646;
  const float x661 = -x408;
  const float x662 = x102 + x569 + x661 + x69;
  const float x663 = -x662;
  const float x664 = 1.0 / x61;
  const float x665 = x188 * x664;
  const float x666 = -x121;
  const float x667 = -x155;
  const float x668 = x182 * x287;
  const float x669 = x192 + x591 - x68 + x76;
  const float x670 = -x669;
  const float x671 = -x121 * x141 + (1.0 / 2.0) * x135 * x138 * x667;
  const float x672
      = x146 * x662 + x147 * (-x121 * x284 + x157 * x666 + x667 * x668)
        + x190 * x671 + x230 * (x104 * x670 + x198 * x663) + x663 * x665;
  const float x673 = -x65;
  const float x674 = x255 + x466 + x599 + x673;
  const float x675 = -x134;
  const float x676 = (1.0 / 2.0) * x135 * x138 * x650 - x141 * x675;
  const float x677 = -x674;
  const float x678 = -x19 + x291 + x32 + x623;
  const float x679 = x146 * x677
                     + x147
                           * (x134 * x157 - x141 * x172 + x162 * x489
                              - x284 * x675 + x650 * x668)
                     + x190 * x676
                     + x230 * (x104 * x678 + x162 * x258 + x198 * x674)
                     + x665 * x674;
  const float x680 = -x63;
  const float x681 = x344 + x461 + x631 + x680;
  const float x682 = -x681;
  const float x683 = -x129;
  const float x684 = -x355;
  const float x685 = -x129 * x141 + (1.0 / 2.0) * x135 * x138 * x684;
  const float x686 = -x16 + x26 + x367 + x654;
  const float x687 = -x686;
  const float x688 = x146 * x681
                     + x147
                           * (-x129 * x284 - x141 * x148 + x157 * x683
                              + x175 * x489 + x668 * x684)
                     + x190 * x685
                     + x230 * (x104 * x687 + x175 * x258 + x198 * x682)
                     + x665 * x682;
  const float x689 = x143 * x260;
  const float x690 = x145 * x260;
  const float x691 = x139 * x271;
  const float x692 = x168 * x277;
  const float x693 = x263 * x264;
  const float x694 = x264 * x273;
  const float x695 = x184 * x278;
  const float x696 = x186 * x277;
  const float x697 = x189 * x260;
  const float x698 = x257 * x97;
  const float x699 = -4 * b_0 * c_0;
  const float x700 = 2 * x81;
  const float x701 = x4 + x700;
  const float x702 = 2 * x80;
  const float x703 = x10 + x702;
  const float x704 = x197 * x257;
  const float x705 = x228 * x259;
  const float x706 = x189 * x279;
  const float x707 = x168 * x265;
  const float x708 = (1.0 / 2.0) * x278;
  const float x709 = (1.0 / 2.0) * x286;
  const float x710 = -x77;
  const float x711 = c_2 * x25;
  const float x712 = x197 * x259;
  const float x713
      = x147
            * (-x170 * x172 * x174 + x170 * x176 * x180 - x350 * x692
               - x359 * x707 + x360 * x709 + x362 * x691 + x363 * x708
               + x364 * x695 - x365 * x696)
        + x230
              * (x108 * (x101 + x710 + x711 + x72) + x345 * x712 + x347 * x704
                 + x368 * x698 + x393 * x705)
        + x340 * x689 + x346 * x690 + x348 * x706 + x361 * x697;
  const float x714 = -x488;
  const float x715 = -x62;
  const float x716 = 4 * x23;
  const float x717 = 4 * x26;
  const float x718 = x63 - x717;
  const float x719
      = x147
            * (-x141 * x485 - x174 * (x129 + x263 * x411)
               + x180 * (x273 * x411 + x355) - x413 * x692 - x423 * x707
               + x424 * x709 + x429 * x691 + x432 * x708 + x433 * x695
               - x434 * x696 + x489 * x714)
        + x230
              * (x108 * (-x239 - x626 - x715 - x716 - x718) + x258 * x714
                 + x426 * x712 + x427 * x704 + x436 * x698 + x452 * x705)
        + x406 * x689 + x410 * x690 + x425 * x697 + x428 * x706;
  const float x720 = x127 * x263;
  const float x721 = x127 * x273;
  const float x722 = c_0 * x89;
  const float x723 = -a_0 * x252;
  const float x724 = a_0 * x89;
  const float x725 = x723 + x724;
  const float x726 = -x722 + x725;
  const float x727
      = x147
            * (-x174 * (x430 + x720) + x180 * (x431 + x721) - x472 * x692
               - x480 * x707 + x481 * x709 + x486 * x691 + x491 * x708
               + x492 * x695 - x493 * x696)
        + x230
              * (x108 * (x439 + x701 + x726) + x470 * x712 + x483 * x704
                 + x495 * x698 + x511 * x705)
        + x465 * x689 + x471 * x690 + x482 * x697 + x484 * x706;
  const float x728 = c_0 + x111;
  const float x729 = -2 * d_0;
  const float x730 = x252 + x729;
  const float x731 = -x153 + x154;
  const float x732 = 4 * x66;
  const float x733 = 4 * x55;
  const float x734 = -x733 + x77;
  const float x735 = 4 * x68;
  const float x736 = x69 - x735;
  const float x737
      = x147
            * (x141 * x728 - x174 * (x118 * x172 + x166)
               + x180 * (x118 * x176 + x731) + x489 * x730 - x527 * x692
               - x533 * x707 + x534 * x709 + x540 * x691 + x544 * x708
               + x545 * x695 - x546 * x696)
        + x230
              * (x108 * (-x567 - x711 - x732 - x734 - x736) + x258 * x730
                 + x536 * x712 + x537 * x704 + x548 * x698 + x564 * x705)
        + x523 * x689 + x526 * x690 + x535 * x697 + x538 * x706;
  const float x738 = x62 - x716;
  const float x739
      = x147
            * (x141 * x616 - x174 * (x113 * x263 + x349)
               + x180 * (x113 * x273 + x490) + x489 * x618 - x572 * x692
               - x580 * x707 + x581 * x709 + x585 * x691 + x588 * x708
               + x589 * x695 - x590 * x696)
        + x230
              * (x108 * (-x499 - x680 - x70 - x717 - x738) + x258 * x618
                 + x570 * x712 + x583 * x704 + x592 * x698 + x596 * x705)
        + x566 * x689 + x571 * x690 + x582 * x697 + x584 * x706;
  const float x740 = x263 * x601;
  const float x741 = x273 * x601;
  const float x742 = x722 + x84;
  const float x743 = -x702 + x725;
  const float x744
      = x147
            * (-x174 * (x586 + x740) + x180 * (x587 + x741) - x602 * x692
               - x610 * x707 + x611 * x709 + x617 * x691 + x620 * x708
               + x621 * x695 - x622 * x696)
        + x230
              * (x108 * (-x594 - x742 - x743) + x613 * x712 + x614 * x704
                 + x624 * x698 + x628 * x705)
        + x598 * x689 + x600 * x690 + x612 * x697 + x615 * x706;
  const float x745 = b_0 + x111;
  const float x746 = x729 + x89;
  const float x747 = -x746;
  const float x748 = 4 * x45;
  const float x749 = x73 - x748;
  const float x750 = x67 - x732;
  const float x751
      = x147
            * (-x141 * x745 - x174 * (x121 + x172 * x634)
               + x180 * (x155 + x176 * x634) + x489 * x747 - x635 * x692
               - x642 * x707 + x643 * x709 + x648 * x691 + x651 * x708
               + x652 * x695 - x653 * x696)
        + x230
              * (x108 * (-x407 - x72 - x735 - x749 - x750) + x258 * x747
                 + x632 * x712 + x645 * x704 + x655 * x698 + x659 * x705)
        + x630 * x689 + x633 * x690 + x644 * x697 + x646 * x706;
  const float x752 = x279 * x664;
  const float x753 = x182 * x708;
  const float x754 = x147
                         * (-x121 * x692 - x141 * x151 + x176 * x489
                            + x666 * x691 + x667 * x753)
                     + x230 * (x176 * x258 + x663 * x712 + x670 * x698)
                     + x662 * x690 + x663 * x752 + x671 * x697;
  const float x755 = x147 * (x134 * x691 + x650 * x753 - x675 * x692)
                     + x230 * (x674 * x712 + x678 * x698) + x674 * x752
                     + x676 * x697 + x677 * x690;
  const float x756 = x147
                         * (-x129 * x692 - x141 * x264 + x273 * x489
                            + x683 * x691 + x684 * x753)
                     + x230 * (x258 * x273 + x682 * x712 + x687 * x698)
                     + x681 * x690 + x682 * x752 + x685 * x697;
  const float x757 = x143 * x348;
  const float x758 = x145 * x348;
  const float x759 = x139 * x356;
  const float x760 = x168 * x359;
  const float x761 = x184 * x360;
  const float x762 = x186 * x359;
  const float x763 = x189 * x348;
  const float x764 = x345 * x97;
  const float x765 = x1 + x700;
  const float x766 = x7 + x702;
  const float x767 = x197 * x347;
  const float x768 = x228 * x347;
  const float x769 = x189 * x361;
  const float x770 = x168 * x350;
  const float x771 = (1.0 / 2.0) * x360;
  const float x772 = (1.0 / 2.0) * x363;
  const float x773 = -x64;
  const float x774 = 4 * x29;
  const float x775 = 4 * x32;
  const float x776 = x65 - x775;
  const float x777 = x197 * x345;
  const float x778
      = x147
            * (x141 * x539 - x174 * (x134 + x263 * x412)
               + x180 * (x273 * x412 + x650) - x413 * x760 - x423 * x770
               + x424 * x772 + x429 * x759 + x432 * x771 + x433 * x761
               - x434 * x762 + x489 * x542)
        + x230
              * (x108 * (-x330 - x657 - x773 - x774 - x776) + x258 * x542
                 + x426 * x767 + x427 * x777 + x436 * x764 + x452 * x768)
        + x406 * x757 + x410 * x758 + x425 * x763 + x428 * x769;
  const float x779 = -x730;
  const float x780 = -x568;
  const float x781 = 4 * x307;
  const float x782 = 4 * x76;
  const float x783 = x408 - x782;
  const float x784
      = x147
            * (-x141 * x728 - x174 * (x121 + x148 * x412)
               + x180 * (x155 + x159 * x412) - x472 * x760 - x480 * x770
               + x481 * x772 + x486 * x759 + x489 * x779 + x491 * x771
               + x492 * x761 - x493 * x762)
        + x230
              * (x108 * (-x711 - x749 - x780 - x781 - x783) + x258 * x779
                 + x470 * x767 + x483 * x777 + x495 * x764 + x511 * x768)
        + x465 * x757 + x471 * x758 + x482 * x763 + x484 * x769;
  const float x785
      = x147
            * (-x174 * (x118 * x148 + x720) + x180 * (x118 * x159 + x721)
               - x527 * x760 - x533 * x770 + x534 * x772 + x540 * x759
               + x544 * x771 + x545 * x761 - x546 * x762)
        + x230
              * (x108 * (x442 + x726 + x765) + x536 * x767 + x537 * x777
                 + x548 * x764 + x564 * x768)
        + x523 * x757 + x526 * x758 + x535 * x763 + x538 * x769;
  const float x786 = -x649;
  const float x787 = x64 - x774;
  const float x788
      = x147
            * (-x141 * x647 - x174 * (x119 * x263 + x280)
               + x180 * (x119 * x273 + x270) + x489 * x786 - x572 * x760
               - x580 * x770 + x581 * x772 + x585 * x759 + x588 * x771
               + x589 * x761 - x590 * x762)
        + x230
              * (x108 * (-x552 - x673 - x74 - x775 - x787) + x258 * x786
                 + x570 * x767 + x583 * x777 + x592 * x764 + x596 * x768)
        + x566 * x757 + x571 * x758 + x582 * x763 + x584 * x769;
  const float x789 = x568 - x781;
  const float x790
      = x147
            * (x141 * x745 - x174 * (x119 * x148 + x166)
               + x180 * (x119 * x159 + x731) + x489 * x746 - x602 * x760
               - x610 * x770 + x611 * x772 + x617 * x759 + x620 * x771
               + x621 * x761 - x622 * x762)
        + x230
              * (x108 * (-x661 - x72 - x734 - x782 - x789) + x258 * x746
                 + x613 * x767 + x614 * x777 + x624 * x764 + x628 * x768)
        + x598 * x757 + x600 * x758 + x612 * x763 + x615 * x769;
  const float x791 = x722 + x83;
  const float x792
      = x147
            * (-x174 * (x148 * x634 + x740) + x180 * (x159 * x634 + x741)
               - x635 * x760 - x642 * x770 + x643 * x772 + x648 * x759
               + x651 * x771 + x652 * x761 - x653 * x762)
        + x230
              * (x108 * (-x595 - x743 - x791) + x632 * x767 + x645 * x777
                 + x655 * x764 + x659 * x768)
        + x630 * x757 + x633 * x758 + x644 * x763 + x646 * x769;
  const float x793 = x361 * x664;
  const float x794 = x182 * x771;
  const float x795 = x147
                         * (-x121 * x760 - x141 * x170 + x159 * x489
                            + x666 * x759 + x667 * x794)
                     + x230 * (x159 * x258 + x663 * x767 + x670 * x764)
                     + x662 * x758 + x663 * x793 + x671 * x763;
  const float x796 = x147
                         * (x134 * x759 - x141 * x263 + x274 * x489
                            + x650 * x794 - x675 * x760)
                     + x230 * (x258 * x274 + x674 * x767 + x678 * x764)
                     + x674 * x793 + x676 * x763 + x677 * x758;
  const float x797 = x147 * (-x129 * x760 + x683 * x759 + x684 * x794)
                     + x230 * (x682 * x767 + x687 * x764) + x681 * x758
                     + x682 * x793 + x685 * x763;
  const float x798 = x143 * x428;
  const float x799 = x145 * x428;
  const float x800 = x139 * x416;
  const float x801 = x168 * x423;
  const float x802 = x118 * x411;
  const float x803 = x115 * x412;
  const float x804 = x412 * x420;
  const float x805 = x184 * x424;
  const float x806 = x186 * x423;
  const float x807 = x189 * x428;
  const float x808 = x426 * x97;
  const float x809 = 2 * x82;
  const float x810 = -4 * a_2 * c_2 + x809;
  const float x811 = 2 * x79;
  const float x812 = -4 * a_1 * c_1 + x811;
  const float x813 = x197 * x427;
  const float x814 = x228 * x427;
  const float x815 = x189 * x425;
  const float x816 = x168 * x413;
  const float x817 = x127 * x174;
  const float x818 = x127 * x180;
  const float x819 = (1.0 / 2.0) * x424;
  const float x820 = (1.0 / 2.0) * x432;
  const float x821 = a_0 * x231;
  const float x822 = x197 * x426;
  const float x823 = x147
                         * (-x118 * x817 + x417 * x818 - x472 * x801
                            - x480 * x816 + x481 * x820 + x486 * x800
                            + x491 * x819 + x492 * x805 - x493 * x806)
                     + x230
                           * (x108 * (x239 + x524 + x680 + x821) + x470 * x813
                              + x483 * x822 + x495 * x808 + x511 * x814)
                     + x465 * x798 + x471 * x799 + x482 * x807 + x484 * x815;
  const float x824 = a_0 * x322;
  const float x825 = x147
                         * (-x115 * x817 + x420 * x818 - x527 * x801
                            - x533 * x816 + x534 * x820 + x540 * x800
                            + x544 * x819 + x545 * x805 - x546 * x806)
                     + x230
                           * (x108 * (x330 + x467 + x673 + x824) + x536 * x813
                              + x537 * x822 + x548 * x808 + x564 * x814)
                     + x523 * x798 + x526 * x799 + x535 * x807 + x538 * x815;
  const float x826 = x115 * x119;
  const float x827 = x119 * x420;
  const float x828 = -x399 + x437 + x809;
  const float x829 = -x397 + x440 + x811;
  const float x830
      = x147
            * (-x174 * (x113 * x118 + x826) + x180 * (x113 * x417 + x827)
               - x572 * x801 - x580 * x816 + x581 * x820 + x585 * x800
               + x588 * x819 + x589 * x805 - x590 * x806)
        + x230
              * (x108 * (x593 + x828 + x829) + x570 * x813 + x583 * x822
                 + x592 * x808 + x596 * x814)
        + x566 * x798 + x571 * x799 + x582 * x807 + x584 * x815;
  const float x831 = x322 + x487;
  const float x832
      = x147
            * (-x141 * x165 - x174 * (x118 * x601 + x349)
               + x180 * (x417 * x601 + x490) + x489 * x831 - x602 * x801
               - x610 * x816 + x611 * x820 + x617 * x800 + x620 * x819
               + x621 * x805 - x622 * x806)
        + x230
              * (x108 * (-x294 - x501 - x625 - x738 - x821) + x258 * x831
                 + x613 * x813 + x614 * x822 + x624 * x808 + x628 * x814)
        + x598 * x798 + x600 * x799 + x612 * x807 + x615 * x815;
  const float x833 = x231 + x541;
  const float x834 = -x833;
  const float x835
      = x147
            * (-x131 * x141 - x174 * (x115 * x601 + x280)
               + x180 * (x270 + x420 * x601) + x489 * x834 - x635 * x801
               - x642 * x816 + x643 * x820 + x648 * x800 + x651 * x819
               + x652 * x805 - x653 * x806)
        + x230
              * (x108 * (-x254 - x554 - x656 - x787 - x824) + x258 * x834
                 + x632 * x813 + x645 * x822 + x655 * x808 + x659 * x814)
        + x630 * x798 + x633 * x799 + x644 * x807 + x646 * x815;
  const float x836 = x425 * x664;
  const float x837 = x182 * x819;
  const float x838 = x147 * (-x121 * x801 + x666 * x800 + x667 * x837)
                     + x230 * (x663 * x813 + x670 * x808) + x662 * x799
                     + x663 * x836 + x671 * x807;
  const float x839 = x147
                         * (-x115 * x141 + x134 * x800 + x421 * x489
                            + x650 * x837 - x675 * x801)
                     + x230 * (x258 * x421 + x674 * x813 + x678 * x808)
                     + x674 * x836 + x676 * x807 + x677 * x799;
  const float x840 = x147
                         * (-x129 * x801 - x141 * x411 + x417 * x489
                            + x683 * x800 + x684 * x837)
                     + x230 * (x258 * x417 + x682 * x813 + x687 * x808)
                     + x681 * x799 + x682 * x836 + x685 * x807;
  const float x841 = x143 * x484;
  const float x842 = x145 * x484;
  const float x843 = x139 * x475;
  const float x844 = x168 * x480;
  const float x845 = -x127;
  const float x846 = x127 * x845;
  const float x847 = -x477;
  const float x848 = x127 * x847;
  const float x849 = x184 * x481;
  const float x850 = x186 * x480;
  const float x851 = x189 * x484;
  const float x852 = x470 * x97;
  const float x853 = 2 * x78;
  const float x854 = -4 * a_0 * c_0 + x853;
  const float x855 = x197 * x483;
  const float x856 = x228 * x483;
  const float x857 = x189 * x482;
  const float x858 = x168 * x472;
  const float x859 = (1.0 / 2.0) * x481;
  const float x860 = (1.0 / 2.0) * x491;
  const float x861 = a_2 * x231;
  const float x862 = x197 * x470;
  const float x863
      = x147
            * (-x115 * x118 * x174 + x118 * x180 * x420 - x527 * x844
               - x533 * x858 + x534 * x860 + x540 * x843 + x544 * x859
               + x545 * x849 - x546 * x850)
        + x230
              * (x108 * (x407 + x661 + x711 + x861) + x536 * x855 + x537 * x862
                 + x548 * x852 + x564 * x856)
        + x523 * x841 + x526 * x842 + x535 * x851 + x538 * x857;
  const float x864 = -x831;
  const float x865
      = x147
            * (-x123 * x141 - x174 * (x113 * x845 + x129)
               + x180 * (x113 * x847 + x355) + x489 * x864 - x572 * x844
               - x580 * x858 + x581 * x860 + x585 * x843 + x588 * x859
               + x589 * x849 - x590 * x850)
        + x230
              * (x108 * (-x293 - x498 - x627 - x718 - x821) + x258 * x864
                 + x570 * x855 + x583 * x862 + x592 * x852 + x596 * x856)
        + x566 * x841 + x571 * x842 + x582 * x851 + x584 * x857;
  const float x866 = x601 * x845;
  const float x867 = x601 * x847;
  const float x868 = x723 - x724 + x853;
  const float x869
      = x147
            * (-x174 * (x826 + x866) + x180 * (x827 + x867) - x602 * x844
               - x610 * x858 + x611 * x860 + x617 * x843 + x620 * x859
               + x621 * x849 - x622 * x850)
        + x230
              * (x108 * (x742 + x828 + x868) + x613 * x855 + x614 * x862
                 + x624 * x852 + x628 * x856)
        + x598 * x841 + x600 * x842 + x612 * x851 + x615 * x857;
  const float x870 = x476 + x729;
  const float x871
      = x147
            * (-x141 * x262 - x174 * (x115 * x634 + x166)
               + x180 * (x420 * x634 + x731) + x489 * x870 - x635 * x844
               - x642 * x858 + x643 * x860 + x648 * x843 + x651 * x859
               + x652 * x849 - x653 * x850)
        + x230
              * (x108 * (-x101 - x736 - x748 - x789 - x861) + x258 * x870
                 + x632 * x855 + x645 * x862 + x655 * x852 + x659 * x856)
        + x630 * x841 + x633 * x842 + x644 * x851 + x646 * x857;
  const float x872 = x482 * x664;
  const float x873 = x182 * x859;
  const float x874 = x147
                         * (-x121 * x844 - x141 * x412 + x420 * x489
                            + x666 * x843 + x667 * x873)
                     + x230 * (x258 * x420 + x663 * x855 + x670 * x852)
                     + x662 * x842 + x663 * x872 + x671 * x851;
  const float x875 = x147 * (x134 * x843 + x650 * x873 - x675 * x844)
                     + x230 * (x674 * x855 + x678 * x852) + x674 * x872
                     + x676 * x851 + x677 * x842;
  const float x876 = x147
                         * (-x127 * x141 - x129 * x844 + x489 * x847
                            + x683 * x843 + x684 * x873)
                     + x230 * (x258 * x847 + x682 * x855 + x687 * x852)
                     + x681 * x842 + x682 * x872 + x685 * x851;
  const float x877 = x143 * x538;
  const float x878 = x145 * x538;
  const float x879 = x139 * x530;
  const float x880 = x168 * x527;
  const float x881 = x184 * x534;
  const float x882 = x186 * x533;
  const float x883 = x189 * x538;
  const float x884 = x536 * x97;
  const float x885 = x197 * x536;
  const float x886 = x228 * x537;
  const float x887 = x189 * x535;
  const float x888 = x168 * x533;
  const float x889 = (1.0 / 2.0) * x544;
  const float x890 = (1.0 / 2.0) * x534;
  const float x891 = x197 * x537;
  const float x892
      = x147
            * (-x141 * x150 - x174 * (x119 * x845 + x134)
               + x180 * (x119 * x847 + x650) + x489 * x833 - x572 * x888
               - x580 * x880 + x581 * x889 + x585 * x879 + x588 * x890
               + x589 * x881 - x590 * x882)
        + x230
              * (x108 * (-x369 - x551 - x658 - x776 - x824) + x258 * x833
                 + x570 * x891 + x583 * x885 + x592 * x884 + x596 * x886)
        + x566 * x877 + x571 * x878 + x582 * x883 + x584 * x887;
  const float x893 = -x870;
  const float x894
      = x147
            * (-x112 * x141 - x174 * (x119 * x411 + x121)
               + x180 * (x119 * x418 + x155) + x489 * x893 - x602 * x888
               - x610 * x880 + x611 * x889 + x617 * x879 + x620 * x890
               + x621 * x881 - x622 * x882)
        + x230
              * (x108 * (-x710 - x733 - x750 - x783 - x861) + x258 * x893
                 + x613 * x891 + x614 * x885 + x624 * x884 + x628 * x886)
        + x598 * x877 + x600 * x878 + x612 * x883 + x615 * x887;
  const float x895
      = x147
            * (-x174 * (x411 * x634 + x866) + x180 * (x418 * x634 + x867)
               - x635 * x888 - x642 * x880 + x643 * x889 + x648 * x879
               + x651 * x890 + x652 * x881 - x653 * x882)
        + x230
              * (x108 * (x791 + x829 + x868) + x632 * x891 + x645 * x885
                 + x655 * x884 + x659 * x886)
        + x630 * x877 + x633 * x878 + x644 * x883 + x646 * x887;
  const float x896 = x535 * x664;
  const float x897 = x182 * x890;
  const float x898 = x147
                         * (-x118 * x141 - x121 * x888 + x418 * x489
                            + x666 * x879 + x667 * x897)
                     + x230 * (x258 * x418 + x663 * x891 + x670 * x884)
                     + x662 * x878 + x663 * x896 + x671 * x883;
  const float x899 = x147
                         * (x134 * x879 - x141 * x845 + x477 * x489
                            + x650 * x897 - x675 * x888)
                     + x230 * (x258 * x477 + x674 * x891 + x678 * x884)
                     + x674 * x896 + x676 * x883 + x677 * x878;
  const float x900 = x147 * (-x129 * x888 + x683 * x879 + x684 * x897)
                     + x230 * (x682 * x891 + x687 * x884) + x681 * x878
                     + x682 * x896 + x685 * x883;
  const float x901 = x143 * x584;
  const float x902 = x145 * x584;
  const float x903 = x139 * x575;
  const float x904 = x168 * x572;
  const float x905 = x113 * x634;
  const float x906 = -x119;
  const float x907 = x119 * x906;
  const float x908 = -x578;
  const float x909 = x119 * x908;
  const float x910 = x184 * x581;
  const float x911 = x186 * x580;
  const float x912 = x189 * x584;
  const float x913 = x570 * x97;
  const float x914 = -4 * a_2 * b_2 + x809;
  const float x915 = -4 * a_1 * b_1 + x811;
  const float x916 = x197 * x583;
  const float x917 = x228 * x583;
  const float x918 = x189 * x582;
  const float x919 = x168 * x580;
  const float x920 = x174 * x601;
  const float x921 = x180 * x601;
  const float x922 = (1.0 / 2.0) * x588;
  const float x923 = (1.0 / 2.0) * x581;
  const float x924 = x197 * x570;
  const float x925 = x147
                         * (-x602 * x919 - x610 * x904 + x611 * x922
                            + x617 * x903 + x620 * x923 + x621 * x910
                            - x622 * x911 - x634 * x920 + x640 * x921)
                     + x230
                           * (x108 * (x496 + x70 + x715 + x821) + x613 * x916
                              + x614 * x924 + x624 * x913 + x628 * x917)
                     + x598 * x901 + x600 * x902 + x612 * x912 + x615 * x918;
  const float x926 = x147
                         * (-x635 * x919 - x642 * x904 + x643 * x922
                            + x648 * x903 + x651 * x923 + x652 * x910
                            - x653 * x911 - x906 * x920 + x908 * x921)
                     + x230
                           * (x108 * (x549 + x74 + x773 + x824) + x632 * x916
                              + x645 * x924 + x655 * x913 + x659 * x917)
                     + x630 * x901 + x633 * x902 + x644 * x912 + x646 * x918;
  const float x927 = x582 * x664;
  const float x928 = x182 * x923;
  const float x929 = x147 * (-x121 * x919 + x666 * x903 + x667 * x928)
                     + x230 * (x663 * x916 + x670 * x913) + x662 * x902
                     + x663 * x927 + x671 * x912;
  const float x930 = x147
                         * (x134 * x903 - x141 * x906 + x489 * x578
                            + x650 * x928 - x675 * x919)
                     + x230 * (x258 * x578 + x674 * x916 + x678 * x913)
                     + x674 * x927 + x676 * x912 + x677 * x902;
  const float x931 = x147
                         * (-x113 * x141 - x129 * x919 + x489 * x640
                            + x683 * x903 + x684 * x928)
                     + x230 * (x258 * x640 + x682 * x916 + x687 * x913)
                     + x681 * x902 + x682 * x927 + x685 * x912;
  const float x932 = x143 * x615;
  const float x933 = x145 * x615;
  const float x934 = x139 * x605;
  const float x935 = x168 * x610;
  const float x936 = x124 * x601;
  const float x937 = x601 * x606;
  const float x938 = x184 * x611;
  const float x939 = x186 * x610;
  const float x940 = x189 * x615;
  const float x941 = x613 * x97;
  const float x942 = -4 * a_0 * b_0 + x853;
  const float x943 = x197 * x614;
  const float x944 = x228 * x614;
  const float x945 = x189 * x646;
  const float x946 = x168 * x642;
  const float x947 = (1.0 / 2.0) * x611;
  const float x948 = (1.0 / 2.0) * x643;
  const float x949 = x197 * x645;
  const float x950
      = x147
            * (-x174 * x634 * x906 + x180 * x634 * x908 - x602 * x946
               + x620 * x948 - x635 * x935 + x648 * x934 + x651 * x947
               + x652 * x938 - x653 * x939)
        + x230
              * (x108 * (x567 + x72 + x780 + x861) + x613 * x949 + x632 * x943
                 + x655 * x941 + x659 * x944)
        + x612 * x945 + x630 * x932 + x633 * x933 + x644 * x940;
  const float x951 = x612 * x664;
  const float x952 = x182 * x947;
  const float x953 = x147
                         * (-x119 * x141 - x121 * x935 + x489 * x908
                            + x666 * x934 + x667 * x952)
                     + x230 * (x258 * x908 + x663 * x943 + x670 * x941)
                     + x662 * x933 + x663 * x951 + x671 * x940;
  const float x954 = x147 * (x134 * x934 + x650 * x952 - x675 * x935)
                     + x230 * (x674 * x943 + x678 * x941) + x674 * x951
                     + x676 * x940 + x677 * x933;
  const float x955 = x147
                         * (-x129 * x935 - x141 * x601 + x489 * x606
                            + x683 * x934 + x684 * x952)
                     + x230 * (x258 * x606 + x682 * x943 + x687 * x941)
                     + x681 * x933 + x682 * x951 + x685 * x940;
  const float x956 = x145 * x646;
  const float x957 = x139 * x638;
  const float x958 = x632 * x97;
  const float x959 = x644 * x664;
  const float x960 = x182 * x948;
  const float x961 = x147
                         * (-x121 * x946 - x141 * x634 + x489 * x576
                            + x666 * x957 + x667 * x960)
                     + x230 * (x258 * x576 + x663 * x949 + x670 * x958)
                     + x662 * x956 + x663 * x959 + x671 * x945;
  const float x962 = x147
                         * (-x124 * x141 + x134 * x957 + x489 * x607
                            + x650 * x960 - x675 * x946)
                     + x230 * (x258 * x607 + x674 * x949 + x678 * x958)
                     + x674 * x959 + x676 * x945 + x677 * x956;
  const float x963 = x147 * (-x129 * x946 + x683 * x957 + x684 * x960)
                     + x230 * (x682 * x949 + x687 * x958) + x681 * x956
                     + x682 * x959 + x685 * x945;
  const float x964 = x663 * x664;
  const float x965 = x143 * x663;
  const float x966 = x139 * x147;
  const float x967 = x667 * x966;
  const float x968 = x664 * x671;
  const float x969 = -x678;
  const float x970 = x134 * x967 + x674 * x968 + x676 * x964 + x965 * x969;
  const float x971 = x682 * x968 + x683 * x967 + x685 * x964 + x686 * x965;
  const float x972 = x664 * x674;
  const float x973 = x143 * x674;
  const float x974 = x650 * x966;
  const float x975 = x664 * x682;
  const float x976 = x676 * x975 + x683 * x974 + x685 * x972 + x686 * x973;

  out_3216172810630303951[0]
      = -x103 * x146 + x144 * x44
        + x147
              * (x157 * (x121 + x149 + x152) - 2 * x164 * x169
                 - x174 * (x171 + x173) + x180 * (x148 * x175 + x177)
                 + x181 * x183 + x185 * (-2 * x160 - 2 * x163)
                 - x187 * (-3.0 / 2.0 * x160 - 3.0 / 2.0 * x163))
        + 2 * x188 * x190
        + x230
              * (2 * x103 * x198 + x104 * (x191 + x192 + x46 - x54)
                 + x108 * (-x193 - x194 - x195 - x196)
                 + x229
                       * (8 * a_0 * b_1 * c_1 + 8 * a_0 * b_2 * c_2 + x200
                          + x201 + x203 + x204 - x206 - x207 - x208 - x209
                          + x210 + x212 + x213 + x215 - x216 - x217 - x218
                          - x219 - x224 - x227));
  out_3216172810630303951[1] = x321;
  out_3216172810630303951[2] = x394;
  out_3216172810630303951[3] = x453;
  out_3216172810630303951[4] = x512;
  out_3216172810630303951[5] = x565;
  out_3216172810630303951[6] = x597;
  out_3216172810630303951[7] = x629;
  out_3216172810630303951[8] = x660;
  out_3216172810630303951[9] = x672;
  out_3216172810630303951[10] = x679;
  out_3216172810630303951[11] = x688;
  out_3216172810630303951[12] = x321;
  out_3216172810630303951[13]
      = x147
            * (-x174 * (x173 + x693) + x180 * (x177 + x694) - 2 * x265 * x692
               + x278 * x286 + x281 * x691 + x288 * x695 - x289 * x696)
        + x230
              * (x108 * (-x194 - x699 - x701 - x703) + 2 * x259 * x704
                 + x292 * x698 + x320 * x705)
        + x249 * x689 + x256 * x690 + 2 * x279 * x697;
  out_3216172810630303951[14] = x713;
  out_3216172810630303951[15] = x719;
  out_3216172810630303951[16] = x727;
  out_3216172810630303951[17] = x737;
  out_3216172810630303951[18] = x739;
  out_3216172810630303951[19] = x744;
  out_3216172810630303951[20] = x751;
  out_3216172810630303951[21] = x754;
  out_3216172810630303951[22] = x755;
  out_3216172810630303951[23] = x756;
  out_3216172810630303951[24] = x394;
  out_3216172810630303951[25] = x713;
  out_3216172810630303951[26]
      = x147
            * (-x174 * (x171 + x693) + x180 * (x159 * x170 + x694)
               - 2 * x350 * x760 + x360 * x363 + x362 * x759 + x364 * x761
               - x365 * x762)
        + x230
              * (x108 * (-x193 - x699 - x765 - x766) + 2 * x345 * x767
                 + x368 * x764 + x393 * x768)
        + x340 * x757 + x346 * x758 + 2 * x361 * x763;
  out_3216172810630303951[27] = x778;
  out_3216172810630303951[28] = x784;
  out_3216172810630303951[29] = x785;
  out_3216172810630303951[30] = x788;
  out_3216172810630303951[31] = x790;
  out_3216172810630303951[32] = x792;
  out_3216172810630303951[33] = x795;
  out_3216172810630303951[34] = x796;
  out_3216172810630303951[35] = x797;
  out_3216172810630303951[36] = x453;
  out_3216172810630303951[37] = x719;
  out_3216172810630303951[38] = x778;
  out_3216172810630303951[39]
      = x147
            * (-x174 * (x802 + x803) + x180 * (x411 * x417 + x804)
               - 2 * x413 * x801 + x424 * x432 + x429 * x800 + x433 * x805
               - x434 * x806)
        + x230
              * (x108 * (-x195 - x810 - x812) + 2 * x426 * x813 + x436 * x808
                 + x452 * x814)
        + x406 * x798 + x410 * x799 + 2 * x425 * x807;
  out_3216172810630303951[40] = x823;
  out_3216172810630303951[41] = x825;
  out_3216172810630303951[42] = x830;
  out_3216172810630303951[43] = x832;
  out_3216172810630303951[44] = x835;
  out_3216172810630303951[45] = x838;
  out_3216172810630303951[46] = x839;
  out_3216172810630303951[47] = x840;
  out_3216172810630303951[48] = x512;
  out_3216172810630303951[49] = x727;
  out_3216172810630303951[50] = x784;
  out_3216172810630303951[51] = x823;
  out_3216172810630303951[52]
      = x147
            * (-x174 * (x803 + x846) + x180 * (x804 + x848) - 2 * x472 * x844
               + x481 * x491 + x486 * x843 + x492 * x849 - x493 * x850)
        + x230
              * (x108 * (-x701 - x810 - x854) + 2 * x470 * x855 + x495 * x852
                 + x511 * x856)
        + x465 * x841 + x471 * x842 + 2 * x482 * x851;
  out_3216172810630303951[53] = x863;
  out_3216172810630303951[54] = x865;
  out_3216172810630303951[55] = x869;
  out_3216172810630303951[56] = x871;
  out_3216172810630303951[57] = x874;
  out_3216172810630303951[58] = x875;
  out_3216172810630303951[59] = x876;
  out_3216172810630303951[60] = x565;
  out_3216172810630303951[61] = x737;
  out_3216172810630303951[62] = x785;
  out_3216172810630303951[63] = x825;
  out_3216172810630303951[64] = x863;
  out_3216172810630303951[65]
      = x147
            * (-x174 * (x802 + x846) + x180 * (x118 * x418 + x848)
               - 2 * x533 * x880 + x534 * x544 + x540 * x879 + x545 * x881
               - x546 * x882)
        + x230
              * (x108 * (-x765 - x812 - x854) + 2 * x537 * x885 + x548 * x884
                 + x564 * x886)
        + x523 * x877 + x526 * x878 + 2 * x535 * x883;
  out_3216172810630303951[66] = x892;
  out_3216172810630303951[67] = x894;
  out_3216172810630303951[68] = x895;
  out_3216172810630303951[69] = x898;
  out_3216172810630303951[70] = x899;
  out_3216172810630303951[71] = x900;
  out_3216172810630303951[72] = x597;
  out_3216172810630303951[73] = x739;
  out_3216172810630303951[74] = x788;
  out_3216172810630303951[75] = x830;
  out_3216172810630303951[76] = x865;
  out_3216172810630303951[77] = x892;
  out_3216172810630303951[78]
      = x147
            * (-x174 * (x905 + x907) + x180 * (x113 * x640 + x909)
               - 2 * x580 * x904 + x581 * x588 + x585 * x903 + x589 * x910
               - x590 * x911)
        + x230
              * (x108 * (-x196 - x914 - x915) + 2 * x570 * x916 + x592 * x913
                 + x596 * x917)
        + x566 * x901 + x571 * x902 + 2 * x582 * x912;
  out_3216172810630303951[79] = x925;
  out_3216172810630303951[80] = x926;
  out_3216172810630303951[81] = x929;
  out_3216172810630303951[82] = x930;
  out_3216172810630303951[83] = x931;
  out_3216172810630303951[84] = x629;
  out_3216172810630303951[85] = x744;
  out_3216172810630303951[86] = x790;
  out_3216172810630303951[87] = x832;
  out_3216172810630303951[88] = x869;
  out_3216172810630303951[89] = x894;
  out_3216172810630303951[90] = x925;
  out_3216172810630303951[91]
      = x147
            * (-x174 * (x907 + x936) + x180 * (x909 + x937) - 2 * x602 * x935
               + x611 * x620 + x617 * x934 + x621 * x938 - x622 * x939)
        + x230
              * (x108 * (-x703 - x914 - x942) + 2 * x613 * x943 + x624 * x941
                 + x628 * x944)
        + x598 * x932 + x600 * x933 + 2 * x612 * x940;
  out_3216172810630303951[92] = x950;
  out_3216172810630303951[93] = x953;
  out_3216172810630303951[94] = x954;
  out_3216172810630303951[95] = x955;
  out_3216172810630303951[96] = x660;
  out_3216172810630303951[97] = x751;
  out_3216172810630303951[98] = x792;
  out_3216172810630303951[99] = x835;
  out_3216172810630303951[100] = x871;
  out_3216172810630303951[101] = x895;
  out_3216172810630303951[102] = x926;
  out_3216172810630303951[103] = x950;
  out_3216172810630303951[104]
      = x143 * x630 * x646
        + x147
              * (-x174 * (x905 + x936) + x180 * (x576 * x634 + x937)
                 + x184 * x643 * x652 - x186 * x642 * x653 - 2 * x635 * x946
                 + x643 * x651 + x648 * x957)
        + x230
              * (x108 * (-x766 - x915 - x942) + x228 * x645 * x659
                 + 2 * x632 * x949 + x655 * x958)
        + x633 * x956 + 2 * x644 * x945;
  out_3216172810630303951[105] = x961;
  out_3216172810630303951[106] = x962;
  out_3216172810630303951[107] = x963;
  out_3216172810630303951[108] = x672;
  out_3216172810630303951[109] = x754;
  out_3216172810630303951[110] = x795;
  out_3216172810630303951[111] = x838;
  out_3216172810630303951[112] = x874;
  out_3216172810630303951[113] = x898;
  out_3216172810630303951[114] = x929;
  out_3216172810630303951[115] = x953;
  out_3216172810630303951[116] = x961;
  out_3216172810630303951[117] = x666 * x967 + x669 * x965 + 2 * x671 * x964;
  out_3216172810630303951[118] = x970;
  out_3216172810630303951[119] = x971;
  out_3216172810630303951[120] = x679;
  out_3216172810630303951[121] = x755;
  out_3216172810630303951[122] = x796;
  out_3216172810630303951[123] = x839;
  out_3216172810630303951[124] = x875;
  out_3216172810630303951[125] = x899;
  out_3216172810630303951[126] = x930;
  out_3216172810630303951[127] = x954;
  out_3216172810630303951[128] = x962;
  out_3216172810630303951[129] = x970;
  out_3216172810630303951[130] = x134 * x974 + 2 * x676 * x972 + x969 * x973;
  out_3216172810630303951[131] = x976;
  out_3216172810630303951[132] = x688;
  out_3216172810630303951[133] = x756;
  out_3216172810630303951[134] = x797;
  out_3216172810630303951[135] = x840;
  out_3216172810630303951[136] = x876;
  out_3216172810630303951[137] = x900;
  out_3216172810630303951[138] = x931;
  out_3216172810630303951[139] = x955;
  out_3216172810630303951[140] = x963;
  out_3216172810630303951[141] = x971;
  out_3216172810630303951[142] = x976;
  out_3216172810630303951[143]
      = x143 * x682 * x686 + x683 * x684 * x966 + 2 * x685 * x975;
}
