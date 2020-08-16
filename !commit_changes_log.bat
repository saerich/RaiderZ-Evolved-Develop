@echo off

SET root_path=%CD%
SET target_path=.

: 로그 생성
svn log --incremental -l 1000 -r HEAD:17500 > %target_path%\Game\Runtime\changes.txt

cd %target_path%\Game\Runtime

svn log --incremental -l 1000 -r HEAD:32000 > changes_runtime.txt

: 서브버전에 커밋
svn commit -m "[builder] 변경사항 로그 커밋" changes.txt changes_runtime.txt


