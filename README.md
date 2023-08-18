# Banana programming language

Banana 🍌, a toy VM-based programming language.

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

**Types**: `bool`, `char`, `short`, `int`, `long`.

**Constructs**: `if`, `else`, `for`, `while`, `return`.

**Binary Operators**: `+`, `-`, `*`, `/`, `%`, `^`, `&`, `|`, `<`, `<=`, `>`, `>=`, `==`, `!=`, `and`, `or`, `+=`, `-=`, `*=`, `/=`, `%=`, `^=`, `&=`, `|=`.

**Unary Operators**: `=`, `-`, `!`, `~`, `++`, `--`.

**Std library**: `print`.

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