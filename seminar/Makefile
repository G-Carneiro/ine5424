baremetal-clint: baremetal-clint.o link.lds
	~/riscv/bin/riscv64-unknown-linux-gnu-ld -T link.lds -o baremetal-clint baremetal-clint.o

baremetal-clint.o: baremetal-clint.s
	~/riscv/bin/riscv64-unknown-linux-gnu-as -o baremetal-clint.o baremetal-clint.s

baremetal-clint.s: baremetal-clint.cc
	~/riscv/bin/riscv64-unknown-linux-gnu-c++ -S baremetal-clint.cc  

clean:
	rm baremetal-clint baremetal-clint.o