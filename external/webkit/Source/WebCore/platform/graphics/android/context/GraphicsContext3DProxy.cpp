

#include "GraphicsContext3DProxy.h"
#include "GraphicsContext3DInternal.h"
#include "GLUtils.h"

namespace WebCore {

GraphicsContext3DProxy::GraphicsContext3DProxy()
{
    LOGWEBGL("GraphicsContext3DProxy::GraphicsContext3DProxy(), this = %p", this);
}

GraphicsContext3DProxy::~GraphicsContext3DProxy()
{
    LOGWEBGL("GraphicsContext3DProxy::~GraphicsContext3DProxy(), this = %p", this);
}

void GraphicsContext3DProxy::setGraphicsContext(GraphicsContext3DInternal* context)
{
    MutexLocker lock(m_mutex);
    m_context = context;
}

void GraphicsContext3DProxy::incr()
{
    MutexLocker lock(m_mutex);
    m_refcount++;
}

void GraphicsContext3DProxy::decr()
{
    MutexLocker lock(m_mutex);
    m_refcount--;
    if (m_refcount == 0) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
}

bool GraphicsContext3DProxy::lockFrontBuffer(GLuint& texture, SkRect& rect)
{
    MutexLocker lock(m_mutex);
    if (!m_context) {
        return false;
    }
    EGLImageKHR image;
    bool locked = m_context->lockFrontBuffer(image, rect);
    if (locked) {
        if (m_texture == 0)
            glGenTextures(1, &m_texture);

#ifdef USE_GL_TEXTURE_EXTERNAL_OES
        glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_texture);
		glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, image);
#else
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, image);
#endif
        
        texture = m_texture;
    }
    return locked;
}

void GraphicsContext3DProxy::releaseFrontBuffer()
{
    MutexLocker lock(m_mutex);
    if (!m_context) {
        return;
    }
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);
    m_context->releaseFrontBuffer();
}
#if  ENABLE(ACCELERATED_2D_CANVAS)
 void GraphicsContext3DProxy::willPublish()
{
    if (!m_context) {
        return;
    }
	
	m_context->willPublish();
}
#endif

}
