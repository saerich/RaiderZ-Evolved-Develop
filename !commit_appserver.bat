@echo off

SET root_path=%CD%
SET target_path=.

: 서브버전에 커밋
svn commit -m "[builder] 어플리케이션 서버 빌드" %target_path%\Game\Runtime\Dev\AppServer\AppServer.exe %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole.exe

