@echo off

SET root_path=%CD%
SET target_path=.

: 서브버전에 커밋
svn commit -m "[builder] 게임 서버 안정버전 커밋" %target_path%\Game\Runtime\Dev\GameServer\GameServer_stable.exe %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole_stable.exe

svn commit -m "[builder] 마스터 서버 안정버전 커밋" %target_path%\Game\Runtime\Dev\MasterServer\MasterServer_stable.exe %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole_stable.exe

svn commit -m "[builder] 로그인 서버 안정버전 커밋" %target_path%\Game\Runtime\Dev\LoginServer\LoginServer_stable.exe %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole_stable.exe

