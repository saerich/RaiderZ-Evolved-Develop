#pragma once

namespace RS3U
{
	/* XML�� ������
	   1.  xxx.zone.xml 			: RWorldSceneNode�� �����ϴ� ������ ��� �ִ� XML����
	   2.  xxx.scene.xml 			: RSceneNodeInstance�� �����ϴ� ������ ��� �ִ� XML����
	   3.  xxx.Effect.scene.xml 	: 
	*/
	class RUSceneXML
	{
	public:
		RUSceneXML(void);
		~RUSceneXML(void);

	public:
		//static void Load
	};

	class RUSceneNodeCreator
	{
	public:
		static void CreateSceneNodeFromFile(char* szFileName){};
	};
}