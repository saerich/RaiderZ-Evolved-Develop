@echo off

: Copy runtime
SET root_path=%CD%
SET target_path=.

: Ŭ���̾�Ʈ �������� ����
copy %target_path%\Game\Runtime\Game.exe %target_path%\Game\Runtime\Raiderz.exe /y
copy %target_path%\Game\Runtime\Game.pdb %target_path%\Game\Runtime\Raiderz.pdb /y

: !�˸�! Raiderz_Publish_Buyer.exe(, pdb) ���ϰ�, Raiderz_Publish_Korea.exe(, pdb) ������ ���尡 ������ ������ �ٷ� �����մϴ�.
:	-> ccnet.config ���� ����.
:	-> Publish(Stable) �� exe, pdb ���� �̸��� ��ġ�� ����.

: Publish(Stable) Ŭ���̾�Ʈ �������� ����
copy %target_path%\Game\Runtime\Game_Publish.exe %target_path%\Game\Runtime\Raiderz_Publish.exe /y
copy %target_path%\Game\Runtime\Game_Publish.pdb %target_path%\Game\Runtime\Raiderz_Publish.pdb /y

: ���� ���� �������� ����
copy %target_path%\Game\Runtime\Dev\GameServer\GameServer_r.exe %target_path%\Game\Runtime\Dev\GameServer\GameServer.exe /y
copy %target_path%\Game\Runtime\Dev\GameServer\GameServer_r.pdb %target_path%\Game\Runtime\Dev\GameServer\GameServer.pdb /y
copy %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole_r.exe %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole.exe /y
copy %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole_r.pdb %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole.pdb /y

: ������ ���� �������� ����
copy %target_path%\Game\Runtime\Dev\MasterServer\MasterServer_r.exe %target_path%\Game\Runtime\Dev\MasterServer\MasterServer.exe /y
copy %target_path%\Game\Runtime\Dev\MasterServer\MasterServer_r.pdb %target_path%\Game\Runtime\Dev\MasterServer\MasterServer.pdb /y
copy %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole_r.exe %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole.exe /y
copy %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole_r.pdb %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole.pdb /y

: �α��� ���� �������� ����
copy %target_path%\Game\Runtime\Dev\LoginServer\LoginServer_r.exe %target_path%\Game\Runtime\Dev\LoginServer\LoginServer.exe /y
copy %target_path%\Game\Runtime\Dev\LoginServer\LoginServer_r.pdb %target_path%\Game\Runtime\Dev\LoginServer\LoginServer.pdb /y
copy %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole_r.exe %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole.exe /y
copy %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole_r.pdb %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole.pdb /y

: �� ���� �������� ����
copy %target_path%\Game\Runtime\Dev\AppServer\AppServer_r.exe %target_path%\Game\Runtime\Dev\AppServer\AppServer.exe /y
copy %target_path%\Game\Runtime\Dev\AppServer\AppServer_r.pdb %target_path%\Game\Runtime\Dev\AppServer\AppServer.pdb /y
copy %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole_r.exe %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole.exe /y
copy %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole_r.pdb %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole.pdb /y

:END
