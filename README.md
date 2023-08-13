# Banana programming language

This is a toy programming language based on a virtual machine.

# Sample code

```
fun fib(n) {
    if (n == 1 or n == 2) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

fun main() {
    for (var i = 1; i < 10; i++) {
        print fib(i);
    }
}
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

**Types**: variables are typed using the `var` keyword, but currently it only supports numbers.

**Constructs**: `fun`, `if`, `else`, `for`, `while`, `return`.

**Binary Operators**: `+`, `-`, `*`, `/`, `%`, `^`, `&`, `|`, `<`, `<=`, `>`, `>=`, `==`, `!=`, `and`, `or` (also `+=`, `-=`, `*=`, `/=`, `%=`, `^=`, `&=`, `|=`).

**Unary Operators**: `=`, `-`, `!`, `~`, `++`, `--`.

**Std library**: `print`.

# CLI

#### Run code from source file

```
$ ./banana -i source.na
```

#### Compile code from source file

This will compile the source file to vm bytecode.

```
$ ./banana -c source.na
```

#### Print vm instructions from source file

`source.na` :
```
fun add(a, b) {
    return a + b;
}

fun main() {
    print add(5, 6);
}
```

```
$ ./banana -a source.na
0       jump 48
9       store 0
18      store 8
27      load 0
36      load 8
45      add
46      ret 1
48      push 6
57      push 5
66      call 9 2
76      print
77      push 10
86      printc
87      halt
```