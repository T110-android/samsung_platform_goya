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


import android.util.Log;

import java.util.ArrayList;
import java.util.List;

public class GPrintPsToPcl {

    static {
        try {
            System.loadLibrary("cupspng");
            System.loadLibrary("cupsgs");
            System.loadLibrary("cupsinvgs");
        } catch (Exception ex) {
            Log.e(GPrintCommon.TAG, "GPrintPsToPcl : Cannot load library" + ex.toString());
        }
    }

    private native int InvokeGhostScript(String[] lpCommand);

    // private String TAG="MobilePrintServer";
    private String gsDriver = null;

    public GPrintPsToPcl() {

    }
    
    
    public int psToPclXl(String psFilePath, String pclFilePath, String paperSize, String color,
            String duplex) {
        int resolution = 150;
        //int resolution = 600;
        if (GPrintCommon.color.equalsIgnoreCase(color))
            gsDriver = GPrintCommon.pxlcolor_device;
        else
            gsDriver = GPrintCommon.pxlmono_device;

        List<String> pclCommand = new ArrayList<String>();
        pclCommand.clear();

        // //gs -sDEVICE=pxlcolor -dNOPAUSE -dBATCH -dSAFER
        // -sOutputFile=/mnt/sdcard/psTopclFile /mnt/sdcard/imgtopsFile -c quit
        pclCommand.add("gs");
        pclCommand.add("-sDEVICE=" + gsDriver);
        if(paperSize.equalsIgnoreCase("JISB5"))
        	pclCommand.add("-sPAPERSIZE=b5");
        else
        	pclCommand.add("-sPAPERSIZE=" + paperSize.toLowerCase());
        pclCommand.add("-r"+resolution); //for testing
        //pclCommand.add("-r300");
        //pclCommand.add("-r72");
        //pclCommand.add("-r150");
        if (!GPrintCommon.simplex.equalsIgnoreCase(duplex))
            pclCommand.add("-sDuplexCapability=both");

        if (!GPrintCommon.simplex.equalsIgnoreCase(duplex)) {
            pclCommand.add("-dDuplex=true");
            if (GPrintCommon.shortedge.equalsIgnoreCase(duplex))
                pclCommand.add("-dTumble=true"); // Short edge flip
            else if (GPrintCommon.longedge.equalsIgnoreCase(duplex))
                pclCommand.add("-dTumble=false"); // Long edge flip
        }
        //pclCommand.add("-sICCProfilesDir=/mnt/sdcard/");
        pclCommand.add("-dCOLORSCREEN=false");
        //pclCommand.add("-dGraphicsAlphaBits=1");
       // pclCommand.add("-dDITHERPPI="+(resolution/5));
        pclCommand.add("-dUseCIEColor");
        pclCommand.add("-dNOPSICC");
        pclCommand.add("-dNOPAUSE");
        pclCommand.add("-dBATCH");
        pclCommand.add("-dSAFER");
        pclCommand.add("-dNOINTERPOLATE");
        pclCommand.add("-sOutputFile=" + pclFilePath);
        pclCommand.add(psFilePath);
        // // pclCommand.add("-c");
        // // pclCommand.add("quit");

        String[] linesArr2 = pclCommand.toArray(new String[pclCommand.size()]);
        int ret = InvokeGhostScript(linesArr2);
        Log.d(GPrintCommon.TAG, "GPrintPsToPcl : psToPclXl : InvokeGhostScript finished with ret="
                + ret);

        return ret;
    }
   

}
