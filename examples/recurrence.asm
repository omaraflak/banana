push 5
call .test 1
halt

.test
dup
push 0
eq
jump_if_false .continue
ret 1
.continue
dup
print
push 10
printc
push 1
sub
call .test 1
ret 1