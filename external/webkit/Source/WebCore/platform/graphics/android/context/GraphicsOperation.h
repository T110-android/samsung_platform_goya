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

#ifndef GraphicsOperation_h
#define GraphicsOperation_h

// 4.2 Merge BEGIN <<
//Removed in 4.2
//#if USE(ACCELERATED_COMPOSITING)
// 4.2 Merge END >>
#include "Color.h"
#include "FloatRect.h"
#include "GlyphBuffer.h"
#include "Font.h"
#include "IntRect.h"
#include "PlatformGraphicsContext.h"
#include "PlatformGraphicsContextSkia.h"
#include "SkCanvas.h"
#include "SkShader.h"
#include "SkRefCnt.h"

#include <utils/threads.h>
#include <wtf/text/CString.h>

#define TYPE_CASE(type) case type: return #type;

// 4.2 Merge BEGIN <<
#define DEBUG_GRAPHICS_OPERATIONS false

#if DEBUG_GRAPHICS_OPERATIONS
#define TYPE(x) virtual OperationType type() { return x; }
#else
#define TYPE(x)
#endif
// 4.2 Merge END >>
namespace WebCore {

// 4.2 Merge BEGIN <<
class CanvasState;
class LinearAllocator;
// 4.2 Merge END >>
namespace GraphicsOperation {
// 4.2 Merge BEGIN <<
//Removed in 4.2
//class Operation : public SkRefCnt {
// 4.2 Merge END >>

// 4.2 Merge BEGIN <<
class Operation {
// 4.2 Merge END >>
public:
// 4.2 Merge BEGIN <<
    Operation()
        : m_state(0)
        , m_canvasState(0)
    {}

    void* operator new(size_t size, LinearAllocator* allocator);

    // Purposely not implemented - use a LinearAllocator please
    void* operator new(size_t size);
    void operator delete(void* ptr);

    // This m_state is applied by ourselves
    PlatformGraphicsContext::State* m_state;
    // This is the canvas state that this operation needs
    // Only used for drawing operations, state operations will be undefined
    CanvasState* m_canvasState;

    bool apply(PlatformGraphicsContext* context) {
        if (m_state)
            context->setRawState(m_state);
        return applyImpl(context);
    }
    virtual bool applyImpl(PlatformGraphicsContext* context) = 0;
    virtual ~Operation() {}

