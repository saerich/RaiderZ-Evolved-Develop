@echo off

: ���� ���̱�
CALL "!kill_server"

: SVN ������Ʈ
CALL "!svn_update"

: ����
CALL "!build" clean

: Copy runtime
CALL "!copy_runtime"

: Ŀ��
CALL "!commit_runtime"

: ���� ����
START "GameServer" /D Game\Runtime\Dev\GameServer\ GameServer.exe

:END
