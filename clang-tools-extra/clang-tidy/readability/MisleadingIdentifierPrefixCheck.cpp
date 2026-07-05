//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MisleadingIdentifierPrefixCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Type.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang::ast_matchers;

namespace clang::tidy::readability {

namespace {
AST_MATCHER(VarDecl, isLocalVarDecl) { return Node.isLocalVarDecl(); }

const char DefaultIgnoredIdentifiers[] = "";
} // namespace

MisleadingIdentifierPrefixCheck::MisleadingIdentifierPrefixCheck(
    StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      IgnoredIdentifiersInput(
          Options.get("IgnoredIdentifiers", DefaultIgnoredIdentifiers)),
      IgnoredIdentifiers(IgnoredIdentifiersInput) {}

void MisleadingIdentifierPrefixCheck::storeOptions(
    ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "IgnoredIdentifiers", IgnoredIdentifiersInput);
}

void MisleadingIdentifierPrefixCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(isLocalVarDecl()).bind("localVar"), this);
}

bool MisleadingIdentifierPrefixCheck::isIteratorType(QualType Type,
                                                     ASTContext *Context) {
  auto HasIteratorName = [](StringRef Name) {
    return Name.contains("iterator");
  };

  if (HasIteratorName(Type.getAsString()))
    return true;

  QualType QT = Type;
  while (true) {
    QualType NewQT = QT.getSingleStepDesugaredType(*Context);
    if (NewQT == QT)
      break;
    QT = NewQT;
    if (HasIteratorName(QT.getAsString()))
      return true;
  }

  if (HasIteratorName(Type.getCanonicalType().getAsString()))
    return true;

  return false;
}

void MisleadingIdentifierPrefixCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Var = Result.Nodes.getNodeAs<VarDecl>("localVar");
  if (!Var || !Var->getIdentifier())
    return;

  StringRef Name = Var->getName();
  if (Name.size() < 2)
    return;

  if (IgnoredIdentifiers.match(Name))
    return;

  QualType Type = Var->getType();

  if (Name.front() == 'i' && Name[1] >= 'A' && Name[1] <= 'Z') {
    if (const auto *BT =
            dyn_cast<BuiltinType>(Type.getCanonicalType().getTypePtr())) {
      if (BT->getKind() == BuiltinType::Int &&
          !isIteratorType(Type, Result.Context))
        diag(Var->getLocation(),
             "local variable of type 'int' should not start with 'i' prefix");
    }
  }

  if (Name.front() == 'p' && Name[1] >= 'A' && Name[1] <= 'Z') {
    if (Type->isPointerType())
      diag(Var->getLocation(),
           "local variable of pointer type should not start with 'p' prefix");
  }
}

} // namespace clang::tidy::readability
