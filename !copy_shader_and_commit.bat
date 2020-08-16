@echo off

: Copy runtime
SET root_path=%CD%
SET target_path=.

copy %target_path%\mdk\RealSpace3\Runtime\Shader\*.* %target_path%\Game\Runtime\Data\Shader\ /y
copy %target_path%\mdk\RealSpace3\Runtime\Shader\components\*.* %target_path%\Game\Runtime\Data\Shader\components\ /y

: 서브버전에 커밋
svn add %target_path%\Game\Runtime\Data\Shader\* --force
svn commit -m "[builder] Shader 변경" %target_path%\Game\Runtime\Data\Shader\*


:END
