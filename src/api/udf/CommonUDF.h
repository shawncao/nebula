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

#pragma once

#include <algorithm>
#include "api/dsl/Base.h"
#include "execution/eval/UDF.h"

/**
 * Define expressions used in the nebula DSL.
 */
namespace nebula {
namespace api {
namespace udf {

// UDAF - a common pattern RK=return kind, EK = expression kind
template <nebula::type::Kind RK, nebula::type::Kind EK>
class CommonUDF : public nebula::execution::eval::UDF<RK> {
public:
  using ReturnType = typename nebula::type::TypeTraits<RK>::CppType;
  using ExprType = typename nebula::type::TypeTraits<EK>::CppType;
  using Logic = std::function<ReturnType(const ExprType&, bool& valid)>;

  explicit CommonUDF(const std::string& name, std::unique_ptr<nebula::execution::eval::ValueEval> expr, Logic&& logic)
    : nebula::execution::eval::UDF<RK>(fmt::format("{0}({1})", name, expr->signature())),
      expr_{ std::move(expr) },
      logic_{ std::move(logic) } {}
  virtual ~CommonUDF() = default;

public:
  // apply a row data to get result
  virtual inline ReturnType run(const nebula::surface::RowData& row, bool& valid) const override {
    return logic_(expr_->eval<ExprType>(row, valid), valid);
  }

private:
  std::unique_ptr<nebula::execution::eval::ValueEval> expr_;
  Logic logic_;
};

} // namespace udf
} // namespace api
} // namespace nebula