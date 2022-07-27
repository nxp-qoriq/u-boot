// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2022 NXP
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Pull in the current config to define the default environment */
#include <linux/kconfig.h>

#ifndef __ASSEMBLY__
#define __ASSEMBLY__ /* get only #defines from config.h */
#include <config.h>
#undef  __ASSEMBLY__
#else
#include <config.h>
#endif

#include <environment.h>
#include <u-boot/crc.h>

int main(int argc, char* argv[])
{
	env_t env;
	FILE *finput, *foutput;
	char *line = NULL;
	size_t len;
	int i, j;

	if (argc < 2) {
		printf("USAGE: ./%s <input_filename> <output_filename>\n", argv[0]);
		printf("e.g.   ./%s uboot_env_sec.txt uboot_env_sec.crc32.txt\n", argv[0]);
		return -1;
	}

	memset(&env, 0, sizeof(env));

	finput = fopen(argv[1],"r");
	if (finput == NULL) {
		printf("Unable to open input file %s\n", argv[1]);
		return -1;
	}

	foutput = fopen(argv[2],"w");
	if (foutput == NULL) {
		fclose(finput);
		printf("Unable to open output file %s\n", argv[2]);
		return -1;
	}

	j = 0;
	while (getline(&line, &len, finput) != -1) {
		line[strcspn(line, "\n")] = '\0';
		for (i = 0; i < strlen(line) + 1; i++) {
#ifdef DEBUG
			if(line[i] == '\0')
				printf("|");
			else
				printf("%c", line[i]);
#endif
			env.data[j] = line[i];
			j++;
		}
	}
	env.crc = crc32(0, env.data, ENV_SIZE);
	printf("env.crc %x\n", env.crc);

	fwrite(&env, 1, sizeof(env), foutput);
	fclose(finput);
	fclose(foutput);
	return 0;
}
