// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

(INFLOOP)
    @KBD
    D=M
    @WHITE
    D;JEQ
    @BLACK
    0;JMP
(END)
@INFLOOP
0;JMP

(WHITE)
    @i
    M=0
    (LOOP2)
    @i
    D=M
    @SCREEN
    A=A+D
    M=0
    @i
    MD=M+1
    @8191
    D=D-A
    @END2
    D;JGT
    @LOOP2
    0;JMP
    (END2)
    @END
    0;JMP

(BLACK)
    @i
    M=0
    (LOOP3)
    @i
    D=M
    @SCREEN
    A=A+D
    M=-1
    @i
    MD=M+1
    @8191
    D=D-A
    @END3
    D;JGT
    @LOOP3
    0;JMP
    (END3)
    @END
    0;JMP
