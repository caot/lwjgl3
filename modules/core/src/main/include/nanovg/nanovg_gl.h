//
// Copyright (c) 2009-2013 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
#ifndef NANOVG_GL_H
#define NANOVG_GL_H

#ifdef __cplusplus
extern "C" {
#endif

// Create flags

enum NVGcreateFlags {
	// Flag indicating if geometry based anti-aliasing is used (may not be needed when using MSAA).
	NVG_ANTIALIAS 		= 1<<0,
	// Flag indicating if strokes should be drawn using stencil buffer. The rendering will be a little
	// slower, but path overlaps (i.e. self-intersecting or sharp turns) will be drawn just once.
	NVG_STENCIL_STROKES	= 1<<1,
	// Flag indicating that additional debug checks are done.
	NVG_DEBUG 			= 1<<2,
};

#if defined NANOVG_GL2_IMPLEMENTATION
#  define NANOVG_GL2 1
#  define NANOVG_GL_IMPLEMENTATION 1
#  define EXT(name) name##GL2
#  define CONFIG_METHOD "configGL"
#elif defined NANOVG_GL3_IMPLEMENTATION
#  define NANOVG_GL3 1
#  define NANOVG_GL_IMPLEMENTATION 1
#  define NANOVG_GL_USE_UNIFORMBUFFER 1
#  define EXT(name) name##GL3
#  define CONFIG_METHOD "configGL"
#elif defined NANOVG_GLES2_IMPLEMENTATION
#  define NANOVG_GLES2 1
#  define NANOVG_GL_IMPLEMENTATION 1
#  define EXT(name) name##GLES2
#  define CONFIG_METHOD "configGLES"
#elif defined NANOVG_GLES3_IMPLEMENTATION
#  define NANOVG_GLES3 1
#  define NANOVG_GL_IMPLEMENTATION 1
#  define EXT(name) name##GLES3
#  define CONFIG_METHOD "configGLES"
#endif

#define NANOVG_GL_USE_STATE_FILTER (1)

// Creates NanoVG contexts for different OpenGL (ES) versions.
// Flags should be combination of the create flags above.

// These are additional flags on top of NVGimageFlags.
enum NVGimageFlagsGL {
	NVG_IMAGE_NODELETE			= 1<<16,	// Do not delete GL texture handle.
};

#ifdef __cplusplus
}
#endif

#endif /* NANOVG_GL_H */

#ifdef NANOVG_GL_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "nanovg.h"
#include <jni.h>

// ---------- LWJGL --------------

typedef void GLvoid;
typedef unsigned char GLboolean;
typedef char GLchar;
typedef int GLsizei;
typedef int GLint;
typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef float GLfloat;

typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

#define GL_ALWAYS                         0x0207
#define GL_ARRAY_BUFFER                   0x8892
#define GL_BACK                           0x0405
#define GL_BLEND                          0x0BE2
#define GL_CCW                            0x0901
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_COLOR                          0x1800
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_COMPILE_STATUS                 0x8B81
#define GL_CULL_FACE                      0x0B44
#define GL_DECR                           0x1E03
#define GL_DECR_WRAP                      0x8508
#define GL_DEPTH                          0x1801
#define GL_DEPTH_TEST                     0x0B71
#define GL_DST_ALPHA                      0x0304
#define GL_DST_COLOR                      0x0306
#define GL_EQUAL                          0x0202
#define GL_FALSE                          0
#define GL_FLOAT                          0x1406
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_FRAMEBUFFER                    0x8D40
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRONT                          0x0404
#define GL_GENERATE_MIPMAP                0x8191
#define GL_INCR                           0x1E02
#define GL_INCR_WRAP                      0x8507
#define GL_INVALID_ENUM                   0x0500
#define GL_KEEP                           0x1E00
#define GL_LINE                           0x1B01
#define GL_LINEAR                         0x2601
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
#define GL_LINK_STATUS                    0x8B82
#define GL_LUMINANCE                      0x1909
#define GL_NOTEQUAL                       0x0205
#define GL_NO_ERROR                       0
#define GL_ONE                            1
#define GL_ONE_MINUS_DST_ALPHA            0x0305
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_R8                             0x8229
#define GL_RED                            0x1903
#define GL_RENDERBUFFER                   0x8D41
#define GL_RENDERBUFFER_BINDING           0x8CA7
#define GL_REPEAT                         0x2901
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_SCISSOR_TEST                   0x0C11
#define GL_SRC_ALPHA                      0x0302
#define GL_SRC_ALPHA_SATURATE             0x0308
#define GL_SRC_COLOR                      0x0300
#define GL_STENCIL                        0x1802
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_STENCIL_INDEX                  0x1901
#define GL_STENCIL_INDEX8                 0x8D48
#define GL_STENCIL_TEST                   0x0B90
#define GL_STREAM_DRAW                    0x88E0
#define GL_TEXTURE                        0x1702
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_FAN                   0x0006
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRUE                           1
#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_UNPACK_ROW_LENGTH              0x0CF2
#define GL_UNPACK_SKIP_PIXELS             0x0CF4
#define GL_UNPACK_SKIP_ROWS               0x0CF3
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_VERTEX_SHADER                  0x8B31
#define GL_ZERO                           0

#ifndef APIENTRY
 #ifdef _WIN32
  #define APIENTRY __stdcall
 #else
  #define APIENTRY
 #endif
#endif
#define GLAPI

