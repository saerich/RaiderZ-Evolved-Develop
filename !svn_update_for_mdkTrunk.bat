@echo off
svn up .
IF NOT %ERRORLEVEL%==0 EXIT 1

svn up game\runtime --accept theirs-full
IF NOT %ERRORLEVEL%==0 EXIT 1

rem svn up ..\sdk\
pause