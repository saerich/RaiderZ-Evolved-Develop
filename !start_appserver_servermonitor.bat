@echo off

SET _cmd=jstart
SET _target=AppServer

CALL Game\Runtime\Dev\ServerMonitor\ServerMonitorClient.exe %_cmd% %_target%
