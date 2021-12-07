package org.cups4j.operations.ipp;

/**
 * Copyright (C) 2009 Harald Weyhing
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.
 * 
 * See the GNU Lesser General Public License for more details. You should have
 * received a copy of the GNU Lesser General Public License along with this
 * program; if not, see <http://www.gnu.org/licenses/>.
 */
import java.io.UnsupportedEncodingException;
import java.net.URL;
import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;

import org.cups4j.CupsPrinter;
import org.cups4j.operations.IppOperation;

import android.util.Log;

import ch.ethz.vppserver.ippclient.IppResult;
import ch.ethz.vppserver.ippclient.IppTag;
import ch.ethz.vppserver.schema.ippclient.Attribute;
import ch.ethz.vppserver.schema.ippclient.AttributeGroup;

public class IppGetPrinterAttributesOperation extends IppOperation {

  public IppGetPrinterAttributesOperation() {
    operationID = 0x000b;
    bufferSize = 8192;
  }

  /**
   * 
   * @param url
   *          printer-uri
   * @return IPP header
   * @throws UnsupportedEncodingException
   */
  public ByteBuffer getIppHeader(String url) throws UnsupportedEncodingException {
    return getIppHeader(url, null);
  }

  /**
   * @param url
   *          printer-uri
   * @param map
   *          attributes i.e.
   *          requesting-user-name,requested-attributes,document-format
   * @return IPP header
   * @throws UnsupportedEncodingException
   */
  public ByteBuffer getIppHeader(String url, Map<String, String> map) throws UnsupportedEncodingException {
    ByteBuffer ippBuf = ByteBuffer.allocateDirect(bufferSize);

    ippBuf = IppTag.getOperation(ippBuf, operationID);
    ippBuf = IppTag.getUri(ippBuf, "printer-uri", url);

    if (map == null) {
      ippBuf = IppTag.getKeyword(ippBuf, "requested-attributes", "all");
      ippBuf = IppTag.getEnd(ippBuf);
      ippBuf.flip();
      return ippBuf;
    }

    ippBuf = IppTag.getNameWithoutLanguage(ippBuf, "requesting-user-name", map.get("requesting-user-name"));
    if (map.get("requested-attributes") != null) {
      String[] sta = map.get("requested-attributes").split(" ");
      if (sta != null) {
        ippBuf = IppTag.getKeyword(ippBuf, "requested-attributes", sta[0]);
        int l = sta.length;
        for (int i = 1; i < l; i++) {
          ippBuf = IppTag.getKeyword(ippBuf, null, sta[i]);
        }
      }
    }

    ippBuf = IppTag.getNameWithoutLanguage(ippBuf, "document-format", map.get("document-format"));

    ippBuf = IppTag.getEnd(ippBuf);
    ippBuf.flip();
    return ippBuf;
  }
  
  public CupsPrinter getPrinter(String hostname, int port) throws Exception {
	  HashMap<String, String> map = new HashMap<String, String>();
	    map
	        .put(
	            "requested-attributes",
	            "copies-supported page-ranges-supported printer-name printer-info printer-location printer-make-and-model printer-uri-supported");

	    IppResult result = request(new URL("http://" + hostname), map);

	    String printerURI = null;
        String printerName = null;
        String modelName = null;
        String printerLocation = null;
        String printerDescription = null;
        URL printerUrl = null;
        
	    for (AttributeGroup group : result.getAttributeGroupList()) {
	        if (group.getTagName().equals("printer-attributes-tag")) {
	          for (Attribute attr : group.getAttribute()) {
	            if (attr.getName().equals("printer-uri-supported")) {
	              printerURI = attr.getAttributeValue().get(0).getValue().replace("ipp://", "http://");
	            } else if (attr.getName().equals("printer-name")) {
	              printerName = attr.getAttributeValue().get(0).getValue();
	            } else if (attr.getName().equals("printer-location")) {
	              if (attr.getAttributeValue() != null && attr.getAttributeValue().size() > 0)
	                printerLocation = attr.getAttributeValue().get(0).getValue();
	            } else if (attr.getName().equals("printer-info")) {
	              if (attr.getAttributeValue() != null && attr.getAttributeValue().size() > 0) {
	                printerDescription = attr.getAttributeValue().get(0).getValue();
	              }
	            } else if (attr.getName().equals("printer-make-and-model")) {
	                if (attr.getAttributeValue() != null && attr.getAttributeValue().size() > 0) {
	              	  modelName = attr.getAttributeValue().get(0).getValue();
	                  }
	                }
	          }
	          try {
	            printerUrl = new URL(printerURI);
	          } catch (Throwable t) {
	            t.printStackTrace();
	            Log.d("cups4j", "Error encountered building URL from printer uri of printer " + printerName
	                + ", uri returned was [" + printerURI + "].  Attribute group tag/description: [" + group.getTagName()
	                + "/" + group.getDescription());
	            throw new Exception(t);
	          }
	        }
	    }
	    CupsPrinter printer = null;
        printer = new CupsPrinter(printerUrl, printerName, false);
        printer.setLocation(printerLocation);
        printer.setDescription(printerDescription);
        printer.setModelName(modelName);
        return printer;
  }
}