typedef void (APIENTRY *glActiveTexturePROC) (GLenum texture);
typedef void (APIENTRY *glAttachShaderPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY *glBindAttribLocationPROC) (GLuint program, GLuint index, const GLchar *name);
typedef void (APIENTRY *glBindBufferPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY *glBindBufferRangePROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (APIENTRY *glBindFramebufferPROC) (GLenum target, GLuint framebuffer);
typedef void (APIENTRY *glBindRenderbufferPROC) (GLenum target, GLuint renderbuffer);
typedef void (APIENTRY *glBindTexturePROC) (GLenum target, GLuint texture);
typedef void (APIENTRY *glBindVertexArrayPROC) (GLuint array);
typedef void (APIENTRY *glBlendFuncPROC) (GLenum sfactor, GLenum dfactor);
typedef void (APIENTRY *glBlendFuncSeparatePROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (APIENTRY *glBufferDataPROC) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef GLenum (APIENTRY *glCheckFramebufferStatusPROC) (GLenum target);
typedef void (APIENTRY *glColorMaskPROC) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (APIENTRY *glCompileShaderPROC) (GLuint shader);
typedef GLuint (APIENTRY *glCreateProgramPROC) (void);
typedef GLuint (APIENTRY *glCreateShaderPROC) (GLenum type);
typedef void (APIENTRY *glCullFacePROC) (GLenum mode);
typedef void (APIENTRY *glDeleteBuffersPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY *glDeleteFramebuffersPROC) (GLsizei n, const GLuint *framebuffers);
typedef void (APIENTRY *glDeleteProgramPROC) (GLuint program);
typedef void (APIENTRY *glDeleteRenderbuffersPROC) (GLsizei n, const GLuint *renderbuffers);
typedef void (APIENTRY *glDeleteShaderPROC) (GLuint shader);
typedef void (APIENTRY *glDeleteTexturesPROC) (GLsizei n, const GLuint *textures);
typedef void (APIENTRY *glDeleteVertexArraysPROC) (GLsizei n, const GLuint *arrays);
typedef void (APIENTRY *glDetachShaderPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY *glDisablePROC) (GLenum cap);
typedef void (APIENTRY *glDisableVertexAttribArrayPROC) (GLuint index);
typedef void (APIENTRY *glDrawArraysPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRY *glEnablePROC) (GLenum cap);
typedef void (APIENTRY *glEnableVertexAttribArrayPROC) (GLuint index);
typedef void (APIENTRY *glFinishPROC) (void);
typedef void (APIENTRY *glFlushPROC) (void);
typedef void (APIENTRY *glFramebufferRenderbufferPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (APIENTRY *glFramebufferTexture2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRY *glFrontFacePROC) (GLenum mode);
typedef void (APIENTRY *glGenBuffersPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY *glGenFramebuffersPROC) (GLsizei n, GLuint *framebuffers);
typedef void (APIENTRY *glGenRenderbuffersPROC) (GLsizei n, GLuint *renderbuffers);
typedef void (APIENTRY *glGenTexturesPROC) (GLsizei n, GLuint *textures);
typedef void (APIENTRY *glGenVertexArraysPROC) (GLsizei n, GLuint *arrays);
typedef void (APIENTRY *glGenerateMipmapPROC) (GLenum target);
typedef GLenum (APIENTRY *glGetErrorPROC) (void);
typedef void (APIENTRY *glGetIntegervPROC) (GLenum pname, GLint *data);
typedef void (APIENTRY *glGetProgramivPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRY *glGetProgramInfoLogPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY *glGetShaderivPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRY *glGetShaderInfoLogPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef GLuint (APIENTRY *glGetUniformBlockIndexPROC) (GLuint program, const GLchar *uniformBlockName);
typedef GLint (APIENTRY *glGetUniformLocationPROC) (GLuint program, const GLchar *name);
typedef void (APIENTRY *glLinkProgramPROC) (GLuint program);
typedef void (APIENTRY *glPixelStoreiPROC) (GLenum pname, GLint param);
typedef void (APIENTRY *glRenderbufferStoragePROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRY *glShaderSourcePROC) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (APIENTRY *glStencilFuncPROC) (GLenum func, GLint ref, GLuint mask);
typedef void (APIENTRY *glStencilMaskPROC) (GLuint mask);
typedef void (APIENTRY *glStencilOpPROC) (GLenum fail, GLenum zfail, GLenum zpass);
typedef void (APIENTRY *glStencilOpSeparatePROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (APIENTRY *glTexImage2DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY *glTexParameteriPROC) (GLenum target, GLenum pname, GLint param);
typedef void (APIENTRY *glTexSubImage2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY *glUniform1iPROC) (GLint location, GLint v0);
typedef void (APIENTRY *glUniform2fvPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY *glUniform4fvPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY *glUniformBlockBindingPROC) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
typedef void (APIENTRY *glUseProgramPROC) (GLuint program);
typedef void (APIENTRY *glVertexAttribPointerPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

// ---------- LWJGL --------------

enum GLNVGuniformLoc {
	GLNVG_LOC_VIEWSIZE,
	GLNVG_LOC_TEX,
	GLNVG_LOC_FRAG,
	GLNVG_MAX_LOCS
};

enum GLNVGshaderType {
	NSVG_SHADER_FILLGRAD,
	NSVG_SHADER_FILLIMG,
	NSVG_SHADER_SIMPLE,
	NSVG_SHADER_IMG
};

#if NANOVG_GL_USE_UNIFORMBUFFER
enum GLNVGuniformBindings {
	GLNVG_FRAG_BINDING = 0,
};
#endif

struct GLNVGshader {
	GLuint prog;
	GLuint frag;
	GLuint vert;
	GLint loc[GLNVG_MAX_LOCS];
};
typedef struct GLNVGshader GLNVGshader;

struct GLNVGtexture {
	int id;
	GLuint tex;
	int width, height;
	int type;
	int flags;
};
typedef struct GLNVGtexture GLNVGtexture;

enum GLNVGcallType {
	GLNVG_NONE = 0,
	GLNVG_FILL,
	GLNVG_CONVEXFILL,
	GLNVG_STROKE,
	GLNVG_TRIANGLES,
};

struct GLNVGcall {
	int type;
	int image;
	int pathOffset;
	int pathCount;
	int triangleOffset;
	int triangleCount;
	int uniformOffset;
};
typedef struct GLNVGcall GLNVGcall;

struct GLNVGpath {
	int fillOffset;
	int fillCount;
	int strokeOffset;
	int strokeCount;
};
typedef struct GLNVGpath GLNVGpath;

struct GLNVGfragUniforms {
	#if NANOVG_GL_USE_UNIFORMBUFFER
		float scissorMat[12]; // matrices are actually 3 vec4s
		float paintMat[12];
		struct NVGcolor innerCol;
		struct NVGcolor outerCol;
		float scissorExt[2];
		float scissorScale[2];
		float extent[2];
		float radius;
		float feather;
		float strokeMult;
		float strokeThr;
		int texType;
		int type;
	#else
		// note: after modifying layout or size of uniform array,
		// don't forget to also update the fragment shader source!
		#define NANOVG_GL_UNIFORMARRAY_SIZE 11
		union {
			struct {
				float scissorMat[12]; // matrices are actually 3 vec4s
				float paintMat[12];
				struct NVGcolor innerCol;
				struct NVGcolor outerCol;
				float scissorExt[2];
				float scissorScale[2];
				float extent[2];
				float radius;
				float feather;
				float strokeMult;
				float strokeThr;
				float texType;
				float type;
			};
			float uniformArray[NANOVG_GL_UNIFORMARRAY_SIZE][4];
		};
	#endif
};
typedef struct GLNVGfragUniforms GLNVGfragUniforms;

struct GLNVGcontext {
	GLNVGshader shader;
	GLNVGtexture* textures;
	float view[2];
	int ntextures;
	int ctextures;
	int textureId;
	GLuint vertBuf;
#if defined NANOVG_GL3
	GLuint vertArr;
#endif
#if NANOVG_GL_USE_UNIFORMBUFFER
	GLuint fragBuf;
#endif
	int fragSize;
	int flags;

	// Per frame buffers
	GLNVGcall* calls;
	int ccalls;
	int ncalls;
	GLNVGpath* paths;
	int cpaths;
	int npaths;
	struct NVGvertex* verts;
	int cverts;
	int nverts;
	unsigned char* uniforms;
	int cuniforms;
	int nuniforms;

	// cached state
	#if NANOVG_GL_USE_STATE_FILTER
	GLuint boundTexture;
	GLuint stencilMask;
	GLenum stencilFunc;
	GLint stencilFuncRef;
	GLuint stencilFuncMask;
	#endif

	// Function pointers
	glActiveTexturePROC ActiveTexture;
	glAttachShaderPROC AttachShader;
	glBindAttribLocationPROC BindAttribLocation;
	glBindBufferPROC BindBuffer;
	glBindBufferRangePROC BindBufferRange;
	glBindFramebufferPROC BindFramebuffer;
	glBindRenderbufferPROC BindRenderbuffer;
	glBindTexturePROC BindTexture;
	glBindVertexArrayPROC BindVertexArray;
	glBlendFuncPROC BlendFunc;
	glBlendFuncSeparatePROC BlendFuncSeparate;
	glBufferDataPROC BufferData;
	glCheckFramebufferStatusPROC CheckFramebufferStatus;
	glColorMaskPROC ColorMask;
	glCompileShaderPROC CompileShader;
	glCreateProgramPROC CreateProgram;
	glCreateShaderPROC CreateShader;
	glCullFacePROC CullFace;
	glDeleteBuffersPROC DeleteBuffers;
	glDeleteFramebuffersPROC DeleteFramebuffers;
	glDeleteProgramPROC DeleteProgram;
	glDeleteRenderbuffersPROC DeleteRenderbuffers;
	glDeleteShaderPROC DeleteShader;
	glDeleteTexturesPROC DeleteTextures;
	glDeleteVertexArraysPROC DeleteVertexArrays;
	glDetachShaderPROC DetachShader;
	glDisablePROC Disable;
	glDisableVertexAttribArrayPROC DisableVertexAttribArray;
	glDrawArraysPROC DrawArrays;
	glEnablePROC Enable;
	glEnableVertexAttribArrayPROC EnableVertexAttribArray;
	glFinishPROC Finish;
	glFlushPROC Flush;
	glFramebufferRenderbufferPROC FramebufferRenderbuffer;
	glFramebufferTexture2DPROC FramebufferTexture2D;
	glFrontFacePROC FrontFace;
	glGenBuffersPROC GenBuffers;
	glGenFramebuffersPROC GenFramebuffers;
	glGenRenderbuffersPROC GenRenderbuffers;
	glGenTexturesPROC GenTextures;
	glGenVertexArraysPROC GenVertexArrays;
	glGenerateMipmapPROC GenerateMipmap;
	glGetErrorPROC GetError;
	glGetIntegervPROC GetIntegerv;
	glGetProgramivPROC GetProgramiv;
	glGetProgramInfoLogPROC GetProgramInfoLog;
	glGetShaderivPROC GetShaderiv;
	glGetShaderInfoLogPROC GetShaderInfoLog;
	glGetUniformBlockIndexPROC GetUniformBlockIndex;
	glGetUniformLocationPROC GetUniformLocation;
	glLinkProgramPROC LinkProgram;
	glPixelStoreiPROC PixelStorei;
	glRenderbufferStoragePROC RenderbufferStorage;
	glShaderSourcePROC ShaderSource;
	glStencilFuncPROC StencilFunc;
	glStencilMaskPROC StencilMask;
	glStencilOpPROC StencilOp;
	glStencilOpSeparatePROC StencilOpSeparate;
	glTexImage2DPROC TexImage2D;
	glTexParameteriPROC TexParameteri;
	glTexSubImage2DPROC TexSubImage2D;
	glUniform1iPROC Uniform1i;
	glUniform2fvPROC Uniform2fv;
	glUniform4fvPROC Uniform4fv;
	glUniformBlockBindingPROC UniformBlockBinding;
	glUseProgramPROC UseProgram;
	glVertexAttribPointerPROC VertexAttribPointer;
};
typedef struct GLNVGcontext GLNVGcontext;

static int glnvg__maxi(int a, int b) { return a > b ? a : b; }

#ifdef NANOVG_GLES2
static unsigned int glnvg__nearestPow2(unsigned int num)
{
	unsigned n = num > 0 ? num - 1 : 0;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n++;
	return n;
}
#endif

static void glnvg__bindTexture(GLNVGcontext* gl, GLuint tex)
{
#if NANOVG_GL_USE_STATE_FILTER
	if (gl->boundTexture != tex) {
		gl->boundTexture = tex;
		gl->BindTexture(GL_TEXTURE_2D, tex);
	}
#else
	gl->BindTexture(GL_TEXTURE_2D, tex);
#endif
}

static void glnvg__stencilMask(GLNVGcontext* gl, GLuint mask)
{
#if NANOVG_GL_USE_STATE_FILTER
	if (gl->stencilMask != mask) {
		gl->stencilMask = mask;
		gl->StencilMask(mask);
	}
#else
	gl->StencilMask(mask);
#endif
}

static void glnvg__stencilFunc(GLNVGcontext* gl, GLenum func, GLint ref, GLuint mask)
{
#if NANOVG_GL_USE_STATE_FILTER
	if ((gl->stencilFunc != func) ||
		(gl->stencilFuncRef != ref) ||
		(gl->stencilFuncMask != mask)) {
		
		gl->stencilFunc = func;
		gl->stencilFuncRef = ref;
		gl->stencilFuncMask = mask;
		gl->StencilFunc(func, ref, mask);
	}
#else
	gl->StencilFunc(func, ref, mask);
#endif
}

static GLNVGtexture* glnvg__allocTexture(GLNVGcontext* gl)
{
	GLNVGtexture* tex = NULL;
	int i;

	for (i = 0; i < gl->ntextures; i++) {
		if (gl->textures[i].id == 0) {
			tex = &gl->textures[i];
			break;
		}
	}
	if (tex == NULL) {
		if (gl->ntextures+1 > gl->ctextures) {
			GLNVGtexture* textures;
			int ctextures = glnvg__maxi(gl->ntextures+1, 4) +  gl->ctextures/2; // 1.5x Overallocate
			textures = (GLNVGtexture*)realloc(gl->textures, sizeof(GLNVGtexture)*ctextures);
			if (textures == NULL) return NULL;
			gl->textures = textures;
			gl->ctextures = ctextures;
		}
		tex = &gl->textures[gl->ntextures++];
	}
	
	memset(tex, 0, sizeof(*tex));
	tex->id = ++gl->textureId;
	
	return tex;
}

static GLNVGtexture* glnvg__findTexture(GLNVGcontext* gl, int id)
{
	int i;
	for (i = 0; i < gl->ntextures; i++)
		if (gl->textures[i].id == id)
			return &gl->textures[i];
	return NULL;
}

static int glnvg__deleteTexture(GLNVGcontext* gl, int id)
{
	int i;
	for (i = 0; i < gl->ntextures; i++) {
		if (gl->textures[i].id == id) {
			if (gl->textures[i].tex != 0 && (gl->textures[i].flags & NVG_IMAGE_NODELETE) == 0)
				gl->DeleteTextures(1, &gl->textures[i].tex);
			memset(&gl->textures[i], 0, sizeof(gl->textures[i]));
			return 1;
		}
	}
	return 0;
}

static void glnvg__dumpShaderError(GLNVGcontext* gl, GLuint shader, const char* name, const char* type)
{
	GLchar str[512+1];
	GLsizei len = 0;
	gl->GetShaderInfoLog(shader, 512, &len, str);
	if (len > 512) len = 512;
	str[len] = '\0';
	printf("Shader %s/%s error:\n%s\n", name, type, str);
}

static void glnvg__dumpProgramError(GLNVGcontext* gl, GLuint prog, const char* name)
{
	GLchar str[512+1];
	GLsizei len = 0;
	gl->GetProgramInfoLog(prog, 512, &len, str);
	if (len > 512) len = 512;
	str[len] = '\0';
	printf("Program %s error:\n%s\n", name, str);
}

static void glnvg__checkError(GLNVGcontext* gl, const char* str)
{
	GLenum err;
	if ((gl->flags & NVG_DEBUG) == 0) return;
	err = gl->GetError();
	if (err != GL_NO_ERROR) {
		printf("Error %08x after %s\n", err, str);
		return;
	}
}

static int glnvg__createShader(GLNVGcontext* gl, GLNVGshader* shader, const char* name, const char* header, const char* opts, const char* vshader, const char* fshader)
{
	GLint status;
	GLuint prog, vert, frag;
	const char* str[3];
	str[0] = header;
	str[1] = opts != NULL ? opts : "";

	memset(shader, 0, sizeof(*shader));

	prog = gl->CreateProgram();
	vert = gl->CreateShader(GL_VERTEX_SHADER);
	frag = gl->CreateShader(GL_FRAGMENT_SHADER);
	str[2] = vshader;
	gl->ShaderSource(vert, 3, str, 0);
	str[2] = fshader;
	gl->ShaderSource(frag, 3, str, 0);

	gl->CompileShader(vert);
	gl->GetShaderiv(vert, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glnvg__dumpShaderError(gl, vert, name, "vert");
		return 0;
	}

	gl->CompileShader(frag);
	gl->GetShaderiv(frag, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glnvg__dumpShaderError(gl, frag, name, "frag");
		return 0;
	}

	gl->AttachShader(prog, vert);
	gl->AttachShader(prog, frag);

	gl->BindAttribLocation(prog, 0, "vertex");
	gl->BindAttribLocation(prog, 1, "tcoord");

	gl->LinkProgram(prog);
	gl->GetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		glnvg__dumpProgramError(gl, prog, name);
		return 0;
	}

	shader->prog = prog;
	shader->vert = vert;
	shader->frag = frag;

	return 1;
}

static void glnvg__deleteShader(GLNVGcontext* gl, GLNVGshader* shader)
{
	if (shader->prog != 0)
		gl->DeleteProgram(shader->prog);
	if (shader->vert != 0)
		gl->DeleteShader(shader->vert);
	if (shader->frag != 0)
		gl->DeleteShader(shader->frag);
}

static void glnvg__getUniforms(GLNVGcontext* gl, GLNVGshader* shader)
{
	shader->loc[GLNVG_LOC_VIEWSIZE] = gl->GetUniformLocation(shader->prog, "viewSize");
	shader->loc[GLNVG_LOC_TEX] = gl->GetUniformLocation(shader->prog, "tex");

#if NANOVG_GL_USE_UNIFORMBUFFER
	shader->loc[GLNVG_LOC_FRAG] = gl->GetUniformBlockIndex(shader->prog, "frag");
#else
	shader->loc[GLNVG_LOC_FRAG] = gl->GetUniformLocation(shader->prog, "frag");
#endif
}

static int glnvg__renderCreate(void* uptr)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	int align = 4;

	// TODO: mediump float may not be enough for GLES2 in iOS.
	// see the following discussion: https://github.com/memononen/nanovg/issues/46
	static const char* shaderHeader =
#if defined NANOVG_GL2
		"#define NANOVG_GL2 1\n"
#elif defined NANOVG_GL3
		"#version 150 core\n"
		"#define NANOVG_GL3 1\n"
#elif defined NANOVG_GLES2
		"#version 100\n"
		"#define NANOVG_GL2 1\n"
#elif defined NANOVG_GLES3
		"#version 300 es\n"
		"#define NANOVG_GL3 1\n"
#endif

#if NANOVG_GL_USE_UNIFORMBUFFER
	"#define USE_UNIFORMBUFFER 1\n"
#else
	"#define UNIFORMARRAY_SIZE 11\n"
#endif
	"\n";

	static const char* fillVertShader =
		"#ifdef NANOVG_GL3\n"
		"	uniform vec2 viewSize;\n"
		"	in vec2 vertex;\n"
		"	in vec2 tcoord;\n"
		"	out vec2 ftcoord;\n"
		"	out vec2 fpos;\n"
		"#else\n"
		"	uniform vec2 viewSize;\n"
		"	attribute vec2 vertex;\n"
		"	attribute vec2 tcoord;\n"
		"	varying vec2 ftcoord;\n"
		"	varying vec2 fpos;\n"
		"#endif\n"
		"void main(void) {\n"
		"	ftcoord = tcoord;\n"
		"	fpos = vertex;\n"
		"	gl_Position = vec4(2.0*vertex.x/viewSize.x - 1.0, 1.0 - 2.0*vertex.y/viewSize.y, 0, 1);\n"
		"}\n";

	static const char* fillFragShader = 
		"#ifdef GL_ES\n"
		"#if defined(GL_FRAGMENT_PRECISION_HIGH) || defined(NANOVG_GL3)\n"
		" precision highp float;\n"
		"#else\n"
		" precision mediump float;\n"
		"#endif\n"
		"#endif\n"
		"#ifdef NANOVG_GL3\n"
		"#ifdef USE_UNIFORMBUFFER\n"
		"	layout(std140) uniform frag {\n"
		"		mat3 scissorMat;\n"
		"		mat3 paintMat;\n"
		"		vec4 innerCol;\n"
		"		vec4 outerCol;\n"
		"		vec2 scissorExt;\n"
		"		vec2 scissorScale;\n"
		"		vec2 extent;\n"
		"		float radius;\n"
		"		float feather;\n"
		"		float strokeMult;\n"
		"		float strokeThr;\n"
		"		int texType;\n"
		"		int type;\n"
		"	};\n"
		"#else\n" // NANOVG_GL3 && !USE_UNIFORMBUFFER
		"	uniform vec4 frag[UNIFORMARRAY_SIZE];\n"
		"#endif\n"
		"	uniform sampler2D tex;\n"
		"	in vec2 ftcoord;\n"
		"	in vec2 fpos;\n"
		"	out vec4 outColor;\n"
		"#else\n" // !NANOVG_GL3
		"	uniform vec4 frag[UNIFORMARRAY_SIZE];\n"
		"	uniform sampler2D tex;\n"
		"	varying vec2 ftcoord;\n"
		"	varying vec2 fpos;\n"
		"#endif\n"
		"#ifndef USE_UNIFORMBUFFER\n"
		"	#define scissorMat mat3(frag[0].xyz, frag[1].xyz, frag[2].xyz)\n"
		"	#define paintMat mat3(frag[3].xyz, frag[4].xyz, frag[5].xyz)\n"
		"	#define innerCol frag[6]\n"
		"	#define outerCol frag[7]\n"
		"	#define scissorExt frag[8].xy\n"
		"	#define scissorScale frag[8].zw\n"
		"	#define extent frag[9].xy\n"
		"	#define radius frag[9].z\n"
		"	#define feather frag[9].w\n"
		"	#define strokeMult frag[10].x\n"
		"	#define strokeThr frag[10].y\n"
		"	#define texType int(frag[10].z)\n"
		"	#define type int(frag[10].w)\n"
		"#endif\n"
		"\n"
		"float sdroundrect(vec2 pt, vec2 ext, float rad) {\n"
		"	vec2 ext2 = ext - vec2(rad,rad);\n"
		"	vec2 d = abs(pt) - ext2;\n"
		"	return min(max(d.x,d.y),0.0) + length(max(d,0.0)) - rad;\n"
		"}\n"
		"\n"
		"// Scissoring\n"
		"float scissorMask(vec2 p) {\n"
		"	vec2 sc = (abs((scissorMat * vec3(p,1.0)).xy) - scissorExt);\n"
		"	sc = vec2(0.5,0.5) - sc * scissorScale;\n"
		"	return clamp(sc.x,0.0,1.0) * clamp(sc.y,0.0,1.0);\n"
		"}\n"
		"#ifdef EDGE_AA\n"
		"// Stroke - from [0..1] to clipped pyramid, where the slope is 1px.\n"
		"float strokeMask() {\n"
		"	return min(1.0, (1.0-abs(ftcoord.x*2.0-1.0))*strokeMult) * min(1.0, ftcoord.y);\n"
		"}\n"
		"#endif\n"
		"\n"
		"void main(void) {\n"
		"   vec4 result;\n"
		"	float scissor = scissorMask(fpos);\n"
		"#ifdef EDGE_AA\n"
		"	float strokeAlpha = strokeMask();\n"
		"#else\n"
		"	float strokeAlpha = 1.0;\n"
		"#endif\n"
		"	if (type == 0) {			// Gradient\n"
		"		// Calculate gradient color using box gradient\n"
		"		vec2 pt = (paintMat * vec3(fpos,1.0)).xy;\n"
		"		float d = clamp((sdroundrect(pt, extent, radius) + feather*0.5) / feather, 0.0, 1.0);\n"
		"		vec4 color = mix(innerCol,outerCol,d);\n"
		"		// Combine alpha\n"
		"		color *= strokeAlpha * scissor;\n"
		"		result = color;\n"
		"	} else if (type == 1) {		// Image\n"
		"		// Calculate color fron texture\n"
		"		vec2 pt = (paintMat * vec3(fpos,1.0)).xy / extent;\n"
		"#ifdef NANOVG_GL3\n"
		"		vec4 color = texture(tex, pt);\n"
		"#else\n"
		"		vec4 color = texture2D(tex, pt);\n"
		"#endif\n"
		"		if (texType == 1) color = vec4(color.xyz*color.w,color.w);"
		"		if (texType == 2) color = vec4(color.x);"
		"		// Apply color tint and alpha.\n"
		"		color *= innerCol;\n"
		"		// Combine alpha\n"
		"		color *= strokeAlpha * scissor;\n"
		"		result = color;\n"
		"	} else if (type == 2) {		// Stencil fill\n"
		"		result = vec4(1,1,1,1);\n"
		"	} else if (type == 3) {		// Textured tris\n"
		"#ifdef NANOVG_GL3\n"
		"		vec4 color = texture(tex, ftcoord);\n"
		"#else\n"
		"		vec4 color = texture2D(tex, ftcoord);\n"
		"#endif\n"
		"		if (texType == 1) color = vec4(color.xyz*color.w,color.w);"
		"		if (texType == 2) color = vec4(color.x);"
		"		color *= scissor;\n"
		"		result = color * innerCol;\n"
		"	}\n"
		"#ifdef EDGE_AA\n"
		"#ifdef STENCIL_STROKES\n"
		"	if (strokeAlpha < strokeThr) discard;\n"
		"#else\n"
		"	if (strokeAlpha < strokeThr) result = vec4(0,0,0,0);\n"
		"#endif\n"
		"#endif\n"
		"#ifdef NANOVG_GL3\n"
		"	outColor = result;\n"
		"#else\n"
		"	gl_FragColor = result;\n"
		"#endif\n"
		"}\n";

	glnvg__checkError(gl, "init");

	char* opts = NULL;
	if (gl->flags & NVG_ANTIALIAS) {
		if (gl->flags & NVG_STENCIL_STROKES)
			opts = "#define EDGE_AA 1\n#define STENCIL_STROKES 1\n";
		else
			opts = "#define EDGE_AA 1\n";
	}
	if (glnvg__createShader(gl, &gl->shader, "shader", shaderHeader, opts, fillVertShader, fillFragShader) == 0)
		return 0;

	glnvg__checkError(gl, "uniform locations");
	glnvg__getUniforms(gl, &gl->shader);

	// Create dynamic vertex array
#if defined NANOVG_GL3
	gl->GenVertexArrays(1, &gl->vertArr);
#endif
	gl->GenBuffers(1, &gl->vertBuf);

#if NANOVG_GL_USE_UNIFORMBUFFER
	// Create UBOs
	gl->UniformBlockBinding(gl->shader.prog, gl->shader.loc[GLNVG_LOC_FRAG], GLNVG_FRAG_BINDING);
	gl->GenBuffers(1, &gl->fragBuf);
	gl->GetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align);
#endif
	gl->fragSize = sizeof(GLNVGfragUniforms) + align - sizeof(GLNVGfragUniforms) % align;

	glnvg__checkError(gl, "create done");

	gl->Finish();

	return 1;
}

static int glnvg__renderCreateTexture(void* uptr, int type, int w, int h, int imageFlags, const unsigned char* data)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGtexture* tex = glnvg__allocTexture(gl);

	if (tex == NULL) return 0;

#ifdef NANOVG_GLES2
	// Check for non-power of 2.
	if (glnvg__nearestPow2(w) != (unsigned int)w || glnvg__nearestPow2(h) != (unsigned int)h) {
		// No repeat
		if ((imageFlags & NVG_IMAGE_REPEATX) != 0 || (imageFlags & NVG_IMAGE_REPEATY) != 0) {
			printf("Repeat X/Y is not supported for non power-of-two textures (%d x %d)\n", w, h);
			imageFlags &= ~(NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY);
		}
		// No mips. 
		if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
			printf("Mip-maps is not support for non power-of-two textures (%d x %d)\n", w, h);
			imageFlags &= ~NVG_IMAGE_GENERATE_MIPMAPS;
		}
	}
#endif

	gl->GenTextures(1, &tex->tex);
	tex->width = w;
	tex->height = h;
	tex->type = type;
	tex->flags = imageFlags;
	glnvg__bindTexture(gl, tex->tex);

	gl->PixelStorei(GL_UNPACK_ALIGNMENT,1);
#ifndef NANOVG_GLES2
	gl->PixelStorei(GL_UNPACK_ROW_LENGTH, tex->width);
	gl->PixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	gl->PixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

#if defined (NANOVG_GL2)
	// GL 1.4 and later has support for generating mipmaps using a tex parameter.
	if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
		gl->TexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	}
#endif

	if (type == NVG_TEXTURE_RGBA)
		gl->TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
#if defined(NANOVG_GLES2)
		gl->TexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
#elif defined(NANOVG_GLES3)
		gl->TexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
#else
		gl->TexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
#endif

	if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
		gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else {
		gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (imageFlags & NVG_IMAGE_REPEATX)
		gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	else
		gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	if (imageFlags & NVG_IMAGE_REPEATY)
		gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	else
		gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	gl->PixelStorei(GL_UNPACK_ALIGNMENT, 4);
#ifndef NANOVG_GLES2
	gl->PixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	gl->PixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	gl->PixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

	// The new way to build mipmaps on GLES and GL3
#if !defined(NANOVG_GL2)
	if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
		gl->GenerateMipmap(GL_TEXTURE_2D);
	}
#endif

	glnvg__checkError(gl, "create tex");
	glnvg__bindTexture(gl, 0);

	return tex->id;
}


