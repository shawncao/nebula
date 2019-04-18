/*
 * Copyright 2017-present Shawn Cao
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

#include "Count.h"

/**
 * Define expressions used in the nebula DSL.
 */
namespace nebula {
namespace api {
namespace udf {

using nebula::type::Kind;
using VarcharNative = nebula::type::TypeTraits<Kind::VARCHAR>::CppType;

template <>
Count<Kind::VARCHAR>::Count(std::shared_ptr<nebula::api::dsl::Expression> expr)
  : CommonUDAF<Kind::VARCHAR>(expr,
                              [](VarcharNative, VarcharNative) -> VarcharNative {
                                throw NException("sum string is not supported currently");
                              }) {}

} // namespace udf
} // namespace api
} // namespace nebula