    virtual const IntRect*  opaqueRect() { return 0; }
    virtual bool isOpaque() { return false; }
    virtual void setOpaqueRect(const IntRect& bounds) {}

#if DEBUG_GRAPHICS_OPERATIONS
// 4.2 Merge END >>
    typedef enum { UndefinedOperation
                  // State management
// 4.2 Merge BEGIN <<
//Removed in 4.2
//                  , BeginTransparencyLayerOperation
//                  , EndTransparencyLayerOperation
//                  , SaveOperation
//                  , RestoreOperation
//                  // State setters
//                  , SetAlphaOperation
//                  , SetCompositeOpOperation
//                  , SetFillColorOperation
//                  , SetFillShaderOperation
//                  , SetLineCapOperation
//                  , SetLineDashOperation
//                  , SetLineJoinOperation
//                  , SetMiterLimitOperation
//                  , SetShadowOperation
//                  , SetShouldAntialiasOperation
//                  , SetStrokeColorOperation
//                  , SetStrokeShaderOperation
//                  , SetStrokeStyleOperation
//                  , SetStrokeThicknessOperation
//                  // Paint setup
//                  , SetupPaintFillOperation
//                  , SetupPaintShadowOperation
//                  , SetupPaintStrokeOperation
// 4.2 Merge END >>
                  // Matrix operations
                  , ConcatCTMOperation
                  , ScaleOperation
                  , RotateOperation
                  , TranslateOperation
                  // Clipping
                  , InnerRoundedRectClipOperation
                  , ClipOperation
                  , ClipPathOperation
                  , ClipOutOperation
                  , ClearRectOperation
                  // Drawing
                  , DrawBitmapPatternOperation
                  , DrawBitmapRectOperation
// 4.2 Merge BEGIN <<
                  , DrawConvexPolygonQuadOperation
// 4.2 Merge END >>
                  , DrawEllipseOperation
// 4.2 Merge BEGIN <<
                  , DrawFocusRingOperation
// 4.2 Merge END >>
                  , DrawLineOperation
                  , DrawLineForTextOperation
                  , DrawLineForTextCheckingOperation
                  , DrawRectOperation
                  , FillPathOperation
                  , FillRectOperation
                  , FillTransparectRectOperation
                  , FillRoundedRectOperation
                  , StrokeArcOperation
                  , StrokePathOperation
                  , StrokeRectOperation
// 4.2 Merge BEGIN <<
                  , DrawMediaButtonOperation
// 4.2 Merge END >>
                  // Text
// 4.2 Merge      , DrawComplexTextOperation //Removed in 4.2
// 4.2 Merge      , DrawTextOperation //Removed in 4.2
                  , DrawPosTextOperation
    } OperationType;
// 4.2 Merge BEGIN <<
//Removed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) = 0;
//    virtual ~Operation() {}
//    virtual OperationType type() { return UndefinedOperation; }
//    virtual String parameters() { return ""; }
// 4.2 Merge END >>
    const char* name() //4.2 Merge : renamed from String name() to 'const char* name()'
    {
        switch (type()) {
            TYPE_CASE(UndefinedOperation)
            // State management
// 4.2 Merge BEGIN <<
//Removed in 4.2
//            TYPE_CASE(BeginTransparencyLayerOperation)
//            TYPE_CASE(EndTransparencyLayerOperation)
//            TYPE_CASE(SaveOperation)
//            TYPE_CASE(RestoreOperation)
//            // State setters
//            TYPE_CASE(SetAlphaOperation)
//            TYPE_CASE(SetCompositeOpOperation)
//            TYPE_CASE(SetFillColorOperation)
//            TYPE_CASE(SetFillShaderOperation)
//            TYPE_CASE(SetLineCapOperation)
//            TYPE_CASE(SetLineDashOperation)
//            TYPE_CASE(SetLineJoinOperation)
//            TYPE_CASE(SetMiterLimitOperation)
//            TYPE_CASE(SetShadowOperation)
//            TYPE_CASE(SetShouldAntialiasOperation)
//            TYPE_CASE(SetStrokeColorOperation)
//            TYPE_CASE(SetStrokeShaderOperation)
//            TYPE_CASE(SetStrokeStyleOperation)
//            TYPE_CASE(SetStrokeThicknessOperation)
//            // Paint setup
//            TYPE_CASE(SetupPaintFillOperation)
//            TYPE_CASE(SetupPaintShadowOperation)
//            TYPE_CASE(SetupPaintStrokeOperation)
// 4.2 Merge END >>
            // Matrix operations
            TYPE_CASE(ConcatCTMOperation)
            TYPE_CASE(ScaleOperation)
            TYPE_CASE(RotateOperation)
            TYPE_CASE(TranslateOperation)
            // Clipping
            TYPE_CASE(InnerRoundedRectClipOperation)
            TYPE_CASE(ClipOperation)
            TYPE_CASE(ClipPathOperation)
            TYPE_CASE(ClipOutOperation)
            TYPE_CASE(ClearRectOperation)
            // Drawing
            TYPE_CASE(DrawBitmapPatternOperation)
            TYPE_CASE(DrawBitmapRectOperation)
// 4.2 Merge BEGIN <<
            TYPE_CASE(DrawConvexPolygonQuadOperation)
// 4.2 Merge END >>
            TYPE_CASE(DrawEllipseOperation)
// 4.2 Merge BEGIN <<
            TYPE_CASE(DrawFocusRingOperation)
// 4.2 Merge END >>
            TYPE_CASE(DrawLineOperation)
            TYPE_CASE(DrawLineForTextOperation)
            TYPE_CASE(DrawLineForTextCheckingOperation)
            TYPE_CASE(DrawRectOperation)
            TYPE_CASE(FillPathOperation)
            TYPE_CASE(FillRectOperation)
            TYPE_CASE(FillTransparectRectOperation)
            TYPE_CASE(FillRoundedRectOperation)
            TYPE_CASE(StrokeArcOperation)
            TYPE_CASE(StrokePathOperation)
            TYPE_CASE(StrokeRectOperation)
// 4.2 Merge BEGIN <<
            TYPE_CASE(DrawMediaButtonOperation)
// 4.2 Merge END >>
            // Text
//          TYPE_CASE(DrawComplexTextOperation) // 4.2 Merge : Removed in 4.2
//          TYPE_CASE(DrawTextOperation) // 4.2 Merge : Removed in 4.2
// 4.2 Merge BEGIN <<
            TYPE_CASE(DrawPosTextOperation)
// 4.2 Merge END >>
        }
        return "Undefined";
    }
// 4.2 Merge BEGIN <<
#endif //#if DEBUG_GRAPHICS_OPERATIONS
    TYPE(UndefinedOperation)
// 4.2 Merge END >>
};

// 4.2 Merge BEGIN <<
class PossiblyOpaqueOperation : public Operation {
public:
    virtual const IntRect* opaqueRect() { return &m_absoluteOpaqueRect; }
    virtual void setOpaqueRect(const IntRect& bounds) { m_absoluteOpaqueRect = bounds; }

private:
    IntRect m_absoluteOpaqueRect;
};
// 4.2 Merge END >>

// 4.2 Merge BEGIN <<
//Removed in 4.2

//**************************************
// State management
//**************************************

//class BeginTransparencyLayer : public Operation {
//public:
//    BeginTransparencyLayer(const float opacity) : m_opacity(opacity) {}
//    virtual void apply(PlatformGraphicsContext* context) { context->beginTransparencyLayer(m_opacity); }
//    virtual OperationType type() { return BeginTransparencyLayerOperation; }
//private:
//    float m_opacity;
//};
//class EndTransparencyLayer : public Operation {
//public:
//    EndTransparencyLayer() {}
//    virtual void apply(PlatformGraphicsContext* context) { context->endTransparencyLayer(); }
//    virtual OperationType type() { return EndTransparencyLayerOperation; }
//};
//class Save : public Operation {
//public:
//    virtual void apply(PlatformGraphicsContext* context) { context->save(); }
//    virtual OperationType type() { return SaveOperation; }
//};
//class Restore : public Operation {
//public:
//    virtual void apply(PlatformGraphicsContext* context) { context->restore(); }
//    virtual OperationType type() { return RestoreOperation; }
//};

//**************************************
// State setters
//**************************************

//class SetAlpha : public Operation {
//public:
//    SetAlpha(const float alpha) : m_alpha(alpha) {}
//    virtual void apply(PlatformGraphicsContext* context) { context->setAlpha(m_alpha); }
//    virtual OperationType type() { return SetAlphaOperation; }
//private:
//    float m_alpha;
//};

//class SetCompositeOperation : public Operation {
//public:
//    SetCompositeOperation(CompositeOperator op) : m_operator(op) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setCompositeOperation(m_operator);
//    }
//    virtual OperationType type() { return SetCompositeOpOperation; }
//private:
//    CompositeOperator m_operator;
//};

//class SetFillColor : public Operation {
//public:
//    SetFillColor(Color color) : m_color(color) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setFillColor(m_color);
//    }
//    virtual OperationType type() { return SetFillColorOperation; }
//    virtual String parameters() {
//        return String::format("r: %d g: %d b: %d a: %d",
//                              m_color.red(),
//                              m_color.green(),
//                              m_color.blue(),
//                              m_color.alpha());
//    }
//private:
//    Color m_color;
//};

//class SetFillShader : public Operation {
//public:
//    SetFillShader(SkShader* shader) : m_shader(shader) {
//        SkSafeRef(m_shader);
//    }
//    ~SetFillShader() { SkSafeUnref(m_shader); }
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setFillShader(m_shader);
//    }
//    virtual OperationType type() { return SetFillShaderOperation; }
//private:
//    SkShader* m_shader;
//};

//class SetLineCap : public Operation {
//public:
//    SetLineCap(LineCap cap) : m_cap(cap) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setLineCap(m_cap);
//    }
//    virtual OperationType type() { return SetLineCapOperation; }
//private:
//    LineCap m_cap;
//};

//class SetLineDash : public Operation {
//public:
//    SetLineDash(const DashArray& dashes, float dashOffset)
//        : m_dashes(dashes), m_dashOffset(dashOffset) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setLineDash(m_dashes, m_dashOffset);
//    }
//    virtual OperationType type() { return SetLineDashOperation; }
//private:
//    DashArray m_dashes;
//    float m_dashOffset;
//};

//class SetLineJoin : public Operation {
//public:
//    SetLineJoin(LineJoin join) : m_join(join) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setLineJoin(m_join);
//    }
//    virtual OperationType type() { return SetLineJoinOperation; }
//private:
//    LineJoin m_join;
//};

//class SetMiterLimit : public Operation {
//public:
//    SetMiterLimit(float limit) : m_limit(limit) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setMiterLimit(m_limit);
//    }
//    virtual OperationType type() { return SetMiterLimitOperation; }
//private:
//    float m_limit;
//};

//class SetShadow : public Operation {
//public:
//    SetShadow(int radius, int dx, int dy, SkColor c)
//        : m_radius(radius), m_dx(dx), m_dy(dy), m_color(c) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setShadow(m_radius, m_dx, m_dy, m_color);
//    }
//    virtual OperationType type() { return SetShadowOperation; }
//private:
//    int m_radius;
//    int m_dx;
//    int m_dy;
//    SkColor m_color;
//};

//class SetShouldAntialias : public Operation {
//public:
//    SetShouldAntialias(bool useAA) : m_useAA(useAA) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setShouldAntialias(m_useAA);
//    }
//    virtual OperationType type() { return SetShouldAntialiasOperation; }
//private:
//    bool m_useAA;
//};

//class SetStrokeColor : public Operation {
//public:
//    SetStrokeColor(const Color& c) : m_color(c) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setStrokeColor(m_color);
//    }
//    virtual OperationType type() { return SetStrokeColorOperation; }
//private:
//    Color m_color;
//};

//class SetStrokeShader : public Operation {
//public:
//    SetStrokeShader(SkShader* strokeShader) : m_shader(strokeShader) {
//        SkSafeRef(m_shader);
//    }
//    ~SetStrokeShader() { SkSafeUnref(m_shader); }
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setStrokeShader(m_shader);
//    }
//    virtual OperationType type() { return SetStrokeShaderOperation; }
//private:
//    SkShader* m_shader;
//};

//class SetStrokeStyle : public Operation {
//public:
//    SetStrokeStyle(StrokeStyle style) : m_style(style) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setStrokeStyle(m_style);
//    }
//    virtual OperationType type() { return SetStrokeStyleOperation; }
//private:
//    StrokeStyle m_style;
//};

//class SetStrokeThickness : public Operation {
//public:
//    SetStrokeThickness(float thickness) : m_thickness(thickness) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setStrokeThickness(m_thickness);
//    }
//    virtual OperationType type() { return SetStrokeThicknessOperation; }
//private:
//    float m_thickness;
//};

//**************************************
// Paint setup
//**************************************

//class SetupPaintFill : public Operation {
//public:
//    SetupPaintFill(SkPaint* paint) : m_paint(*paint) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setupPaintFill(&m_paint);
//    }
//    virtual OperationType type() { return SetupPaintFillOperation; }
//private:
//    SkPaint m_paint;
//};

//class SetupPaintShadow : public Operation {
//public:
//    SetupPaintShadow(SkPaint* paint, SkPoint* offset)
//        : m_paint(*paint), m_offset(*offset) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setupPaintShadow(&m_paint, &m_offset);
//    }
//    virtual OperationType type() { return SetupPaintShadowOperation; }
//private:
//    SkPaint m_paint;
//    SkPoint m_offset;
//};

//class SetupPaintStroke : public Operation {
//public:
//    SetupPaintStroke(SkPaint* paint, SkRect* rect, bool isHLine)
//        : m_paint(*paint), m_rect(*rect), m_isHLine(isHLine) {}
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->setupPaintStroke(&m_paint, &m_rect, m_isHLine);
//    }
//    virtual OperationType type() { return SetupPaintStrokeOperation; }
//private:
//    SkPaint m_paint;
//    SkRect m_rect;
//    bool m_isHLine;
//};
// 4.2 Merge END >>

//**************************************
// Matrix operations
//**************************************

class ConcatCTM : public Operation {
public:
    ConcatCTM(const AffineTransform& affine) : m_matrix(affine) {}
//4.2 Merge BEGIN <<
//Following code is changed in 42
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->concatCTM(m_matrix);
//    }
//    virtual OperationType type() { return ConcatCTMOperation; }
//4.2 Merge END >>

//4.2 Merge BEGIN <<
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->concatCTM(m_matrix);
        return true;
    }
    TYPE(ConcatCTMOperation)
//4.2 Merge END >>
private:
    AffineTransform m_matrix;
};

class Rotate : public Operation {
public:
    Rotate(float angleInRadians) : m_angle(angleInRadians) {}
//4.2 Merge BEGIN <<
//Following code is changed in 42
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->rotate(m_angle);
//    }
//    virtual OperationType type() { return RotateOperation; }
//4.2 Merge END >>

//4.2 Merge BEGIN <<
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->rotate(m_angle);
        return true;
    }
    TYPE(RotateOperation)
//4.2 Merge END >>
private:
    float m_angle;
};

