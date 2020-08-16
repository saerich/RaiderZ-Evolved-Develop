@echo off

SET source_path=.\Game\Runtime\Dev\CoreToolRS3
SET target_path=.\Game\Runtime\Dev\CoreToolRS3_DevRaiderz

svn cleanup %target_path%
svn revert -R %target_path%
svn update %target_path%

copy %source_path%\*.* %target_path% /y
copy %source_path%\plugins\*.* %target_path%\plugins /y

svn commit -m "CoreTool Develop Version Commit" %target_path%