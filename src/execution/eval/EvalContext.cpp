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

#include "ValueEval.h"

/**
 * Value evaluation context.
 * It provides same expression evaluation cache.
 * Every evaluation expression has a unique signature. 
 * Also it provides reference return rather than value return comparing to RowData interface. 
 */
namespace nebula {
namespace execution {
namespace eval {

// reset to a new row
void EvalContext::reset(const nebula::surface::RowData& row) {
  // clear t he evaluation map
  cursor_ = 1;
  map_.clear();

  // std::addressof ?
  this->row_ = &row;
}

template <>
std::string_view EvalContext::eval(const nebula::execution::eval::ValueEval& ve, bool& valid) {
  if (LIKELY(!cache_)) {
    return ve.eval<std::string_view>(*this, valid);
  }

  auto sign = ve.signature();

  // if in evaluated list
  auto itr = map_.find(sign);
  if (itr != map_.end()) {
    // offset length
    const auto& ol = itr->second;
    valid = ol.first > 0;
    if (!valid) {
      return "";
    }

    return slice_.read(ol.first, ol.second);
  }

  N_ENSURE_NOT_NULL(row_, "reference a row object before evaluation.");
  const auto value = ve.eval<std::string_view>(*this, valid);
  if (!valid) {
    map_[sign] = { 0, 0 };
    return "";
  }

  const auto offset = cursor_;
  const auto size = value.size();
  map_[sign] = { offset, size };
  cursor_ += slice_.write(cursor_, value.data(), value.size());

  return slice_.read(offset, size);
}

} // namespace eval
} // namespace execution
} // namespace nebula