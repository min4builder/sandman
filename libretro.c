#include <stdio.h>
#include <string.h>
#include "bus.h"
#include "cart.h"
#include "controller.h"
#include "gpu.h"
#include "m68k.h"
#include "libretro.h"

static enum { CON, KB } inputs[2];

static retro_environment_t environ_cb;
static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

void on_keyboard_event(bool down, unsigned keycode, uint32_t ch, uint16_t mods) {
	if(down && !ch || ch > 256) return;
	for(int i = 0; i < 2; i++) {
		if(inputs[i] == KB)
			con_key(i, ch & 0xff, down);
	}
}

unsigned retro_api_version(void) {
	return RETRO_API_VERSION;
}

void retro_set_environment(retro_environment_t cb) {
	environ_cb = cb;

	static struct retro_keyboard_callback kcb = { on_keyboard_event };
	environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &kcb);

	static struct retro_controller_description cons[] = {
		{ "Gamepad", RETRO_DEVICE_JOYPAD },
		{ "Keyboard", RETRO_DEVICE_KEYBOARD },
		{ "Disconnected", RETRO_DEVICE_NONE },
		{ 0 }
	};
	static struct retro_controller_info ports[] = {
		{ cons, 3 },
		{ cons, 3 },
		{ 0 }
	};
	environ_cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, ports);
}

void retro_set_video_refresh(retro_video_refresh_t cb) {
	video_cb = cb;
}

void retro_set_audio_sample(retro_audio_sample_t cb) {
	audio_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb) {
	input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb) {
	input_state_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) {}
void retro_set_controller_port_device(unsigned port, unsigned dev) {
	if(port < 2) {
		switch(dev) {
		case RETRO_DEVICE_JOYPAD:
			inputs[port] = CON;
			break;
		case RETRO_DEVICE_KEYBOARD:
			inputs[port] = KB;
			break;
		}
	}
}

size_t retro_serialize_size(void) { return 0; }
bool retro_serialize(void *data, size_t size) { return 0; }
bool retro_unserialize(void const *data, size_t size) { return 0; }

void retro_cheat_reset(void) {}
void retro_cheat_set(unsigned idx, bool en, char const *code) {}

void *retro_get_memory_data(unsigned id) { return 0; }
size_t retro_get_memory_size(unsigned id) { return 0; }

void retro_get_system_info(struct retro_system_info *info) {
	memset(info, 0, sizeof(*info));
	info->library_name = "Sandman";
	info->library_version = "0.0.0";
	info->need_fullpath = 0;
	info->valid_extensions = "sand|snd";
}

void retro_get_system_av_info(struct retro_system_av_info *info) {
	enum retro_pixel_format f = RETRO_PIXEL_FORMAT_RGB565;
	environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &f);

	memset(info, 0, sizeof(*info));
	info->timing.fps = 60./1.001;
	info->timing.sample_rate = 44100;
	info->geometry.base_width = 256;
	info->geometry.base_height = 224;
	info->geometry.max_width = 512;
	info->geometry.max_height = 512;
	info->geometry.aspect_ratio = 4. / 3.;
}

unsigned retro_get_region(void) {
	return RETRO_REGION_NTSC;
}

void cpu_instr_callback(void) {
	char buf[64];
	unsigned pc = m68k_get_reg(0, M68K_REG_PC);
	unsigned s = m68k_disassemble(buf, pc, M68K_CPU_TYPE_68000);
	printf("$%03x\t%s\n", pc, buf);
}

void retro_init(void) {
	m68k_init();
	m68k_set_cpu_type(M68K_CPU_TYPE_68000);
	//m68k_set_instr_hook_callback(cpu_instr_callback);
	rom_init();

	struct retro_input_descriptor d[] = {
		{ 0,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "Left" },
		{ 0,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "Up" },
		{ 0,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "Down" },
		{ 0,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
		{ 0,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "A" },
		{ 0,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "B" },
		{ 0,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "C" },
		{ 0,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "D" },
		{ 0,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "E" },
		{ 0,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "F" },
		{ 0,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "L" },
		{ 0,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "R" },

		{ 1,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "Left" },
		{ 1,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "Up" },
		{ 1,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "Down" },
		{ 1,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
		{ 1,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "A" },
		{ 1,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "B" },
		{ 1,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "C" },
		{ 1,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "D" },
		{ 1,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "E" },
		{ 1,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "F" },
		{ 1,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "L" },
		{ 1,  RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "R" },

		{ 0, 0, 0, 0, 0 }
	};
	environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, d);
}

void retro_deinit(void) {}

bool retro_load_game(struct retro_game_info const *info) {
	m68k_pulse_reset();
	if(info && info->data) {
		cart_load(info->data, info->size);
	}
	return 1;
}

bool retro_load_game_special(unsigned t, struct retro_game_info const *info, size_t num_info) {}

void retro_unload_game(void) {}

void retro_reset(void) {}

static void joystick_update_state(unsigned port) {
	if(inputs[port] == CON) {
		con_write(port, (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP) ? CON_UP : 0)
		              | (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN) ? CON_DOWN : 0)
		              | (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT) ? CON_LEFT : 0)
		              | (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) ? CON_RIGHT : 0)
		              | (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B) ? CON_A : 0)
		              | (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A) ? CON_B : 0)
		              | (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y) ? CON_C : 0)
		              | (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X) ? CON_D : 0)
		              | (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START) ? CON_E : 0)
		              | (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT) ? CON_F : 0)
		              | (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L) ? CON_L : 0)
		              | (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R) ? CON_R : 0));
	}
}

void retro_run(void) {
	static int const cpupline = 762;
	static int const cpuvblank = 14859;
	static int const cpuhblank = 131;
	static unsigned short framebuffer[512*487];

	input_poll_cb();
	joystick_update_state(0);
	joystick_update_state(1);

	m68k_execute(cpuvblank);
	gpu_start_field();
	for(int i = 0; i < gpu_vert(); i++) {
		m68k_execute(cpupline-cpuhblank);
		gpu_scanline(framebuffer + i * 512, i);
		m68k_execute(cpuhblank);
	}
	gpu_end_field();

	for(int i = 0; i < 44100/60; i++)
		audio_cb(1, 1);

	video_cb(framebuffer, gpu_horiz(), gpu_vert(), sizeof(unsigned short) * 512);
}

