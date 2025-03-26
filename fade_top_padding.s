section .text
    global fade_top

fade_top:
    push    ebp
    mov     ebp, esp
    
    sub     esp,8  ;declaing local variable epb-4 for padding and ebp-8 for width*3

    push    ebx
    push    esi
    push    edi

    mov     esi, [ebp + 8] ; Get the image pointer (img)
    mov     ebx, [ebp + 12] ; Get the width (width)
    mov     ecx, [ebp + 16] ; Get the height (height)
    mov     edi, [ebp + 20] ; Get the dist (dist)

    ; if no fade then do nothing
    test    edi, edi 
    jz      fin

    ; multiplaying by 3 (the size of the color pixel)
    lea     ebx, [ebx*2 + ebx]
    mov     [ebp-8], ebx
    
    ; calculating padding - (4 - width*3%4)%4
    mov     edx, 4
    mov     eax, ebx
    and     eax, 3
    sub     edx, eax
    and     edx, 3
    ; dl - padding

    mov     [ebp-4], edx ; lading whole edx because later we add it to esi - need whole by
    

    add     ebx, edx
    ; Calculate the start row for the fade effect into edx
    sub     ecx, edi
    

    ; moving to a correct row for applying fade
    imul    ecx, ebx
    add     esi, ecx

    ; chagning so that ebx have no padding
    sub     ebx, edx

    
    ;adding 1 beacause the fade for h-dist line is equal to 0
    inc     edi

    
    mov     ecx, 1  ; starting from the k row from top
    
calculate_fade:
    xor eax, eax

    mov al, [esi]
    mov edx, edi
    sub edx, ecx    ; dist - n
    imul eax, edx   ; (dist - n) * original


    mov edx, ecx    
    sub eax, edx    

    shl edx, 8
    add eax, edx    ;(dist - n) * original  + (256 -1)*n

    xor edx, edx 
    div edi

    cmp eax, 0x000000FF
    jb save
    
    mov     al, 0xFF
    ; (FF - al) * skala
    ; add to al
save:
    mov     [esi], al
    inc     esi
    dec     ebx
    jnz     calculate_fade
    ;loading and skipping the padding length
    add     esi, [ebp-4] ; local variable for padding

    inc     ecx
    mov     ebx, [ebp-8]
    cmp     ecx, edi
    jne     calculate_fade

fin:
    pop     edi
    pop     esi
    pop     ebx 
    mov     esp, ebp  
    pop     ebp
    ret