class Scale : public Operation {
public:
    Scale(const FloatSize& size) : m_scale(size) {}
//4.2 Merge BEGIN <<
//Following code is changed in 42
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->scale(m_scale);
//    }
//    virtual OperationType type() { return ScaleOperation; }
//4.2 Merge END >>

//4.2 Merge BEGIN <<
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->scale(m_scale);
        return true;
    }
    TYPE(ScaleOperation)
//4.2 Merge END >>
private:
    FloatSize m_scale;
};

class Translate : public Operation {
public:
    Translate(float x, float y) : m_x(x), m_y(y) {}
//4.2 Merge BEGIN <<
//Following code is changed in 42
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->translate(m_x, m_y);
//    }
//    virtual OperationType type() { return TranslateOperation; }
//4.2 Merge END >>

//4.2 Merge BEGIN <<
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->translate(m_x, m_y);
        return true;
    }
    TYPE(TranslateOperation)
//4.2 Merge END >>
private:
    float m_x;
    float m_y;
};

//**************************************
// Clipping
//**************************************

class InnerRoundedRectClip : public Operation {
public:
    InnerRoundedRectClip(const IntRect& rect, int thickness)
        : m_rect(rect), m_thickness(thickness) {}
//4.2 Merge BEGIN <<
//Following code is changed in 42
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->addInnerRoundedRectClip(m_rect, m_thickness);
//    }
//    virtual OperationType type() { return InnerRoundedRectClipOperation; }
//4.2 Merge END >>

//4.2 Merge BEGIN <<
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->addInnerRoundedRectClip(m_rect, m_thickness);
        return true;
    }
    TYPE(InnerRoundedRectClipOperation)
