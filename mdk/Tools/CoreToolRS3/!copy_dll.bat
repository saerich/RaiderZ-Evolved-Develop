IF "%1" == "" goto :END
set outPath=..\..\Game\Runtime\Dev\CoreToolRS3\
IF NOT exist %outPath% (mkdir %outPath%)
copy %1.dll %outPath%
copy %1.pdb %outPath%
:END