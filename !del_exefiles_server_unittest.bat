@echo off
: Copy runtime
SET root_path=%CD%
SET target_path=.

: ���� �׽�Ʈ �������� ����
del %target_path%\Game\Runtime\Dev\GameServer\GameServer_unittest.exe
del %target_path%\Game\Runtime\Dev\GameServer\GameServer_unittest.pdb

