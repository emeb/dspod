
	

    THUMB

	EXPORT st_hsp_memcpy32
	AREA ||.text||, CODE, READONLY, ALIGN=4

st_hsp_memcpy32 PROC

    PUSH {R4, R5, R6, R7, R8, R9, R10, R11}

    BICS R11, R2, #63
    BEQ l2
   
l1
    LDMIA R1!,  { R3, R4, R5, R6, R7, R8, R9, R10 }
    STMIA R0!, { R3, R4, R5, R6, R7, R8, R9, R10 }
    LDMIA R1!,  { R3, R4, R5, R6, R7, R8, R9, R10 }
    SUBS R11, #64
    STMIA R0!, { R3, R4, R5, R6, R7, R8, R9, R10 }
    BGT l1
    
l2

    TST R2, #32
    ITT NE
    LDMIANE R1!, { R3, R4, R5, R6, R7, R8, R9, R10 }
    STMIANE R0!, { R3, R4, R5, R6, R7, R8, R9, R10 }

    TST R2, #16
    ITT NE
    LDMIANE R1!, { R3, R4, R5, R6 }
    STMIANE R0!, { R3, R4, R5, R6 }

    TST R2, #8
    ITT NE
    LDRDNE R3, R4, [R1], #+8
    STRDNE R3, R4, [R0], #+8

    TST R2, #4
    ITT NE
    LDRNE R3, [R1], #+4
    STRNE R3, [R0], #+4

    TST R2, #2
    ITT NE
    LDRHNE R3, [R1], #+2
    STRHNE R3, [R0], #+2

    TST R2, #1
    ITT NE
    LDRBNE R3, [R1], #+1
    STRBNE R3, [R0], #+1

    POP {R4, R5, R6, R7, R8, R9, R10, R11}
    BX LR
    ENDP 
    
    END