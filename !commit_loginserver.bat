@echo off

SET root_path=%CD%
SET target_path=.

: ��������� Ŀ��
svn commit -m "[builder] �α��� ���� ����" %target_path%\Game\Runtime\Dev\LoginServer\LoginServer.exe %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole.exe

