.globl mutex_lock
mutex_lock:
    mov  w1, #0x1
loop:
    ldaxr w2, [x0]
    cbnz w2, loop
    stxr w2, w1, [x0]
    cbnz w2, loop
    ret

.globl mutex_unlock
mutex_unlock:
    mov  w1, #0x0
    stlr w1, [x0]
    ret