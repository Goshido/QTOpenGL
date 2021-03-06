#include <GXEngine/opengl.h>
#include <stdio.h>

#define OPENGL_GET_PROC(p,n)                    \
{                                               \
    n = (p)wglGetProcAddress(#n);               \
    if (!n)                                     \
        printf("%s not found\n", #n);           \
}

PFNWGLCREATECONTEXTATTRIBSARBPROC   wglCreateContextAttribsARB  = nullptr;
PFNWGLSWAPINTERVALEXTPROC           wglSwapIntervalEXT          = nullptr;
PFNGLGETSHADERIVPROC                glGetShaderiv               = nullptr;
PFNGLGETPROGRAMIVPROC               glGetProgramiv              = nullptr;
PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog          = nullptr;
PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog         = nullptr;
PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays           = nullptr;
PFNGLBINDVERTEXARRAYPROC            glBindVertexArray           = nullptr;
PFNGLGENBUFFERSPROC                 glGenBuffers                = nullptr;
PFNGLBINDBUFFERPROC                 glBindBuffer                = nullptr;
PFNGLBUFFERDATAPROC                 glBufferData                = nullptr;
PFNGLCREATEPROGRAMPROC              glCreateProgram             = nullptr;
PFNGLCREATESHADERPROC               glCreateShader              = nullptr;
PFNGLSHADERSOURCEPROC               glShaderSource              = nullptr;
PFNGLCOMPILESHADERPROC              glCompileShader             = nullptr;
PFNGLATTACHSHADERPROC               glAttachShader              = nullptr;
PFNGLLINKPROGRAMPROC                glLinkProgram               = nullptr;
PFNGLUSEPROGRAMPROC                 glUseProgram                = nullptr;
PFNGLVALIDATEPROGRAMPROC            glValidateProgram           = nullptr;
PFNGLGETATTRIBLOCATIONPROC          glGetAttribLocation         = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer       = nullptr;
PFNGLVERTEXATTRIB4NUBPROC           glVertexAttrib4Nub          = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray   = nullptr;
PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation        = nullptr;
PFNGLDELETESHADERPROC               glDeleteShader              = nullptr;
PFNGLDELETEPROGRAMPROC              glDeleteProgram             = nullptr;
PFNGLUNIFORM1IPROC                  glUniform1i                 = nullptr;
PFNGLUNIFORM1FPROC                  glUniform1f                 = nullptr;
PFNGLUNIFORM1FVPROC                 glUniform1fv                = nullptr;
PFNGLUNIFORM2FVPROC                 glUniform2fv                = nullptr;
PFNGLUNIFORM3FVPROC                 glUniform3fv                = nullptr;
PFNGLUNIFORM4FPROC                  glUniform4f                 = nullptr;
PFNGLUNIFORM4FVPROC                 glUniform4fv                = nullptr;
PFNGLUNIFORMMATRIX3FVPROC           glUniformMatrix3fv          = nullptr;
PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv          = nullptr;
PFNGLDELETEVERTEXARRAYSPROC         glDeleteVertexArrays        = nullptr;
PFNGLDELETEBUFFERSPROC              glDeleteBuffers             = nullptr;
PFNGLGENFRAMEBUFFERSPROC            glGenFramebuffers           = nullptr;
PFNGLBINDFRAMEBUFFERPROC            glBindFramebuffer           = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC       glFramebufferTexture2D      = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC         glDeleteFramebuffers        = nullptr;
PFNGLACTIVETEXTUREPROC              glActiveTexture             = nullptr;
PFNGLBINDRENDERBUFFERPROC           glBindRenderbuffer          = nullptr;
PFNGLGENRENDERBUFFERSPROC           glGenRenderbuffers          = nullptr;
PFNGLRENDERBUFFERSTORAGEPROC        glRenderbufferStorage       = nullptr;
PFNGLFRAMEBUFFERRENDERBUFFERPROC    glFramebufferRenderbuffer   = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC     glCheckFramebufferStatus    = nullptr;
PFNGLDELETERENDERBUFFERSPROC        glDeleteRenderbuffers       = nullptr;
PFNGLDRAWBUFFERSPROC                glDrawBuffers               = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray  = nullptr;
PFNGLDRAWRANGEELEMENTSPROC          glDrawRangeElements         = nullptr;
PFNGLBLENDEQUATIONPROC              glBlendEquation             = nullptr;
PFNGLBLENDFUNCSEPARATEPROC          glBlendFuncSeparate         = nullptr;
PFNGLGENERATEMIPMAPPROC             glGenerateMipmap            = nullptr;
PFNGLSTENCILOPSEPARATEPROC          glStencilOpSeparate         = nullptr;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC  glTransformFeedbackVaryings = nullptr;
PFNGLBINDBUFFERBASEPROC             glBindBufferBase            = nullptr;
PFNGLBINDBUFFERRANGEPROC            glBindBufferRange           = nullptr;
PFNGLBEGINTRANSFORMFEEDBACKPROC     glBeginTransformFeedback    = nullptr;
PFNGLENDTRANSFORMFEEDBACKPROC       glEndTransformFeedback      = nullptr;
PFNGLTEXIMAGE3DPROC                 glTexImage3D                = nullptr;
PFNGLBINDATTRIBLOCATIONPROC         glBindAttribLocation        = nullptr;
PFNGLBINDFRAGDATALOCATIONPROC       glBindFragDataLocation      = nullptr;

namespace gx_engine {

void __stdcall initOpenGLBackend()
{
    OPENGL_GET_PROC(PFNWGLCREATECONTEXTATTRIBSARBPROC,  wglCreateContextAttribsARB  );
    OPENGL_GET_PROC(PFNWGLSWAPINTERVALEXTPROC,          wglSwapIntervalEXT          );
    OPENGL_GET_PROC(PFNGLGETSHADERIVPROC,               glGetShaderiv               );
    OPENGL_GET_PROC(PFNGLGETPROGRAMIVPROC,              glGetProgramiv              );
    OPENGL_GET_PROC(PFNGLGETSHADERINFOLOGPROC,          glGetShaderInfoLog          );
    OPENGL_GET_PROC(PFNGLGETPROGRAMINFOLOGPROC,         glGetProgramInfoLog         );
    OPENGL_GET_PROC(PFNGLGENVERTEXARRAYSPROC,           glGenVertexArrays           );
    OPENGL_GET_PROC(PFNGLBINDVERTEXARRAYPROC,           glBindVertexArray           );
    OPENGL_GET_PROC(PFNGLGENBUFFERSPROC,                glGenBuffers                );
    OPENGL_GET_PROC(PFNGLBINDBUFFERPROC,                glBindBuffer                );
    OPENGL_GET_PROC(PFNGLBUFFERDATAPROC,                glBufferData                );
    OPENGL_GET_PROC(PFNGLCREATEPROGRAMPROC,             glCreateProgram             );
    OPENGL_GET_PROC(PFNGLCREATESHADERPROC,              glCreateShader              );
    OPENGL_GET_PROC(PFNGLSHADERSOURCEPROC,              glShaderSource              );
    OPENGL_GET_PROC(PFNGLCOMPILESHADERPROC,             glCompileShader             );
    OPENGL_GET_PROC(PFNGLATTACHSHADERPROC,              glAttachShader              );
    OPENGL_GET_PROC(PFNGLLINKPROGRAMPROC,               glLinkProgram               );
    OPENGL_GET_PROC(PFNGLUSEPROGRAMPROC,                glUseProgram                );
    OPENGL_GET_PROC(PFNGLVALIDATEPROGRAMPROC,           glValidateProgram           );
    OPENGL_GET_PROC(PFNGLGETATTRIBLOCATIONPROC,         glGetAttribLocation         );
    OPENGL_GET_PROC(PFNGLVERTEXATTRIBPOINTERPROC,       glVertexAttribPointer       );
    OPENGL_GET_PROC(PFNGLVERTEXATTRIB4NUBPROC,          glVertexAttrib4Nub          );
    OPENGL_GET_PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC,   glEnableVertexAttribArray   );
    OPENGL_GET_PROC(PFNGLGETUNIFORMLOCATIONPROC,        glGetUniformLocation        );
    OPENGL_GET_PROC(PFNGLDELETESHADERPROC,              glDeleteShader              );
    OPENGL_GET_PROC(PFNGLDELETEPROGRAMPROC,             glDeleteProgram             );
    OPENGL_GET_PROC(PFNGLUNIFORM1IPROC,                 glUniform1i                 );
    OPENGL_GET_PROC(PFNGLUNIFORM1FPROC,                 glUniform1f                 );
    OPENGL_GET_PROC(PFNGLUNIFORM1FVPROC,                glUniform1fv                );
    OPENGL_GET_PROC(PFNGLUNIFORM2FVPROC,                glUniform2fv                );
    OPENGL_GET_PROC(PFNGLUNIFORM3FVPROC,                glUniform3fv                );
    OPENGL_GET_PROC(PFNGLUNIFORM4FPROC,                 glUniform4f                 );
    OPENGL_GET_PROC(PFNGLUNIFORM4FVPROC,                glUniform4fv                );
    OPENGL_GET_PROC(PFNGLUNIFORMMATRIX3FVPROC,          glUniformMatrix3fv          );
    OPENGL_GET_PROC(PFNGLUNIFORMMATRIX4FVPROC,          glUniformMatrix4fv          );
    OPENGL_GET_PROC(PFNGLDELETEVERTEXARRAYSPROC,        glDeleteVertexArrays        );
    OPENGL_GET_PROC(PFNGLDELETEBUFFERSPROC,             glDeleteBuffers             );
    OPENGL_GET_PROC(PFNGLGENFRAMEBUFFERSPROC,           glGenFramebuffers           );
    OPENGL_GET_PROC(PFNGLBINDFRAMEBUFFERPROC,           glBindFramebuffer           );
    OPENGL_GET_PROC(PFNGLFRAMEBUFFERTEXTURE2DPROC,      glFramebufferTexture2D      );
    OPENGL_GET_PROC(PFNGLDELETEFRAMEBUFFERSPROC,        glDeleteFramebuffers        );
    OPENGL_GET_PROC(PFNGLACTIVETEXTUREPROC,             glActiveTexture             );
    OPENGL_GET_PROC(PFNGLBINDRENDERBUFFERPROC,          glBindRenderbuffer          );
    OPENGL_GET_PROC(PFNGLGENRENDERBUFFERSPROC,          glGenRenderbuffers          );
    OPENGL_GET_PROC(PFNGLRENDERBUFFERSTORAGEPROC,       glRenderbufferStorage       );
    OPENGL_GET_PROC(PFNGLFRAMEBUFFERRENDERBUFFERPROC,   glFramebufferRenderbuffer   );
    OPENGL_GET_PROC(PFNGLCHECKFRAMEBUFFERSTATUSPROC,    glCheckFramebufferStatus    );
    OPENGL_GET_PROC(PFNGLDELETERENDERBUFFERSPROC,       glDeleteRenderbuffers       );
    OPENGL_GET_PROC(PFNGLDRAWBUFFERSPROC,               glDrawBuffers               );
    OPENGL_GET_PROC(PFNGLDISABLEVERTEXATTRIBARRAYPROC,  glDisableVertexAttribArray  );
    OPENGL_GET_PROC(PFNGLDRAWRANGEELEMENTSPROC,         glDrawRangeElements         );
    OPENGL_GET_PROC(PFNGLBLENDEQUATIONPROC,             glBlendEquation             );
    OPENGL_GET_PROC(PFNGLBLENDFUNCSEPARATEPROC,         glBlendFuncSeparate         );
    OPENGL_GET_PROC(PFNGLGENERATEMIPMAPPROC,            glGenerateMipmap            );
    OPENGL_GET_PROC(PFNGLSTENCILOPSEPARATEPROC,         glStencilOpSeparate         );
    OPENGL_GET_PROC(PFNGLTRANSFORMFEEDBACKVARYINGSPROC, glTransformFeedbackVaryings );
    OPENGL_GET_PROC(PFNGLBINDBUFFERBASEPROC,            glBindBufferBase            );
    OPENGL_GET_PROC(PFNGLBINDBUFFERRANGEPROC,           glBindBufferRange           );
    OPENGL_GET_PROC(PFNGLBEGINTRANSFORMFEEDBACKPROC,    glBeginTransformFeedback    );
    OPENGL_GET_PROC(PFNGLENDTRANSFORMFEEDBACKPROC,      glEndTransformFeedback      );
    OPENGL_GET_PROC(PFNGLTEXIMAGE3DPROC,                glTexImage3D                );
    OPENGL_GET_PROC(PFNGLBINDATTRIBLOCATIONPROC,        glBindAttribLocation        );
    OPENGL_GET_PROC(PFNGLBINDFRAGDATALOCATIONPROC,      glBindFragDataLocation      );
}

void __stdcall checkOpenGLError()
{
    GLenum error;
    if ((error = glGetError()) != GL_NO_ERROR)
        printf("OpenGL::Error 0x%08X\n",(unsigned int)error);
}

} // namespace gx_engine
