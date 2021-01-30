/*
 * Copyright 2017-present varchar.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Errors.h"

// If NDBG is defined by build, print call stack of each exception
#ifdef NDBG
#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
#define BOOST_STACKTRACE_USE_ADDR2LINE
#include <boost/stacktrace.hpp>
#include <glog/logging.h>
#endif

namespace nebula {
namespace common {

NebulaException::NebulaException(const std::string& file,
                                 const uint32_t line,
                                 const std::string& method,
                                 const std::string& expr,
                                 const std::string& msg)
  : format_{ fmt::format("[{0}:{1} at function {2}] Nebula Exception={3}: {4}",
                         file, line, method, expr, msg) } {
#ifdef NDBG
  LOG(INFO) << "Exception Call Stack: " << boost::stacktrace::stacktrace();
#endif
}
} // namespace common
} // namespace nebula
