@echo off

SET root_path=%CD%
SET target_path=.

: ��������� Ŀ��
svn commit -m "[builder] ���� ���� �������� Ŀ��" %target_path%\Game\Runtime\Dev\GameServer\GameServer_stable.exe %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole_stable.exe

svn commit -m "[builder] ������ ���� �������� Ŀ��" %target_path%\Game\Runtime\Dev\MasterServer\MasterServer_stable.exe %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole_stable.exe

svn commit -m "[builder] �α��� ���� �������� Ŀ��" %target_path%\Game\Runtime\Dev\LoginServer\LoginServer_stable.exe %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole_stable.exe

