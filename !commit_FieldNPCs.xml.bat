@echo off

SET root_path=%CD%
SET target_path=.

: ��������� Ŀ��
svn add %target_path%\Game\Runtime\Data\\Field\map\* --force
svn commit -m "FieldNPCs xml Ŀ��" %target_path%\Game\Runtime\Data\\Field\map\*

