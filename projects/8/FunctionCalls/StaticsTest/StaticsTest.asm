@256
D=A
@SP
M=D
@LABEL1
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@SP
D=M
@0
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.init
0;JMP
(LABEL1)
(Class1.set)
@0
D=A
@SP
A=M
@0
D=A
@SP
M=D+M
@0
D=A
@ARG
A=D+M
D=M
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
@Class1.0
M=D
@1
D=A
@ARG
A=D+M
D=M
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
@Class1.1
M=D
@0
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@R13
M=D
@5
D=A
@LCL
A=M-D
D=M
@R14
M=D
@SP
A=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M
@SP
M=D+1
@R13
MD=M-1
A=D
D=M
@THAT
M=D
@R13
MD=M-1
A=D
D=M
@THIS
M=D
@R13
MD=M-1
A=D
D=M
@ARG
M=D
@R13
MD=M-1
A=D
D=M
@LCL
M=D
@R14
A=M
0;JMP
(Class1.get)
@0
D=A
@SP
A=M
@0
D=A
@SP
M=D+M
@Class1.0
D=M
@SP
M=M+1
A=M-1
M=D
@Class1.1
D=M
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=M-D
@LCL
D=M
@R13
M=D
@5
D=A
@LCL
A=M-D
D=M
@R14
M=D
@SP
A=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M
@SP
M=D+1
@R13
MD=M-1
A=D
D=M
@THAT
M=D
@R13
MD=M-1
A=D
D=M
@THIS
M=D
@R13
MD=M-1
A=D
D=M
@ARG
M=D
@R13
MD=M-1
A=D
D=M
@LCL
M=D
@R14
A=M
0;JMP
(Class2.set)
@0
D=A
@SP
A=M
@0
D=A
@SP
M=D+M
@0
D=A
@ARG
A=D+M
D=M
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
@Class2.0
M=D
@1
D=A
@ARG
A=D+M
D=M
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
@Class2.1
M=D
@0
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@R13
M=D
@5
D=A
@LCL
A=M-D
D=M
@R14
M=D
@SP
A=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M
@SP
M=D+1
@R13
MD=M-1
A=D
D=M
@THAT
M=D
@R13
MD=M-1
A=D
D=M
@THIS
M=D
@R13
MD=M-1
A=D
D=M
@ARG
M=D
@R13
MD=M-1
A=D
D=M
@LCL
M=D
@R14
A=M
0;JMP
(Class2.get)
@0
D=A
@SP
A=M
@0
D=A
@SP
M=D+M
@Class2.0
D=M
@SP
M=M+1
A=M-1
M=D
@Class2.1
D=M
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=M-D
@LCL
D=M
@R13
M=D
@5
D=A
@LCL
A=M-D
D=M
@R14
M=D
@SP
A=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M
@SP
M=D+1
@R13
MD=M-1
A=D
D=M
@THAT
M=D
@R13
MD=M-1
A=D
D=M
@THIS
M=D
@R13
MD=M-1
A=D
D=M
@ARG
M=D
@R13
MD=M-1
A=D
D=M
@LCL
M=D
@R14
A=M
0;JMP
(Sys.init)
@0
D=A
@SP
A=M
@0
D=A
@SP
M=D+M
@6
D=A
@SP
M=M+1
A=M-1
M=D
@8
D=A
@SP
M=M+1
A=M-1
M=D
@LABEL2
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@SP
D=M
@2
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class1.set
0;JMP
(LABEL2)
@SP
AM=M-1
D=M
@R5
M=D
@23
D=A
@SP
M=M+1
A=M-1
M=D
@15
D=A
@SP
M=M+1
A=M-1
M=D
@LABEL3
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@SP
D=M
@2
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class2.set
0;JMP
(LABEL3)
@SP
AM=M-1
D=M
@R5
M=D
@LABEL4
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@SP
D=M
@0
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class1.get
0;JMP
(LABEL4)
@LABEL5
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@SP
D=M
@0
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class2.get
0;JMP
(LABEL5)
(Sys.init$END)
@Sys.init$END
0;JMP
