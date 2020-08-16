@echo off

: 서버 죽이기
CALL !kill_server


: SVN 업데이트
CALL !svn_update

: 빌드
CALL !build


:END
