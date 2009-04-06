# =============================================================================
#
# Make file for BlueBoard examples
#
# Processor: LPC214x
#
# Example:   
#
# Compiler: GCC ARM  3.4.3 (GNUARM + Cygwin) or 4.10
#
# !!!! Do not use  GCC ARM 4.0.1. It makes a few problems  !!!!
#
#  No dependencies support. Call 'make clean' and than 'make' to rebuild all
#
# =============================================================================

# =========  General =============

FORMAT = ihex
#TST_N  = 1

# =========  Pathes ==============

#ROOTDIR  = D:/CYGWIN
INCLUDES = -I ./ -I ../Source/App/Inc -I ../Source/Drivers/Inc -I ../Source/efsl/Inc -I ../Source/LpcUsbStack/Inc -I ../Source/os/tn_kernel/Inc -I ../Source/Platform/Inc -I ../Source/Utils/Inc -I ../Source/Drivers/AtmelFlash/Inc -I ../Source/Drivers/Audio/Inc -I ../Source/Drivers/VGA/Inc -I ../Source/Drivers/PS2KB/Inc -I ../Source/Drivers/74hc595/Inc -I ../Source/Drivers/Buzzer/Inc -I ../Source/Drivers/LCD/Inc

#TOOLSROOT = D:/GNUARM
TOOLS     =  arm-elf-

PRJPATH   = .

SOURCEPATH = ../Source

DEBUG              = $(PRJPATH)/DEBUG
OBJ                = $(PRJPATH)/OBJ
APP_SRC_PATH       = $(SOURCEPATH)/App/Src
DRIVERS_SRC_PATH   = $(SOURCEPATH)/Drivers
EFSL_SRC_PATH      = $(SOURCEPATH)/efsl/Src
LPCUSB_SRC_PATH    = $(SOURCEPATH)/LpcUsbStack/Src
TNKERNEL_SRC_PATH  = $(SOURCEPATH)/os/tn_kernel/Src
PLATFORM_SRC_PATH  = $(SOURCEPATH)/Platform/Src
UTILS_SRC_PATH     = $(SOURCEPATH)/Utils/Src
TESTFILESPATH      = $(SOURCEPATH)/App/Src


#===========  Files  ==============

#--  Project target file name (without extension).

IMAGE = app

#--  Linker script

LDSCRIPT = $(PRJPATH)/lpc2148_gcc.ld

#-- List of objects

OBJECTS = \
  $(OBJ)/tnkernel_startup_gcc.o\
  $(OBJ)/startup_hardware_init.o\
  $(OBJ)/tn_port_asm.o\
  $(OBJ)/fs_api.o\
  $(OBJ)/led.o\
  $(OBJ)/message_queue.o\
  $(OBJ)/test.o\
  $(OBJ)/VCOM.o\
  $(OBJ)/74HC595.o\
  $(OBJ)/PlayInt.o\
  $(OBJ)/buzzer.o\
  $(OBJ)/lcd.o\
  $(OBJ)/extISR.o\
  $(OBJ)/ps2kbd.o\
  $(OBJ)/vga.o\
  $(OBJ)/debug.o\
  $(OBJ)/dir.o\
  $(OBJ)/disc.o\
  $(OBJ)/efs.o\
  $(OBJ)/extract.o\
  $(OBJ)/fat.o\
  $(OBJ)/file.o\
  $(OBJ)/fs.o\
  $(OBJ)/lpc2000_dbg_printf.o\
  $(OBJ)/lpc2000_sd.o\
  $(OBJ)/sd.o\
  $(OBJ)/ioman.o\
  $(OBJ)/ls.o\
  $(OBJ)/mkfs.o\
  $(OBJ)/partition.o\
  $(OBJ)/plibc.o\
  $(OBJ)/time.o\
  $(OBJ)/ui.o\
  $(OBJ)/usbcontrol.o\
  $(OBJ)/usbhw_lpc.o\
  $(OBJ)/usbinit.o\
  $(OBJ)/usbstdreq.o\
  $(OBJ)/tn.o\
  $(OBJ)/tn_dqueue.o\
  $(OBJ)/tn_event.o\
  $(OBJ)/tn_mem.o\
  $(OBJ)/tn_mutex.o\
  $(OBJ)/tn_port.o\
  $(OBJ)/tn_sem.o\
  $(OBJ)/tn_tasks.o\
  $(OBJ)/tn_utils.o\
  $(OBJ)/tn_user.o\
  $(OBJ)/adc.o\
  $(OBJ)/dac.o\
  $(OBJ)/ext_interrupt.o\
  $(OBJ)/hardware.o\
  $(OBJ)/i2c.o\
  $(OBJ)/pwm.o\
  $(OBJ)/rtc.o\
  $(OBJ)/rtcISR.o\
  $(OBJ)/spi.o\
  $(OBJ)/ssp.o\
  $(OBJ)/timer.o\
  $(OBJ)/uart.o\
  $(OBJ)/uart_isr.o\
  $(OBJ)/circular_buf.o\
  $(OBJ)/malloc.o\
  $(OBJ)/SoftTimer.o\
  $(OBJ)/utils.o\
  $(OBJ)/app.o
