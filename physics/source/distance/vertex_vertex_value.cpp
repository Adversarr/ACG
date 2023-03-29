/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'acgipc'                        *
 ******************************************************************************/
#include "aphysics/ccd/distances/vertex_vertex_value.hpp"

#include <math.h>
double vertex_vertex_value(double a_0, double a_1, double a_2, double b_0, double b_1, double b_2, double d_h) {
   const double x0 = ((a_0 - b_0) * (a_0 - b_0)) + ((a_1 - b_1) * (a_1 - b_1)) + ((a_2 - b_2) * (a_2 - b_2));

   double vertex_vertex_value_result;
   vertex_vertex_value_result = -1.0/2.0*(2*log(d_h) - log(((a_0) * (a_0)) - 2*a_0*b_0 + ((a_1) * (a_1)) - 2*a_1*b_1 + ((a_2) * (a_2)) - 2*a_2*b_2 + ((b_0) * (b_0)) + ((b_1) * (b_1)) + ((b_2) * (b_2))))*(((d_h) * (d_h)) - 2*d_h*sqrt(x0) + x0);
   return vertex_vertex_value_result;

}

void vertex_vertex_grad(double a_0, double a_1, double a_2, double b_0, double b_1, double b_2, double d_h, double *out_5290998971776097939) {
   const double x0 = 2*b_0;
   const double x1 = 2*b_1;
   const double x2 = 2*b_2;
   const double x3 = ((a_0) * (a_0)) - a_0*x0 + ((a_1) * (a_1)) - a_1*x1 + ((a_2) * (a_2)) - a_2*x2 + ((b_0) * (b_0)) + ((b_1) * (b_1)) + ((b_2) * (b_2));
   const double x4 = -log(d_h) + (1.0/2.0)*log(x3);
   const double x5 = 2*a_0;
   const double x6 = a_0 - b_0;
   const double x7 = a_1 - b_1;
   const double x8 = a_2 - b_2;
   const double x9 = ((x6) * (x6)) + ((x7) * (x7)) + ((x8) * (x8));
   const double x10 = sqrt(x9);
   const double x11 = 2*d_h;
   const double x12 = x11/x10;
   const double x13 = -x0 + x5;
   const double x14 = (1.0/2.0)*(((d_h) * (d_h)) - x10*x11 + x9)/x3;
   const double x15 = 2*a_1;
   const double x16 = -x1 + x15;
   const double x17 = 2*a_2;
   const double x18 = x17 - x2;

   out_5290998971776097939[0] = x13*x14 + x4*(-x0 - x12*x6 + x5);
   out_5290998971776097939[1] = x14*x16 + x4*(-x1 - x12*x7 + x15);
   out_5290998971776097939[2] = x14*x18 + x4*(-x12*x8 + x17 - x2);
   out_5290998971776097939[3] = -x13*x14 + x4*(x12*x6 - x13);
   out_5290998971776097939[4] = -x14*x16 + x4*(x12*x7 - x16);
   out_5290998971776097939[5] = -x14*x18 + x4*(x12*x8 - x18);

}
float vertex_vertex_value(float a_0, float a_1, float a_2, float b_0, float b_1, float b_2, float d_h) {
   const float x0 = ((a_0 - b_0) * (a_0 - b_0)) + ((a_1 - b_1) * (a_1 - b_1)) + ((a_2 - b_2) * (a_2 - b_2));

   float vertex_vertex_value_result;
   vertex_vertex_value_result = -1.0/2.0*(2*log(d_h) - log(((a_0) * (a_0)) - 2*a_0*b_0 + ((a_1) * (a_1)) - 2*a_1*b_1 + ((a_2) * (a_2)) - 2*a_2*b_2 + ((b_0) * (b_0)) + ((b_1) * (b_1)) + ((b_2) * (b_2))))*(((d_h) * (d_h)) - 2*d_h*sqrt(x0) + x0);
   return vertex_vertex_value_result;

}

void vertex_vertex_grad(float a_0, float a_1, float a_2, float b_0, float b_1, float b_2, float d_h, float *out_5290998971776097939) {
   const float x0 = 2*b_0;
   const float x1 = 2*b_1;
   const float x2 = 2*b_2;
   const float x3 = ((a_0) * (a_0)) - a_0*x0 + ((a_1) * (a_1)) - a_1*x1 + ((a_2) * (a_2)) - a_2*x2 + ((b_0) * (b_0)) + ((b_1) * (b_1)) + ((b_2) * (b_2));
   const float x4 = -log(d_h) + (1.0/2.0)*log(x3);
   const float x5 = 2*a_0;
   const float x6 = a_0 - b_0;
   const float x7 = a_1 - b_1;
   const float x8 = a_2 - b_2;
   const float x9 = ((x6) * (x6)) + ((x7) * (x7)) + ((x8) * (x8));
   const float x10 = sqrt(x9);
   const float x11 = 2*d_h;
   const float x12 = x11/x10;
   const float x13 = -x0 + x5;
   const float x14 = (1.0/2.0)*(((d_h) * (d_h)) - x10*x11 + x9)/x3;
   const float x15 = 2*a_1;
   const float x16 = -x1 + x15;
   const float x17 = 2*a_2;
   const float x18 = x17 - x2;

   out_5290998971776097939[0] = x13*x14 + x4*(-x0 - x12*x6 + x5);
   out_5290998971776097939[1] = x14*x16 + x4*(-x1 - x12*x7 + x15);
   out_5290998971776097939[2] = x14*x18 + x4*(-x12*x8 + x17 - x2);
   out_5290998971776097939[3] = -x13*x14 + x4*(x12*x6 - x13);
   out_5290998971776097939[4] = -x14*x16 + x4*(x12*x7 - x16);
   out_5290998971776097939[5] = -x14*x18 + x4*(x12*x8 - x18);

}
