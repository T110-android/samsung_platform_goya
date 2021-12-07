/*
 *  Copyright (C) 2012 Marvell, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _PLUGIN_CODA_H
#define _PLUGIN_CODA_H

#define MAX_DATA_LEN	128
#define CODA_UPDATE 1
#define CODA_STOP	2

enum video_resolution {
    VGA,
    HD720P,
    HD1080P,
};

enum video_format {
    STRM_FMT_MPG1             = 0,
    STRM_FMT_MPG2             = 1,
    STRM_FMT_MPG4             = 2,
    STRM_FMT_H261             = 3,
    STRM_FMT_H263             = 4,
    STRM_FMT_H264             = 5,
    STRM_FMT_VC1              = 6,            /*vc-1 AP*/
    STRM_FMT_JPEG             = 7,
    STRM_FMT_MJPG             = 8,
    STRM_FMT_VC1M             = 10,           /*vc-1 SP&MP*/
    STRM_FMT_VP8              = 11,
    STRM_FMT_DIVX3            = 12,           /*divx3.11*/
    STRM_FMT_DIVX             = 13,           /*divx4,5,6*/
    STRM_FMT_RV30             = 14,           /*real video 8*/
    STRM_FMT_RV40             = 15,           /*real video 9,10*/
    STRM_FMT_SORENSON         = 16,           /*sorenson spark*/
};

struct coda_video_profile {
    int codec_type;     //0: dec, 1: enc
    int resolution;     //0: vga, 1:720p, 2:1080p
    int fps;            //only valid for encoder currently, just set it as 0 for decoder
    int strm_fmt;       //stream format, not used currently, just set it as 0
    int sideinfo;       //only for debug purpose currently, just set it as 0
};

struct coda_info {
    unsigned long long tid;
    int event;  /* 0:start, 1:update, 2:stop */
    struct coda_video_profile profile;
};

#endif
