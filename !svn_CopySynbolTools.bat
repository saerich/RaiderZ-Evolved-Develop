symstore.exe add /o /r /f %CD%\Game\Runtime\Dev\CoreToolRS3\CoreToolRS3.pdb /s \\icecream\SYMBOLS\PRODUCTSYMBOLS /t "CoreToolRS3" /compress > !svn_CopySynbolTools.log
symstore.exe add /o /r /f %CD%\Game\Runtime\Dev\CoreToolRS3\CoreTool.pdb /s \\icecream\SYMBOLS\PRODUCTSYMBOLS /t "CoreTool" /compress > !svn_CopySynbolTools.log
symstore.exe add /o /r /f %CD%\Game\Runtime\Dev\CoreToolRS3\plugins\Plugin_CutScene.pdb /s \\icecream\SYMBOLS\PRODUCTSYMBOLS /t "Plugin_CutScene" /compress > !svn_CopySynbolTools.log
symstore.exe add /o /r /f %CD%\Game\Runtime\Dev\CoreToolRS3\plugins\Plugin_Raiderz.pdb /s \\icecream\SYMBOLS\PRODUCTSYMBOLS /t "Plugin_Raiderz" /compress > !svn_CopySynbolTools.log
symstore.exe add /o /r /f %CD%\mdk\cml2\ReleaseDLL\vc90.pdb /s \\icecream\SYMBOLS\PRODUCTSYMBOLS /t "cml2" /compress > !svn_CopySynbolTools.log
symstore.exe add /o /r /f %CD%\mdk\RealSpace3\ReleaseDLL\vc90.pdb /s \\icecream\SYMBOLS\PRODUCTSYMBOLS /t "RealSpace3" /compress > !svn_CopySynbolTools.log