//4.2 Merge END >>
private:
    IntRect m_rect;
    int m_thickness;
};

class Clip : public Operation {
public:
    Clip(const FloatRect& rect) : m_rect(rect) {}
//4.2 Merge BEGIN <<
//Following code is changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->clip(m_rect);
//    }
//    virtual OperationType type() { return ClipOperation; }
//4.2 Merge END >>

//4.2 Merge BEGIN <<
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        return context->clip(m_rect);
    }
    TYPE(ClipOperation)
//4.2 Merge END >>
private:
    const FloatRect m_rect;
};

class ClipPath : public Operation {
public:
    ClipPath(const Path& path, bool clipout = false)
        : m_path(path), m_clipOut(clipout), m_hasWindRule(false) {}
    void setWindRule(WindRule rule) { m_windRule = rule; m_hasWindRule = true; }
//4.2 Merge BEGIN <<
//Following code is changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        if (m_hasWindRule) {
//            context->clipPath(m_path, m_windRule);
//            return;
//        }
//4.2 Merge END >>

//4.2 Merge BEGIN <<
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        if (m_hasWindRule) {
            return context->clipPath(m_path, m_windRule);
        }

        if (m_clipOut)
            return context->clipOut(m_path); //Changed from return context->clipOut(m_path);
        else
            return context->clip(m_path); //Changed from return context->clip(m_path);
    }
    // virtual OperationType type() { return ClipPathOperation; }
    TYPE(ClipPathOperation)
