#include "irq.h"
#include "gpu.h"

/* 32-byte entry:
     4-bit pixel 4-bit pixel (per byte)
 */
static unsigned char tiletab[GPU_TILETAB_MAX];
/* 8-byte entry:
            t1lo
            t2lo
            t3lo
            t4lo
       p p p|p p p p p
       h|v|p p p|p p p
     t4hi t3hi t2hi t1hi
             00
   TODO: h v
   t[1-4] tile number
   p palette number
   h v horizontal/vertical flip
 */
static unsigned char vram[GPU_VRAM_MAX];
/* 8-byte entry:
   x
   y
   d|s s s|c ch|xh yh
     xsz  |  ysz
   tlo
   p p p p|h v|thi
     cidx | coll
   00
   [td
    tdf
    bd
    bdf
    ld
    ldf
    rd
    rdf]
   TODO: sz d *d h v
   x y position
   t starting tile
   s tile size
   p palette from top 16
   sz screen size
   d distortion on (if on, entry becomes 16-byte wide, otherwise *d assumed 0)
   *d distortion (8.8 fixed point)
   h v flip
   c collide on
   ch hide collided pixels
   cidx collision index for this sprite
   coll with which collision index this sprite collided (0=none, 1-3=backgrounds)
 */
static unsigned char sprite[GPU_SPRITE_MAX];
static unsigned short cgram[GPU_CGRAM_MAX/2];
static unsigned xdisp[4], ydisp[4];
/* 0000 0000 0000 00hv */
static unsigned hires;

unsigned gpu_horiz(void) {
	return hires&2 ? 512 : 256;
}

unsigned gpu_vert(void) {
	return hires&1 ? 487 : 243;
}

/* Color coding:
   grgb rrrr gggg bbbb
   5444 0123 0123 0123
            |
            V
   rrrr rggg gggb bbbb
   0123 4012 3450 1234
 */
static unsigned short color(unsigned short c) {
	unsigned short r, g, b;
	r = c>>7&0x1e | c>>14&0x1;
	g = c>>2&0x3c | c>>12&0x2 | c>>15&0x1;
	b = c<<1&0x1e | c>>12&0x1;
	return r<<11 | g<<5 | b;
}

/* Since the GPU is clocked at 12MHz (same as CPU), a line has 762⅔ cycles to
   do its thing (on NTSC) or 768 cycles on PAL. 131 (145 on PAL) are in the
   Hblank, so in practice only 631 (623) are useful (because Hblank must be
   available to the CPU for random effects).

   It uses double-buffering of a scanline; while one buffer is being output,
   the other one is being filled with sprites. Tiles are rendered afterwards,
   while the pixels are being sent to screen.

   For practical purposes, this only means that sprite changes only apply on
   the following scanline. This is so little used that we don't even worry
   about emulating it here.
 */
