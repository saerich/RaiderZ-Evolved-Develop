@echo off

echo ===== Game 업데이트 =====
svn up

echo ===== MDK 업데이트 =====
cd "mdk\"
svn up

cd ..\

: 바로 가기로 실행할 경우 마지막에 콘솔창이 바로 닫히는 것을 막으려면 pause를 인자로 적어주면 된다.
IF "%1" == "pause" (
pause
)