//4.2 Merge END >>
private:
    const Path m_path;
    bool m_clipOut;
    WindRule m_windRule;
    bool m_hasWindRule;
};

class ClipOut : public Operation {
public:
    ClipOut(const IntRect& rect) : m_rect(rect) {}
//4.2 Merge BEGIN <<
//Following code is changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->clipOut(m_rect);
//    }
//    virtual OperationType type() { return ClipOutOperation; }
//4.2 Merge END >>

//4.2 Merge BEGIN <<
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        return context->clipOut(m_rect);
    }
    TYPE(ClipOutOperation)
//4.2 Merge END >>
private:
    const IntRect m_rect;
};

class ClearRect : public Operation {
public:
    ClearRect(const FloatRect& rect) : m_rect(rect) {}
//4.2 Merge BEGIN <<
//Following code is changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->clearRect(m_rect);
//    }
//    virtual OperationType type() { return ClearRectOperation; }
//4.2 Merge END >>

//4.2 Merge BEGIN <<
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->clearRect(m_rect);
        return true;
    }
    TYPE(ClearRectOperation)
//4.2 Merge END >>
private:
    FloatRect m_rect;
};

//**************************************
// Drawing
//**************************************

//4.2 Merge BEGIN
//class DrawBitmapPattern : public Operation {
class DrawBitmapPattern : public PossiblyOpaqueOperation {
//4.2 Merge END
public:
    DrawBitmapPattern(const SkBitmap& bitmap, const SkMatrix& matrix,
                      CompositeOperator op, const FloatRect& destRect)
        : m_bitmap(bitmap), m_matrix(matrix), m_operator(op), m_destRect(destRect) {}
//4.2 Merge BEGIN
//Following code changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->drawBitmapPattern(m_bitmap, m_matrix, m_operator, m_destRect);
//    }
//    virtual OperationType type() { return DrawBitmapPatternOperation; }
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->drawBitmapPattern(m_bitmap, m_matrix, m_operator, m_destRect);
        return true;
    }
    virtual bool isOpaque() { return m_bitmap.isOpaque(); }
    TYPE(DrawBitmapPatternOperation)
//4.2 Merge END
private:
//4.2 Merge BEGIN
    SkBitmap m_bitmap; // Changed from const SkBitmap m_bitmap;
