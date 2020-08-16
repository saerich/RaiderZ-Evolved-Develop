@echo off

CALL "!kill_server"
CALL "!svn_update"

@rem START "GameServer" /D Game\Runtime\Dev\GameServer\ GameServer.exe
@rem CALL "!start_masterserver_servermonitor"
@rem CALL "!start_appserver_servermonitor"
@rem CALL "!start_gameserver_servermonitor"
@rem CALL "!start_gameserver2_servermonitor"
@rem CALL "!start_loginserver(Icecream)_servermonitor"

CALL "!start_allserver_servermonitor"