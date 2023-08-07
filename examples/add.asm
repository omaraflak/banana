; 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55

push 0
call .fib 1
print
push 10
printc
halt

.fib
store 0

load 0
push 0
eq
jump_if .end

load 0
push 1
eq
jump_if .end

load 0
push 1
sub
call .fib 1

load 0
push 2
sub
call .fib 1

add

.end
ret