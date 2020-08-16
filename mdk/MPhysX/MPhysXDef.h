#pragma once

#pragma push_macro("min")
#pragma push_macro("max")
#ifdef min
	#undef min
#endif
#ifdef max
	#undef max
#endif

#include <vector>
#include <map>
#include <list>
#include <string>
#include <set>

#include "NxPhysics.h"

#include "NxController.h"
#include "NxControllerManager.h"

#include "MTypes.h"
#include "MHandle.h"
#include "MXml.h"

#pragma pop_macro("min")
#pragma pop_macro("max")

// MPhysX�� �̷�� Ŭ������
namespace physx
{
	class MPhysX;
	class MPhysXUserAllocator;
	class MPhysXObject;
	class MPhysXCollisionReport;
}
