/*
 * Copyright 2018 NXP
 * SPDX-License-Identifier:	GPL-2.0+        BSD-3-Clause
 *
 * Author York Sun <york.sun@nxp.com>
 */

#ifndef _MESSAGE_H_
#ifdef DEBUG
const static struct {
	uint32_t index;
	const char * msg;
} messages[] = {
	{0x00000001,
	 "PMU1:prbsGenCtl:%x\n"
	},
	{0x00010000,
	 "PMU1: loading 2D acsm sequence\n"
	},
	{0x00020000,
	 "PMU1: loading 1D acsm sequence\n"
	},
	{0x00030002,
	 "PMU3: %d memclocks @ %d to get half of 300ns\n"
	},
	{0x00040000,
	 "PMU: Error: User requested MPR read pattern for read DQS training in DDR3 Mode\n"
	},
	{0x00050000,
	 "PMU3: Running 1D search for left eye edge\n"
	},
	{0x00060001,
	 "PMU1: In Phase Left Edge Search cs %d\n"
	},
	{0x00070001,
	 "PMU1: Out of Phase Left Edge Search cs %d\n"
	},
	{0x00080000,
	 "PMU3: Running 1D search for right eye edge\n"
	},
	{0x00090001,
	 "PMU1: In Phase Right Edge Search cs %d\n"
	},
	{0x000a0001,
	 "PMU1: Out of Phase Right Edge Search cs %d\n"
	},
	{0x000b0001,
	 "PMU1: mxRdLat training pstate %d\n"
	},
	{0x000c0001,
	 "PMU1: mxRdLat search for cs %d\n"
	},
	{0x000d0001,
	 "PMU0: MaxRdLat non consistant DtsmLoThldXingInd 0x%03x\n"
	},
	{0x000e0003,
	 "PMU4: CS %d Dbyte %d worked with DFIMRL = %d DFICLKs \n"
	},
	{0x000f0004,
	 "PMU3: MaxRdLat Read Lane err mask for csn %d, DFIMRL %2d DFIClks, dbyte %d = 0x%03x\n"
	},
	{0x00100003,
	 "PMU3: MaxRdLat Read Lane err mask for csn %d DFIMRL %2d, All dbytes = 0x%03x\n"
	},
	{0x00110001,
	 "PMU: Error: CS%d failed to find a DFIMRL setting that worked for all bytes during MaxRdLat training\n"
	},
	{0x00120002,
	 "PMU3: Smallest passing DFIMRL for all dbytes in CS%d = %d DFIClks\n"
	},
	{0x00130000,
	 "PMU: Error: No passing DFIMRL value found for any chip select during MaxRdLat training\n"
	},
	{0x00140003,
	 "PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n"
	},
	{0x00150000,
	 "PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n"
	},
	{0x00160005,
	 "PMU4: DB %d Lane %d: %3d %3d -> %3d\n"
	},
	{0x00170002,
	 "PMU2: TXDQ delayLeft[%2d] = %3d (DISCONNECTED)\n"
	},
	{0x00180004,
	 "PMU2: TXDQ delayLeft[%2d] = %3d oopScaled = %3d selectOop %d\n"
	},
	{0x00190002,
	 "PMU2: TXDQ delayRight[%2d] = %3d (DISCONNECTED)\n"
	},
	{0x001a0004,
	 "PMU2: TXDQ delayRight[%2d] = %3d oopScaled = %3d selectOop %d\n"
	},
	{0x001b0003,
	 "PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n"
	},
	{0x001c0000,
	 "PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n"
	},
	{0x001d0002,
	 "PMU4: DB %d Lane %d: (DISCONNECTED)\n"
	},
	{0x001e0005,
	 "PMU4: DB %d Lane %d: %3d %3d -> %3d\n"
	},
	{0x001f0002,
	 "PMU3: Running 1D search csn %d for DM Right/NotLeft(%d) eye edge\n"
	},
	{0x00200002,
	 "PMU3: WrDq DM byte%2d with Errcnt %d\n"
	},
	{0x00210002,
	 "PMU3: WrDq DM byte%2d avgDly 0x%04x\n"
	},
	{0x00220002,
	 "PMU1: WrDq DM byte%2d with Errcnt %d\n"
	},
	{0x00230001,
	 "PMU: Error: Dbyte %d txDqDly DM training did not start inside the eye\n"
	},
	{0x00240000,
	 "PMU4: DM TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n"
	},
	{0x00250002,
	 "PMU4: DB %d Lane %d: (DISCONNECTED)\n"
	},
	{0x00260005,
	 "PMU4: DB %d Lane %d: %3d %3d -> %3d\n"
	},
	{0x00270003,
	 "PMU: Error: Dbyte %d lane %d txDqDly DM passing region is too small (width = %d)\n"
	},
	{0x00280004,
	 "PMU3: Errcnt for MRD/MWD search nib %2d delay = (%d, 0x%02x) = %d\n"
	},
	{0x00290000,
	 "PMU3: Precharge all open banks\n"
	},
	{0x002a0002,
	 "PMU: Error: Dbyte %d nibble %d found mutliple working coarse delay setting for MRD/MWD\n"
	},
	{0x002b0000,
	 "PMU4: MRD Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n"
	},
	{0x002c0000,
	 "PMU4: MRW Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n"
	},
	{0x002d0004,
	 "PMU10: Warning: DB %d nibble %d has multiple working coarse delays, %d and %d, choosing the smaller delay\n"
	},
	{0x002e0003,
	 "PMU: Error: Dbyte %d nibble %d MRD/MWD passing region is too small (width = %d)\n"
	},
	{0x002f0006,
	 "PMU4: DB %d nibble %d: %3d, %3d %3d -> %3d\n"
	},
	{0x00300002,
	 "PMU1: Start MRD/nMWD %d for csn %d\n"
	},
	{0x00310002,
	 "PMU2: RXDQS delayLeft[%2d] = %3d (DISCONNECTED)\n"
	},
	{0x00320006,
	 "PMU2: RXDQS delayLeft[%2d] = %3d delayOop[%2d] = %3d OopScaled %4d, selectOop %d\n"
	},
	{0x00330002,
	 "PMU2: RXDQS delayRight[%2d] = %3d (DISCONNECTED)\n"
	},
	{0x00340006,
	 "PMU2: RXDQS delayRight[%2d] = %3d delayOop[%2d] = %4d OopScaled %4d, selectOop %d\n"
	},
	{0x00350000,
	 "PMU4: RxClkDly Passing Regions (EyeLeft EyeRight -> EyeCenter)\n"
	},
	{0x00360002,
	 "PMU4: DB %d nibble %d: (DISCONNECTED)\n"
	},
	{0x00370005,
	 "PMU4: DB %d nibble %d: %3d %3d -> %3d\n"
	},
	{0x00380003,
	 "PMU: Error: Dbyte %d nibble %d rxClkDly passing region is too small (width = %d)\n"
	},
	{0x00390002,
	 "PMU0: goodbar = %d for RDWR_BLEN %d\n"
	},
	{0x003a0001,
	 "PMU3: RxClkDly = %d\n"
	},
	{0x003b0005,
	 "PMU0: db %d l %d absLane %d -> bottom %d top %d\n"
	},
	{0x003c0009,
	 "PMU3: BYTE %d - %3d %3d %3d %3d %3d %3d %3d %3d\n"
	},
	{0x003d0002,
	 "PMU: Error: dbyte %d lane %d's per-lane vrefDAC's had no passing region\n"
	},
	{0x003e0004,
	 "PMU0: db%d l%d - %d %d\n"
	},
	{0x003f0002,
	 "PMU0: goodbar = %d for RDWR_BLEN %d\n"
	},
	{0x00400004,
	 "PMU3: db%d l%d saw %d issues at rxClkDly %d\n"
	},
	{0x00410003,
	 "PMU3: db%d l%d first saw a pass->fail edge at rxClkDly %d\n"
	},
	{0x00420002,
	 "PMU3: lane %d PBD = %d\n"
	},
	{0x00430003,
	 "PMU3: db%d l%d first saw a DBI pass->fail edge at rxClkDly %d\n"
	},
	{0x00440003,
	 "PMU2: db%d l%d already passed rxPBD = %d\n"
	},
	{0x00450003,
	 "PMU0: db%d l%d, PBD = %d\n"
	},
	{0x00460002,
	 "PMU: Error: dbyte %d lane %d failed read deskew\n"
	},
	{0x00470003,
	 "PMU1: Running lane deskew on pstate %d csn %d rdDBIEn %d\n"
	},
	{0x00480000,
	 "PMU: Error: Read deskew training has been requested, but csrMajorModeDbyte[2] is set\n"
	},
	{0x00490002,
	 "PMU1: AcsmCsMapCtrl%02d 0x%04x\n"
	},
	{0x004a0002,
	 "PMU1: AcsmCsMapCtrl%02d 0x%04x\n"
	},
	{0x004b0001,
	 "PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3U Type\n"
	},
	{0x004c0001,
	 "PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3R Type\n"
	},
	{0x004d0001,
	 "PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4U Type\n"
	},
	{0x004e0001,
	 "PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4R Type\n"
	},
	{0x004f0001,
	 "PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4LR Type\n"
	},
	{0x00500000,
	 "PMU: Error: Both 2t timing mode and ddr4 geardown mode specifed in the messageblock's PhyCfg and MR3 fields. Only one can be enabled\n"
	},
	{0x00510003,
	 "PMU10: PHY TOTALS - NUM_DBYTES %d NUM_NIBBLES %d NUM_ANIBS %d\n"
	},
	{0x00520006,
	 "PMU10: CSA=0x%02X, CSB=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR3\n"
	},
	{0x00530006,
	 "PMU10: CSA=0x%02X, CSB=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR4\n"
	},
	{0x00540008,
	 "PMU10: CS=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, 2T=%d, MMISC=%d AddrMirror=%d DRAMFreq=%dMT DramType=%d\n"
	},
	{0x00550004,
	 "PMU10: Pstate%d MR0=0x%04X MR1=0x%04X MR2=0x%04X\n"
	},
	{0x00560008,
	 "PMU10: Pstate%d MRS MR0=0x%04X MR1=0x%04X MR2=0x%04X MR3=0x%04X MR4=0x%04X MR5=0x%04X MR6=0x%04X\n"
	},
	{0x00570005,
	 "PMU10: Pstate%d MRS MR1_A0=0x%04X MR2_A0=0x%04X MR3_A0=0x%04X MR11_A0=0x%04X\n"
	},
	{0x00580000,
	 "PMU10: UseBroadcastMR set. All ranks and channels use MRXX_A0 for MR settings.\n"
	},
	{0x00590005,
	 "PMU10: Pstate%d MRS MR01_A0=0x%02X MR02_A0=0x%02X MR03_A0=0x%02X MR11_A0=0x%02X\n"
	},
	{0x005a0005,
	 "PMU10: Pstate%d MRS MR12_A0=0x%02X MR13_A0=0x%02X MR14_A0=0x%02X MR22_A0=0x%02X\n"
	},
	{0x005b0005,
	 "PMU10: Pstate%d MRS MR01_A1=0x%02X MR02_A1=0x%02X MR03_A1=0x%02X MR11_A1=0x%02X\n"
	},
	{0x005c0005,
	 "PMU10: Pstate%d MRS MR12_A1=0x%02X MR13_A1=0x%02X MR14_A1=0x%02X MR22_A1=0x%02X\n"
	},
	{0x005d0005,
	 "PMU10: Pstate%d MRS MR01_B0=0x%02X MR02_B0=0x%02X MR03_B0=0x%02X MR11_B0=0x%02X\n"
	},
	{0x005e0005,
	 "PMU10: Pstate%d MRS MR12_B0=0x%02X MR13_B0=0x%02X MR14_B0=0x%02X MR22_B0=0x%02X\n"
	},
	{0x005f0005,
	 "PMU10: Pstate%d MRS MR01_B1=0x%02X MR02_B1=0x%02X MR03_B1=0x%02X MR11_B1=0x%02X\n"
	},
	{0x00600005,
	 "PMU10: Pstate%d MRS MR12_B1=0x%02X MR13_B1=0x%02X MR14_B1=0x%02X MR22_B1=0x%02X\n"
	},
	{0x00610002,
	 "PMU1: AcsmOdtCtrl%02d 0x%02x\n"
	},
	{0x00620002,
	 "PMU1: AcsmCsMapCtrl%02d 0x%04x\n"
	},
	{0x00630002,
	 "PMU1: AcsmCsMapCtrl%02d 0x%04x\n"
	},
	{0x00640000,
	 "PMU1: HwtCAMode set\n"
	},
	{0x00650001,
	 "PMU3: DDR4 infinite preamble enter/exit mode %d\n"
	},
	{0x00660002,
	 "PMU1: In rxenb_train() csn=%d pstate=%d\n"
	},
	{0x00670000,
	 "PMU3: Finding DQS falling edge\n"
	},
	{0x00680000,
	 "PMU3: Searching for DDR3/LPDDR3/LPDDR4 read preamble\n"
	},
	{0x00690009,
	 "PMU3: dtsm fails Even Nibbles : %2x %2x %2x %2x %2x %2x %2x %2x %2x\n"
	},
	{0x006a0009,
	 "PMU3: dtsm fails Odd  Nibbles : %2x %2x %2x %2x %2x %2x %2x %2x %2x\n"
	},
	{0x006b0002,
	 "PMU3: Preamble search pass=%d anyfail=%d\n"
	},
	{0x006c0000,
	 "PMU: Error: RxEn training preamble not found\n"
	},
	{0x006d0000,
	 "PMU3: Found DQS pre-amble\n"
	},
	{0x006e0001,
	 "PMU: Error: Dbyte %d couldn't find the rising edge of DQS during RxEn Training\n"
	},
	{0x006f0000,
	 "PMU3: RxEn aligning to first rising edge of burst\n"
	},
	{0x00700001,
	 "PMU3: Decreasing RxEn delay by %d fine step to allow full capture of reads\n"
	},
	{0x00710001,
	 "PMU3: MREP Delay = %d\n"
	},
	{0x00720003,
	 "PMU3: Errcnt for MREP nib %2d delay = %2d is %d\n"
	},
	{0x00730002,
	 "PMU3: MREP nibble %d sampled a 1 at data buffer delay %d\n"
	},
	{0x00740002,
	 "PMU3: MREP nibble %d saw a 0 to 1 transition at data buffer delay %d\n"
	},
	{0x00750000,
	 "PMU2:  MREP did not find a 0 to 1 transition for all nibbles. Assuming 0 delay was already in the passing region for failing nibbles\n"
	},
	{0x00760002,
	 "PMU3: Training DIMM %d CSn %d\n"
	},
	{0x00770001,
	 "PMU3: exitCAtrain_lp3 cs 0x%x\n"
	},
	{0x00780001,
	 "PMU3: enterCAtrain_lp3 cs 0x%x\n"
	},
	{0x00790001,
	 "PMU3: CAtrain_switchmsb_lp3 cs 0x%x\n"
	},
	{0x007a0001,
	 "PMU3: CATrain_rdwr_lp3 looking for pattern %x\n"
	},
	{0x007b0000,
	 "PMU3: exitCAtrain_lp4\n"
	},
	{0x007c0001,
	 "PMU3: DEBUG enterCAtrain_lp4 1: cs 0x%x\n"
	},
	{0x007d0001,
	 "PMU3: DEBUG enterCAtrain_lp4 3: Put dbyte %d in async mode\n"
	},
	{0x007e0000,
	 "PMU3: DEBUG enterCAtrain_lp4 5: Send MR13 to turn on CA training\n"
	},
	{0x007f0003,
	 "PMU3: DEBUG enterCAtrain_lp4 7: idx = %d vref = %x mr12 = %x \n"
	},
	{0x00800001,
	 "PMU3: CATrain_rdwr_lp4 looking for pattern %x\n"
	},
	{0x00810004,
	 "PMU3: Phase %d CAreadbackA db:%d %x xo:%x\n"
	},
	{0x00820004,
	 "PMU3: Phase %d CAreadbackB db:%d %x xo:%x\n"
	},
	{0x00830005,
	 "PMU3: DEBUG lp4SetCatrVref 1: cs=%d chan=%d mr12=%x vref=%d.%d%%\n"
	},
	{0x00840003,
	 "PMU3: DEBUG lp4SetCatrVref 3: mr12 = %x send vref= %x to db=%d\n"
	},
	{0x00850003,
	 "PMU3: DEBUG lp4SetCatrVref 4: mr12= %x send vref= %x to db=%d\n"
	},
	{0x00860000,
	 "PMU10:Optimizing vref\n"
	},
	{0x00870004,
	 "PMU4:mr12:%2x cs:%d chan %d r:%4x\n"
	},
	{0x00880005,
	 "PMU3: i:%2d bstr:%2d bsto:%2d st:%d r:%d\n"
	},
	{0x00890002,
	 "Failed to find sufficient CA Vref Passing Region for CS %d channel %d\n"
	},
	{0x008a0005,
	 "PMU3:Found %d.%d%% MR12:%x for cs:%d chan %d\n"
	},
	{0x008b0002,
	 "PMU3:Calculated %d for AtxImpedence from acx %d.\n"
	},
	{0x008c0000,
	 "PMU3:CA Odt impedence ==0.  Use default vref.\n"
	},
	{0x008d0003,
	 "PMU3:Calculated %d.%d%% for Vref MR12=0x%x.\n"
	},
	{0x008e0000,
	 "PMU3: CAtrain_lp\n"
	},
	{0x008f0000,
	 "PMU3: CAtrain Begins.\n"
	},
	{0x00900001,
	 "PMU3: CAtrain_lp testing dly %d\n"
	},
	{0x00910001,
	 "PMU5: CA bitmap dump for cs %x\n"
	},
	{0x00920001,
	 "PMU5: CAA%d "
	},
	{0x00930001,
	 "%02x"
	},
	{0x00940000,
	 "\n"
	},
	{0x00950001,
	 "PMU5: CAB%d "
	},
	{0x00960001,
	 "%02x"
	},
	{0x00970000,
	 "\n"
	},
	{0x00980003,
	 "PMU3: anibi=%d, anibichan[anibi]=%d ,chan=%d\n"
	},
	{0x00990001,
	 "%02x"
	},
	{0x009a0001,
	 "\nPMU3:Raw CA setting :%x"
	},
	{0x009b0002,
	 "\nPMU3:ATxDly setting:%x margin:%d\n"
	},
	{0x009c0002,
	 "\nPMU3:InvClk ATxDly setting:%x margin:%d\n"
	},
	{0x009d0000,
	 "\nPMU3:No Range found!\n"
	},
	{0x009e0003,
	 "PMU3: 2 anibi=%d, anibichan[anibi]=%d ,chan=%d"
	},
	{0x009f0002,
	 "\nPMU3: no neg clock => CA setting anib=%d, :%d\n"
	},
	{0x00a00001,
	 "PMU3:Normal margin:%d\n"
	},
	{0x00a10001,
	 "PMU3:Inverted margin:%d\n"
	},
	{0x00a20000,
	 "PMU3:Using Inverted clock\n"
	},
	{0x00a30000,
	 "PMU3:Using normal clk\n"
	},
	{0x00a40003,
	 "PMU3: 3 anibi=%d, anibichan[anibi]=%d ,chan=%d\n"
	},
	{0x00a50002,
	 "PMU3: Setting ATxDly for anib %x to %x\n"
	},
	{0x00a60000,
	 "PMU: Error: CA Training Failed.\n"
	},
	{0x00a70000,
	 "PMU1: Writing MRs\n"
	},
	{0x00a80000,
	 "PMU4:Using MR12 values from 1D CA VREF training.\n"
	},
	{0x00a90000,
	 "PMU3:Writing all MRs to fsp 1\n"
	},
	{0x00aa0000,
	 "PMU10:Lp4Quickboot mode.\n"
	},
	{0x00ab0000,
	 "PMU3: Writing MRs\n"
	},
	{0x00ac0001,
	 "PMU10: Setting boot clock divider to %d\n"
	},
	{0x00ad0000,
	 "PMU3: Resetting DRAM\n"
	},
	{0x00ae0000,
	 "PMU3: setup for RCD initalization\n"
	},
	{0x00af0000,
	 "PMU3: pmu_exit_SR from dev_init()\n"
	},
	{0x00b00000,
	 "PMU3: initializing RCD\n"
	},
	{0x00b10000,
	 "PMU10: **** Executing 2D Image ****\n"
	},
	{0x00b20001,
	 "PMU10: **** Start DDR4 Training. PMU Firmware Revision 0x%04x ****\n"
	},
	{0x00b30001,
	 "PMU10: **** Start DDR3 Training. PMU Firmware Revision 0x%04x ****\n"
	},
	{0x00b40001,
	 "PMU10: **** Start LPDDR3 Training. PMU Firmware Revision 0x%04x ****\n"
	},
	{0x00b50001,
	 "PMU10: **** Start LPDDR4 Training. PMU Firmware Revision 0x%04x ****\n"
	},
	{0x00b60000,
	 "PMU: Error: Mismatched internal revision between DCCM and ICCM images\n"
	},
	{0x00b70001,
	 "PMU10: **** Testchip %d Specific Firmware ****\n"
	},
	{0x00b80000,
	 "PMU1: LRDIMM with EncodedCS mode, one DIMM\n"
	},
	{0x00b90000,
	 "PMU1: LRDIMM with EncodedCS mode, two DIMMs\n"
	},
	{0x00ba0000,
	 "PMU1: RDIMM with EncodedCS mode, one DIMM\n"
	},
	{0x00bb0000,
	 "PMU2: Starting LRDIMM MREP training for all ranks\n"
	},
	{0x00bc0000,
	 "PMU199: LRDIMM MREP training for all ranks completed\n"
	},
	{0x00bd0000,
	 "PMU2: Starting LRDIMM DWL training for all ranks\n"
	},
	{0x00be0000,
	 "PMU199: LRDIMM DWL training for all ranks completed\n"
	},
	{0x00bf0000,
	 "PMU2: Starting LRDIMM MRD training for all ranks\n"
	},
	{0x00c00000,
	 "PMU199: LRDIMM MRD training for all ranks completed\n"
	},
	{0x00c10000,
	 "PMU2: Starting RXEN training for all ranks\n"
	},
	{0x00c20000,
	 "PMU2: Starting write leveling fine delay training for all ranks\n"
	},
	{0x00c30000,
	 "PMU2: Starting LRDIMM MWD training for all ranks\n"
	},
	{0x00c40000,
	 "PMU199: LRDIMM MWD training for all ranks completed\n"
	},
	{0x00c50000,
	 "PMU2: Starting read deskew training\n"
	},
	{0x00c60000,
	 "PMU2: Starting SI friendly 1d RdDqs training for all ranks\n"
	},
	{0x00c70000,
	 "PMU2: Starting write leveling coarse delay training for all ranks\n"
	},
	{0x00c80000,
	 "PMU2: Starting 1d WrDq training for all ranks\n"
	},
	{0x00c90000,
	 "PMU2: Running DQS2DQ Oscillator for all ranks\n"
	},
	{0x00ca0000,
	 "PMU2: Starting 1d RdDqs training for all ranks\n"
	},
	{0x00cb0000,
	 "PMU2: Starting MaxRdLat training\n"
	},
	{0x00cc0000,
	 "PMU2: Redoing LRDIMM MREP training for all ranks\n"
	},
	{0x00cd0000,
	 "PMU199: LRDIMM MREP training for all ranks completed\n"
	},
	{0x00ce0000,
	 "PMU2: Redoing LRDIMM DWL training for all ranks\n"
	},
	{0x00cf0000,
	 "PMU199: LRDIMM DWL training for all ranks completed\n"
	},
	{0x00d00000,
	 "PMU2: Redoing LRDIMM MRD training for all ranks\n"
	},
	{0x00d10000,
	 "PMU199: LRDIMM MRD training for all ranks completed\n"
	},
	{0x00d20000,
	 "PMU2: Redoing LRDIMM MWD training for all ranks\n"
	},
	{0x00d30000,
	 "PMU199: LRDIMM MWD training for all ranks completed\n"
	},
	{0x00d40000,
	 "PMU2: Starting 2d RdDqs training for all ranks\n"
	},
	{0x00d50000,
	 "PMU2: Starting 2d WrDq training for all ranks\n"
	},
	{0x00d60002,
	 "PMU3:read_fifo %x %x\n"
	},
	{0x00d70001,
	 "PMU: Error: Invalid PhyDrvImpedance of 0x%x specified in message block.\n"
	},
	{0x00d80001,
	 "PMU: Error: Invalid PhyOdtImpedance of 0x%x specified in message block.\n"
	},
	{0x00d90001,
	 "PMU: Error: Invalid BPZNResVal of 0x%x specified in message block.\n"
	},
	{0x00da0005,
	 "PMU3: fixRxEnBackOff csn:%d db:%d dn:%d bo:%d dly:%x\n"
	},
	{0x00db0001,
	 "PMU3: fixRxEnBackOff dly:%x\n"
	},
	{0x00dc0000,
	 "PMU3: Entering setupPpt\n"
	},
	{0x00dd0000,
	 "PMU3: Start lp4PopulateHighLowBytes\n"
	},
	{0x00de0002,
	 "PMU3:Dbyte Detect: db%d received %x\n"
	},
	{0x00df0002,
	 "PMU3:getDqs2Dq read %x from dbyte %d\n"
	},
	{0x00e00002,
	 "PMU3:getDqs2Dq(2) read %x from dbyte %d\n"
	},
	{0x00e10001,
	 "PMU: Error: Dbyte %d read 0 from the DQS oscillator it is connected to\n"
	},
	{0x00e20002,
	 "PMU4: Dbyte %d dqs2dq = %d/32 UI\n"
	},
	{0x00e30003,
	 "PMU3:getDqs2Dq set dqs2dq:%d/32 ui (%d ps) from dbyte %d\n"
	},
	{0x00e40003,
	 "PMU3: Setting coarse delay in AtxDly chiplet %d from 0x%02x to 0x%02x \n"
	},
	{0x00e50003,
	 "PMU3: Clearing coarse delay in AtxDly chiplet %d from 0x%02x to 0x%02x \n"
	},
	{0x00e60000,
	 "PMU3: Performing DDR4 geardown sync sequence\n"
	},
	{0x00e70000,
	 "PMU1: Enter self refresh\n"
	},
	{0x00e80000,
	 "PMU1: Exit self refresh\n"
	},
	{0x00e90000,
	 "PMU: Error: No dbiEnable with lp4\n"
	},
	{0x00ea0000,
	 "PMU: Error: No dbiDisable with lp4\n"
	},
	{0x00eb0001,
	 "PMU1: DDR4 update Rx DBI Setting disable %d\n"
	},
	{0x00ec0001,
	 "PMU1: DDR4 update 2nCk WPre Setting disable %d\n"
	},
	{0x00ed0005,
	 "PMU1: read_delay: db%d lane%d delays[%2d] = 0x%02x (max 0x%02x)\n"
	},
	{0x00ee0001,
	 "PMU5: ID=%d -- db0  db1  db2  db3  db4  db5  db6  db7  db8  db9 --\n"
	},
	{0x00ef000b,
	 "PMU5: [%d]:0x %4x %4x %4x %4x %4x %4x %4x %4x %4x %4x\n"
	},
	{0x00f00003,
	 "PMU2: dump delays - pstate=%d dimm=%d csn=%d\n"
	},
	{0x00f10000,
	 "PMU3: Printing Mid-Training Delay Information\n"
	},
	{0x00f20001,
	 "PMU5: CS%d <<KEY>> 0 TrainingCntr <<KEY>> coarse(15:10) fine(9:0)\n"
	},
	{0x00f30001,
	 "PMU5: CS%d <<KEY>> 0 RxEnDly, 1 RxClkDly <<KEY>> coarse(10:6) fine(5:0)\n"
	},
	{0x00f40001,
	 "PMU5: CS%d <<KEY>> 0 TxDqsDly, 1 TxDqDly <<KEY>> coarse(9:6) fine(5:0)\n"
	},
	{0x00f50001,
	 "PMU5: CS%d <<KEY>> 0 RxPBDly <<KEY>> 1 Delay Unit ~= 7ps \n"
	},
	{0x00f60000,
	 "PMU5: all CS <<KEY>> 0 DFIMRL <<KEY>> Units = DFI clocks\n"
	},
	{0x00f70000,
	 "PMU5: all CS <<KEY>> VrefDACs <<KEY>> DAC(6:0)\n"
	},
	{0x00f80000,
	 "PMU1: Set DMD in MR13 and wrDBI in MR3 for training\n"
	},
	{0x00f90000,
	 "PMU: Error: getMaxRxen() failed to find largest rxen nibble delay\n"
	},
	{0x00fa0003,
	 "PMU2: getMaxRxen(): maxDly %d maxTg %d maxNib %d\n"
	},
	{0x00fb0003,
	 "PMU2: getRankMaxRxen(): maxDly %d Tg %d maxNib %d\n"
	},
	{0x00fc0000,
	 "PMU1: skipping CDD calculation in 2D image\n"
	},
	{0x00fd0001,
	 "PMU3: Calculating CDDs for pstate %d\n"
	},
	{0x00fe0003,
	 "PMU3: rxDly[%d][%d] = %d\n"
	},
	{0x00ff0003,
	 "PMU3: txDly[%d][%d] = %d\n"
	},
	{0x01000003,
	 "PMU3: allFine CDD_RR_%d_%d = %d\n"
	},
	{0x01010003,
	 "PMU3: allFine CDD_WW_%d_%d = %d\n"
	},
	{0x01020003,
	 "PMU3: CDD_RR_%d_%d = %d\n"
	},
	{0x01030003,
	 "PMU3: CDD_WW_%d_%d = %d\n"
	},
	{0x01040003,
	 "PMU3: allFine CDD_RW_%d_%d = %d\n"
	},
	{0x01050003,
	 "PMU3: allFine CDD_WR_%d_%d = %d\n"
	},
	{0x01060003,
	 "PMU3: CDD_RW_%d_%d = %d\n"
	},
	{0x01070003,
	 "PMU3: CDD_WR_%d_%d = %d\n"
	},
	{0x01080004,
	 "PMU3: F%dBC2x_B%d_D%d = 0x%02x\n"
	},
	{0x01090004,
	 "PMU3: F%dBC3x_B%d_D%d = 0x%02x\n"
	},
	{0x010a0004,
	 "PMU3: F%dBC4x_B%d_D%d = 0x%02x\n"
	},
	{0x010b0004,
	 "PMU3: F%dBC5x_B%d_D%d = 0x%02x\n"
	},
	{0x010c0004,
	 "PMU3: F%dBC8x_B%d_D%d = 0x%02x\n"
	},
	{0x010d0004,
	 "PMU3: F%dBC9x_B%d_D%d = 0x%02x\n"
	},
	{0x010e0004,
	 "PMU3: F%dBCAx_B%d_D%d = 0x%02x\n"
	},
	{0x010f0004,
	 "PMU3: F%dBCBx_B%d_D%d = 0x%02x\n"
	},
	{0x01100001,
	 "PMU1: enter_lp3: DEBUG: pstate = %d\n"
	},
	{0x01110001,
	 "PMU1: enter_lp3: DEBUG: dfifreqxlat_pstate = %d\n"
	},
	{0x01120001,
	 "PMU1: enter_lp3: DEBUG: pllbypass = %d\n"
	},
	{0x01130001,
	 "PMU1: enter_lp3: DEBUG: forcecal = %d\n"
	},
	{0x01140001,
	 "PMU1: enter_lp3: DEBUG: pllmaxrange = 0x%x\n"
	},
	{0x01150001,
	 "PMU1: enter_lp3: DEBUG: dacval_out = 0x%x\n"
	},
	{0x01160001,
	 "PMU1: enter_lp3: DEBUG: pllctrl3 = 0x%x\n"
	},
	{0x01170000,
	 "PMU3: Loading DRAM with BIOS supplied MR values and entering self refresh prior to exiting PMU code.\n"
	},
	{0x01180002,
	 "PMU3: Setting DataBuffer function space of dimmcs 0x%02x to %d\n"
	},
	{0x01190002,
	 "PMU4: Setting RCW FxRC%Xx = 0x%02x\n"
	},
	{0x011a0002,
	 "PMU4: Setting RCW FxRC%02X = 0x%02x\n"
	},
	{0x011b0001,
	 "PMU1: DDR4 update Rd Pre Setting disable %d\n"
	},
	{0x011c0002,
	 "PMU2: Setting BCW FxBC%Xx = 0x%02x\n"
	},
	{0x011d0002,
	 "PMU2: Setting BCW BC%02X = 0x%02x\n"
	},
	{0x011e0002,
	 "PMU2: Setting BCW PBA mode FxBC%Xx = 0x%02x\n"
	},
	{0x011f0002,
	 "PMU2: Setting BCW PBA mode BC%02X = 0x%02x\n"
	},
	{0x01200003,
	 "PMU4: BCW value for dimm %d, fspace %d, addr 0x%04x\n"
	},
	{0x01210002,
	 "PMU4: DB %d, value 0x%02x\n"
	},
	{0x01220000,
	 "PMU6: WARNING MREP underflow, set to min value -2 coarse, 0 fine\n"
	},
	{0x01230004,
	 "PMU6: LRDIMM Writing final data buffer fine delay value nib %2d, trainDly %3d, fineDly code %2d, new MREP fine %2d\n"
	},
	{0x01240003,
	 "PMU6: LRDIMM Writing final data buffer fine delay value nib %2d, trainDly %3d, fineDly code %2d\n"
	},
	{0x01250003,
	 "PMU6: LRDIMM Writing data buffer fine delay type %d nib %2d, code %2d\n"
	},
	{0x01260002,
	 "PMU6: Writing final data buffer coarse delay value dbyte %2d, coarse = 0x%02x\n"
	},
	{0x01270003,
	 "PMU4: data 0x%04x at MB addr 0x%08x saved at CSR addr 0x%08x\n"
	},
	{0x01280003,
	 "PMU4: data 0x%04x at MB addr 0x%08x restored from CSR addr 0x%08x\n"
	},
	{0x01290003,
	 "PMU4: data 0x%04x at MB addr 0x%08x saved at CSR addr 0x%08x\n"
	},
	{0x012a0003,
	 "PMU4: data 0x%04x at MB addr 0x%08x restored from CSR addr 0x%08x\n"
	},
	{0x012b0001,
	 "PMU3: Update BC00, BC01, BC02 for rank-dimm 0x%02x\n"
	},
	{0x012c0000,
	 "PMU3: Writing D4 RDIMM RCD Control words F0RC00 -> F0RC0F\n"
	},
	{0x012d0000,
	 "PMU3: Disable parity in F0RC0E\n"
	},
	{0x012e0000,
	 "PMU3: Writing D4 Data buffer Control words BC00 -> BC0E\n"
	},
	{0x012f0002,
	 "PMU1: setAltCL Sending MR0 0x%x cl=%d\n"
	},
	{0x01300002,
	 "PMU1: restoreFromAltCL Sending MR0 0x%x cl=%d\n"
	},
	{0x01310002,
	 "PMU1: restoreAcsmFromAltCL Sending MR0 0x%x cl=%d\n"
	},
	{0x01320002,
	 "PMU2: Setting D3R RC%d = 0x%01x\n"
	},
	{0x01330000,
	 "PMU3: Writing D3 RDIMM RCD Control words RC0 -> RC11\n"
	},
	{0x01340002,
	 "PMU0: VrefDAC0/1 vddqStart %d dacToVddq %d\n"
	},
	{0x01350001,
	 "PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated LPDDR4 receivers. Please see the pub databook\n"
	},
	{0x01360001,
	 "PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated DDR4 receivers. Please see the pub databook\n"
	},
	{0x01370001,
	 "PMU0: PHY VREF @ (%d/1000) VDDQ\n"
	},
	{0x01380002,
	 "PMU0: initalizing phy vrefDacs to %d ExtVrefRange %x\n"
	},
	{0x01390002,
	 "PMU0: initalizing global vref to %d range %d\n"
	},
	{0x013a0002,
	 "PMU4: Setting initial device vrefDQ for CS%d to MR6 = 0x%04x\n"
	},
	{0x013b0003,
	 "PMU1: In write_level_fine() csn=%d dimm=%d pstate=%d\n"
	},
	{0x013c0000,
	 "PMU3: Fine write leveling hardware search increasing TxDqsDly until full bursts are seen\n"
	},
	{0x013d0000,
	 "PMU3: Exiting write leveling mode\n"
	},
	{0x013e0001,
	 "PMU3: got %d for cl in load_wrlvl_acsm\n"
	},
	{0x013f0003,
	 "PMU1: In write_level_coarse() csn=%d dimm=%d pstate=%d\n"
	},
	{0x01400003,
	 "PMU3: left eye edge search db:%d ln:%d dly:0x%x\n"
	},
	{0x01410003,
	 "PMU3: right eye edge search db:%d ln:%d dly:0x%x\n"
	},
	{0x01420004,
	 "PMU3: eye center db:%d ln:%d dly:0x%x (maxdq:%x)\n"
	},
	{0x01430003,
	 "PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n"
	},
	{0x01440003,
	 "PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n"
	},
	{0x01450002,
	 "PMU3: Coarse write leveling dbyte%2d is still failing for TxDqsDly=0x%04x\n"
	},
	{0x01460002,
	 "PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n"
	},
	{0x01470000,
	 "PMU: Error: Failed write leveling coarse\n"
	},
	{0x01480001,
	 "PMU3: got %d for cl in load_wrlvl_acsm\n"
	},
	{0x01490003,
	 "PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n"
	},
	{0x014a0003,
	 "PMU3: left eye edge search db:%d ln:%d dly:0x%x\n"
	},
	{0x014b0003,
	 "PMU3: right eye edge search db: %d ln: %d dly: 0x%x\n"
	},
	{0x014c0004,
	 "PMU3: eye center db: %d ln: %d dly: 0x%x (maxdq: 0x%x)\n"
	},
	{0x014d0003,
	 "PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n"
	},
	{0x014e0003,
	 "PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n"
	},
	{0x014f0002,
	 "PMU3: Coarse write leveling nibble%2d is still failing for TxDqsDly=0x%04x\n"
	},
	{0x01500002,
	 "PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n"
	},
	{0x01510000,
	 "PMU: Error: Failed write leveling coarse\n"
	},
	{0x01520003,
	 "PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n"
	},
	{0x01530005,
	 "PMU2: Write level: dbyte %d nib%d dq/dmbi %2d dqsfine 0x%04x dqDly 0x%04x\n"
	},
	{0x01540002,
	 "PMU3: Coarse write leveling nibble%2d is still failing for TxDqsDly=0x%04x\n"
	},
	{0x01550002,
	 "PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n"
	},
	{0x01560000,
	 "PMU: Error: Failed write leveling coarse\n"
	},
	{0x01570001,
	 "PMU3: DWL delay = %d\n"
	},
	{0x01580003,
	 "PMU3: Errcnt for DWL nib %2d delay = %2d is %d\n"
	},
	{0x01590002,
	 "PMU3: DWL nibble %d sampled a 1 at delay %d\n"
	},
	{0x015a0003,
	 "PMU3: DWL nibble %d passed at delay %d. Rising edge was at %d\n"
	},
	{0x015b0000,
	 "PMU2: DWL did nto find a rising edge of memclk for all nibbles. Failing nibbles assumed to have rising edge at fine delay 63\n"
	},
	{0x04000000,
	 "PMU: Error:Mailbox Buffer Overflowed.\n"
	},
	{0x04010000,
	 "PMU: Error:Mailbox Buffer Overflowed.\n"
	},
	{0x04020000,
	 "PMU: ***** Assertion Error - terminating *****\n"
	},
	{0x04030002,
	 "PMU1: swapByte db %d by %d\n"
	},
	{0x04040003,
	 "PMU3: get_cmd_dly max(%d ps, %d memclk) = %d\n"
	},
	{0x04050002,
	 "PMU0: Write CSR 0x%06x 0x%04x\n"
	},
	{0x04060002,
	 "PMU0: hwt_init_ppgc_prbs(): Polynomial: %x, Deg: %d\n"
	},
	{0x04070001,
	 "PMU: Error: acsm_set_cmd to non existant instruction adddress %d\n"
	},
	{0x04080001,
	 "PMU: Error: acsm_set_cmd with unknown ddr cmd 0x%x\n"
	},
	{0x0409000c,
	 "PMU1: acsm_addr %02x, acsm_flgs %04x, ddr_cmd %02x, cmd_dly %02x, ddr_addr %04x, ddr_bnk %02x, ddr_cs %02x, cmd_rcnt %02x, AcsmSeq0/1/2/3 %04x %04x %04x %04x\n"
	},
	{0x040a0000,
	 "PMU: Error: Polling on ACSM done failed to complete in acsm_poll_done()...\n"
	},
	{0x040b0000,
	 "PMU1: acsm RUN\n"
	},
	{0x040c0000,
	 "PMU1: acsm STOPPED\n"
	},
	{0x040d0002,
	 "PMU1: acsm_init: acsm_mode %04x mxrdlat %04x\n"
	},
	{0x040e0002,
	 "PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 2 and 5, resp. CL=%d CWL=%d\n"
	},
	{0x040f0002,
	 "PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 5. CL=%d CWL=%d\n"
	},
	{0x04100002,
	 "PMU1: setAcsmCLCWL: CASL %04d WCASL %04d\n"
	},
	{0x04110001,
	 "PMU: Error: Reserved value of register F0RC0F found in message block: 0x%04x\n"
	},
	{0x04120001,
	 "PMU3: Written MRS to CS=0x%02x\n"
	},
	{0x04130001,
	 "PMU3: Written MRS to CS=0x%02x\n"
	},
	{0x04140000,
	 "PMU3: Entering Boot Freq Mode.\n"
	},
	{0x04150001,
	 "PMU: Error: Boot clock divider setting of %d is too small\n"
	},
	{0x04160000,
	 "PMU3: Exiting Boot Freq Mode.\n"
	},
	{0x04170002,
	 "PMU3: Writing MR%d OP=%x\n"
	},
	{0x04180000,
	 "PMU: Error: Delay too large in slomo\n"
	},
	{0x04190001,
	 "PMU3: Written MRS to CS=0x%02x\n"
	},
	{0x041a0000,
	 "PMU3: Enable Channel A\n"
	},
	{0x041b0000,
	 "PMU3: Enable Channel B\n"
	},
	{0x041c0000,
	 "PMU3: Enable All Channels\n"
	},
	{0x041d0002,
	 "PMU2: Use PDA mode to set MR%d with value 0x%02x\n"
	},
	{0x041e0001,
	 "PMU3: Written Vref with PDA to CS=0x%02x\n"
	},
	{0x041f0000,
	 "PMU1: start_cal: DEBUG: setting CalRun to 1\n"
	},
	{0x04200000,
	 "PMU1: start_cal: DEBUG: setting CalRun to 0\n"
	},
	{0x04210001,
	 "PMU1: lock_pll_dll: DEBUG: pstate = %d\n"
	},
	{0x04220001,
	 "PMU1: lock_pll_dll: DEBUG: dfifreqxlat_pstate = %d\n"
	},
	{0x04230001,
	 "PMU1: lock_pll_dll: DEBUG: pllbypass = %d\n"
	},
	{0x04240001,
	 "PMU3: SaveLcdlSeed: Saving seed seed %d\n"
	},
	{0x04250000,
	 "PMU1: in phy_defaults()\n"
	},
	{0x04260003,
	 "PMU3: ACXConf:%d MaxNumDbytes:%d NumDfi:%d\n"
	},
	{0x04270005,
	 "PMU1: setAltAcsmCLCWL setting cl=%d cwl=%d\n"
	},
};
#endif /* DEBUG */
#endif
