@256
D=A
@SP
M=D
@17
D=A
@SP
M=M+1
A=M-1
M=D
@17
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LABEL1
D;JEQ
D=0
@LABEL2
0;JMP
(LABEL1)
D=-1
(LABEL2)
@SP
A=M-1
M=D
@17
D=A
@SP
M=M+1
A=M-1
M=D
@16
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LABEL3
D;JEQ
D=0
@LABEL4
0;JMP
(LABEL3)
D=-1
(LABEL4)
@SP
A=M-1
M=D
@16
D=A
@SP
M=M+1
A=M-1
M=D
@17
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LABEL5
D;JEQ
D=0
@LABEL6
0;JMP
(LABEL5)
D=-1
(LABEL6)
@SP
A=M-1
M=D
@892
D=A
@SP
M=M+1
A=M-1
M=D
@891
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LABEL7
D;JLT
D=0
@LABEL8
0;JMP
(LABEL7)
D=-1
(LABEL8)
@SP
A=M-1
M=D
@891
D=A
@SP
M=M+1
A=M-1
M=D
@892
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LABEL9
D;JLT
D=0
@LABEL10
0;JMP
(LABEL9)
D=-1
(LABEL10)
@SP
A=M-1
M=D
@891
D=A
@SP
M=M+1
A=M-1
M=D
@891
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LABEL11
D;JLT
D=0
@LABEL12
0;JMP
(LABEL11)
D=-1
(LABEL12)
@SP
A=M-1
M=D
@32767
D=A
@SP
M=M+1
A=M-1
M=D
@32766
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LABEL13
D;JGT
D=0
@LABEL14
0;JMP
(LABEL13)
D=-1
(LABEL14)
@SP
A=M-1
M=D
@32766
D=A
@SP
M=M+1
A=M-1
M=D
@32767
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LABEL15
D;JGT
D=0
@LABEL16
0;JMP
(LABEL15)
D=-1
(LABEL16)
@SP
A=M-1
M=D
@32766
D=A
@SP
M=M+1
A=M-1
M=D
@32766
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LABEL17
D;JGT
D=0
@LABEL18
0;JMP
(LABEL17)
D=-1
(LABEL18)
@SP
A=M-1
M=D
@57
D=A
@SP
M=M+1
A=M-1
M=D
@31
D=A
@SP
M=M+1
A=M-1
M=D
@53
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=D+M
@112
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=M-D
@SP
A=M-1
M=-M
@SP
AM=M-1
D=M
A=A-1
M=D&M
@82
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=D|M
@SP
A=M-1
M=!M
