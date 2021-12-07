#if !defined(PlatformWebGLTexture_h_)
#define PlatformWebGLTexture_h_

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "SkRefCnt.h"
#include <utils/StrongPointer.h>
#include <utils/RefBase.h>
#include <wtf/PassOwnPtr.h>

namespace android {
class SurfaceTexture;
}

namespace WebCore {

class WebGLTexture;
class HTMLVideoElement;
class VideoLayerAndroid;

class PlatformWebGLTexture
{
public:
	enum Type { eUnknown, eVideo };

	PlatformWebGLTexture(Type type) : m_type(type) {}
	virtual ~PlatformWebGLTexture() {} 
	Type type() { return  m_type; }
private:
	Type m_type;
};

class VideoLayerAndroid;
class PlatformWebGLVideoTextureFactory;
class PlatformWebGLVideoTexture;

typedef bool (PlatformWebGLVideoTexture::*VideoImageCallback)(android::sp<android::SurfaceTexture>& surface);

class PlatformWebGLVideoTexture : public PlatformWebGLTexture
{
friend class PlatformWebGLVideoTextureFactory;

public:
	~PlatformWebGLVideoTexture();
	bool texImage2D(HTMLVideoElement* vid, GLenum texture);
	bool doTexImage2D(android::sp<android::SurfaceTexture>& surface);
	bool texSubImage2D(HTMLVideoElement* vid, GLenum texture, GLint xOffset, GLint yOffset);
	bool doTexSubImage2D(android::sp<android::SurfaceTexture>& surface);
	void getGLState();
	void restoreGLState();

private:
	bool texCommonImage2D(HTMLVideoElement* vid, GLenum texture, VideoImageCallback callback );
	void createImage(android::sp<android::SurfaceTexture>& surface);
	void createFBO(bool resize);
	PlatformWebGLVideoTexture(WebGLTexture* parent);

	GLenum m_externalTextureId;
	GLenum m_realTexture;
	GLenum m_FBO;
	EGLImageKHR m_eglImage;
	EGLDisplay m_dpy;
	android::wp<android::SurfaceTexture> m_surfaceTexture;
	SkRefPtr<VideoLayerAndroid> m_videoLayer;
	WebGLTexture* m_parent;
	unsigned int m_height;
	unsigned int m_width;
	float m_texTransform[16];

	GLint m_currentBuffer;
	GLint m_currentProgram;
	GLint m_currentFBO;
	mutable GLint m_xOffset;
	mutable GLint m_yOffset;
	GLint m_viewPort[4];
	GLboolean m_colorMask[4];
};


class PlatformWebGLVideoTextureFactory
{
friend PlatformWebGLVideoTexture::~PlatformWebGLVideoTexture();
public:
	static PassOwnPtr<PlatformWebGLVideoTexture> createObj(WebGLTexture* obj);
	static void draw(PlatformWebGLVideoTexture* webGLTexture);

private:
	static void deref();
	static void initShaderProgram();
	static void destroyShaderProgram();
	
	static int g_shaderRefCount;
	static GLenum g_program;
	static GLenum g_buffer;

	static GLenum g_vPositionLoc;
	static GLenum g_uTextureMatrix;
	static GLenum g_uVideoSampler;
};


} //namespace WebCore

#endif //PlatformWebGLTexture
