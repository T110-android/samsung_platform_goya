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

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.AssetManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.NetworkInfo.DetailedState;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.p2p.WifiP2pManager;
import android.os.Environment;
import android.util.Log;


public class GPrintPrintingManager {

    private Callback mCallback = null;

    private Context mContext = null;

    private static int mJobId;

    private static int mCurrentProgress = 0;

    private static long mPrintFileSize = 0;

    private GPrintJob mPrintJob = null;

    private String psFilePath;

    private String psMonoFilePath;

    private String psPreProcessFilePath; // GS - pdf to ps

    private String pclFilePath;
    
    private String mPrintableFilePath;
    
    private ArrayList<String> mFilePathList;

    private String mPrinterName;

    private String mExternalStorageDirPath;

    private String mPrintDirectoryPath;

    private String mPPDFilePath;

    private String mJobTitle;

    private String mCurrentPrintFileName;

    private boolean mError = false;

    private String genPSmono = "Generic-PostScript";//private String genPSmono = "Generic-PostScript-mono";

    private String genPScolor = "Generic-PostScript";

    private String genPCLmono = "Generic-PCL_6_PCL_XL_Printer-pxlmono";

    private String genPCLcolor = "Generic-PCL_6_PCL_XL_Printer-pxlcolor";

    private GPrintPsToPcl mGhostMgr = null;

    private List<String> printCommand = null;

    private int mFileCount;

    private int mPrintType; // 0: PS , 1 PCL XL

    private boolean useGSForPdf = true; // :false - poppler will be used

    private boolean mPcl6Supported = false;

    static {
        try {
            System.loadLibrary("cupspng");
            System.loadLibrary("cups");
            System.loadLibrary("cupsimage");
            System.loadLibrary("cupsdevfileusb");
            System.loadLibrary("invusb");
            System.loadLibrary("cupsusb");
            System.loadLibrary("cupsipp");
            System.loadLibrary("cupssocket");
            System.loadLibrary("cupsimgtops");
            System.loadLibrary("invCupsjni");

        } catch (Exception ex) {
            Log.i("GPrintPrintingManager", "Cannot load library" + ex.toString());
        }
    }

    // private native int cancelJob(String[] lpCommand);
    private native int InvokeImageToPs(String[] lpCommand);
    
    private native String[] GetUsbDeviceList();

    private native String[] GetDevFileUsbDevice();

    private native int InvokeUsbBackend(String[] lpCommand);

    private native int InvokeDevFileUsbBackend(String[] lpCommand);

    private native int InvokeIppBackend(String[] lpCommand);

    private native int InvokeSocketBackend(String[] lpCommand);

    public static interface Callback {
        public void sendResponse(int job_id, String result, String progress);
    }

    public GPrintPrintingManager(Context mAppContext, Callback printManagerCallback) {
        // TODO Auto-generated constructor stub
        mContext = mAppContext;
        mCallback = printManagerCallback;
        mExternalStorageDirPath = Environment.getExternalStorageDirectory().getAbsolutePath();
        mPrintDirectoryPath = mExternalStorageDirPath + File.separator + ".GenericPrintDir";
        if (createTemDir()) {
            psFilePath = mPrintDirectoryPath + File.separator + "imgtopsfile";
            psPreProcessFilePath = mPrintDirectoryPath + File.separator + "pspreprocessfile";
            psMonoFilePath = mPrintDirectoryPath + File.separator + "imgtopsmonofile";
            pclFilePath = mPrintDirectoryPath + File.separator + "pstopclfile";
        } else {
            psFilePath = mExternalStorageDirPath + File.separator + "imgtopsfile";
            psPreProcessFilePath = mExternalStorageDirPath + File.separator + "pspreprocessfile";
            psMonoFilePath = mExternalStorageDirPath + File.separator + "imgtopsmonofile";
            pclFilePath = mExternalStorageDirPath + File.separator + "pstopclfile";

        }

        Log.d(GPrintCommon.TAG, "GPrintPrintingManager : psFilePath=" + psFilePath);
        Log.d(GPrintCommon.TAG, "GPrintPrintingManager : psPreProcessFilePath="
                + psPreProcessFilePath);
        Log.d(GPrintCommon.TAG, "GPrintPrintingManager : psMonoFilePath=" + psMonoFilePath);
        Log.d(GPrintCommon.TAG, "GPrintPrintingManager : pclFilePath=" + pclFilePath);
    }

    private boolean createTemDir() {
        try {
            File tempDirectory = new File(mPrintDirectoryPath);
            if (tempDirectory.exists()) {
                if (tempDirectory.isDirectory())
                    return true;
                else
                    // it is a file -- delete that file
                    tempDirectory.delete();
            }
        } catch (Exception ex) {
            ex.printStackTrace();
            return false;
        }
        try { // directory does not exist, create new
            File tempDirectory = new File(mPrintDirectoryPath);
            return tempDirectory.mkdirs();
        } catch (Exception ex) {
            ex.printStackTrace();

        }
        return false;
    }

