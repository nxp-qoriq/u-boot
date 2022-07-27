// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2022 NXP
 */

#include <common.h>
#include <command.h>
#include <spi_flash.h>
#include <linux/ctype.h>
 #include <u-boot/crc.h>

/* for MAX_NUM_PORTS in board-specific file */
#ifndef MAX_NUM_PORTS
#define MAX_NUM_PORTS	16
#endif
#define NXID_VERSION	1

/**
 * static id_spi_flash: SPIFLASH layout for NXID formats
 *
 * See application note AN3638 for details.
 */
static struct __attribute__ ((__packed__)) id_spi_flash {
	u8 id[4];         /* 0x00 - 0x03 SPIFLASH Tag 'NXID' */
	u8 sn[16];        /* 0x04 - 0x0F IMEISV Number */
	u8 errata[5];     /* 0x10 - 0x14 Errata Level */
	u8 date[6];       /* 0x15 - 0x1a Build Date */
	u8 res_0;         /* 0x1b        Reserved */
	u32 version;      /* 0x1c - 0x1f NXID Version */
	u8 tempcal[8];    /* 0x20 - 0x27 Temperature Calibration Factors */
	u8 tempcalsys[2]; /* 0x28 - 0x29 System Temperature Calibration Factors */
	u8 tempcalflags;  /* 0x2a        Temperature Calibration Flags */
	u8 res_1[21];     /* 0x2b - 0x3f Reserved */
	u8 mac_count;     /* 0x40        Number of MAC addresses */
	u8 mac_flag;      /* 0x41        MAC table flags */
	u8 mac[MAX_NUM_PORTS][6];     /* 0x42 - 0xa1 MAC addresses */
	u8 res_2[90];     /* 0xa2 - 0xfb Reserved */
	u32 crc;          /* 0xfc - 0xff CRC32 checksum */
} f;

/* Set to 1 if we've read SPIFLASH into memory */
static int has_been_read = 0;

/* Is this a valid NXID SPIFLASH? */
#define is_valid ((f.id[0] == 'N') || (f.id[1] == 'X') || \
		  (f.id[2] == 'I') || (f.id[3] == 'D'))


/**
 * show_spi_flash - display the contents of the SPIFLASH
 */
static void show_spi_flash(void)
{
	int i;
	unsigned int crc;

	/* SPIFLASH tag ID, either CCID or NXID */
	printf("ID: %c%c%c%c v%u\n", f.id[0], f.id[1], f.id[2], f.id[3],
	       be32_to_cpu(f.version));

	/* Serial number */
	printf("SN: %s\n", f.sn);

	/* Errata level. */
	printf("Errata: %s\n", f.errata);

	/* Build date, BCD date values, as YYMMDDhhmmss */
	printf("Build date: 20%02x/%02x/%02x %02x:%02x:%02x %s\n",
		f.date[0], f.date[1], f.date[2],
		f.date[3] & 0x7F, f.date[4], f.date[5],
		f.date[3] & 0x80 ? "PM" : "");

	/* Show MAC addresses  */
	for (i = 0; i < min(f.mac_count, (u8)MAX_NUM_PORTS); i++) {

		u8 *p = f.mac[i];

		printf("Eth%u: %02x:%02x:%02x:%02x:%02x:%02x\n", i,
			p[0], p[1], p[2], p[3],	p[4], p[5]);
	}

	crc = crc32(0, (void *)&f, sizeof(f) - 4);

	if (crc == be32_to_cpu(f.crc))
		printf("CRC: %08x\n", be32_to_cpu(f.crc));
	else
		printf("CRC: %08x (should be %08x)\n",
			be32_to_cpu(f.crc), crc);

#ifdef DEBUG
	printf("SPIFLASH dump: (0x%x bytes)\n", sizeof(f));
	for (i = 0; i < sizeof(f); i++) {
		if ((i % 16) == 0)
			printf("%02X: ", i);
		printf("%02X ", ((u8 *)&f)[i]);
		if (((i % 16) == 15) || (i == sizeof(f) - 1))
			printf("\n");
	}
#endif
}

/**
 * read_spi_flash - read the SPIFLASH into memory
 */
static int read_spi_flash(void)
{
	int ret;
	struct spi_flash *id;
	struct udevice *new;

	if (has_been_read)
		return 0;

	ret = spi_flash_probe_bus_cs(CONFIG_ENV_SPI_BUS,
			CONFIG_ENV_SPI_CS,
			CONFIG_ENV_SPI_MAX_HZ,
			CONFIG_ENV_SPI_MODE,
			&new);

	if (ret) {
		printf("SF: probe failed\n");
		device_remove(new, DM_REMOVE_NORMAL);
		return ret;
	}

	id = dev_get_uclass_priv(new);
	if (!id) {
		printf("SF: probe failed\n");
		device_remove(new, DM_REMOVE_NORMAL);
		return -ENODEV;
	}


	ret = spi_flash_read(id, CONFIG_SYS_ID_OFFSET, sizeof(f), (void *)&f);

	if (ret) {
		printf("SF: read failed\n");
		device_remove(new, DM_REMOVE_NORMAL);
		return ret;
	}
#ifdef DEBUG
	show_spi_flash();
#endif

	has_been_read = (ret == 0) ? 1 : 0;

	spi_flash_free(id);
	device_remove(new, DM_REMOVE_NORMAL);
	return ret;
}

