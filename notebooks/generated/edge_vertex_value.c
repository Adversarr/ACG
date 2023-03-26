/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'acgipc'                        *
 ******************************************************************************/
#include "edge_vertex_value.h"
#include <math.h>

double edge_vertex_value(double a_0, double a_1, double a_2, double b_0, double b_1, double b_2, double c_0, double c_1, double c_2, double d_h) {
   const double x0 = a_0 - b_0;
   const double x1 = a_1 - b_1;
   const double x2 = a_2 - b_2;
   const double x3 = ((x0) * (x0)) + ((x1) * (x1)) + ((x2) * (x2));
   const double x4 = a_1 - c_1;
   const double x5 = a_0 - c_0;
   const double x6 = a_2 - c_2;
   const double x7 = ((x0*x4 - x1*x5) * (x0*x4 - x1*x5)) + ((x0*x6 - x2*x5) * (x0*x6 - x2*x5)) + ((x1*x6 - x2*x4) * (x1*x6 - x2*x4));
   const double x8 = ((a_0) * (a_0));
   const double x9 = ((a_1) * (a_1));
   const double x10 = ((a_2) * (a_2));
   const double x11 = ((b_0) * (b_0));
   const double x12 = ((b_1) * (b_1));
   const double x13 = ((b_2) * (b_2));
   const double x14 = 2*a_0;
   const double x15 = b_0*x14;
   const double x16 = 2*a_1;
   const double x17 = b_1*x16;
   const double x18 = 2*a_2;
   const double x19 = b_2*x18;
   const double x20 = a_1*x15;
   const double x21 = c_0*x14;
   const double x22 = a_1*x21;
   const double x23 = a_2*b_2;
   const double x24 = a_2*c_2;
   const double x25 = b_1*c_1;
   const double x26 = b_2*c_2;
   const double x27 = c_1*x16;
   const double x28 = b_0*c_0;
   const double x29 = c_2*x18;
   const double x30 = 2*x28;
   const double x31 = ((c_1) * (c_1));
   const double x32 = ((c_2) * (c_2));
   const double x33 = ((c_0) * (c_0));
   const double x34 = 2*x8;
   const double x35 = 2*x9;
   const double x36 = 2*x10;

   double edge_vertex_value_result;
   edge_vertex_value_result = (-2*log(d_h) + log((-b_1*x20 + b_1*x22 + c_1*x20 - c_1*x22 + x10*x11 + x10*x12 + x10*x31 + x10*x33 - x11*x27 - x11*x29 + x11*x31 + x11*x32 + x11*x9 - x12*x21 - x12*x29 + x12*x32 + x12*x33 + x12*x8 - x13*x21 - x13*x27 + x13*x31 + x13*x33 + x13*x8 + x13*x9 - x15*x23 + x15*x24 + x15*x25 + x15*x26 - x15*x31 - x15*x32 - x17*x23 + x17*x24 + x17*x26 + x17*x28 - x17*x32 - x17*x33 + x19*x25 + x19*x28 - x19*x31 - x19*x33 + x21*x23 - x21*x24 + x21*x25 + x21*x26 + x23*x27 - x24*x27 - 2*x25*x26 + x25*x29 - x25*x30 - x25*x34 - x25*x36 + x26*x27 - x26*x30 - x26*x34 - x26*x35 + x27*x28 + x28*x29 - x28*x35 - x28*x36 + x31*x8 + x32*x8 + x32*x9 + x33*x9)/(x10 + x11 + x12 + x13 - x15 - x17 - x19 + x8 + x9)))*((1.0/2.0)*((d_h) * (d_h)) - d_h*sqrt(x7)/sqrt(x3) + (1.0/2.0)*x7/x3);
   return edge_vertex_value_result;

}

