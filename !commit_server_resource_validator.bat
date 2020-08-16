@echo off

SET root_path=%CD%
SET target_path=.

copy %target_path%\Game\Runtime\Dev\ResourceValidator\ServerResourceValidator_r.exe %target_path%\Game\Runtime\Dev\ResourceValidator\ServerResourceValidator.exe	/y
copy %target_path%\Game\Runtime\Dev\ResourceValidator\ServerResourceValidator_r.pdb %target_path%\Game\Runtime\Dev\ResourceValidator\ServerResourceValidator.pdb	/y

: ¼­ºê¹öÀü¿¡ Ä¿¹Ô
svn commit -m "[builder] Server Resource Validator Ä¿¹Ô" %target_path%\Game\Runtime\Dev\ResourceValidator\ServerResourceValidator.exe