# ---------------------------------------------------------------------------
#  Tools, tools options(flags)
# ---------------------------------------------------------------------------

SHELL   = D:/cygwin/bin/sh
CC      = $(TOOLS)gcc
AS      = $(TOOLS)as
CPP     = $(TOOLS)g++
LD      = $(TOOLS)ld
OBJCOPY = $(TOOLS)objcopy
OBJDUMP = $(TOOLS)objdump
SIZE    = $(TOOLS)size
NM      = $(TOOLS)nm
REMOVE  = rm -f
COPY    = cp


OPTFLAGS = -mcpu=arm7tdmi
DEBUG =

#-- Assembler flags

ADEFS =

#ASMOPTS  = $(OPTFLAGS) $(INCLUDES) -D_GNU_ASSEMBLER_ \
#           -mlittle-endian -nostdinc -x assembler-with-cpp \
#           $(ADEFS)  -Wa,-gstabs


ASMOPTS  =  --traditional-format -march=armv4t -EL

# -Wa,-adhlns=$(<:.S=.lst),-g$(DEBUG)

#-- Compiler  flags

CDEF =

CCOPTS  = $(OPTFLAGS) $(INCLUDES) $(CDEF) -c \
     -mlittle-endian -Wall -gdwarf-2 -std=c99 \
     -O1 -march=armv4t -mlittle-endian -fno-builtin

#-- Linker    flags

LDFLAGS =  -nostartfiles -Wl,-Map=$(OBJ)/$(IMAGE).map,--cref  -lc \
           $(NEWLIBLPC) -lm -lc -lgcc -T$(LDSCRIPT)


#-- Combine all necessary flags and optional flags.

ALL_CFLAGS =  -I. $(CFLAGS)
ALL_ASFLAGS = -I. $(ASFLAGS)

# ---------------------------------------------------------------------------
#  Build
# ---------------------------------------------------------------------------

all: $(PRJPATH)/$(IMAGE).elf  sizeafter


#--- Compiling/Assembling ---

$(OBJ)/tnkernel_startup_gcc.o: $(PRJPATH)/tnkernel_startup_gcc.s
	$(AS) $(ASMOPTS) $(PRJPATH)/tnkernel_startup_gcc.s -o $(OBJ)/tnkernel_startup_gcc.o
$(OBJ)/startup_hardware_init.o: $(PRJPATH)/startup_hardware_init.s
	$(AS) $(ASMOPTS) $(PRJPATH)/startup_hardware_init.s -o $(OBJ)/startup_hardware_init.o
$(OBJ)/tn_port_asm.o: $(TNKERNEL_SRC_PATH)/tn_port_asm.s
	$(AS) $(ASMOPTS) $(TNKERNEL_SRC_PATH)/tn_port_asm.s -o $(OBJ)/tn_port_asm.o
#---- Compiling Application code -----
$(OBJ)/fs_api.o: $(APP_SRC_PATH)/fs_api.c
	$(CC) $(CCOPTS) $(APP_SRC_PATH)/fs_api.c -o $(OBJ)/fs_api.o
$(OBJ)/led.o: $(APP_SRC_PATH)/led.c
	$(CC) $(CCOPTS) $(APP_SRC_PATH)/led.c -o $(OBJ)/led.o
$(OBJ)/message_queue.o: $(APP_SRC_PATH)/message_queue.c
	$(CC) $(CCOPTS) $(APP_SRC_PATH)/message_queue.c -o $(OBJ)/message_queue.o
$(OBJ)/test.o: $(APP_SRC_PATH)/test.c
	$(CC) $(CCOPTS) $(APP_SRC_PATH)/test.c -o $(OBJ)/test.o
