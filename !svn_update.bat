@echo off

echo ===== Game ������Ʈ =====
svn up .

echo ===== SDK ������Ʈ =====
svn up ..\sdk\

echo ===== Runtime ������Ʈ =====

if "%1" == "noruntime" goto :END_RUNTIME
cd game\runtime
call !svnupdate.bat
cd ..\..
rem svn up game\runtime

:END_RUNTIME

: �ٷ� ����� ������ ��� �������� �ܼ�â�� �ٷ� ������ ���� �������� pause�� ���ڷ� �����ָ� �ȴ�.
IF "%1" == "pause" (
pause
)