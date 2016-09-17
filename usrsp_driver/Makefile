SYS_ROOT=/home/sergio/ITESMSemanaI/raspbianSysRoot
CC_PREFIX=arm-linux-gnueabihf

#CC=$(CC_PREFIX)-gcc
CC=gcc

OUT_DIR=obj

LIB_PATHS+=-L"lib"
LIB_PATHS+=-L"$(OUT_DIR)"

INCLUDE_PATHS+=-I"/include"
INCLUDE_PATHS+=-I"/include/linux"
INCLUDE_PATHS+=-I"/include/$(CC_PREFIX)"

DEBUG_FLAGS=-g

#LIBS+=-lpthread

# use raspberry pi syste root
#CFLAGS= $(LIB_PATHS) $(INCLUDE_PATHS) $(DEBUG_FLAGS) -Wall -O0 -pthread --sysroot=$(SYS_ROOT)
CFLAGS= $(LIB_PATHS) $(INCLUDE_PATHS) $(DEBUG_FLAGS) -Wall -O0 -pthread #--sysroot=$(SYS_ROOT)
#CFLAGS= $(LIB_PATHS) $(INCLUDE_PATHS) $(DEBUG_FLAGS) -Wall -O0 -pthread -DINTEGRATION_TEST #--sysroot=$(SYS_ROOT)

RASPICAN_LIB_SRC=raspi_candrv.c raspi_gui_ipc.c
RASPICAN_LIB_OBJ=$(RASPICAN_LIB_SRC:%.c=$(OUT_DIR)/%.o)

RASPI_QUEUE_SRC=raspi_can_queue.c
RASPI_QUEUE_OBJ=$(RASPI_QUEUE_SRC:%.c=$(OUT_DIR)/%.o)

RASPICAN_TEST_SRC=can_test.c
RASPICAN_TEST_OBJ=$(RASPICAN_TEST_SRC:%.c=$(OUT_DIR)/%.o)

DIRS=$(OUT_DIR)

TARGETS=raspi_can_test libraspi_can.a  #libraspi_can.so

TARGETS_OUT=$(TARGETS:%=$(OUT_DIR)/%)

all: dirs $(TARGETS_OUT) 

dirs:
	mkdir -p $(DIRS)

.PHONY: clean
clean:
	rm -rf $(TARGETS_OUT) $(RASPICAN_LIB_OBJ) $(RASPICAN_TEST_OBJ) $(RASPI_QUEUE_OBJ) $(OUT_DIR) 

#raspi can tests
$(OUT_DIR)/raspi_can_test: $(RASPICAN_TEST_OBJ) $(OUT_DIR)/libraspi_can.a
	$(CC) $(CFLAGS) $(LIBS)  -o $@ $(RASPICAN_TEST_OBJ) -lraspi_can

#raspi can library
#$(OUT_DIR)/libraspi_can.so: $(RASPICAN_LIB_OBJ)
#	$(CC) $(CFLAGS) -shared $(RASPICAN_LIB_OBJ) -o $@

#raspi can static library
$(OUT_DIR)/libraspi_can.a: $(RASPICAN_LIB_OBJ) $(RASPI_QUEUE_OBJ)
	$(AR) rcs $@ $(RASPICAN_LIB_OBJ) $(RASPI_QUEUE_OBJ)

#pattern rules
$(OUT_DIR)/%.o:%.c
	$(CC) $(CFLAGS) -fpic -c $^ -o $@
