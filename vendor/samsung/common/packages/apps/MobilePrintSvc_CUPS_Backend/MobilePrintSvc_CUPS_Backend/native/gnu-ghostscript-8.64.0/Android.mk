LOCAL_PATH:= $(call my-dir)

##################################################
# Build "gs" Interpreter for Android

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := eng

gs_src_files := \
		obj/gsromfs1.c psi/gs.c base/gp_getnv.c base/gp_upapr.c base/gp_unix.c base/gp_unifs.c base/gp_unifn.c base/gp_stdia.c base/gp_unix_cache.c base/smd5.c base/md5.c psi/iscanbin.c psi/zbseq.c \
		base/gscolor1.c base/gsht1.c base/gxicolor.c psi/zcolor1.c psi/zht1.c psi/zupath.c psi/ibnum.c base/gdevhit.c base/gsdps1.c psi/zcharx.c psi/zvmem2.c  psi/zdps1.c psi/zcid.c psi/zfcid.c \
		psi/zchar1.c psi/zcharout.c psi/zfont1.c psi/zmisc1.c base/seexec.c  base/gscrypt1.c psi/iutil2.c psi/zmisc2.c psi/zusparam.c psi/zchar2.c psi/zfont2.c base/gstype1.c \
		base/gxtype1.c base/gxhintn.c base/gxhintn1.c base/gxchrout.c base/gstype2.c psi/zfrsd.c psi/zcie.c psi/zcrd.c base/gscie.c base/gsciemap.c base/gscscie.c base/gscrd.c base/gscrdp.c base/gxctable.c psi/zfcmap.c \
		base/gsfcmap.c base/gsfcmap1.c base/gsfcid.c base/gsfcid2.c base/gschar0.c base/gsfont0.c psi/zcfont.c psi/zfont0.c base/sdctc.c base/sjpegc.c base/sdcte.c  base/sjpege.c base/sdeparam.c \
		base/sdcparam.c psi/zfdcte.c base/sdctd.c  base/sjpegd.c  base/sddparam.c psi/zfdctd.c base/scantab.c base/scfparam.c base/sfilter2.c psi/zfdecode.c base/scfd.c base/scfdtab.c base/slzwd.c \
		base/slzwc.c base/spdiff.c base/spngp.c base/srld.c psi/zfilter2.c base/scfe.c base/scfetab.c base/shc.c base/slzwe.c base/srle.c psi/ziodev2.c psi/zdevcal.c psi/zdevice2.c psi/zmedia2.c \
		base/gxclipm.c base/gximage3.c base/gximage4.c base/gxmclip.c base/gscolor2.c base/gspcolor.c base/gsptype1.c base/gxclip2.c base/gxp1fill.c base/gxpcmap.c psi/zpcolor.c psi/imain.c \
		psi/imainarg.c base/gsargs.c psi/idisp.c obj/gconfig.c obj/gscdefs.c obj/iconfig.c psi/iapi.c psi/icontext.c psi/idebug.c psi/idict.c psi/idparam.c psi/idstack.c psi/iinit.c psi/interp.c \
		psi/iparam.c psi/ireclaim.c psi/iplugin.c psi/iscan.c psi/iscannum.c psi/istack.c psi/iutil.c base/sa85d.c psi/sfilter1.c base/sstring.c base/stream.c psi/zarith.c psi/zarray.c psi/zcontrol.c \
		psi/zdict.c psi/zfile.c psi/zfile1.c psi/zfileio.c psi/zfilter.c psi/zfproc.c psi/zgeneric.c psi/ziodev.c psi/ziodevsc.c psi/zmath.c psi/zalg.c psi/zmisc.c psi/zpacked.c  psi/zrelbit.c  psi/zstack.c \
		psi/zstring.c psi/zsysvm.c psi/ztoken.c psi/ztype.c psi/zvmem.c psi/zbfont.c psi/zchar.c psi/zcolor.c psi/zdevice.c  psi/zfont.c  psi/zfontenum.c psi/zgstate.c  psi/zdfilter.c psi/zht.c  psi/zimage.c \
		psi/zmatrix.c psi/zpaint.c psi/zpath.c psi/zncdummy.c psi/ialloc.c psi/igc.c psi/igcref.c psi/igcstr.c  psi/ilocate.c psi/iname.c psi/isave.c  base/sfxstdio.c base/sfxcommon.c psi/zdscpars.c \
		psi/dscparse.c  base/sbcp.c psi/zfbcp.c psi/zpath1.c base/gspath1.c psi/zcolor2.c psi/zcsindex.c psi/zht2.c  psi/zimage2.c psi/zcssepr.c psi/zfsample.c psi/zfunc4.c psi/zfunc.c  psi/zfunc0.c base/gsdsrc.c \
		base/gsfunc.c base/gsfunc0.c base/gsfunc4.c base/spprint.c base/gscsepr.c base/gsnamecl.c base/gsncdummy.c psi/zchar32.c psi/zfont32.c psi/zfimscale.c base/simscale.c psi/zcspixel.c base/gscpixel.c \
		base/gsclipsr.c base/gscdevn.c base/gxdevndi.c base/gscolor3.c base/gsfunc3.c base/gsptype2.c base/gsshade.c base/gxshade.c base/gxshade1.c base/gxshade4.c base/gxshade6.c  psi/zfunc3.c psi/zimage3.c \
		psi/zmisc3.c psi/zcolor3.c psi/zshade.c base/szlibc.c base/szlibe.c base/szlibd.c psi/zfzlib.c psi/ztrans.c base/gstrans.c base/gximag3x.c base/gxblend.c base/gxblend1.c base/gdevp14.c base/gdevdevn.c \
		base/gdevdcrd.c base/gdevbbox.c psi/zfmd5.c psi/zfarc4.c base/sarc4.c psi/zfaes.c base/saes.c base/aes.c \
		base/gsdps.c base/gximage2.c psi/zdps.c psi/zcontext.c base/gsalphac.c psi/zdpnext.c base/gdevpipe.c \
		base/gsnogc.c base/gshtx.c base/gdevprn.c base/gxclist.c base/gxclbits.c  base/gxclpage.c base/gxclrast.c base/gxclread.c base/gxclrect.c  base/gxclutil.c base/gsparams.c base/gsroptab.c \
		base/gxclimag.c base/gxclpath.c base/gxdhtserial.c base/gxclthrd.c base/gsmchunk.c base/gsmemlok.c base/gxclfile.c  base/gxclmem.c base/gxclzlib.c base/gp_psync.c base/gdevpbm.c \
		base/gdevppla.c base/gdevmpla.c base/gdevpcx.c base/gdevpccm.c base/gdevp2up.c base/gdevpdf.c base/gdevpdfb.c base/gdevpdfc.c base/gdevpdfd.c base/gdevpdfe.c base/gdevpdfg.c base/gdevpdfi.c base/gdevpdfj.c \
		base/gdevpdfk.c base/gdevpdfm.c base/gdevpdfo.c base/gdevpdfp.c base/gdevpdft.c base/gdevpdfr.c base/gdevpdfu.c base/gdevpdfv.c base/ConvertUTF.c base/gsflip.c base/gdevpsdi.c base/gdevpsdp.c \
		base/gdevpsds.c base/gdevpsdu.c base/spsdf.c base/gsparamx.c base/gdevvec.c base/gdevpdt.c base/gdevpdtb.c base/gdevpdtc.c base/gdevpdtd.c base/gdevpdte.c base/gdevpdtf.c base/gdevpdti.c base/gdevpdts.c \
		base/gdevpdtt.c base/gdevpdtv.c base/gdevpdtw.c base/gsfont0c.c base/gxfcopy.c base/gdevpsf1.c base/gdevpsf2.c base/gdevpsfm.c base/gdevpsft.c base/gdevpsfu.c base/gdevpsfx.c base/gdevpsim.c \
		base/gdevpsu.c base/gdevps.c base/gdevpx.c base/gdevpxut.c base/gdevpcl.c contrib/gdevgdi.c  \
		base/gdevpng.c base/gsiorom.c base/gpmisc.c  base/strmio.c  base/gsalloc.c base/gsalpha.c \
		base/gsbitcom.c base/gsbitops.c base/gsbittab.c base/gscedata.c base/gscencs.c base/gschar.c base/gscolor.c base/gscoord.c base/gscparam.c base/gscspace.c base/gscicach.c base/gsovrc.c base/gxoprect.c\
		base/gsdevice.c base/gsdevmem.c base/gsdparam.c  base/gsdfilt.c base/gsfname.c base/gsfont.c base/gsgdata.c base/gsgcache.c base/gsht.c base/gshtscr.c base/gswts.c base/gsimage.c base/gsimpath.c \
		base/gsinit.c base/gsiodev.c base/gsistate.c base/gsline.c base/gsmalloc.c base/gsmatrix.c base/gsmemory.c base/gsmemret.c base/gsmisc.c base/gsnotify.c base/gslibctx.c base/gspaint.c \
		base/gsparam.c base/gspath.c base/gsserial.c base/gsstate.c base/gstext.c  base/gsutil.c base/gxacpath.c base/gxbcache.c base/gxccache.c base/gxccman.c base/gxchar.c base/gxcht.c base/gxclip.c \
		base/gxcmap.c base/gxcpath.c base/gxdcconv.c base/gxdcolor.c base/gxhldevc.c base/gxfill.c base/gxfdrop.c base/gxht.c base/gxhtbit.c base/gxwts.c base/gxidata.c base/gxifast.c \
		base/gximage.c base/gximage1.c base/gximono.c base/gxipixel.c base/gximask.c base/gxi12bit.c base/gxi16bit.c base/gxiscale.c base/gxpaint.c base/gxpath.c base/gxpath2.c base/gxpcopy.c base/gxpdash.c\
		base/gxpflat.c base/gxsample.c base/gxstroke.c base/gxsync.c base/vdtrace.c  base/gdevabuf.c base/gdevdbit.c base/gdevddrw.c base/gdevdflt.c base/gdevdgbr.c base/gdevnfwd.c base/gdevmem.c \
		base/gdevplnx.c base/gdevm1.c base/gdevm2.c base/gdevm4.c base/gdevm8.c base/gdevm16.c base/gdevm24.c base/gdevm32.c base/gdevm40.c base/gdevm48.c base/gdevm56.c base/gdevm64.c base/gdevdsha.c \
		base/siinterp.c base/siscale.c base/sidscale.c base/gdevdrop.c base/gdevmr1.c base/gdevmr2n.c base/gdevmr8n.c base/gdevrops.c base/gsrop.c base/gp_strdl.c \
		psi/zicc.c base/gsicc.c icclib/icc.c

