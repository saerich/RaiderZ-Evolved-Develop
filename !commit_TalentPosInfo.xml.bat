@echo off

SET root_path=%CD%
SET target_path=.

: ��������� Ŀ��
svn commit -m "[TalentExport] talent_pos_info.xml Ŀ��" %target_path%\Game\Runtime\Dev\GameServer\system\talent_pos_info.xml

