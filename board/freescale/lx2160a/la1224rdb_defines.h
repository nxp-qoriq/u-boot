#ifndef __LA1224RDB_DEFINES_H_
#define __LA1224RDB_DEFINES_H_
#define GPIO1_BASE_ADDR           (CONFIG_SYS_IMMR +  0x01300000)
#define GPIO1_INPUT_BUFFER_ENABLE (GPIO1_BASE_ADDR +  0x18)
#define GPIO1_DATA_REG            (GPIO1_BASE_ADDR +  0x8)
#define GPIO1_DIR_REG             (GPIO1_BASE_ADDR +  0x0)
#define GPIO1_ENABLE_INPUT        0x00000004
#define GPIO1_PIN_29_HIGH         0x00000004
#define GPIO1_PIN_29_LOW          0x00000000

/* Toggle TCLK 10 times max for RevA */
#define TCLK_TOGGLE_MAX_COUNT		10
#define SDHC1_DS_PMUX_DSPI              2
#endif

#if defined(CONFIG_TARGET_LA1224RDB)
// LA1224RDB RevA, RevB and RevC
// GPIO IO Expander Port1
// Port1[6:7]
//    00 = RevA
//    01 = RevB
//    10 = RevC
#define REVA                            0
#define REVB                            1
#define REVC                            2

/* IO expander addresses */
#define I2C_IO_EXP_ADDR_PRI_REVAB	0x20
#define I2C_IO_EXP_ADDR_SEC_REVAB       0x21
#define I2C_IO_EXP_ADDR_PRI_REVC	0x22
#define I2C_IO_EXP_ADDR_SEC_REVC        0x23

#define IO_EXAPNDER_CONF_REG_REVAB	0x7
#define IO_EXAPNDER_CONF_REG_REVC	0xE
#define IO_EXAPNDER_INPUT_REG_REVAB	0x1
#define IO_EXAPNDER_INPUT_REG_REVC	0x2
#define IO_EXAPNDER_OUTPUT_REG_REVAB	0x3
#define IO_EXAPNDER_OUTPUT_REG_REVC	0x6

// Get the Port1[6:7] bit of PCAL6416A
// bit position ( 7 6 5 4 3 2 1 0)
#define BOARD_REV_SHIFT_MASK            6
#define BOARD_REV_MASK                  3
#endif /*CONFIG_TARGET_LA1224RDB*/

