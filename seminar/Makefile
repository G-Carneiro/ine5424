baremetal-clint: baremetal-clint.o link.lds
	riscv64-unknown-elf-ld -T link.lds -o baremetal-clint baremetal-clint.o

baremetal-clint.o: baremetal-clint.s
	riscv64-unknown-elf-as -o baremetal-clint.o baremetal-clint.s

clean:
	rm baremetal-clint baremetal-clint.o