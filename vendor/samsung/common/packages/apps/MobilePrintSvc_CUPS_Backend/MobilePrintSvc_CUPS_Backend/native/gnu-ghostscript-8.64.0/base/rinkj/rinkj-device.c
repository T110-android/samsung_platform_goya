/* Copyright (C) 2001-2006 Artifex Software, Inc.
   All Rights Reserved.
  
  This file is part of GNU ghostscript

  GNU ghostscript is free software; you can redistribute it and/or
  modify it under the terms of the version 2 of the GNU General Public
  License as published by the Free Software Foundation.

  GNU ghostscript is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  ghostscript; see the file COPYING. If not, write to the Free Software Foundation,
  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/

/*$Id: rinkj-device.c,v 1.4 2007/09/11 15:25:10 Arabidopsis Exp $ */
/* The device abstraction within the Rinkj driver. */

#include "rinkj-device.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Deprecated */
int
rinkj_device_set (RinkjDevice *self, const char *config)
{
  if (self->init_happened != 0)
    return -1;
  return self->set (self, config);
}

/* Preferred, as it matches IJS */
int
rinkj_device_set_param (RinkjDevice *self, const char *key,
			const char *value, int value_size)
{
  int keylen = strlen (key);
  int bufsize = keylen + value_size + 3;
  char *buf = malloc (bufsize);
  int status;

  /* This implementation is in terms of device_set, but we're going to
     change the prototype of the device so this is native. */
  memcpy (buf, key, keylen);
  memcpy (buf + keylen, ": ", 2);
  memcpy (buf + keylen + 2, value, value_size);
  buf[keylen + 2 + value_size] = 0;
  status = rinkj_device_set (self, buf);
  free (buf);
  return status;
}

int
rinkj_device_set_param_string (RinkjDevice *self, const char *key,
			       const char *value)
{
  return rinkj_device_set_param (self, key, value, strlen (value));
}

int
rinkj_device_set_param_int (RinkjDevice *self, const char *key, int value)
{
  char buf[32];
  int value_size = sprintf (buf, "%d", value);
  return rinkj_device_set_param (self, key, buf, value_size);
}

int
rinkj_device_init (RinkjDevice *self, const RinkjDeviceParams *params)
{
  int status;

  if (self->init_happened != 0)
    return -1;
  status = self->init (self, params);
  self->init_happened = 42;
  return status;
}

int
rinkj_device_write (RinkjDevice *self, const char **data)
{
  if (self->init_happened != 42)
    return -1;
  return self->write (self, data);
}
