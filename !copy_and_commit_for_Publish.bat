@echo off

: Copy runtime
SET root_path=%CD%
SET target_path=.

: 클라이언트 실행파일 복사
copy %target_path%\Game\Runtime\Game.exe %target_path%\Game\Runtime\Raiderz.exe /y
copy %target_path%\Game\Runtime\Game.pdb %target_path%\Game\Runtime\Raiderz.pdb /y

: !알림! Raiderz_Publish_Buyer.exe(, pdb) 파일과, Raiderz_Publish_Korea.exe(, pdb) 파일은 빌드가 끝나는 시점에 바로 복사합니다.
:	-> ccnet.config 설정 참조.
:	-> Publish(Stable) 과 exe, pdb 파일 이름이 겹치기 때문.

: Publish(Stable) 클라이언트 실행파일 복사
copy %target_path%\Game\Runtime\Game_Publish.exe %target_path%\Game\Runtime\Raiderz_Publish.exe /y
copy %target_path%\Game\Runtime\Game_Publish.pdb %target_path%\Game\Runtime\Raiderz_Publish.pdb /y

: 게임 서버 실행파일 복사
copy %target_path%\Game\Runtime\Dev\GameServer\GameServer_r.exe %target_path%\Game\Runtime\Dev\GameServer\GameServer.exe /y
copy %target_path%\Game\Runtime\Dev\GameServer\GameServer_r.pdb %target_path%\Game\Runtime\Dev\GameServer\GameServer.pdb /y
copy %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole_r.exe %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole.exe /y
copy %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole_r.pdb %target_path%\Game\Runtime\Dev\GameServer\GameServerConsole.pdb /y

: 마스터 서버 실행파일 복사
copy %target_path%\Game\Runtime\Dev\MasterServer\MasterServer_r.exe %target_path%\Game\Runtime\Dev\MasterServer\MasterServer.exe /y
copy %target_path%\Game\Runtime\Dev\MasterServer\MasterServer_r.pdb %target_path%\Game\Runtime\Dev\MasterServer\MasterServer.pdb /y
copy %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole_r.exe %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole.exe /y
copy %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole_r.pdb %target_path%\Game\Runtime\Dev\MasterServer\MasterServerConsole.pdb /y

: 로그인 서버 실행파일 복사
copy %target_path%\Game\Runtime\Dev\LoginServer\LoginServer_r.exe %target_path%\Game\Runtime\Dev\LoginServer\LoginServer.exe /y
copy %target_path%\Game\Runtime\Dev\LoginServer\LoginServer_r.pdb %target_path%\Game\Runtime\Dev\LoginServer\LoginServer.pdb /y
copy %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole_r.exe %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole.exe /y
copy %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole_r.pdb %target_path%\Game\Runtime\Dev\LoginServer\LoginServerConsole.pdb /y

: 앱 서버 실행파일 복사
copy %target_path%\Game\Runtime\Dev\AppServer\AppServer_r.exe %target_path%\Game\Runtime\Dev\AppServer\AppServer.exe /y
copy %target_path%\Game\Runtime\Dev\AppServer\AppServer_r.pdb %target_path%\Game\Runtime\Dev\AppServer\AppServer.pdb /y
copy %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole_r.exe %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole.exe /y
copy %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole_r.pdb %target_path%\Game\Runtime\Dev\AppServer\AppServerConsole.pdb /y

: 커밋
CALL "!commit_gamePublish"
CALL "!commit_gameserver"
CALL "!commit_masterserver"
CALL "!commit_loginserver"
CALL "!commit_appserver"

:END
