@echo off

SET _cmd=jstart

SET _target=GameServer
CALL Game\Runtime\Dev\ServerMonitor\ServerMonitorClient.exe %_cmd% %_target%