void gpu_scanline(unsigned short *line, int i_) {
	static unsigned char coll[512] = {0};
	unsigned i1 = i_+ydisp[0] & 0x1ff;
	unsigned j1 = xdisp[0]-1 & 0x1ff;
	unsigned i2 = i_+ydisp[1] & 0x1ff;
	unsigned j2 = xdisp[1]-1 & 0x1ff;
	unsigned i3 = i_+ydisp[2] & 0x1ff;
	unsigned j3 = xdisp[2]-1 & 0x1ff;
	unsigned i4 = i_+ydisp[3] & 0x1ff;
	unsigned j4 = xdisp[3]-1 & 0x1ff;

	/* always takes exactly 512cyc, done during display */
	for(int j_ = 0; j_ < (hires&2 ? 512 : 256); j_++) {
		j1 = (j1 + 1) & 0x1ff;
		unsigned id = (i1&~7)<<6 | j1&~7;
		unsigned short bt = vram[id]
		                  | (unsigned short)(vram[id|6] & 0x3) << 8;
		int b1 = tiletab[bt<<5 | (i1&7)<<2 | (j1&7)>>1] >> ((~j1&1)<<2) & 0xf;
		unsigned short bc1 = cgram[(unsigned short)(vram[id|4] & 0x1f) << 4 | b1];

		j2 = (j2 + 1) & 0x1ff;
		id = (i2&~7)<<6 | j2&~7;
		bt = vram[id|1]
		   | (unsigned short)(vram[id|6] & 0xb) << 6;
		int b2 = tiletab[bt<<5 | (i2&7)<<2 | (j2&7)>>1] >> ((~j2&1)<<2) & 0xf;
		unsigned short bc2 = cgram[(unsigned short)(vram[id|4] >> 5) << 4 | b2];

		j3 = (j3 + 1) & 0x1ff;
		id = (i3&~7)<<6 | j3&~7;
		bt = vram[id|2]
		   | (unsigned short)(vram[id|6] & 0x30) << 4;
		int b3 = tiletab[bt<<5 | (i3&7)<<2 | (j3&7)>>1] >> ((~j3&1)<<2) & 0xf;
		unsigned short bc3 = cgram[(unsigned short)(vram[id|5] & 7) << 4 | b3];

		j4 = (j4 + 1) & 0x1ff;
		id = (i4&~7)<<6 | j4&~7;
		bt = vram[id|3]
		   | (unsigned short)(vram[id|6] & 0xb0) << 2;
		int b4 = tiletab[bt<<5 | (i4&7)<<2 | (j4&7)>>1] >> ((~j4&1)<<2) & 0xf;
		unsigned short bc4 = cgram[(unsigned short)(vram[id|5] >> 3 & 7) << 4 | b4];

		line[j_] = color(b1 ? bc1 : b2 ? bc2 : b3 ? bc3 : bc4);
		coll[j_] = b1 ? 3 : b2 ? 2 : b3 ? 1 : 0;
	}

	/* this may take at most 631cyc, after that is Hblank and then whatever
	   is done will be displayed */
	for(int s = 0, cyc = 0; s < GPU_SPRITE_MAX && cyc < 631; ) {
		int y = sprite[s|1] | (unsigned short)(sprite[s|2]&1) << 8;
		int sz = sprite[s|2]>>4 & 7;
		cyc += 1;
		if(y <= i_ && i_ < y + (8 << ((sz>>1) + (sz&1)))) {
			int x = sprite[s] | (unsigned short)(sprite[s|2]&2) << 7;
			int cidx = 0;
			if(sprite[s|2]&8) {
				cidx = sprite[s|6] >> 4;
				cyc += 2;
			}
			unsigned short bt = sprite[s|4]
			                  | (unsigned short)(sprite[s|5] & 3) << 8;
			bt += i_-y >> 3;
			for(int n0 = 0; n0 < 1 << (sz>>1); n0++) {
				unsigned tile = (unsigned)tiletab[bt<<5 | (i_-y & 7)<<2] << 24
				              | (unsigned)tiletab[bt<<5 | (i_-y & 7)<<2 | 1] << 16
				              | (unsigned)tiletab[bt<<5 | (i_-y & 7)<<2 | 2] << 8
				              | tiletab[bt<<5 | (i_-y & 7)<<2 | 3];
				for(int n1 = 0; n1 < 8; n1++) {
					int c = (tile&0xf0000000) >> 28;
					if(c) {
						int collided = (sprite[s|2]&8) && cidx <= coll[x & 0x1ff];
						if(collided)
							sprite[s|6] = cidx<<4 | coll[x & 0x1ff];
						else
							coll[x & 0x1ff] = cidx;
						if(!((sprite[s|2]&4) && collided))
							line[x & 0x1ff] = color(cgram[sprite[s|5]&0xf0 | 0x100 | c]);
					}
					tile <<= 4;
					x++;
				}
				bt += 1 << (sz>>1);
				cyc += 2;
			}
			cyc += 2;
		}
		s += sprite[s|2] & 0x80 ? 16 : 8;
	}
}

