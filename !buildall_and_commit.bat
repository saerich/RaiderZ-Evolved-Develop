echo off

: SVN ������Ʈ
CALL "!svn_update"

: ����
CALL "!build"

: Copy runtime
SET root_path=%CD%
SET target_path=.

: Ŭ���̾�Ʈ �������� ����
copy %target_path%\Game\Runtime\Game.exe %target_path%\Game\Runtime\Raiderz.exe /y
copy %target_path%\Game\Runtime\Game.pdb %target_path%\Game\Runtime\Raiderz.pdb /y

: ���� �������� ����
copy %target_path%\Game\Runtime\Dev\GameServer\GameServer_r.exe %target_path%\Game\Runtime\Dev\GameServer\GameServer.exe /y
copy %target_path%\Game\Runtime\Dev\GameServer\GameServer_r.pdb %target_path%\Game\Runtime\Dev\GameServer\GameServer.pdb /y

: �ɺ� ����
copy %target_path%\Game\Runtime\Game.pdb e:\SYMBOLS\PRODUCTSYMBOLS\Game.pdb
copy %target_path%\Game\Runtime\Dev\GameServer\GameServer_r.pdb e:\SYMBOLS\PRODUCTSYMBOLS\GameServer_r.pdb

: Ŀ��
CALL "!commit_game"
CALL "!commit_gameserver"

:END
