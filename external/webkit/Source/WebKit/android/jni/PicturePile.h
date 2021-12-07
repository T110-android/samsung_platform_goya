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

#ifndef PicturePile_h
#define PicturePile_h

#include "IntRect.h"
#include "IntSize.h"
#include "PrerenderedInval.h"
#include "SkBitmap.h"
#include "SkRegion.h"
#include "SkRefCnt.h"

#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/Vector.h>

// 4.2 Merge BEGIN <<
#define USE_RECORDING_CONTEXT true
//#if USE_RECORDING_CONTEXT
namespace WebCore {
class Recording;
}
//typedef WebCore::Recording Picture;
//#else
// 4.2 Merge END >>
class SkPicture;
// 4.2 Merge BEGIN <<
//typedef SkPicture Picture;
//#endif
// 4.2 Merge END >>
class SkCanvas;

namespace WebCore {

class GraphicsContext;

class PicturePainter {
public:
    virtual void paintContents(GraphicsContext* gc, IntRect& dirty) = 0;
    virtual SkCanvas* createPrerenderCanvas(PrerenderedInval* prerendered)
    {
        return 0;
    }
    virtual ~PicturePainter() {}
};

class PictureContainer {
public:
    SkPicture* skPicture;//4
    WebCore::Recording* recordingPicture;//4
    bool m_skiaPictureEnabled;
    IntRect area;
    bool dirty;
    RefPtr<PrerenderedInval> prerendered;
    float maxZoomScale;//4.2 Merge

    PictureContainer(const IntRect& area)
        : skPicture(0)
        , recordingPicture(0)
        , m_skiaPictureEnabled(false)
        , area(area)
        , dirty(true)
        , maxZoomScale(1)//4.2 Merge
    {}

//4.2 Merge : Removed  constructor ans destructor definitions
    PictureContainer(const PictureContainer& other);//4.2 Merge
    ~PictureContainer();//4.2 Merge
};

class PicturePile {
public:
//Samsung Change >>	
//    PicturePile() {}
    PicturePile();
//Samsung Change <<
    PicturePile(const PicturePile& other);
    bool isSkiaPictureEnabled(){return m_skiaPictureEnabled;};
    void setSkiaPictureEnabled(bool enable) {m_skiaPictureEnabled = enable;}	
//    PicturePile(SkPicture* picture); //4.2 Merge : removed in 4.2

    const IntSize& size() { return m_size; }

    void clearPrerenders();

    // used by PicturePileLayerContents
    void draw(SkCanvas* canvas);

    // Used by WebViewCore
    void invalidate(const IntRect& dirtyRect);
    void setSize(const IntSize& size);
    void updatePicturesIfNeeded(PicturePainter* painter);
    void reset();
    SkRegion& dirtyRegion() { return m_dirtyRegion; }
    PrerenderedInval* prerenderedInvalForArea(const IntRect& area);

// 4.2 Merge BEGIN <<
    // UI-side methods used to check content, after construction/updates are complete
    float maxZoomScale() const;
    bool isEmpty() const;
// 4.2 Merge END >>
private:
    void applyWebkitInvals();
    void updatePicture(PicturePainter* painter, PictureContainer& container);
    void drawPictureRecording(SkCanvas* canvas, PictureContainer& pc);
    Recording* recordPictureRecording(PicturePainter* painter, PictureContainer& pc);
    void drawPictureSkia(SkCanvas* canvas, PictureContainer& pc);
    SkPicture* recordPictureSkia(PicturePainter* painter, PictureContainer& pc);
    void appendToPile(const IntRect& inval, const IntRect& originalInval = IntRect());
// 4.2 Merge BEGIN <<
    void drawWithClipRecursive(SkCanvas* canvas, int index);
    //void drawPicture(SkCanvas* canvas, PictureContainer& pc);
// 4.2 Merge END >>

    IntSize m_size;
    Vector<PictureContainer> m_pile;
    Vector<IntRect> m_webkitInvals;
    SkRegion m_dirtyRegion;
    bool m_updatePictureTimeout;//Seems a SAMSUNG change
    bool m_skiaPictureEnabled; //For disabling Recording
};

} // namespace android

#endif // PicturePile_h
