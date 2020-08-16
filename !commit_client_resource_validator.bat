@echo off

SET root_path=%CD%
SET target_path=.

copy %target_path%\Game\Runtime\Dev\ResourceValidator\ClientResourceValidator\ClientResourceValidator_r.exe %target_path%\Game\Runtime\Dev\ResourceValidator\ClientResourceValidator\ClientResourceValidator.exe	/y
copy %target_path%\Game\Runtime\Dev\ResourceValidator\ClientResourceValidator\ClientResourceValidator_r.pdb %target_path%\Game\Runtime\Dev\ResourceValidator\ClientResourceValidator\ClientResourceValidator.pdb	/y

: ¼­ºê¹öÀü¿¡ Ä¿¹Ô
svn commit -m "[builder] Server Resource Validator Ä¿¹Ô" %target_path%\Game\Runtime\Dev\ResourceValidator\ClientResourceValidator
