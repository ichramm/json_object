###########################
# File: 	target-example.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_EXAMPLE.MK
TARGET_EXAMPLE.MK := INCLUDED

.SECONDEXPANSION:

.PHONY: example
example: example-$(BUILD)

.PHONY: example-analysis
example-analysis: $$(addprefix ./$(BUILDDIR.base)/, $$(addsuffix -analysis, $$(EXAMPLE.TGTS)))

.PHONY: example-coverage
example-coverage: $$(addprefix ./$(BUILDDIR.base)/, $$(addsuffix -coverage, $$(EXAMPLE.TGTS)))

.PHONY: example-debug
example-debug: $$(addprefix ./$(BUILDDIR.base)/, $$(addsuffix -debug, $$(EXAMPLE.TGTS)))

.PHONY: example-profile
example-profile: $$(addprefix ./$(BUILDDIR.base)/, $$(addsuffix -profile, $$(EXAMPLE.TGTS)))

.PHONY: example-release
example-release: $$(addprefix ./$(BUILDDIR.base)/, $$(addsuffix -release, $$(EXAMPLE.TGTS)))

endif
