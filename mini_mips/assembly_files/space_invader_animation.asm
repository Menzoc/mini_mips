add R0,65535,R1 ; RGB cyan color (65535 is FFFF in hex so 00FFFF in RGB which is cyan)
add R0,0,R2 ; RGB black color (000000 in RGB)

add R0,0,R3 ; bool to switch between the 2 sprites
add R0,0,R31 ; trash register
add R0,0,R4 ; invader position
add R0,20,R5 ; invader sprite animation counter

WHILE_1:
    braz R5, STOP
    sub R5, 1, R5
    braz R3, INVADER_1
    jmp INVADER_2, R31

INVADER_1:
    mult R0,0,R10
    mult R0,0,R11
    mult R0,0,R12
    mult R0,0,R13
    mult R0,0,R14
    mult R0,0,R15
    mult R0,0,R16
    mult R0,0,R17
    mult R0,0,R18
    mult R0,0,R19
    add R0,2,R10
    add R0,8,R11
    add R0,103,R12
    add R0,107,R13
    add R0,202,R14
    add R0,203,R15
    add R0,204,R16
    add R0,205,R17
    add R0,206,R18
    add R0,207,R19
    store R10,0,R1
    store R11,0,R1
    store R12,0,R1
    store R13,0,R1
    store R14,0,R1
    store R15,0,R1
    store R16,0,R1
    store R17,0,R1
    store R18,0,R1
    store R19,0,R1

    mult R0,0,R10
    mult R0,0,R11
    mult R0,0,R12
    mult R0,0,R13
    mult R0,0,R14
    mult R0,0,R15
    mult R0,0,R16
    mult R0,0,R17
    mult R0,0,R18
    mult R0,0,R19
    add R0,208,R10
    add R0,301,R11
    add R0,302,R12
    add R0,304,R13
    add R0,305,R14
    add R0,306,R15
    add R0,308,R16
    add R0,309,R17
    add R0,400,R18
    add R0,401,R19
    store R10,0,R1
    store R11,0,R1
    store R12,0,R1
    store R13,0,R1
    store R14,0,R1
    store R15,0,R1
    store R16,0,R1
    store R17,0,R1
    store R18,0,R1
    store R19,0,R1

    mult R0,0,R10
    mult R0,0,R11
    mult R0,0,R12
    mult R0,0,R13
    mult R0,0,R14
    mult R0,0,R15
    mult R0,0,R16
    mult R0,0,R17
    mult R0,0,R18
    mult R0,0,R19
    add R0,402,R10
    add R0,403,R11
    add R0,404,R12
    add R0,405,R13
    add R0,406,R14
    add R0,407,R15
    add R0,408,R16
    add R0,409,R17
    add R0,410,R18
    add R0,500,R19
    store R10,0,R1
    store R11,0,R1
    store R12,0,R1
    store R13,0,R1
    store R14,0,R1
    store R15,0,R1
    store R16,0,R1
    store R17,0,R1
    store R18,0,R1
    store R19,0,R1

    mult R0,0,R10
    mult R0,0,R11
    mult R0,0,R12
    mult R0,0,R13
    mult R0,0,R14
    mult R0,0,R15
    mult R0,0,R16
    mult R0,0,R17
    mult R0,0,R18
    mult R0,0,R19
    add R0,502,R10
    add R0,503,R11
    add R0,504,R12
    add R0,505,R13
    add R0,506,R14
    add R0,507,R15
    add R0,508,R16
    add R0,510,R17
    add R0,600,R18
    add R0,602,R19
    store R10,0,R1
    store R11,0,R1
    store R12,0,R1
    store R13,0,R1
    store R14,0,R1
    store R15,0,R1
    store R16,0,R1
    store R17,0,R1
    store R18,0,R1
    store R19,0,R1

    ; white
    mult R0,0,R10
    mult R0,0,R11
    mult R0,0,R12
    mult R0,0,R13
    mult R0,0,R14
    mult R0,0,R15
    mult R0,0,R16
    mult R0,0,R17
    mult R0,0,R18
    mult R0,0,R19
    add R0,603,R10
    add R0,604,R11
    add R0,605,R12
    add R0,606,R13
    add R0,702,R14
    add R0,708,R15
    store R10,0,R2
    store R11,0,R2
    store R12,0,R2
    store R13,0,R2
    store R14,0,R2
    store R15,0,R2

    mult R0,0,R10
    mult R0,0,R11
    mult R0,0,R12
    mult R0,0,R13
    mult R0,0,R14
    mult R0,0,R15
    mult R0,0,R16
    mult R0,0,R17
    mult R0,0,R18
    mult R0,0,R19
    add R0,608,R10
    add R0,610,R11
    add R0,703,R12
    add R0,704,R13
    add R0,706,R14
    add R0,707,R15
    store R10,0,R1
    store R11,0,R1
    store R12,0,R1
    store R13,0,R1
    store R14,0,R1
    store R15,0,R1

    scall 2

    add R3,1,R3
    jmp WHILE_1, R31


