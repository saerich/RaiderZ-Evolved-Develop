@echo off

call "%VS90COMNTOOLS%\vsvars32"

SET target_path=.

devenv /clean release "%target_path%\Game\CoreTool\CoreToolRS3.sln"
devenv /clean debug "%target_path%\Game\CoreTool\CoreToolRS3.sln"
devenv /clean release "%target_path%\Tools\Character_NET\Character_NET.sln"
devenv /clean debug "%target_path%\Tools\Character_NET\Character_NET.sln"
devenv /clean release "%target_path%\mdk\Tools\EffectTool_NET\EffectTool_NET.sln"
devenv /clean debug "%target_path%\mdk\Tools\EffectTool_NET\EffectTool_NET.sln"
devenv /clean release "%target_path%\mdk\Tools\McvRS3\Mcv.sln"
devenv /clean debug "%target_path%\mdk\Tools\McvRS3\Mcv.sln"
devenv /clean release "%target_path%\mdk\Tools\RS3Debugger\RS3Debugger.sln"
devenv /clean debug "%target_path%\mdk\Tools\RS3Debugger\RS3Debugger.sln"
