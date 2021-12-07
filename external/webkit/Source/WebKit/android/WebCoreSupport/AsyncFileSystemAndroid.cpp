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


#include "config.h"
#include "AsyncFileSystemAndroid.h"

#define LOG_NDEBUG 0
#define LOGTAG "FileSystem"

#if ENABLE(FILE_SYSTEM)

#include "AsyncFileSystemCallbacks.h"
#include "WebFileInfo.h"
#include "WebFileWriter.h"
#include "WebKit.h"
#include "File.h"
#include "FileSystem.h"
#include "Logging.h"
#include "FileError.h"
#include "FileMetadata.h"
#include <wtf/text/CString.h>
#undef LOG
#include <utils/Log.h>

namespace WebCore{

bool AsyncFileSystem::isAvailable()
{
    return true;
}

}

using namespace WebCore;

namespace android {

//static const char* databaseName = "FileSystemQuota.db";

unsigned AsyncFileSystemAndroid::s_quota;
String AsyncFileSystemAndroid::s_basepath;
String AsyncFileSystemAndroid::s_identifier;
AsyncFileWriterAndroid* AsyncFileSystemAndroid::s_asyncFileWriterAndroid;


AsyncFileSystemAndroid::AsyncFileSystemAndroid(AsyncFileSystem::Type type, const String& rootPath)
    : AsyncFileSystem(type, rootPath)
{
    isDirectory = false;
}

AsyncFileSystemAndroid::~AsyncFileSystemAndroid()
{
}

void AsyncFileSystemAndroid::openFileSystem(const String& basePath, const String& storageIdentifier, Type type, bool, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    s_basepath = basePath;
    s_identifier = storageIdentifier;
    String typeString = (type == Persistent) ? "Persistent" : "Temporary";

    String name = storageIdentifier;
    name += ":";
    name += typeString;

    String rootPath = basePath;
    rootPath.append(PlatformFilePathSeparator);
    rootPath += storageIdentifier;
    rootPath.append(PlatformFilePathSeparator);
    rootPath += typeString;
    if(makeAllDirectories(rootPath)){
       rootPath.append(PlatformFilePathSeparator);
       callbacks->didOpenFileSystem(name, AsyncFileSystemAndroid::create(type, rootPath));
    }
}

void AsyncFileSystemAndroid::move(const String& sourcePath, const String& destinationPath, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    
}

void AsyncFileSystemAndroid::copy(const String& sourcePath, const String& destinationPath, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    
}

void AsyncFileSystemAndroid::remove(const String& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
//    LOGV("AsyncFileSystemAndroid::remove path value is %s",path.utf8().data());
    long long int len = 0 ;
    getFileSize(path,len);
    int res = deleteFile(path);    
    if(res){
        callbacks->didSucceed();
        if(len > 0){
	   if(s_asyncFileWriterAndroid)
	       s_asyncFileWriterAndroid->removeSize(len, s_identifier);
	   else{
	       PassOwnPtr<AsyncFileWriterAndroid> asyncFileWriterAndroid = adoptPtr(new AsyncFileWriterAndroid(NULL,path,s_quota,s_basepath, s_identifier));    
	       AsyncFileWriterAndroid* asyncWriter = asyncFileWriterAndroid.get();
	       asyncWriter->removeSize(len, s_identifier);
	   }
        }
    }
    else
        callbacks->didFail(res);
           
}

void AsyncFileSystemAndroid::removeRecursively(const String& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
//    LOGV("AsyncFileSystemAndroid::removeRecursively path value is %s",path.utf8().data());

    long long int len = 0 ;
    Vector<String> list = listDirectory(path,"*");
    unsigned size = list.size();
    if(size == 0)
        deleteDirectory(path);
    else{
        for(unsigned i=0;i<size;i++){
            String dirName = list[i];
	    getFileSize(dirName,len);
	    int res = deleteFile(dirName);
            if(res){
                if(len > 0){
	            if(s_asyncFileWriterAndroid)
	                s_asyncFileWriterAndroid->removeSize(len, s_identifier);
	            else{
	                PassOwnPtr<AsyncFileWriterAndroid> asyncFileWriterAndroid = adoptPtr(new AsyncFileWriterAndroid(NULL,path,s_quota,s_basepath, s_identifier));    
	                AsyncFileWriterAndroid* asyncWriter = asyncFileWriterAndroid.get();
	                asyncWriter->removeSize(len, s_identifier);
	            }
               }
	       deleteDirectory(path);
	       callbacks->didSucceed();
           }
	   else
	       callbacks->didFail(res);
	    
        }
    }
}

void AsyncFileSystemAndroid::readMetadata(const String& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    FileMetadata metadata;
    long long size;
    time_t modtime;
    if(getFileSize(path,size)){
       if(getFileModificationTime(path,modtime)){
          metadata.modificationTime = modtime;
          metadata.length = size;
          callbacks->didReadMetadata(metadata);
       }
    }
}

void AsyncFileSystemAndroid::createFile(const String& path, bool exclusive, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
//    LOGV("AsyncFileSystemAndroid:: createFile Path value is %s",path.utf8().data());
      
    String dirName = directoryName(path);
    bool res = makeAllDirectories(dirName);
    PlatformFileHandle file= NULL;
    if(WebCore::fileExists(path)){
	callbacks->didSucceed();
    }
    else{
        file=openFile(path,OpenForWrite);
        if(isHandleValid(file)){
	    callbacks->didSucceed();
        }
        else
        {
           callbacks->didFail(FileError::NOT_FOUND_ERR);
        }
     }
    closeFile(file);
}

void AsyncFileSystemAndroid::createDirectory(const String& path, bool exclusive, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
//     LOGV("AsyncFileSystemAndroid:: createDirectory Path value is %s",path.utf8().data());
     if(makeAllDirectories(path)){
       callbacks->didSucceed();
      }
     else
     {
       callbacks->didFail(FileError::NOT_FOUND_ERR);
     }
}

void AsyncFileSystemAndroid::fileExists(const String& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    if(WebCore::fileExists(path)){
        callbacks->didSucceed();
    }
    else{
	callbacks->didFail(FileError::NOT_FOUND_ERR);
    }
    
}

void AsyncFileSystemAndroid::directoryExists(const String& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    
}

void AsyncFileSystemAndroid::readDirectory(const String& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    Vector<String> list = listDirectory(path,"*");
    unsigned size = list.size();
 
    for(unsigned i=0;i<size;i++){
     String dirName = list[i];
     int index = dirName.reverseFind("///");
     String dirExt = dirName.substring(index + 1);
     int index1 = dirExt.reverseFind('.');
     if(index1 == -1)
        isDirectory = true;
     else
        isDirectory = false;
     callbacks->didReadDirectoryEntry(dirName,isDirectory);
    }
    callbacks->didReadDirectoryEntries(false);
      
}

void AsyncFileSystemAndroid::createWriter(AsyncFileWriterClient* client, const String& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    long long int len ;
    int exception = 0;
    getFileSize(path,len);
    PassOwnPtr<AsyncFileWriterAndroid> asyncFileWriterAndroid = adoptPtr(new AsyncFileWriterAndroid(client,path,s_quota,s_basepath, s_identifier));
    s_asyncFileWriterAndroid = asyncFileWriterAndroid.get();
           callbacks->didCreateFileWriter(asyncFileWriterAndroid,len);
    
}

void AsyncFileSystemAndroid::fileSystemStorage(unsigned quota)
{
    s_quota = quota;
}

} // namespace WebCore

#endif
