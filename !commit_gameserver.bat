@echo off

SET root_path=%CD%
SET target_path=.

: ��������� Ŀ��
svn commit -m "[builder] ���� ���� ����" %target_path%\Game\Runtime\Dev\GameServer\GameServer.exe %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole.exe

