;=======================================================
;                         Syracuse
;
;		r0 = r0/2 si r0 pair
;		r0 = 3*r0 + 1 si r0 impair
;=======================================================

	add r0,15,r0		 ; r0 = 15
loop:
	;seq r3,10,r4		 ; r4 = 1 si r3 = 10
	seq r0,1,r4
	branz r4, label_end      ; si r3 > 10, fin de boucle
	add r3,1,r3		 ; increment r3

	div r0,2,r1	         ; r1 = r0/2 - division entiere
	mult r1,2,r1             ; r1 = 2*r1 - multiplication entiere
	sub r0,r1,r2             ; r2 = 1 si r0 pair et vaut 0 sinon
	braz r2, calcul_pair     ; go to calcul_pair si r2 = 0
	branz r2, calcul_impair  ; go to calcul_impair si r2 = 1

;=======================================================
; procedure calcul pair :
;=======================================================

calcul_pair:
	div r0,2,r0              ; r0 = r0/2
	jmp loop,r5              ; on poursuit la boucle

;=======================================================
; procedure calcul impair :
;=======================================================

calcul_impair:

	mult r0,3,r0             ; r0 = 3*r0
	add r0,1,r0              ; r0 += 1
	jmp loop,r5              ; on poursuit la boucle

label_end:
	stop

