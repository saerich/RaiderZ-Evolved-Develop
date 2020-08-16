@echo off

: ¼­¹ö Á×ÀÌ±â
CALL "!kill_server"

: SVN ¾÷µ¥ÀÌÆ®
CALL "!svn_update"

: ºôµå
CALL "!build" clean

: Copy runtime
CALL "!copy_runtime"

: Ä¿¹Ô
CALL "!commit_runtime"

: ¼­¹ö ¶ç¿ì±â
START "GameServer" /D Game\Runtime\Dev\GameServer\ GameServer.exe

:END
