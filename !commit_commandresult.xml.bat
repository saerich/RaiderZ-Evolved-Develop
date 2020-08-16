@echo off

SET root_path=%CD%
SET target_path=.

: 서브버전에 커밋
svn commit -m "[builder] commandresult" %target_path%\Game\Runtime\Data\system\lang\ko_kr\result.xml