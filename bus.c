#include <stdio.h>
#include <stdlib.h>
#include "bus.h"
#include "cart.h"
#include "controller.h"
#include "gpu.h"
#include "m68k.h"

#define RAM_START 0
#define RAM_MAX 0x20000
#define ROM_START 0x00c00000
#define ROM_START2 0
#define ROM_MAX 0x8000
#define LOW_ROM_SWITCH 0x00c00000

static unsigned char rom[ROM_MAX];
static unsigned char ram[RAM_MAX];
static unsigned short lowrom = 1;

void rom_init(void) {
	FILE *f = fopen("ior.rom", "r");
	if(!f)
		exit(1);
	if(fread(rom, ROM_MAX, 1, f) < 0)
		exit(1);
}

unsigned m68k_read_memory_8(unsigned addr) {
	return (m68k_read_memory_16(addr & ~1) >> (addr % 2 ? 0 : 8)) & 0xff;
}

unsigned m68k_read_memory_16(unsigned addr) {
	unsigned val;
	if(ROM_START <= addr && addr < ROM_START + ROM_MAX)
		return (rom[addr-ROM_START] << 8) | rom[addr-ROM_START+1];
	else if(lowrom && ROM_START2 <= addr && addr < ROM_START2 + ROM_MAX)
		return (rom[addr-ROM_START2] << 8) | rom[addr-ROM_START2+1];
	else if(RAM_START <= addr && addr < RAM_START + RAM_MAX)
		return (ram[addr-RAM_START] << 8) | ram[addr-RAM_START+1];
	else if(GPU_TILETAB_START <= addr && addr < GPU_TILETAB_START + GPU_TILETAB_MAX
	     || GPU_VRAM_START <= addr && addr < GPU_VRAM_START + GPU_VRAM_MAX
	     || GPU_SPRITE_START <= addr && addr < GPU_SPRITE_START + GPU_SPRITE_MAX
	     || GPU_CGRAM_START <= addr && addr < GPU_CGRAM_START + GPU_CGRAM_MAX
	     || GPU_PARAM_START <= addr && addr < GPU_PARAM_START + GPU_PARAM_MAX)
		return gpu_read(addr);
	else if(CON_START <= addr && addr < CON_START + CON_MAX)
		return con_read(addr);
	else
		return cart_read(addr);
}

unsigned m68k_read_memory_32(unsigned addr) {
	return (m68k_read_memory_16(addr) << 16) | m68k_read_memory_16(addr+2);
}

void m68k_write_memory_8(unsigned addr, unsigned val) {
	unsigned mem = m68k_read_memory_16(addr & ~1);
	mem &= addr % 2 ? 0xff00 : 0x00ff;
	mem |= addr % 2 ? val & 0xff : (val & 0xff) << 8;
	m68k_write_memory_16(addr & ~1, mem);
}

void m68k_write_memory_16(unsigned addr, unsigned val) {
	if(RAM_START <= addr && addr < RAM_START + RAM_MAX) {
		ram[addr - RAM_START] = val >> 8;
		ram[addr - RAM_START + 1] = val & 0xff;
	} else if(addr == LOW_ROM_SWITCH) {
		lowrom = val & 1;
	} else if(GPU_TILETAB_START <= addr && addr < GPU_TILETAB_START + GPU_TILETAB_MAX
	       || GPU_VRAM_START <= addr && addr < GPU_VRAM_START + GPU_VRAM_MAX
	       || GPU_SPRITE_START <= addr && addr < GPU_SPRITE_START + GPU_SPRITE_MAX
	       || GPU_CGRAM_START <= addr && addr < GPU_CGRAM_START + GPU_CGRAM_MAX
	       || GPU_PARAM_START <= addr && addr < GPU_PARAM_START + GPU_PARAM_MAX)
		gpu_write(addr, val);
}

void m68k_write_memory_32(unsigned addr, unsigned val) {
	m68k_write_memory_16(addr, val >> 16);
	m68k_write_memory_16(addr+2, val & 0xffff);
}

unsigned m68k_read_disassembler_16(unsigned addr) {
	return m68k_read_memory_16(addr);
}

unsigned m68k_read_disassembler_32(unsigned addr) {
	return m68k_read_memory_32(addr);
}

