@echo off
: Copy runtime
SET root_path=%CD%
SET target_path=.

: Ŭ���̾�Ʈ �������� ����
del %target_path%\Game\Runtime\Raiderz.exe
del %target_path%\Game\Runtime\Raiderz.pdb

: ���� ���� �������� ����
del %target_path%\Game\Runtime\Dev\GameServer\GameServer.exe
del %target_path%\Game\Runtime\Dev\GameServer\GameServer.pdb
del %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole.exe
del %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole.pdb

: ������ ���� �������� ����
del %target_path%\Game\Runtime\Dev\MasterServer\MasterServer.exe
del %target_path%\Game\Runtime\Dev\MasterServer\MasterServer.pdb
del %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole.exe
del %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole.pdb

: �α��� ���� �������� ����
del %target_path%\Game\Runtime\Dev\LoginServer\LoginServer.exe
del %target_path%\Game\Runtime\Dev\LoginServer\LoginServer.pdb
del %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole.exe
del %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole.pdb

: ���ø����̼� ���� �������� ����
del %target_path%\Game\Runtime\Dev\AppServer\AppServer.exe
del %target_path%\Game\Runtime\Dev\AppServer\AppServer.pdb
del %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole.exe
del %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole.pdb
