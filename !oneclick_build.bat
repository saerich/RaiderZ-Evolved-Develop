@echo off

: ���� ���̱�
CALL !kill_server


: SVN ������Ʈ
CALL !svn_update

: ����
CALL !build


:END
