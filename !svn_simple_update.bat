@echo off

echo ===== Game ������Ʈ =====
svn up

echo ===== MDK ������Ʈ =====
cd "mdk\"
svn up

cd ..\

: �ٷ� ����� ������ ��� �������� �ܼ�â�� �ٷ� ������ ���� �������� pause�� ���ڷ� �����ָ� �ȴ�.
IF "%1" == "pause" (
pause
)