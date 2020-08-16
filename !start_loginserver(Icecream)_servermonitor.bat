@echo off

SET _cmd=jstart
SET _target=Icecream

CALL Game\Runtime\Dev\ServerMonitor\ServerMonitorClient.exe %_cmd% %_target%
