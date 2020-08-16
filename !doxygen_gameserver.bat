@echo off

@rem  GameServer
SET _target=GameServer
SET _path=.\Server\%_target%\

PUSHD %_path%
doxygen doxyfile

IF EXIST .\Doc\html\%_target%.chm (
	MOVE /Y .\Doc\html\%_target%.chm .\Doc\%_target%.chm
	RD /S /Q .\Doc\html
	
	svn commit -m "[bot] Doxygen 문서 자동 생성" .\Doc\%_target%.chm
)
POPD
