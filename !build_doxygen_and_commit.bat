@echo off

SET target_path=.

SET game_path=game
SET game_server_path=Server\GameServer

SET tools_path=game\runtime\dev\CoreToolRS3 game\runtime\dev\EffectTool_Net game\runtime\dev\Character_Net game\runtime\dev\McvRS3
SET mdk_tools_path=mdk\RealSpace3\Runtime\Tools\

time /t
doxygen %game_path%\Doxyfile

rem svn commit -m "!buildtools_and_commit.bat build" %tools_path%

:END
