#ifndef GX_MATH_H
#define GX_MATH_H

#define GX_MATH_TWO_PI    6.2831853f
#define GX_MATH_PI        3.1415927f
#define GX_MATH_HALFPI    1.5707963f

namespace gx_common {

struct GXVec2 {
    union {
        struct {
            float x;
            float y;
        };
        struct {
            float u;
            float v;
        };
        float arr[2];
    };
};

GXVec2 __stdcall gxCreateVec2(float component_1, float component_2);
void __stdcall gxSubVec2Vec2(GXVec2& out, const GXVec2& a, const GXVec2& b);

//-------------------------------------------------------------

struct GXVec3 {
    explicit GXVec3();
    explicit GXVec3(float component_1, float component_2, float component_3);
    void operator =(const GXVec3& v);

    union {
        struct {
            float x;
            float y;
            float z;
        };
        struct {
            float r;
            float g;
            float b;
        };
        struct {
            float pitch_rad;
            float yaw_rad;
            float roll_rad;
        };
        float arr[3];
    };
};

GXVec3 __stdcall gxCreateVec3(float component_1, float component_2, float component_3);
void __stdcall gxNormalizeVec3(GXVec3& inOut);
void __stdcall gxSumVec3Vec3(GXVec3& out, const GXVec3& a, const GXVec3& b);
void __stdcall gxSubVec3Vec3(GXVec3& out, const GXVec3& a, const GXVec3& b);
void __stdcall gxMulVec3Vec3(GXVec3& out, const GXVec3& a, const GXVec3& b);
void __stdcall gxMulVec3Scalar(GXVec3& out, const GXVec3& v, float factor);
float __stdcall gxDotVec3Fast(const GXVec3& a, const GXVec3& b);                //"a" and "b" must be unit vectors
void __stdcall gxCrossVec3Vec3(GXVec3& out, const GXVec3& a, const GXVec3& b);
float __stdcall gxLengthVec3(const GXVec3& v);
float __stdcall gxDistanceVec3Vec3(const GXVec3& a, const GXVec3& b);
void __stdcall gxProjectVec3Vec3(GXVec3& projection, const GXVec3& vector, const GXVec3& unitVector);

//-------------------------------------------------------------

struct GXVec4
{
    explicit GXVec4();
    explicit GXVec4(GXVec3& v, float component_4);
    explicit GXVec4(float component_1, float component_2, float component_3, float component_4);
    void operator =(const GXVec4 &v);

    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        struct
        {
            float r;
            float g;
            float b;
            float a;
        };
        float arr[4];
    };
};

GXVec4 __stdcall gxCreateVec4(float component_1, float component_2, float component_3, float component_4);

//-------------------------------------------------------------

struct GXMat4;
struct GXMat3;

typedef GXVec4 GXQuat;

GXQuat __stdcall gxCreateQuat(const GXMat4& mat);

void __stdcall gxSetQuatIdentity(GXQuat& out);
void __stdcall gxSetQuatRotationAxis(GXQuat& out, float x, float y, float z, float angle);
void __stdcall gxSetQuatRotationAxis(GXQuat& out, const GXVec3& axis, float angle);

void __stdcall gxQuatRehandCoordinateSystem(GXQuat& out, const GXQuat& src);
void __stdcall gxQuatToEulerAngles(GXVec3& out_rad, const GXQuat& q);    //TODO

void __stdcall gxMulQuatQuat(GXQuat& out, const GXQuat& a, const GXQuat& b);
void __stdcall gxSumQuatQuat(GXQuat& out, const GXQuat& a, const GXQuat& b);
void __stdcall gxSubQuatQuat(GXQuat& out, const GXQuat& a, const GXQuat& b);
void __stdcall gxQuatSLerp(GXQuat& out, const GXQuat& a, const GXQuat& b, float k);
void __stdcall gxInverseQuat(GXQuat& out, const GXQuat& q);
void __stdcall gxNormalizeQuat(GXQuat& out, GXQuat& q);
void __stdcall gxQuatTransform(GXVec3& out, const GXQuat& q, const GXVec3& v);

//-------------------------------------------------------------

