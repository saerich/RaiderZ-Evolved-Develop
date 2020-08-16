#include "stdafx.h"
#include "XAnimationEventValidator.h"

bool XAnimationEventValidator::Check()
{
	_VLI(L"data/model/Player/hf/hf.elu.animation.xml");

	bool ret = true;

	ret = CheckEvent();

	return ret;
}

// none_spell_fire1 �ִϸ��̼ǿ� sh_spell_fire �̺�Ʈ�� �ִ� �� �˻�.
bool XAnimationEventValidator::CheckEvent()
{
	MXml xml;
	bool bOk = xml.LoadFile("data/model/Player/hf/hf.elu.animation.xml");
	if(!bOk) return false;

	MXmlElement* pRootNode = xml.Doc()->FirstChildElement();

	MXmlElement* pChild = pRootNode->FirstChildElement();

	for (pChild; pChild != NULL; pChild = pChild->NextSiblingElement())
	{
		char NodeName[256];
		char IDName[256];
		
		AnimationLoopType MLoopType = RAniLoopType_Loop;
		float speed =1.0f;

		strcpy(NodeName, pChild->Value());

		if(strcmp(NodeName, "AddAnimation")==0)
		{
			_Attribute(IDName, pChild,			"name");

			_VLE(L"AddAnimation");	
			_VLA(L"name", MLocale::ConvAnsiToUTF16(IDName));
			_VLP;

			if (strcmp(IDName, "none_spell_fire1") == 0)
			{
				char szNodeName[256];
				char szEventName[256];
				//int nFrame;

				MXmlElement* pEventElement = pChild->FirstChildElement();
				for (pEventElement; pEventElement != NULL; pEventElement = pEventElement->NextSiblingElement())
				{

					strcpy(szNodeName, pEventElement->Value());

					if (strcmp(szNodeName, "EVENT")==0)
					{
						_Attribute(szEventName, pEventElement,	 "name");

						_VLE(L"EVENT");
						_VLA(L"name", MLocale::ConvAnsiToUTF16(szEventName));
						_VLP;

						if (strcmp(szEventName, "sh_spell_fire") == 0)
						{
							return true;
						}
						else
						{
					
							m_Logger.Log(L"�̺�Ʈ �̸��� �޶��.\n");

							return false;
						}
					}
				}

				m_Logger.Log(L"�̺�Ʈ�� �������� �ʾƿ�.\n");

				return false;				
			}
		}
	}

	return true;
}