/**
 *  update_crc - update the CRC
 *
 *  This function should be called after each update to the SPIFLASH structure,
 *  to make sure the CRC is always correct.
 */
static void update_crc(void)
{
	u32 crc;

	crc = crc32(0, (void *)&f, sizeof(f) - 4);
	f.crc = cpu_to_be32(crc);
}

/**
 * prog_spi_flash - write the SPIFLASH from memory
 */
static int prog_spi_flash(void)
{
	int ret = 0;
	struct spi_flash *id;
	struct udevice *new;
	u32 sector;

	ret = spi_flash_probe_bus_cs(CONFIG_ENV_SPI_BUS,
			CONFIG_ENV_SPI_CS,
			CONFIG_ENV_SPI_MAX_HZ,
			CONFIG_ENV_SPI_MODE,
			&new);

	if (ret) {
		printf("SF: probe failed\n");
		device_remove(new, DM_REMOVE_NORMAL);
		return ret;
	}

	id = dev_get_uclass_priv(new);
	if (!id) {
		printf("SF: probe failed\n");
		device_remove(new, DM_REMOVE_NORMAL);
		return -ENODEV;
	}
	/* Set the reserved values to 0xFF   */
	f.res_0 = 0xFF;
	memset(f.res_1, 0xFF, sizeof(f.res_1));
	update_crc();

	sector = DIV_ROUND_UP(sizeof(f), CONFIG_ENV_SECT_SIZE);

	puts("Erasing SPI flash...");
	ret = spi_flash_erase(id, CONFIG_SYS_ID_OFFSET, sector * CONFIG_ENV_SECT_SIZE);
	if (ret) {
		printf("SF: erase failed\n");
		device_remove(new, DM_REMOVE_NORMAL);
		return ret;
	}
	puts("Done.\n");

	puts("Writing to SPI flash...");
	ret = spi_flash_write(id, CONFIG_SYS_ID_OFFSET, sizeof(f), &f);
	if (ret) {
		printf("SF: write failed\n");
		device_remove(new, DM_REMOVE_NORMAL);
		return ret;
	}
	puts("Done.\n");

	if (!ret) {
		/* Verify the write by reading back the SPIFLASH and comparing */
		struct id_spi_flash f2;

		ret = spi_flash_read(id, CONFIG_SYS_ID_OFFSET, sizeof(f2), (void *)&f2);
		if (!ret && memcmp(&f, &f2, sizeof(f)))
			ret = -1;
	}

	if (ret) {
		printf("Programming failed.\n");
		has_been_read = 0;
		return -1;
	}

	printf("Programming passed.\n");
	spi_flash_free(id);
	device_remove(new, DM_REMOVE_NORMAL);
	return 0;
}

/**
 * h2i - converts hex character into a number
 *
 * This function takes a hexadecimal character (f.g. '7' or 'C') and returns
 * the integer equivalent.
 */
static inline u8 h2i(char p)
{
	if ((p >= '0') && (p <= '9'))
		return p - '0';

	if ((p >= 'A') && (p <= 'F'))
		return (p - 'A') + 10;

	if ((p >= 'a') && (p <= 'f'))
		return (p - 'a') + 10;

	return 0;
}

/**
 * set_date - stores the build date into the SPIFLASH
 *
 * This function takes a pointer to a string in the format "YYMMDDhhmmss"
 * (2-digit year, 2-digit month, etc), converts it to a 6-byte BCD string,
 * and stores it in the build date field of the SPIFLASH local copy.
 */
static void set_date(const char *string)
{
	unsigned int i;

	if (strlen(string) != 12) {
		printf("Usage: mac date YYMMDDhhmmss\n");
		return;
	}

	for (i = 0; i < 6; i++)
		f.date[i] = h2i(string[2 * i]) << 4 | h2i(string[2 * i + 1]);

	update_crc();
}

/**
 * set_mac_address - stores a MAC address into the SPIFLASH
 *
 * This function takes a pointer to MAC address string
 * (i.f."XX:XX:XX:XX:XX:XX", where "XX" is a two-digit hex number) and
 * stores it in one of the MAC address fields of the SPIFLASH local copy.
 */
static void set_mac_address(unsigned int index, const char *string)
{
	char *p = (char *) string;
	unsigned int i;

	if ((index >= MAX_NUM_PORTS) || !string) {
		printf("Usage: mac <n> XX:XX:XX:XX:XX:XX\n");
		return;
	}

	for (i = 0; *p && (i < 6); i++) {
		f.mac[index][i] = simple_strtoul(p, &p, 16);
		if (*p == ':')
			p++;
	}

	update_crc();
}

