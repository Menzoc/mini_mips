add R0,65535,R8
add R0,0,R9

add R0,100,R1
add R0,1200,R2
add R0,0,R3

WHILE_1:
    braz R3,BLINK_ON
    jmp BLINK_OFF,R0

BLINK_ON:
    store R1,0,R8
    store R2,0,R8
    scall 2
    add R3,1,R3
    jmp WHILE_1, R0

BLINK_OFF:
    store R1,0,R9
    store R2,0,R9
    scall 2
    sub R3,1,R3
    jmp WHILE_1, R0

stop
;0xe0000002 / 0b11100000000000000000000000000010 .