static int glnvg__renderDeleteTexture(void* uptr, int image)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	return glnvg__deleteTexture(gl, image);
}

static int glnvg__renderUpdateTexture(void* uptr, int image, int x, int y, int w, int h, const unsigned char* data)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGtexture* tex = glnvg__findTexture(gl, image);

	if (tex == NULL) return 0;
	glnvg__bindTexture(gl, tex->tex);

	gl->PixelStorei(GL_UNPACK_ALIGNMENT,1);

#ifndef NANOVG_GLES2
	gl->PixelStorei(GL_UNPACK_ROW_LENGTH, tex->width);
	gl->PixelStorei(GL_UNPACK_SKIP_PIXELS, x);
	gl->PixelStorei(GL_UNPACK_SKIP_ROWS, y);
#else
	// No support for all of skip, need to update a whole row at a time.
	if (tex->type == NVG_TEXTURE_RGBA)
		data += y*tex->width*4;
	else
		data += y*tex->width;
	x = 0;
	w = tex->width;
#endif

	if (tex->type == NVG_TEXTURE_RGBA)
		gl->TexSubImage2D(GL_TEXTURE_2D, 0, x,y, w,h, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
#ifdef NANOVG_GLES2
		gl->TexSubImage2D(GL_TEXTURE_2D, 0, x,y, w,h, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
#else
		gl->TexSubImage2D(GL_TEXTURE_2D, 0, x,y, w,h, GL_RED, GL_UNSIGNED_BYTE, data);
#endif

	gl->PixelStorei(GL_UNPACK_ALIGNMENT, 4);
#ifndef NANOVG_GLES2
	gl->PixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	gl->PixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	gl->PixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

	glnvg__bindTexture(gl, 0);

	return 1;
}

static int glnvg__renderGetTextureSize(void* uptr, int image, int* w, int* h)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGtexture* tex = glnvg__findTexture(gl, image);
	if (tex == NULL) return 0;
	*w = tex->width;
	*h = tex->height;
	return 1;
}