int do_mac(struct cmd_tbl *cmdtp, int flag, int argc, char * const argv[])
{
	char cmd;

	if (argc == 1) {
		show_spi_flash();
		return 0;
	}

	cmd = argv[1][0];

	if (cmd == 'r') {
		read_spi_flash();
		return 0;
	}

	if (cmd == 'i') {
		memcpy(f.id, "NXID", sizeof(f.id));
		f.version = cpu_to_be32(NXID_VERSION);
		update_crc();
		return 0;
	}

	if (!is_valid) {
		printf("Please read the SPIFLASH ('r') and/or set the ID ('i') first.\n");
		return 0;
	}

	if (argc == 2) {
		switch (cmd) {
		case 's':	/* save */
			prog_spi_flash();
			break;
		default:
			return cmd_usage(cmdtp);
		}

		return 0;
	}

	/* We know we have at least one parameter  */

	switch (cmd) {
	case 'n':	/* serial number */
		memset(f.sn, 0, sizeof(f.sn));
		strncpy((char *)f.sn, argv[2], sizeof(f.sn) - 1);
		update_crc();
		break;
	case 'e':	/* errata */
		memset(f.errata, 0, 5);
		strncpy((char *)f.errata, argv[2], 4);
		update_crc();
		break;
	case 'd':	/* date BCD format YYMMDDhhmmss */
		set_date(argv[2]);
		break;
	case 'p':	/* MAC table size */
		f.mac_count = simple_strtoul(argv[2], NULL, 16);
		update_crc();
		break;
	case '0' ... '9':	/* "mac 0" through "mac 22" */
		set_mac_address(simple_strtoul(argv[1], NULL, 10), argv[2]);
		break;
	case 'h':	/* help */
	default:
		return cmd_usage(cmdtp);
	}

	return 0;
}

/**
 * mac_read_from_spi_flash - read the MAC addresses from SPIFLASH
 *
 * This function reads the MAC addresses from SPIFLASH and sets the
 * appropriate environment variables for each one read.
 *
 * The environment variables are only set if they haven't been set already.
 * This ensures that any user-saved variables are never overwritten.
 *
 * This function must be called after relocation.
 *
 * For NXID v1 SPIFLASHs, we support loading and up-converting the older NXID v0
 * format.  In a v0 SPIFLASH, there are only eight MAC addresses and the CRC is
 * located at a different offset.
 */
int mac_read_from_spi_flash(void)
{
	unsigned int i;
	u32 crc, crc_offset = offsetof(struct id_spi_flash, crc);
	u32 *crcp; /* Pointer to the CRC in the data read from the SPIFLASH */

	puts("MAC: ");

	if (read_spi_flash()) {
		printf("Read failed.\n");
		return 0;
	}

	if (!is_valid) {
		printf("Invalid ID (%02x %02x %02x %02x)\n",
		       f.id[0], f.id[1], f.id[2], f.id[3]);
		return 0;
	}

	/*
	 * If we've read an NXID v0 SPIFLASH, then we need to set the CRC offset
	 * to where it is in v0.
	 */
	if (f.version == 0)
		crc_offset = 0x72;

	crc = crc32(0, (void *)&f, crc_offset);
	crcp = (void *)&f + crc_offset;
	if (crc != be32_to_cpu(*crcp)) {
		printf("CRC mismatch (%08x != %08x)\n", crc, be32_to_cpu(f.crc));
		return 0;
	}

	/*
	 * MAC address #9 in v1 occupies the same position as the CRC in v0.
	 * Erase it so that it's not mistaken for a MAC address.  We'll
	 * update the CRC later.
	 */
	if (f.version == 0)
		memset(f.mac[8], 0xff, 6);

	for (i = 0; i < min(f.mac_count, (u8)MAX_NUM_PORTS); i++) {
		if (memcmp(&f.mac[i], "\0\0\0\0\0\0", 6) &&
		    memcmp(&f.mac[i], "\xFF\xFF\xFF\xFF\xFF\xFF", 6)) {
			char ethaddr[18];
			char enetvar[9];

			sprintf(ethaddr, "%02X:%02X:%02X:%02X:%02X:%02X",
				f.mac[i][0],
				f.mac[i][1],
				f.mac[i][2],
				f.mac[i][3],
				f.mac[i][4],
				f.mac[i][5]);
			sprintf(enetvar, i ? "eth%daddr" : "ethaddr", i);
			/* Only initialize environment variables that are blank
			 * (i.f. have not yet been set)
			 */
			if (!env_get(enetvar))
				env_set(enetvar, ethaddr);
		}
	}

	printf("%c%c%c%c v%u\n", f.id[0], f.id[1], f.id[2], f.id[3],
	       be32_to_cpu(f.version));

	/*
	 * Now we need to upconvert the data into v1 format.  We do this last so
	 * that at boot time, U-Boot will still say "NXID v0".
	 */
	if (f.version == 0) {
		f.version = cpu_to_be32(NXID_VERSION);
		update_crc();
	}

	return 0;
}
