package org.cups4j.operations.cups;

/**
 * Copyright (C) 2009 Harald Weyhing
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * See the GNU Lesser General Public License for more details. You should have received a copy of
 * the GNU Lesser General Public License along with this program; if not, see
 * <http://www.gnu.org/licenses/>.
 */
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;

import org.cups4j.CupsPrinter;
import org.cups4j.operations.IppOperation;

import android.util.Log;

import ch.ethz.vppserver.ippclient.IppResult;
import ch.ethz.vppserver.schema.ippclient.Attribute;
import ch.ethz.vppserver.schema.ippclient.AttributeGroup;

public class CupsGetPrintersOperation extends IppOperation {

  public CupsGetPrintersOperation() {
    operationID = 0x4002;
    bufferSize = 8192;
  }

  public CupsGetPrintersOperation(int port) {
    this();
    this.ippPort = port;
  }

  public ArrayList<CupsPrinter> getPrinters(String hostname, int port) throws Exception {
    ArrayList<CupsPrinter> printers = new ArrayList<CupsPrinter>();

    HashMap<String, String> map = new HashMap<String, String>();
    map
        .put(
            "requested-attributes",
            "copies-supported page-ranges-supported printer-name printer-info printer-location printer-make-and-model printer-uri-supported");
    // map.put("requested-attributes", "all");
    CupsGetPrintersOperation command = new CupsGetPrintersOperation(port);

    IppResult result = command.request(new URL("http://" + hostname + "/printers"), map);

    // IppResultPrinter.print(result);

    for (AttributeGroup group : result.getAttributeGroupList()) {
      CupsPrinter printer = null;
      if (group.getTagName().equals("printer-attributes-tag")) {
        String printerURI = null;
        String printerName = null;
        String modelName = null;
        String printerLocation = null;
        String printerDescription = null;
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
        URL printerUrl = null;
        try {
          printerUrl = new URL(printerURI);
        } catch (Throwable t) {
          t.printStackTrace();
          Log.d("cups4j", "Error encountered building URL from printer uri of printer " + printerName
              + ", uri returned was [" + printerURI + "].  Attribute group tag/description: [" + group.getTagName()
              + "/" + group.getDescription());
          throw new Exception(t);
        }
        printer = new CupsPrinter(printerUrl, printerName, false);
        printer.setLocation(printerLocation);
        printer.setDescription(printerDescription);
        printer.setModelName(modelName);
        printers.add(printer);
      }
    }

    return printers;
  }
}
