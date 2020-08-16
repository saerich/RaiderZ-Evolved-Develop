@echo off
svn cleanup . game\runtime
svn up . --accept theirs-full --non-interactive
IF NOT %ERRORLEVEL%==0 EXIT 1
rem ..\sdk