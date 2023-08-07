push 2
call .fibo 1
print
push 10
printc
halt

.fibo
dup
push 0
eq
jump_if .end

dup
push 1
eq
jump_if .end

dup
push 1
sub
call .fibo 1

swap
push 2
sub
call .fibo 1

add

.end
ret
