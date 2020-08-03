CFLAGS = -g -Wall -Wextra -std=c99 -fPIC
LFLAGS = -g
M68K = m68kcpu.o m68kdasm.o m68kops.o m68kopac.o m68kopdm.o m68kopnz.o
OBJ = libretro.o bus.o cart.o controller.o gpu.o irq.o $(M68K)
GENC = m68kops.c m68kopac.c m68kopdm.c m68kopnz.c
GENH = m68kops.h

all: sandman-libretro.so ior.rom

sandman-libretro.so: $(OBJ) $(GENH)
	$(CC) $(LFLAGS) -shared -o $@ $(OBJ) $(LIBS)

ior.rom: ior.s
	@#asm68k -ew -b -l -o $@ $<
	vasmm68k_mot -quiet -spaces -Fbin -o $@ $<

devcart.rom: devcart.s font.s def.i
	vasmm68k_mot -quiet -spaces -Fbin -o $@ devcart.s

$(GENC) $(GENH): m68kmake m68k_in.c
	./m68kmake

m68kmake: m68kmake.c
	$(CC) $(LFLAGS) $(CFLAGS) -o $@ $<

.c.o: $*.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o *.lst $(GENC) $(GENH) sandman-libretro.so ior.rom

m68kcpu.o m68kdasm.o: $(GENH)

