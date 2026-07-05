//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_MISLEADINGIDENTIFIERPREFIXCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_MISLEADINGIDENTIFIERPREFIXCHECK_H

#include "../ClangTidyCheck.h"
#include "llvm/Support/Regex.h"

namespace clang::tidy::readability {

/// Flags local variables where a Hungarian notation prefix (i for int, p for
/// pointer) is misleading because the identifier is actually a valid word
/// or the prefix does not match the type.
///
/// Warns when:
/// - An 'int' variable starts with 'i' followed by an uppercase letter
///   (e.g., 'int iValue;'), unless the type is an iterator.
/// - A pointer variable starts with 'p' followed by an uppercase letter
///   (e.g., 'int* pSubscriber;').
///
/// Natural words like 'index', 'publish' are not flagged because the second
/// character is lowercase.
class MisleadingIdentifierPrefixCheck : public ClangTidyCheck {
public:
  MisleadingIdentifierPrefixCheck(StringRef Name, ClangTidyContext *Context);
  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  static bool isIteratorType(QualType Type, ASTContext *Context);

  StringRef IgnoredIdentifiersInput;
  llvm::Regex IgnoredIdentifiers;
};

} // namespace clang::tidy::readability

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_MISLEADINGIDENTIFIERPREFIXCHECK_H
