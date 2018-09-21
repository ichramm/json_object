###########################
# File: 	target-doc.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_DOC.MK
TARGET_DOC.MK := INCLUDED

.PHONY: doc
doc:
	$(call WHITE,"Compling documentation...")
	$(PRINT)doxygen doc/Doxygen.cfg $(OUTPUT)
	
endif
