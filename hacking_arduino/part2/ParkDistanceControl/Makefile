ARD_REV = 22
ARD_HOME = /Applications/Arduino.app/Contents/Resources/Java
AVR_HOME = $(ARD_HOME)/hardware/tools/avr
ARD_BIN = $(AVR_HOME)/bin
AVRDUDE = $(ARD_BIN)/avrdude
AVRDUDE_CONF = $(AVR_HOME)/etc/avrdude.conf
PROGRAMMER = stk500v1
MON_TERM = xterm
MON_SPEED = 57600
MON_CMD = screen
PORT = /dev/tty.usbmodemfa141
BOARD = uno
INC_DIRS = ./inc
LIB_DIRS = $(addprefix $(ARD_HOME)/libraries/, $(LIBS))
#LIBS = LiquidCrystal
include ../Makefile.master

