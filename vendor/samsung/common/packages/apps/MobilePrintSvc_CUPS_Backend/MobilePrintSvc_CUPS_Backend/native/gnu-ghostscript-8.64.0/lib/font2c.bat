@echo off
@rem $Id: font2c.bat,v 1.3 2007/05/07 11:22:07 Arabidopsis Exp $

call gssetgs.bat
%GSC% -q -dNODISPLAY -dWRITESYSTEMDICT -- font2c.ps %1 %2 %3 %4 %5 %6 %7 %8 %9
