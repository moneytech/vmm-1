#include <kernel/pci.h>
#include <kernel/io.h>
#include <kernel/module.h>
#include <stdint.h>

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

MODULE("PCI");

static struct pci_device devs[32];
static int devs_i = 0;

static void pci_add_device(struct pci_device d)
{
	devs[devs_i++] = d;
}

static uint16_t pci_read_word(struct pci_device dev, uint16_t offset)
{
	uint64_t lbus = (uint64_t)dev.bus;
	uint64_t ldev = (uint64_t)dev.dev;
	uint64_t lfnc = (uint64_t)dev.fnc;
	uint16_t tmp = 0;
	uint64_t address = 0x80000000;
	address |= offset & 0xfc;
	address |= lfnc << 8;
	address |= ldev << 11;
	address |= lbus << 16;
	outl (CONFIG_ADDRESS, address);
	tmp = (uint16_t)((inl(CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
	return tmp;
}

static void pci_write_word(struct pci_device dev, uint16_t offset, uint16_t value)
{
	uint64_t lbus = (uint64_t)dev.bus;
	uint64_t ldev = (uint64_t)dev.dev;
	uint64_t lfnc = (uint64_t)dev.fnc;
	uint64_t address = 0x80000000;
	uint32_t tmp = 0;
	address |= offset & 0xfc;
	address |= lfnc << 8;
	address |= ldev << 11;
	address |= lbus << 16;
	outl(CONFIG_ADDRESS, address);
	tmp = inl(CONFIG_DATA);
	tmp &= ~(0xffff << ((offset & 2) * 8));
	tmp |= value << ((offset & 2) * 8);
	outl(CONFIG_DATA, tmp);
	return (tmp);
}


uint16_t pci_get_vendor(struct pci_device dev)
{
	uint16_t r0 = pci_read_word(dev, 0);
	return r0;
}

uint16_t pci_get_device(struct pci_device dev)
{
	uint16_t r0 = pci_read_word(dev, 2);
	return r0;
}

uint16_t pci_get_class(struct pci_device dev)
{
	uint16_t r0 = pci_read_word(dev, 0xA);
	return (r0 & 0xFF00) >> 8;
}

uint16_t pci_get_subclass(struct pci_device dev)
{
	uint16_t r0 = pci_read_word(dev, 0xA);
	return (r0 & 0x00FF);
}

uint32_t pci_get_command(struct pci_device dev)
{
	uint16_t r0 = pci_read_word(dev, 0x4);
	return r0;
}

void pci_set_command(struct pci_device dev, uint16_t value)
{
	pci_write_word(dev, 0x4, value);
	return r0;
}

void pci_probe(void)
{
	for(uint32_t bus = 0; bus < 256; bus++)
		for(uint32_t dev = 0; dev < 32; dev++)
			for(uint32_t fnc = 0; fnc < 8; fnc++)
			{
				struct pci_device d = {.bus = bus, .dev =  dev, .fnc = fnc};
				uint16_t vendor = pci_get_vendor(d);
				if(vendor == 0xffff) continue;
			}
}

void pci_list()
{
	for(int i = 0; i<devs_i; i++)
	{
		pci_device d = devs[i];
		mprint("%x:%x.%x", d.bus, d.dev, d.fnc);
	}
}