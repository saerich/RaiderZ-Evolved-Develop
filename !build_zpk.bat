@echo off

SET sourcepath=Game\Runtime\Data\Zone
SET destpath=Game\Runtime\Dev\GameServer\zone

: runtime 업데이트
cd game\runtime
svn cleanup
call !svnupdate.bat
cd ..\..

: zpk 삭제
del %destpath%\*.zpk

: 클라이언트 존파일 복사
powershell -command "$files = Get-ChildItem %sourcepath% -include *.cl2, *.dds, *.pickz.xml, *.pathengine, *.zone.xml -recurse; foreach($file in $files){ Copy-Item $file %destpath% };

: zpk 만들기
: 포트 바인딩 문제 때문에 게임서버가 반드시 종료돼있어야 합니다.
: 게임서버 종료는 CC.Net 에서 담당합니다. - aibeast
Game\Runtime\Dev\GameServer\GameServerConsole.exe --exit

: 서브버전에 커밋
svn commit -m "[builder] zpk 커밋" Game\Runtime\Dev\GameServer\Zone
