@echo off
@rem $Id: gssetgs.bat,v 1.3 2007/05/07 11:22:07 Arabidopsis Exp $

rem Set default values for GS (gs with graphics window) and GSC
rem (console mode gs) if the user hasn't set them.

if %GS%/==/ set GS=gswin32
if %GSC%/==/ set GSC=gswin32c