static void glnvg__xformToMat3x4(float* m3, float* t)
{
	m3[0] = t[0];
	m3[1] = t[1];
	m3[2] = 0.0f;
	m3[3] = 0.0f;
	m3[4] = t[2];
	m3[5] = t[3];
	m3[6] = 0.0f;
	m3[7] = 0.0f;
	m3[8] = t[4];
	m3[9] = t[5];
	m3[10] = 1.0f;
	m3[11] = 0.0f;
}

static NVGcolor glnvg__premulColor(NVGcolor c)
{
	c.r *= c.a;
	c.g *= c.a;
	c.b *= c.a;
	return c;
}

static int glnvg__convertPaint(GLNVGcontext* gl, GLNVGfragUniforms* frag, NVGpaint* paint,
							   NVGscissor* scissor, float width, float fringe, float strokeThr)
{
	GLNVGtexture* tex = NULL;
	float invxform[6];

	memset(frag, 0, sizeof(*frag));

	frag->innerCol = glnvg__premulColor(paint->innerColor);
	frag->outerCol = glnvg__premulColor(paint->outerColor);

	if (scissor->extent[0] < -0.5f || scissor->extent[1] < -0.5f) {
		memset(frag->scissorMat, 0, sizeof(frag->scissorMat));
		frag->scissorExt[0] = 1.0f;
		frag->scissorExt[1] = 1.0f;
		frag->scissorScale[0] = 1.0f;
		frag->scissorScale[1] = 1.0f;
	} else {
		nvgTransformInverse(invxform, scissor->xform);
		glnvg__xformToMat3x4(frag->scissorMat, invxform);
		frag->scissorExt[0] = scissor->extent[0];
		frag->scissorExt[1] = scissor->extent[1];
		frag->scissorScale[0] = sqrtf(scissor->xform[0]*scissor->xform[0] + scissor->xform[2]*scissor->xform[2]) / fringe;
		frag->scissorScale[1] = sqrtf(scissor->xform[1]*scissor->xform[1] + scissor->xform[3]*scissor->xform[3]) / fringe;
	}

	memcpy(frag->extent, paint->extent, sizeof(frag->extent));
	frag->strokeMult = (width*0.5f + fringe*0.5f) / fringe;
	frag->strokeThr = strokeThr;

	if (paint->image != 0) {
		tex = glnvg__findTexture(gl, paint->image);
		if (tex == NULL) return 0;
		if ((tex->flags & NVG_IMAGE_FLIPY) != 0) {
			float flipped[6];
			nvgTransformScale(flipped, 1.0f, -1.0f);
			nvgTransformMultiply(flipped, paint->xform);
			nvgTransformInverse(invxform, flipped);
		} else {
			nvgTransformInverse(invxform, paint->xform);
		}
		frag->type = NSVG_SHADER_FILLIMG;

		if (tex->type == NVG_TEXTURE_RGBA)
			frag->texType = (tex->flags & NVG_IMAGE_PREMULTIPLIED) ? 0 : 1;
		else
			frag->texType = 2;
//		printf("frag->texType = %d\n", frag->texType);
	} else {
		frag->type = NSVG_SHADER_FILLGRAD;
		frag->radius = paint->radius;
		frag->feather = paint->feather;
		nvgTransformInverse(invxform, paint->xform);
	}

	glnvg__xformToMat3x4(frag->paintMat, invxform);

	return 1;
}

