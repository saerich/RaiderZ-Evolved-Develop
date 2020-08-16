echo off

: SVN 업데이트
CALL "!svn_update"

: 빌드
CALL "!build"

: Copy runtime
SET root_path=%CD%
SET target_path=.

: 클라이언트 실행파일 복사
copy %target_path%\Game\Runtime\Game.exe %target_path%\Game\Runtime\Raiderz.exe /y
copy %target_path%\Game\Runtime\Game.pdb %target_path%\Game\Runtime\Raiderz.pdb /y

: 서버 실행파일 복사
copy %target_path%\Game\Runtime\Dev\GameServer\GameServer_r.exe %target_path%\Game\Runtime\Dev\GameServer\GameServer.exe /y
copy %target_path%\Game\Runtime\Dev\GameServer\GameServer_r.pdb %target_path%\Game\Runtime\Dev\GameServer\GameServer.pdb /y

: 심볼 복사
copy %target_path%\Game\Runtime\Game.pdb e:\SYMBOLS\PRODUCTSYMBOLS\Game.pdb
copy %target_path%\Game\Runtime\Dev\GameServer\GameServer_r.pdb e:\SYMBOLS\PRODUCTSYMBOLS\GameServer_r.pdb

: 커밋
CALL "!commit_game"
CALL "!commit_gameserver"

:END