//4.2 Merge END
    SkMatrix m_matrix;
    CompositeOperator m_operator;
    FloatRect m_destRect;
};
//4.2 Merge BEGIN
//class DrawBitmapRect : public Operation {
class DrawBitmapRect : public PossiblyOpaqueOperation {
//4.2 Merge END
public:
    DrawBitmapRect(const SkBitmap& bitmap, const SkIRect& srcR,
                   const SkRect& dstR, CompositeOperator op)
        : m_bitmap(bitmap), m_srcR(srcR), m_dstR(dstR), m_operator(op) {}
//4.2 Merge BEGIN
//Following code changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->drawBitmapRect(m_bitmap, &m_srcR, m_dstR, m_operator);
//    }
//    virtual OperationType type() { return DrawBitmapRectOperation; }
//    virtual String parameters() {
//        return String::format("%.2f, %.2f - %.2f x %.2f",
//                 m_dstR.fLeft, m_dstR.fTop,
//                 m_dstR.width(), m_dstR.height());
//    }
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->drawBitmapRect(m_bitmap, &m_srcR, m_dstR, m_operator);
        return true;
    }
    virtual bool isOpaque() { return m_bitmap.isOpaque(); }
    TYPE(DrawBitmapRectOperation)
//4.2 Merge END
private:
//4.2 Merge BEGIN
    SkBitmap m_bitmap; //Changed form const SkBitmap& m_bitmap;
//4.2 Merge END
    SkIRect m_srcR;
    SkRect m_dstR;
    CompositeOperator m_operator;
};
//4.2 Merge BEGIN
class DrawConvexPolygonQuad : public Operation {
public:
    DrawConvexPolygonQuad(const FloatPoint* points, bool shouldAntiAlias)
        : m_shouldAntiAlias(shouldAntiAlias)
    {
        memcpy(m_points, points, 4 * sizeof(FloatPoint));
    }
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->drawConvexPolygon(4, m_points, m_shouldAntiAlias);
        return true;
    }
    TYPE(DrawConvexPolygonQuadOperation)
private:
    bool m_shouldAntiAlias;
    FloatPoint m_points[4];
};
//4.2 Merge END

class DrawEllipse : public Operation {
public:
    DrawEllipse(const IntRect& rect) : m_rect(rect) {}
//4.2 Merge BEGIN
//Following code changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->drawEllipse(m_rect);
//    }
//    virtual OperationType type() { return DrawEllipseOperation; }
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->drawEllipse(m_rect);
        return true;
    }
    TYPE(DrawEllipseOperation)
//4.2 Merge END
private:
    IntRect m_rect;
};

//4.2 Merge BEGIN
class DrawFocusRing : public Operation {
public:
    DrawFocusRing(const Vector<IntRect>& rects, int width, int offset, const Color& color)
        : m_rects(rects)
        , m_width(width)
        , m_offset(offset)
        , m_color(color)
    {}
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->drawFocusRing(m_rects, m_width, m_offset, m_color);
        return true;
    }
    TYPE(DrawFocusRingOperation)
private:
    Vector<IntRect> m_rects;
    int m_width;
    int m_offset;
    Color m_color;
};
//4.2 Merge END

class DrawLine : public Operation {
public:
    DrawLine(const IntPoint& point1, const IntPoint& point2)
        : m_point1(point1), m_point2(point2) {}
//4.2 Merge BEGIN
//Following code changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->drawLine(m_point1, m_point2);
//    }
//    virtual OperationType type() { return DrawLineOperation; }
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->drawLine(m_point1, m_point2);
        return true;
    }
    TYPE(DrawLineOperation)
//4.2 Merge END
private:
    IntPoint m_point1;
    IntPoint m_point2;
};

class DrawLineForText : public Operation {
public:
    DrawLineForText(const FloatPoint& pt, float width)
        : m_point(pt), m_width(width) {}
// 4.2 Merge BEGIN <<
//Following code changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->drawLineForText(m_point, m_width);
//    }
//    virtual OperationType type() { return DrawLineForTextOperation; }
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->drawLineForText(m_point, m_width);
        return true;
    }
    TYPE(DrawLineForTextOperation)
//4.2 Merge END
private:
    FloatPoint m_point;
    float m_width;
};

class DrawLineForTextChecking : public Operation {
public:
    DrawLineForTextChecking(const FloatPoint& pt, float width,
                            GraphicsContext::TextCheckingLineStyle lineStyle)
        : m_point(pt), m_width(width), m_lineStyle(lineStyle) {}
// 4.2 Merge BEGIN <<
//Following code changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->drawLineForTextChecking(m_point, m_width, m_lineStyle);
//    }
//    virtual OperationType type() { return DrawLineForTextCheckingOperation; }
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->drawLineForTextChecking(m_point, m_width, m_lineStyle);
        return true;
    }
    TYPE(DrawLineForTextCheckingOperation)
//4.2 Merge END
private:
    FloatPoint m_point;
    float m_width;
    GraphicsContext::TextCheckingLineStyle m_lineStyle;
};

