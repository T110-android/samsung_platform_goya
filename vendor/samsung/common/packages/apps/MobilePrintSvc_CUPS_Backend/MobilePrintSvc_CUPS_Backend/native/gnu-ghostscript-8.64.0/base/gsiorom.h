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

/* $Id: gsiorom.h,v 1.4 2007/09/11 15:23:44 Arabidopsis Exp $ */
/* Rom File System settings */

#ifndef gsiorom_INCLUDED
#  define gsiorom_INCLUDED

#define ROMFS_BLOCKSIZE 16384
#define ROMFS_CBUFSIZE ((int)((ROMFS_BLOCKSIZE) * 1.001) + 12)

/* enble the ROMFS_COMPRESSION as optional
  #define ROMFS_COMPRESSION
*/

/*
 * in memory structure is:
 *
 *	offset_to_next_inode (total length of this inode)	[32-bit big-endian]
 *	length_of_uncompressed_file				[32-bit big-endian]
 *	data_block_struct[]		count is (length+ROMFS_BLOCKSIZE-1)/ROMFS_BLOCKSIZE
 *	padded_file_name (char *)	includes as least one terminating <nul>
 *	padded_data_blocks
 */
/*
 *	data_block_struct:
 *	    data_length			(not including pad)	[32-bit big-endian]
 *	    data_block_offset		(start of each block)	[32-bit big-endian]
 */

#endif /* gsiorom_INCLUDED */
