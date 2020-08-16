@echo off

SET root_path=%CD%
SET target_path=.

: VC.NET 을 Command-Line 에서 실행하도록 Path나 기타 환경 변수 등록 - 주석처리
call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall"

del "%target_path%\Game\Runtime\Game.exe"
del "%target_path%\Game\Runtime\Game.pdb"

if not ("%1" == "clean") goto :NO_GAME_CLEAN

: 클라이언트 빌드
echo "클라이언트 클린"
devenv /clean release "%target_path%\Game\Game.sln"

:NO_GAME_CLEAN
devenv /build release "%target_path%\Game\Game.sln"


: Build 결과에 exe 파일을 검출
IF NOT EXIST "%root_path%\%target_path%\Game\Runtime\Game.exe" (

	   : EXE가 없으면 Build 가 실패했으므로 Log 를 자동으로 열어주자.
	   "C:\Program Files\Internet Explorer\iexplore.exe" "%root_path%\%target_path%\Game\Release\BuildLog.htm"
     GOTO END
)

del "%target_path%\Game\Runtime\Dev\GameServer\GameServer_r.exe"
del "%target_path%\Game\Runtime\Dev\GameServer\GameServer_r.pdb"

if not ("%1" == "clean") goto :NO_GAMESERVER_CLEAN

: 서버 빌드
devenv /clean release "%target_path%\Server\GameServer\GameServer.sln"

:NO_GAMESERVER_CLEAN
devenv /build release "%target_path%\Server\GameServer\GameServer.sln"


: Build 결과에 exe 파일을 검출
IF NOT EXIST "%root_path%\%target_path%\Game\Runtime\Dev\GameServer\GameServer_r.exe" (

	   : EXE가 없으면 Build 가 실패했으므로 Log 를 자동으로 열어주자.
	   "C:\Program Files\Internet Explorer\iexplore.exe" "%root_path%\%target_path%\Server\GameServer\Release\BuildLog.htm"
     GOTO END
)


:END
