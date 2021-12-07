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

import com.siso.app.print.ipc.*;

import android.os.RemoteException;

public class GPrintExtandPrintBackend extends GPrintBackend.Stub{
    private GPrintBackendService mBackendService;
    public GPrintExtandPrintBackend(GPrintBackendService gPrintBackendService) {
        // TODO Auto-generated constructor stub
        mBackendService = gPrintBackendService;
    }

	@Override
    public void searchUsbPrinter(GPrintUsbResponse callback) throws RemoteException {
        // TODO Auto-generated method stub
        mBackendService.searchUsbPrinter(callback);
        
    }

	 @Override
    public int print(int jobId, GPrintJob job, GPrintJobStatus response)
            throws RemoteException {
        // TODO Auto-generated method stub
        return mBackendService.print(jobId,job,response);
    }

	@Override
    public int cancel() throws RemoteException {
        // TODO Auto-generated method stub
        return mBackendService.cancel();
    }

}
