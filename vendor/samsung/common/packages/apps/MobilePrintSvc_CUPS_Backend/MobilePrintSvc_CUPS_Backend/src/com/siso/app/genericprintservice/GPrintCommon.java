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

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.media.ExifInterface;
import android.util.Log;

public class GPrintCommon {
	public static final String TAG = "GenericPrintService";
	public static final String PRINTER_DEV_USB_FILE = "/dev/usb/lp0";
	
	public static final String pxlcolor_device = "pxlcolor";
	public static final String pxlmono_device = "pxlmono";
	public static final String ps2write_device = "ps2write";
	public static final String pdfwrite_device = "pdfwrite";
	public static final int psDone_monoPrint = 25;
	public static final int psToPsStart_monoPrint = 38; // calling psToPs
	public static final int psDone_psPrint = 50;
	public static final int psDone_pclPrint = 33;
	public static final int pclDone_pclPrint = 66;
	public static final int pclDone_gspclPrint = 50;
	public static final int pclFail_pclPrint = 49;
	public static final int job_done = 100;
	public static final int image_print_mode = 0;
	public static final int pdf_print_mode = 1;
	public static final int print_type_ps = 0;
    public static final int print_type_pcl = 1;
    
    public static final String color = "color";
    public static final String mono = "mono";
    
    public static final String unknown = "unknown";
    public static final String auto = "auto";
    public static final String portrait = "portrait";
    public static final String landscape = "landscape";
    
    public static final String shortedge = "shortedge";
    public static final String longedge = "longedge";
    public static final String simplex = "simplex";
    public static final String wifi = "wifi";
    public static final String usb = "usb";
    
    public static final String starting = "starting";
    public static final String totalpages = "totalpages";
    public static final String currentpage = "currentpage";
    public static final String printingpage = "printingpage";
    public static final String finishedpage = "finishedpage";
    public static final String finishedjob = "finishedjob";
    public static final String canceled = "canceled";
    public static final String error = "error";
    public static final String fileNotSupported = "filenotsupported";
    public static final String printerNotSupported = "printerNotSupported";
    public static final String cannotReadFile = "cannotreadfile";
    public static final String outOfMemoryException = "outofmemoryexception";
    public static final String printerNotAvailable = "printernotavailable";
    
	public static boolean isUSBConnected(){
		File devUSB = new File(PRINTER_DEV_USB_FILE);
		Boolean present = devUSB.exists();
		Log.d(GPrintCommon.TAG,"GPrintCommon : isUSBConnected : present :"+present);
		return present;
	}
	
	@SuppressLint("NewApi")
	public static boolean isUSBPrinterConnected(Context mAppContext){
		try{
			UsbManager mUsbManager = (UsbManager) mAppContext.getSystemService(Context.USB_SERVICE);
			HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
            Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
            //Log.d(TAG, "GPrintUsbManager : SearchPrinterThread : got list" );
            while(deviceIterator.hasNext()){
            	UsbDevice device = deviceIterator.next();
                //Log.d(TAG, "GPrintUsbManager : SearchPrinterThread : " + device);

                for(int i = 0; i < device.getInterfaceCount(); i++) {
                	 UsbInterface intf = device.getInterface(i);
                	if(intf.getInterfaceClass() == UsbConstants.USB_CLASS_PRINTER) {
                		Log.d(GPrintCommon.TAG,"GPrintCommon : isUSBPrinterConnected -- yes");
                		return true;
                	}
                }

            }
            Log.d(GPrintCommon.TAG,"GPrintCommon : isUSBPrinterConnected : No usb printer found");
			
		}catch(Exception ex){
			Log.e(GPrintCommon.TAG,"GPrintCommon : isUSBPrinterConnected : error :"+ex.toString());
			ex.printStackTrace();
		}
		return false;
	}
	
	public static String getImageOrientation(String filepath) {
		if(filepath == null) {
			Log.e(TAG, "GPrintCommon : getImageOrientation() : filepath is null!");
			return unknown;
		}

		int imageWidth, imageHeight;

		ExifInterface exif = null;
		try {
			exif = new ExifInterface(filepath);
		} catch (IOException e) {
			e.printStackTrace();
		}

		if(exif != null && exif.getAttributeInt(ExifInterface.TAG_IMAGE_WIDTH, 0) > 0) {
			imageWidth = exif.getAttributeInt(ExifInterface.TAG_IMAGE_WIDTH, 1);
			imageHeight = exif.getAttributeInt(ExifInterface.TAG_IMAGE_LENGTH, 1);
		}
		else {
			BitmapFactory.Options options = new BitmapFactory.Options();
			options.inSampleSize = 4;
			Bitmap image = BitmapFactory.decodeFile(filepath, options);
			if(image == null) {
				Log.e(TAG, "GPrintCommon : getImageOrientation() : image is invalid. " + filepath);
				return unknown;
			}

			imageWidth = image.getWidth();
			imageHeight = image.getHeight();
		}

		if(imageWidth > imageHeight) {
			return landscape;
		}

		return portrait;
	}
}
