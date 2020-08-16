@echo off

SET _cmd=jstart
SET _target=all

CALL Game\Runtime\Dev\ServerMonitor\ServerMonitorClient.exe %_cmd% %_target%
