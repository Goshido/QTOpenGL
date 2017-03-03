#include <GXCommon/gx_math.h>
#include <math.h>
#include <cstdlib>
#include <cfloat>
#include <string.h>

#define GX_1_DIV_256 0.00392157f
#define GX_PI_DIV_180 0.0174533f

namespace gx_common {

GXVec2 __stdcall gxCreateVec2(float component_1, float component_2)
{
    GXVec2 ans;
    ans.x = component_1;
    ans.y = component_2;
    return ans;
}

void __stdcall gxSubVec2Vec2(GXVec2& out, const GXVec2& a, const GXVec2& b)
{
    out.x = a.x - b.x;
    out.y = a.y - b.y;
}

//------------------------------------------------------------------------------------------------

GXVec3::GXVec3()
{
    //NOTHING
}

GXVec3::GXVec3(float component_1, float component_2, float component_3)
{
    x = component_1;
    y = component_2;
    z = component_3;
}

void GXVec3::operator =(const GXVec3& v)
{
    memcpy(arr, v.arr, sizeof(GXVec3));
}

//----------------------------------------------------------------------------

GXVec3 __stdcall gxCreateVec3(float component_1, float component_2, float component_3)
{
    GXVec3 ans;
    ans.x = component_1;
    ans.y = component_2;
    ans.z = component_3;
    return ans;
}

void __stdcall gxNormalizeVec3(GXVec3& inOut)
{
    float inv_a = 1.0f / sqrtf(inOut.x * inOut.x + inOut.y * inOut.y + inOut.z * inOut.z);
    inOut.x *= inv_a;
    inOut.y *= inv_a;
    inOut.z *= inv_a;
}

void __stdcall gxSumVec3Vec3(GXVec3& out, const GXVec3& a, const GXVec3& b)
{
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    out.z = a.z + b.z;
}

void __stdcall gxSubVec3Vec3(GXVec3& out, const GXVec3& a, const GXVec3& b)
{
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    out.z = a.z - b.z;
}

void __stdcall gxMulVec3Vec3(GXVec3& out, const GXVec3& a, const GXVec3& b)
{
    out.x = a.x * b.x;
    out.y = a.y * b.y;
    out.z = a.z * b.z;
}

void __stdcall gxMulVec3Scalar(GXVec3& out, const GXVec3& v, float factor)
{
    out.x = v.x * factor;
    out.y = v.y * factor;
    out.z = v.z * factor;
}

float __stdcall gxDotVec3Fast(const GXVec3& a, const GXVec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

void __stdcall gxCrossVec3Vec3(GXVec3& out, const GXVec3& a, const GXVec3& b)
{
    out.x = a.y * b.z - a.z * b.y;
    out.y = a.z * b.x - a.x * b.z;
    out.z = a.x * b.y - a.y * b.x;
}

float __stdcall gxLengthVec3(const GXVec3& v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float __stdcall gxDistanceVec3Vec3(const GXVec3& a, const GXVec3& b)
{
    GXVec3 diff;
    gxSubVec3Vec3(diff, a, b);
    return gxLengthVec3(diff);
}

void __stdcall gxProjectVec3Vec3(GXVec3& projection, const GXVec3& vector, const GXVec3& unitVector)
{
    GXVec3 normalVector = vector;
    gxNormalizeVec3(normalVector);

    float factor = gxLengthVec3(vector) * gxDotVec3Fast(normalVector, unitVector);

    projection.x = unitVector.x * factor;
    projection.y = unitVector.y * factor;
    projection.z = unitVector.z * factor;
}

//----------------------------------------------------------------------------------------

GXVec4::GXVec4()
{
    //NOTHING
}

GXVec4::GXVec4(GXVec3& v, float component_4)
{
    memcpy(arr, v.arr, sizeof(GXVec3));
    w = component_4;
}

GXVec4::GXVec4(float component_1, float component_2, float component_3, float component_4)
{
    x = component_1;
    y = component_2;
    z = component_3;
    w = component_4;
}

void GXVec4::operator =(const GXVec4 &v)
{
    memcpy(arr, v.arr, sizeof(GXVec4));
}

GXVec4 __stdcall gxCreateVec4(float component_1, float component_2, float component_3, float component_4)
{
    return GXVec4(component_1, component_2, component_3, component_4);
}

//-------------------------------------------------------------------

GXQuat __stdcall gxCreateQuat(const GXMat4& mat)
{
    float trace;
    float s;
    float t;
    int i;
    int j;
    int k;

    GXQuat v;

    static int next[3] = {1, 2, 0};

    trace = mat.m[0][0] + mat.m[1][1] + mat.m[2][2];

    if (trace > 0.0f) {
        t = trace + 1.0f;
        s = 1.0f / sqrtf(t) * 0.5f;

        v.arr[3] = s * t;
        v.arr[0] = (mat.m[2][1] - mat.m[1][2]) * s;
        v.arr[1] = (mat.m[0][2] - mat.m[2][0]) * s;
        v.arr[2] = (mat.m[1][0] - mat.m[0][1]) * s;
    } else {
        i = 0;

        if (mat.m[1][1] > mat.m[0][0]) {
            i = 1;
        }

        if (mat.m[2][2] > mat.m[i][i]) {
            i = 2;
        }

        j = next[i];
        k = next[j];

        t = (mat.m[i][i] - (mat.m[j][j] + mat.m[k][k])) + 1.0f;
        s = 1.0f / sqrtf(t) * 0.5f;

        v.arr[i] = s * t;
        v.arr[3] = (mat.m[k][j] - mat.m[j][k]) * s;
        v.arr[j] = (mat.m[j][i] + mat.m[i][j]) * s;
        v.arr[k] = (mat.m[k][i] + mat.m[i][k]) * s;
    }

    return v;
}

void __stdcall gxQuatRehandCoordinateSystem(GXQuat& out, const GXQuat& src)
{
    out = src;
    out.w = -src.w;
}

void __stdcall gxQuatToEulerAngles(GXVec3& out_rad, const GXQuat& q)
{
    //TODO
    float factor = q.z * q.z;

    out_rad.x = atan2f(2.0f * (q.x * q.y + q.z * q.w), 1.0f - 2.0f * (q.y * q.y + factor));
    out_rad.y = asinf(2.0f * (q.x * q.z - q.w * q.y));
    out_rad.z = atan2f(2.0f * (q.x * q.w + q.y * q.z), 1.0f - 2.0f * (factor + q.w * q.w));
}

void __stdcall gxSetQuatIdentity(GXQuat& out)
{
    out.x = out.y = out.z = 0.0f;
    out.w = 1.0f;
}

void __stdcall gxSetQuatRotationAxis(GXQuat& out, float x, float y, float z, float angle)
{
    float sn = sinf(angle * 0.5f);
    float cs = cosf(angle * 0.5f);

    out.x = x * sn;
    out.y = y * sn;
    out.z = z * sn;
    out.w = cs;
}

void __stdcall gxSetQuatRotationAxis(GXQuat& out, const GXVec3& axis, float angle)
{
    float sn = sinf(angle * 0.5f);
    float cs = cosf(angle * 0.5f);

    out.x = axis.x * sn;
    out.y = axis.y * sn;
    out.z = axis.z * sn;
    out.w = cs;
}

void __stdcall gxMulQuatQuat(GXQuat& out, const GXQuat& a, const GXQuat& b)
{
    float A, B, C, D, E, F, G, H;

    A = (a.w + a.x) * (b.w + b.x);
    B = (a.z - a.y) * (b.y - b.z);
    C = (a.x - a.w) * (b.y + b.z);
    D = (a.y + a.z) * (b.x - b.w);
    E = (a.x + a.z) * (b.x + b.y);
    F = (a.x - a.z) * (b.x - b.y);
    G = (a.w + a.y) * (b.w - b.z);
    H = (a.w - a.y) * (b.w + b.z);

    out.w =  B + (-E - F + G + H) * 0.5f;
    out.x =  A - ( E + F + G + H) * 0.5f;
    out.y = -C + ( E - F + G - H) * 0.5f;
    out.z = -D + ( E - F - G + H) * 0.5f;
}

void __stdcall gxSumQuatQuat(GXQuat& out, const GXQuat& a, const GXQuat& b)
{
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    out.z = a.z + b.z;
    out.w = a.w + b.w;
}

void __stdcall gxSubQuatQuat(GXQuat& out, const GXQuat& a, const GXQuat& b)
{
    GXQuat bi;
    gxInverseQuat(bi, b);
    gxMulQuatQuat(out, bi, a);
}

void __stdcall gxQuatSLerp(GXQuat& out, const GXQuat& a, const GXQuat& b, float k)
{
    GXQuat temp;
    float omega;
    float cosom;
    float sinom;
    float scale0;
    float scale1;

    if ( k <= 0.0f ) {
        out = a;
        return;
    }

    if ( k >= 1.0f ) {
        out = b;
        return;
    }

    cosom = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    if ( cosom < 0.0f ) {
        temp.x = -b.x;
        temp.y = -b.y;
        temp.z = -b.z;
        temp.w = -b.w;
        cosom = -cosom;
    } else {
        temp = b;
    }

    if ( ( 1.0f - cosom ) > 1e-6f ) {
        omega = acosf(cosom);
        sinom = 1.0f / sinf(omega);
        scale0 = sinf((1.0f - k) * omega) * sinom;
        scale1 = sinf(k * omega) * sinom;
    } else {
        scale0 = 1.0f - k;
        scale1 = k;
    }

    out.x = a.x * scale0 + temp.x * scale1;
    out.y = a.y * scale0 + temp.y * scale1;
    out.z = a.z * scale0 + temp.z * scale1;
    out.w = a.w * scale0 + temp.w * scale1;
}

void __stdcall gxInverseQuat(GXQuat& out, const GXQuat& q)
{
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if ( lenSq > 0.0f ) {
        float invLenSq = 1.0f / lenSq;
        out.x = -q.x * invLenSq;
        out.y = -q.y * invLenSq;
        out.z = -q.z * invLenSq;
        out.w = q.w * invLenSq;
        return;
    } else {
        out.x = out.y = out.z = 0.0f;
        out.w = 1.0f;
    }
}

void __stdcall gxNormalizeQuat(GXQuat& out, const GXQuat& q)
{
    float len = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    if (len) {
        float ilength = 1.0f / len;
        out.x *= ilength;
        out.y *= ilength;
        out.z *= ilength;
        out.w *= ilength;
    }
}

void __stdcall gxQuatTransform(GXVec3& out, const GXQuat& q, const GXVec3& v)
{
    float xxzz = q.x * q.x - q.z * q.z;
    float wwyy = q.w * q.w - q.y * q.y;

    float xw2 = q.x * q.w * 2.0f;
    float xy2 = q.x * q.y * 2.0f;
    float xz2 = q.x * q.z * 2.0f;
    float yw2 = q.y * q.w * 2.0f;
    float yz2 = q.y * q.z * 2.0f;
    float zw2 = q.z * q.w * 2.0f;

    out.x = ( xxzz + wwyy ) *v.x + ( xy2 + zw2 ) * v.y                                      + ( xz2 - yw2 ) * v.z;
    out.y = ( xy2 - zw2 ) * v.x  + ( q.y * q.y + q.w * q.w - q.x * q.x - q.z * q.z ) * v.y  + ( yz2 + xw2 ) * v.z;
    out.z = ( xz2 + yw2 ) * v.x  + ( yz2 - xw2 ) * v.y                                      + ( wwyy - xxzz ) * v.z;
}

//------------------------------------------------------------------

GXMat4::GXMat4()
{
    memset(arr, 0, sizeof(GXMat4));
}

GXMat4::GXMat4(float* v16)
{
    memcpy(arr, v16, sizeof(GXMat4));
}

void GXMat4::operator =(const GXMat4& m)
{
    memcpy (arr, m.arr, sizeof(GXMat4));
}

void GXMat4::setRotation(const GXQuat& q)
{
    float wx, wy, wz;
    float xx, yy, yz;
    float xy, xz, zz;
    float x2, y2, z2;

    x2 = q.x + q.x;
    y2 = q.y + q.y;
    z2 = q.z + q.z;

    xx = q.x * x2;
    xy = q.x * y2;
    xz = q.x * z2;

    yy = q.y * y2;
    yz = q.y * z2;
    zz = q.z * z2;

    wx = q.w * x2;
    wy = q.w * y2;
    wz = q.w * z2;

    m[0][0] = 1.0f - (yy + zz);
    m[0][1] = xy - wz;
    m[0][2] = xz + wy;

    m[1][0] = xy + wz;
    m[1][1] = 1.0f - (xx + zz);
    m[1][2] = yz - wx;

    m[2][0] = xz - wy;
    m[2][1] = yz + wx;
    m[2][2] = 1.0f - (xx + yy);
}

void GXMat4::setOrigin(const GXVec3& origin)
{ 
    memcpy(arr + 12, origin.arr, sizeof(GXVec3));
}

void GXMat4::from(const GXQuat& quat, const GXVec3& origin)
{
    setRotation(quat);
    setOrigin(origin);
    m14 = m24 = m34 = 0.0f;
    m44 = 1.0f;
}

void GXMat4::from(const GXMat3& rotation, const GXVec3& origin)
{
    memcpy(&xv, &rotation.xv, sizeof(GXVec3));
    memcpy(&yv, &rotation.yv, sizeof(GXVec3));
    memcpy(&zv, &rotation.zv, sizeof(GXVec3));

    memcpy(&wv, &origin, sizeof(GXVec3));

    m14 = m24 = m34 = 0.0f;
    m44 = 1.0f;
}

void __stdcall gxSetMat4Identity(GXMat4& out)
{
    out.m12 = out.m13 = out.m14 = 0.0f;
    out.m21 = out.m23 = out.m24 = 0.0f;
    out.m31 = out.m32 = out.m34 = 0.0f;
    out.m41 = out.m42 = out.m43 = 0.0f;

    out.m11 = out.m22 = out.m33 = out.m44 = 1.0f;
}

void __stdcall gxSetMat4Perspective(GXMat4& out, float fovy_rad, float aspect, float znear, float zfar)
{
    float halfFovy = fovy_rad * 0.5f;
    float ctan = cosf(halfFovy) / sinf(halfFovy);
    float invRange = 1.0f / (zfar - znear);

    out.m11 = ctan / aspect;
    out.m22 = ctan;
    out.m33 = (zfar + znear) * invRange;
    out.m34 = 1.0f;
    out.m43 = -2.0f * zfar * znear * invRange;

    out.m12 = out.m13 = out.m14 = 0.0f;
    out.m21 = out.m23 = out.m24 = 0.0f;
    out.m31 = out.m32 = 0.0f;
    out.m41 = out.m42 = out.m44 = 0.0f;
}

void __stdcall gxSetMat4Ortho(GXMat4& out, float width, float height, float znear, float zfar)
{
    float invRange = 1.0f / (zfar - znear);

    out.m11 = 2.0f / width;
    out.m22 = 2.0f / height;
    out.m33 = 2.0f * invRange;
    out.m43 = -invRange * (zfar + znear);

    out.m12 = out.m13 = out.m14 = 0.0f;
    out.m21 = out.m23 = out.m24 = 0.0f;
    out.m31 = out.m32 = out.m34 = 0.0f;
    out.m41 = out.m42 = 0.0f;
    out.m44 = 1.0f;
}

void __stdcall gxSetMat4Translation(GXMat4& out, float x, float y, float z)
{
    out.m12 = out.m13 = out.m14 = 0.0f;
    out.m21 = out.m23 = out.m24 = 0.0f;
    out.m31 = out.m32 = out.m34 = 0.0f;

    out.m11 = out.m22 = out.m33 = out.m44 = 1.0f;

    out.m41 = x;
    out.m42 = y;
    out.m43 = z;
}

void __stdcall gxSetMat4TranslateTo(GXMat4& out, float x, float y, float z)
{
    out.wv = gxCreateVec3(x, y, z);
}

void __stdcall gxSetMat4TranslateTo(GXMat4& out, const GXVec3& location)
{
    memcpy(out.wv.arr, location.arr, sizeof(GXVec3));
}

void __stdcall gxSetMat4RotationX(GXMat4& out, float angle)
{
    out.m12 = out.m13 = 0.0f;
    out.m21 = 0.0f;
    out.m31 = 0.0f;

    float c;
    float s;

    c = cosf(angle);
    s = sinf(angle);

    out.m22 = c;
    out.m23 = s;
    out.m32 = -s;
    out.m33 = c;

    out.m11 = 1.0f;

    out.m44 = 1.0f;
    out.m41 = out.m42 = out.m43 = out.m14 = out.m24 = out.m34 = 0;
}

void __stdcall gxSetMat4RotationY(GXMat4& out, float angle)
{
    out.m12 = 0.0f;
    out.m21 = out.m23 = 0.0f;
    out.m32 = 0.0f;

    float c;
    float s;

    c = cosf(angle);
    s = sinf(angle);

    out.m11 = c;
    out.m13 = -s;
    out.m31 = s;
    out.m33 = c;

    out.m22 = 1.0f;

    out.m44 = 1.0f;
    out.m41 = out.m42 = out.m43 = out.m14 = out.m24 = out.m34 = 0.0f;
}

void __stdcall gxSetMat4RotationZ(GXMat4& out, float angle)
{
    out.m13 = 0.0f;
    out.m23 = 0.0f;
    out.m31 = out.m32 = 0.0f;

    float c;
    float s;

    c = cosf(angle);
    s = sinf(angle);

    out.m11 = c;
    out.m12 = s;
    out.m21 = -s;
    out.m22 = c;

    out.m33 = 1.0f;

    out.m44 = 1.0f;
    out.m41 = out.m42 = out.m43 = out.m14 = out.m24 = out.m34 = 0.0f;
}

void __stdcall gxSetMat4RotationXY(GXMat4& out, float pitch_rad, float yaw_rad)
{
    GXMat4 x;
    GXMat4 y;

    gxSetMat4RotationX(x, pitch_rad);
    gxSetMat4RotationY(y, yaw_rad);

    gxMulMat4Mat4(out, x, y);
}

void __stdcall gxSetMat4RotationXYZ(GXMat4& out, float pitch_rad, float yaw_rad, float roll_rad)
{
    GXMat4 x;
    GXMat4 y;
    GXMat4 z;

    gxSetMat4RotationX(x, pitch_rad);
    gxSetMat4RotationY(y, yaw_rad);
    gxSetMat4RotationZ(z, roll_rad);

    GXMat4 temp;
    gxMulMat4Mat4(temp, x, y);
    gxMulMat4Mat4(out, temp, z);
}

void __stdcall gxSetMat4RotationRelative(GXMat4& out, float xDir, float yDir, float zDir, float angle)
{
    float c;
    float s;

    c = cosf(angle);
    s = sinf(angle);

    float ic = 1.0f - c;

    out.m11 = ic * xDir * xDir + c;
    out.m12 = ic * xDir * yDir - s * zDir;
    out.m13 = ic * xDir * zDir + s * yDir;

    out.m21 = ic * yDir * xDir + s * zDir;
    out.m22 = ic * yDir * yDir + c;
    out.m23 = ic * yDir * zDir - s * xDir;

    out.m31 = ic * zDir * xDir - s * yDir;
    out.m32 = ic * zDir * yDir + s * xDir;
    out.m33 = ic * zDir * zDir + c;

    out.m44 = 1.0f;
    out.m41 = out.m42 = out.m43 = out.m14 = out.m24 = out.m34 = 0.0f;
}

void __stdcall gxSetMat4ClearRotation(GXMat4& out, const GXMat4& mod_mat)
{
    GXVec3 scaleFactor;
    scaleFactor.x = 1.0f / gxLengthVec3(mod_mat.xv);
    scaleFactor.y = 1.0f / gxLengthVec3(mod_mat.yv);
    scaleFactor.z = 1.0f / gxLengthVec3(mod_mat.zv);

    gxMulVec3Scalar(out.xv, mod_mat.xv, scaleFactor.x);
    gxMulVec3Scalar(out.yv, mod_mat.yv, scaleFactor.y);
    gxMulVec3Scalar(out.zv, mod_mat.zv, scaleFactor.z);

    out.m14 = out.m24 = out.m34 = 0.0f;
    out.m41 = out.m42 = out.m43 = 0.0f;

    out.m44 = 1.0f;
}

void __stdcall gxSetMat4Scale(GXMat4& out, float x, float y, float z)
{
    out.m12 = out.m13 = 0.0f;
    out.m21 = out.m23 = 0.0f;
    out.m31 = out.m32 = 0.0f;

    out.m11 = x;
    out.m22 = y;
    out.m33 = z;

    out.m44 = 1.0f;
    out.m41 = out.m42 = out.m43 = out.m14 = out.m24 = out.m34 = 0.0f;
}


void __stdcall gxSetMat4Inverse(GXMat4& out, const GXMat4& src)
{
    float det, invDet;

    // 2x2 sub-determinants required to calculate 4x4 determinant
    float det2_01_01 = src.m[0][0] * src.m[1][1] - src.m[1][0] * src.m[0][1];
    float det2_01_02 = src.m[0][0] * src.m[2][1] - src.m[2][0] * src.m[0][1];
    float det2_01_03 = src.m[0][0] * src.m[3][1] - src.m[3][0] * src.m[0][1];
    float det2_01_12 = src.m[1][0] * src.m[2][1] - src.m[2][0] * src.m[1][1];
    float det2_01_13 = src.m[1][0] * src.m[3][1] - src.m[3][0] * src.m[1][1];
    float det2_01_23 = src.m[2][0] * src.m[3][1] - src.m[3][0] * src.m[2][1];

    // 3x3 sub-determinants required to calculate 4x4 determinant
    float det3_201_012 = src.m[0][2] * det2_01_12 - src.m[1][2] * det2_01_02 + src.m[2][2] * det2_01_01;
    float det3_201_013 = src.m[0][2] * det2_01_13 - src.m[1][2] * det2_01_03 + src.m[3][2] * det2_01_01;
    float det3_201_023 = src.m[0][2] * det2_01_23 - src.m[2][2] * det2_01_03 + src.m[3][2] * det2_01_02;
    float det3_201_123 = src.m[1][2] * det2_01_23 - src.m[2][2] * det2_01_13 + src.m[3][2] * det2_01_12;

    det = (-det3_201_123 * src.m[0][3] + det3_201_023 * src.m[1][3] - det3_201_013 * src.m[2][3]
            + det3_201_012 * src.m[3][3]);

    invDet = 1.0f / det;

    // remaining 2x2 sub-determinants
    float det2_03_01 = src.m[0][0] * src.m[1][3] - src.m[1][0] * src.m[0][3];
    float det2_03_02 = src.m[0][0] * src.m[2][3] - src.m[2][0] * src.m[0][3];
    float det2_03_03 = src.m[0][0] * src.m[3][3] - src.m[3][0] * src.m[0][3];
    float det2_03_12 = src.m[1][0] * src.m[2][3] - src.m[2][0] * src.m[1][3];
    float det2_03_13 = src.m[1][0] * src.m[3][3] - src.m[3][0] * src.m[1][3];
    float det2_03_23 = src.m[2][0] * src.m[3][3] - src.m[3][0] * src.m[2][3];

    float det2_13_01 = src.m[0][1] * src.m[1][3] - src.m[1][1] * src.m[0][3];
    float det2_13_02 = src.m[0][1] * src.m[2][3] - src.m[2][1] * src.m[0][3];
    float det2_13_03 = src.m[0][1] * src.m[3][3] - src.m[3][1] * src.m[0][3];
    float det2_13_12 = src.m[1][1] * src.m[2][3] - src.m[2][1] * src.m[1][3];
    float det2_13_13 = src.m[1][1] * src.m[3][3] - src.m[3][1] * src.m[1][3];
    float det2_13_23 = src.m[2][1] * src.m[3][3] - src.m[3][1] * src.m[2][3];

    // remaining 3x3 sub-determinants
    float det3_203_012 = src.m[0][2] * det2_03_12 - src.m[1][2] * det2_03_02 + src.m[2][2] * det2_03_01;
    float det3_203_013 = src.m[0][2] * det2_03_13 - src.m[1][2] * det2_03_03 + src.m[3][2] * det2_03_01;
    float det3_203_023 = src.m[0][2] * det2_03_23 - src.m[2][2] * det2_03_03 + src.m[3][2] * det2_03_02;
    float det3_203_123 = src.m[1][2] * det2_03_23 - src.m[2][2] * det2_03_13 + src.m[3][2] * det2_03_12;

    float det3_213_012 = src.m[0][2] * det2_13_12 - src.m[1][2] * det2_13_02 + src.m[2][2] * det2_13_01;
    float det3_213_013 = src.m[0][2] * det2_13_13 - src.m[1][2] * det2_13_03 + src.m[3][2] * det2_13_01;
    float det3_213_023 = src.m[0][2] * det2_13_23 - src.m[2][2] * det2_13_03 + src.m[3][2] * det2_13_02;
    float det3_213_123 = src.m[1][2] * det2_13_23 - src.m[2][2] * det2_13_13 + src.m[3][2] * det2_13_12;

    float det3_301_012 = src.m[0][3] * det2_01_12 - src.m[1][3] * det2_01_02 + src.m[2][3] * det2_01_01;
    float det3_301_013 = src.m[0][3] * det2_01_13 - src.m[1][3] * det2_01_03 + src.m[3][3] * det2_01_01;
    float det3_301_023 = src.m[0][3] * det2_01_23 - src.m[2][3] * det2_01_03 + src.m[3][3] * det2_01_02;
    float det3_301_123 = src.m[1][3] * det2_01_23 - src.m[2][3] * det2_01_13 + src.m[3][3] * det2_01_12;

    out.m[0][0] = - det3_213_123 * invDet;
    out.m[0][1] = + det3_213_023 * invDet;
    out.m[0][2] = - det3_213_013 * invDet;
    out.m[0][3] = + det3_213_012 * invDet;

    out.m[1][0] = + det3_203_123 * invDet;
    out.m[1][1] = - det3_203_023 * invDet;
    out.m[1][2] = + det3_203_013 * invDet;
    out.m[1][3] = - det3_203_012 * invDet;

    out.m[2][0] = + det3_301_123 * invDet;
    out.m[2][1] = - det3_301_023 * invDet;
    out.m[2][2] = + det3_301_013 * invDet;
    out.m[2][3] = - det3_301_012 * invDet;

    out.m[3][0] = - det3_201_123 * invDet;
    out.m[3][1] = + det3_201_023 * invDet;
    out.m[3][2] = - det3_201_013 * invDet;
    out.m[3][3] = + det3_201_012 * invDet;
}

void __stdcall gxMulMat4Mat4(GXMat4& out, const GXMat4& a, const GXMat4& b)
{
    out.m11 = a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41;
    out.m12 = a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42;
    out.m13 = a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43;
    out.m14 = a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44;

    out.m21 = a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41;
    out.m22 = a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42;
    out.m23 = a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43;
    out.m24 = a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44;

    out.m31 = a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41;
    out.m32 = a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42;
    out.m33 = a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43;
    out.m34 = a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44;

    out.m41 = a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41;
    out.m42 = a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42;
    out.m43 = a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43;
    out.m44 = a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44;
}

void __stdcall gxMulVec4Mat4(GXVec4 &out, const GXVec4 &v, const GXMat4& m)
{
    out.x = v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + v.w * m.m41;
    out.y = v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + v.w * m.m42;
    out.z = v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + v.w * m.m43;
    out.w = v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + v.w * m.m44;
}

void __stdcall gxMulVec3Mat4AsNormal(GXVec3& out, const GXVec3& v, const GXMat4& m)
{
    out.x = m.m11 * v.x + m.m21 * v.y + m.m31 * v.z;
    out.y = m.m12 * v.x + m.m22 * v.y + m.m32 * v.z;
    out.z = m.m13 * v.x + m.m23 * v.y + m.m33 * v.z;
}

void __stdcall gxMulVec3Mat4AsPoint(GXVec3& out, const GXVec3& v, const GXMat4& m)
{
    out.x = m.m11 * v.x + m.m21 * v.y + m.m31 * v.z + m.m41;
    out.y = m.m12 * v.x + m.m22 * v.y + m.m32 * v.z + m.m42;
    out.z = m.m13 * v.x + m.m23 * v.y + m.m33 * v.z + m.m43;
}

void __stdcall gxGetPerspectiveParams(const GXMat4& m, float &fovy_rad, float &aspect, float &zNear, float &zFar)
{
    fovy_rad = 2.0f * atanf(1.0f / m.m22);
    aspect = m.m22 / m.m11;

    zNear = -m.m43 / (m.m33 + 1.0f);
    zFar = (zNear * m.m43) / (m.m43 + 2.0f * zNear);
}

void __stdcall gxGetOrthoParams(const GXMat4& m, float &width, float &height, float &zNear, float &zFar)
{
    width = 2.0f / m.m11;
    height = 2.0f / m.m22;

    float factor = 1.0f / m.m33;

    zNear = -(1.0f + m.m43) * factor;
    zFar = (2.0f + zNear * m.m33) * factor;
}

void __stdcall gxGetRayPerspective(GXVec3& rayView, const GXMat4& proj_mat, const GXVec2& mouseCVV)
{
    rayView.x = mouseCVV.x / proj_mat.m11;
    rayView.y = mouseCVV.y / proj_mat.m22;
    rayView.z = 1.0f;
}

//-------------------------------------------------------------------

GXMat3::GXMat3()
{
    memset(arr, 0, sizeof(GXMat3));
}

GXMat3::GXMat3(GXMat4& m)
{
    memcpy(arr, m.arr, 3 * sizeof(float));
    memcpy(arr + 3, m.arr + 4, 3 * sizeof(float));
    memcpy(arr + 6, m.arr + 8, 3 * sizeof(float));
}

void GXMat3::operator =(const GXMat3& m)
{
    memcpy(arr, m.arr, sizeof(GXMat4));
}

void __stdcall gxSetMat3Transponse(GXMat3& out, GXMat3& a)
{
    out.m11 = a.m11;
    out.m12 = a.m21;
    out.m13 = a.m31;

    out.m21 = a.m12;
    out.m22 = a.m22;
    out.m23 = a.m32;

    out.m31 = a.m13;
    out.m32 = a.m23;
    out.m33 = a.m33;
}

void __stdcall gxSetMat3FromMat4(GXMat3& out, const GXMat4& m)
{
    out.m11 = m.m11;
    out.m12 = m.m12;
    out.m13 = m.m13;

    out.m21 = m.m21;
    out.m22 = m.m22;
    out.m23 = m.m23;

    out.m31 = m.m31;
    out.m32 = m.m32;
    out.m33 = m.m33;
}

void __stdcall gxSetMat3Identity(GXMat3& out)
{
    out.m11 = out.m22 = out.m33 = 1.0f;
    out.m12 = out.m13 = 0.0f;
    out.m21 = out.m23 = 0.0f;
    out.m31 = out.m32 = 0.0f;
}

void __stdcall gxSetMat3Zero(GXMat3& out)
{
    memset(&out, 0, sizeof(GXMat3));
}

void __stdcall gxSetMat3Inverse(GXMat3& out, const GXMat3& m)
{
    float determinant = m.m11 * (m.m22 * m.m33 - m.m32 * m.m23);
    determinant -= m.m12 * (m.m21 * m.m33 - m.m31 * m.m23);
    determinant += m.m13 * (m.m21 * m.m32 - m.m31 * m.m22);

    float invDeterminant = 1.0f / determinant;

    out.m11 = invDeterminant * (m.m22 * m.m33 - m.m32 * m.m23);
    out.m12 = invDeterminant * (m.m13 * m.m32 - m.m33 * m.m12);
    out.m13 = invDeterminant * (m.m12 * m.m23 - m.m22 * m.m13);

    out.m21 = invDeterminant * (m.m23 * m.m31 - m.m33 * m.m21);
    out.m22 = invDeterminant * (m.m11 * m.m33 - m.m31 * m.m13);
    out.m23 = invDeterminant * (m.m13 * m.m21 - m.m23 * m.m11);

    out.m31 = invDeterminant * (m.m21 * m.m32 - m.m31 * m.m22);
    out.m32 = invDeterminant * (m.m12 * m.m31 - m.m32 * m.m11);
    out.m33 = invDeterminant * (m.m11 * m.m22 - m.m21 * m.m12);
}

void __stdcall gxMulMat3Mat3(GXMat3& out, const GXMat3& a, const GXMat3& b)
{
    out.m11 = a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31;
    out.m12 = a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32;
    out.m13 = a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33;

    out.m21 = a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31;
    out.m22 = a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32;
    out.m23 = a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33;

    out.m31 = a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31;
    out.m32 = a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32;
    out.m33 = a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33;
}

void __stdcall gxMulVec3Mat3(GXVec3& out, const GXVec3& v, const GXMat3& m)
{
    out.x = m.m11 * v.x + m.m21 * v.y + m.m31 * v.z;
    out.y = m.m12 * v.x + m.m22 * v.y + m.m32 * v.z;
    out.z = m.m13 * v.x + m.m23 * v.y + m.m33 * v.z;
}

void __stdcall gxMulMat3Scalar(GXMat3& out, const GXMat3& m, float a)
{
    out.m11 = m.m11 * a;
    out.m12 = m.m12 * a;
    out.m13 = m.m13 * a;

    out.m21 = m.m21 * a;
    out.m22 = m.m22 * a;
    out.m23 = m.m23 * a;

    out.m31 = m.m31 * a;
    out.m32 = m.m32 * a;
    out.m33 = m.m33 * a;
}

void __stdcall gxSumMat3Mat3(GXMat3& out, const GXMat3& a, const GXMat3& b)
{
    out.m11 = a.m11 + b.m11;
    out.m12 = a.m12 + b.m12;
    out.m13 = a.m13 + b.m13;

    out.m21 = a.m21 + b.m21;
    out.m22 = a.m22 + b.m22;
    out.m23 = a.m23 + b.m23;

    out.m31 = a.m31 + b.m31;
    out.m32 = a.m32 + b.m32;
    out.m33 = a.m33 + b.m33;
}

void __stdcall gxSubMat3Mat3(GXMat3& out, const GXMat3& a, const GXMat3& b)
{
    out.m11 = a.m11 - b.m11;
    out.m12 = a.m12 - b.m12;
    out.m13 = a.m13 - b.m13;

    out.m21 = a.m21 - b.m21;
    out.m22 = a.m22 - b.m22;
    out.m23 = a.m23 - b.m23;

    out.m31 = a.m31 - b.m31;
    out.m32 = a.m32 - b.m32;
    out.m33 = a.m33 - b.m33;
}

//----------------------------------------------------------------------------

GXAABB::GXAABB()
{
    vertices = 0;

    min = GXVec3(FLT_MAX, FLT_MAX, FLT_MAX);
    max = GXVec3(FLT_MIN, FLT_MIN, FLT_MIN);
}

float __stdcall gxGetAABBWidth(const GXAABB& bounds)
{
    if (bounds.vertices < 2) return -1.0f;

    return bounds.max.x - bounds.min.x;
}

float __stdcall gxGetAABBHeight(const GXAABB& bounds)
{
    if (bounds.vertices < 2) return -1.0f;

    return bounds.max.y - bounds.min.y;
}

float __stdcall gxGetAABBDepth(const GXAABB& bounds)
{
    if (bounds.vertices < 2) return -1.0f;

    return bounds.max.z - bounds.min.z;
}

void __stdcall gxSetAABBEmpty(GXAABB& inOut)
{
    inOut.vertices = 0;

    inOut.min = GXVec3(FLT_MAX, FLT_MAX, FLT_MAX);
    inOut.max = GXVec3(FLT_MIN, FLT_MIN, FLT_MIN);
}

void __stdcall gxSetAABBTransformed(GXAABB& out, const GXAABB& bounds, const GXMat4& transform)
{
    GXVec3 verticesLocal[8];
    verticesLocal[0] = gx_common::gxCreateVec3(bounds.min.x, bounds.min.y, bounds.min.z);
    verticesLocal[1] = gx_common::gxCreateVec3(bounds.max.x, bounds.min.y, bounds.min.z);
    verticesLocal[2] = gx_common::gxCreateVec3(bounds.max.x, bounds.min.y, bounds.max.z);
    verticesLocal[3] = gx_common::gxCreateVec3(bounds.min.x, bounds.min.y, bounds.max.z);

    verticesLocal[4] = gx_common::gxCreateVec3(bounds.min.x, bounds.max.y, bounds.min.z);
    verticesLocal[5] = gx_common::gxCreateVec3(bounds.max.x, bounds.max.y, bounds.min.z);
    verticesLocal[6] = gx_common::gxCreateVec3(bounds.max.x, bounds.max.y, bounds.max.z);
    verticesLocal[7] = gx_common::gxCreateVec3(bounds.min.x, bounds.max.y, bounds.max.z);

    gxSetAABBEmpty(out);

    for (unsigned char i = 0; i < 8; i++) {
        GXVec3 vertex;
        gxMulVec3Mat4AsPoint(vertex, verticesLocal[i], transform);
        gxAddVertexToAABB(out, vertex);
    }
}

void __stdcall gxAddVertexToAABB(GXAABB& inOut, const GXVec3& vertex)
{
    gxAddVertexToAABB(inOut, vertex.x, vertex.y, vertex.z);
}

void __stdcall gxAddVertexToAABB(GXAABB& inOut, float x, float y, float z)
{
    if (inOut.vertices > 1) {
        if (inOut.min.x > x) {
            inOut.min.x = x;
        } else if (inOut.max.x < x) {
            inOut.max.x = x;
        }

        if (inOut.min.y > y) {
            inOut.min.y = y;
        } else if (inOut.max.y < y) {
            inOut.max.y = y;
        }

        if (inOut.min.z > z) {
            inOut.min.z = z;
        } else if (inOut.max.z < z) {
            inOut.max.z = z;
        }

        return;
    } else if (inOut.vertices == 0)
    {
        inOut.vertices++;

        inOut.min = gxCreateVec3(x, y, z);
        return;
    }

    inOut.vertices++;

    if (inOut.min.x > x) {
        inOut.max.x = inOut.min.x;
        inOut.min.x = x;
    } else {
        inOut.max.x = x;
    }

    if (inOut.min.y > y) {
        inOut.max.y = inOut.min.y;
        inOut.min.y = y;
    } else {
        inOut.max.y = y;
    }

    if (inOut.min.z > z) {
        inOut.max.z = inOut.min.z;
        inOut.min.z = z;
    } else {
        inOut.max.z = z;
    }
}

bool __stdcall gxIsOverlapedAABBAABB(const GXAABB& a, const GXAABB& b)
{
    if (a.min.x > b.max.x) return false;
    if (a.min.y > b.max.y) return false;
    if (a.min.z > b.max.z) return false;

    if (b.min.x > a.max.x) return false;
    if (b.min.y > a.max.y) return false;
    if (b.min.z > a.max.z) return false;

    return true;
}

bool __stdcall gxIsOverlapedAABBVec3(const GXAABB& a, const GXVec3& b)
{
    if (a.min.x > b.x) return false;
    if (a.min.y > b.y) return false;
    if (a.min.z > b.z) return false;

    if (a.max.x < b.x) return false;
    if (a.max.y < b.y) return false;
    if (a.max.z < b.z) return false;

    return true;
}

bool __stdcall gxIsOverlapedAABBVec3(const GXAABB& a, float x, float y, float z)
{
    if (a.min.x > x) return false;
    if (a.min.y > y) return false;
    if (a.min.z > z) return false;

    if (a.max.x < x) return false;
    if (a.max.y < y) return false;
    if (a.max.z < z) return false;

    return true;
}

void __stdcall gxGetAABBCenter(GXVec3& center, const GXAABB& bounds)
{
    center.x = (bounds.min.x + bounds.max.x) * 0.5f;
    center.y = (bounds.min.y + bounds.max.y) * 0.5f;
    center.z = (bounds.min.z + bounds.max.z) * 0.5f;
}

float __stdcall gxGetAABBSphereRadius(const GXAABB& bounds)
{
    GXVec3 center;
    gxGetAABBCenter(center, bounds);
    return gxDistanceVec3Vec3(center, bounds.min);
}

//------------------------------------------------------------------

void GXPlane::from(const GXVec3& a, const GXVec3& b, const GXVec3& c)
{
    GXVec3 ab(b.x - a.x, b.y - a.y, b.z - a.z);
    GXVec3 ac(c.x - a.x, c.y - a.y, c.z - a.z);

    GXVec3 normal;
    gxCrossVec3Vec3(normal, ab, ac);
    gxNormalizeVec3(normal);

    this->a = normal.x;
    this->b = normal.y;
    this->c = normal.z;
    this->d = -gxDotVec3Fast(normal, a);
}

void GXPlane::fromLineToPoint(const GXVec3& lineStart, const GXVec3& lineEnd, const GXVec3& point)
{
    GXVec3 startToPoint;
    gxSubVec3Vec3(startToPoint, point, lineStart);

    GXVec3 startToEnd;
    gxSubVec3Vec3(startToEnd, lineEnd, lineStart);

    GXVec3 tempCross;
    gxCrossVec3Vec3(tempCross, startToEnd, startToPoint);

    GXVec3 normal;
    gxCrossVec3Vec3(normal, tempCross, startToEnd);

    a = normal.x;
    b = normal.y;
    c = normal.z;
    d = -a * lineStart.x - b * lineStart.y - c * lineStart.z;

    if (gxPlaneClassifyVertex(*this, point) != GXPlaneClassifyVertex::InFront) {
        gxFlipPlane(*this);
    }
}

GXPlaneClassifyVertex __stdcall gxPlaneClassifyVertex(const GXPlane& plane, const GXVec3& vertex)
{
    float test = plane.a * vertex.x + plane.b * vertex.y + plane.c * vertex.z + plane.d;

    if (test < 0.0f) return GXPlaneClassifyVertex::Behind;
    if (test > 0.0f) return GXPlaneClassifyVertex::InFront;
    return GXPlaneClassifyVertex::On;
}

GXPlaneClassifyVertex __stdcall gxPlaneClassifyVertex(const GXPlane& plane, float x, float y, float z)
{
    float test = plane.a * x + plane.b * y + plane.c * z + plane.d;

    if (test < 0.0f) return GXPlaneClassifyVertex::Behind;
    if (test > 0.0f) return GXPlaneClassifyVertex::InFront;
    return GXPlaneClassifyVertex::On;
}

void __stdcall gxNormalizePlane(GXPlane& inOut)
{
    float length = sqrtf(inOut.a * inOut.a + inOut.b * inOut.b + inOut.c * inOut.c);

    inOut.a /= length;
    inOut.b /= length;
    inOut.c /= length;
    inOut.d /= length;
}

void __stdcall gxFlipPlane(GXPlane& inOut)
{
    inOut.a = -inOut.a;
    inOut.b = -inOut.b;
    inOut.c = -inOut.c;
    inOut.d = -inOut.d;
}

//------------------------------------------------------------------

GXProjectionClipPlanes::GXProjectionClipPlanes()
{
    memset(_planes, 0, 6 * sizeof(GXPlane));
}

GXProjectionClipPlanes::GXProjectionClipPlanes(const GXMat4& src)
{
    from(src);
}

void GXProjectionClipPlanes::from(const GXMat4& m)
{
    //Left clipping plane
    _planes[0].a = m.m14 + m.m11;
    _planes[0].b = m.m24 + m.m21;
    _planes[0].c = m.m34 + m.m31;
    _planes[0].d = m.m44 + m.m41;

    //Right clipping plane
    _planes[1].a = m.m14 - m.m11;
    _planes[1].b = m.m24 - m.m21;
    _planes[1].c = m.m34 - m.m31;
    _planes[1].d = m.m44 - m.m41;

    //Top clipping plane
    _planes[2].a = m.m14 - m.m12;
    _planes[2].b = m.m24 - m.m22;
    _planes[2].c = m.m34 - m.m32;
    _planes[2].d = m.m44 - m.m42;

    //Bottom clipping plane
    _planes[3].a = m.m14 + m.m12;
    _planes[3].b = m.m24 + m.m22;
    _planes[3].c = m.m34 + m.m32;
    _planes[3].d = m.m44 + m.m42;

    //Near clipping plane
    _planes[4].a = m.m14 + m.m13;
    _planes[4].b = m.m24 + m.m23;
    _planes[4].c = m.m34 + m.m33;
    _planes[4].d = m.m44 + m.m43;

    //Far clipping plane
    _planes[ 5 ].a = m.m14 - m.m13;
    _planes[ 5 ].b = m.m24 - m.m23;
    _planes[ 5 ].c = m.m34 - m.m33;
    _planes[ 5 ].d = m.m44 - m.m43;
}

bool GXProjectionClipPlanes::isVisible(const GXAABB& bounds)
{
    unsigned char flags = planeTest(bounds.min.x, bounds.min.y, bounds.min.z);
    flags &= planeTest(bounds.min.x, bounds.max.y, bounds.min.z);
    flags &= planeTest(bounds.max.x, bounds.max.y, bounds.min.z);
    flags &= planeTest(bounds.max.x, bounds.min.y, bounds.min.z);

    flags &= planeTest(bounds.min.x, bounds.min.y, bounds.max.z);
    flags &= planeTest(bounds.min.x, bounds.max.y, bounds.max.z);
    flags &= planeTest(bounds.max.x, bounds.max.y, bounds.max.z);
    flags &= planeTest(bounds.max.x, bounds.min.y, bounds.max.z);

    return (flags > 0) ? false : true;
}

unsigned char GXProjectionClipPlanes::planeTest(float x, float y, float z)
{
    unsigned char flags = 0;

    for (unsigned char i = 0; i < 6; i++) {
        if (gxPlaneClassifyVertex(_planes[ i ], x, y, z ) == GXPlaneClassifyVertex::Behind)
            flags |= (unsigned char)(1 << i);
    }

    return flags;
}

//------------------------------------------------------------------

float __stdcall gxDegToRad(float degrees)
{
    return degrees * GX_PI_DIV_180;
}

void __stdcall gxColorToVec4(GXVec4 &out, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    out.r = r * GX_1_DIV_256;
    out.g = g * GX_1_DIV_256;
    out.b = b * GX_1_DIV_256;
    out.a = a * GX_1_DIV_256;
}

void __stdcall gxColorToVec3(GXVec3 &out, unsigned char r, unsigned char g, unsigned char b)
{
    out.r = r * GX_1_DIV_256;
    out.g = g * GX_1_DIV_256;
    out.b = b * GX_1_DIV_256;
}

void __stdcall gxConvert3DSMaxToGXEngine(GXVec3& gx_out, float x, float y, float z)
{
    gx_out.x = -x;
    gx_out.y = z;
    gx_out.z = -y;
}

void __stdcall gxConvertXYZBlenderToGXEngine(GXVec3& gx_out, float x, float y, float z)
{
    gx_out.x = x;
    gx_out.y = z;
    gx_out.z = y;
}

void __stdcall gxConvertEulerBlenderToGXEngine(GXVec3& gx_out, float x_deg, float y_deg, float z_deg)
{
    gx_out.pitch_rad = gxDegToRad(90.0f - x_deg);
    gx_out.yaw_rad = gxDegToRad(-z_deg);
    gx_out.roll_rad = gxDegToRad(-y_deg);
}

float __stdcall gxRandomNormalize()
{
#define GX_INV_RAND_MAX        3.05185e-5f;

    return rand () * GX_INV_RAND_MAX;

#undef GX_INV_RAND_MAX
}

void __stdcall gxGetTangentBitangent(GXVec3& outTangent, GXVec3& outBitangent, unsigned char vertexID,
                                     const unsigned char* vertices, unsigned int vertexStride,
                                     const unsigned char* uvs, unsigned int uvStride)
{
    const GXVec3* v0;
    const GXVec3* v1;
    const GXVec3* v2;

    const GXVec2* uv0;
    const GXVec2* uv1;
    const GXVec2* uv2;

    switch (vertexID) {
        case 0: {
            v0 = (const GXVec3*)vertices;
            v1 = (const GXVec3*)( vertices + vertexStride );
            v2 = (const GXVec3*)( vertices + 2 * vertexStride );

            uv0 = (const GXVec2*)uvs;
            uv1 = (const GXVec2*)( uvs + uvStride );
            uv2 = (const GXVec2*)( uvs + 2 * uvStride );

            break;
        }
        case 1: {
            v0 = (const GXVec3*)( vertices + vertexStride );
            v1 = (const GXVec3*)( vertices + 2 * vertexStride );
            v2 = (const GXVec3*)vertices;

            uv0 = (const GXVec2*)( uvs + uvStride );
            uv1 = (const GXVec2*)( uvs + 2 * uvStride );
            uv2 = (const GXVec2*)uvs;

            break;
        }
        case 2:
        default: {
            v0 = (const GXVec3*)( vertices + 2 * vertexStride );
            v1 = (const GXVec3*)vertices;
            v2 = (const GXVec3*)( vertices + vertexStride );

            uv0 = (const GXVec2*)( uvs + 2 * uvStride );
            uv1 = (const GXVec2*)uvs;
            uv2 = (const GXVec2*)( uvs + uvStride );

            break;
        }
    }

    GXVec3 a;
    GXVec3 b;
    gxSubVec3Vec3(a, *v1, *v0);
    gxSubVec3Vec3(b, *v2, *v0);

    GXVec2 dUVa;
    GXVec2 dUVb;
    gxSubVec2Vec2(dUVa, *uv1, *uv0);
    gxSubVec2Vec2(dUVb, *uv2, *uv0);

    float factor = 1.0f / (dUVa.u * dUVb.v - dUVb.u * dUVa.v);

    outTangent.x = factor * (dUVb.v * a.x - dUVa.v * b.x);
    outTangent.y = factor * (dUVb.v * a.y - dUVa.v * b.y);
    outTangent.z = factor * (dUVb.v * a.z - dUVa.v * b.z);
    gxNormalizeVec3(outTangent);

    outBitangent.x = factor * (-dUVb.u * a.x + dUVa.u * b.x);
    outBitangent.y = factor * (-dUVb.u * a.y + dUVa.u * b.y);
    outBitangent.z = factor * (-dUVb.u * a.z + dUVa.u * b.z);
    gxNormalizeVec3(outBitangent);
}

float __stdcall gxClampf(float value, float minValue, float maxValue)
{
    return (value < minValue) ? minValue : (value > maxValue) ? maxValue : value;
}

unsigned int __stdcall gxClampi(int value, int minValue, int maxValue)
{
    return (value < minValue) ? minValue : (value > maxValue) ? maxValue : value;
}

void __stdcall gxGetBarycentricCoords(GXVec3& out, const GXVec3& point,
                                      const GXVec3& a, const GXVec3& b, const GXVec3& c)
{
    GXVec3 v0;
    GXVec3 v1;
    GXVec3 v2;

    gxSubVec3Vec3(v0, b, a);
    gxSubVec3Vec3(v1, c, a);
    gxSubVec3Vec3(v2, point, a);

    float d00 = gxDotVec3Fast(v0, v0);
    float d01 = gxDotVec3Fast(v0, v1);
    float d11 = gxDotVec3Fast(v1, v1);
    float d20 = gxDotVec3Fast(v2, v0);
    float d21 = gxDotVec3Fast(v2, v1);

    float denom = 1.0f / (d00 * d11 - d01 * d01);

    out.y = (d11 * d20 - d01 * d21) * denom;
    out.z = (d00 * d21 - d01 * d20) * denom;
    out.x = 1.0f - out.y - out.z;
}

} // namespace gx_common
