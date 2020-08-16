@echo off

if "%1" == "x64" goto X64
call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall"
goto VC_VAR_SET_END

:X64
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall"

:VC_VAR_SET_END

SET target_path=.

devenv /clean release "%target_path%\Game\Game.sln"
devenv /clean UnitTest "%target_path%\Game\Game.sln"
devenv /clean release "%target_path%\Server\GameServer\GameServer.sln"
devenv /clean release "%target_path%\Server\GameServer\GameServer_unittest.sln
devenv /clean release "%target_path%\Server\MasterServer\MasterServer.sln"
devenv /clean release "%target_path%\Server\MasterServer\MasterServer_unittest.sln
devenv /clean release "%target_path%\Server\LoginServer\LoginServer.sln"
devenv /clean release "%target_path%\Server\LoginServer\LoginServer_unittest.sln
devenv /clean release "%target_path%\Server\AppServer\AppServer.sln"
devenv /clean release "%target_path%\Server\AppServer\AppServer_unittest.sln
devenv /clean release "%target_path%\Tools\CoreToolRS3\CoreToolRS3.sln"
devenv /clean release "%target_path%\Tools\Character_NET\Character_NET.sln"
devenv /clean release "%target_path%\Tools\EffectTool_NET\EffectTool_NET.sln"
devenv /clean release "%target_path%\Game\TestRS3\TestRS3.sln"
devenv /clean release "%target_path%\mdk\Tools\McvRS3\Mcv.sln"
devenv /clean release "%target_path%\mdk\Tools\RS3Debugger\RS3Debugger.sln"
