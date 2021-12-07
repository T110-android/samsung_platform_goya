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
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.RectF;
import android.util.Log;

public class GPrintImageManager {
	
	
	public static String rotateImage(String inFilePath,String internalPath,int angle)
	{
	    Log.d(GPrintCommon.TAG,"GPrintImageManager : rotateImage");
		try{
			FileInputStream inStream = null;
		    FileOutputStream outStream = null;
		    String inFileName = inFilePath.substring(inFilePath.lastIndexOf(File.separator)+1);
		    Log.d(GPrintCommon.TAG,"GPrintImageManager : rotateImage : inFileName"+inFileName);
		    String outFilePath = internalPath + File.separator + inFileName;
		    BitmapFactory.Options options = new BitmapFactory.Options();
		    Matrix matrix = new Matrix();
		    matrix.postRotate(angle);
		    for (options.inSampleSize = 1; options.inSampleSize <= 32; options.inSampleSize++)
		    {
		    	try
		        {
		    		inStream = new FileInputStream(inFilePath);
		    		Bitmap originalBitmap = BitmapFactory.decodeStream(inStream, null, options);
	 
		    		// Rotate the bitmap
		    		Bitmap rotatedBitmap = Bitmap.createBitmap(originalBitmap, 0, 0, originalBitmap.getWidth(), originalBitmap.getHeight(), matrix, true);
	 
		    		// Save the rotated bitmap
		    		outStream = new FileOutputStream(outFilePath);
		    		rotatedBitmap.compress(Bitmap.CompressFormat.JPEG, 100, outStream);
		    		outStream.close();
	 
		    		// Recycle the bitmaps to immediately free memory
		    		originalBitmap.recycle();
		    		originalBitmap = null;
		    		rotatedBitmap.recycle();
		    		rotatedBitmap = null;
		    		Log.d(GPrintCommon.TAG,"GPrintImageManager : rotateImage : outFilePath="+outFilePath);
		    		// Return
		            return outFilePath;
		        }catch (OutOfMemoryError e)
		        {
		            // If an OutOfMemoryError occurred, we continue with for loop and next inSampleSize value
		        }finally
		        {
		            if (outStream != null)
		            {
		                try
		                {
		                    outStream.close();
		                }
		                catch (IOException e)
		                {
		                }
		            }
		        }

		    }
		    
		}catch(Exception ex){
			
		}
	     
	    // Failed
	    return null;
	}
	
	public static String changeResolution(String mInFilePath,String internalPath,int mOutWidth,int mOutHeight){
		try{
			
			
			FileInputStream inStream = new FileInputStream(mInFilePath);
			BitmapFactory.Options options = new BitmapFactory.Options();
			//options.inSampleSize = 1;
			options.inJustDecodeBounds = true;
			BitmapFactory.decodeStream(inStream, null, options);
			inStream.close();
			inStream = null;

			// save width and height
			int mOrigWidth = options.outWidth;
			int mOrigHeight = options.outHeight;
	    			
			Log.d(GPrintCommon.TAG,"GPrintImageManager : changeResolution : mOrigWidth :"+mOrigWidth);
			Log.d(GPrintCommon.TAG,"GPrintImageManager : changeResolution : mOrigHeight :"+mOrigHeight);
		
			if((mOrigWidth > mOutWidth) && (mOrigHeight > mOutHeight)){
			
				int mWidthScale = (int)Math.ceil((double) (mOrigWidth / mOutWidth));
				int mHeightScale = (int)Math.ceil((double)(mOrigHeight / mOutHeight));
			
				Log.d(GPrintCommon.TAG,"GPrintImageManager : changeResolution : mWidthScale :"+mWidthScale);
				Log.d(GPrintCommon.TAG,"GPrintImageManager : changeResolution : mHeightScale :"+mHeightScale);
			
				int mInSampleSize = (mWidthScale > mHeightScale) ? mWidthScale : mHeightScale;
				if(mInSampleSize < 1)
					mInSampleSize = 1;
            
				inStream = new FileInputStream(mInFilePath);
				options = new BitmapFactory.Options();
            	options.inSampleSize = mInSampleSize;
            	// decode full image
            	Bitmap originalBitmap = BitmapFactory.decodeStream(inStream, null, options);

            	// calc exact destination size
            	Matrix mTx = new Matrix();
            	RectF inRect = new RectF(0, 0, originalBitmap.getWidth(), originalBitmap.getHeight());
            	RectF outRect = new RectF(0, 0, mOutWidth, mOutHeight);
            	//mTx.setRectToRect(inRect, outRect, Matrix.ScaleToFit.CENTER);
            	mTx.setRectToRect(inRect, outRect, Matrix.ScaleToFit.FILL);
            	float[] values = new float[9];
            	mTx.getValues(values);
            	// resize bitmap
            	Bitmap resizedBitmap = Bitmap.createScaledBitmap(originalBitmap, (int) (originalBitmap.getWidth() * values[0]), (int) (originalBitmap.getHeight() * values[4]), true);
            	//Bitmap resizedBitmap = Bitmap.createScaledBitmap(originalBitmap,mOutWidth , mOutHeight, true);

            	// save image
            	try
            	{
            		String inFileName = mInFilePath.substring(mInFilePath.lastIndexOf(File.separator)+1);
    				String mOutFilePath = internalPath + File.separator + inFileName;
    				
            		FileOutputStream out = new FileOutputStream(mOutFilePath);
            		resizedBitmap.compress(Bitmap.CompressFormat.JPEG,90, out);
            		Log.d(GPrintCommon.TAG,"GPrintImageManager : changeResolution : mOutFilePath :"+mOutFilePath);
            		return mOutFilePath;
            	}
            	catch (Exception e)
            	{
            		Log.e("Image", e.getMessage(), e);
            	}
			}
		} catch (FileNotFoundException e) {
		// TODO Auto-generated catch block
			clearDir(internalPath);
		e.printStackTrace();
		} catch (IOException e) {
		// TODO Auto-generated catch block
			clearDir(internalPath);
		e.printStackTrace();
		}catch (Exception e) {
		// TODO Auto-generated catch block
			clearDir(internalPath);
		e.printStackTrace();
		}

		return null;
	}
	
	public static void clearDir(String dirName) {
        if(dirName != null) {
            File parentDir = new File(dirName);
            if(parentDir != null && parentDir.exists()) {
                if(parentDir.isDirectory() == true) {
                    File[] childFiles = parentDir.listFiles();
                    for(int i = 0; i < childFiles.length; i++) {
                         if (childFiles[i].exists()) {
                        	 	childFiles[i].delete();
                         }
                    }
                }
                
            }
        }
    }
	
	
}