    public void reportJobState(int type, long value) {
        Log.d(GPrintCommon.TAG, "GPrintPrintingManager : reportJobState : type=" + type + " value="
                + value);
        switch (type) {
            case 1: // preparing give total page count
                break;
            case 3:// held
                break;
            case 5: // processing

                break;
            case 9: // print completed
                // if(mCurrentTransaction != null)
                // Log.i("reportJobState","mCurrentTransaction.ID"+mCurrentTransaction.ID);
                // responsePrintCompleted(mCurrentTransaction.ID);
                break;
            case 101:
                if (mPrintFileSize > 0) {
                    int progress = (int)((value * 100) / mPrintFileSize);
                    Log.d(GPrintCommon.TAG,
                            "GPrintPrintingManager : reportJobState : mPrintFileSize :"
                                    + mPrintFileSize + " progress :" + progress
                                    + "mCurrentProgress :" + mCurrentProgress);

                    if (mCurrentProgress == GPrintCommon.psDone_psPrint) // PostScript
                                                                         // printing
                    {
                        progress = (int)((progress / 2) + mCurrentProgress);
                        sendCallback(mJobId, GPrintCommon.printingpage,String.valueOf(progress));
                    } else if (mCurrentProgress == GPrintCommon.pclDone_pclPrint) // PCL
                                                                                  // printing
                    {
                        progress = (int)(((progress * 17) / 50) + mCurrentProgress);
                        sendCallback(mJobId, GPrintCommon.printingpage, String.valueOf(progress));
                    }
                }

                break;
        }
    }

    public int sendCallback(int job_id, String result, String progress) {
        if (mCallback != null)
            mCallback.sendResponse(job_id, result, progress);

        return 0;
    }

    public void deleteTempFiles() {
        File tempDir = new File(mPrintDirectoryPath);
        if(tempDir != null && tempDir.exists()) {
            if(tempDir.isDirectory() == true) {
                File[] childFiles = tempDir.listFiles();
                for(int i = 0; i < childFiles.length; i++) {
                    if (childFiles[i].exists()&& (!childFiles[i].getAbsolutePath().endsWith("ppd"))) {
                        childFiles[i].delete();
                    }
                }
            }
           
        }
    }
    
