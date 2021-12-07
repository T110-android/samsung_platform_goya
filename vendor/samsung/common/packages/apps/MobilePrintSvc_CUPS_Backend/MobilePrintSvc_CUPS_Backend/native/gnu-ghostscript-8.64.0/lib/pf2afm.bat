@echo off 
@rem $Id: pf2afm.bat,v 1.3 2007/05/07 11:22:07 Arabidopsis Exp $
@rem Make an AFM file from PFB / PFA and (optionally) PFM files.

if %1/==/ goto usage
if not %2/==/ goto usage
call gssetgs.bat

%GSC% -q -dNODISPLAY -dSAFER -dDELAYSAFER -- pf2afm.ps %1
goto end

:usage
echo "Usage: pf2afm disk_font_name"

:end


