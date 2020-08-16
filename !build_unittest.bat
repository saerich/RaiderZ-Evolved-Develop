@echo off

SET root_path=%CD%
SET target_path=.

: VC.NET �� Command-Line ���� �����ϵ��� Path�� ��Ÿ ȯ�� ���� ��� - �ּ�ó��
call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall"

del "%target_path%\Game\Runtime\Game.exe"
del "%target_path%\Game\Runtime\Game.pdb"

if not ("%1" == "clean") goto :NO_GAME_CLEAN

: Ŭ���̾�Ʈ ����
echo "Ŭ���̾�Ʈ Ŭ��"
devenv /clean release "%target_path%\Game\Game.sln"

:NO_GAME_CLEAN
devenv /build release "%target_path%\Game\Game.sln"


: Build ����� exe ������ ����
IF NOT EXIST "%root_path%\%target_path%\Game\Runtime\Game.exe" (

	   : EXE�� ������ Build �� ���������Ƿ� Log �� �ڵ����� ��������.
	   "C:\Program Files\Internet Explorer\iexplore.exe" "%root_path%\%target_path%\Game\Release\BuildLog.htm"
     GOTO END
)

del "%target_path%\Game\Runtime\Dev\GameServer\GameServer_r.exe"
del "%target_path%\Game\Runtime\Dev\GameServer\GameServer_r.pdb"

if not ("%1" == "clean") goto :NO_GAMESERVER_CLEAN

: ���� ����
devenv /clean release "%target_path%\Server\GameServer\GameServer.sln"

:NO_GAMESERVER_CLEAN
devenv /build release "%target_path%\Server\GameServer\GameServer.sln"


: Build ����� exe ������ ����
IF NOT EXIST "%root_path%\%target_path%\Game\Runtime\Dev\GameServer\GameServer_r.exe" (

	   : EXE�� ������ Build �� ���������Ƿ� Log �� �ڵ����� ��������.
	   "C:\Program Files\Internet Explorer\iexplore.exe" "%root_path%\%target_path%\Server\GameServer\Release\BuildLog.htm"
     GOTO END
)


:END
