# Banana programming language

Banana 🍌, a toy VM-based programming language.

# Sample code

```
long fib(n) {
    if (n == 1 or n == 2) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

int main() {
    for (long i = 1; i < 10; i++) {
        print fib(i);
    }
}
```

Prerequisites:

```
sudo apt install libffi-dev
```

Compile 🍌:

```
cmake .
make
```

Run program:

```
./banana -i ./examples/fibonacci.na
```

# Features

Currently, the language is very limited and supports the following features.

**Types**: `bool`, `char`, `int`, `long`.

The language supports automatic casting of types when needed.

**Constructs**: `if`, `else`, `for`, `while`, `return`.

**Binary Operators**: `+`, `-`, `*`, `/`, `%`, `^`, `&`, `|`, `<`, `<=`, `>`, `>=`, `==`, `!=`, `and`, `or`, `+=`, `-=`, `*=`, `/=`, `%=`, `^=`, `&=`, `|=`.

**Unary Operators**: `=`, `-`, `!`, `~`, `++`, `--`.

**Std library**: `print`.

**Native C Calls**: `@native()`

You can call native C code from Banana, provided that you expose the code in a shared library.

`mylib.cpp`:

```cpp
#include "../lib/c_function.h"

long some_native_c_function(long n) {
    // something complicated here
    return 2 * n;
}

class MyNativeFunction : public CFunction {
    cfunction::ArgType get_return_type() const { return cfunction::LONG; }
    std::vector<cfunction::ArgType> get_arg_types() const { return {cfunction::LONG}; }
    std::string get_name() const { return "math::times_two"; }
    void* get_function() const { return (void*) nth_prime; }
};

std::vector<CFunction*> get_classes() { return {new MyNativeFunction()}; }
```

Compile this as a shared library:

```
g++ mylib.cpp -o lib_folder/mylib.so -shared -fPIC
```

Bind it in Banana:

```
@native("math::times_two")
long timesTwo(long n);

int main() {
    print timesTwo(10);
}
```

Provide the shared library directory with `--lib` when running the code:

```
./banana -i myscript.na --lib lib_folder
```

# CLI

#### Run code from compiled file

```
$ ./banana source.obj
```

#### Run code from source file

```
$ ./banana -i source.na
```

#### Compile source file

```
$ ./banana -c source.na
```

#### Print VM instructions from source file

```
int add(int a, int b) {
    return a + b;
}

int main() {
    print add(5, 6);
}
```

```
$ ./banana -a source.na
0       jump 48
9       store 0
18      store 1
27      load 0
36      load 1
45      add
46      ret 1
48      push int 6
54      push int 5
60      call 9 2
70      print
71      push char 10
74      print
75      halt
```