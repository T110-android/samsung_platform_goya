/*
 * Copyright 2009, The Android Open Source Project
 * Copyright (C) 2006 Apple Computer, Inc.
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

// This file is part of the internal font implementation.  It should not be included by anyone other than
// FontMac.cpp, FontWin.cpp and Font.cpp.

#ifndef FontPlatformData_h
#define FontPlatformData_h

#include "FontOrientation.h"
#include "TextOrientation.h"
#include <wtf/text/StringImpl.h>
// 4.2 Merge BEGIN <<
#include "SkLanguage.h"
#include "SkPaint.h"
// 4.2 Merge END >>

#ifndef NDEBUG
#include "PlatformString.h"
#endif

//class SkPaint; //// 4.2 Merge
class SkTypeface;

struct HB_FaceRec_;

namespace WebCore {

class FontPlatformData {
public:
    static FontPlatformData Deleted() {
        return FontPlatformData(NULL, -1, false, false);
    }

    FontPlatformData();
    FontPlatformData(const FontPlatformData&);
    FontPlatformData(SkTypeface*, float textSize, bool fakeBold, bool fakeItalic,
                     FontOrientation = Horizontal, TextOrientation = TextOrientationVerticalRight);
    FontPlatformData(const FontPlatformData& src, float textSize);
    FontPlatformData(float size, bool syntheticBold, bool syntheticOblique);
    FontPlatformData(const FontPlatformData& src, SkTypeface* typeface);

    ~FontPlatformData();

    FontPlatformData(WTF::HashTableDeletedValueType)
        : m_typeface(hashTableDeletedFontValue()) { } // 4.2 Merge : mTypeface changed to m_typeface
    bool isHashTableDeletedValue() const {
        return m_typeface == hashTableDeletedFontValue();// 4.2 Merge : mTypeface changed to m_typeface
    }

    FontOrientation orientation() const { return m_orientation; }// 4.2 Merge : mOrientation changed to m_orientation
    void setOrientation(FontOrientation orientation) { m_orientation = orientation; }// 4.2 Merge : mOrientation changed to m_orientation
    FontPlatformData& operator=(const FontPlatformData&);
    bool operator==(const FontPlatformData& a) const;

    void     setupPaint(SkPaint*) const;

    // -------------------------------------------------------------------------
    // Return Skia's unique id for this font. This encodes both the style and
    // the font's file name so refers to a single face.
    // -------------------------------------------------------------------------
    uint32_t uniqueID() const;

    float size() const { return m_textSize; }// 4.2 Merge : mTextSize changed to m_textSize
    unsigned hash() const;
    int emSizeInFontUnits() const;
    bool isFixedPitch() const;

#ifndef NDEBUG
    String description() const { return ""; }
#endif

    HB_FaceRec_* harfbuzzFace() const;
    SkTypeface* typeface() const { return m_typeface; }// 4.2 Merge : mTypeface changed to m_typeface
    void  setFakeItalic(bool val)  { m_fakeItalic = val;}// 4.2 Merge : mFakeItalic changed to m_fakeItalic

    bool isFakeBold() const { return m_fakeBold; }// 4.2 Merge : mFakeBold changed to m_fakeBold
    bool isFakeItalic() const { return m_fakeItalic; }// 4.2 Merge : mFakeItalic changed to m_fakeItalic

    static void setDefaultLanguage(const char* language);//4.2 Merge : New function

private:
    class RefCountedHarfbuzzFace : public RefCounted<RefCountedHarfbuzzFace> {
    public:
        static PassRefPtr<RefCountedHarfbuzzFace> create(HB_FaceRec_* harfbuzzFace)
        {
            return adoptRef(new RefCountedHarfbuzzFace(harfbuzzFace));
        }

        ~RefCountedHarfbuzzFace();

        HB_FaceRec_* face() const { return m_harfbuzzFace; }

    private:
        RefCountedHarfbuzzFace(HB_FaceRec_* harfbuzzFace) : m_harfbuzzFace(harfbuzzFace)
        {
        }

        HB_FaceRec_* m_harfbuzzFace;
    };

// 4.2 Merge BEGIN <<
    SkTypeface* m_typeface;
    float       m_textSize;
    mutable int m_emSizeInFontUnits;
    bool        m_fakeBold;
    bool        m_fakeItalic;
    FontOrientation m_orientation;
    TextOrientation m_textOrientation;
// 4.2 Merge END >>
    mutable RefPtr<RefCountedHarfbuzzFace> m_harfbuzzFace;
    static SkLanguage s_defaultLanguage;//4.2 Merge : new

    static SkTypeface* hashTableDeletedFontValue() {
        return reinterpret_cast<SkTypeface*>(-1);
    }
};

} /* namespace */

#endif
