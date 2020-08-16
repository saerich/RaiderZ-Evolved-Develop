@echo off

: Copy runtime
SET root_path=%CD%
SET target_path=.

: 클라이언트 실행파일 복사
copy %target_path%\Game\Runtime\DummyClient.exe %target_path%\Game\Runtime\Raiderz_DummyClient.exe /y
copy %target_path%\Game\Runtime\DummyClient_r.pdb %target_path%\Game\Runtime\Raiderz_DummyClient.pdb /y

: 커밋
CALL "!commit_dummy_client"

:END
