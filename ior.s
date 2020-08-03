	org $00c00000

	dc.l $00020000
	dc.l init

init:
	move.w #0, $00c00000
	move.l #noint, $0000007c
	jmp $00100000
	; initialise tilemap with something
	movea.l #$00f00000, a0

.1:	move.l #$00000000, (a0)+
	move.l #$00000300, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$30000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00030000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00300000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000300, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$03300000, (a0)+
	move.l #$03300000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$12111121, (a0)+
	move.l #$22211222, (a0)+
	move.l #$12111122, (a0)+
	move.l #$12222112, (a0)+
	move.l #$11122111, (a0)+
	move.l #$11221121, (a0)+
	move.l #$11121222, (a0)+
	move.l #$11111122, (a0)+

	move.l #$00000000, (a0)+
	move.l #$11111000, (a0)+
	move.l #$00100000, (a0)+
	move.l #$00100001, (a0)+
	move.l #$00100010, (a0)+
	move.l #$00100010, (a0)+
	move.l #$00100010, (a0)+
	move.l #$11111010, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000011, (a0)+
	move.l #$00000100, (a0)+
	move.l #$00000100, (a0)+
	move.l #$00000100, (a0)+
	move.l #$00000100, (a0)+
	move.l #$00000100, (a0)+
	move.l #$00000011, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$11000110, (a0)+
	move.l #$00101000, (a0)+
	move.l #$00100110, (a0)+
	move.l #$00100001, (a0)+
	move.l #$00101110, (a0)+

	move.l #$00000000, (a0)+
	move.l #$10000000, (a0)+
	move.l #$01000000, (a0)+
	move.l #$00001100, (a0)+
	move.l #$00000010, (a0)+
	move.l #$00001110, (a0)+
	move.l #$01010010, (a0)+
	move.l #$10001110, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00110001, (a0)+
	move.l #$01001010, (a0)+
	move.l #$01110010, (a0)+
	move.l #$01000010, (a0)+
	move.l #$00111010, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000001, (a0)+
	move.l #$00000001, (a0)+
	move.l #$01110111, (a0)+
	move.l #$10000001, (a0)+
	move.l #$10000001, (a0)+
	move.l #$10000001, (a0)+
	move.l #$10000001, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000100, (a0)+
	move.l #$00000100, (a0)+
	move.l #$11011111, (a0)+
	move.l #$00000100, (a0)+
	move.l #$00000100, (a0)+
	move.l #$00000100, (a0)+
	move.l #$00000100, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$11000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+
	move.l #$00000000, (a0)+

	; initialise VRAM with tiles
	movea.l #$00f08000, a0
	move.l #$4bc53dc1, d0
.2:	move.w d0, d1
	and.w #$0303, d1
	or.w #$0400, d1
	move.w d1, (a0)+
	move.w #$0800, (a0)+
	move.w #$0000, (a0)+
	move.w #0, (a0)+
	move.l d0, d1
	lsr.l #3, d0
	add.l d1, d0
	cmpa.l #$00f10000, a0
	bne.s .2

	; sprites
	move.l #$00004000, (a0)+
	move.l #$09000000, (a0)+

	; initialise CGRAM
	movea.l #$00f18000, a0
	move.w #$0000, (a0)+
	; 01000 010000 01000
	; 0100 0010 0000 1000
	move.w #$4208, (a0)+
	; 01010 010100 01010
	; 0101 0010 1000 1010
	move.w #$528a, (a0)+
	move.w #$ffff, (a0)+
	; sprite palettes
	movea.l #$00f18200, a0
	move.w #$0, (a0)+
	move.w #$ffff, (a0)+

	move.w #$0000, d0
	move.w #$0001, d1
	move.w #$0002, d2
	move.w #$0003, d3
	move.w sr, d4
	and.w #$f8ff, d4
	or.w #$0600, d4
	move.w d4, sr
	move.b #0, d4

	jmp $

noint:
	rte

vtrace:
	add.b #1, d4

	move.b d4, d5
	and.b #$3, d5
	bne.s .1
	sub.w #$0101, d0
	move.w d0, $00f20000
.1:
	move.b d4, d5
	and.b #$7, d5
	bne.s .2
	sub.w #$0101, d1
	move.w d1, $00f20002
.2:
	move.b d4, d5
	and.b #$f, d5
	bne.s .3
	add.w #$0101, d2
	move.w d2, $00f20004
.3:
	move.b d4, d5
	and.b #$1f, d5
	bne.s .4
	add.w #$0101, d3
	move.w d3, $00f20006

.4:
	move.w $00e00000, d5
	move.w $00f10000, d7

	move.w d5, d6
	and.w #$8, d6
	beq.s .5
	sub.b #1, d7

.5:
	move.w d5, d6
	and.w #$4, d6
	beq.s .6
	add.b #1, d7

.6:
	move.w d5, d6
	and.w #$2, d6
	beq.s .7
	sub.w #$0100, d7

.7:
	move.w d5, d6
	and.w #$1, d6
	beq.s .8
	add.w #$0100, d7

.8:
	move.w d7, $00f10000

	rte

