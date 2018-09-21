###########################
# File: 	target-project.mk
#
# Date: 	Apr 02, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_PROJECT.MK
TARGET_PROJECT.MK := INCLUDED

.PHONY: project
project:
ifeq (,$(wildcard $(ROOT)/conf))
	$(call BLUE,"Creating directory conf...")
	$(PRINT)$(MKDIR) conf
endif
ifeq (,$(wildcard $(ROOT)/doc))
	$(call BLUE,"Creating directory doc...")
	$(PRINT)$(MKDIR) doc
endif
ifeq (,$(wildcard $(ROOT)/example))
	$(call BLUE,"Creating directory example...")
	$(PRINT)$(MKDIR) example
endif
ifeq (,$(wildcard $(ROOT)/include))
	$(call BLUE,"Creating directory include...")
	$(PRINT)$(MKDIR) include
endif
ifeq (,$(wildcard $(ROOT)/scripts))
	$(call BLUE,"Creating directory scripts...")
	$(PRINT)$(MKDIR) scripts
endif
ifeq (,$(wildcard $(ROOT)/src))
	$(call BLUE,"Creating directory src...")
	$(PRINT)$(MKDIR) src
endif
ifeq (,$(wildcard $(ROOT)/test))
	$(call BLUE,"Creating directory test...")
	$(PRINT)$(MKDIR) test
endif
ifeq (,$(wildcard $(ROOT)/CHANGELOG.md))
	$(call CYAN,"Creating file CHANGELOG.md...")
	$(PRINT)$(TOUCH) CHANGELOG.md
endif
ifeq (,$(wildcard $(ROOT)/INSTALL.md))
	$(call CYAN,"Creating file INSTALL.md...")
	$(PRINT)$(TOUCH) INSTALL.md
endif
ifeq (,$(wildcard $(ROOT)/README.md))
	$(call CYAN,"Creating file README.md...")
	$(PRINT)$(TOUCH) README.md
endif

endif
