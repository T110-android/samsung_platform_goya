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


// 4.2 Merge BEGIN <<
#include "config.h"
#include "DumpLayer.h"

#if USE(ACCELERATED_COMPOSITING)

#define WRITE_VAL(format, ...) (snprintf(m_valueBuffer, BUF_SIZE, format, __VA_ARGS__), writeEntry(label, m_valueBuffer))

namespace WebCore {




void LayerDumper::writeIntVal(const char* label, int value)
{
    WRITE_VAL("%d", value);
}

void LayerDumper::writeHexVal(const char* label, int value)
{
    WRITE_VAL("%x", value);
}

void LayerDumper::writeFloatVal(const char* label, float value)
{
    WRITE_VAL("%.3f", value);
}

void LayerDumper::writePoint(const char* label, SkPoint point)
{
    WRITE_VAL("{ x = %.3f; y = %.3f; }", point.fX, point.fY);
}

void LayerDumper::writeIntPoint(const char* label, IntPoint point)
{
    WRITE_VAL("{ x = %d; y = %d; }", point.x(), point.y());
}

void LayerDumper::writeSize(const char* label, SkSize size)
{
    WRITE_VAL("{ w = %.3f; h = %.3f; }", size.width(), size.height());
}

void LayerDumper::writeRect(const char* label, SkRect rect)
{
    WRITE_VAL("{ x = %.3f; y = %.3f; w = %.3f; h = %.3f; }",
            rect.fLeft, rect.fTop, rect.width(), rect.height());
}

void LayerDumper::writeMatrix(const char* label, const TransformationMatrix& matrix)
{
    WRITE_VAL("{ (%.2f,%.2f,%.2f,%.2f),(%.2f,%.2f,%.2f,%.2f),"
            "(%.2f,%.2f,%.2f,%.2f),(%.2f,%.2f,%.2f,%.2f) }",
            matrix.m11(), matrix.m12(), matrix.m13(), matrix.m14(),
            matrix.m21(), matrix.m22(), matrix.m23(), matrix.m24(),
            matrix.m31(), matrix.m32(), matrix.m33(), matrix.m34(),
            matrix.m41(), matrix.m42(), matrix.m43(), matrix.m44());
}

void LayerDumper::writeLength(const char* label, SkLength value)
{
    if (value.defined())
        WRITE_VAL("{ type = %d; value = %.2f; }", value.type, value.value);
    else
        writeEntry(label, "<undefined>");
}

void FileLayerDumper::beginLayer(const char* className, const LayerAndroid* layerPtr)
{
    LayerDumper::beginLayer(className, layerPtr);
    writeLine("{");
    writeHexVal("layer", (int)layerPtr);
}

void FileLayerDumper::endLayer()
{
    writeLine("}");
    LayerDumper::endLayer();
}

void FileLayerDumper::writeEntry(const char* label, const char* value)
{
    fprintf(m_file, "%*s%s = %s\n", (m_indentLevel + 1) * 2, " ", label, value);
}

void FileLayerDumper::writeLine(const char* str)
{
    if (m_indentLevel)
        fprintf(m_file, "%*s", m_indentLevel * 2, " ");
    fprintf(m_file, "%s\n", str);
}

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)
// 4.2 Merge END >>
