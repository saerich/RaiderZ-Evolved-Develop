@echo off

: Copy runtime
SET root_path=%CD%
SET target_path=.

: Ŭ���̾�Ʈ �������� ����
copy %target_path%\Game\Runtime\DummyClient.exe %target_path%\Game\Runtime\Raiderz_DummyClient.exe /y
copy %target_path%\Game\Runtime\DummyClient_r.pdb %target_path%\Game\Runtime\Raiderz_DummyClient.pdb /y

: Ŀ��
CALL "!commit_dummy_client"

:END
