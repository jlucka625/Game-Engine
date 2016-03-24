#pragma once

#include <stdexcept>
#include <unordered_map>
#include <windows.h>
#include <fstream>
#include <sstream>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "glm/glm.hpp"
#include "glm/gtx/simd_mat4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/detail/func_trigonometric.hpp"
#include "expat.h"
#include "RTTI.h"

//Library
#include "SList.h"
#include "vector.h"
#include "Stack.h"
#include "HashMap.h"
#include "Datum.h"
#include "Scope.h"
#include "Attributed.h"
#include "XmlParseMaster.h"
#include "IXmlParseHelper.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

#include "RFoo.h"
#include "Foo.h"
#include "Bar.h"
#include "StringSharedData.h"
#include "TestHelper.h"
#include "XmlParseHelperTable.h"
#include "Factory.h"