$(OBJ)/VCOM.o: $(APP_SRC_PATH)/VCOM.c
	$(CC) $(CCOPTS) $(APP_SRC_PATH)/VCOM.c -o $(OBJ)/VCOM.o
#---- Compiling Drivers -----
$(OBJ)/74HC595.o: $(DRIVERS_SRC_PATH)/74HC595/Src/74HC595.c
	$(CC) $(CCOPTS) $(DRIVERS_SRC_PATH)/74HC595/Src/74HC595.c -o $(OBJ)/74HC595.o
$(OBJ)/PlayInt.o: $(DRIVERS_SRC_PATH)/Audio/Src/PlayInt.c
	$(CC) $(CCOPTS) $(DRIVERS_SRC_PATH)/Audio/Src/PlayInt.c -o $(OBJ)/PlayInt.o
$(OBJ)/buzzer.o: $(DRIVERS_SRC_PATH)/Buzzer/Src/buzzer.c
	$(CC) $(CCOPTS) $(DRIVERS_SRC_PATH)/Buzzer/Src/buzzer.c -o $(OBJ)/buzzer.o
$(OBJ)/lcd.o: $(DRIVERS_SRC_PATH)/LCD/Src/lcd.c
	$(CC) $(CCOPTS) $(DRIVERS_SRC_PATH)/LCD/Src/lcd.c -o $(OBJ)/lcd.o
$(OBJ)/ps2kbd.o: $(DRIVERS_SRC_PATH)/PS2KB/Src/ps2kbd.c
	$(CC) $(CCOPTS) $(DRIVERS_SRC_PATH)/PS2KB/Src/ps2kbd.c -o $(OBJ)/ps2kbd.o
$(OBJ)/extISR.o: $(DRIVERS_SRC_PATH)/PS2KB/Src/extISR.c
	$(CC) $(CCOPTS) $(DRIVERS_SRC_PATH)/PS2KB/Src/extISR.c -o $(OBJ)/extISR.o
$(OBJ)/vga.o: $(DRIVERS_SRC_PATH)/VGA/Src/VGA.c
	$(CC) $(CCOPTS) $(DRIVERS_SRC_PATH)/VGA/Src/VGA.c -o $(OBJ)/vga.o
#---- Compiling EFSL --------
$(OBJ)/debug.o: $(EFSL_SRC_PATH)/debug.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/debug.c -o $(OBJ)/debug.o
$(OBJ)/dir.o: $(EFSL_SRC_PATH)/dir.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/dir.c -o $(OBJ)/dir.o
$(OBJ)/disc.o: $(EFSL_SRC_PATH)/disc.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/disc.c -o $(OBJ)/disc.o
$(OBJ)/efs.o: $(EFSL_SRC_PATH)/efs.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/efs.c -o $(OBJ)/efs.o
$(OBJ)/extract.o: $(EFSL_SRC_PATH)/extract.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/extract.c -o $(OBJ)/extract.o
$(OBJ)/fat.o: $(EFSL_SRC_PATH)/fat.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/fat.c -o $(OBJ)/fat.o
$(OBJ)/file.o: $(EFSL_SRC_PATH)/file.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/file.c -o $(OBJ)/file.o
$(OBJ)/fs.o: $(EFSL_SRC_PATH)/fs.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/fs.c -o $(OBJ)/fs.o
$(OBJ)/lpc2000_dbg_printf.o: $(EFSL_SRC_PATH)/interfaces/lpc2000_dbg_printf.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/interfaces/lpc2000_dbg_printf.c -o $(OBJ)/lpc2000_dbg_printf.o
$(OBJ)/lpc2000_sd.o: $(EFSL_SRC_PATH)/interfaces/lpc2000_sd.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/interfaces/lpc2000_sd.c -o $(OBJ)/lpc2000_sd.o
$(OBJ)/sd.o: $(EFSL_SRC_PATH)/interfaces/sd.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/interfaces/sd.c -o $(OBJ)/sd.o
$(OBJ)/ioman.o: $(EFSL_SRC_PATH)/ioman.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/ioman.c -o $(OBJ)/ioman.o
$(OBJ)/ls.o: $(EFSL_SRC_PATH)/ls.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/ls.c -o $(OBJ)/ls.o
$(OBJ)/mkfs.o: $(EFSL_SRC_PATH)/mkfs.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/mkfs.c -o $(OBJ)/mkfs.o
$(OBJ)/partition.o: $(EFSL_SRC_PATH)/partition.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/partition.c -o $(OBJ)/partition.o
$(OBJ)/plibc.o: $(EFSL_SRC_PATH)/plibc.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/plibc.c -o $(OBJ)/plibc.o
$(OBJ)/time.o: $(EFSL_SRC_PATH)/time.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/time.c -o $(OBJ)/time.o
$(OBJ)/ui.o: $(EFSL_SRC_PATH)/ui.c
	$(CC) $(CCOPTS) $(EFSL_SRC_PATH)/ui.c -o $(OBJ)/ui.o
