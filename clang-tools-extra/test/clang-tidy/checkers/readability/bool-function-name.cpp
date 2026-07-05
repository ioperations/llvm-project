// RUN: %check_clang_tidy %s readability-bool-function-name %t

// Positive: function returns bool but name doesn't start with Is
bool not_camel_case();
// CHECK-MESSAGES: [[@LINE-1]]:6: warning: function returning 'bool' should be named with an 'Is' prefix

bool getValue();
// CHECK-MESSAGES: [[@LINE-1]]:6: warning: function returning 'bool' should be named with an 'Is' prefix

bool foo();
// CHECK-MESSAGES: [[@LINE-1]]:6: warning: function returning 'bool' should be named with an 'Is' prefix

// Negative: function returns bool and name starts with Is
bool IsValid();
bool IsEmpty();
bool IsReady();

// Negative: non-bool return types
int getInt();
double getDouble();
void doSomething();

// Negative: overloaded operators
struct S {
  bool operator==(const S &) const;
  bool operator!=(const S &) const;
  bool operator<(const S &) const;
  explicit operator bool() const;
};

// Negative: constructors and destructors
struct T {
  T();
  ~T();
  bool IsAvailable();
};

// Negative: function already has Is prefix
bool IsRunning();
bool IsEnabled();
