@echo off

@rem ��� ���� ��ġ���� ����
call "%VS80COMNTOOLS%vsvars32.bat"

@rem �� ���� ����
devenv /build Release "./Character_NET/Character_NET.sln"
devenv /build Release "./CoreToolRS3/CoreToolRS3.sln"
devenv /build Release "./EffectTool_NET/EffectTool_NET.sln"