    public int startPrint(int TransactionID, GPrintJob PrintJob) {
        // TODO Auto-generated method stub

		Log.d(GPrintCommon.TAG, "GPrintPrintingManager : psFilePath=" + psFilePath);
		Log.d(GPrintCommon.TAG, "GPrintPrintingManager : psPreProcessFilePath=" + psPreProcessFilePath);
		Log.d(GPrintCommon.TAG, "GPrintPrintingManager : psMonoFilePath=" + psMonoFilePath);
		Log.d(GPrintCommon.TAG, "GPrintPrintingManager : pclFilePath=" + pclFilePath);        
        
        String paperSize = "a4";
        mJobId = TransactionID;
        mCurrentProgress = 0;
        mPrintJob = PrintJob;
        sendCallback(mJobId, GPrintCommon.starting, "0");
        
        if (!selectPrinter()) {
            sendCallback(mJobId, GPrintCommon.error, GPrintCommon.printerNotSupported);// Not supported printer
            return (-1);
        }

        if (!getDriverFile()) {
            sendCallback(mJobId, GPrintCommon.error, GPrintCommon.outOfMemoryException);// out off memory
            return (-1);
        }

        mFilePathList = mPrintJob.getPrintJobFileList();
        mFileCount = mFilePathList.size();
        Log.d(GPrintCommon.TAG, "GPrintPrintingManager : startPrint :" + mFileCount);
        if (mFileCount == 0) {
            sendCallback(mJobId, GPrintCommon.error,  GPrintCommon.cannotReadFile);// contents_not_available;
            return (-1);
        }
        // check if input file is available
        for (int ii = 0; ii < mFileCount; ii++) {
            File pFile = new File(mFilePathList.get(ii));
            if (pFile.exists()) {
                if (!pFile.canRead()) {
                    Log.e(GPrintCommon.TAG,
                            "GPrintPrintingManager : startPrint : cannot read file :"+mFilePathList.get(ii));
                    sendCallback(mJobId, GPrintCommon.error, GPrintCommon.cannotReadFile);// contents_not_available;
                    return (-1);
                }

            } else {
                Log.e(GPrintCommon.TAG,
                        "GPrintPrintingManager : startPrint : input file is not readable");
                sendCallback(mJobId, GPrintCommon.error,  GPrintCommon.cannotReadFile);// contents_not_available;
                return (-1);
            }
        }

        if (GPrintCommon.wifi.equalsIgnoreCase(mPrintJob.getPrinterBackend()) && (isWifiConnected() == false)) {
            Log.e(GPrintCommon.TAG, "GPrintPrintingManager : startPrint : wifi not available");
            mCurrentProgress = 0;
            sendCallback(mJobId, GPrintCommon.error, GPrintCommon.printerNotAvailable);// Output Stream error
            return (-1);
        }

        if (GPrintCommon.usb.equalsIgnoreCase(mPrintJob.getPrinterBackend())) {
            if (!GPrintCommon.isUSBConnected()) { // dev/usb/lp0 not there
                if (!GPrintCommon.isUSBPrinterConnected(mContext)) {
                    // no printer connected over usb
                    Log.e(GPrintCommon.TAG,
                            "GPrintPrintingManager : startPrint : USB printer not available");
                    mCurrentProgress = 0;
                    sendCallback(mJobId, GPrintCommon.error,  GPrintCommon.printerNotAvailable);// Output Stream error
                    return (-1);
                }
            }
        }

		// Image print
		if (mPrinterName.equalsIgnoreCase(genPCLmono)
				|| mPrinterName.equalsIgnoreCase(genPCLcolor)) {
			// For converting PS to PCL
			if (mGhostMgr == null)
				mGhostMgr = new GPrintPsToPcl();
		}

		mJobTitle = mFilePathList.get(0);

        if (mPrinterName.equalsIgnoreCase(genPCLmono) || mPrinterName.equalsIgnoreCase(genPCLcolor))
            mPrintType = GPrintCommon.print_type_pcl;
        else
            mPrintType = GPrintCommon.print_type_ps;

        if (mFileCount == 1)
            mPrintJob.setPrintJobDuplex(GPrintCommon.simplex);

        Log.d(GPrintCommon.TAG, "GPrintPrintingManager : startPrint : value of mFileCount="
                + mFileCount);
        sendCallback(mJobId, GPrintCommon.totalpages, String.valueOf(mFileCount));// send number of files

        // Big for loop to process each file / page
        int pageCount = 0;
        int callBackPageCount = 0;
               
        // Process every file or page
        for (; pageCount < mFileCount; pageCount++, callBackPageCount++) {
            Log.d(GPrintCommon.TAG, "GPrintPrintingManager : startPrint : value of pageCount="
                    + pageCount);
            deleteTempFiles();
            // processing file number
            sendCallback(mJobId,GPrintCommon.currentpage, String.valueOf(callBackPageCount + 1));
            // Change Duplex to Simplex for last file if number of files are odd
            if ((pageCount + 1) == mFileCount && (mFileCount % 2 != 0))
                mPrintJob.setPrintJobDuplex(GPrintCommon.simplex);
            int ret = -1;
            if (mPrintType == GPrintCommon.print_type_ps) {

				ret = -1;
				// call image to ps
				if (!GPrintCommon.simplex.equalsIgnoreCase(mPrintJob
						.getPrintJobDuplex())) {
					// convert two images together to ps
					ret = createImageToPs(pageCount, pageCount + 1);
					// increment count because we have handled 2 pages
					pageCount = pageCount + 1;
					callBackPageCount++;
				} else {
					// convert single image to ps
					ret = createImageToPs(pageCount, pageCount);
				}
                
                if(ret == 0){
                    // Call backend to send file to printer
                    mPrintableFilePath = psFilePath;
                    // Send PS file to printer
                    // 50% task complete
                    mCurrentProgress = GPrintCommon.psDone_psPrint;
                    sendCallback(mJobId, GPrintCommon.printingpage, String.valueOf(mCurrentProgress));
                    int ret3 = sendToBackend(callBackPageCount);
                    if (ret3 > 0) // send to backend failed
                        break;// break for loop, will not print further files

                } else {
                    // conversion to ps fail;
                    Log.e(GPrintCommon.TAG,
                            "GPrintPrintingManager : startPrint : Conversion to PS fail... Abort!!!");
                    mError = true;
                    mCurrentProgress = 0;
                    sendCallback(mJobId, GPrintCommon.error,GPrintCommon.fileNotSupported);// not supported content
                    break;// break for loop, will not print further files
                }

            } else if (mPrintType == GPrintCommon.print_type_pcl) {
                int ret1 = -1;

				// call image to ps
				ret = -1;
				if (!GPrintCommon.simplex.equalsIgnoreCase(mPrintJob
						.getPrintJobDuplex())) {
					// convert two images together to ps
					ret = createImageToPs(pageCount, pageCount + 1);
					// increment count because we have handled 2 pages
					pageCount = pageCount + 1;
					callBackPageCount++;
				} else {
					// convert single image to ps
					ret = createImageToPs(pageCount, pageCount);
				}

				if (ret == 0) {
					// 33% = ps to pcl completed
					mCurrentProgress = GPrintCommon.psDone_pclPrint;
					sendCallback(mJobId, GPrintCommon.printingpage,
							String.valueOf(mCurrentProgress));

					// call ps to pcl
					Log.d(GPrintCommon.TAG,
							"GPrintPrintingManager : startPrint : Calling GS to convert PS to PCl : pageCount ="
									+ pageCount);
					ret1 = createPsToPcl();
					// 66% = ps to pcl completed
					mCurrentProgress = GPrintCommon.pclDone_pclPrint;
					sendCallback(mJobId, GPrintCommon.printingpage,
							String.valueOf(mCurrentProgress));
				} else {
					// conversion to ps fail;
					Log.e(GPrintCommon.TAG,
							"GPrintPrintingManager : startPrint : Conversion to PS fail... Abort!!!");
					mError = true;
					mCurrentProgress = 0;
					sendCallback(mJobId, GPrintCommon.error,
							GPrintCommon.fileNotSupported);// not supported
															// content
					break;// break for loop, will not print further files
				}

                if (ret1 == 0) {
                    // Send to Backend
                    mPrintableFilePath = pclFilePath;
                    int ret3 = sendToBackend(callBackPageCount);
                    if (ret3 > 0) // send to backend failed
                        break;// break for loop, will not print further

                } else {
                    Log.e(GPrintCommon.TAG,
                            "GPrintPrintingManager : startPrint : Conversion to PCL fail... Abort!!!");
                    mError = true;
                    mCurrentProgress = 0;
                    sendCallback(mJobId, GPrintCommon.error,GPrintCommon.fileNotSupported);// not supported content
                    break;// break for loop, will not print further files
                }
            }

        }
        // end of for loop
        if (mError != true) {
            mCurrentProgress = 100;
            sendCallback(mJobId, GPrintCommon.finishedjob,String.valueOf(mCurrentProgress));// job completed
        }
        // comment it to retain files after printing
        deleteTempFiles();
        return (0);
    }

