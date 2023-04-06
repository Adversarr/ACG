/******************************************************************************
 *                      Code generated with SymPy 1.10.1                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'acgipc'                        *
 ******************************************************************************/
#include <aphysics/collision/distances/vfd_value.hpp>
#include <cmath>
namespace acg::physics::collision::details {

double vfd_value(double a_0, double a_1, double a_2, double b_0, double b_1,
                 double b_2, double c_0, double c_1, double c_2, double d_0,
                 double d_1, double d_2) {
  const double x0 = a_0 - b_0;
  const double x1 = a_1 - c_1;
  const double x2 = a_0 - c_0;
  const double x3 = a_1 - b_1;
  const double x4 = x0 * x1 - x2 * x3;
  const double x5 = a_2 - c_2;
  const double x6 = a_2 - b_2;
  const double x7 = x0 * x5 - x2 * x6;
  const double x8 = -x1 * x6 + x3 * x5;

  double vfd_value_result;
  vfd_value_result = (-x4 * (a_2 - d_2) + x7 * (a_1 - d_1) - x8 * (a_0 - d_0)) /
                     sqrt(pow(x4, 2) + pow(x7, 2) + pow(x8, 2));
  return vfd_value_result;
}

void vfd_grad(double a_0, double a_1, double a_2, double b_0, double b_1,
              double b_2, double c_0, double c_1, double c_2, double d_0,
              double d_1, double d_2, double *out_1250655405979693308) {
  const double x0 = -c_1;
  const double x1 = a_1 + x0;
  const double x2 = a_2 - b_2;
  const double x3 = x1 * x2;
  const double x4 = a_1 - d_1;
  const double x5 = -c_2;
  const double x6 = b_2 + x5;
  const double x7 = a_2 - d_2;
  const double x8 = -x7;
  const double x9 = b_1 + x0;
  const double x10 = a_1 - b_1;
  const double x11 = a_2 + x5;
  const double x12 = x10 * x11;
  const double x13 = a_0 - b_0;
  const double x14 = x1 * x13;
  const double x15 = -c_0;
  const double x16 = a_0 + x15;
  const double x17 = x10 * x16;
  const double x18 = x14 - x17;
  const double x19 = x11 * x13 - x16 * x2;
  const double x20 = x12 - x3;
  const double x21 = pow(x18, 2) + pow(x19, 2) + pow(x20, 2);
  const double x22 = pow(x21, -1.0 / 2.0);
  const double x23 = 2 * b_1;
  const double x24 = -2 * c_1;
  const double x25 = x23 + x24;
  const double x26 = (1.0 / 2.0) * x18;
  const double x27 = 2 * b_2;
  const double x28 = -2 * c_2;
  const double x29 = (1.0 / 2.0) * x27 + (1.0 / 2.0) * x28;
  const double x30 = a_0 - d_0;
  const double x31 = (-x18 * x7 + x19 * x4 - x20 * x30) / pow(x21, 3.0 / 2.0);
  const double x32 = -x30;
  const double x33 = -b_0 - x15;
  const double x34 = 2 * b_0;
  const double x35 = -2 * c_0;
  const double x36 = -x34 - x35;
  const double x37 = (1.0 / 2.0) * x19;
  const double x38 = (1.0 / 2.0) * x20;
  const double x39 = -x11;
  const double x40 = 2 * a_1;
  const double x41 = x24 + x40;
  const double x42 = 2 * a_2;
  const double x43 = -x28 - x42;
  const double x44 = 2 * a_0;
  const double x45 = x35 + x44;
  const double x46 = -x23 + x40;
  const double x47 = -x27 + x42;
  const double x48 = -x13;
  const double x49 = x34 - x44;

  out_1250655405979693308[0] =
      x22 * (-x12 + x3 + x4 * x6 + x8 * x9) + x31 * (-x19 * x29 - x25 * x26);
  out_1250655405979693308[1] =
      x22 * (x19 + x32 * x6 + x33 * x8) + x31 * (-x20 * x29 - x26 * x36);
  out_1250655405979693308[2] =
      x22 * (-x14 + x17 - x32 * x9 + x33 * x4) + x31 * (x25 * x38 - x36 * x37);
  out_1250655405979693308[3] =
      x22 * (-x1 * x8 + x39 * x4) + x31 * (x26 * x41 - x37 * x43);
  out_1250655405979693308[4] =
      x22 * (x16 * x8 + x32 * x39) + x31 * (-x26 * x45 - x38 * x43);
  out_1250655405979693308[5] =
      x22 * (x1 * x32 + x16 * x4) + x31 * (-x37 * x45 - x38 * x41);
  out_1250655405979693308[6] =
      x22 * (x10 * x8 + x2 * x4) + x31 * (-x26 * x46 - x37 * x47);
  out_1250655405979693308[7] =
      x22 * (x2 * x32 + x48 * x8) + x31 * (-x26 * x49 - x38 * x47);
  out_1250655405979693308[8] =
      x22 * (-x10 * x32 + x4 * x48) + x31 * (-x37 * x49 + x38 * x46);
  out_1250655405979693308[9] = x20 * x22;
  out_1250655405979693308[10] = -x19 * x22;
  out_1250655405979693308[11] = x18 * x22;
}

float vfd_value(float a_0, float a_1, float a_2, float b_0, float b_1,
                float b_2, float c_0, float c_1, float c_2, float d_0,
                float d_1, float d_2) {
  const float x0 = a_0 - b_0;
  const float x1 = a_1 - c_1;
  const float x2 = a_0 - c_0;
  const float x3 = a_1 - b_1;
  const float x4 = x0 * x1 - x2 * x3;
  const float x5 = a_2 - c_2;
  const float x6 = a_2 - b_2;
  const float x7 = x0 * x5 - x2 * x6;
  const float x8 = -x1 * x6 + x3 * x5;

  float vfd_value_result;
  vfd_value_result = (-x4 * (a_2 - d_2) + x7 * (a_1 - d_1) - x8 * (a_0 - d_0)) /
                     sqrt(pow(x4, 2) + pow(x7, 2) + pow(x8, 2));
  return vfd_value_result;
}

void vfd_grad(float a_0, float a_1, float a_2, float b_0, float b_1, float b_2,
              float c_0, float c_1, float c_2, float d_0, float d_1, float d_2,
              float *out_1250655405979693308) {
  const float x0 = -c_1;
  const float x1 = a_1 + x0;
  const float x2 = a_2 - b_2;
  const float x3 = x1 * x2;
  const float x4 = a_1 - d_1;
  const float x5 = -c_2;
  const float x6 = b_2 + x5;
  const float x7 = a_2 - d_2;
  const float x8 = -x7;
  const float x9 = b_1 + x0;
  const float x10 = a_1 - b_1;
  const float x11 = a_2 + x5;
  const float x12 = x10 * x11;
  const float x13 = a_0 - b_0;
  const float x14 = x1 * x13;
  const float x15 = -c_0;
  const float x16 = a_0 + x15;
  const float x17 = x10 * x16;
  const float x18 = x14 - x17;
  const float x19 = x11 * x13 - x16 * x2;
  const float x20 = x12 - x3;
  const float x21 = pow(x18, 2) + pow(x19, 2) + pow(x20, 2);
  const float x22 = pow(x21, -1.0 / 2.0);
  const float x23 = 2 * b_1;
  const float x24 = -2 * c_1;
  const float x25 = x23 + x24;
  const float x26 = (1.0 / 2.0) * x18;
  const float x27 = 2 * b_2;
  const float x28 = -2 * c_2;
  const float x29 = (1.0 / 2.0) * x27 + (1.0 / 2.0) * x28;
  const float x30 = a_0 - d_0;
  const float x31 = (-x18 * x7 + x19 * x4 - x20 * x30) / pow(x21, 3.0 / 2.0);
  const float x32 = -x30;
  const float x33 = -b_0 - x15;
  const float x34 = 2 * b_0;
  const float x35 = -2 * c_0;
  const float x36 = -x34 - x35;
  const float x37 = (1.0 / 2.0) * x19;
  const float x38 = (1.0 / 2.0) * x20;
  const float x39 = -x11;
  const float x40 = 2 * a_1;
  const float x41 = x24 + x40;
  const float x42 = 2 * a_2;
  const float x43 = -x28 - x42;
  const float x44 = 2 * a_0;
  const float x45 = x35 + x44;
  const float x46 = -x23 + x40;
  const float x47 = -x27 + x42;
  const float x48 = -x13;
  const float x49 = x34 - x44;

  out_1250655405979693308[0] =
      x22 * (-x12 + x3 + x4 * x6 + x8 * x9) + x31 * (-x19 * x29 - x25 * x26);
  out_1250655405979693308[1] =
      x22 * (x19 + x32 * x6 + x33 * x8) + x31 * (-x20 * x29 - x26 * x36);
  out_1250655405979693308[2] =
      x22 * (-x14 + x17 - x32 * x9 + x33 * x4) + x31 * (x25 * x38 - x36 * x37);
  out_1250655405979693308[3] =
      x22 * (-x1 * x8 + x39 * x4) + x31 * (x26 * x41 - x37 * x43);
  out_1250655405979693308[4] =
      x22 * (x16 * x8 + x32 * x39) + x31 * (-x26 * x45 - x38 * x43);
  out_1250655405979693308[5] =
      x22 * (x1 * x32 + x16 * x4) + x31 * (-x37 * x45 - x38 * x41);
  out_1250655405979693308[6] =
      x22 * (x10 * x8 + x2 * x4) + x31 * (-x26 * x46 - x37 * x47);
  out_1250655405979693308[7] =
      x22 * (x2 * x32 + x48 * x8) + x31 * (-x26 * x49 - x38 * x47);
  out_1250655405979693308[8] =
      x22 * (-x10 * x32 + x4 * x48) + x31 * (-x37 * x49 + x38 * x46);
  out_1250655405979693308[9] = x20 * x22;
  out_1250655405979693308[10] = -x19 * x22;
  out_1250655405979693308[11] = x18 * x22;
}
} // namespace acg::physics::collision::details