struct GXMat4 {
    explicit GXMat4();
    explicit GXMat4(float* v16);
    void operator =(const GXMat4& M);
    void setRotation(const GXQuat& q);
    void setOrigin(const GXVec3& origin);
    void from(const GXQuat& quat, const GXVec3& origin);
    void from(const GXMat3& rotation, const GXVec3& origin);

    union {
        float arr[16];
        float m[4][4];
        struct {
            float m11, m12, m13, m14;
            float m21, m22, m23, m24;
            float m31, m32, m33, m34;
            float m41, m42, m43, m44;
        };
        struct {
            GXVec3 xv; float xw;
            GXVec3 yv; float yw;
            GXVec3 zv; float zw;
            GXVec3 wv; float ww;
        };
    };
};

void __stdcall gxSetMat4Identity(GXMat4& m);
void __stdcall gxSetMat4Perspective(GXMat4& out, float fovy_rad, float aspect, float znear, float zfar);
void __stdcall gxSetMat4Ortho(GXMat4& out, float width, float height, float znear, float zfar);
void __stdcall gxSetMat4Translation(GXMat4& out, float x, float y, float z);
void __stdcall gxSetMat4TranslateTo(GXMat4& out, float x, float y, float z);
void __stdcall gxSetMat4TranslateTo(GXMat4& out, const GXVec3& location);
void __stdcall gxSetMat4RotationX(GXMat4& out, float angle);
void __stdcall gxSetMat4RotationY(GXMat4& out, float angle);
void __stdcall gxSetMat4RotationZ(GXMat4& out, float angle);
void __stdcall gxSetMat4RotationXY(GXMat4& out, float pitch_rad, float yaw_rad);
void __stdcall gxSetMat4RotationXYZ(GXMat4& out, float pitch_rad, float yaw_rad, float roll_rad);
void __stdcall gxSetMat4RotationRelative(GXMat4& out, float xDir, float yDir, float zDir, float angle);
void __stdcall gxSetMat4ClearRotation(GXMat4& out, const GXMat4& mod_mat);
void __stdcall gxSetMat4Scale(GXMat4& out, float x, float y, float z);
void __stdcall gxSetMat4Inverse(GXMat4& out, const GXMat4& src);

void __stdcall gxMulMat4Mat4(GXMat4& out, const GXMat4& a, const GXMat4& b);
void __stdcall gxMulVec4Mat4(GXVec4 &out, const GXVec4 &v, const GXMat4& m);
void __stdcall gxMulVec3Mat4AsNormal(GXVec3& out, const GXVec3& v, const GXMat4& m);
void __stdcall gxMulVec3Mat4AsPoint(GXVec3& out, const GXVec3& v, const GXMat4& m);

void __stdcall gxGetPerspectiveParams(const GXMat4& m, float &fovy_rad, float &aspect, float &zNear, float &zFar);
void __stdcall gxGetOrthoParams(const GXMat4& m, float &width, float &height, float &zNear, float &zFar);

void __stdcall gxGetRayPerspective(GXVec3& rayView, const GXMat4& proj_mat, const GXVec2& mouseCVV );

//-------------------------------------------------------------

struct GXMat3 {
    explicit GXMat3();
    explicit GXMat3(GXMat4& m);
    void operator =(const GXMat3& m);

    union {
        float arr[9];
        float m[3][3];
        struct {
            float m11, m12, m13;
            float m21, m22, m23;
            float m31, m32, m33;
        };
        struct {
            GXVec3 xv;
            GXVec3 yv;
            GXVec3 zv;
        };
    };
};

void __stdcall gxSetMat3Transponse(GXMat3& out, GXMat3& a);
void __stdcall gxSetMat3FromMat4(GXMat3& out, const GXMat4& m);
void __stdcall gxSetMat3Identity(GXMat3& out);
void __stdcall gxSetMat3Inverse(GXMat3& out, const GXMat3& m);
void __stdcall gxSetMat3Zero(GXMat3& out);
void __stdcall gxMulMat3Mat3(GXMat3& out, const GXMat3& a, const GXMat3& b);
void __stdcall gxMulVec3Mat3(GXVec3& out, const GXVec3& v, const GXMat3& m);
void __stdcall gxMulMat3Scalar(GXMat3& out, const GXMat3& m, float a);
void __stdcall gxSumMat3Mat3(GXMat3& out, const GXMat3& a, const GXMat3& b);
void __stdcall gxSubMat3Mat3(GXMat3& out, const GXMat3& a, const GXMat3& b);

