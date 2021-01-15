IF "%1" == "" goto :END
set outPath=.\Runtime\
IF NOT exist %outPath% (mkdir %outPath%)
copy %1.dll %outPath%
copy %1.pdb %outPath%
:END