void edge_vertex_grad(double a_0, double a_1, double a_2, double b_0, double b_1, double b_2, double c_0, double c_1, double c_2, double d_h, double *out_5229921342576304612) {
   const double x0 = ((a_0) * (a_0));
   const double x1 = ((a_1) * (a_1));
   const double x2 = ((a_2) * (a_2));
   const double x3 = ((b_0) * (b_0));
   const double x4 = ((b_1) * (b_1));
   const double x5 = ((b_2) * (b_2));
   const double x6 = 2*a_0;
   const double x7 = b_0*x6;
   const double x8 = 2*b_1;
   const double x9 = a_1*x8;
   const double x10 = 2*b_2;
   const double x11 = a_2*x10;
   const double x12 = x0 + x1 - x11 + x2 + x3 + x4 + x5 - x7 - x9;
   const double x13 = 1.0/x12;
   const double x14 = b_0*x9;
   const double x15 = 2*c_1;
   const double x16 = a_1*x15;
   const double x17 = b_0*x16;
   const double x18 = c_0*x9;
   const double x19 = c_0*x16;
   const double x20 = b_0*x11;
   const double x21 = 2*c_2;
   const double x22 = a_2*x21;
   const double x23 = b_0*x22;
   const double x24 = c_0*x11;
   const double x25 = c_0*x22;
   const double x26 = b_1*x15;
   const double x27 = b_0*x26;
   const double x28 = b_2*x21;
   const double x29 = b_0*x28;
   const double x30 = c_0*x26;
   const double x31 = c_0*x28;
   const double x32 = b_2*x9;
   const double x33 = c_2*x9;
   const double x34 = b_2*x16;
   const double x35 = c_2*x16;
   const double x36 = b_2*c_2;
   const double x37 = b_2*x26;
   const double x38 = c_2*x26;
   const double x39 = ((c_1) * (c_1));
   const double x40 = ((c_2) * (c_2));
   const double x41 = ((c_0) * (c_0));
   const double x42 = x4*x6;
   const double x43 = x5*x6;
   const double x44 = x0*x15;
   const double x45 = x0*x21;
   const double x46 = x15*x3;
   const double x47 = x15*x5;
   const double x48 = 2*b_0;
   const double x49 = x1*x48;
   const double x50 = x1*x21;
   const double x51 = x21*x3;
   const double x52 = x21*x4;
   const double x53 = x2*x48;
   const double x54 = x15*x2;
   const double x55 = -a_0*x14 + a_0*x17 + a_0*x18 - a_0*x19 - a_0*x20 + a_0*x23 + a_0*x24 - a_0*x25 + a_0*x27 + a_0*x29 + a_0*x30 + a_0*x31 - a_1*x46 - a_1*x47 - a_2*x32 + a_2*x33 + a_2*x34 - a_2*x35 + a_2*x37 + a_2*x38 - a_2*x51 - a_2*x52 + b_0*x19 + b_0*x25 - b_1*x44 - b_1*x54 - b_2*x45 - b_2*x50 + c_0*x14 + c_0*x20 - c_0*x27 - c_0*x29 - c_0*x42 - c_0*x43 - c_0*x49 - c_0*x53 + x0*x39 + x0*x4 + x0*x40 + x0*x5 + x1*x3 + x1*x40 + x1*x41 + x1*x5 - x11*x39 - x11*x41 + x16*x36 + x2*x3 + x2*x39 + x2*x4 + x2*x41 - x26*x36 + x3*x39 + x3*x40 + x36*x9 + x39*x5 - x39*x7 + x4*x40 + x4*x41 - x40*x7 - x40*x9 + x41*x5 - x41*x9;
   const double x56 = -2*log(d_h) + log(x13*x55);
   const double x57 = a_0 - b_0;
   const double x58 = a_1 - b_1;
   const double x59 = a_2 - b_2;
   const double x60 = ((x57) * (x57)) + ((x58) * (x58)) + ((x59) * (x59));
   const double x61 = 1.0/x60;
   const double x62 = -x15;
   const double x63 = x62 + x8;
   const double x64 = a_1 - c_1;
   const double x65 = a_0 - c_0;
   const double x66 = x57*x64 - x58*x65;
   const double x67 = x63*x66;
   const double x68 = -x21;
   const double x69 = x10 + x68;
   const double x70 = a_2 - c_2;
   const double x71 = x57*x70 - x59*x65;
   const double x72 = x69*x71;
   const double x73 = -x48 + x6;
   const double x74 = -x73;
   const double x75 = pow(x60, -2);
   const double x76 = x58*x70 - x59*x64;
   const double x77 = ((x66) * (x66)) + ((x71) * (x71)) + ((x76) * (x76));
   const double x78 = sqrt(x77);
   const double x79 = d_h*x78;
   const double x80 = x79/pow(x60, 3.0/2.0);
   const double x81 = pow(x60, -1.0/2.0);
   const double x82 = d_h*x81/x78;
   const double x83 = x39*x48;
   const double x84 = x40*x48;
   const double x85 = 2*c_0;
   const double x86 = x4*x85;
   const double x87 = x5*x85;
   const double x88 = x39*x6;
   const double x89 = x40*x6;
   const double x90 = 4*a_0;
   const double x91 = b_1*c_1;
   const double x92 = x18 + x24;
   const double x93 = x17 + x23;
   const double x94 = x55/((x12) * (x12));
   const double x95 = ((1.0/2.0)*((d_h) * (d_h)) + (1.0/2.0)*x61*x77 - x79*x81)/x55;
   const double x96 = x12*x95;
   const double x97 = -x85;
   const double x98 = -x48 - x97;
   const double x99 = x66*x98;
   const double x100 = x69*x76;
   const double x101 = 2*a_1;
   const double x102 = x101 - x8;
   const double x103 = -x102;
   const double x104 = x41*x8;
   const double x105 = x40*x8;
   const double x106 = x101*x3;
   const double x107 = x101*x5;
   const double x108 = x101*x41;
   const double x109 = x101*x40;
   const double x110 = 4*a_1;
   const double x111 = b_0*c_0;
   const double x112 = a_0*b_0;
   const double x113 = x112*x8;
   const double x114 = a_0*c_0;
   const double x115 = x114*x15;
   const double x116 = a_2*x8;
   const double x117 = b_2*x116;
   const double x118 = a_2*x15;
   const double x119 = c_2*x118;
   const double x120 = x111*x8;
   const double x121 = x111*x15;
   const double x122 = x36*x8;
   const double x123 = x15*x36;
   const double x124 = b_2*x118 + x112*x15;
   const double x125 = c_2*x116 + x114*x8;
   const double x126 = x71*x98;
   const double x127 = -x63*x76;
   const double x128 = 2*a_2;
   const double x129 = -2*b_2 + x128;
   const double x130 = -x129;
   const double x131 = x10*x41;
   const double x132 = x10*x39;
   const double x133 = x128*x3;
   const double x134 = x128*x4;
   const double x135 = x128*x41;
   const double x136 = x128*x39;
   const double x137 = 4*a_2;
   const double x138 = x10*x112;
   const double x139 = x114*x21;
   const double x140 = x10*x111;
   const double x141 = x111*x21;
   const double x142 = x112*x21 + x33;
   const double x143 = x10*x114 + x34;
   const double x144 = x101 + x62;
   const double x145 = -x144*x66;
   const double x146 = -x128 - x68;
   const double x147 = x146*x71;
   const double x148 = x1*x85;
   const double x149 = x2*x85;
   const double x150 = c_1*x110;
   const double x151 = c_2*x137;
   const double x152 = a_0*x9;
   const double x153 = a_0*x11;
   const double x154 = a_0*x16;
   const double x155 = a_0*x22;
   const double x156 = a_0*x26 + a_0*x28;
   const double x157 = x6 + x97;
   const double x158 = x157*x66;
   const double x159 = x146*x76;
   const double x160 = x0*x8;
   const double x161 = x2*x8;
   const double x162 = c_0*x90;
   const double x163 = a_1*x7;
   const double x164 = a_1*x11;
   const double x165 = a_1*c_0;
   const double x166 = x165*x6;
   const double x167 = a_1*x22;
   const double x168 = a_1*x28 + x165*x48;
   const double x169 = x157*x71;
   const double x170 = x144*x76;
   const double x171 = x0*x10;
   const double x172 = x1*x10;
   const double x173 = a_2*x7;
   const double x174 = a_2*x9;
   const double x175 = a_2*c_0;
   const double x176 = x175*x6;
   const double x177 = a_2*x16;
   const double x178 = a_2*x26 + x175*x48;
   const double x179 = x102*x66;
   const double x180 = x129*x71;
   const double x181 = b_1*x110;
   const double x182 = b_2*x137;
   const double x183 = x66*x74;
   const double x184 = x129*x76;
   const double x185 = b_0*x90;
   const double x186 = x71*x74;
   const double x187 = x103*x76;

   out_5229921342576304612[0] = x56*(x57*x80 + (1.0/2.0)*x61*(x67 + x72) + (1.0/2.0)*x74*x75*x77 - x82*((1.0/2.0)*x67 + (1.0/2.0)*x72)) + x96*(x13*(-x14 - x19 - x20 - x25 + x27 + x29 + x30 + x31 - x36*x90 + x42 + x43 - x83 - x84 - x86 - x87 + x88 + x89 - x90*x91 + x92 + x93) + x74*x94);
   out_5229921342576304612[1] = x56*((1.0/2.0)*x103*x75*x77 + x58*x80 + (1.0/2.0)*x61*(x100 + x99) - x82*((1.0/2.0)*x100 + (1.0/2.0)*x99)) + x96*(x103*x94 + x13*(-x104 - x105 + x106 + x107 + x108 + x109 - x110*x111 - x110*x36 - x113 - x115 - x117 - x119 + x120 + x121 + x122 + x123 + x124 + x125 - x46 - x47));
   out_5229921342576304612[2] = x56*((1.0/2.0)*x130*x75*x77 + x59*x80 + (1.0/2.0)*x61*(x126 + x127) - x82*((1.0/2.0)*x126 + (1.0/2.0)*x127)) + x96*(x13*(-x111*x137 - x131 - x132 + x133 + x134 + x135 + x136 - x137*x91 - x138 - x139 + x140 + x141 + x142 + x143 - x32 - x35 + x37 + x38 - x51 - x52) + x130*x94);
   out_5229921342576304612[3] = x56*(-x57*x80 + (1.0/2.0)*x61*(x145 + x147) + (1.0/2.0)*x73*x75*x77 - x82*((1.0/2.0)*x145 + (1.0/2.0)*x147)) + x96*(x13*(-b_0*x150 - b_0*x151 - x148 - x149 - x152 - x153 + x154 + x155 + x156 + x19 + x25 - x30 - x31 + x49 + x53 + x83 + x84 - x88 - x89 + x92) + x73*x94);
   out_5229921342576304612[4] = x56*((1.0/2.0)*x102*x75*x77 - x58*x80 + (1.0/2.0)*x61*(x158 + x159) - x82*((1.0/2.0)*x158 + (1.0/2.0)*x159)) + x96*(x102*x94 + x13*(-b_1*x151 - b_1*x162 + x104 + x105 - x108 - x109 + x115 + x119 - x121 - x123 + x124 + x160 + x161 - x163 - x164 + x166 + x167 + x168 - x44 - x54));
   out_5229921342576304612[5] = x56*((1.0/2.0)*x129*x75*x77 - x59*x80 + (1.0/2.0)*x61*(x169 + x170) - x82*((1.0/2.0)*x169 + (1.0/2.0)*x170)) + x96*(x129*x94 + x13*(-b_2*x150 - b_2*x162 + x131 + x132 - x135 - x136 + x139 - x141 + x142 + x171 + x172 - x173 - x174 + x176 + x177 + x178 + x35 - x38 - x45 - x50));
   out_5229921342576304612[6] = x56*((1.0/2.0)*x61*(x179 + x180) - x82*((1.0/2.0)*x179 + (1.0/2.0)*x180)) + x95*(-c_0*x181 - c_0*x182 + x14 + x148 + x149 + x152 + x153 - x154 - x155 + x156 + x20 - x27 - x29 - x42 - x43 - x49 - x53 + x86 + x87 + x93);
   out_5229921342576304612[7] = x56*((1.0/2.0)*x61*(x183 + x184) - x82*((1.0/2.0)*x183 + (1.0/2.0)*x184)) + x95*(-c_1*x182 - c_1*x185 - x106 - x107 + x113 + x117 - x120 - x122 + x125 - x160 - x161 + x163 + x164 - x166 - x167 + x168 + x44 + x46 + x47 + x54);
   out_5229921342576304612[8] = x56*((1.0/2.0)*x61*(x186 + x187) - x82*((1.0/2.0)*x186 + (1.0/2.0)*x187)) + x95*(-c_2*x181 - c_2*x185 - x133 - x134 + x138 - x140 + x143 - x171 - x172 + x173 + x174 - x176 - x177 + x178 + x32 - x37 + x45 + x50 + x51 + x52);

}
