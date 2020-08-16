#ifndef _MINET_H
#define _MINET_H

#include "MiNetLib.h"

namespace minet {

	/// MiNet ���̺귯���� �Ѱ��ϴ� Ŭ����
	/// MiNet ���̺귯������ �ʿ��� �͵��� �ʱ�ȭ�Ѵ�.
MINET_API bool Init(const int nCommandVersion);
/// MiNet���� ����� ���� ��� �����Ѵ�.
MINET_API void Fini();
MINET_API const int GetCommandVersion();
MINET_API void ClearCommandDescs();

} // namespace minet

#define MCOMMAND_VERSION		(minet::GetCommandVersion())

#endif

