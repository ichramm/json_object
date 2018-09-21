###########################
# File: 	target-install.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_INSTALL.MK
TARGET_INSTALL.MK := INCLUDED

.SECONDEXPANSION:

DESTDIR ?= /opt/

.PHONY: install $$(addsuffix -install, $$(TGTS))
install: $$(addsuffix -install, $$(TGTS))

endif
