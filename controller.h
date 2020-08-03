#define CON_START 0x00e00000
#define CON_MAX 10
#define CON_1_ADDR 0x00e00000
#define CON_2_ADDR 0x00e00002
#define CON_3_ADDR 0x00e00004
#define CON_4_ADDR 0x00e00006
#define CON_TYPE_ADDR 0x00e00008

enum {
	CON_RIGHT = 0x1,
	CON_LEFT = 0x2,
	CON_DOWN = 0x4,
	CON_UP = 0x8,
	CON_A = 0x10,
	CON_B = 0x20,
	CON_C = 0x40,
	CON_D = 0x80,
	CON_E = 0x100,
	CON_F = 0x200,
	CON_R = 0x400,
	CON_L = 0x800
};

void con_write(unsigned, unsigned);
void con_key(unsigned, unsigned char, int);
unsigned con_read(unsigned);