class DrawRect : public Operation {
public:
    DrawRect(const IntRect& rect) : m_rect(rect) {}
// 4.2 Merge BEGIN <<
//Following code changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->drawRect(m_rect);
//    }
//    virtual OperationType type() { return DrawRectOperation; }
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->drawRect(m_rect);
        return true;
    }
    TYPE(DrawRectOperation)
//4.2 Merge END
private:
    IntRect m_rect;
};

class FillPath : public Operation {
public:
    FillPath(const Path& pathToFill, WindRule fillRule)
        : m_path(pathToFill), m_fillRule(fillRule) {}
// 4.2 Merge BEGIN <<
//Following code changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->fillPath(m_path, m_fillRule);
//    }
//    virtual OperationType type() { return FillPathOperation; }
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->fillPath(m_path, m_fillRule);
        return true;
    }
    TYPE(FillPathOperation)
//4.2 Merge END
private:
    Path m_path;
    WindRule m_fillRule;
};

// 4.2 Merge BEGIN <<
//class FillRect : public Operation {
class FillRect : public PossiblyOpaqueOperation {
//4.2 Merge END
public:
    FillRect(const FloatRect& rect) : m_rect(rect), m_hasColor(false) {}
    void setColor(Color c) { m_color = c; m_hasColor = true; }
// 4.2 Merge BEGIN <<
    virtual bool applyImpl(PlatformGraphicsContext* context) { //virtual void apply(PlatformGraphicsContext* context) {
//4.2 Merge END
        if (m_hasColor)
             context->fillRect(m_rect, m_color);
        else
             context->fillRect(m_rect);
        return true; //4.2 merge
    }
// 4.2 Merge BEGIN <<
//    virtual OperationType type() { return FillRectOperation; } //Removed in 4.2
    virtual bool isOpaque() { return (m_hasColor && !m_color.hasAlpha())
            || (!m_hasColor && SkColorGetA(m_state->fillColor) == 0xFF); }
    TYPE(FillRectOperation)
//4.2 Merge END
private:
    FloatRect m_rect;
    Color m_color;
    bool m_hasColor;
};

class FillTransparentRect : public Operation{
public:
    FillTransparentRect(const FloatRect& rect,const Color& color)
        :m_rect(rect)
        ,m_color(color)
    {}
    virtual bool applyImpl(PlatformGraphicsContext*context){
        context->fillTransparentRect(m_rect,m_color);
        return true;
    }

    TYPE(FillTransparectRectOperation)

private:
    FloatRect m_rect;
    Color m_color;
};

class FillRoundedRect : public Operation {
public:
    FillRoundedRect(const IntRect& rect,
                    const IntSize& topLeft,
                    const IntSize& topRight,
                    const IntSize& bottomLeft,
                    const IntSize& bottomRight,
                    const Color& color)
        : m_rect(rect)
        , m_topLeft(topLeft)
        , m_topRight(topRight)
        , m_bottomLeft(bottomLeft)
        , m_bottomRight(bottomRight)
        , m_color(color)
    {}
// 4.2 Merge BEGIN <<
    virtual bool applyImpl(PlatformGraphicsContext* context) { //virtual void apply(PlatformGraphicsContext* context) {
        context->fillRoundedRect(m_rect, m_topLeft, m_topRight,
                                 m_bottomLeft, m_bottomRight,
                                 m_color);
        return true; //4.2 Merge
    }
//    virtual OperationType type() { return FillRoundedRectOperation; }
    TYPE(FillRoundedRectOperation)
//4.2 Merge END
private:
    IntRect m_rect;
    IntSize m_topLeft;
    IntSize m_topRight;
    IntSize m_bottomLeft;
    IntSize m_bottomRight;
    Color m_color;
};

class StrokeArc : public Operation {
public:
    StrokeArc(const IntRect& r, int startAngle, int angleSpan)
        : m_rect(r)
        , m_startAngle(startAngle)
        , m_angleSpan(angleSpan)
    {}
// 4.2 Merge BEGIN <<
////Following code changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->strokeArc(m_rect, m_startAngle, m_angleSpan);
//    }
//    virtual OperationType type() { return StrokeArcOperation; }
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->strokeArc(m_rect, m_startAngle, m_angleSpan);
        return true;
    }
    TYPE(StrokeArcOperation)
//4.2 Merge END
private:
    IntRect m_rect;
    int m_startAngle;
    int m_angleSpan;
};

class StrokePath : public Operation {
public:
    StrokePath(const Path& path) : m_path(path) {}
// 4.2 Merge BEGIN <<
////Following code changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->strokePath(m_path);
//    }
//    virtual OperationType type() { return StrokePathOperation; }
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->strokePath(m_path);
        return true;
    }
    TYPE(StrokePathOperation)
