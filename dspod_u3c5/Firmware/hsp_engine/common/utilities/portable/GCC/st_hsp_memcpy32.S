
    .global st_hsp_memcpy32
    .syntax unified
    .section .text.st_hsp_memcpy32
    .weak   st_hsp_memcpy32
    .type   st_hsp_memcpy32, %function
    .thumb
   
st_hsp_memcpy32:

    push {r4, r5, r6, r7, r8, r9, r10, r11}

    bics r11, r2, #63
    beq l2
   
l1:
    ldmia r1!,  { r3, r4, r5, r6, r7, r8, r9, r10 }
    stmia r0!, { r3, r4, r5, r6, r7, r8, r9, r10 }
    ldmia r1!,  { r3, r4, r5, r6, r7, r8, r9, r10 }
    subs r11, #64
    stmia r0!, { r3, r4, r5, r6, r7, r8, r9, r10 }
    bgt l1
    
l2:

    tst r2, #32
    itt NE
    ldmiane r1!, { r3, r4, r5, r6, r7, r8, r9, r10 }
    stmiane r0!, { r3, r4, r5, r6, r7, r8, r9, r10 }

    tst r2, #16
    itt NE
    ldmiane r1!, { r3, r4, r5, r6 }
    stmiane r0!, { r3, r4, r5, r6 }

    tst r2, #8
    itt NE
    ldrdne r3, r4, [r1], #+8
    strdne r3, r4, [r0], #+8

    tst r2, #4
    itt NE
    ldrne r3, [r1], #+4
    strne r3, [r0], #+4

    tst r2, #2
    itt NE
    ldrhne r3, [r1], #+2
    strhne r3, [r0], #+2

    tst r2, #1
    itt NE
    ldrbne r3, [r1], #+1
    strbne r3, [r0], #+1

    pop {r4, r5, r6, r7, r8, r9, r10, r11}
    bx lr
     
    
