@echo off

: Copy runtime
SET root_path=%CD%
SET target_path=.

: 클라이언트 실행파일 복사
copy %target_path%\Game\Runtime\Game_Publish.exe %target_path%\Game\Runtime\Raiderz_Publish_Korea.exe /y
copy %target_path%\Game\Runtime\Game_Publish.pdb %target_path%\Game\Runtime\Raiderz_Publish_Korea.pdb /y

:END
