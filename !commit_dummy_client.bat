@echo off

SET root_path=%CD%
SET target_path=.

: ��������� Ŀ��
svn commit -m "[builder] ���� Ŭ���̾�Ʈ ����" %target_path%\Game\Runtime\Raiderz_DummyClient.exe

