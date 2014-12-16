PACKAGES		:=	gobject-2.0
PACKAGES		+=	glib-2.0
PACKAGES		+=	gio-2.0
PACKAGES		+=	gee-1.0

GEN_FLAGS		:=	$(addprefix --pkg ,$(PACKAGES))
GEN_FLAGS		+=	--ccode

CC_FLAGS		:=	-w
CC_FLAGS		+=	$(shell pkg-config --cflags $(PACKAGES))

LD_FLAGS		+=	$(shell pkg-config --libs $(PACKAGES))

SOURCES			:=	$(wildcard src/*.vala)
CFILES			:=	${SOURCES:.vala=.c}
OBJECTS			:=	${SOURCES:.vala=.o}
VAPIFILES		:=	${SOURCES:.vala=.vapi}

EXECUTABLE		:=	dpll


ifdef VERBOSE_DPLL
	GEN_FLAGS	+=	-D VERBOSE_DPLL
endif


.SECONDARY:
	@#

.PHONY: all clean install uninstall debug optimized
	@#


all: $(EXECUTABLE)
	@#

clean:
	rm -f $(CFILES)
	rm -f $(OBJECTS)
	rm -f $(VAPIFILES)
	rm -f $(EXECUTABLE)

install:
	@#

uninstall:
	@#

debug: GEN_FLAGS += --debug
debug: CC_FLAGS += --debug
debug: all

optimized: CC_CFLAGS += -03
optimized: all


%.vapi: %.vala
	@echo "  GEN     $@"
	@valac --fast-vapi="$@" $<

MISSINGVAPIFILES=$(subst $(subst .c,.vapi,$@),,$(VAPIFILES))
%.c: %.vala $(MISSINGVAPIFILES)
	@echo "  GEN     $@"
	@valac $(GEN_FLAGS) $(addprefix --use-fast-vapi=,$(MISSINGVAPIFILES)) "$<"

%.o: %.c
	@echo "  CC      $@"
	@$(CC) $(CC_FLAGS) -c "$<" -o "$@"

$(EXECUTABLE): $(OBJECTS)
	@echo "  LD      $@"
	@$(CC) $(OBJECTS) $(LD_FLAGS) -o "$(EXECUTABLE)"