static GLNVGfragUniforms* nvg__fragUniformPtr(GLNVGcontext* gl, int i);

static void glnvg__setUniforms(GLNVGcontext* gl, int uniformOffset, int image)
{
#if NANOVG_GL_USE_UNIFORMBUFFER
	gl->BindBufferRange(GL_UNIFORM_BUFFER, GLNVG_FRAG_BINDING, gl->fragBuf, uniformOffset, sizeof(GLNVGfragUniforms));
#else
	GLNVGfragUniforms* frag = nvg__fragUniformPtr(gl, uniformOffset);
	gl->Uniform4fv(gl->shader.loc[GLNVG_LOC_FRAG], NANOVG_GL_UNIFORMARRAY_SIZE, &(frag->uniformArray[0][0]));
#endif

	if (image != 0) {
		GLNVGtexture* tex = glnvg__findTexture(gl, image);
		glnvg__bindTexture(gl, tex != NULL ? tex->tex : 0);
		glnvg__checkError(gl, "tex paint tex");
	} else {
		glnvg__bindTexture(gl, 0);
	}
}

static void glnvg__renderViewport(void* uptr, int width, int height, float devicePixelRatio)
{
	UNUSED_PARAM(devicePixelRatio)
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	gl->view[0] = (float)width;
	gl->view[1] = (float)height;
}

