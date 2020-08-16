@echo off

echo ===== Game 업데이트 =====
svn up .

echo ===== SDK 업데이트 =====
svn up ..\sdk\

echo ===== Runtime 업데이트 =====

if "%1" == "noruntime" goto :END_RUNTIME
cd game\runtime
call !svnupdate.bat
cd ..\..
rem svn up game\runtime

:END_RUNTIME

: 바로 가기로 실행할 경우 마지막에 콘솔창이 바로 닫히는 것을 막으려면 pause를 인자로 적어주면 된다.
IF "%1" == "pause" (
pause
)