# TODO
- [x] makefiles! make it recursive way (solved with `find`)
- [ ] grub! booting the multiboot way
- [ ] proper configuration system with wistles and bells
	+ [ ] #define DO_IT options (no value?)
	+ [ ] ncurses gui (*optional*)
- [ ] memory management (we still need it)
	+ [ ] memory map (E820h? nah, grub)
	+ [ ] physical memory management
	+ [ ] virtual memory management
	+ [ ] heap (need a proper algorithm, huh?)
- [ ] Intel-lpss UART support (PCI: vendor 8086h, device a166h)
- [ ] ATA PIO
- [ ] FAT32 (should I use someone's realization?)
- [ ] GPT
- [ ] heap
- [ ] ioremap-style MMIO interactions
- [ ] USB
	+ [ ] xHCI (?)
	+ [ ] mass storage support
- [ ] virtualization
	+ [ ] make it work on revision > 1 (?!)
	+ [ ] EPT
	+ [ ] seabios
	+ [ ] UEFI? (Tianocore y'know)
	+ [ ] this is where magic happens
- [ ] seriously? by this time I'll graduate from university