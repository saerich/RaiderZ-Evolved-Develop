@echo off

SET sourcepath=Game\Runtime\Data\Zone
SET destpath=Game\Runtime\Dev\GameServer\zone

: runtime ������Ʈ
cd game\runtime
svn cleanup
call !svnupdate.bat
cd ..\..

: zpk ����
del %destpath%\*.zpk

: Ŭ���̾�Ʈ ������ ����
powershell -command "$files = Get-ChildItem %sourcepath% -include *.cl2, *.dds, *.pickz.xml, *.pathengine, *.zone.xml -recurse; foreach($file in $files){ Copy-Item $file %destpath% };

: zpk �����
: ��Ʈ ���ε� ���� ������ ���Ӽ����� �ݵ�� ������־�� �մϴ�.
: ���Ӽ��� ����� CC.Net ���� ����մϴ�. - aibeast
Game\Runtime\Dev\GameServer\GameServerConsole.exe --exit

: ��������� Ŀ��
svn commit -m "[builder] zpk Ŀ��" Game\Runtime\Dev\GameServer\Zone