    private int sendToBackend(int loopCount) {
        int ret3 = -1;
        mPrintFileSize = 0;
        try {
            File printFile = new File(mPrintableFilePath);
            if (printFile.exists() && printFile.isFile()) {
                mPrintFileSize = printFile.length();
                Log.d(GPrintCommon.TAG, "GPrintPrintingManager : sendToBackend : mPrintFileSize :"
                        + mPrintFileSize);
            } else
                return -1;
        } catch (Exception ex) {

        }

        if (GPrintCommon.usb.equalsIgnoreCase(mPrintJob.getPrinterBackend())) {
            // USB print
            if (!GPrintCommon.isUSBConnected()) { // dev/usb/lp0 not there
                if (!GPrintCommon.isUSBPrinterConnected(mContext)) { 
                 // no printer  connected   over usb                
                    Log.e(GPrintCommon.TAG,
                            "GPrintPrintingManager : startPrint : USB printer not available");
                    mCurrentProgress = 0;
                    sendCallback(mJobId, GPrintCommon.error,  GPrintCommon.printerNotAvailable);// Output Stream error
                    return (-1);
                }
            }

            ret3 = sendToUSB();
            Log.i(GPrintCommon.TAG,
                    "GPrintPrintingManager : startPrint : sendToUSB  finished with return value:"
                            + ret3);
        } else {
            // IPP print
            if (isWifiConnected() == false) {
                Log.e(GPrintCommon.TAG,
                        "GPrintPrintingManager : sendToBackend : wifi not available");
                mError = true;
                mCurrentProgress = 0;
                sendCallback(mJobId, GPrintCommon.error,  GPrintCommon.printerNotAvailable);// Output Stream Error
                return (-1);
            }
            ret3 = sendToSocket();
            Log.i(GPrintCommon.TAG,
                    "GPrintPrintingManager : startPrint : sendToSocket finished with return value:"
                            + ret3);

            // ret3=sendToIpp();
            // Log.i(GPrintCommon.TAG,
            // "GPrintPrintingManager : startPrint : sendToIpp finished with return value:"+ret3);
        }
        switch (ret3) {
            case 0:
                mCurrentProgress = 100;
                sendCallback(mJobId, GPrintCommon.printingpage, String.valueOf(mCurrentProgress)); // 100% = page
                                                           // progress value
                sendCallback(mJobId, GPrintCommon.finishedpage, String.valueOf(loopCount + 1)); // page completed
                break;
            case 1:
                mError = true;
                mCurrentProgress = 0;
                sendCallback(mJobId, GPrintCommon.error,  GPrintCommon.printerNotAvailable);// Output Stream Error
                break;
            case 2:
                mError = true;
                mCurrentProgress = 0;
                sendCallback(mJobId, GPrintCommon.error,  GPrintCommon.printerNotAvailable);// Output Stream Error
                break;
            default:// Job failed, unable to locate printer
                mError = true;
                mCurrentProgress = 0;
                sendCallback(mJobId, GPrintCommon.error,  GPrintCommon.printerNotAvailable);// Output Stream Error
                break;
        }

        return ret3;
    }

    private int sendToSocket() {
        List<String> socketCommand = new ArrayList<String>();
        socketCommand.clear();
        // socketCommand.add("socket://"+mPrintJob.mIPAddr+":9100");//printer
       // printer uri ?waiteof=false
        socketCommand.add("socket://" + mPrintJob.getPrinterIpAddress() + ":9100/?waiteof=false");
        socketCommand.add("1"); // job-id
        socketCommand.add("root"); // user-id
        socketCommand.add(mFilePathList.get(0)); // job Title
        socketCommand.add("1");
        socketCommand.add(mPrintableFilePath); // in place of passing options
        socketCommand.add(mPrintableFilePath);

        String[] linesArr1 = socketCommand.toArray(new String[socketCommand.size()]);
        int ret = InvokeSocketBackend(linesArr1);

        return ret;
    }
	
