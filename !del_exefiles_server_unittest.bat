@echo off
: Copy runtime
SET root_path=%CD%
SET target_path=.

: 서버 테스트 실행파일 삭제
del %target_path%\Game\Runtime\Dev\GameServer\GameServer_unittest.exe
del %target_path%\Game\Runtime\Dev\GameServer\GameServer_unittest.pdb

