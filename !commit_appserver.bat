@echo off

SET root_path=%CD%
SET target_path=.

: ��������� Ŀ��
svn commit -m "[builder] ���ø����̼� ���� ����" %target_path%\Game\Runtime\Dev\AppServer\AppServer.exe %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole.exe

