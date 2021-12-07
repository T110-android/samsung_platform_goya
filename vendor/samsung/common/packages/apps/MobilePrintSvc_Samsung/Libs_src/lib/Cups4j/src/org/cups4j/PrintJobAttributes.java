package org.cups4j;

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

/**
 * Holds print job attributes
 * 
 * 
 */
public class PrintJobAttributes {
  URL jobURL = null;
  URL printerURL = null;
  int jobID = -1;
  JobStateEnum jobState = null;
  String jobName = null;
  String userName = null;

  public URL getJobURL() {
    return jobURL;
  }

  public void setJobURL(URL jobURL) {
    this.jobURL = jobURL;
  }

  public URL getPrinterURL() {
    return printerURL;
  }

  public void setPrinterURL(URL printerURL) {
    this.printerURL = printerURL;
  }

  public int getJobID() {
    return jobID;
  }

  public void setJobID(int jobID) {
    this.jobID = jobID;
  }

  public JobStateEnum getJobState() {
    return jobState;
  }

  public void setJobState(JobStateEnum jobState) {
    this.jobState = jobState;
  }

  public String getJobName() {
    return jobName;
  }

  public void setJobName(String jobName) {
    this.jobName = jobName;
  }

  public String getUserName() {
    return userName;
  }

  public void setUserName(String userName) {
    this.userName = userName;
  }
}
