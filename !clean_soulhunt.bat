@echo off

call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall"

SET target_path=.

devenv /clean release "%target_path%\Game\Game.sln"
devenv /clean release "%target_path%\Server\GameServer\GameServer.sln"