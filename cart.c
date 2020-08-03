#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *cart;
static unsigned map = 0x00100000;
static size_t size;

void cart_load(void const *data, size_t size_) {
	unsigned char const *d = data;
	cart = malloc(size_);
	memcpy(cart, d, size_);
	size = size_;
}

unsigned cart_read(unsigned addr) {
	addr &= ~1;
	addr -= map;
	if(addr < size) {
		return cart[addr] << 8
		     | cart[addr+1];
	}
	return 0x0C48;
}

