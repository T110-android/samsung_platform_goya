@rem $Id: rm.bat,v 1.3 2007/05/07 11:21:44 Arabidopsis Exp $
@echo off
:next
if '%1'=='' goto exit
if '%1'=='-f' goto sh
if exist %1 erase %1
:sh
shift
goto next
:exit