static void glnvg__fill(GLNVGcontext* gl, GLNVGcall* call)
{
	GLNVGpath* paths = &gl->paths[call->pathOffset];
	int i, npaths = call->pathCount;

	// Draw shapes
	gl->Enable(GL_STENCIL_TEST);
	glnvg__stencilMask(gl, 0xff);
	glnvg__stencilFunc(gl, GL_ALWAYS, 0, 0xff);
	gl->ColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// set bindpoint for solid loc
	glnvg__setUniforms(gl, call->uniformOffset, 0);
	glnvg__checkError(gl, "fill simple");

	gl->StencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
	gl->StencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);
	gl->Disable(GL_CULL_FACE);
	for (i = 0; i < npaths; i++)
		gl->DrawArrays(GL_TRIANGLE_FAN, paths[i].fillOffset, paths[i].fillCount);
	gl->Enable(GL_CULL_FACE);

	// Draw anti-aliased pixels
	gl->ColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glnvg__setUniforms(gl, call->uniformOffset + gl->fragSize, call->image);
	glnvg__checkError(gl, "fill fill");

	if (gl->flags & NVG_ANTIALIAS) {
		glnvg__stencilFunc(gl, GL_EQUAL, 0x00, 0xff);
		gl->StencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		// Draw fringes
		for (i = 0; i < npaths; i++)
			gl->DrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
	}

	// Draw fill
	glnvg__stencilFunc(gl, GL_NOTEQUAL, 0x0, 0xff);
	gl->StencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
	gl->DrawArrays(GL_TRIANGLES, call->triangleOffset, call->triangleCount);

	gl->Disable(GL_STENCIL_TEST);
}

static void glnvg__convexFill(GLNVGcontext* gl, GLNVGcall* call)
{
	GLNVGpath* paths = &gl->paths[call->pathOffset];
	int i, npaths = call->pathCount;

	glnvg__setUniforms(gl, call->uniformOffset, call->image);
	glnvg__checkError(gl, "convex fill");

	for (i = 0; i < npaths; i++)
		gl->DrawArrays(GL_TRIANGLE_FAN, paths[i].fillOffset, paths[i].fillCount);
	if (gl->flags & NVG_ANTIALIAS) {
		// Draw fringes
		for (i = 0; i < npaths; i++)
			gl->DrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
	}
}

static void glnvg__stroke(GLNVGcontext* gl, GLNVGcall* call)
{
	GLNVGpath* paths = &gl->paths[call->pathOffset];
	int npaths = call->pathCount, i;

	if (gl->flags & NVG_STENCIL_STROKES) {

		gl->Enable(GL_STENCIL_TEST);
		glnvg__stencilMask(gl, 0xff);

		// Fill the stroke base without overlap
		glnvg__stencilFunc(gl, GL_EQUAL, 0x0, 0xff);
		gl->StencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glnvg__setUniforms(gl, call->uniformOffset + gl->fragSize, call->image);
		glnvg__checkError(gl, "stroke fill 0");
		for (i = 0; i < npaths; i++)
			gl->DrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);

		// Draw anti-aliased pixels.
		glnvg__setUniforms(gl, call->uniformOffset, call->image);
		glnvg__stencilFunc(gl, GL_EQUAL, 0x00, 0xff);
		gl->StencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		for (i = 0; i < npaths; i++)
			gl->DrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);

		// Clear stencil buffer.		
		gl->ColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glnvg__stencilFunc(gl, GL_ALWAYS, 0x0, 0xff);
		gl->StencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
		glnvg__checkError(gl, "stroke fill 1");
		for (i = 0; i < npaths; i++)
			gl->DrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
		gl->ColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		gl->Disable(GL_STENCIL_TEST);

//		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset + gl->fragSize), paint, scissor, strokeWidth, fringe, 1.0f - 0.5f/255.0f);

	} else {
		glnvg__setUniforms(gl, call->uniformOffset, call->image);
		glnvg__checkError(gl, "stroke fill");
		// Draw Strokes
		for (i = 0; i < npaths; i++)
			gl->DrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
	}
}

static void glnvg__triangles(GLNVGcontext* gl, GLNVGcall* call)
{
	glnvg__setUniforms(gl, call->uniformOffset, call->image);
	glnvg__checkError(gl, "triangles fill");

	gl->DrawArrays(GL_TRIANGLES, call->triangleOffset, call->triangleCount);
}

static void glnvg__renderCancel(void* uptr) {
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	gl->nverts = 0;
	gl->npaths = 0;
	gl->ncalls = 0;
	gl->nuniforms = 0;
}

static GLenum glnvg_convertBlendFuncFactor(int factor)
{
	if (factor == NVG_ZERO)
		return GL_ZERO;
	if (factor == NVG_ONE)
		return GL_ONE;
	if (factor == NVG_SRC_COLOR)
		return GL_SRC_COLOR;
	if (factor == NVG_ONE_MINUS_SRC_COLOR)
		return GL_ONE_MINUS_SRC_COLOR;
	if (factor == NVG_DST_COLOR)
		return GL_DST_COLOR;
	if (factor == NVG_ONE_MINUS_DST_COLOR)
		return GL_ONE_MINUS_DST_COLOR;
	if (factor == NVG_SRC_ALPHA)
		return GL_SRC_ALPHA;
	if (factor == NVG_ONE_MINUS_SRC_ALPHA)
		return GL_ONE_MINUS_SRC_ALPHA;
	if (factor == NVG_DST_ALPHA)
		return GL_DST_ALPHA;
	if (factor == NVG_ONE_MINUS_DST_ALPHA)
		return GL_ONE_MINUS_DST_ALPHA;
	if (factor == NVG_SRC_ALPHA_SATURATE)
		return GL_SRC_ALPHA_SATURATE;
	return GL_INVALID_ENUM;
}