//-------------------------------------------------------------

struct GXAABB {
    GXAABB();

    unsigned char vertices;
    GXVec3 min;
    GXVec3 max;
};

void __stdcall gxSetAABBEmpty(GXAABB& inOut);
void __stdcall gxSetAABBTransformed(GXAABB& out, const GXAABB& bounds, const GXMat4& transform);
void __stdcall gxAddVertexToAABB(GXAABB& inOut, const GXVec3& vertex);
void __stdcall gxAddVertexToAABB(GXAABB& inOut, float x, float y, float z);
bool __stdcall gxIsOverlapedAABBAABB(const GXAABB& a, const GXAABB& b);
bool __stdcall gxIsOverlapedAABBVec3(const GXAABB& a, const GXVec3& b);
bool __stdcall gxIsOverlapedAABBVec3(const GXAABB& a, float x, float y, float z);
void __stdcall gxGetAABBCenter(GXVec3& c, const GXAABB &bounds);
float __stdcall gxGetAABBWidth(const GXAABB& bounds);
float __stdcall gxGetAABBHeight(const GXAABB& bounds);
float __stdcall gxGetAABBDepth(const GXAABB& bounds);
float __stdcall gxGetAABBSphereRadius(const GXAABB& bounds);

//-------------------------------------------------------------

enum class GXPlaneClassifyVertex {
    InFront,
    Behind,
    On
};

struct GXPlane {
    float a;
    float b;
    float c;
    float d;

    void from(const GXVec3& a, const GXVec3& b, const GXVec3& c);
    void fromLineToPoint(const GXVec3& lineStart, const GXVec3& lineEnd, const GXVec3& point);
};

GXPlaneClassifyVertex __stdcall gxPlaneClassifyVertex(const GXPlane& plane, const GXVec3& vertex);
GXPlaneClassifyVertex __stdcall gxPlaneClassifyVertex(const GXPlane& plane, float x, float y, float z);
void __stdcall gxNormalizePlane(GXPlane& inOut);
void __stdcall gxFlipPlane(GXPlane& inOut);

//-------------------------------------------------------------

class GXProjectionClipPlanes {
public:
    explicit GXProjectionClipPlanes();
    explicit GXProjectionClipPlanes(const GXMat4& src);

    void from(const GXMat4& src);            //Normals will be directed inside view volume
    bool isVisible(const GXAABB& bounds);    //Trivial invisibility test

private:
    unsigned char planeTest(float x, float y, float z);

    GXPlane _planes[6];
};

//-------------------------------------------------------------

float __stdcall gxDegToRad(float degrees);
void __stdcall gxColorToVec4(GXVec4 &out, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void __stdcall gxColorToVec3(GXVec3 &out, unsigned char r, unsigned char g, unsigned char b);
void __stdcall gxConvert3DSMaxToGXEngine(GXVec3& gx_out, float x, float y, float z);
void __stdcall gxConvertXYZBlenderToGXEngine(GXVec3& gx_out, float x, float y, float z);
void __stdcall gxConvertEulerBlenderToGXEngine(GXVec3& gx_out, float x_deg, float y_deg, float z_deg);
float __stdcall gxRandomNormalize();
void __stdcall gxGetTangentBitangent(GXVec3& outTangent, GXVec3& outBitangent, unsigned char vertexID,
                                     const unsigned char* vertices, unsigned int vertexStride,
                                     const unsigned char* uvs, unsigned int uvStride);

float __stdcall gxClampf(float value, float minValue, float maxValue);
unsigned int __stdcall gxClampi(int value, int minValue, int maxValue);
void __stdcall gxGetBarycentricCoords(GXVec3& out, const GXVec3& point,
                                            const GXVec3& a, const GXVec3& b, const GXVec3& c);

} // namespace gx_common

#endif GX_MATH_H