    private int sendToIpp() {
        List<String> ippCommand = new ArrayList<String>();
        ippCommand.clear();
        // printer uri
        ippCommand.add("ipp://" + mPrintJob.getPrinterIpAddress() + ":631");
                                                                 
        ippCommand.add("1"); // job-id
        ippCommand.add("root"); // user-id
        ippCommand.add(mFilePathList.get(0)); // job Title
        ippCommand.add("1"); // number of copy
        ippCommand.add(mPrintableFilePath); // in place of passing options
        ippCommand.add(mPPDFilePath); // ppd file path
        ippCommand.add(mPrintableFilePath);

        String[] linesArr1 = ippCommand.toArray(new String[ippCommand.size()]);
        int ret = InvokeIppBackend(linesArr1);

        return ret;
    }

	
    private int sendToUSB() {

        if (GPrintCommon.isUSBConnected()) {
            List<String> usbCommand = new ArrayList<String>();
            usbCommand.clear();
            String dev[] = GetDevFileUsbDevice();
            if (dev != null) {
                int dev_length = dev.length;
                String deviceIP = dev[0];
                String makeModel = dev[1];
                String idInfo = dev[2];

                Log.i(GPrintCommon.TAG, "GPrintPrintingManager : sendToUSB : usb search 1: "
                        + deviceIP); // Ip address

                if (deviceIP != null) {
                    usbCommand.add(deviceIP);
                    usbCommand.add("2"); // job-id
                    usbCommand.add("root"); // user-id
                    usbCommand.add(mFilePathList.get(0)); // job Title
                    // Log.d(GPrintCommon.TAG,
                    // "GPrintPrintingManager : sendToUSB : Number of copies="+mPrintJob.mNumberOfCopy);
                    // if(mPrintJob.mNumberOfCopy >= 1 &&
                    // mPrintJob.mNumberOfCopy < 999)
                    // usbCommand.add(Integer.toString(mPrintJob.mNumberOfCopy));
                    // else
                    usbCommand.add("1");
                    // printCommand.add(printOptions.toString());
                    usbCommand.add(mPrintableFilePath); // in place of passing
                                                        // options
                    usbCommand.add(mPrintableFilePath);
                    String[] linesArr1 = usbCommand.toArray(new String[usbCommand.size()]);
                    int ret = InvokeDevFileUsbBackend(linesArr1);

                    return ret;

                } else
                    return 1; // cannot connect to printer
            } else
                return 1; // cannot connect to printer

        } else {
            List<String> usbCommand = new ArrayList<String>();
            usbCommand.clear();
            String dev[] = GetUsbDeviceList();
            if (dev != null) {
                int dev_length = dev.length;
                String deviceIP = dev[0];
                String makeModel = dev[1];
                String idInfo = dev[2];

                Log.i(GPrintCommon.TAG, "GPrintPrintingManager : sendToUSB : usb search 1: "
                        + deviceIP); // Ip address
                if (deviceIP != null) {
                    usbCommand.add(deviceIP);
                    usbCommand.add("2"); // job-id
                    usbCommand.add("root"); // user-id
                    usbCommand.add(mFilePathList.get(0)); // job Title
                    // Log.d(GPrintCommon.TAG,
                    // "GPrintPrintingManager : sendToUSB : Number of copies="+mPrintJob.mNumberOfCopy);
                    // if(mPrintJob.mNumberOfCopy >= 1 &&
                    // mPrintJob.mNumberOfCopy < 999)
                    // usbCommand.add(Integer.toString(mPrintJob.mNumberOfCopy));
                    // else
                    usbCommand.add("1");
                    // printCommand.add(printOptions.toString());
                    usbCommand.add(mPrintableFilePath); // in place of passing
                                                        // options
                    usbCommand.add(mPrintableFilePath);
                    String[] linesArr1 = usbCommand.toArray(new String[usbCommand.size()]);
                    int ret = InvokeUsbBackend(linesArr1);

                    return ret;

                } else
                    return 1; // cannot connect to printer
            } else
                return 1; // cannot connect to printer

        }

    }

  
    
    private int createPsToPcl() {
        int ret1 = -1;
        if (mGhostMgr != null) {
            // check which pcl output is required
            String paperSize = "a4";
            if (mPrintJob.getPrintJobPaperSize() == "JISB5")
                paperSize = "b5";
            else
                paperSize = mPrintJob.getPrintJobPaperSize().toLowerCase();

            if (mPcl6Supported == true)
                ret1 = mGhostMgr.psToPclXl(psFilePath, pclFilePath, paperSize,
                        mPrintJob.getPrintJobColor(), mPrintJob.getPrintJobDuplex());
//            else
//                ret1 = mGhostMgr.psToPcl3(psFilePath, pclFilePath, paperSize,
//                        mPrintJob.mSettings.getColorType(), mPrintJob.mSettings.getDuplexType());
        }
        return ret1;
    }
	
    
    
  
    
