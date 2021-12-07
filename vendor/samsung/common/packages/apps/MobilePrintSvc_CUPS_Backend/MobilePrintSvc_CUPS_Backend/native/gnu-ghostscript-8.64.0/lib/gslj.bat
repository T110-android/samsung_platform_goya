@echo off
@rem $Id: gslj.bat,v 1.3 2007/05/07 11:22:07 Arabidopsis Exp $

call gssetgs.bat
%GSC% -q -sDEVICE=laserjet -r300 -dNOPAUSE -sPROGNAME=gslj -- gslp.ps %1 %2 %3 %4 %5 %6 %7 %8 %9
