@echo off

SET root_path=%CD%
SET target_path=.

: 서브버전에 커밋
svn commit -m "[builder] 게임 서버 빌드" %target_path%\Game\Runtime\Dev\GameServer\GameServer.exe %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole.exe

