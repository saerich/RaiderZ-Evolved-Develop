@echo off

: ���� ���̱�
CALL !kill_server

: SVN ������Ʈ
CALL !svn_update

: ����
CALL !build clean

: Ŀ��
CALL !commit_runtime

cd Game\Runtime

: Databank�� ����
CALL !publish stable
CALL !publish stable ..\..\..\Runtime_Stable

cd ..\..

: ���� ����(Active, Stable �� ��)
START "GameServer" /D Game\Runtime\Dev\GameServer GameServer.exe

cd ..
START "GameServer_Stable" /D Runtime_Stable\Dev\GameServer GameServer.exe

cd Develop

:END
