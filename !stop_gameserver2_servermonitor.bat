@echo off

SET _cmd=jstop

SET _target=GameServer2
CALL Game\Runtime\Dev\ServerMonitor\ServerMonitorClient.exe %_cmd% %_target%