static void glnvg__blendCompositeOperation(GLNVGcontext* gl, NVGcompositeOperationState op)
{
	GLenum srcRGB = glnvg_convertBlendFuncFactor(op.srcRGB);
	GLenum dstRGB = glnvg_convertBlendFuncFactor(op.dstRGB);
	GLenum srcAlpha = glnvg_convertBlendFuncFactor(op.srcAlpha);
	GLenum dstAlpha = glnvg_convertBlendFuncFactor(op.dstAlpha);
	if (srcRGB == GL_INVALID_ENUM || dstRGB == GL_INVALID_ENUM || srcAlpha == GL_INVALID_ENUM || dstAlpha == GL_INVALID_ENUM)
		gl->BlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	else
		gl->BlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

static void glnvg__renderFlush(void* uptr, NVGcompositeOperationState compositeOperation)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	int i;

	if (gl->ncalls > 0) {

		// Setup require GL state.
		gl->UseProgram(gl->shader.prog);

		glnvg__blendCompositeOperation(gl, compositeOperation);
		gl->Enable(GL_CULL_FACE);
		gl->CullFace(GL_BACK);
		gl->FrontFace(GL_CCW);
		gl->Enable(GL_BLEND);
		gl->Disable(GL_DEPTH_TEST);
		gl->Disable(GL_SCISSOR_TEST);
		gl->ColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		gl->StencilMask(0xffffffff);
		gl->StencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		gl->StencilFunc(GL_ALWAYS, 0, 0xffffffff);
		gl->ActiveTexture(GL_TEXTURE0);
		gl->BindTexture(GL_TEXTURE_2D, 0);
		#if NANOVG_GL_USE_STATE_FILTER
		gl->boundTexture = 0;
		gl->stencilMask = 0xffffffff;
		gl->stencilFunc = GL_ALWAYS;
		gl->stencilFuncRef = 0;
		gl->stencilFuncMask = 0xffffffff;
		#endif

#if NANOVG_GL_USE_UNIFORMBUFFER
		// Upload ubo for frag shaders
		gl->BindBuffer(GL_UNIFORM_BUFFER, gl->fragBuf);
		gl->BufferData(GL_UNIFORM_BUFFER, gl->nuniforms * gl->fragSize, gl->uniforms, GL_STREAM_DRAW);
#endif

		// Upload vertex data
#if defined NANOVG_GL3
		gl->BindVertexArray(gl->vertArr);
#endif
		gl->BindBuffer(GL_ARRAY_BUFFER, gl->vertBuf);
		gl->BufferData(GL_ARRAY_BUFFER, gl->nverts * sizeof(NVGvertex), gl->verts, GL_STREAM_DRAW);
		gl->EnableVertexAttribArray(0);
		gl->EnableVertexAttribArray(1);
		gl->VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(NVGvertex), (const GLvoid*)(size_t)0);
		gl->VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(NVGvertex), (const GLvoid*)(0 + 2*sizeof(float)));

		// Set view and texture just once per frame.
		gl->Uniform1i(gl->shader.loc[GLNVG_LOC_TEX], 0);
		gl->Uniform2fv(gl->shader.loc[GLNVG_LOC_VIEWSIZE], 1, gl->view);

#if NANOVG_GL_USE_UNIFORMBUFFER
		gl->BindBuffer(GL_UNIFORM_BUFFER, gl->fragBuf);
#endif

		for (i = 0; i < gl->ncalls; i++) {
			GLNVGcall* call = &gl->calls[i];
			if (call->type == GLNVG_FILL)
				glnvg__fill(gl, call);
			else if (call->type == GLNVG_CONVEXFILL)
				glnvg__convexFill(gl, call);
			else if (call->type == GLNVG_STROKE)
				glnvg__stroke(gl, call);
			else if (call->type == GLNVG_TRIANGLES)
				glnvg__triangles(gl, call);
		}

		gl->DisableVertexAttribArray(0);
		gl->DisableVertexAttribArray(1);
#if defined NANOVG_GL3
		gl->BindVertexArray(0);
#endif	
		gl->Disable(GL_CULL_FACE);
			gl->BindBuffer(GL_ARRAY_BUFFER, 0);
		gl->UseProgram(0);
		glnvg__bindTexture(gl, 0);
	}

	// Reset calls
	gl->nverts = 0;
	gl->npaths = 0;
	gl->ncalls = 0;
	gl->nuniforms = 0;
}

static int glnvg__maxVertCount(const NVGpath* paths, int npaths)
{
	int i, count = 0;
	for (i = 0; i < npaths; i++) {
		count += paths[i].nfill;
		count += paths[i].nstroke;
	}
	return count;
}

static GLNVGcall* glnvg__allocCall(GLNVGcontext* gl)
{
	GLNVGcall* ret = NULL;
	if (gl->ncalls+1 > gl->ccalls) {
		GLNVGcall* calls;
		int ccalls = glnvg__maxi(gl->ncalls+1, 128) + gl->ccalls/2; // 1.5x Overallocate
		calls = (GLNVGcall*)realloc(gl->calls, sizeof(GLNVGcall) * ccalls);
		if (calls == NULL) return NULL;
		gl->calls = calls;
		gl->ccalls = ccalls;
	}
	ret = &gl->calls[gl->ncalls++];
	memset(ret, 0, sizeof(GLNVGcall));
	return ret;
}

static int glnvg__allocPaths(GLNVGcontext* gl, int n)
{
	int ret = 0;
	if (gl->npaths+n > gl->cpaths) {
		GLNVGpath* paths;
		int cpaths = glnvg__maxi(gl->npaths + n, 128) + gl->cpaths/2; // 1.5x Overallocate
		paths = (GLNVGpath*)realloc(gl->paths, sizeof(GLNVGpath) * cpaths);
		if (paths == NULL) return -1;
		gl->paths = paths;
		gl->cpaths = cpaths;
	}
	ret = gl->npaths;
	gl->npaths += n;
	return ret;
}

static int glnvg__allocVerts(GLNVGcontext* gl, int n)
{
	int ret = 0;
	if (gl->nverts+n > gl->cverts) {
		NVGvertex* verts;
		int cverts = glnvg__maxi(gl->nverts + n, 4096) + gl->cverts/2; // 1.5x Overallocate
		verts = (NVGvertex*)realloc(gl->verts, sizeof(NVGvertex) * cverts);
		if (verts == NULL) return -1;
		gl->verts = verts;
		gl->cverts = cverts;
	}
	ret = gl->nverts;
	gl->nverts += n;
	return ret;
}

static int glnvg__allocFragUniforms(GLNVGcontext* gl, int n)
{
	int ret = 0, structSize = gl->fragSize;
	if (gl->nuniforms+n > gl->cuniforms) {
		unsigned char* uniforms;
		int cuniforms = glnvg__maxi(gl->nuniforms+n, 128) + gl->cuniforms/2; // 1.5x Overallocate
		uniforms = (unsigned char*)realloc(gl->uniforms, structSize * cuniforms);
		if (uniforms == NULL) return -1;
		gl->uniforms = uniforms;
		gl->cuniforms = cuniforms;
	}
	ret = gl->nuniforms * structSize;
	gl->nuniforms += n;
	return ret;
}

static GLNVGfragUniforms* nvg__fragUniformPtr(GLNVGcontext* gl, int i)
{
	return (GLNVGfragUniforms*)&gl->uniforms[i];
}

static void glnvg__vset(NVGvertex* vtx, float x, float y, float u, float v)
{
	vtx->x = x;
	vtx->y = y;
	vtx->u = u;
	vtx->v = v;
}

static void glnvg__renderFill(void* uptr, NVGpaint* paint, NVGscissor* scissor, float fringe,
							  const float* bounds, const NVGpath* paths, int npaths)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGcall* call = glnvg__allocCall(gl);
	NVGvertex* quad;
	GLNVGfragUniforms* frag;
	int i, maxverts, offset;

	if (call == NULL) return;

	call->type = GLNVG_FILL;
	call->pathOffset = glnvg__allocPaths(gl, npaths);
	if (call->pathOffset == -1) goto error;
	call->pathCount = npaths;
	call->image = paint->image;

	if (npaths == 1 && paths[0].convex)
		call->type = GLNVG_CONVEXFILL;

	// Allocate vertices for all the paths.
	maxverts = glnvg__maxVertCount(paths, npaths) + 6;
	offset = glnvg__allocVerts(gl, maxverts);
	if (offset == -1) goto error;

	for (i = 0; i < npaths; i++) {
		GLNVGpath* copy = &gl->paths[call->pathOffset + i];
		const NVGpath* path = &paths[i];
		memset(copy, 0, sizeof(GLNVGpath));
		if (path->nfill > 0) {
			copy->fillOffset = offset;
			copy->fillCount = path->nfill;
			memcpy(&gl->verts[offset], path->fill, sizeof(NVGvertex) * path->nfill);
			offset += path->nfill;
		}
		if (path->nstroke > 0) {
			copy->strokeOffset = offset;
			copy->strokeCount = path->nstroke;
			memcpy(&gl->verts[offset], path->stroke, sizeof(NVGvertex) * path->nstroke);
			offset += path->nstroke;
		}
	}

	// Quad
	call->triangleOffset = offset;
	call->triangleCount = 6;
	quad = &gl->verts[call->triangleOffset];
	glnvg__vset(&quad[0], bounds[0], bounds[3], 0.5f, 1.0f);
	glnvg__vset(&quad[1], bounds[2], bounds[3], 0.5f, 1.0f);
	glnvg__vset(&quad[2], bounds[2], bounds[1], 0.5f, 1.0f);

	glnvg__vset(&quad[3], bounds[0], bounds[3], 0.5f, 1.0f);
	glnvg__vset(&quad[4], bounds[2], bounds[1], 0.5f, 1.0f);
	glnvg__vset(&quad[5], bounds[0], bounds[1], 0.5f, 1.0f);

	// Setup uniforms for draw calls
	if (call->type == GLNVG_FILL) {
		call->uniformOffset = glnvg__allocFragUniforms(gl, 2);
		if (call->uniformOffset == -1) goto error;
		// Simple shader for stencil
		frag = nvg__fragUniformPtr(gl, call->uniformOffset);
		memset(frag, 0, sizeof(*frag));
		frag->strokeThr = -1.0f;
		frag->type = NSVG_SHADER_SIMPLE;
		// Fill shader
		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset + gl->fragSize), paint, scissor, fringe, fringe, -1.0f);
	} else {
		call->uniformOffset = glnvg__allocFragUniforms(gl, 1);
		if (call->uniformOffset == -1) goto error;
		// Fill shader
		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset), paint, scissor, fringe, fringe, -1.0f);
	}

	return;