    private int createImageToPs(int fileNo1, int fileNo2) {
        int ret = -1;
        String newPath1 = null;
        String newPath2 = null;
        String newFile = null;
        Log.d(GPrintCommon.TAG,"GPrintPrintingManager : createImageToPs : model:"+mPrintJob.getPrinterModelName());
        
        if(mPrintJob.getPrinterModelName().toUpperCase().contains("BROTHER")){
        	//Brother is giving outoff memory error for big images
        	//so reduce image size
        	Log.d(GPrintCommon.TAG,"GPrintPrintingManager : createImageToPs : reduce image size for Brother");
        	String orientation = GPrintCommon.getImageOrientation(mFilePathList.get(fileNo1));
        	if(orientation.equalsIgnoreCase(GPrintCommon.landscape))
        		newFile = GPrintImageManager.changeResolution(mFilePathList.get(fileNo1), mPrintDirectoryPath,842,595);
        	else
        		newFile = GPrintImageManager.changeResolution(mFilePathList.get(fileNo1), mPrintDirectoryPath, 595, 842);
        	if(newFile != null && newFile.length()>1){
        		Log.d(GPrintCommon.TAG,"GPrintPrintingManager : createImageToPs : using newFile :"+newFile);
        		mFilePathList.set(fileNo1, newFile);
        	}
        	if(fileNo2 != fileNo1){
        		String newFile2 = GPrintImageManager.changeResolution(mFilePathList.get(fileNo2), mPrintDirectoryPath, 595, 842);
            	if(newFile2 != null && newFile2.length()>1){
            		Log.d(GPrintCommon.TAG,"GPrintPrintingManager : createImageToPs : using newFile2 :"+newFile2);
            		mFilePathList.set(fileNo2, newFile2);
            	}
        	}
        }
        
        if(mPrinterName.equalsIgnoreCase(genPCLcolor) || mPrinterName.equalsIgnoreCase(genPCLmono)){
            Log.d(GPrintCommon.TAG,"GPrintPrintingManager : createImageToPs : PCL Print");
            if(fileNo1 == fileNo2){
                String orientation = GPrintCommon.getImageOrientation(mFilePathList.get(fileNo1));
                if (orientation.equalsIgnoreCase(GPrintCommon.unknown)) {
                    Log.e(GPrintCommon.TAG,
                            "GPrintPrintingManager : startPrint : Cannot find orientation");
                    sendCallback(mJobId, GPrintCommon.error,GPrintCommon.fileNotSupported);// Not supported contents
                    return (-1);
                }
                if(((orientation == GPrintCommon.landscape)&&(GPrintCommon.auto.equalsIgnoreCase(mPrintJob.getPrintJobOrientation())|| GPrintCommon.landscape.equalsIgnoreCase(mPrintJob.getPrintJobOrientation())))||
                        (GPrintCommon.portrait.equalsIgnoreCase(orientation) && GPrintCommon.landscape.equalsIgnoreCase(mPrintJob.getPrintJobOrientation()))){
                    //need to rotate input image
                    if(orientation == GPrintCommon.landscape)
                        newPath1 = GPrintImageManager.rotateImage(mFilePathList.get(fileNo1), mPrintDirectoryPath, 90);
                    else
                        newPath1 = GPrintImageManager.rotateImage(mFilePathList.get(fileNo1), mPrintDirectoryPath, -90);
                    
                    if(newPath1 != null){
                        newPath2 = newPath1;
                        ret=imageToPS(psFilePath,newPath1,newPath2,GPrintCommon.portrait);
                    }else{
                        Log.e(GPrintCommon.TAG,"GPrintPrintingManager : createImageToPs : Error: can not rotare image");
                        sendCallback(mJobId, GPrintCommon.error,GPrintCommon.fileNotSupported);// Not supported contents
                        return (-1);
                    }
                }else{
                    //no need to change rotation
                    Log.d(GPrintCommon.TAG,"GPrintPrintingManager : createImageToPs : No need to change rotation");
                    newPath1 = mFilePathList.get(fileNo1);
                    newPath2 = newPath1;
                    ret=imageToPS(psFilePath,newPath1,newPath2,mPrintJob.getPrintJobOrientation());
                }
                
            }else{//both files are different
                String orient;
                String orientation1 = GPrintCommon.getImageOrientation(mFilePathList.get(fileNo1));
                if (orientation1.equalsIgnoreCase(GPrintCommon.unknown)) {
                    Log.e(GPrintCommon.TAG,
                            "GPrintPrintingManager : createImageToPs : Cannot find orientation");
                    sendCallback(mJobId, GPrintCommon.error,GPrintCommon.fileNotSupported);// Not supported contents
                    return (-1);
                }
                if(((GPrintCommon.landscape.equalsIgnoreCase(orientation1))&&((GPrintCommon.auto.equalsIgnoreCase(mPrintJob.getPrintJobOrientation()))|| GPrintCommon.landscape.equalsIgnoreCase(mPrintJob.getPrintJobOrientation())))||
                    ( GPrintCommon.portrait.equalsIgnoreCase(orientation1) && GPrintCommon.landscape.equalsIgnoreCase(mPrintJob.getPrintJobOrientation()))){
                    //need to rotate
                    if(GPrintCommon.landscape.equalsIgnoreCase(orientation1))
                        newPath1 = GPrintImageManager.rotateImage(mFilePathList.get(fileNo1), mPrintDirectoryPath, 90);
                    else
                        newPath1 = GPrintImageManager.rotateImage(mFilePathList.get(fileNo1), mPrintDirectoryPath, -90);
                    
                    orient = GPrintCommon.portrait;
                    
                }else{
                    newPath1 = mFilePathList.get(fileNo1);
                    orient = mPrintJob.getPrintJobOrientation();
                }
                
                String orientation2 = GPrintCommon.getImageOrientation(mFilePathList.get(fileNo2));
                if (orientation2.equalsIgnoreCase(GPrintCommon.unknown)) {
                    Log.e(GPrintCommon.TAG,
                            "GPrintPrintingManager : startPrint : Cannot find orientation");
                    sendCallback(mJobId, GPrintCommon.error, GPrintCommon.fileNotSupported);// Not supported contents
                    return (-1);
                }
                if(((GPrintCommon.landscape.equalsIgnoreCase(orientation2))&&((GPrintCommon.auto.equalsIgnoreCase(mPrintJob.getPrintJobOrientation()))|| GPrintCommon.landscape.equalsIgnoreCase(mPrintJob.getPrintJobOrientation())))||
                    (GPrintCommon.portrait.equalsIgnoreCase(orientation2) && GPrintCommon.landscape.equalsIgnoreCase(mPrintJob.getPrintJobOrientation()))){
                    //need to rotate
                    if(GPrintCommon.landscape.equalsIgnoreCase(orientation2))
                        newPath2 = GPrintImageManager.rotateImage(mFilePathList.get(fileNo2), mPrintDirectoryPath, 90);
                    else
                        newPath2 = GPrintImageManager.rotateImage(mFilePathList.get(fileNo2), mPrintDirectoryPath, -90);
                    
                    orient = GPrintCommon.portrait;
                }else{
                    newPath2 = mFilePathList.get(fileNo2);
                    orient = mPrintJob.getPrintJobOrientation();
                }
                if(newPath1 != null && newPath2 != null){
                    
                    ret=imageToPS(psFilePath,newPath1,newPath2,orient);
                }else{
                    Log.e(GPrintCommon.TAG,"GPrintPrintingManager : createImageToPs : Error : Cannot rotate Image");
                    sendCallback(mJobId,GPrintCommon.error,GPrintCommon.fileNotSupported);// Not supported contents
                    return (-1);
                }
                
                
            }
            
        }else{
            //PostScript print
            Log.d(GPrintCommon.TAG,"GPrintPrintingManager : createImageToPs : PS Print");
            ret = imageToPS(psFilePath, mFilePathList.get(fileNo1),mFilePathList.get(fileNo2),mPrintJob.getPrintJobOrientation());
        }
            
            
        return ret;
    }
    
