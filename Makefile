# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS_RELEASE = -O2
CFLAGS_DEBUG = -g -Og
CFLAGS_CSTANDARD = c11
CFLAGS_WARN  = -Wall
CFLAGS_WARN += -Wpedantic
CFLAGS_WARN += -Wextra
#CFLAGS_WARN += -Wdouble-promotion
#CFLAGS_WARN += -Wmissing-include-dirs
#CFLAGS_WARN += -Wswitch-default
#CFLAGS_WARN += -Wswitch-enum
#CFLAGS_WARN += -Wunused-parameter
#CFLAGS_WARN += -Wfloat-equal
#CFLAGS_WARN += -Wundef
#CFLAGS_WARN += -Wshadow
#CFLAGS_WARN += -Wunsafe-loop-optimizations
#CFLAGS_WARN += -Wbad-function-cast
#CFLAGS_WARN += -Wcast-qual
#CFLAGS_WARN += -Wmissing-declarations
#CFLAGS_WARN += -Wstrict-prototypes
#CFLAGS_WARN += -Wold-style-definition
#CFLAGS_WARN += -Waggregate-return
#CFLAGS_WARN += -Wlogical-op
#CFLAGS_WARN += -Wconversion

CFLAGS_DEFINE = -D _GNU_SOURCE

CFLAGS = $(CFLAGS_WARN) -std=$(CFLAGS_CSTANDARD) $(CFLAGS_DEFINE)

# define any directories containing header files other than /usr/include
#
#INCLUDES = -I/home/newhall/include -I../include
INCLUDES  = -I.
INCLUDES += -I./abcc_adapt
INCLUDES += -I./abcc_drv/inc
INCLUDES += -I./abcc_drv/src
INCLUDES += -I./abcc_obj
INCLUDES += -I./abcc_abp
INCLUDES += -I./example_app
INCLUDES += -I./abcc_obj/nw_obj
INCLUDES += -I./ext_lib/Logprint

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
#LFLAGS = -L/home/newhall/lib -L../lib
LFLAGS =

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname
#   option, something like (this will link in libmylib.so and libm.so:
#LIBS = -lmylib -lm
LIBS = -lbcm2835

# define the C source files
SRCS  = ./main.c
SRCS += ./abcc_adapt/abcc_sw_port.c
SRCS += ./abcc_adapt/abcc_sys_adapt.c
SRCS += ./abcc_adapt/abcc_time.c
SRCS += ./abcc_drv/src/abcc_cmd_seq.c
SRCS += ./abcc_drv/src/abcc_debug_err.c
SRCS += ./abcc_drv/src/abcc_handler.c
SRCS += ./abcc_drv/src/abcc_link.c
SRCS += ./abcc_drv/src/abcc_mem.c
SRCS += ./abcc_drv/src/abcc_remap.c
SRCS += ./abcc_drv/src/abcc_seg.c
SRCS += ./abcc_drv/src/abcc_setup.c
SRCS += ./abcc_drv/src/abcc_timer.c
SRCS += ./abcc_drv/src/par30/abcc_handler_par30.c
SRCS += ./abcc_drv/src/par30/abcc_par30_drv.c
SRCS += ./abcc_drv/src/par/abcc_handler_par.c
SRCS += ./abcc_drv/src/par/abcc_par_drv.c
SRCS += ./abcc_drv/src/serial/abcc_crc16.c
SRCS += ./abcc_drv/src/serial/abcc_handler_ser.c
SRCS += ./abcc_drv/src/serial/abcc_serial_drv.c
SRCS += ./abcc_drv/src/spi/abcc_crc32.c
SRCS += ./abcc_drv/src/spi/abcc_handler_spi.c
SRCS += ./abcc_drv/src/spi/abcc_spi_drv.c
SRCS += ./abcc_obj/ad_obj.c
SRCS += ./abcc_obj/anb_fsi_obj.c
SRCS += ./abcc_obj/app_fsi_obj.c
SRCS += ./abcc_obj/app_obj.c
SRCS += ./abcc_obj/asm_obj.c
SRCS += ./abcc_obj/etn_obj.c
SRCS += ./abcc_obj/mqtt_obj.c
SRCS += ./abcc_obj/nw_obj/bac.c
SRCS += ./abcc_obj/nw_obj/ccl.c
SRCS += ./abcc_obj/nw_obj/cfn.c
SRCS += ./abcc_obj/nw_obj/cop.c
SRCS += ./abcc_obj/nw_obj/dev.c
SRCS += ./abcc_obj/nw_obj/dpv1.c
SRCS += ./abcc_obj/nw_obj/ect.c
SRCS += ./abcc_obj/nw_obj/eip.c
SRCS += ./abcc_obj/nw_obj/epl.c
SRCS += ./abcc_obj/nw_obj/mod.c
SRCS += ./abcc_obj/nw_obj/prt.c
SRCS += ./abcc_obj/opcua_obj.c
SRCS += ./abcc_obj/safe_obj.c
SRCS += ./abcc_obj/sync_obj.c
SRCS += ./example_app/appl_abcc_handler.c
SRCS += ./example_app/appl_adimap_alltypes.c
SRCS += ./example_app/appl_adimap_asm.c
SRCS += ./example_app/appl_adimap_bacnet.c
SRCS += ./example_app/appl_adimap_separate16.c
SRCS += ./example_app/appl_adimap_simple16.c
SRCS += ./example_app/appl_adimap_speed_example.c
SRCS += ./example_app/appl_adimap_sync.c
SRCS += ./example_app/appl_adimap_verif.c
SRCS += ./example_app/appl_obj_port.c
SRCS += ./ext_lib/Logprint/logprint.c

# define the C object files
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c=.o)

# define the executable file
TARGET = main

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

release: CFLAGS += $(CFLAGS_RELEASE)
release: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) $(OBJS) *~ $(TARGET)

# DO NOT DELETE THIS LINE -- make depend needs it
