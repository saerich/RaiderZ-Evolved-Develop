@echo off

SET root_path=%CD%
SET target_path=.

: ��������� Ŀ��
svn commit -m "[builder] Ŭ���̾�Ʈ ����" %target_path%\Game\Runtime\Raiderz.exe
svn commit -m "[builder] Ŭ���̾�Ʈ Publish_Buyer ����" %target_path%\Game\Runtime\Raiderz_Publish_Buyer.exe
svn commit -m "[builder] Ŭ���̾�Ʈ Publish_Korea ����" %target_path%\Game\Runtime\Raiderz_Publish_Korea.exe
svn commit -m "[builder] Ŭ���̾�Ʈ Publish(Stable) ����" %target_path%\Game\Runtime\Raiderz_Publish.exe

