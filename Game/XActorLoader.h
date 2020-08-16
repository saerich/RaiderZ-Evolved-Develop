#ifndef _XActorLoader_h
#define _XActorLoader_h

//#include "XCharacter.h"

class XCharacter;

// ��׶���� ���͸� ����/���� �Ѵ�
class XActorLoader
{
public:
	XActorLoader(void);
	~XActorLoader();

	XCharacter*			CreateActor();
	void				DeleteActor(XCharacter* pActor, bool bBackgroundCreation = true);

	void				Destroy();
};

#endif//_RActorLoaderUnit_h
