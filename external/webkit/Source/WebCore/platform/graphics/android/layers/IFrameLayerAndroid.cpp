#define LOG_TAG "IFrameLayerAndroid"
#define LOG_NDEBUG 1

#include "config.h"
#include "IFrameLayerAndroid.h"
#include "FixedPositioning.h"//Seems a SAMSUNG change

#if USE(ACCELERATED_COMPOSITING)

#include "AndroidLog.h"
#include "DumpLayer.h"

namespace WebCore {

IFrameLayerAndroid* IFrameLayerAndroid::updatePosition(SkRect viewport,
                                                       IFrameLayerAndroid* parentIframeLayer)
{
    // As we are an iframe, accumulate the offset from the parent with
    // the current position, and change the parent pointer.
    ////Seems a SAMSUNG change
    if ( m_fixedPosition ) {
        m_fixedPosition->updatePosition(viewport, parentIframeLayer);
    }

    // If this is the top level, take the current position
    SkPoint parentOffset;
    parentOffset.set(0,0);
    if (parentIframeLayer)
        parentOffset = parentIframeLayer->getPosition();

    SkPoint offset = parentOffset + getPosition();
    m_iframeOffset = IntPoint(offset.fX, offset.fY);

    return this;
}

void IFrameLayerAndroid::dumpLayer(FILE* file, int indentLevel) const
{
	//4.2 MergeCommented for compilation error
    //writeIntVal(file, indentLevel + 1, "m_isIframe", true);
    //writeIntPoint(file, indentLevel + 1, "m_iframeOffset", m_iframeOffset);
    //LayerAndroid::dumpLayer(dumper);
    //dumper->writeIntPoint("m_iframeOffset", m_iframeOffset);

    LayerAndroid::dumpLayer(file, indentLevel); //Seems a SAMSUNG change
}

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)
