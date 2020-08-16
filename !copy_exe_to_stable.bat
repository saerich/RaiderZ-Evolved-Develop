@echo off

SET root_path=%CD%
SET target_path=.

: 클라이언트 실행파일 복사
copy %target_path%\Game\Runtime\Raiderz.exe %target_path%\Game\Runtime\Raiderz_stable.exe /y

: 서버 실행파일 복사
copy %target_path%\Game\Runtime\Dev\GameServer\GameServer.exe %target_path%\Game\Runtime\Dev\GameServer\GameServer_stable.exe /y
copy %target_path%\Game\Runtime\Dev\MasterServer\MasterServer.exe %target_path%\Game\Runtime\Dev\MasterServer\MasterServer_stable.exe /y
copy %target_path%\Game\Runtime\Dev\LoginServer\LoginServer.exe %target_path%\Game\Runtime\Dev\LoginServer\LoginServer_stable.exe /y
copy %target_path%\Game\Runtime\Dev\AppServer\AppServer.exe %target_path%\Game\Runtime\Dev\AppServer\AppServer_stable.exe /y

copy %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole.exe %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole_stable.exe /y
copy %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole.exe %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole_stable.exe /y
copy %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole.exe %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole_stable.exe /y
copy %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole.exe %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole_stable.exe /y

:END