void gpu_start_field(void) {
	irq_set(7, 0);
}

void gpu_end_field(void) {
	irq_set(7, 1);
}

unsigned gpu_read(unsigned addr) {
	if(GPU_TILETAB_START <= addr && addr < GPU_TILETAB_START + GPU_TILETAB_MAX) {
		return (tiletab[addr-GPU_TILETAB_START] << 8)
		     | (tiletab[addr-GPU_TILETAB_START+1]);
	} else if(GPU_VRAM_START <= addr && addr < GPU_VRAM_START + GPU_VRAM_MAX) {
		return (vram[addr-GPU_VRAM_START] << 8)
		     | (vram[addr-GPU_VRAM_START+1]);
	} else if(GPU_SPRITE_START <= addr && addr < GPU_SPRITE_START + GPU_SPRITE_MAX) {
		return (sprite[addr-GPU_SPRITE_START] << 8)
		     | (sprite[addr-GPU_SPRITE_START+1]);
	} else if(GPU_CGRAM_START <= addr && addr < GPU_CGRAM_START + GPU_CGRAM_MAX) {
		return cgram[(addr-GPU_CGRAM_START)/2];
	} else if(GPU_PARAM_START <= addr && addr < GPU_PARAM_START + GPU_PARAM_MAX) {
		if(addr == GPU_PARAM_START)
			return xdisp[0]<<7 | ydisp[0]>>1;
		else if(addr == GPU_PARAM_START+2)
			return xdisp[1]<<7 | ydisp[1]>>1;
		else if(addr == GPU_PARAM_START+4)
			return xdisp[2]<<7 | ydisp[2]>>1;
		else if(addr == GPU_PARAM_START+6)
			return xdisp[3]<<7 | ydisp[3]>>1;
	}
	return 0x4040;
}

void gpu_write(unsigned addr, unsigned val) {
	if(GPU_TILETAB_START <= addr && addr < GPU_TILETAB_START + GPU_TILETAB_MAX) {
		tiletab[addr-GPU_TILETAB_START] = val >> 8;
		tiletab[addr-GPU_TILETAB_START+1] = val & 0xff;
	} else if(GPU_VRAM_START <= addr && addr < GPU_VRAM_START + GPU_VRAM_MAX) {
		vram[addr-GPU_VRAM_START] = val >> 8;
		vram[addr-GPU_VRAM_START+1] = val & 0xff;
	} else if(GPU_SPRITE_START <= addr && addr < GPU_SPRITE_START + GPU_SPRITE_MAX) {
		sprite[addr-GPU_SPRITE_START] = val >> 8;
		sprite[addr-GPU_SPRITE_START+1] = val & 0xff;
	} else if(GPU_CGRAM_START <= addr && addr < GPU_CGRAM_START + GPU_CGRAM_MAX) {
		if((addr-GPU_CGRAM_START) % 16 || addr == GPU_CGRAM_START)
			cgram[(addr-GPU_CGRAM_START)/2] = val;
	} else if(GPU_PARAM_START <= addr && addr < GPU_PARAM_START + GPU_PARAM_MAX) {
		if(addr == GPU_PARAM_START) {
			xdisp[0] = (val >> 7) & 0x1fe;
			ydisp[0] = (val & 0xff) << 1;
		} else if(addr == GPU_PARAM_START+2) {
			xdisp[1] = (val >> 7) & 0x1fe;
			ydisp[1] = (val & 0xff) << 1;
		} else if(addr == GPU_PARAM_START+4) {
			xdisp[2] = (val >> 7) & 0x1fe;
			ydisp[2] = (val & 0xff) << 1;
		} else if(addr == GPU_PARAM_START+6) {
			xdisp[3] = (val >> 7) & 0x1fe;
			ydisp[3] = (val & 0xff) << 1;
		} else if(addr == GPU_PARAM_START+8) {
			hires = val;
		}
	}
}