# ---- Compiling LPC usb stack -----
$(OBJ)/usbcontrol.o: $(LPCUSB_SRC_PATH)/usbcontrol.c
	$(CC) $(CCOPTS) $(LPCUSB_SRC_PATH)/usbcontrol.c -o $(OBJ)/usbcontrol.o
$(OBJ)/usbhw_lpc.o: $(LPCUSB_SRC_PATH)/usbhw_lpc.c
	$(CC) $(CCOPTS) $(LPCUSB_SRC_PATH)/usbhw_lpc.c -o $(OBJ)/usbhw_lpc.o
$(OBJ)/usbinit.o: $(LPCUSB_SRC_PATH)/usbinit.c
	$(CC) $(CCOPTS) $(LPCUSB_SRC_PATH)/usbinit.c -o $(OBJ)/usbinit.o
$(OBJ)/usbstdreq.o: $(LPCUSB_SRC_PATH)/usbstdreq.c
	$(CC) $(CCOPTS) $(LPCUSB_SRC_PATH)/usbstdreq.c -o $(OBJ)/usbstdreq.o
#--- Compiling TnKernel Sources ----
$(OBJ)/tn.o: $(TNKERNEL_SRC_PATH)/tn.c
	$(CC) $(CCOPTS) $(TNKERNEL_SRC_PATH)/tn.c -o $(OBJ)/tn.o
$(OBJ)/tn_dqueue.o: $(TNKERNEL_SRC_PATH)/tn_dqueue.c
	$(CC) $(CCOPTS) $(TNKERNEL_SRC_PATH)/tn_dqueue.c -o $(OBJ)/tn_dqueue.o
$(OBJ)/tn_event.o: $(TNKERNEL_SRC_PATH)/tn_event.c
	$(CC) $(CCOPTS) $(TNKERNEL_SRC_PATH)/tn_event.c -o $(OBJ)/tn_event.o
$(OBJ)/tn_mem.o: $(TNKERNEL_SRC_PATH)/tn_mem.c
	$(CC) $(CCOPTS) $(TNKERNEL_SRC_PATH)/tn_mem.c -o $(OBJ)/tn_mem.o
$(OBJ)/tn_mutex.o: $(TNKERNEL_SRC_PATH)/tn_mutex.c
	$(CC) $(CCOPTS) $(TNKERNEL_SRC_PATH)/tn_mutex.c -o $(OBJ)/tn_mutex.o
$(OBJ)/tn_port.o: $(TNKERNEL_SRC_PATH)/tn_port.c
	$(CC) $(CCOPTS) $(TNKERNEL_SRC_PATH)/tn_port.c -o $(OBJ)/tn_port.o
$(OBJ)/tn_sem.o: $(TNKERNEL_SRC_PATH)/tn_sem.c
	$(CC) $(CCOPTS) $(TNKERNEL_SRC_PATH)/tn_sem.c -o $(OBJ)/tn_sem.o
$(OBJ)/tn_tasks.o: $(TNKERNEL_SRC_PATH)/tn_tasks.c
	$(CC) $(CCOPTS) $(TNKERNEL_SRC_PATH)/tn_tasks.c -o $(OBJ)/tn_tasks.o
$(OBJ)/tn_utils.o: $(TNKERNEL_SRC_PATH)/tn_utils.c
	$(CC) $(CCOPTS) $(TNKERNEL_SRC_PATH)/tn_utils.c -o $(OBJ)/tn_utils.o
