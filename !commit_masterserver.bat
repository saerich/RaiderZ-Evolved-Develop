@echo off

SET root_path=%CD%
SET target_path=.

: ��������� Ŀ��
svn commit -m "[builder] ������ ���� ����" %target_path%\Game\Runtime\Dev\MasterServer\MasterServer.exe %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole.exe

