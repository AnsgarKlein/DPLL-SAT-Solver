CC_FLAGS		+=	-Wall -std=c99

SOURCES			:=	$(wildcard src/*.c)
HEADERS			:=	${SOURCES:.c=.h}
OBJECTS			:=	${SOURCES:.c=.o}

EXECUTABLE		:=	dpll


ifdef VERBOSE_DPLL
	CC_FLAGS	+=	-D VERBOSE_DPLL
endif


.SECONDARY:
	@#

.PHONY: all clean install uninstall debug optimized
	@#


all: $(EXECUTABLE)
	@#

clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)

install:
	@#

uninstall:
	@#

debug: CC_FLAGS += --debug
debug: all

optimized: CC_FLAGS += -O3
optimized: all

%.o: %.c
	@echo "  CC      $@"
	@$(CC) $(CC_FLAGS) -c "$<" -o "$@"

$(EXECUTABLE): $(OBJECTS)
	@echo "  LD      $@"
	@$(CC) $(OBJECTS) -o "$(EXECUTABLE)"

