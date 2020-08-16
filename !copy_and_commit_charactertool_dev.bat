@echo off

SET source_path=.\Game\Runtime\Dev\Character_NET
SET target_path=.\Game\Runtime\Dev\Character_NET_DevRaiderz

svn cleanup %target_path%
svn revert -R %target_path%
svn update %target_path%

copy %source_path%\*.* %target_path% /y

svn commit -m "Character_NET Develop Version Commit" %target_path%