error:
	// We get here if call alloc was ok, but something else is not.
	// Roll back the last call to prevent drawing it.
	if (gl->ncalls > 0) gl->ncalls--;
}

static void glnvg__renderStroke(void* uptr, NVGpaint* paint, NVGscissor* scissor, float fringe,
								float strokeWidth, const NVGpath* paths, int npaths)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGcall* call = glnvg__allocCall(gl);
	int i, maxverts, offset;

	if (call == NULL) return;

	call->type = GLNVG_STROKE;
	call->pathOffset = glnvg__allocPaths(gl, npaths);
	if (call->pathOffset == -1) goto error;
	call->pathCount = npaths;
	call->image = paint->image;

	// Allocate vertices for all the paths.
	maxverts = glnvg__maxVertCount(paths, npaths);
	offset = glnvg__allocVerts(gl, maxverts);
	if (offset == -1) goto error;

	for (i = 0; i < npaths; i++) {
		GLNVGpath* copy = &gl->paths[call->pathOffset + i];
		const NVGpath* path = &paths[i];
		memset(copy, 0, sizeof(GLNVGpath));
		if (path->nstroke) {
			copy->strokeOffset = offset;
			copy->strokeCount = path->nstroke;
			memcpy(&gl->verts[offset], path->stroke, sizeof(NVGvertex) * path->nstroke);
			offset += path->nstroke;
		}
	}

	if (gl->flags & NVG_STENCIL_STROKES) {
		// Fill shader
		call->uniformOffset = glnvg__allocFragUniforms(gl, 2);
		if (call->uniformOffset == -1) goto error;

		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset), paint, scissor, strokeWidth, fringe, -1.0f);
		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset + gl->fragSize), paint, scissor, strokeWidth, fringe, 1.0f - 0.5f/255.0f);

	} else {
		// Fill shader
		call->uniformOffset = glnvg__allocFragUniforms(gl, 1);
		if (call->uniformOffset == -1) goto error;
		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset), paint, scissor, strokeWidth, fringe, -1.0f);
	}

	return;

error:
	// We get here if call alloc was ok, but something else is not.
	// Roll back the last call to prevent drawing it.
	if (gl->ncalls > 0) gl->ncalls--;
}

static void glnvg__renderTriangles(void* uptr, NVGpaint* paint, NVGscissor* scissor,
								   const NVGvertex* verts, int nverts)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGcall* call = glnvg__allocCall(gl);
	GLNVGfragUniforms* frag;

	if (call == NULL) return;

	call->type = GLNVG_TRIANGLES;
	call->image = paint->image;

	// Allocate vertices for all the paths.
	call->triangleOffset = glnvg__allocVerts(gl, nverts);
	if (call->triangleOffset == -1) goto error;
	call->triangleCount = nverts;

	memcpy(&gl->verts[call->triangleOffset], verts, sizeof(NVGvertex) * nverts);

	// Fill shader
	call->uniformOffset = glnvg__allocFragUniforms(gl, 1);
	if (call->uniformOffset == -1) goto error;
	frag = nvg__fragUniformPtr(gl, call->uniformOffset);
	glnvg__convertPaint(gl, frag, paint, scissor, 1.0f, 1.0f, -1.0f);
	frag->type = NSVG_SHADER_IMG;

	return;

error:
	// We get here if call alloc was ok, but something else is not.
	// Roll back the last call to prevent drawing it.
	if (gl->ncalls > 0) gl->ncalls--;
}

static void glnvg__renderDelete(void* uptr)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	int i;
	if (gl == NULL) return;

	glnvg__deleteShader(gl, &gl->shader);

#if NANOVG_GL3
#if NANOVG_GL_USE_UNIFORMBUFFER
	if (gl->fragBuf != 0)
		gl->DeleteBuffers(1, &gl->fragBuf);
#endif
	if (gl->vertArr != 0)
		gl->DeleteVertexArrays(1, &gl->vertArr);
#endif
	if (gl->vertBuf != 0)
		gl->DeleteBuffers(1, &gl->vertBuf);

	for (i = 0; i < gl->ntextures; i++) {
		if (gl->textures[i].tex != 0 && (gl->textures[i].flags & NVG_IMAGE_NODELETE) == 0)
			gl->DeleteTextures(1, &gl->textures[i].tex);
	}
	free(gl->textures);

	free(gl->paths);
	free(gl->verts);
	free(gl->uniforms);
	free(gl->calls);

	free(gl);
}


NVGcontext* EXT(nvgCreate)(JNIEnv* env, int flags)
{
	jclass NanoVGGLConfig = (*env)->FindClass(env, "org/lwjgl/nanovg/NanoVGGLConfig");
	jmethodID config = (*env)->GetStaticMethodID(env, NanoVGGLConfig, CONFIG_METHOD, "(J)V");

	NVGparams params;
	NVGcontext* ctx = NULL;
	GLNVGcontext* gl = (GLNVGcontext*)malloc(sizeof(GLNVGcontext));
	if (gl == NULL) goto error;
	memset(gl, 0, sizeof(GLNVGcontext));

	(*env)->CallStaticVoidMethod(env, NanoVGGLConfig, config, (jlong)(intptr_t)&gl->ActiveTexture);
	if ( (*env)->ExceptionCheck(env) )
		goto error;

	memset(&params, 0, sizeof(params));
	params.renderCreate = glnvg__renderCreate;
	params.renderCreateTexture = glnvg__renderCreateTexture;
	params.renderDeleteTexture = glnvg__renderDeleteTexture;
	params.renderUpdateTexture = glnvg__renderUpdateTexture;
	params.renderGetTextureSize = glnvg__renderGetTextureSize;
	params.renderViewport = glnvg__renderViewport;
	params.renderCancel = glnvg__renderCancel;
	params.renderFlush = glnvg__renderFlush;
	params.renderFill = glnvg__renderFill;
	params.renderStroke = glnvg__renderStroke;
	params.renderTriangles = glnvg__renderTriangles;
	params.renderDelete = glnvg__renderDelete;
	params.userPtr = gl;
	params.edgeAntiAlias = flags & NVG_ANTIALIAS ? 1 : 0;

	gl->flags = flags;

	ctx = nvgCreateInternal(&params);
	if (ctx == NULL) goto error;

	return ctx;

error:
	// 'gl' is freed by nvgDeleteInternal.
	if (ctx != NULL) nvgDeleteInternal(ctx);
	return NULL;
}

void EXT(nvgDelete)(NVGcontext* ctx)
{
	nvgDeleteInternal(ctx);
}

int EXT(nvglCreateImageFromHandle)(NVGcontext* ctx, GLuint textureId, int w, int h, int imageFlags)
{
	GLNVGcontext* gl = (GLNVGcontext*)nvgInternalParams(ctx)->userPtr;
	GLNVGtexture* tex = glnvg__allocTexture(gl);

	if (tex == NULL) return 0;

	tex->type = NVG_TEXTURE_RGBA;
	tex->tex = textureId;
	tex->flags = imageFlags;
	tex->width = w;
	tex->height = h;

	return tex->id;
}

GLuint EXT(nvglImageHandle)(NVGcontext* ctx, int image)
{
	GLNVGcontext* gl = (GLNVGcontext*)nvgInternalParams(ctx)->userPtr;
	GLNVGtexture* tex = glnvg__findTexture(gl, image);
	return tex->tex;
}

#endif /* NANOVG_GL_IMPLEMENTATION */
