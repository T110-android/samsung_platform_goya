/*
 * Copyright 2012, The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "config.h"
#include "RenderVideo.h"
#include "RenderLayer.h"
#include "VideoLayerAndroid.h"
#include "WebGLTexture.h"
#include "HTMLVideoElement.h"
#include "PlatformWebGLTexture.h"
#include "GLUtils.h"
#include "EGL/egl.h"
#include <cutils/log.h>
#include <gui/SurfaceTexture.h>

#undef XLOG
#define XLOG(...) android_printLog(ANDROID_LOG_DEBUG, "PlatformWebGLVideoTexture", __VA_ARGS__)

//#define DEBUG_LOG(...)
#define DEBUG_LOG XLOG
#if 1
static void doLogLastGLError(const char* func, int line)
{
	GLenum error = glGetError();
	const char* errorStr;
	char buff[20];
	switch(error) {
	case GL_NO_ERROR:
		return;
	case GL_INVALID_ENUM:
		errorStr = "INVALID_ENUM";
		break; 
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		errorStr = "INVALID_FRAMEBUFFER_OPERATION";
		break; 
	case GL_INVALID_VALUE:
		errorStr = "INVALID_VALUE";
		break; 
	case GL_INVALID_OPERATION:
		errorStr = "INVALID_OPERATION";
		break; 
	case GL_OUT_OF_MEMORY:
		errorStr = "OUT_OF_MEMORY";
		break; 
	default:
		snprintf(buff, sizeof(buff) - 1, "0x%04x", error);
		errorStr = buff;
	}
	android_printLog(ANDROID_LOG_DEBUG, "WebGLError", "%s on line %d (%s)", errorStr, line, func);
}
#define logLastGLError(x) doLogLastGLError(x, __LINE__)
#define logLastGLError0 doLogLastGLError(__FUNCTION__, __LINE__)

#else
#define logLastGLError(x)
#endif

namespace WebCore {

int PlatformWebGLVideoTextureFactory::g_shaderRefCount = 0;
GLenum PlatformWebGLVideoTextureFactory::g_program = 0;
GLenum PlatformWebGLVideoTextureFactory::g_buffer = 0;

GLenum PlatformWebGLVideoTextureFactory::g_vPositionLoc = 0;
GLenum PlatformWebGLVideoTextureFactory::g_uTextureMatrix = 0;
GLenum PlatformWebGLVideoTextureFactory::g_uVideoSampler = 0;


PassOwnPtr<PlatformWebGLVideoTexture> PlatformWebGLVideoTextureFactory::createObj(WebGLTexture* obj)
{
	DEBUG_LOG("%s" , __func__);
	PassOwnPtr<PlatformWebGLVideoTexture> ret = adoptPtr(new PlatformWebGLVideoTexture(obj));
	if (g_shaderRefCount == 0) {
		initShaderProgram();
	} else {
		glValidateProgram(g_program);
		logLastGLError("createObj");
		GLint status;
		glGetProgramiv(g_program, GL_VALIDATE_STATUS, &status);
		logLastGLError("createObj");
		if (status != GL_TRUE) {
			destroyShaderProgram();
			initShaderProgram();
		}
	}
		

	++g_shaderRefCount;
	return ret;
}

void PlatformWebGLVideoTextureFactory::deref()
{
	DEBUG_LOG("%s" , __func__);
	--g_shaderRefCount;
	if (g_shaderRefCount == 0) {
		destroyShaderProgram();
	}
}

static GLuint loadShader(GLenum shaderType, const char* pSource)
{
	DEBUG_LOG("%s" , __func__);
	GLuint shader = glCreateShader(shaderType);
	if (shader) {
		glShaderSource(shader, 1, &pSource, 0);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*) malloc(infoLen);
				if (buf) {
				glGetShaderInfoLog(shader, infoLen, 0, buf);
				XLOG("could not compile shader %d:\n%s\n", shaderType, buf);
				free(buf);
			}
			glDeleteShader(shader);
			shader = 0;
			}
		}
	}
	return shader;
}

static GLuint createProgram(const char* pVertexSource, const char* pFragmentSource)
{
	DEBUG_LOG("%s" , __func__);
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		XLOG("couldn't load the vertex shader!");
		return -1;
	}

	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		XLOG("couldn't load the pixel shader!");
		return -1;
	}

	GLuint program = glCreateProgram();
	if (program) {
		glAttachShader(program, vertexShader);
		logLastGLError("glAttachShader vertex");
		glAttachShader(program, pixelShader);
		logLastGLError("glAttachShader pixel");
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = (char*) malloc(bufLength);
				if (buf) {
					glGetProgramInfoLog(program, bufLength, 0, buf);
					XLOG("could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = -1;
		}
	}
	//mark shaders for deletion when the program gets deleted
	glDeleteShader(vertexShader);
	glDeleteShader(pixelShader);
	return program;
}

void PlatformWebGLVideoTextureFactory::initShaderProgram()
{
	DEBUG_LOG("%s" , __func__);
	const char videoVertexShader[] =
	"attribute vec2 vPosition;\n"
	"uniform mat4 textureMatrix;\n"
	"varying vec2 v_texCoord;\n"
	"void main() {\n"
	"  vec4 pos = vec4(vPosition, 0.0, 1.0);\n"
	"  gl_Position = pos;\n"
	"  pos.x = 0.5 * (1.0 + pos.x);\n"
	"  pos.y = 0.5 * (1.0 - pos.y);\n"
	"  v_texCoord = vec2(textureMatrix * pos);\n"
	"}\n";

	const char videoFragmentShader[] =
	"#extension GL_OES_EGL_image_external : require\n"
	"precision mediump float;\n"
	"uniform samplerExternalOES s_yuvTexture;\n"
	"varying vec2 v_texCoord;\n"
	"void main() {\n"
	"  gl_FragColor = texture2D(s_yuvTexture, v_texCoord);\n"
	"}\n";

	g_program = createProgram(videoVertexShader, videoFragmentShader);
	if (g_program == 0) {
		return;
	}
	g_uTextureMatrix = glGetUniformLocation(g_program, "textureMatrix");
	g_uVideoSampler = glGetUniformLocation(g_program, "s_yuvTexture");
	g_vPositionLoc = glGetAttribLocation(g_program, "vPosition");
	
	const GLfloat coord[] = {
		-1.0f, -1.0f, 
		 1.0f, -1.0f, 
		-1.0f,  1.0f, 
		 1.0f,  1.0f 
	};

	glGenBuffers(1, &g_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, g_buffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), coord, GL_STATIC_DRAW);

	logLastGLError("init");
}

void PlatformWebGLVideoTextureFactory::destroyShaderProgram()
{
	DEBUG_LOG("%s" , __func__);
	glDeleteBuffers(1, &g_buffer); 
	glDeleteProgram(g_program);
	g_program = -1;
	g_buffer = -1;

	g_vPositionLoc = -1;
	g_uTextureMatrix = -1;
	g_uVideoSampler = -1;
}

void PlatformWebGLVideoTextureFactory::draw(PlatformWebGLVideoTexture* webGLTexture)
{
	DEBUG_LOG("%s" , __func__);
	glUseProgram(g_program);
	logLastGLError0;
    glUniformMatrix4fv(g_uTextureMatrix, 1, GL_FALSE, webGLTexture->m_texTransform);
	logLastGLError0;
    glActiveTexture(GL_TEXTURE0);
	logLastGLError0;
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, webGLTexture->m_externalTextureId);
	logLastGLError0;
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	logLastGLError0;
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	logLastGLError0;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	logLastGLError0;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	logLastGLError0;
    glUniform1i(g_uVideoSampler, 0);
	logLastGLError0;

    glBindBuffer(GL_ARRAY_BUFFER, g_buffer);
	logLastGLError0;
    glVertexAttribPointer(g_vPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	logLastGLError0;
    glEnableVertexAttribArray(g_vPositionLoc);
	logLastGLError0;

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	logLastGLError0;
}
	
void PlatformWebGLVideoTexture::getGLState()
{
	DEBUG_LOG("%s" , __func__);
	glGetIntegerv(GL_CURRENT_PROGRAM, &m_currentProgram);
	logLastGLError("get current program");
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &m_currentBuffer);
	logLastGLError("get current buffer");
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_currentFBO);
	logLastGLError("get current fbo");
	glGetIntegerv(GL_VIEWPORT, m_viewPort);
	logLastGLError("get current viewport");
	glGetBooleanv(GL_COLOR_WRITEMASK, m_colorMask);
	logLastGLError("get current viewport");
}
	
void PlatformWebGLVideoTexture::restoreGLState()
{
	DEBUG_LOG("%s" , __func__);
	glUseProgram(m_currentProgram);
	logLastGLError("restore program");
	glBindBuffer(GL_ARRAY_BUFFER, m_currentBuffer);
	logLastGLError("restore buffer");
	glBindFramebuffer(GL_FRAMEBUFFER, m_currentFBO);
	logLastGLError("restore fbo");
	glViewport(m_viewPort[0],m_viewPort[1], m_viewPort[2], m_viewPort[3]);
	logLastGLError("restore viewport");
	glColorMask(m_colorMask[0], m_colorMask[1], m_colorMask[2] ,m_colorMask[3]);
	logLastGLError("restore color mask");

}

PlatformWebGLVideoTexture::PlatformWebGLVideoTexture(WebGLTexture* parent) 
	: PlatformWebGLTexture(PlatformWebGLTexture::eVideo)
	, m_externalTextureId(0)
	, m_realTexture(0)
	, m_FBO(0)
	, m_eglImage(EGL_NO_IMAGE_KHR)
	, m_dpy(EGL_NO_DISPLAY)
	, m_surfaceTexture(NULL)
	, m_videoLayer(NULL)
	, m_parent( parent)
	, m_height (0)
	, m_width (0)
{
	DEBUG_LOG("%s" , __func__);
}

PlatformWebGLVideoTexture::~PlatformWebGLVideoTexture()
{
	DEBUG_LOG("%s" , __func__);
	glDeleteTextures(1, &m_externalTextureId);
	glDeleteFramebuffers(1, &m_FBO);
	eglDestroyImageKHR(m_dpy, m_eglImage);
	PlatformWebGLVideoTextureFactory::deref();
}


void PlatformWebGLVideoTexture::createImage(android::sp<android::SurfaceTexture>& surface)
{
	DEBUG_LOG("%s" , __func__);
	EGLBoolean ret;
	if (m_externalTextureId == 0) {
		glGenTextures(1, &m_externalTextureId);
	}
	if (m_dpy == EGL_NO_DISPLAY) {
		m_dpy = eglGetCurrentDisplay();
		if (m_dpy == EGL_NO_DISPLAY) {
			XLOG("PlatformWebGLVideoTexture::createImage error fetching egl display");
			GLUtils::checkEglError("eglGetCurrentDisplay", EGL_FALSE);
		}
	}
	if (m_eglImage != EGL_NO_IMAGE_KHR) {
		ret = eglDestroyImageKHR(m_dpy, m_eglImage);
		if (!ret) {
			XLOG("PlatformWebGLVideoTexture::createImage error destroying egl texture");
			GLUtils::checkEglError("eglDestroyImageKHR", ret);
		}
	}
	surface->updateTexImage();
	sp<GraphicBuffer> graphicBuffer(surface->getCurrentBuffer() );	
	EGLClientBuffer cbuf = (EGLClientBuffer)graphicBuffer->getNativeBuffer();
	EGLint attrs[] = {
		EGL_IMAGE_PRESERVED_KHR,	EGL_TRUE,
		EGL_NONE,
	};
	m_eglImage = eglCreateImageKHR(m_dpy, EGL_NO_CONTEXT,
			EGL_NATIVE_BUFFER_ANDROID, cbuf, attrs);
	if (m_eglImage == EGL_NO_IMAGE_KHR) {
		EGLint error = eglGetError();
		XLOG("error creating EGLImage: %#x", error);
	}
	glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_externalTextureId);
	logLastGLError("glBindTexture GL_TEXTURE_EXTERNAL_OES");
	glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, (GLeglImageOES)m_eglImage);
	logLastGLError("glEGLImageTargetTexture2DOES");
	m_surfaceTexture = surface;
	m_width = graphicBuffer->getWidth();
	m_height = graphicBuffer->getHeight();
	surface->getTransformMatrix(m_texTransform);
	DEBUG_LOG("Image created successfully");
}

void PlatformWebGLVideoTexture::createFBO(bool resize)
{
	DEBUG_LOG("%s" , __func__);
	if (!resize && m_FBO > 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		return;
	}

	if (m_FBO == 0) {
		glGenFramebuffers(1, &m_FBO);
		resize = true;
	}

	glBindTexture(GL_TEXTURE_2D, m_realTexture);
	logLastGLError("glBindTexture GL_TEXTURE_2D (createFBO)");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	logLastGLError("glTexImage2D (createFBO)");

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	logLastGLError("glBindFramebuffer (createFBO)");
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_realTexture, 0);
	logLastGLError("glFramebufferTexture2D (createFBO)");
	GLenum ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
	if ( ret != GL_FRAMEBUFFER_COMPLETE) {
		XLOG("PlatformWebGLVideoTexture::createFBO - framebuffer not complete %p", ret);
		return;
	}
}

bool PlatformWebGLVideoTexture::texCommonImage2D(HTMLVideoElement* vid, 
		GLenum texture, VideoImageCallback callback )
{
	DEBUG_LOG("%s" , __func__);
	LayerAndroid* layer = vid->platformLayer();
	if (!layer->isVideo()) {
		XLOG("PlatformWebGLVideoTexture::texImage2D - error: "
				"layer is not a video layer");
		return false;
	}
	m_videoLayer = static_cast<VideoLayerAndroid*>(layer);
	m_realTexture = texture;
	//We need to do this otherwise nothing will play
	MediaPlayer* player = vid->player();
    player->setFrameView(vid->document()->view());
	if (!player->visible() && !player->paused()) {
		XLOG("making player visible and starting play");
		player->setVisible(true);
		if (player->readyState() >= MediaPlayer::HaveCurrentData ) {
			player->play();
		}
	}

	bool ret = m_videoLayer->captureImage(this, callback);
	if (!ret) { 
		XLOG("Warning: video cannot be played at this time");
		if (!player->paused() && player->readyState() >= MediaPlayer::HaveCurrentData) {
			XLOG("retrying play again");
			player->play();
		}
		else {
			XLOG("Not attempting to replay (paused = %s readyState = %d)",
					player->paused() ? "true" : "false" , player->readyState());
		}
	}
	return ret; 
}
bool PlatformWebGLVideoTexture::texImage2D(HTMLVideoElement* vid, GLenum texture)
{
	m_xOffset = 0;
	m_yOffset = 0;
	return texCommonImage2D(vid, texture, &PlatformWebGLVideoTexture::doTexImage2D);
}

	
bool PlatformWebGLVideoTexture::doTexImage2D(android::sp<android::SurfaceTexture>& surface)
{
	DEBUG_LOG("%s" , __func__);
	int status = surface->updateTexImage();
	if (status != android::NO_ERROR) {
		DEBUG_LOG("update status returned %d", status);
	}
	sp<GraphicBuffer> graphicsBuffer = surface->getCurrentBuffer();	
	if (!graphicsBuffer.get() ) {
		DEBUG_LOG("The surface's graphics buffer does not exist");
		return false;
	}
	getGLState();
	DEBUG_LOG("GraphicsBuffer %p height %d width %d", graphicsBuffer.get(), graphicsBuffer->getHeight(), graphicsBuffer->getWidth());
	bool resizeImage = (graphicsBuffer->getHeight() != m_height ||graphicsBuffer->getWidth() != m_width);
	createImage(surface);
	createFBO(resizeImage);
	glViewport(0, 0, m_width, m_height);
	PlatformWebGLVideoTextureFactory::draw(this);
	restoreGLState();
	return true;
}

bool PlatformWebGLVideoTexture::texSubImage2D(HTMLVideoElement* vid, GLenum texture, GLint xOffset, GLint yOffset)
{
	m_xOffset = xOffset;
	m_yOffset = yOffset;
	return texCommonImage2D(vid, texture, &PlatformWebGLVideoTexture::doTexSubImage2D);
}
	
bool PlatformWebGLVideoTexture::doTexSubImage2D(android::sp<android::SurfaceTexture>& surface)
{
	DEBUG_LOG("%s" , __func__);
	int status = surface->updateTexImage();
	if (status != android::NO_ERROR) {
		DEBUG_LOG("update status returned %d", status);
	}
	sp<GraphicBuffer> graphicsBuffer = surface->getCurrentBuffer();	
	if (!graphicsBuffer.get() ) {
		DEBUG_LOG("The surface's graphics buffer does not exist");
		return false;
	}
	getGLState();
	DEBUG_LOG("GraphicsBuffer %p height %d width %d", graphicsBuffer.get(), graphicsBuffer->getHeight(), graphicsBuffer->getWidth());
	bool resizeImage = (graphicsBuffer->getHeight() != m_height ||graphicsBuffer->getWidth() != m_width);
	createImage(surface);
	createFBO(resizeImage);
	glViewport(m_xOffset, m_yOffset, m_width, m_height);
	glScissor(m_xOffset, m_yOffset, m_width, m_height);
	glEnable(GL_SCISSOR_TEST);
	PlatformWebGLVideoTextureFactory::draw(this);
	glDisable(GL_SCISSOR_TEST);
	restoreGLState();
	return true;
}

} //WebCore

