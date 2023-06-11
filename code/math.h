#ifndef MATH_H
#define MATH_H

#define Sq(x) ((x)*(x))
#define Sqrt(x) sqrt((x))
#define PI 3.14159265358

#define PIVOT_X 0
#define PIVOT_Y 1
#define PIVOT_Z 1

// ************************** Primarily for 2d things ***************************
typedef struct Vec3 {
  r32 x;
  r32 y;
  r32 z;
} Vec3;

Vec3 ScalerAdd3(Vec3 Vec, r32 Scaler)
{
  Vec.x += Scaler;
  Vec.y += Scaler;
  Vec.z += Scaler;

  return Vec;
}

Vec3 ScalerMul3(Vec3 Vec, r32 Scaler)
{
  Vec.x *= Scaler;
  Vec.y *= Scaler;
  Vec.z *= Scaler;

  return Vec;
}

Vec3 ScalerDiv3(Vec3 Vec, r32 Scaler)
{
  Vec.x = Vec.x/Scaler;
  Vec.y = Vec.y/Scaler;
  Vec.z = Vec.y/Scaler;

  return Vec;
}

Vec3 AddVec3(Vec3 V, Vec3 K)
{
  Vec3 Res;
  Res.x = V.x + K.x;
  Res.y = V.y + K.y;
  Res.z = V.z + K.z;

  return Res;
}

r32 VecLen3(Vec3 V)
{
  r32 L = Sqrt(Sq(V.x) + Sq(V.y) + Sq(V.z));
  return L;
}

Vec3 UnitVec3(Vec3 V)
{
  Vec3 R;
  r32 L = VecLen3(V);
  R.x = V.x/L;
  R.y = V.y/L;
  R.z = V.z/L;

  return R;
}

r32 DotProductVec3(Vec3 S, Vec3 K)
{
  Vec3 R;
  R.x = S.x*K.x;
  R.y = S.y*K.y;
  R.z = S.z*K.z;

  r32 DotProd = R.x + R.y + R.z;
  return DotProd;
}

Vec3 CrossProductVec3(Vec3 S, Vec3 K)
{
  Vec3 R;
  R.x = (S.y*K.z) - (S.z*K.y);
  R.y = (S.z*K.x) - (S.x*K.z);
  R.z = (S.x*K.y) - (S.y*K.x);

  return R;
}

// *************************** Primarily for 3d things ********************************

typedef struct Vec4 {
  r32 x;
  r32 y;
  r32 z;
  r32 w;
} Vec4;

typedef struct Mat4 {
  r32 x0, x1, x2, x3;
  r32 y0, y1, y2, y3;
  r32 z0, z1, z2, z3;
  r32 w0, w1, w2, w3;
} Mat4;

Vec4 ScalerAdd4(Vec4 Vec, r32 Scaler)
{
  Vec.x += Scaler;
  Vec.y += Scaler;
  Vec.z += Scaler;
  Vec.w += Scaler;

  return Vec;
}

Vec4 ScalerMul4(Vec4 Vec, r32 Scaler)
{
  Vec.x *= Scaler;
  Vec.y *= Scaler;
  Vec.z *= Scaler;
  Vec.w *= Scaler;

  return Vec;
}

Vec4 ScalerDiv4(Vec4 Vec, r32 Scaler)
{
  Vec.x = Vec.x/Scaler;
  Vec.y = Vec.y/Scaler;
  Vec.z = Vec.z/Scaler;
  Vec.w = Vec.w/Scaler;

  return Vec;
}

Vec4 AddVec4(Vec4 V, Vec4 K)
{
  Vec4 Res;
  Res.x = V.x + K.x;
  Res.y = V.y + K.y;
  Res.z = V.z + K.z;
  Res.w = V.w + K.w;

  return Res;
}

r32 VecLen4(Vec4 V)
{
  r32 L = Sqrt(Sq(V.x) + Sq(V.y) + Sq(V.z) +Sq(V.w));
  return L;
}

