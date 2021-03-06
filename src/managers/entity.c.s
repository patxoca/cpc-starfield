    .area _CODE

    .globl _entities
    .globl _num_entities
    .globl _free_ptr

    SIZEOF_ENTITY_T = 10

    ;; ---------------------------------
    ;; entity_mgr_gc
    ;; ---------------------------------

    ;; void entity_mgr_gc(void) {
    ;;     entity_t *curr = entities;
    ;;     entity_t *end = free_ptr - 1;
    ;;     u8 count = num_entities;
    ;;     u8 foo = 0;
    ;;
    ;;     while (count--) {
    ;;         if (!ENTITY_IS_ALIVE(curr)) {
    ;;             foo++;
    ;;             if (count) {
    ;;                 cpct_memcpy(curr, end, sizeof(entity_t));
    ;;             }
    ;;             end--;
    ;;         } else {
    ;;             curr++;
    ;;         }
    ;;     }
    ;;     free_ptr = curr;
    ;;     num_entities -= foo;
    ;; }

_entity_mgr_gc::
    ld	de, #_entities          ; DE apunta a la entitat actual
    ld  bc, #-SIZEOF_ENTITY_T
    ld	hl, (#_free_ptr)
    add hl, bc                  ; HL apunta a la última entitat
    ld  a, (#_num_entities)
    ld  b, a                    ; B nombre d'entitats pendents
    xor a
    ld  c, a                    ; C nombre d'entitats recuperades

__entity_mgr_gc_loop:
    ;; count-- == 0?
    ld a, b
    dec b
    or a
    jr z, __entity_mgr_gc_loop_end

    ;; queden entitats per processar
    ld a, (de)
    rlca                        ; entitat viva?
    jr nc, __entity_mgr_gc_entity_alive

    ;; entitat marcada per gc
    inc c                       ; incrementa comptador d'entitats
                                ; recuperades
    ld a, b                     ; queden entitats per processar?
    or a
    jr z, __entity_mgr_gc_skip_memcpy
                                ; si no queden entitas (és la última)
                                ; ens estalviem el memcpy

    ;; mempcy

    ;; TODO: optimitzar? Eliminar push/pop HL, DE i provar reajustant
    ;; el valor de HL i DE amb restes. Crec que no suposa una gran
    ;; millora.

    push hl
    push de
    push bc
    ld bc, #SIZEOF_ENTITY_T
    ldir
    pop bc
    pop de
    pop hl
__entity_mgr_gc_skip_memcpy:
    ;; endarrereix el punter a la última entitat (HL)
    ld a, l
    sub #SIZEOF_ENTITY_T
    ld l, a
    ld a, h
    sbc a, #0
    ld h, a

    jr __entity_mgr_gc_end_if

__entity_mgr_gc_entity_alive:
    ;; avança el punter a l'entitat actual (DE)
    ld a, #SIZEOF_ENTITY_T
    add a, e
    ld e, a
    adc a, d
    sub e
    ld d, a

__entity_mgr_gc_end_if:
    jr __entity_mgr_gc_loop

__entity_mgr_gc_loop_end:

    ;; free_ptr = end + 1
    ld (#_free_ptr), de

    ;; actualitza (decrementa) num_entities
    ld hl, #_num_entities
    ld a, (hl)
    sub c
    ld (hl), a

    ret



    ;; --------------------------------
    ;; entity_mgr_foreach
    ;; --------------------------------
    ;;
    ;; Utilitza el conveni de crida __z88dk_fastcall
    ;;
    ;;  - HL: punter a la funció
    ;;
    ;; La funció s'automodifica i no és reentrant.

    ;; void entity_mgr_foreach(void (*f)(entity_t *e) __z88dk_fastcall) __z88dk_fastcall {
    ;;     entity_t *p = entities;
    ;;     u8 count = num_entities;
    ;;     void (*lf)(entity_t *e)  __z88dk_fastcall = f;
    ;;
    ;;     while (count--) {
    ;;         lf(p);
    ;;         p++;
    ;;     }
    ;; }

_entity_mgr_foreach::
    ld (#__entity_mgr_foreach_call + 1), hl
                                ; modifica la instrucción call
    ld hl, #_entities+0         ; HL punter a l'entitat actual
	ld a,(#_num_entities)
    ld b, a                     ; B comptador entitats romanents

__entity_mgr_foreach_loop:
	ld a, b                     ; while (count != 0)
    or a, a
	jr z, __entity_mgr_foreach_exit_loop

    push hl                     ; preserva HL
    push bc                     ; preserva B
__entity_mgr_foreach_call:
	call #0xffff                ; l'adreça es modificada
    pop bc
    pop hl

	dec b                       ; count--
    ld a, #SIZEOF_ENTITY_T      ; p++
    add l
    ld l, a
    adc h
    sub l
    ld h, a

	jr	__entity_mgr_foreach_loop

__entity_mgr_foreach_exit_loop:
	ret
