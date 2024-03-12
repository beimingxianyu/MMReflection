#pragma once

#include <cstddef>

namespace MM {
namespace Utils {
#define OFFSET_OF(class_, member) offsetof(class_, member)

#define MM_CAT_IMP(a, b) a##b

#define MM_CAT(a, b) MM_CAT_IMP(a, b)

#define MM_STR_CAT_IMP_IMP(str) #str

#define MM_STR_CAT_IMP(str1, str2) MM_STR_CAT_IMP_IMP(str1##str2)

#define MM_STR_CAT(str1, str2) MM_STR_CAT_IMP(str1, str2)

#define MM_STR_IMP(str) #str

#define MM_STR(str) MM_STR_IMP(str)
}  // namespace Utils
}  // namespace MM
