###########################
# File: 	target-clean.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_CLEAN.MK
TARGET_CLEAN.MK := INCLUDED

.SECONDEXPANSION:

.PHONY: clean $$(addsuffix -clean, $$(TGTS)) $$(addsuffix -clean, $$(EXAMPLE.TGTS)) $$(addsuffix -clean, $$(TEST.TGTS))
#clean: $$(addsuffix -clean, $$(TGTS))
#clean: $$(addsuffix -clean, $$(EXAMPLE.TGTS))
#clean: $$(addsuffix -clean, $$(TEST.TGTS))
clean:
	$(call RED,"Removing $(BUILDDIR.base)...")
	$(PRINT)$(RM) $(BUILDDIR.base)
	$(call RED,"Removing $(OUTDIR.analysis)...")
	$(PRINT)$(RM) $(OUTDIR.analysis)
	$(call RED,"Removing $(OUTDIR.coverage)...")
	$(PRINT)$(RM) $(OUTDIR.coverage)
	$(call RED,"Removing $(OUTDIR.profile)...")
	$(PRINT)$(RM) $(OUTDIR.profile)

endif
