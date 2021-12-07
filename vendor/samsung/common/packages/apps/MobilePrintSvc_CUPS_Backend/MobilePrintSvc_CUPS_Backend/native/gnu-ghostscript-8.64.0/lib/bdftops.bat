@echo off
@rem $Id: bdftops.bat,v 1.3 2007/05/07 11:22:07 Arabidopsis Exp $

call gssetgs.bat
%GSC% -q -dBATCH -dNODISPLAY -- bdftops.ps %1 %2 %3 %4 %5 %6 %7 %8 %9
