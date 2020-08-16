@echo off

SET root_path=%CD%
SET target_path=.

: 클라이언트 실행파일 복사
copy %target_path%\Game\Runtime\Game.exe %target_path%\Game\Runtime\Raiderz.exe /y
copy %target_path%\Game\Runtime\Game.pdb %target_path%\Game\Runtime\Raiderz.pdb /y

: 서버 실행파일 복사
copy %target_path%\Game\Runtime\Dev\GameServer\GameServer_r.exe %target_path%\Game\Runtime\Dev\GameServer\GameServer.exe /y
copy %target_path%\Game\Runtime\Dev\GameServer\GameServer_r.pdb %target_path%\Game\Runtime\Dev\GameServer\GameServer.pdb /y

: 서버 리소스 동기화
CALL %target_path%\Game\Runtime\Dev\GameServer\copy_share %target_path%\Game\Runtime %target_path%\Game\Runtime\Dev\GameServer


:END
