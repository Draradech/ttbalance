# MCU name
MCU = atmega644p


# Processor frequency
F_CPU = 20000000


# Target file name
TARGET = ttbalance


# C source files
SRC = actuator.c environment.c init.c main.c ram.c sensor.c serial.c


# Optimization level
OPT = s


# Debug info
DEBUG = dwarf-2


#---------------- Compiler Options ----------------
CFLAGS  = -mmcu=$(MCU)
CFLAGS += -O$(OPT)
CFLAGS += -g$(DEBUG)
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -ffreestanding
CFLAGS += -combine
CFLAGS += -fwhole-program
CFLAGS += -Wall
CFLAGS += -Winline
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wsign-compare
CFLAGS += -std=gnu99
CFLAGS += -DF_CPU=$(F_CPU)UL
CFLAGS += -I.


#---------------- Linker Options ----------------
LDFLAGS  = -Wl,--relax
LDFLAGS += -lm


#---------------- Programming Options (avrdude) ----------------
# Programming hardware
AVRDUDE_PROGRAMMER = avr109

# Programming port
AVRDUDE_PORT = /dev/rfcomm0
#AVRDUDE_PORT = \\\\.\COM17

# Avrdude config
#AVRDUDE_CONFIG = D:\Privat\Programme\arduino-1.0.6\hardware\tools\avr\etc\avrdude.conf

# Programming command
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep

AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) #-C $(AVRDUDE_CONFIG)


#============================================================================
# Define programs and commands.
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AVRDUDE = avrdude
REMOVE = rm -f


# Define Messages
# English
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_COMPILING_LINKING = Compiling and Linking:
MSG_CLEANING = Cleaning project:
MSG_PROGRAM_FLASH = Programming target flash:
MSG_PROGRAM_EEPROM = Programming target eeprom:
MSG_NEWLINE = \n

# Default target.
all: build size

build: elf hex eep lss

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss


# Display size of file.
ELFSIZE = $(SIZE) --format=avr --mcu=$(MCU) $(TARGET).elf

size:
	@printf "$(MSG_NEWLINE)"
	@printf "$(MSG_NEWLINE)"
	$(ELFSIZE)


# Program the device
program: $(TARGET).hex
	#@cat bootloader > $(AVRDUDE_PORT)
	@printf "$(MSG_NEWLINE)"
	@printf "$(MSG_PROGRAM_FLASH)"
	@printf "$(MSG_NEWLINE)"
	#@sleep 1
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)
	#@sleep 1
	#@cat endbootloader > $(AVRDUDE_PORT)


# Program the eeprom
program_eeprom: $(TARGET).eep
	@cat bootloader > $(AVRDUDE_PORT)
	@printf "$(MSG_NEWLINE)"
	@printf "$(MSG_PROGRAM_EEPROM)"
	@printf "$(MSG_NEWLINE)"
	@sleep 1
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_EEPROM)
	@sleep 1
	@cat endbootloader > $(AVRDUDE_PORT)


# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@printf "$(MSG_NEWLINE)"
	@printf "$(MSG_FLASH) $@"
	@printf "$(MSG_NEWLINE)"
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.eep: %.elf
	@printf "$(MSG_NEWLINE)"
	@printf "$(MSG_EEPROM) $@"
	@printf "$(MSG_NEWLINE)"
	-$(OBJCOPY) -j .eeprom --set-section-flags .eeprom=alloc,load \
	--change-section-lma .eeprom=0 -O ihex $< $@


# Create extended listing file from ELF output file.
%.lss: %.elf
	@printf "$(MSG_NEWLINE)"
	@printf "$(MSG_EXTENDED_LISTING) $@"
	@printf "$(MSG_NEWLINE)"
	$(OBJDUMP) -h -S $< > $@


# Link: create ELF output file from source files.
%.elf: $(SRC)
	@printf "$(MSG_NEWLINE)"
	@printf "$(MSG_COMPILING_LINKING) $@"
	@printf "$(MSG_NEWLINE)"
	$(CC) $(CFLAGS) $(LDFLAGS) $^ --output $@


# Target: clean project.
clean: begin clean_list end

clean_list :
	@printf "$(MSG_NEWLINE)"
	@printf "$(MSG_CLEANING)"
	@printf "$(MSG_NEWLINE)"
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).eep
	$(REMOVE) $(TARGET).lss

# Listing of phony targets.
.PHONY : $(SRC) \
all begin end sizebefore sizeafter gccversion \
build elf hex eep lss \
clean clean_list program program_eeprom
