@echo off

SET root_path=%CD%
SET target_path=.

copy %target_path%\Game\Runtime\Dev\ResourceValidator\ClientResourceValidator\ClientResourceValidator_r.exe %target_path%\Game\Runtime\Dev\ResourceValidator\ClientResourceValidator\ClientResourceValidator.exe	/y
copy %target_path%\Game\Runtime\Dev\ResourceValidator\ClientResourceValidator\ClientResourceValidator_r.pdb %target_path%\Game\Runtime\Dev\ResourceValidator\ClientResourceValidator\ClientResourceValidator.pdb	/y

: ��������� Ŀ��
svn commit -m "[builder] Server Resource Validator Ŀ��" %target_path%\Game\Runtime\Dev\ResourceValidator\ClientResourceValidator
