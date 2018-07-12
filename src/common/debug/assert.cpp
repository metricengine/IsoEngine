#include "isoengine/common/debug/assert.h"
#include <sstream>

AssertFailure::AssertFailure(const char * expression, const char * file, int line, const char * message)
{
    std::ostringstream oss;
    oss << expression << " failed at: " << file << ":" << line << " -- " << message;
    report = oss.str();
}