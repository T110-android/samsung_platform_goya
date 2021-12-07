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

public class GPrintUsbPrinter implements Parcelable {

    public static final String TAG = "Capabilities";

    
    private String mDeviceIP=null;
    private String mMakeModel=null;
    private String mDeviceID = null;
  

    public static final Parcelable.Creator<GPrintUsbPrinter> CREATOR = new Parcelable.Creator<GPrintUsbPrinter>() {
        public GPrintUsbPrinter createFromParcel(Parcel source) {
            return new GPrintUsbPrinter(source);
        }

        public GPrintUsbPrinter[] newArray(int size) {
            return new GPrintUsbPrinter[size];
        }
    };

    
    public GPrintUsbPrinter() {
        mDeviceIP="Unknown";
        mMakeModel = "Unknown";
        mDeviceID = "Unknown";
    }

    private GPrintUsbPrinter(Parcel source) {
        mDeviceIP = source.readString();
        mMakeModel = source.readString();
        mDeviceID = source.readString();
    }

    public String getDeviceIp() {
        return mDeviceIP;
    }

    public String getMakeModel() {
           return mMakeModel;
     }
     
    public String getDeviceId() {
        return mDeviceID;
    }
    
    public void setDeviceId(String deviceId) {
        if(deviceId != null)
            mDeviceID = deviceId;
    }

    public void setDeviceIp(String deviceIp) {
        if(deviceIp != null)
            mDeviceIP = deviceIp;
    }

    public void setMakeModel(String makeMdl) {
        if(makeMdl != null)
            mMakeModel = makeMdl;
     }
       

    @Override
    public int describeContents() {
        return 0;
    }

    
    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(mDeviceIP);
        dest.writeString(mMakeModel);
        dest.writeString(mDeviceID);
    }

    @Override
    public String toString() {
        return "Capabilities [mDeviceIP=" + mDeviceIP + ", mMakeModel=" + mMakeModel
                + ", mDeviceID=" + mDeviceID + "]";
    }
}