    private int imageToPS(String psFilePath, String inFile, String inFile2,String orient) {

        printCommand = new ArrayList<String>();
        StringBuilder printOptions = new StringBuilder("finishings=3");
        printCommand.add(mPPDFilePath); // 0
        printCommand.add("1"); // job-id //1
        printCommand.add("root"); // user-id //2
        printCommand.add(mJobTitle); // Job Title //3
        // Add Number of copy option
        if (mPrintJob.getNumberOfCopies() >= 1 && mPrintJob.getNumberOfCopies() <= 999)
            printCommand.add(Integer.toString(mPrintJob.getNumberOfCopies())); // 4
        else
            printCommand.add("1"); // 4

        //if (mPrintJob.getPrintJobPaperSize() == "JISB5"
        //        && (mPrinterName.equalsIgnoreCase(genPCLmono) || mPrinterName
        //                .equalsIgnoreCase(genPCLcolor))) {
        Log.d(GPrintCommon.TAG,"GPrintprintingmanager : imageToPS : paper size="+mPrintJob.getPrintJobPaperSize());
        if (mPrintJob.getPrintJobPaperSize().equalsIgnoreCase("JISB5")){
            printOptions.append(" media=" + "B5");
            printOptions.append(" PageSize=" + "B5");
        } else {
            printOptions.append(" media=" + mPrintJob.getPrintJobPaperSize());
            printOptions.append(" PageSize=" + mPrintJob.getPrintJobPaperSize());
        }
        printOptions.append(" fit-to-page");
        // Add orientation option
        if (GPrintCommon.landscape.equalsIgnoreCase(orient)) {
            // printCommand.append("-o landscape -o orientation-requested=4;");
            // printCommand.append("-o landscape;");
            printOptions.append(" orientation-requested=4");
        } else {
            // printCommand.append("-o orient-requested=3;");
            printOptions.append(" orientation-requested=3");
        }
        if (GPrintCommon.shortedge.equalsIgnoreCase(mPrintJob.getPrintJobDuplex())) {
            // printOptions.append(" sides=two-sided-short-edge");
            printOptions.append(" Duplex=DuplexTumble");
        } else if (GPrintCommon.longedge.equalsIgnoreCase(mPrintJob.getPrintJobDuplex())) {
            // printOptions.append(" sides=two-sided-long-edge");
            printOptions.append(" Duplex=DuplexNoTumble");
        }

        printOptions.append(" number-up=1");
        printOptions.append(" job-originating-host-name=localhost");
        printOptions.append(" time-at-creation=" + (int)System.currentTimeMillis());
        printOptions.append(" time-at-processing=" + (int)System.currentTimeMillis());
        Log.d(GPrintCommon.TAG,
                "createImageToPsCommand : printOptions.toString()=" + printOptions.toString());
        printCommand.add(printOptions.toString());
        printCommand.add(mPPDFilePath); // ppd file path

        String ext = inFile.substring(inFile.lastIndexOf(".") + 1).toLowerCase();
        Log.d(GPrintCommon.TAG, "GPrintPrintingManager : imageToPS :ext=" + ext);
        if (ext.equalsIgnoreCase("jpeg") || ext.equalsIgnoreCase("jpg")
                || ext.equalsIgnoreCase("jpe") || ext.equalsIgnoreCase("bmp")
                || ext.equalsIgnoreCase("wbmp"))
            printCommand.add("image/jpeg"); // 7
        else if (ext.equalsIgnoreCase("png"))
            printCommand.add("image/png"); // 7
        else if (ext.equalsIgnoreCase("gif"))
            printCommand.add("image/gif"); // 7

        printCommand.add(psFilePath); // 8

        // printCommand.add(isEOF);
        if (GPrintCommon.mono.equalsIgnoreCase(mPrintJob.getPrintJobColor())) {
            printCommand.add("0"); // 9
        } else {
            printCommand.add("1"); // 9
        }

        printCommand.add(inFile);// 10
        if (!inFile.equalsIgnoreCase(inFile2))
            printCommand.add(inFile2);// 11
        String[] linesArr = printCommand.toArray(new String[printCommand.size()]);
        int ret = InvokeImageToPs(linesArr);
        Log.d(GPrintCommon.TAG,
                "GPrintPrintingManager : imageToPS : InvokeImageToPs finished with ret=" + ret);
        return ret;
    }
    


