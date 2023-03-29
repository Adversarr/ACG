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
        * ((1.0 / 2.0) * ((d_h) * (d_h)) - d_h * fabs(x10) / sqrt(x9)
           + (1.0 / 2.0) * ((x10) * (x10)) / x9);
  return vertex_face_value_result;
}

void vertex_face_grad(double a_0, double a_1, double a_2, double b_0,
                      double b_1, double b_2, double c_0, double c_1,
                      double c_2, double d_0, double d_1, double d_2,
                      double d_h, double *out_1164389158182881131) {
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
  const double x77 = a_2 - d_2;
  const double x78 = -c_1;
  const double x79 = b_1 + x78;
  const double x80 = x77 * x79;
  const double x81 = a_1 - d_1;
  const double x82 = -x81;
  const double x83 = -c_2;
  const double x84 = b_2 + x83;
  const double x85 = x82 * x84;
  const double x86 = a_1 - b_1;
  const double x87 = a_2 + x83;
  const double x88 = x86 * x87;
  const double x89 = a_1 + x78;
  const double x90 = a_2 - b_2;
  const double x91 = x89 * x90;
  const double x92 = x88 - x91;
  const double x93 = a_0 - b_0;
  const double x94 = x89 * x93;
  const double x95 = -c_0;
  const double x96 = a_0 + x95;
  const double x97 = x86 * x96;
  const double x98 = x94 - x97;
  const double x99 = x87 * x93;
  const double x100 = x90 * x96;
  const double x101 = -x100 + x99;
  const double x102 = ((x101) * (x101)) + ((x92) * (x92)) + ((x98) * (x98));
  const double x103 = pow(x102, -1.0 / 2.0);
  const double x104 = a_0 - d_0;
  const double x105 = -x101 * x81 + x104 * x92 + x77 * x98;
  const double x106 = d_h * x103 * (((x105) > 0) - ((x105) < 0));
  const double x107 = 2 * x88 - 2 * x91;
  const double x108 = 1.0 / x102;
  const double x109 = -x16;
  const double x110 = x109 + x14;
  const double x111 = x110 * x98;
  const double x112 = -x23;
  const double x113 = x112 + x21;
  const double x114 = x101 * x113;
  const double x115 = d_h * fabs(x105);
  const double x116 = x115 / pow(x102, 3.0 / 2.0);
  const double x117 = pow(x102, -2);
  const double x118 = ((x105) * (x105));
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
      = (1.0 / 2.0) * ((d_h) * (d_h)) - x103 * x115 + (1.0 / 2.0) * x108 * x118;
  const double x139 = x138 * x74 / x13;
  const double x140 = x104 * x84;
  const double x141 = -b_0 - x95;
  const double x142 = x141 * x77;
  const double x143 = 2 * x99;
  const double x144 = 2 * x100;
  const double x145 = -x57;
  const double x146 = -x145 - x54;
  const double x147 = x146 * x98;
  const double x148 = x113 * x92;
  const double x149 = d_2 * x54;
  const double x150 = d_0 * x21;
  const double x151 = -d_0 * x23 + d_2 * x57;
  const double x152 = -x25 + x29;
  const double x153 = 2 * a_1;
  const double x154 = x153 * x51;
  const double x155 = x153 * x47;
  const double x156 = x153 * x52;
  const double x157 = x153 * x49;
  const double x158 = b_0 * x15;
  const double x159 = b_2 * x32;
  const double x160 = b_0 * x18;
  const double x161 = c_0 * x28;
  const double x162 = b_2 * x33;
  const double x163 = a_0 * x28 + a_2 * x40;
  const double x164 = a_0 * x18 + x34;
  const double x165 = -x104 * x79;
  const double x166 = x141 * x82;
  const double x167 = 2 * x94 - 2 * x97;
  const double x168 = x101 * x146;
  const double x169 = -x110 * x92;
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
  const double x188 = -x77 * x89;
  const double x189 = -x87;
  const double x190 = x189 * x82;
  const double x191 = x109 + x153;
  const double x192 = -x191 * x98;
  const double x193 = -x112 - x174;
  const double x194 = x101 * x193;
  const double x195 = a_1 * x23;
  const double x196 = d_1 * x174 - d_2 * x153;
  const double x197 = x50 * x57;
  const double x198 = x53 * x57;
  const double x199 = a_1 * x15;
  const double x200 = a_2 * x22;
  const double x201 = a_1 * x17;
  const double x202 = a_2 * x24;
  const double x203 = x0 * x16 + x1 * x23;
  const double x204 = x77 * x96;
  const double x205 = x104 * x189;
  const double x206 = x124 + x145;
  const double x207 = x206 * x98;
  const double x208 = x193 * x92;
  const double x209 = a_2 * x57;
  const double x210 = d_0 * x174 - d_2 * x124;
  const double x211 = x14 * x45;
  const double x212 = x14 * x53;
  const double x213 = a_1 * x54;
  const double x214 = a_0 * x213;
  const double x215 = a_1 * x21;
  const double x216 = a_2 * x215;
  const double x217 = a_1 * x57;
  const double x218 = a_0 * x217;
  const double x219 = a_2 * x195;
  const double x220 = c_0 * x213 + x23 * x4;
  const double x221 = x82 * x96;
  const double x222 = x104 * x89;
  const double x223 = x101 * x206;
  const double x224 = x191 * x92;
  const double x225 = d_0 * x153 - d_1 * x124;
  const double x226 = x21 * x45;
  const double x227 = x21 * x50;
  const double x228 = a_2 * x54;
  const double x229 = a_0 * x228;
  const double x230 = a_1 * x32;
  const double x231 = a_0 * x209;
  const double x232 = a_1 * x35;
  const double x233 = c_0 * x228 + x16 * x7;
  const double x234 = x82 * x90;
  const double x235 = x77 * x86;
  const double x236 = -x14 + x153;
  const double x237 = x236 * x98;
  const double x238 = x174 - x21;
  const double x239 = x101 * x238;
  const double x240 = x215 - x32;
  const double x241 = -x93;
  const double x242 = x241 * x77;
  const double x243 = x104 * x90;
  const double x244 = -x124 + x54;
  const double x245 = x244 * x98;
  const double x246 = x238 * x92;
  const double x247 = x22 - x228;
  const double x248 = x241 * x82;
  const double x249 = -x104 * x86;
  const double x250 = x101 * x244;
  const double x251 = -x236 * x92;
  const double x252 = x15 - x213;
  const double x253 = x138 / x12;

  out_1164389158182881131[0]
      = x139
            * (x123 * (-x119 + x120 + x121 + x122)
               + x137
                     * (4 * a_0 * b_1 * c_1 + 4 * a_0 * b_2 * c_2 - x125 - x126
                        - x127 - x128 + x129 + x130 + x131 + x132 - x133 - x134
                        - x135 - x136 - x42 - x43 + x55 + x56 + x58 + x59))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (x107 + 2 * x80 + 2 * x85)
                 - x106 * (x80 + x85 + x92)
                 - x116 * (-1.0 / 2.0 * x111 - 1.0 / 2.0 * x114)
                 + (1.0 / 2.0) * x117 * x118 * (-x111 - x114));
  out_1164389158182881131[1]
      = x139
            * (x123 * (x149 - x150 - x151 - x152)
               + x137
                     * (4 * a_1 * b_0 * c_0 + 4 * a_1 * b_2 * c_2 - x154 - x155
                        - x156 - x157 + x158 + x159 - x160 - x161 - x162 - x163
                        - x164 + x20 + x36 - x44 + x62 + x63 + x64 + x65))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x140 + 2 * x142 - x143 + x144)
                 - x106 * (x100 + x140 + x142 - x99)
                 - x116 * (-1.0 / 2.0 * x147 - 1.0 / 2.0 * x148)
                 + (1.0 / 2.0) * x117 * x118 * (-x147 - x148));
  out_1164389158182881131[2]
      = x139
            * (x123 * (-x170 + x171 + x172 + x173)
               + x137
                     * (4 * a_2 * b_0 * c_0 + 4 * a_2 * b_1 * c_1 - x175 - x176
                        - x177 - x178 + x179 + x180 + x181 - x182 - x183 - x184
                        - x185 - x186 - x187 + x27 + x68 + x69 + x70 + x71))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x165 + 2 * x166 + x167)
                 - x106 * (x165 + x166 + x98)
                 - x116 * (-1.0 / 2.0 * x168 - 1.0 / 2.0 * x169)
                 + (1.0 / 2.0) * x117 * x118 * (-x168 - x169));
  out_1164389158182881131[3]
      = x139
            * (x123 * (-x121 - x195 - x196 + x35)
               + x137
                     * (4 * a_1 * b_0 * c_1 + 4 * a_2 * b_0 * c_2 + x127 + x128
                        - x130 - x132 + x133 + x134 - x135 + x197 + x198 + x199
                        + x200 - x201 - x202 - x203 - x55 - x56 - x66 - x72))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x188 + 2 * x190)
                 - x106 * (x188 + x190)
                 - x116 * (-1.0 / 2.0 * x192 - 1.0 / 2.0 * x194)
                 + (1.0 / 2.0) * x117 * x118 * (-x192 - x194));
  out_1164389158182881131[4]
      = x139
            * (x123 * (x151 - x209 + x210 + x24)
               + x137
                     * (4 * a_0 * b_1 * c_0 + 4 * a_2 * b_1 * c_2 + x156 + x157
                        + x161 - x163 - x20 - x211 - x212 + x214 + x216 - x218
                        - x219 - x220 - x36 + x44 + x60 - x63 - x64 + x73))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x204 + 2 * x205)
                 - x106 * (x204 + x205)
                 - x116 * (-1.0 / 2.0 * x207 - 1.0 / 2.0 * x208)
                 + (1.0 / 2.0) * x117 * x118 * (-x207 - x208));
  out_1164389158182881131[5]
      = x139
            * (x123 * (-x17 - x172 + x217 - x225)
               + x137
                     * (4 * a_0 * b_2 * c_0 + 4 * a_1 * b_2 * c_1 + x177 + x178
                        - x181 + x183 + x185 - x186 - x226 - x227 + x229 + x230
                        - x231 - x232 - x233 - x27 + x61 + x67 - x70 - x71))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x221 + 2 * x222)
                 - x106 * (x221 + x222)
                 - x116 * (-1.0 / 2.0 * x223 - 1.0 / 2.0 * x224)
                 + (1.0 / 2.0) * x117 * x118 * (-x223 - x224));
  out_1164389158182881131[6]
      = x139
            * (x123 * (x119 - x120 + x196 + x240)
               + x137
                     * (4 * a_1 * b_1 * c_0 + 4 * a_2 * b_2 * c_0 + x125 + x126
                        - x129 - x131 - x136 - x197 - x198 - x199 - x200 + x201
                        + x202 - x203 + x42 + x43 - x58 - x59 + x66 + x72))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x234 + 2 * x235)
                 - x106 * (x234 + x235)
                 - x116 * (-1.0 / 2.0 * x237 - 1.0 / 2.0 * x239)
                 + (1.0 / 2.0) * x117 * x118 * (-x237 - x239));
  out_1164389158182881131[7]
      = x139
            * (x123 * (-x149 + x150 - x210 - x247)
               + x137
                     * (4 * a_0 * b_0 * c_1 + 4 * a_2 * b_2 * c_1 + x154 + x155
                        - x158 - x159 + x160 + x162 - x164 + x211 + x212 - x214
                        - x216 + x218 + x219 - x220 - x60 - x62 - x65 - x73))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x242 + 2 * x243)
                 - x106 * (x242 + x243)
                 - x116 * (-1.0 / 2.0 * x245 - 1.0 / 2.0 * x246)
                 + (1.0 / 2.0) * x117 * x118 * (-x245 - x246));
  out_1164389158182881131[8]
      = x139
            * (x123 * (x170 - x171 + x225 + x252)
               + x137
                     * (4 * a_0 * b_0 * c_2 + 4 * a_1 * b_1 * c_2 + x175 + x176
                        - x179 - x180 + x182 + x184 - x187 + x226 + x227 - x229
                        - x230 + x231 + x232 - x233 - x61 - x67 - x68 - x69))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x248 + 2 * x249)
                 - x106 * (x248 + x249)
                 - x116 * (-1.0 / 2.0 * x250 - 1.0 / 2.0 * x251)
                 + (1.0 / 2.0) * x117 * x118 * (-x250 - x251));
  out_1164389158182881131[9]
      = x253 * (-x122 + x195 - x240 - x35)
        + x76 * (-1.0 / 2.0 * x105 * x107 * x108 + x106 * x92);
  out_1164389158182881131[10]
      = x253 * (x152 + x209 - x24 + x247)
        + x76 * (-x101 * x106 + (1.0 / 2.0) * x105 * x108 * (x143 - x144));
  out_1164389158182881131[11]
      = x253 * (x17 - x173 - x217 - x252)
        + x76 * (-1.0 / 2.0 * x105 * x108 * x167 + x106 * x98);
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
        * ((1.0 / 2.0) * ((d_h) * (d_h)) - d_h * fabs(x10) / sqrt(x9)
           + (1.0 / 2.0) * ((x10) * (x10)) / x9);
  return vertex_face_value_result;
}

