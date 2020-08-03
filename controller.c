#include <stdio.h>
#include "controller.h"
#include "m68k.h"

static unsigned con[2];

void con_write(unsigned n, unsigned buttons) {
	con[n] = buttons;
}

void con_key(unsigned n, unsigned char key, int pressed) {
	con[n] = (pressed ? 0x100 : 0) | key&0xff;
}

unsigned con_read(unsigned addr) {
	int cyc = m68k_cycles_remaining();
	m68k_modify_timeslice(cyc < 16 ? 0 : cyc - 16);
	if(addr == CON_1_ADDR)
		return con[0];
	else if(addr == CON_2_ADDR)
		return con[1];
}