    private boolean getDriverFile() {
        AssetManager assetManager = mContext.getAssets();

        File tempDir = new File(mPrintDirectoryPath);
        if (tempDir.exists() && tempDir.isDirectory())
            mPPDFilePath = mPrintDirectoryPath + File.separator + mPrinterName + ".ppd";
        else
            mPPDFilePath = mExternalStorageDirPath + File.separator + mPrinterName + ".ppd";
        File mPpdFile = new File(mPPDFilePath);
        if (mPpdFile.exists()) {
            return true;
        }

        InputStream in = null;
        OutputStream out = null;
        try {
            in = assetManager.open(mPrinterName + ".ppd");
            // out = new FileOutputStream("/mnt/sdcard/"+ mPrinterName+".ppd");
            out = new FileOutputStream(mPPDFilePath);
            copyFile(in, out);
            in.close();
            in = null;
            out.flush();
            out.close();
            out = null;
        } catch (Exception e) {

            Log.e(GPrintCommon.TAG,
                    "GPrintPrintingManager : getDriverFile: error :" + e.getMessage());
            e.printStackTrace();
            return false;
        }

        return true;
    }

    private void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while ((read = in.read(buffer)) != -1) {
            out.write(buffer, 0, read);
        }
    }

    private boolean selectPrinter() {
        boolean postScriptSupported = false;
        boolean pclSupported = false;
        mPcl6Supported = false;
        ArrayList<String> languages = mPrintJob.getPrinterLanguageList();
        int numLang = languages.size();
        Log.d(GPrintCommon.TAG, "GPrintPrintingManager : selectPrinter(): languages :"
                + languages);
        for (int lang = 0; lang < numLang; lang++) {
            String language = languages.get(lang).toUpperCase();
            if (language.contains("POSTSCRIPT") || language.equalsIgnoreCase("PS")
                    || language.equalsIgnoreCase("PS3") || language.equalsIgnoreCase("PS2")
                    || language.equalsIgnoreCase("PS 3") || language.equalsIgnoreCase("PS 2")) {
                postScriptSupported = true;
            } else if (language.contains("PCL") || language.contains("PRINTER COMMAND LANGUAGE")) {
                //pclSupported = true; //to only support pcl6
                if (language.contains("PCL6") || language.contains("PCL 6")
                        || language.contains("PCLXL") || language.contains("PCL XL")|| language.contains("PAGE CONTROL LANGUAGE ENHANCED")||language.contains("ENHANCED")) {
                    mPcl6Supported = true;
                    pclSupported = true;
                }
            }
        }
        // int fileDesc=0;

        if (postScriptSupported && pclSupported) {
            if (GPrintCommon.color.equalsIgnoreCase(mPrintJob.getPrintJobColor())) {
                mPrinterName = genPSmono;
            } else {

                if (GPrintCommon.mono.equalsIgnoreCase(mPrintJob.getPrinterColor())) {
                    mPrinterName = genPSmono;
                } else if (GPrintCommon.color.equalsIgnoreCase(mPrintJob.getPrinterColor())) {
                    mPrinterName = genPScolor;

                }
            }
        } else if (postScriptSupported) {
            if (GPrintCommon.mono.equalsIgnoreCase(mPrintJob.getPrintJobColor())) {
                mPrinterName = genPSmono;
            } else {
                if (GPrintCommon.mono.equalsIgnoreCase(mPrintJob.getPrinterColor())) {
                    mPrinterName = genPSmono;
                } else if (GPrintCommon.color.equalsIgnoreCase(mPrintJob.getPrinterColor())) {
                    // add color printer ppd file
                    mPrinterName = genPScolor;
                }
            }
        } else if (pclSupported) {
            // Add generic PCL PPD
            if (GPrintCommon.mono.equalsIgnoreCase(mPrintJob.getPrintJobColor())) {
                // user has selected monochrome print
                mPrinterName = genPCLmono;
            } else {

                if (GPrintCommon.mono.equalsIgnoreCase(mPrintJob.getPrinterColor())) {
                    // user has selected color print but printer does not
                    // support color printing
                    mPrinterName = genPCLmono;
                } else if (GPrintCommon.color.equalsIgnoreCase(mPrintJob.getPrinterColor())) {
                    // add color printer ppd file
                    mPrinterName = genPCLcolor;
                }
            }

        } else {
            // Cannot print by CUPS
            Log.e(GPrintCommon.TAG,
                    "GPrintPrintingManager : addCupsPrinter():Printer does not support PostScript & PCL");
            // Respond Error
            return (false);
        }

        return true;
    }

    @SuppressLint("NewApi")
    public boolean isWifiConnected() {

        WifiManager wifi = (WifiManager)mContext.getSystemService(Context.WIFI_SERVICE);
        WifiP2pManager wifiP2pManager = (WifiP2pManager)mContext
                .getSystemService(Context.WIFI_P2P_SERVICE);
        if (wifiP2pManager != null) {
            ConnectivityManager connectivityManager = (ConnectivityManager)mContext
                    .getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo netInfo = connectivityManager
                    .getNetworkInfo(ConnectivityManager.TYPE_WIFI_P2P);
            if (netInfo != null) {
                if (DetailedState.CONNECTED.equals(netInfo.getDetailedState())) {
                    Log.d(GPrintCommon.TAG,
                            netInfo.getTypeName() + " : " + netInfo.getDetailedState());
                    return true;
                }
            }
        }

        if (wifi.isWifiApEnabled() == true) {
            Log.d(GPrintCommon.TAG, "WifiAp : Enabled");
            return true;
        }

        if (wifi.isWifiEnabled() == true) {
            WifiInfo wifiInfo = wifi.getConnectionInfo();
            if (wifiInfo.getIpAddress() != 0) {
                return true;
            }
        }

        return false;
    }

}