Vec4 UnitVec3(Vec4 V)
{
  Vec4 R;
  r32 L = VecLen4(V);
  R.x = V.x/L;
  R.y = V.y/L;
  R.z = V.z/L;
  R.w = V.w/L;

  return R;
}

r32 DotProductVec4(Vec4 S, Vec4 K)
{
  Vec4 R;
  R.x = S.x*K.x;
  R.y = S.y*K.y;
  R.z = S.z*K.z;
  R.w = S.w*K.w;

  r32 DotProd = R.x + R.y + R.z + R.w;
  return DotProd;
}

// TODO: multiplication operations can be SSE'd
Vec4 Mul_Mat4Vec4(Mat4 Matrix, Vec4 S)
{
  Vec4 Res;
  Res.x = (Matrix.x0*S.x) + (Matrix.x1*S.y) + (Matrix.x2*S.z) + (Matrix.x3*S.w);
  Res.y = (Matrix.y0*S.y) + (Matrix.y1*S.y) + (Matrix.y2*S.z) + (Matrix.y3*S.w);
  Res.z = (Matrix.z0*S.z) + (Matrix.z1*S.y) + (Matrix.z2*S.z) + (Matrix.z3*S.w);
  Res.w = (Matrix.w0*S.w) + (Matrix.w1*S.y) + (Matrix.w2*S.z) + (Matrix.w3*S.w);

  return Res;
}

Mat4 Mul_Mat(Mat4 M1, Mat4 M2)
{
  Mat4 Res;
  // Row 0
  Res.x0 = (M1.x0*M2.x0) + (M1.x1*M2.y0) + (M1.x2*M2.z0) + (M1.x3*M2.w0);
  Res.x1 = (M1.x0*M2.x1) + (M1.x1*M2.y1) + (M1.x2*M2.z1) + (M1.x3*M2.w1);
  Res.x2 = (M1.x0*M2.x2) + (M1.x1*M2.y2) + (M1.x2*M2.z2) + (M1.x3*M2.w2);
  Res.x3 = (M1.x0*M2.x3) + (M1.x1*M2.y3) + (M1.x2*M2.z3) + (M1.x3*M2.w3);

  Res.y0 = (M1.y0*M2.x0) + (M1.y1*M2.y0) + (M1.y2*M2.z0) + (M1.y3*M2.w0);
  Res.y1 = (M1.y0*M2.x1) + (M1.y1*M2.y1) + (M1.y2*M2.z1) + (M1.y3*M2.w1);
  Res.y2 = (M1.y0*M2.x2) + (M1.y1*M2.y2) + (M1.y2*M2.z2) + (M1.y3*M2.w2);
  Res.y3 = (M1.y0*M2.x3) + (M1.y1*M2.y3) + (M1.y2*M2.z3) + (M1.y3*M2.w3);

  Res.z0 = (M1.z0*M2.x0) + (M1.z1*M2.y0) + (M1.z2*M2.z0) + (M1.z3*M2.w0);
  Res.z1 = (M1.z0*M2.x1) + (M1.z1*M2.y1) + (M1.z2*M2.z1) + (M1.z3*M2.w1);
  Res.z2 = (M1.z0*M2.x2) + (M1.z1*M2.y2) + (M1.z2*M2.z2) + (M1.z3*M2.w2);
  Res.z3 = (M1.z0*M2.x3) + (M1.z1*M2.y3) + (M1.z2*M2.z3) + (M1.z3*M2.w3);

  Res.w0 = (M1.w0*M2.x0) + (M1.w1*M2.y0) + (M1.w2*M2.z0) + (M1.w3*M2.w0);
  Res.w1 = (M1.w0*M2.x1) + (M1.w1*M2.y1) + (M1.w2*M2.z1) + (M1.w3*M2.w1);
  Res.w2 = (M1.w0*M2.x2) + (M1.w1*M2.y2) + (M1.w2*M2.z2) + (M1.w3*M2.w2);
  Res.w3 = (M1.w0*M2.x3) + (M1.w1*M2.y3) + (M1.w2*M2.z3) + (M1.w3*M2.w3);

  return Res;
}

