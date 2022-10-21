CC=gcc

DEFINES=
CFLAGS=-std=gnu11 -Wall -Wpedantic -fPIC -c $(addprefix -D, $(DEFINES))
LFLAGS=

TARGET=debug

_CORE_OBJ=state.o

LIB_OBJ=$(addprefix target/$(TARGET)/obj/, $(_CORE_OBJ))
LIB_T=target/$(TARGET)/libkettle.so

# KETTLE_OBJ=$(addprefix target/$(TARGET)/obj/, $(_CORE_OBJ) main.o)
# KETTLE_T=target/$(TARGET)/kettle

all: $(TARGET)

debug: CC+=-g
debug: DEFINES+=DEBUG
debug: build

release: CC+=-O3
release: DEFINES+=RELEASE
release: build

build: $(LIB_T)

$(LIB_T): $(LIB_OBJ)
	@mkdir -p $(@D)
	$(CC) $(LFLAGS) -shared -o $@ $^

# $(KETTLE_T): $(KETTLE_OBJ)
# 	@mkdir -p $(@D)
# 	$(CC) $(LFLAGS) -o $@ $^

target/$(TARGET)/obj/%.o: src/%.c $(wildcard src/*.h)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $<

test: $(TARGET)
	cd test && $(MAKE) TARGET=$(TARGET)

clean:
	rm -r target 2> /dev/null || true
	cd test && $(MAKE) clean

.PHONY: all debug release build test clean
