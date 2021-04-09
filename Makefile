## Copyright (c) 2021 LabOS developers and contributors

PHONY := __all
__all:

srctree := .
objtree := $(or ${OBJDIR},obj)
instree := $(or ${INSTALLDIR},out)

export srctree objtree instree

include scripts/Makefile.include

MAKEFLAGS += --no-print-directory

CC 	  ?= clang
AR 	  ?= ar
LD 	  ?= ld
AS 	  ?= nasm
SHELL ?= sh

INCLUDE_DIR := -Iinclude

BUILD_CFLAGS := $(strip -O1 					   \
						-g  					   \
						-Wall -Wno-unused-function \
						${DEBUG}				   \
						${CCFLAGS})
BUILD_ASFLAGS := $(strip -g -F dwarf ${DEBUG} ${ASFLAGS})
BUILD_LDFLAGS := $(strip ${LDFLAGS})

export SHELL CC AR LD AS INCLUDE_DIR BUILD_CFLAGS BUILD_ASFLAGS BUILD_LDFLAGS

include scripts/Makefile.config

build-dirs    := src
kernel        := $(objtree)/LabKernel
linker-script := $(srctree)/src/arch/$(ARCH)/link_$(BITS).lds
kernel-objs   := $(addsuffix /built-in.a,$(addprefix $(objtree)/,$(build-dirs)))

PHONY += $(build-dirs)

$(build-dirs):
	@$(MAKE) $(build)=$@

$(kernel-objs): $(build-dirs)
	@

$(kernel): $(kernel-objs) $(linker-script)
	@cpp $(DEFINE_FLAGS) $(linker_script) | grep -v '^#' > $(kernel-file).ld
	@${LD} ${BUILD_LDFLAGS}    \
		  -T $(kernel-file).ld \
		  -o $@                \
		  -n                   \
		  --whole-archive      \
		  $(kernel-objs)       \
		  --no-whole-archive

build: $(kernel-file)

install: build
	-cp $(kernel-file) $(installdir)
	-sync $(installdir)

clean:
	-rm -rf $(objtree)

__all: build

PHONY += build clean

.PHONY: $(PHONY)