/*
 * Matrix multiplication orders
 * Scale -> Rotate -> Translate
 */

Mat4 CreateTranslationMat(Vec4 S)
{
  Mat4 TranslationMatrix = (Mat4){
    .x0=1.0f,.x3=S.x,
    .y1=1.0f,.y3=S.y,
    .z2=1.0f,.z3=S.z,
    .w3=1.0f};

  return TranslationMatrix;
}

Mat4 CreateScaleMat(Vec4 S)
{
  Mat4 ScaleMatrix = (Mat4){.x0=S.x,.y1=S.y,.z2=S.z,.w3=1.0f};
  return ScaleMatrix;
}

Mat4 CreateRotationMat(r32 Theta, u8 Pivot)
{
  r32 CosTheta = cos(Theta);
  r32 SinTheta = cos(Theta);

  Mat4 RotMat;
  if (Pivot == PIVOT_X)
  {
    RotMat.x0 = 1.0f;
    RotMat.y1 = CosTheta; RotMat.y2 = -SinTheta;
    RotMat.z1 = SinTheta; RotMat.z2 = CosTheta;
    RotMat.w3 = 1.0f;
  }
  else if (Pivot == PIVOT_Y)
  {
    RotMat.x0 = CosTheta; RotMat.x2 = SinTheta;
    RotMat.y1 = 1.0f;
    RotMat.z0 = -SinTheta; RotMat.z2 = CosTheta;
    RotMat.w3 = 1.0f;
  }
  else if (Pivot == PIVOT_Z)
  {
    RotMat.x0 = CosTheta; RotMat.x1 = -SinTheta;
    RotMat.y0 = SinTheta; RotMat.y1 = CosTheta;
    RotMat.z3 = 1.0f;
    RotMat.w3 = 1.0f;
  }

  return RotMat;
}

/*
 * This scales a 3d Vec, using 4d vectors
 * for matrix multiplication
 */
Vec3 Scale3DVec(Vec3 S, Vec3 Factor)
{
  Vec4 S4 = (Vec4){.x=S.x, .y=S.y, .z=S.z, .w=1};
  Vec4 Factor4 = (Vec4){.x=Factor.x, .y=Factor.y, .z=Factor.z, .w=1.0f};
  Mat4 ScaleMatrix = CreateScaleMat(Factor4);

  Vec4 Res = Mul_Mat4Vec4(ScaleMatrix, S4);
  Vec3 Res3 = (Vec3){.x=Res.x,.y=Res.y,.z=Res.z};

  return Res3;
}

/*
 * This translates a 3d Vec, using 4d vectors
 * for matrix multiplication
 */
Vec3 TranslateVec4(Vec3 S, Vec3 Offset)
{
  Vec4 S4 = (Vec4){.x=S.x, .y=S.y, .z=S.z, .w=1.0f};
  Vec4 Offset4 = (Vec4){.x=Offset.x, .y=Offset.y, .z=Offset.z, .w=1.0f};
  Mat4 TranslationMatrix = CreateTranslationMat(Offset4);

  Vec4 Res = Mul_Mat4Vec4(TranslationMatrix, S4);
  Vec3 Res3 = (Vec3){.x=Res.x,.y=Res.y,.z=Res.z};

  return Res3;
}

/*
 * Rotate a 3D Vector around a pivot dimension
 * Angle is in Theta
 */
Vec3 Rotate3DVec(Vec3 S, r32 Theta, u8 Pivot)
{
  Mat4 RotMat = CreateRotationMat(Theta, Pivot);
  Vec4 S4 = (Vec4){.x=S.z, .y=S.y, .z=S.z, .w=1.0f};
  Vec4 Res = Mul_Mat4Vec4(RotMat, S4);
  Vec3 Res3 = (Vec3){.x=Res.x,.y=Res.y,.z=Res.z};

  return Res3;
}

#endif
