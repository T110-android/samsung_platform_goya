 /*

 * Copyright (c)  2010-2013 Samsung Electronics Co., Ltd. All rights reserved.

 *

 * Mobile Communication Division,

 * Digital Media & Communications Business, Samsung Electronics Co., Ltd.

 *

 * This software and its documentation are confidential and proprietary

 * information of Samsung Electronics Co., Ltd.  No part of the software and

 * documents may be copied, reproduced, transmitted, translated, or reduced to

 * any electronic medium or machine-readable form without the prior written

 * consent of Samsung Electronics.

 *

 * Samsung Electronics makes no representations with respect to the contents,

 * and assumes no responsibility for any errors that might appear in the

 * software and documents. This publication and the contents hereof are subject

 * to change without notice.

 *



 * ========================================================================= 

 * Alternatively, the contents of this file may be used under the terms of

 * either the GNU General Public License Version 2.

 * 

 *

 * Copyright (c)  2010-2013 Samsung Electronics Co., Ltd. All rights reserved.

 *

 * This software is licensed under the terms of the GNU General Public

 * License version 2, as published by the Free Software Foundation, and

 * may be copied, distributed, and modified under those terms.

 *

 * This program is distributed in the hope that it will be useful,

 * but WITHOUT ANY WARRANTY; without even the implied warranty of

 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

 * GNU General Public License for more details.

 */

package com.siso.app.genericprintservice;


import com.siso.app.print.ipc.GPrintJob;
import com.siso.app.print.ipc.GPrintJobStatus;
import com.siso.app.print.ipc.GPrintUsbPrinter;
import com.siso.app.print.ipc.GPrintUsbResponse;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;

public class GPrintBackendService extends Service{
    private String BIND_GENERIC_PRINT_SERVICE = "com.siso.intent.action.GENERIC_PRINT_BACKEND_SERVICE";
    private static GPrintExtandPrintBackend mIPrintService;
    private Context mCtx;
    private Thread mPrintThread = null;
    private GPrintJobStatus mPrintJObStatus;
    @Override
    public synchronized void onCreate() {
        super.onCreate();
        mIPrintService = new GPrintExtandPrintBackend(this);
        mCtx = getBaseContext(); 
    }
    
    @Override
    public IBinder onBind(Intent intent) {
        // TODO Auto-generated method stub
     // TODO Auto-generated method stub
        if(BIND_GENERIC_PRINT_SERVICE.equals(intent.getAction())) {
            Log.i(GPrintCommon.TAG,"The GPrintBackendService was binded.");
            return mIPrintService;
        }
                
            return null;
    }

    public int cancel() {
        // TODO Auto-generated method stub
        return 0;
    }

    public void searchUsbPrinter(GPrintUsbResponse callback) {
        // TODO Auto-generated method stub
        GPrintUsbManager usbMgr = new GPrintUsbManager();
        if(usbMgr.getDevice()){
            GPrintUsbPrinter uPtr = new GPrintUsbPrinter();
            uPtr.setDeviceId(usbMgr.mDeviceID);
            uPtr.setMakeModel(usbMgr.mMakeModel);
            uPtr.setDeviceIp(usbMgr.mDeviceIP);
            try {
                Log.e(GPrintCommon.TAG,"GPrintBackendService : searchUsbPrinter : sending callback");
                callback.responseCapabilities(uPtr);
            } catch (RemoteException e) {
                // TODO Auto-generated catch block
                Log.e(GPrintCommon.TAG,"GPrintBackendService : searchUsbPrinter : cannot send callback");
                e.printStackTrace();
            }
        }else{
            Log.e(GPrintCommon.TAG,"GPrintBackendService : searchUsbPrinter : printer not found");
        }
    }
    
//   public void searchUsbPrinter(usbPrinterCallback callback) {
//        // TODO Auto-generated method stub
//        GPrintUsbManager usbMgr = new GPrintUsbManager();
//        usbMgr.getDevice(callback);
//    }

    public int print(int jobId, GPrintJob job, GPrintJobStatus response) {
        // TODO Auto-generated method stub
        mPrintJObStatus = response;
        
        mPrintThread = new PrintThread(jobId, job);
        mPrintThread.start();
        
        return 0;
    }

    private class PrintThread extends Thread {
       private int TransactionID;
       private GPrintJob setting;
        PrintThread(int id,GPrintJob set){
            TransactionID = id;
            setting = set;
        }
        
        @Override
        public void run() {
            GPrintPrintingManager printMgr = new GPrintPrintingManager(mCtx, new printManagerCallback());
            printMgr.startPrint(TransactionID, setting);
        }
        
        
    }
    
    private class printManagerCallback implements GPrintPrintingManager.Callback{
        public void sendResponse(int job_id, String result, String progress) {
            // TODO Auto-generated method stub
            Log.d(GPrintCommon.TAG, "GPrintBackendService : printManagerCallback : sendResponse : statusChanged :job-id="+job_id+" result = "+result+" progress="+progress);
            try {
                mPrintJObStatus.responsePrint(job_id,result,progress);
            } catch (RemoteException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }
 }
    
