# VM-based programming language

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
    print fib(10);
}
```

Execute:

```
make
./compiler -i file.txt
```

# Features

Currently, the language is very limited and supports the following features.

**Types**: variables are typed using the `var` keyword, but currently it only supports numbers.

**Keywords**: `if`, `else`, `for`, `while`, `fun`, `return`, `and`, `or` 

**Operators**: `+`, `-`, `*`, `/`, `%`, `^`, `&`, `|` (also `+=`, `-=`, `*=`, `/=`, `%=`, `^=`, `&=`, `|=`, and `++`, `--`)

**Std library**:`print`
