@echo off

SET root_path=%CD%
SET target_path=.

: 서브버전에 커밋
svn add %target_path%\Game\Runtime\Data\\Field\map\* --force
svn commit -m "FieldNPCs xml 커밋" %target_path%\Game\Runtime\Data\\Field\map\*

