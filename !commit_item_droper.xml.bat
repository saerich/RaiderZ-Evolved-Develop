@echo off

SET root_path=%CD%
SET target_path=.

: 서브버전에 커밋
svn commit -m "item_droper.xml 커밋" %target_path%\Game\Runtime\Data\system\item_droper.xml

