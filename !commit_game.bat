@echo off

SET root_path=%CD%
SET target_path=.

: 서브버전에 커밋
svn commit -m "[builder] 클라이언트 빌드" %target_path%\Game\Runtime\Raiderz.exe

