.PHONY: all clean x86-asm x86-obj x86-exe arm-asm arm-obj arm-exe

all: x86-asm x86-obj x86-exe arm-asm arm-obj arm-exe

x86-asm:
	for i in *.c; do gcc -O0 -S -fno-stack-protector -o$${i%%.c}.x86.s $$i; done

arm-asm:
	for i in *.c; do armv7l-linux-gnueabihf-gcc -O0 -S -fno-stack-protector -o$${i%%.c}.arm.s $$i; done

x86-obj:
	for i in *.x86.s; do as -o$${i%%.s}.o $$i; done

arm-obj:
	for i in *.arm.s; do armv7l-linux-gnueabihf-as -o$${i%%.s}.o $$i; done

x86-exe:
	for i in *.x86.s; do gcc -o$${i%%.s}.exec $$i; done

arm-exe:
	for i in *.arm.s; do armv7l-linux-gnueabihf-gcc -o$${i%%.s}.exec $$i; done

arm-build-run: arm-exe
	qemu-arm -L /usr/armv7l-linux-gnueabihf -E LD_LIBRARY_PATH=/usr/armv7l-linux-gnueabihf/lib fib.arm.exec

clean:
	rm -f *.exec *.o