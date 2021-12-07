@echo off
@rem $Id: ps2ascii.bat,v 1.3 2007/05/07 11:22:07 Arabidopsis Exp $

call gssetgs.bat
if '%1'=='' goto a0
if '%2'=='' goto a1
%GSC% -q -dNODISPLAY -dSAFER -dDELAYBIND -dWRITESYSTEMDICT -dSIMPLE ps2ascii.ps %1 -c quit >%2
goto x
:a0
%GSC% -q -dNODISPLAY -dSAFER -dDELAYBIND -dWRITESYSTEMDICT -dSIMPLE ps2ascii.ps - -c quit
goto x
:a1
%GSC% -q -dNODISPLAY -dSAFER -dDELAYBIND -dWRITESYSTEMDICT -dSIMPLE ps2ascii.ps %1 -c quit
goto x
:x
