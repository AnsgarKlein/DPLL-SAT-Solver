SOURCEDIR       :=  src
BUILDDIR        :=  build

SOURCES         :=  $(notdir $(wildcard $(SOURCEDIR)/*.c))
HEADERS         :=  $(notdir $(wildcard $(SOURCEDIR)/*.h))
OBJECTS         :=  ${SOURCES:.c=.o}

EXECUTABLE      :=  dpll

CC_FLAGS        +=  -Wall
CC_FLAGS        +=  -Wextra
CC_FLAGS        +=  -pedantic
CC_FLAGS        +=  -std=c99

ifdef VERBOSE_DPLL
	CC_FLAGS	+=	-D VERBOSE_DPLL
endif


.SECONDARY:
	@#

.PHONY: all clean install uninstall debug optimized
	@#

all: $(BUILDDIR)/$(EXECUTABLE)
	@#

clean:
	rm -f $(addprefix $(BUILDDIR)/, $(OBJECTS))
	rm -f $(BUILDDIR)/$(EXECUTABLE)

install:
	@#

uninstall:
	@#

debug: CC_FLAGS += --debug
debug: all

optimized: CC_FLAGS += -O3
optimized: all

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c | $(BUILDDIR)
	@echo "  CC      $(notdir $@)"
	@$(CC) $(CC_FLAGS) -c "$<" -o "$@"

$(BUILDDIR)/$(EXECUTABLE): $(addprefix $(BUILDDIR)/, $(OBJECTS)) | $(BUILDDIR)
	@echo "  LD      $(notdir $@)"
	@$(CC) $(addprefix $(BUILDDIR)/, $(OBJECTS)) -o "$@"

