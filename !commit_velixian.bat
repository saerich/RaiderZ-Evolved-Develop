@echo off

SET root_path=%CD%
SET target_path=.

: ��������� Ŀ��
svn commit -m "[builder] Velixian.exe Ŀ��" %target_path%\Game\Runtime\Dev\Velixian\Velixian.exe
svn commit -m "[builder] Raiderz.dll Ŀ��" %target_path%\Game\Runtime\Dev\Velixian\Raiderz.dll


