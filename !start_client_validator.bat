@echo off

SET _cmd=jstart
SET _target=all

CALL Game\Runtime\Dev\ResourceValidator\ClientResourceValidator\ClientResourceValidator.exe ccnet