INVADER_2:
    mult R0,0,R10
    mult R0,0,R11
    mult R0,0,R12
    mult R0,0,R13
    mult R0,0,R14
    mult R0,0,R15
    mult R0,0,R16
    mult R0,0,R17
    mult R0,0,R18
    mult R0,0,R19
    add R0,2,R10
    add R0,8,R11
    add R0,103,R12
    add R0,107,R13
    add R0,202,R14
    add R0,203,R15
    add R0,204,R16
    add R0,205,R17
    add R0,206,R18
    add R0,207,R19
    store R10,0,R1
    store R11,0,R1
    store R12,0,R1
    store R13,0,R1
    store R14,0,R1
    store R15,0,R1
    store R16,0,R1
    store R17,0,R1
    store R18,0,R1
    store R19,0,R1

    mult R0,0,R10
    mult R0,0,R11
    mult R0,0,R12
    mult R0,0,R13
    mult R0,0,R14
    mult R0,0,R15
    mult R0,0,R16
    mult R0,0,R17
    mult R0,0,R18
    mult R0,0,R19
    add R0,208,R10
    add R0,301,R11
    add R0,302,R12
    add R0,304,R13
    add R0,305,R14
    add R0,306,R15
    add R0,308,R16
    add R0,309,R17
    add R0,400,R18
    add R0,401,R19
    store R10,0,R1
    store R11,0,R1
    store R12,0,R1
    store R13,0,R1
    store R14,0,R1
    store R15,0,R1
    store R16,0,R1
    store R17,0,R1
    store R18,0,R1
    store R19,0,R1

    mult R0,0,R10
    mult R0,0,R11
    mult R0,0,R12
    mult R0,0,R13
    mult R0,0,R14
    mult R0,0,R15
    mult R0,0,R16
    mult R0,0,R17
    mult R0,0,R18
    mult R0,0,R19
    add R0,402,R10
    add R0,403,R11
    add R0,404,R12
    add R0,405,R13
    add R0,406,R14
    add R0,407,R15
    add R0,408,R16
    add R0,409,R17
    add R0,410,R18
    add R0,500,R19
    store R10,0,R1
    store R11,0,R1
    store R12,0,R1
    store R13,0,R1
    store R14,0,R1
    store R15,0,R1
    store R16,0,R1
    store R17,0,R1
    store R18,0,R1
    store R19,0,R1

    mult R0,0,R10
    mult R0,0,R11
    mult R0,0,R12
    mult R0,0,R13
    mult R0,0,R14
    mult R0,0,R15
    mult R0,0,R16
    mult R0,0,R17
    mult R0,0,R18
    mult R0,0,R19
    add R0,502,R10
    add R0,503,R11
    add R0,504,R12
    add R0,505,R13
    add R0,506,R14
    add R0,507,R15
    add R0,508,R16
    add R0,510,R17
    add R0,600,R18
    add R0,602,R19
    store R10,0,R1
    store R11,0,R1
    store R12,0,R1
    store R13,0,R1
    store R14,0,R1
    store R15,0,R1
    store R16,0,R1
    store R17,0,R1
    store R18,0,R1
    store R19,0,R2

    ; white
    mult R0,0,R10
    mult R0,0,R11
    mult R0,0,R12
    mult R0,0,R13
    mult R0,0,R14
    mult R0,0,R15
    mult R0,0,R16
    mult R0,0,R17
    mult R0,0,R18
    mult R0,0,R19
    add R0,603,R10
    add R0,604,R11
    add R0,605,R12
    add R0,606,R13
    add R0,702,R14
    add R0,708,R15
    store R10,0,R2
    store R11,0,R1
    store R12,0,R2
    store R13,0,R1
    store R14,0,R1
    store R15,0,R1

    mult R0,0,R10
    mult R0,0,R11
    mult R0,0,R12
    mult R0,0,R13
    mult R0,0,R14
    mult R0,0,R15
    mult R0,0,R16
    mult R0,0,R17
    mult R0,0,R18
    mult R0,0,R19
    add R0,608,R10
    add R0,610,R11
    add R0,703,R12
    add R0,704,R13
    add R0,706,R14
    add R0,707,R15
    store R10,0,R2
    store R11,0,R1
    store R12,0,R1
    store R13,0,R2
    store R14,0,R2
    store R15,0,R1

    scall 2

    sub R3,1,R3
    jmp WHILE_1, R31

STOP:
    stop
