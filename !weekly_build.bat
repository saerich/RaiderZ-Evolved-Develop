@echo off

: 서버 죽이기
CALL !kill_server

: SVN 업데이트
CALL !svn_update

: 빌드
CALL !build clean

: 커밋
CALL !commit_runtime

cd Game\Runtime

: Databank로 배포
CALL !publish stable
CALL !publish stable ..\..\..\Runtime_Stable

cd ..\..

: 서버 띄우기(Active, Stable 둘 다)
START "GameServer" /D Game\Runtime\Dev\GameServer GameServer.exe

cd ..
START "GameServer_Stable" /D Runtime_Stable\Dev\GameServer GameServer.exe

cd Develop

:END
