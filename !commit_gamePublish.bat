@echo off

SET root_path=%CD%
SET target_path=.

: 서브버전에 커밋
svn commit -m "[builder] 클라이언트 빌드" %target_path%\Game\Runtime\Raiderz.exe
svn commit -m "[builder] 클라이언트 Publish_Buyer 빌드" %target_path%\Game\Runtime\Raiderz_Publish_Buyer.exe
svn commit -m "[builder] 클라이언트 Publish_Korea 빌드" %target_path%\Game\Runtime\Raiderz_Publish_Korea.exe
svn commit -m "[builder] 클라이언트 Publish(Stable) 빌드" %target_path%\Game\Runtime\Raiderz_Publish.exe

