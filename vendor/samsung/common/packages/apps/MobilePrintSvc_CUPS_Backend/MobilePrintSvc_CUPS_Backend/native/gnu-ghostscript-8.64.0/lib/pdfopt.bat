@echo off 
@rem $Id: pdfopt.bat,v 1.4 2007/05/07 11:22:07 Arabidopsis Exp $
@rem Convert PDF to "optimized" form.

if %1/==/ goto usage
if %2/==/ goto usage
call gssetgs.bat
echo -q -dNODISPLAY -dSAFER -dDELAYSAFER >_.at
:cp
if %3/==/ goto doit
echo %1 >>_.at
shift
goto cp

:doit
%GSC% -q @_.at -- pdfopt.ps %1 %2
if exist _.at erase _.at
goto end

:usage
echo "Usage: pdfopt input.pdf output.pdf"

:end
