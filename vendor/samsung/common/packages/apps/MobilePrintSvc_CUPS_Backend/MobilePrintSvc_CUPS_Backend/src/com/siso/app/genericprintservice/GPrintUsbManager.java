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

import com.siso.app.print.ipc.GPrintUsbResponse;

import java.util.ArrayList;
import java.util.StringTokenizer;
import android.util.Log;

public class GPrintUsbManager {
	public String mMakeModel=null;
	public String mDeviceIP=null;
//	public String mLanguages=null;
	public String mDeviceID = null;
//	private int mColorModel=-1;
//	private int mDuplexMode=-1;
//	private GPrintModelAnalyser mdlAnalyser = null;
	
	static {
		try {

			System.loadLibrary("cups");
			System.loadLibrary("invusb");
			System.loadLibrary("cupsusb");
			System.loadLibrary("cupsdevfileusb");
			
		} catch (Exception ex) {
			Log.i("GPrintUsbManager",
					"Cannot load library" + ex.toString());
		}
	}
	
	private native String[] GetUsbDeviceList();
	private native String[] GetDevFileUsbDevice();
	
	public GPrintUsbManager(){
//		mdlAnalyser = new GPrintModelAnalyser();
	}
	
	   public boolean getDevice(){
	        if(GPrintCommon.isUSBConnected()){
	            //Device File present
	            try{
	                String dev[] = GetDevFileUsbDevice();
	                int dev_length = dev.length;
	                Log.i(GPrintCommon.TAG,"GPrintUsbManager : getDevice :usb printer:dev_sixe="+dev_length);
	                //for(int count=0;count<dev_length;){
//	                  Log.i(GPrintCommon.TAG,"GPrintUsbManager : getDevice  : device file : usb search 1: "+dev[0]); //Ip address
//	                  Log.i(GPrintCommon.TAG,"GPrintUsbManager : getDevice  : device file :usb search 2: "+dev[1]); //Make Model
//	                  Log.i(GPrintCommon.TAG,"GPrintUsbManager : getDevice  : device file :usb search 3: "+dev[2]); //Device ID info
	                    
	                    mDeviceIP = dev[0];
	                    mMakeModel = dev[1];
	                    mDeviceID = dev[2];
	                    if(mDeviceIP != null){
	                        if(mMakeModel != null){         
	                            if(mDeviceID != null){
	                                
	                                return true;
	                                
	                            }
	                        }
	                    }
	                    
	            }catch(Exception ex){
	                Log.e(GPrintCommon.TAG, "GPrintUsbManager : getDevice : Cannot search USB devices:"+ex.toString());
	            }
	        }else{
	            
	            try{
	                String dev[] = GetUsbDeviceList();
	                int dev_length = dev.length;
	                Log.i(GPrintCommon.TAG,"GPrintUsbManager : getDevice :usb printer:dev_sixe="+dev_length);
	                    
	                    mDeviceIP = dev[0];
	                    mMakeModel = dev[1];
	                    mDeviceID = dev[2];
	                    if(mDeviceIP != null){
	                        if(mMakeModel != null){         
	                            if(mDeviceID != null){
	                            
	                                return true;
	                                
	                            }
	                        }
	                    }
	                    
	            }catch(Exception ex){
	                Log.e(GPrintCommon.TAG, "GPrintUsbManager : getDevice : Cannot search USB devices:"+ex.toString());
	            }
	        }
	        
	        return false;
	    }

}


