# GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=release_x32
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),release_x32)
  itoa_config = release_x32
endif
ifeq ($(config),release_x64)
  itoa_config = release_x64
endif

PROJECTS := itoa

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

itoa:
ifneq (,$(itoa_config))
	@echo "==== Building itoa ($(itoa_config)) ===="
	@${MAKE} --no-print-directory -C . -f itoa.make config=$(itoa_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f itoa.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  release_x32"
	@echo "  release_x64"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   itoa"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"