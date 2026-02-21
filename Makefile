TARGET := arm_dev
BUILD_TYPE = Release

TRIPLE  = 	arm-none-eabi
CC 		=	${TRIPLE}-gcc
LD 		= 	${TRIPLE}-ld
AS 		= 	${TRIPLE}-as
GDB 	= 	${TRIPLE}-gdb
OBJCOPY =  	${TRIPLE}-objcopy

LDFLAGS := -mcpu=cortex-m3 -mfloat-abi=soft -mthumb --specs=nano.specs --specs=nosys.specs
MAPFLAGS:= -Wl,--gc-sections,--print-gc-sections,--cref
CFLAGS := -mcpu=cortex-m3 -mthumb -mno-thumb-interwork -mfpu=vfp -msoft-float\
 -mfix-cortex-m3-ldrd -std=gnu11 -Os -Wall -fno-tree-loop-distribute-patterns\
  -fdata-sections -ffunction-sections -Werror

ifeq ($(BUILD_TYPE), Debug)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP 

# Modules
MODULES:= HAL drivers app startup

BUILD_DIR:= build
LINKER_SCRIPT = linker.ld

SRC_DIRS := $(addsuffix /src, $(MODULES))
SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c) $(wildcard $(dir)/*.S))

#Find all .c source files
SRCS_C := $(filter %.c, $(SRCS))
#Find all .S source files
SRCS_S := $(filter %.S, $(SRCS))

INC_DIRS := $(addsuffix /include, $(MODULES))
INCLUDES := $(addprefix -I, $(INC_DIRS))

RM_DIRS := $(addprefix build/, $(SRC_DIRS))

# Replace .c with $(BUILD_DIR)/%.o for C source files
OBJS_C := $(SRCS_C:%.c=$(BUILD_DIR)/%.o)
# Replace .S with $(BUILD_DIR)/%.o for Assembly source files
OBJS_S := $(SRCS_S:%.S=$(BUILD_DIR)/%.o)

OBJS := $(OBJS_C) $(OBJS_S)

$(BUILD_DIR)/$(TARGET).elf: $(OBJS) 
	@$(CC) $(LDFLAGS) $(INCLUDES) $(MAPFLAGS) -o $@ $(OBJS) -T$(LINKER_SCRIPT) -Wl,-Map="$(BUILD_DIR)/$(TARGET).map"

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

flash:
	@# openocd -d2 -s /opt/openocd/scripts -f interface/stlink-v2.cfg -c "transport select hla_swd" -f target/stm32f1x.cfg -c "program {build/$(TARGET).elf}  verify reset; shutdown;"
	openocd -d2 -f interface/stlink.cfg -c "transport select hla_swd" -f target/stm32f1x.cfg -c "program {build/$(TARGET).elf}  verify reset; shutdown;"

all: $(BUILD_DIR)/$(TARGET).elf

clean:
	rm -rf 	$(RM_DIRS)\
			$(BUILD_DIR)/$(TARGET).elf\
			$(BUILD_DIR)/$(TARGET).map
