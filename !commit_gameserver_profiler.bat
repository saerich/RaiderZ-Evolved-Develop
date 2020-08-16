@echo off

SET root_path=%CD%
SET target_path=.

: 서브버전에 커밋
svn commit -m "[builder] 게임서버 프로파일러 빌드" %target_path%\Game\Runtime\Dev\GameServer\test_GameServerProfiler.exe

