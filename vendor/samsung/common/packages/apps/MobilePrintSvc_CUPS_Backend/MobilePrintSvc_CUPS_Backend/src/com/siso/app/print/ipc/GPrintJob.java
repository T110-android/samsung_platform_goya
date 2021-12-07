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


package com.siso.app.print.ipc;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

import java.util.ArrayList;

public class GPrintJob implements Parcelable {

    public static final String TAG = "PrintSettings";

    private int mNumberOfCopies;

    private String mSelectedPaperSize;
    
    private String mPrinterModelName;

    private String mSelectedOrientation; //portrait , landscape, auto
    
    private String mPrinterBackend; // usb , wifi ;

    private String mSelectedColor;  // color , mono 
    
    private String mPrinterColorSupport; // color , mono 

    private String mSelectedDuplex; //simplex, longedge, shortedge,

    private ArrayList<String> mPrinterLanguageList;

    private ArrayList<String> mPrintJobFileList;
    
    private String mPrinterIpAddress;

    private int mSelectedPageRangeStart;

    private int mSelectedPageRangeEnd;

    public static final Parcelable.Creator<GPrintJob> CREATOR = new Parcelable.Creator<GPrintJob>() {
        public GPrintJob createFromParcel(Parcel source) {
            return new GPrintJob(source);
        }

        public GPrintJob[] newArray(int size) {
            return new GPrintJob[size];
        }
    };

    public GPrintJob() {
        mNumberOfCopies = 1;
        mSelectedPaperSize = "A4";
        mSelectedOrientation = "portrait";
        mSelectedColor = "mono";
        mPrinterColorSupport = "mono";
        mSelectedDuplex = "simplex";
        mPrinterBackend = "wifi";
        mPrinterLanguageList = new ArrayList<String>();
        mPrintJobFileList = new ArrayList<String>();
        mSelectedPageRangeStart = 1;
        mSelectedPageRangeEnd = 1;
        mPrinterIpAddress = "Unknown";
        mPrinterModelName = "Unknown";
    }

    private GPrintJob(Parcel source) {
        mNumberOfCopies = source.readInt();
        mSelectedPaperSize = source.readString();
        mSelectedOrientation = source.readString();
        mSelectedColor = source.readString();
        mPrinterColorSupport = source.readString();
        mSelectedDuplex = source.readString();
        mPrinterBackend = source.readString();
        mPrinterLanguageList = new ArrayList<String>();
        source.readStringList(mPrinterLanguageList);
        mPrintJobFileList = new ArrayList<String>();
        source.readStringList(mPrintJobFileList);
        mSelectedPageRangeStart = source.readInt();
        mSelectedPageRangeEnd = source.readInt();
        mPrinterIpAddress = source.readString();
        mPrinterModelName = source.readString();
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(mNumberOfCopies);
        dest.writeString(mSelectedPaperSize);
        dest.writeString(mSelectedOrientation);
        dest.writeString(mSelectedColor);
        dest.writeString(mPrinterColorSupport);
        dest.writeString(mSelectedDuplex);
        dest.writeString(mPrinterBackend);
        dest.writeStringList(mPrinterLanguageList);
        dest.writeStringList(mPrintJobFileList);
        dest.writeInt(mSelectedPageRangeStart);
        dest.writeInt(mSelectedPageRangeEnd);
        dest.writeString(mPrinterIpAddress);
        dest.writeString(mPrinterModelName);
        
    }
    
    public int getNumberOfCopies() {
        return mNumberOfCopies;
    }

    public void setNumberOfCopies(int copies) {
        mNumberOfCopies = copies;
    }

    public String getPrintJobPaperSize() {
        return mSelectedPaperSize;
    }

    public void setPrintJobPaperSize(String paperSize) {
        if(mSelectedPaperSize != null)
            mSelectedPaperSize = paperSize;
    }

    public String getPrintJobOrientation() {
        return mSelectedOrientation;
    }

    public void setPrintJobOrientation(String orientation) {
        mSelectedOrientation = orientation;
    }

    public String getPrintJobColor() {
        return mSelectedColor;
    }

    public void setPrintJobColor(String color) {
        mSelectedColor = color;
    }
    
    public String getPrinterColor() {
        return mPrinterColorSupport;
    }

    public void setPrinterColor(String color) {
        mPrinterColorSupport = color;
    }

    public String getPrintJobDuplex() {
        return mSelectedDuplex;
    }

    public void setPrintJobDuplex(String duplex) {
        mSelectedDuplex = duplex;
    }

    public String getPrinterBackend() {
        return mPrinterBackend;
    }

    public void setPrinterBackend(String backend) {
        mPrinterBackend = backend;
    }
    
    public ArrayList<String> getPrinterLanguageList() {
        return mPrinterLanguageList;
    }

    public void addPrinterLanguage(String language) {
        if (language != null)
           mPrinterLanguageList.add(language);
    }

    public void clearLanguageList() {
        mPrinterLanguageList.clear();
    }

    public ArrayList<String> getPrintJobFileList() {
        return mPrintJobFileList;
    }

    public void addPrintJobFile(String filepath) {
        if (filepath != null)
           mPrintJobFileList.add(filepath);
    }

    public void clearPrintJobFileList() {
        mPrintJobFileList.clear();
    }

    public int getPrintJobRangeStart() {
        return mSelectedPageRangeStart;
    }

    public int getPrintJobRangeEnd() {
        return mSelectedPageRangeEnd;
    }

    public void setPrintJobPageRange(int start, int end) {
        mSelectedPageRangeStart = start;
        mSelectedPageRangeEnd = end;
    }
    
    public String getPrinterIpAddress() {
        return mPrinterIpAddress;
    }

    public void setPrinterIpAddr(String ip) {
        if(ip != null)
            mPrinterIpAddress = ip;
    }
    
    public String getPrinterModelName() {
        return mPrinterModelName;
    }

    public void setPrinterModelName(String mdl) {
        if(mdl != null)
            mPrinterModelName = mdl;
    }

    @Override
    public int describeContents() {
        return 0;
    }

 

  
}
