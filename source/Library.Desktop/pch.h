#pragma once

#include <stdexcept>
#include <cstdint>
#include <new>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <windows.h>
#include <fstream>
#include <sstream>

#include "RTTI.h"

#include "glm/glm.hpp"
#include "glm/gtx/simd_mat4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/detail/func_trigonometric.hpp"

#include "expat.h"

#include "GameException.h"
#include "SList.h"
#include "vector.h"
#include "Stack.h"
#include "HashMap.h"
#include "Datum.h"
#include "Scope.h"
#include "Attributed.h"
#include "GameTime.h"
#include "GameClock.h"
#include "WorldState.h"
#include "Factory.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionListIf.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "ActionAssign.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"

#include "IXmlParseHelper.h"
#include "XmlParseMaster.h"
#include "XmlParseHelperTable.h"
#include "XmlParseHelperAction.h"