qemu-system-riscv64 -machine virt \
					-nographic \
					-device loader,file=silly_little_ucore.img,addr=0x80200000 \
					-bios default
