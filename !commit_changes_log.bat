@echo off

SET root_path=%CD%
SET target_path=.

: �α� ����
svn log --incremental -l 1000 -r HEAD:17500 > %target_path%\Game\Runtime\changes.txt

cd %target_path%\Game\Runtime

svn log --incremental -l 1000 -r HEAD:32000 > changes_runtime.txt

: ��������� Ŀ��
svn commit -m "[builder] ������� �α� Ŀ��" changes.txt changes_runtime.txt


