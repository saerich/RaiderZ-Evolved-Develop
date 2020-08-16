CALL "!buildall_and_commit"

cd ..\..\Develop

CALL "!kill_server"
CALL "!svn_update"

START "GameServer" /D Game\Runtime\Dev\GameServer\ GameServer.exe