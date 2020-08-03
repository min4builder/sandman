#include "m68k.h"
#include "irq.h"

static int assert[7] = {0};

void irq_set(unsigned lvl, int on) {
	assert[lvl-1] = on;
	for(int i = 7; i > 0; i--) {
		if(assert[i-1]) {
			m68k_set_irq(i);
			return;
		}
	}
	m68k_set_irq(0);
}

