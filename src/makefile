CC=gcc
INCLUDE_DIR=../include
LIBS_DIR=../lib
OBJS_DIR=./obj
LIBS=-lm
C_FLAGS=-I$(INCLUDE_DIR)
DYNAMIC_LIB_NAME=libdbcore

_DEPS = utils.h
DEPS = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_LIBOBJ = utils.o tables.o
LIBOBJ = $(patsubst %,$(OBJS_DIR)/%,$(_LIBOBJ))
LIBSRC = utils.c tables.c
_APPOBJ = main.o
APPOBJ = $(patsubst %,$(OBJS_DIR)/%,$(_APPOBJ))

$(OBJS_DIR)/utils.o: utils.c
	$(CC) -c -fPIC $< $(C_FLAGS) -o $@

$(OBJS_DIR)/tables.o: tables.c
	$(CC) -c -fPIC $< $(C_FLAGS) -o $@

$(LIBS_DIR)/$(DYNAMIC_LIB_NAME).so:$(LIBOBJ)
	$(CC) -shared $^ -o $@

$(APPOBJ): main.c $(DEPS)
	$(CC) -c $< $(C_FLAGS) -o $@
	
../bin/main.out: $(APPOBJ) $(LIBS_DIR)/$(DYNAMIC_LIB_NAME).so
	$(CC) $^ $(C_FLAGS) $(LIBS) $(LIBS_DIR)/$(DYNAMIC_LIB_NAME).so -o $@

.phony: clean
clean:
	rm -f ../bin/main.out $(OBJS_DIR)/*.o $(LIBS_DIR)/*.a $(LIBS_DIR)/*.so

.phony: all
all: $(LIBS_DIR)/$(DYNAMIC_LIB_NAME).so ../bin/main.out