$(OBJ)/tn_user.o: $(PLATFORM_SRC_PATH)/tn_user.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/tn_user.c -o $(OBJ)/tn_user.o
#---- Compiling Platform Specific Code -------
$(OBJ)/adc.o: $(PLATFORM_SRC_PATH)/adc.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/adc.c -o $(OBJ)/adc.o
$(OBJ)/dac.o: $(PLATFORM_SRC_PATH)/dac.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/dac.c -o $(OBJ)/dac.o
$(OBJ)/ext_interrupt.o: $(PLATFORM_SRC_PATH)/ext_interrupt.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/ext_interrupt.c -o $(OBJ)/ext_interrupt.o
$(OBJ)/hardware.o: $(PLATFORM_SRC_PATH)/hardware.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/hardware.c -o $(OBJ)/hardware.o
$(OBJ)/i2c.o: $(PLATFORM_SRC_PATH)/i2c.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/i2c.c -o $(OBJ)/i2c.o
$(OBJ)/pwm.o: $(PLATFORM_SRC_PATH)/pwm.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/pwm.c -o $(OBJ)/pwm.o
$(OBJ)/rtc.o: $(PLATFORM_SRC_PATH)/rtc.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/rtc.c -o $(OBJ)/rtc.o
$(OBJ)/rtcISR.o: $(PLATFORM_SRC_PATH)/rtcISR.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/rtcISR.c -o $(OBJ)/rtcISR.o
$(OBJ)/spi.o: $(PLATFORM_SRC_PATH)/spi.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/spi.c -o $(OBJ)/spi.o
$(OBJ)/ssp.o: $(PLATFORM_SRC_PATH)/ssp.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/ssp.c -o $(OBJ)/ssp.o
$(OBJ)/timer.o: $(PLATFORM_SRC_PATH)/timer.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/timer.c -o $(OBJ)/timer.o
$(OBJ)/uart.o: $(PLATFORM_SRC_PATH)/uart.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/uart.c -o $(OBJ)/uart.o
$(OBJ)/uart_isr.o: $(PLATFORM_SRC_PATH)/uart_isr.c
	$(CC) $(CCOPTS) $(PLATFORM_SRC_PATH)/uart_isr.c -o $(OBJ)/uart_isr.o
#---- Compiling Utilities sources -----
$(OBJ)/circular_buf.o: $(UTILS_SRC_PATH)/circular_buf.c
	$(CC) $(CCOPTS) $(UTILS_SRC_PATH)/circular_buf.c -o $(OBJ)/circular_buf.o
$(OBJ)/malloc.o: $(UTILS_SRC_PATH)/malloc.c
	$(CC) $(CCOPTS) $(UTILS_SRC_PATH)/malloc.c -o $(OBJ)/malloc.o
$(OBJ)/SoftTimer.o: $(UTILS_SRC_PATH)/SoftTimer.c
	$(CC) $(CCOPTS) $(UTILS_SRC_PATH)/SoftTimer.c -o $(OBJ)/SoftTimer.o
$(OBJ)/utils.o: $(UTILS_SRC_PATH)/utils.c
	$(CC) $(CCOPTS) $(UTILS_SRC_PATH)/utils.c -o $(OBJ)/utils.o
#---- Compiling Application main ----
$(OBJ)/App.o: $(TESTFILESPATH)/App.c
	$(CC) $(CCOPTS) $(TESTFILESPATH)/App.c -o $(OBJ)/App.o

#-- Linking ---

$(PRJPATH)/$(IMAGE).elf: $(OBJECTS)

	$(REMOVE)  $(OBJ)/$(IMAGE).elf
	$(REMOVE)  $(OBJ)/$(IMAGE).hex
	$(REMOVE)  $(OBJ)/$(IMAGE).lst
	$(REMOVE)  $(OBJ)/$(IMAGE).map

	$(CC) $(ALL_CFLAGS) $(OBJECTS) --output $(OBJ)/$(notdir $@) $(LDFLAGS)

	$(OBJCOPY) -O$(FORMAT)    $(OBJ)/$(IMAGE).elf  $(OBJ)/$(IMAGE).hex
#	$(NM) -n $(OBJ)/$(IMAGE).elf

#--- Display size of file.

#HEXSIZE = $(SIZE) --target=$(FORMAT) $(OBJ)/$(IMAGE).hex
ELFSIZE = $(SIZE) -A $(OBJ)/$(IMAGE).elf

sizeafter:
	@if [ -f $(OBJ)/$(IMAGE).elf ]; then echo; $(ELFSIZE); echo; fi


# ===========================================================================

.PHONY:   clean

clean:

	$(REMOVE) $(OBJECTS)

# ===========================================================================
# ===========================================================================
# ===========================================================================