//4.2 Merge END
private:
    Path m_path;
};


class StrokeRect : public Operation {
public:
    StrokeRect(const FloatRect& rect, float lineWidth)
        : m_rect(rect), m_lineWidth(lineWidth) {}
// 4.2 Merge BEGIN <<
////Following code changed in 4.2
//    virtual void apply(PlatformGraphicsContext* context) {
//        context->strokeRect(m_rect, m_lineWidth);
//    }
//    virtual OperationType type() { return StrokeRectOperation; }
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->strokeRect(m_rect, m_lineWidth);
        return true;
    }
    TYPE(StrokeRectOperation)
//4.2 Merge END
private:
    FloatRect m_rect;
    float m_lineWidth;
};

// 4.2 Merge BEGIN <<
class DrawMediaButton : public Operation {
public:
    DrawMediaButton(const IntRect& rect, RenderSkinMediaButton::MediaButton buttonType,
                    bool translucent, bool drawBackground,
                    const IntRect& thumb)
        : m_rect(rect)
        , m_thumb(thumb)
        , m_buttonType(buttonType)
        , m_translucent(translucent)
        , m_drawBackground(drawBackground)
    {}
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->drawMediaButton(m_rect, m_buttonType, m_translucent, m_drawBackground, m_thumb);
        return true;
    }
    TYPE(DrawMediaButtonOperation)
private:
    IntRect m_rect;
    IntRect m_thumb;
    RenderSkinMediaButton::MediaButton m_buttonType;
    bool m_translucent : 1;
    bool m_drawBackground : 1;
};
//4.2 Merge END

//**************************************
// Text
//**************************************
// 4.2 Merge BEGIN <<
class DrawPosText : public Operation {
public:
    DrawPosText(const void* text, size_t byteLength,
                const SkPoint pos[], const SkPaint* paint)
        : m_text(text)
        , m_byteLength(byteLength)
        , m_pos(pos)
        , m_paint(paint)
    {}
    virtual bool applyImpl(PlatformGraphicsContext* context) {
        context->drawPosText(m_text, m_byteLength, m_pos, *m_paint);
        return true;
    }
    TYPE(DrawPosTextOperation)
private:
    const void* m_text;
    size_t m_byteLength;
    const SkPoint* m_pos;
    const SkPaint* m_paint;
};
//4.2 Merge END

// 4.2 Merge BEGIN <<
////Following code is removed in 4.2
//class DrawComplexText : public Operation {
//public:
//    DrawComplexText(SkPicture* picture) : m_picture(picture) {
//        SkSafeRef(m_picture);
//    }
//    ~DrawComplexText() { SkSafeUnref(m_picture); }
//    virtual void apply(PlatformGraphicsContext* context) {
//        if (!context->getCanvas())
//            return;
//        context->getCanvas()->drawPicture(*m_picture);
//    }
//    virtual OperationType type() { return DrawComplexTextOperation; }
//private:
//    SkPicture* m_picture;
//};

//class DrawText : public Operation {
//public:
//    DrawText(const Font* font, const SimpleFontData* simpleFont,
//             const GlyphBuffer& glyphBuffer,
//             int from, int numGlyphs, const FloatPoint& point)
//        : m_font(font), m_simpleFont(simpleFont)
//        , m_glyphBuffer(glyphBuffer), m_from(from)
//        , m_numGlyphs(numGlyphs), m_point(point) {

//        SkPicture* picture = new SkPicture();
//        SkCanvas* canvas = picture->beginRecording(0, 0, 0);
//        PlatformGraphicsContextSkia platformContext(canvas);
//        GraphicsContext graphicsContext(&platformContext);
//        m_font->drawGlyphs(&graphicsContext, m_simpleFont,
//                           m_glyphBuffer, m_from, m_numGlyphs, m_point);
//        picture->endRecording();
//        m_picture = picture;
//    }
//    ~DrawText() { SkSafeUnref(m_picture); }
//    virtual void apply(PlatformGraphicsContext* context) {
//        if (!context->getCanvas())
//            return;
//        context->getCanvas()->drawPicture(*m_picture);
//    }
//    virtual OperationType type() { return DrawTextOperation; }
//private:
//    SkPicture* m_picture;
//    const Font* m_font;
//    const SimpleFontData* m_simpleFont;
//    const GlyphBuffer m_glyphBuffer;
//    int m_from;
//    int m_numGlyphs;
//    const FloatPoint m_point;
//};

}

}
// 4.2 Merge BEGIN <<
//Removed in 4.2
//#endif // USE(ACCELERATED_COMPOSITING)
//4.2 Merge END

#endif // GraphicsOperation_h
