@echo off

SET root_path=%CD%
SET target_path=.

: ��������� Ŀ��
svn commit -m "[builder] ���Ӽ��� �������Ϸ� ����" %target_path%\Game\Runtime\Dev\GameServer\test_GameServerProfiler.exe

