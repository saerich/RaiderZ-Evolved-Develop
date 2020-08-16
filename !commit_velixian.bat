@echo off

SET root_path=%CD%
SET target_path=.

: ¼­ºê¹öÀü¿¡ Ä¿¹Ô
svn commit -m "[builder] Velixian.exe Ä¿¹Ô" %target_path%\Game\Runtime\Dev\Velixian\Velixian.exe
svn commit -m "[builder] Raiderz.dll Ä¿¹Ô" %target_path%\Game\Runtime\Dev\Velixian\Raiderz.dll


