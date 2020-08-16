@echo off

SET root_path=%CD%
SET target_path=.

: 서브버전에 커밋
svn commit -m "[TalentExport] talent_pos_info.xml 커밋" %target_path%\Game\Runtime\Dev\GameServer\system\talent_pos_info.xml

