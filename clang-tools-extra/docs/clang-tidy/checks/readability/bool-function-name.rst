.. title:: clang-tidy - readability-bool-function-name

readability-bool-function-name
==============================

Checks that functions returning ``bool`` have names starting with ``Is``.

Examples:

.. code-block:: c++

  // Bad: function returns bool but name doesn't start with Is.
  bool not_camel_case();
  bool getValue();
  bool foo();

  // Good: function name starts with Is.
  bool IsValid();
  bool IsEmpty();
