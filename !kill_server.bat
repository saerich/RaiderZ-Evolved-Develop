@echo off

: ¼­¹ö Á×ÀÌ±â
@rem taskkill /F /IM GameServer.exe
@rem CALL "!stop_loginserver(Icecream)_servermonitor"
@rem CALL "!stop_gameserver2_servermonitor"
@rem CALL "!stop_gameserver_servermonitor"
@rem CALL "!stop_appserver_servermonitor"
@rem CALL "!stop_masterserver_servermonitor"

CALL "!stop_allserver_servermonitor"