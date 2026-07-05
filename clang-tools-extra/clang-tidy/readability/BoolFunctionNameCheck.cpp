//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "BoolFunctionNameCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::readability {

void BoolFunctionNameCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      functionDecl(returns(booleanType()),
                   unless(anyOf(cxxConstructorDecl(), cxxDestructorDecl(),
                                cxxConversionDecl(),
                                cxxMethodDecl(isOverride()))))
          .bind("func"),
      this);
}

void BoolFunctionNameCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("func");

  if (FD->isOverloadedOperator())
    return;

  StringRef Name = FD->getName();
  if (!Name.starts_with("Is"))
    diag(FD->getLocation(),
         "function returning 'bool' should be named with an 'Is' prefix");
}

} // namespace clang::tidy::readability
