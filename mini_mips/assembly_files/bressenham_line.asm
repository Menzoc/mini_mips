;=======================================================
;
; 		Tracé de droites de Bressenham
;
;		Elouan-Mathis-Elodie
; 		Version 09/02/2022
;
;		x1:R1	e:R7		G_color:R13
;		y1:R2	pixel_color:R8	B_color:R14
;		x2:R3	temp1:R9	16^2:R15
;		y2:R4	pixel_nb:R10	16^4:R16
;		dx:R5	temp:R11
;		dy:R6	R_color:R12
;
;=======================================================

init:
	add R0,1,R1		 ; Assign x1=1
	add R0,90,R3		 ; Assign x2=90
	add R0,1,R2		 ; Assign y1=1
	add R0,90,R4		 ; Assign y2=90
	add R0,255,R12		 ; R_color=255
	add R0,255,R13		 ; G_color=255
	add R0,255,R14		 ; B_color=255

find_dec_from_rgb:
	add R0,16,R15		 ; R15 = 16
	mult R15,R15,R15	 ; R15 = 16^2
	mult R15,R15,R16	 ; R16 = 16^4

	mult R12,R16,R8	 	 ; R8 = R12*R16
	mult R13,R15,R9	 	 ; R9 = R13*R15
	add R17,R18,R8		 ; R8 = R12*R16 + R13*R15
	add R8,R14,R8		 ; pixel_color = R12*R16 + R13*R15 + R14

begin_prog:
	sub R4,R2,R6		 ; dy = y2-y1
	mult R6,2,R6		 ; dy = (y2-y1)*2
	sub R3,R1,R7		 ; e = x2-x1
	mult R7,2,R5		 ; dx = 2*e

while:
	sle R1,R3,R11		 ; while x1 ≤ x2
	braz R11,end		 ; stop while
	add R1,1,R1		 ; x1 increment
	sub R7,R6,R7		 ; e = e - dy, error for next_pixel in same y1
	sle R7,0,R11		 ; if e <=0, R11 = 1
	branz R11,choose_next_pixel;
end_ch:	scall 2			; refresh screen
	jmp put_pixel_on,R0	 ; put pixel (x1,y1) on

choose_next_pixel:
	add R2,1,R2		; increment y1 to choose next pixel for same y1
	add R7,R5,R7		; adjust the error e
	jmp end_ch,R0		; go to end_ch

put_pixel_on:
	mult R2,100,R10		; screen is 100x100, so pixel at x,y is at 100*y+x
	add R10,R1,R10		; pixel_nb = R10 = 100*y+x
	store R10,0,R8		; store pixel_color to pixel_nb to switch it on
	jmp while,R0		; go to while

end:
	stop			; end of program