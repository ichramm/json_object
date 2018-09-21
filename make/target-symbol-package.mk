###########################
# File: 	target-symbol-package.mk
#
# Date: 	Sep 05, 2016
#
# Author: 	Juan Ramirez <juan.ramirez@intraway.com>
# Owner: 	Juan Ramirez <juan.ramirez@intraway.com>
#
###########################

ifndef TARGET_DEBUGSYMBOLS.MK
TARGET_DEBUGSYMBOLS.MK := INCLUDED

.SECONDEXPANSION:

ifeq ($(LD_HAS_BUILD_ID),yes)
.PHONY: symbol-package $$(addsuffix -symbol-package, $$(TGTS))
symbol-package: package
symbol-package: $$(addsuffix -symbol-package, $$(TGTS))
	$(foreach TGT, $(TGTS), \
		$(PRINT)if [ -n "$(PACKAGEDIR)" ]; then \
			if [ "$(VERBOSEU)" = "OFF" ]; then \
				if [ "$(TPUT_WORKS)" = "yes" ]; then tput setaf 3; fi; \
				echo "Extracting and packing debug symbols from $(TGT) <$(BUILD)/$(BUILD.ARCH)>..."; \
				if [ "$(TPUT_WORKS)" = "yes" ]; then tput sgr0; fi; \
			fi; \
			if [ "$($(TGT).TYPE)" = "BINARY" ]; then \
				PKGDBGDIR=$(PKGDBGDIR.base) $(SCRIPTS)/symbol-package.sh $(PACKAGEDIR)/bin/$($(TGT).BIN); \
			elif [ "$($(TGT).TYPE)" = "SHARED" ]; then \
				PKGDBGDIR=$(PKGDBGDIR.base) $(SCRIPTS)/symbol-package.sh $(PACKAGEDIR)/lib/$($(TGT).BIN); \
			fi \
		fi)
else
symbol-package:
endif

endif
