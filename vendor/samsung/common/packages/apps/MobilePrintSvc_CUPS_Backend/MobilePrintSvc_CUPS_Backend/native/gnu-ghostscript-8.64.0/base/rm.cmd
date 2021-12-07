@echo off
@rem $Id: rm.cmd,v 1.3 2007/05/07 11:21:47 Arabidopsis Exp $
:next
if '%1'=='' goto exit
if '%1'=='-f' goto sh
erase %1
:sh
shift
goto next
:exit
