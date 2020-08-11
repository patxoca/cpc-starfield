    .area _CODE

    .globl _cpct_memcpy
    .globl _entities
    .globl _num_entities
    .globl _free_ptr

    SIZEOF_ENTITY_T = 9

    ;; ---------------------------------
    ;; entity_mgr_gc_sweep
    ;; ---------------------------------

    ;; void entity_mgr_gc_sweep(void) {
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
    ;;     free_ptr = end + 1;
    ;;     num_entities -= foo;
    ;; }

_entity_mgr_gc_sweep::
    ld	de, #_entities          ; DE apunta a la entitat actual
    ld  bc, #-SIZEOF_ENTITY_T
    ld	hl, (#_free_ptr)
    add hl, bc                  ; HL apunta a la última entitat
    ld  a, (#_num_entities)
    ld  b, a                    ; B nombre d'entitats pendents
    xor a
    ld  c, a                    ; C nombre d'entitats recuperades

__entity_mgr_gc_sweep_loop:
    ;; count-- == 0?
    ld a, b
    dec b
    or a
    jr z, __entity_mgr_gc_sweep_loop_end

    ;; queden entitats per processar
    ld a, (de)
    rlca                        ; entitat viva?
    jr nc, __entity_mgr_gc_sweep_entity_alive

    ;; entitat marcada per gc
    inc c                       ; incrementa comptador d'entitats
                                ; recuperades
    ld a, b                     ; queden entitats per processar?
    or a
    jr z, __entity_mgr_gc_sweep_skip_memcpy
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
__entity_mgr_gc_sweep_skip_memcpy:
    ;; endarrereix el punter a la última entitat (HL)
    ld a, l
    sub #SIZEOF_ENTITY_T
    ld l, a
    ld a, h
    sbc a, #0
    ld h, a

    jr __entity_mgr_gc_sweep_end_if

__entity_mgr_gc_sweep_entity_alive:
    ;; avança el punter a l'entitat actual (DE)
    ld a, #SIZEOF_ENTITY_T
    add a, e
    ld e, a
    adc a, d
    sub e
    ld d, a

__entity_mgr_gc_sweep_end_if:
    jr __entity_mgr_gc_sweep_loop

__entity_mgr_gc_sweep_loop_end:

    ;; free_ptr = end + 1

    ;; TODO: optimitzar? crec que al final curr apunta al primer
    ;; lliure i no cal sumar sizeof(entity_t) a end.

    ld	de, #SIZEOF_ENTITY_T
    add	hl, de
    ld	(_free_ptr), hl

    ;; actualitza (decrementa) num_entities
    ld hl, #_num_entities
    ld a, (hl)
    sub c
    ld (hl), a

    ret
