@echo off

SET target_path=.\Game\Runtime
SET target_file=MDKDevRaiderz

copy %target_path%\Game.exe %target_path%\%target_file%.exe /y
rem copy %target_path%\Game.pdb %target_path%\%target_file%.pdb /y

svn commit -m "Raiderz Develop Version Commit" %target_path%\%target_file%.exe