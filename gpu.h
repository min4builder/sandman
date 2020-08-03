#define GPU_TILETAB_START 0x00f00000
#define GPU_TILETAB_MAX 0x8000
#define GPU_VRAM_START 0x00f08000
#define GPU_VRAM_MAX 0x8000
#define GPU_SPRITE_START 0x00f10000
#define GPU_SPRITE_MAX 0x1000
#define GPU_CGRAM_START 0x00f18000
#define GPU_CGRAM_MAX 0x400
#define GPU_PARAM_START 0x00f20000
#define GPU_PARAM_MAX 10

void gpu_scanline(unsigned short *, int);
void gpu_start_field(void);
void gpu_end_field(void);
unsigned gpu_read(unsigned);
void gpu_write(unsigned, unsigned);