void vertex_face_grad(float a_0, float a_1, float a_2, float b_0, float b_1,
                      float b_2, float c_0, float c_1, float c_2, float d_0,
                      float d_1, float d_2, float d_h,
                      float *out_1164389158182881131) {
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
  const float x77 = a_2 - d_2;
  const float x78 = -c_1;
  const float x79 = b_1 + x78;
  const float x80 = x77 * x79;
  const float x81 = a_1 - d_1;
  const float x82 = -x81;
  const float x83 = -c_2;
  const float x84 = b_2 + x83;
  const float x85 = x82 * x84;
  const float x86 = a_1 - b_1;
  const float x87 = a_2 + x83;
  const float x88 = x86 * x87;
  const float x89 = a_1 + x78;
  const float x90 = a_2 - b_2;
  const float x91 = x89 * x90;
  const float x92 = x88 - x91;
  const float x93 = a_0 - b_0;
  const float x94 = x89 * x93;
  const float x95 = -c_0;
  const float x96 = a_0 + x95;
  const float x97 = x86 * x96;
  const float x98 = x94 - x97;
  const float x99 = x87 * x93;
  const float x100 = x90 * x96;
  const float x101 = -x100 + x99;
  const float x102 = ((x101) * (x101)) + ((x92) * (x92)) + ((x98) * (x98));
  const float x103 = pow(x102, -1.0 / 2.0);
  const float x104 = a_0 - d_0;
  const float x105 = -x101 * x81 + x104 * x92 + x77 * x98;
  const float x106 = d_h * x103 * (((x105) > 0) - ((x105) < 0));
  const float x107 = 2 * x88 - 2 * x91;
  const float x108 = 1.0 / x102;
  const float x109 = -x16;
  const float x110 = x109 + x14;
  const float x111 = x110 * x98;
  const float x112 = -x23;
  const float x113 = x112 + x21;
  const float x114 = x101 * x113;
  const float x115 = d_h * fabs(x105);
  const float x116 = x115 / pow(x102, 3.0 / 2.0);
  const float x117 = pow(x102, -2);
  const float x118 = ((x105) * (x105));
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
      = (1.0 / 2.0) * ((d_h) * (d_h)) - x103 * x115 + (1.0 / 2.0) * x108 * x118;
  const float x139 = x138 * x74 / x13;
  const float x140 = x104 * x84;
  const float x141 = -b_0 - x95;
  const float x142 = x141 * x77;
  const float x143 = 2 * x99;
  const float x144 = 2 * x100;
  const float x145 = -x57;
  const float x146 = -x145 - x54;
  const float x147 = x146 * x98;
  const float x148 = x113 * x92;
  const float x149 = d_2 * x54;
  const float x150 = d_0 * x21;
  const float x151 = -d_0 * x23 + d_2 * x57;
  const float x152 = -x25 + x29;
  const float x153 = 2 * a_1;
  const float x154 = x153 * x51;
  const float x155 = x153 * x47;
  const float x156 = x153 * x52;
  const float x157 = x153 * x49;
  const float x158 = b_0 * x15;
  const float x159 = b_2 * x32;
  const float x160 = b_0 * x18;
  const float x161 = c_0 * x28;
  const float x162 = b_2 * x33;
  const float x163 = a_0 * x28 + a_2 * x40;
  const float x164 = a_0 * x18 + x34;
  const float x165 = -x104 * x79;
  const float x166 = x141 * x82;
  const float x167 = 2 * x94 - 2 * x97;
  const float x168 = x101 * x146;
  const float x169 = -x110 * x92;
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
  const float x188 = -x77 * x89;
  const float x189 = -x87;
  const float x190 = x189 * x82;
  const float x191 = x109 + x153;
  const float x192 = -x191 * x98;
  const float x193 = -x112 - x174;
  const float x194 = x101 * x193;
  const float x195 = a_1 * x23;
  const float x196 = d_1 * x174 - d_2 * x153;
  const float x197 = x50 * x57;
  const float x198 = x53 * x57;
  const float x199 = a_1 * x15;
  const float x200 = a_2 * x22;
  const float x201 = a_1 * x17;
  const float x202 = a_2 * x24;
  const float x203 = x0 * x16 + x1 * x23;
  const float x204 = x77 * x96;
  const float x205 = x104 * x189;
  const float x206 = x124 + x145;
  const float x207 = x206 * x98;
  const float x208 = x193 * x92;
  const float x209 = a_2 * x57;
  const float x210 = d_0 * x174 - d_2 * x124;
  const float x211 = x14 * x45;
  const float x212 = x14 * x53;
  const float x213 = a_1 * x54;
  const float x214 = a_0 * x213;
  const float x215 = a_1 * x21;
  const float x216 = a_2 * x215;
  const float x217 = a_1 * x57;
  const float x218 = a_0 * x217;
  const float x219 = a_2 * x195;
  const float x220 = c_0 * x213 + x23 * x4;
  const float x221 = x82 * x96;
  const float x222 = x104 * x89;
  const float x223 = x101 * x206;
  const float x224 = x191 * x92;
  const float x225 = d_0 * x153 - d_1 * x124;
  const float x226 = x21 * x45;
  const float x227 = x21 * x50;
  const float x228 = a_2 * x54;
  const float x229 = a_0 * x228;
  const float x230 = a_1 * x32;
  const float x231 = a_0 * x209;
  const float x232 = a_1 * x35;
  const float x233 = c_0 * x228 + x16 * x7;
  const float x234 = x82 * x90;
  const float x235 = x77 * x86;
  const float x236 = -x14 + x153;
  const float x237 = x236 * x98;
  const float x238 = x174 - x21;
  const float x239 = x101 * x238;
  const float x240 = x215 - x32;
  const float x241 = -x93;
  const float x242 = x241 * x77;
  const float x243 = x104 * x90;
  const float x244 = -x124 + x54;
  const float x245 = x244 * x98;
  const float x246 = x238 * x92;
  const float x247 = x22 - x228;
  const float x248 = x241 * x82;
  const float x249 = -x104 * x86;
  const float x250 = x101 * x244;
  const float x251 = -x236 * x92;
  const float x252 = x15 - x213;
  const float x253 = x138 / x12;

  out_1164389158182881131[0]
      = x139
            * (x123 * (-x119 + x120 + x121 + x122)
               + x137
                     * (4 * a_0 * b_1 * c_1 + 4 * a_0 * b_2 * c_2 - x125 - x126
                        - x127 - x128 + x129 + x130 + x131 + x132 - x133 - x134
                        - x135 - x136 - x42 - x43 + x55 + x56 + x58 + x59))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (x107 + 2 * x80 + 2 * x85)
                 - x106 * (x80 + x85 + x92)
                 - x116 * (-1.0 / 2.0 * x111 - 1.0 / 2.0 * x114)
                 + (1.0 / 2.0) * x117 * x118 * (-x111 - x114));
  out_1164389158182881131[1]
      = x139
            * (x123 * (x149 - x150 - x151 - x152)
               + x137
                     * (4 * a_1 * b_0 * c_0 + 4 * a_1 * b_2 * c_2 - x154 - x155
                        - x156 - x157 + x158 + x159 - x160 - x161 - x162 - x163
                        - x164 + x20 + x36 - x44 + x62 + x63 + x64 + x65))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x140 + 2 * x142 - x143 + x144)
                 - x106 * (x100 + x140 + x142 - x99)
                 - x116 * (-1.0 / 2.0 * x147 - 1.0 / 2.0 * x148)
                 + (1.0 / 2.0) * x117 * x118 * (-x147 - x148));
  out_1164389158182881131[2]
      = x139
            * (x123 * (-x170 + x171 + x172 + x173)
               + x137
                     * (4 * a_2 * b_0 * c_0 + 4 * a_2 * b_1 * c_1 - x175 - x176
                        - x177 - x178 + x179 + x180 + x181 - x182 - x183 - x184
                        - x185 - x186 - x187 + x27 + x68 + x69 + x70 + x71))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x165 + 2 * x166 + x167)
                 - x106 * (x165 + x166 + x98)
                 - x116 * (-1.0 / 2.0 * x168 - 1.0 / 2.0 * x169)
                 + (1.0 / 2.0) * x117 * x118 * (-x168 - x169));
  out_1164389158182881131[3]
      = x139
            * (x123 * (-x121 - x195 - x196 + x35)
               + x137
                     * (4 * a_1 * b_0 * c_1 + 4 * a_2 * b_0 * c_2 + x127 + x128
                        - x130 - x132 + x133 + x134 - x135 + x197 + x198 + x199
                        + x200 - x201 - x202 - x203 - x55 - x56 - x66 - x72))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x188 + 2 * x190)
                 - x106 * (x188 + x190)
                 - x116 * (-1.0 / 2.0 * x192 - 1.0 / 2.0 * x194)
                 + (1.0 / 2.0) * x117 * x118 * (-x192 - x194));
  out_1164389158182881131[4]
      = x139
            * (x123 * (x151 - x209 + x210 + x24)
               + x137
                     * (4 * a_0 * b_1 * c_0 + 4 * a_2 * b_1 * c_2 + x156 + x157
                        + x161 - x163 - x20 - x211 - x212 + x214 + x216 - x218
                        - x219 - x220 - x36 + x44 + x60 - x63 - x64 + x73))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x204 + 2 * x205)
                 - x106 * (x204 + x205)
                 - x116 * (-1.0 / 2.0 * x207 - 1.0 / 2.0 * x208)
                 + (1.0 / 2.0) * x117 * x118 * (-x207 - x208));
  out_1164389158182881131[5]
      = x139
            * (x123 * (-x17 - x172 + x217 - x225)
               + x137
                     * (4 * a_0 * b_2 * c_0 + 4 * a_1 * b_2 * c_1 + x177 + x178
                        - x181 + x183 + x185 - x186 - x226 - x227 + x229 + x230
                        - x231 - x232 - x233 - x27 + x61 + x67 - x70 - x71))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x221 + 2 * x222)
                 - x106 * (x221 + x222)
                 - x116 * (-1.0 / 2.0 * x223 - 1.0 / 2.0 * x224)
                 + (1.0 / 2.0) * x117 * x118 * (-x223 - x224));
  out_1164389158182881131[6]
      = x139
            * (x123 * (x119 - x120 + x196 + x240)
               + x137
                     * (4 * a_1 * b_1 * c_0 + 4 * a_2 * b_2 * c_0 + x125 + x126
                        - x129 - x131 - x136 - x197 - x198 - x199 - x200 + x201
                        + x202 - x203 + x42 + x43 - x58 - x59 + x66 + x72))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x234 + 2 * x235)
                 - x106 * (x234 + x235)
                 - x116 * (-1.0 / 2.0 * x237 - 1.0 / 2.0 * x239)
                 + (1.0 / 2.0) * x117 * x118 * (-x237 - x239));
  out_1164389158182881131[7]
      = x139
            * (x123 * (-x149 + x150 - x210 - x247)
               + x137
                     * (4 * a_0 * b_0 * c_1 + 4 * a_2 * b_2 * c_1 + x154 + x155
                        - x158 - x159 + x160 + x162 - x164 + x211 + x212 - x214
                        - x216 + x218 + x219 - x220 - x60 - x62 - x65 - x73))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x242 + 2 * x243)
                 - x106 * (x242 + x243)
                 - x116 * (-1.0 / 2.0 * x245 - 1.0 / 2.0 * x246)
                 + (1.0 / 2.0) * x117 * x118 * (-x245 - x246));
  out_1164389158182881131[8]
      = x139
            * (x123 * (x170 - x171 + x225 + x252)
               + x137
                     * (4 * a_0 * b_0 * c_2 + 4 * a_1 * b_1 * c_2 + x175 + x176
                        - x179 - x180 + x182 + x184 - x187 + x226 + x227 - x229
                        - x230 + x231 + x232 - x233 - x61 - x67 - x68 - x69))
        + x76
              * ((1.0 / 2.0) * x105 * x108 * (2 * x248 + 2 * x249)
                 - x106 * (x248 + x249)
                 - x116 * (-1.0 / 2.0 * x250 - 1.0 / 2.0 * x251)
                 + (1.0 / 2.0) * x117 * x118 * (-x250 - x251));
  out_1164389158182881131[9]
      = x253 * (-x122 + x195 - x240 - x35)
        + x76 * (-1.0 / 2.0 * x105 * x107 * x108 + x106 * x92);
  out_1164389158182881131[10]
      = x253 * (x152 + x209 - x24 + x247)
        + x76 * (-x101 * x106 + (1.0 / 2.0) * x105 * x108 * (x143 - x144));
  out_1164389158182881131[11]
      = x253 * (x17 - x173 - x217 - x252)
        + x76 * (-1.0 / 2.0 * x105 * x108 * x167 + x106 * x98);
}
