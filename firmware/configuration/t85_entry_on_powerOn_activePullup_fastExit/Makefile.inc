# Name: Makefile
# Project: Micronucleus
# License: GNU GPL v2 (see License.txt)

# Controller type: ATtiny 85 - 16.5 MHz
# Configuration:   Default + ENTRY_POWER_ON + START_WITHOUT_PULLUP + FAST_EXIT_NO_USB_MS=300
# Last Change:     Jun 16,2020


F_CPU = 16500000
DEVICE = attiny85

# hexadecimal address for bootloader section to begin. To calculate the best value:
# - make clean; make main.hex; ### output will list data: 2124 | 1624 (or something like that)
# - for the size of your device (8kb = 1024 * 8 = 8192) subtract above value = 6068 | 5668
# - How many pages in is that? 6068 / 64 (tiny85 page size in bytes) = 94.8125 | 102.625
# - round that down to 94 | 102
# - our new bootloader address is 94 | 102 * 64 = 6016 | 6528, in hex = 1780 | 1980
BOOTLOADER_ADDRESS = 19C0

FUSEOPT = -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xfe:m
FUSEOPT_DISABLERESET = -U lfuse:w:0xe1:m -U efuse:w:0xfe:m -U hfuse:w:0x5d:m

#---------------------------------------------------------------------
# ATtiny85
#---------------------------------------------------------------------
# Fuse extended byte:
# 0xFE = - - - -   - 1 1 0
#                        ^
#                        |
#                        +---- SELFPRGEN (enable self programming flash)
#
# Fuse high byte:
# 0xdd = 1 1 0 1   1 1 0 1
#        ^ ^ ^ ^   ^ \-+-/
#        | | | |   |   +------ BODLEVEL 2..0 (brownout trigger level -> 2.7V)
#        | | | |   +---------- EESAVE (preserve EEPROM on Chip Erase -> not preserved)
#        | | | +-------------- WDTON (watchdog timer always on -> disable)
#        | | +---------------- SPIEN (enable serial programming -> enabled)
#        | +------------------ DWEN (debug wire enable)
#        +-------------------- RSTDISBL (disable external reset -> enabled)
#
# Fuse high byte ("no reset": external reset disabled, can't program through SPI anymore)
# 0x5d = 0 1 0 1   1 1 0 1
#        ^ ^ ^ ^   ^ \-+-/
#        | | | |   |   +------ BODLEVEL 2..0 (brownout trigger level -> 2.7V)
#        | | | |   +---------- EESAVE (preserve EEPROM on Chip Erase -> not preserved)
#        | | | +-------------- WDTON (watchdog timer always on -> disable)
#        | | +---------------- SPIEN (enable serial programming -> enabled)
#        | +------------------ DWEN (debug wire enable)
#        +-------------------- RSTDISBL (disable external reset -> disabled!)
#
# Fuse low byte:
# 0xe1 = 1 1 1 0   0 0 0 1
#        ^ ^ \+/   \--+--/
#        | |  |       +------- CKSEL 3..0 (clock selection -> HF PLL)
#        | |  +--------------- SUT 1..0 (BOD enabled, fast rising power)
#        | +------------------ CKOUT (clock output on CKOUT pin -> disabled)
#        +-------------------- CKDIV8 (divide clock by 8 -> don't divide)

###############################################################################