LOCAL_SRC_FILES := $(gs_src_files)

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/base \
		$(LOCAL_PATH)/obj \
		$(LOCAL_PATH)/icclib \
		$(LOCAL_PATH)/psi \
		external/jpeg \
		external/libpng \
		external/zlib \
		

LOCAL_CFLAGS :=  -w -DPLATFORM_ANDROID -DHAVE_MKSTEMP -DHAVE_SETLOCALE -DHAVE_SSE2  -DHAVE_BSWAP32 -DHAVE_BYTESWAP_H -O2 -Wall -Wstrict-prototypes -Wundef -Wmissing-declarations -Wmissing-prototypes -Wwrite-strings -Wno-strict-aliasing -Wdeclaration-after-statement -fno-builtin -fno-common -DHAVE_STDINT_H -DGX_COLOR_INDEX_TYPE="unsigned long int"

LOCAL_SHARED_LIBRARIES := \
			 liblog \
			 libjpeg \
			 libcupspng \
			 libdl \
			 libz

LOCAL_LDLIBS := -lz -lthread_db -lm -llog

LOCAL_PRELINK_MODULE := false

#LOCAL_MODULE := gs
LOCAL_MODULE := libcupsgs

#include $(BUILD_EXECUTABLE)
include $(BUILD_SHARED_LIBRARY)
