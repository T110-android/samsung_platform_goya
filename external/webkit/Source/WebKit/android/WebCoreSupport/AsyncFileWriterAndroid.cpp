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
#include "AsyncFileWriterAndroid.h"

#define LOG_NDEBUG 0
#define LOGTAG "FileSystem"

#if ENABLE(FILE_SYSTEM)

#include "AsyncFileWriterClient.h"
#include "Blob.h"
#include "WebFileWriter.h"
#include "FileSystem.h"
#include <wtf/text/CString.h>
#undef LOG
#include <utils/Log.h>
#include "BlobRegistry.h"
#include "BlobRegistryImpl.h"
#include "BlobStorageData.h"
#include "FileError.h"

using namespace WebCore;

namespace android {

static const char* databaseName = "FileSyetmQuota.db";
String AsyncFileWriterAndroid::s_basePath;
AsyncFileWriterAndroid::QuotaMap AsyncFileWriterAndroid::s_fileSystemQuota;


AsyncFileWriterAndroid::AsyncFileWriterAndroid(AsyncFileWriterClient* client, String path, unsigned quota,const String basepath, const String identifier)
    : m_client(client),m_path(path),m_quota(quota) 
{
   s_basePath = basepath;
   m_identifier = identifier;
   
   int length = s_basePath.length();
   String newstr = m_path.substring(length+1);
   int index = newstr.find("/");
   String newstr2 = newstr.substring(0,index);
   m_identifier = newstr2;
   
   if(!isHashmapInitialized())
   	{
   	  loadStorageQuotaInfo();
   	}
}

AsyncFileWriterAndroid::~AsyncFileWriterAndroid()
{
}


bool AsyncFileWriterAndroid::isHashmapInitialized()
{
   int sizemap = s_fileSystemQuota.size();
   if(s_fileSystemQuota.size() > 0 &&  !s_fileSystemQuota.isEmpty())
   	return true;
   else
   	return false;
}

void AsyncFileWriterAndroid::write(long long position, Blob* data)
{
    ASSERT(m_writer);
    String type = data->type();

    long long int len ;
    getFileSize(m_path,len);
    BlobRegistryImpl &blobRegistryimpl = static_cast<BlobRegistryImpl&>(blobRegistry());
    RefPtr<BlobStorageData> blobstoragedata = blobRegistryimpl.getBlobDataFromURL(data->url()).get();
    BlobDataItemList blobdataitemlist = blobstoragedata->items();
    BlobDataItem blobdataitem = blobdataitemlist[blobdataitemlist.size()-1];
    const char *str = blobdataitem.data->data();
    int length = blobdataitem.data->mutableData()->size();
    int quotaused = 0;
    quotaused = s_fileSystemQuota.get(m_identifier);
       
    PlatformFileHandle file=openFile(m_path,OpenForWriteOnly);
        if(length > m_quota){
            if(m_client)
	        m_client->didFail(FileError::QUOTA_EXCEEDED_ERR);
	        return;
        }	
        else if(length + quotaused > m_quota){
	    if(m_client)
	        m_client->didFail(FileError::QUOTA_EXCEEDED_ERR);
	        return;
        }
        else{
	    quotaused = s_fileSystemQuota.get(m_identifier);	    
	    if(position ==0){
	        if(length > len){
	            s_fileSystemQuota.set(m_identifier,length);
	       	}
	    }
	    else{
	        int data =  length+quotaused;
	        s_fileSystemQuota.set(m_identifier,data);
	        int dataset = s_fileSystemQuota.get(m_identifier);
            }
	    if(isHandleValid(file)){
                position = WebCore::seekFile(file,position,SeekFromBeginning);    
                int res = WebCore::writeToFile(file,str,length);
	        m_client->didWrite(length, true);
	   }
	}
	closeFile(file);
    }    



void AsyncFileWriterAndroid::clearAll()
{
    s_fileSystemQuota.clear();
}

void AsyncFileWriterAndroid::truncate(long long length)
{
    //ASSERT(m_writer);
    //m_writer->truncate(length);
}

void AsyncFileWriterAndroid::abort()
{
    //ASSERT(m_writer);
    //m_writer->cancel();
}

void AsyncFileWriterAndroid::removeSize(long long int length, const String identifier)
{
    if(identifier != NULL){
    bool isPresent = s_fileSystemQuota.contains(identifier);
    if(isPresent){
        int quotaused = s_fileSystemQuota.get(identifier);
        int newquota = quotaused-length;
        s_fileSystemQuota.set(identifier,newquota);
    }
    	}
}

bool AsyncFileWriterAndroid::OpenDatabase(SQLiteDatabase* database)
{ 
    ASSERT(database);
    if(s_basePath.isNull())
        return false;
    String filename = SQLiteFileSystem::appendDatabaseFileNameToPath(s_basePath, databaseName);
    if (!database->open(filename))
        return false;
    if (chmod(filename.utf8().data(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) {
        database->close();
        return false;
    }
    return true;
}


void AsyncFileWriterAndroid::loadStorageQuotaInfo()
{
    SQLiteDatabase database;
    if (!OpenDatabase(&database))
        return;

    // Create the table here, such that even if we've just created the DB, the
    // commands below should succeed.
    if (!database.executeCommand("CREATE TABLE IF NOT EXISTS FileSystemQuota (origin TEXT UNIQUE NOT NULL, quota LONG NOT NULL)")) { 
	database.close();
        return;
    }

    SQLiteStatement statement(database, "SELECT * FROM FileSystemQuota");
    if (statement.prepare() != SQLResultOk) {
        database.close();
        return;
    }

    ASSERT(s_fileSystemQuota.size() == 0);
    while (statement.step() == SQLResultRow)
        s_fileSystemQuota.set(statement.getColumnText(0), statement.getColumnInt64(1));

    database.close();

    //m_exception = 1;
}

void AsyncFileWriterAndroid::storeStorageQuotaInfo()
{
    SQLiteDatabase database;
    if (!OpenDatabase(&database))
        return;

    SQLiteTransaction transaction(database);

    // The number of entries should be small enough that it's not worth trying
    // to perform a diff. Simply clear the table and repopulate it.
    if (!database.executeCommand("DELETE FROM FileSystemQuota")) {
        database.close();
        return;
    }

    QuotaMap::const_iterator end = s_fileSystemQuota.end();
    for (QuotaMap::const_iterator iter = s_fileSystemQuota.begin(); iter != end; ++iter) {
         SQLiteStatement statement(database, "INSERT INTO FileSystemQuota (origin, quota) VALUES (?, ?)");
         if (statement.prepare() != SQLResultOk)
             continue;
         statement.bindText(1, iter->first);
         statement.bindInt64(2, iter->second);
         statement.executeCommand();
    }

    transaction.commit();
    database.close();


}


} // namespace

#endif // ENABLE(FILE_SYSTEM)
