/*
 * Copyright 2009, The Android Open Source Project
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


#ifndef AsyncFileWriterAndroid_h
#define AsyncFileWriterAndroid_h

#if ENABLE(FILE_SYSTEM)

#include "AsyncFileWriter.h"
#include "AsyncFileSystemCallbacks.h"
#include "WebFileError.h"
#include "WebFileWriterClient.h"
#include <wtf/PassOwnPtr.h>
#include <wtf/HashMap.h>

#include "SQLiteDatabase.h"
#include "SQLiteFileSystem.h"
#include "SQLiteStatement.h"
#include "SQLiteTransaction.h"

namespace WebCore {
class AsyncFileWriterClient;
}

using namespace WebCore;

namespace android {

//class Blob;
//class AsyncFileWriterClient;

class AsyncFileWriterAndroid : public AsyncFileWriter {
public:
    AsyncFileWriterAndroid(AsyncFileWriterClient* client, String path, unsigned quota, const String basepath, const String identifier);
    ~AsyncFileWriterAndroid();
    
    // FileWriter
    virtual void write(long long position, Blob* data);
    virtual void truncate(long long length);
    virtual void abort();
	

    static bool OpenDatabase(SQLiteDatabase* database);
    static void loadStorageQuotaInfo();
    static void storeStorageQuotaInfo();
    void removeSize(long long int, const String );
    static void clearAll();
    static bool isHashmapInitialized();
    
private:
    AsyncFileWriterClient* m_client;
    String m_path;
    unsigned m_quota;
    static String s_basePath;
    String m_identifier;
    typedef WTF::HashMap<WTF::String,int> QuotaMap;
    static QuotaMap s_fileSystemQuota;
    static bool isFileNew;
    	
};

} // namespace

#endif // ENABLE(FILE_SYSTEM)

#endif // AsyncFileWriterAndroid_h
