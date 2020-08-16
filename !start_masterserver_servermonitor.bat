@echo off

SET _cmd=jstart
SET _target=MasterServer

CALL Game\Runtime\Dev\ServerMonitor\ServerMonitorClient.exe %_cmd% %_target%
@rem START "ServerMonitorClient" /D Game\Runtime\Dev\ServerMonitor\ ServerMonitorClient.exe %_cmd% %_target% 1
