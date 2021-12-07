@echo off
@rem $Id: lpgs.bat,v 1.3 2007/05/07 11:22:07 Arabidopsis Exp $

call gssetgs.bat
%GSC% -sDEVICE#djet500 -dNOPAUSE -sPROGNAME=lpgs -- gslp.ps -fCourier9 %1 %2 %3 %4 %5 %6 %7 %8 %9
