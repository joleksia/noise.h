# ========

# root directory of the makefile
MK_ROOT	= $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
MK_NAME = noise

# ========

MODULES = \
		$(MK_ROOT)noise.h \
		$(MK_ROOT)perlin.h \

# ========

.PHONY : all

all : install 

.PHONY : install

install :
	mkdir -p /usr/local/include/$(MK_NAME)/
	cp -f $(MODULES) /usr/local/include/$(MK_NAME)/

.PHONY : remove

remove :
	rm -fr /usr/local/include/$(MK_NAME)

# ========
