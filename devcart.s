	include def.i

lastkey$w = umem
autorepeat$b = umem+2
text = umem+4

	org $00100000

texteditor:
	move.l #vblank, int$vblank

	movea.l #font, a0
	movea.l #tilemap, a1
	move.w #255, d0
.lfont:
	move.l (a0)+, (a1)+
	move.l (a0)+, (a1)+
	move.l (a0)+, (a1)+
	move.l (a0)+, (a1)+
	move.l (a0)+, (a1)+
	move.l (a0)+, (a1)+
	move.l (a0)+, (a1)+
	move.l (a0)+, (a1)+
	dbra d0, .lfont

	move.l #$0000FFFF, cgram
	move.l #$0000FFFF, cgram+16*32

	move.w #scr$hires$horiz, scr$hires$w

	movea.l #text, a0
	move.w #0, d0
.getkey:
	btst.b #8, con0
	bne.s .checkkey
	move.w #0, lastkey$w
	bra.s .getkey
.checkkey:
	move.w lastkey$w, d0
	cmp.w con0, d0
	beq.s .getkey
	; key was pressed
	move.w con0, d0
	move.w d0, lastkey$w
	cmp.b #$08, d0
	beq.s .bs
	cmp.b #$7F, d0
	beq.s .bs
	move.b d0, (a0)+
	bra.s .getkey
.bs:
	move.b #' ', -(a0)
	bra.s .getkey

vblank:
	subi.b #1, autorepeat$b
	bne.s .norep
	move.w #0, lastkey$w
	move.b #20, autorepeat$b
.norep:

	move.l d0, -(sp)
	move.l a1, -(sp)
	move.l a2, -(sp)
	movea.l a0, a2

	movea.l #text, a0
	movea.l #vram, a1
.loop:
	cmpa.l #vram + (vram$end-vram)/2, a1
	beq.s .exit
	cmpa.l a0, a2
	beq.s .cursor
	move.b #$00, (1, a1)
.1:	move.b (a0)+, d0
	beq.s .exit
	cmp.b #$0A, d0
	beq.s .nl
	cmp.b #$0D, d0
	beq.s .nl
	move.b d0, (a1)+
	adda.w #7, a1
	bra.s .loop
.nl:
	move.l a1, d0
	add.l #$0200, d0
	and.l #$FFFFE00, d0
	movea.l d0, a1
	bra.s .loop
.cursor:
	move.b #$7F, (1, a1)
	bra.s .1
.exit:
	movea.l a2, a0
	movea.l (sp)+, a2
	movea.l (sp)+, a1
	move.l (sp)+, d0
	rte

	include font.s

