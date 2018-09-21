###########################
# File: 	target-analysis.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_ANALYSIS.MK
TARGET_ANALYSIS.MK := INCLUDED

.SECONDEXPANSION:

.PHONY: analysis $$(addsuffix -analysis, $$(TGTS))
analysis: override BUILD := analysis
analysis: $$(addsuffix -analysis, $$(TGTS))
	$(PRINT)$(MKDIR) $(OUTDIR.analysis)
	$(call YELLOW,"Running cccc analysis into $(OUTDIR.analysis)/cccc.xml...")
	$(PRINT)-find include src -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.c" -o -name "*.cpp" \) | xargs \
		cccc $(CCCC.PARAMS) --outdir=$(OUTDIR.analysis)/.cccc --xml_outfile=$(OUTDIR.analysis)/cccc.xml
	$(call YELLOW,"Running cppcheck analysis into $(OUTDIR.analysis)/cppcheck.xml...")
	$(PRINT)-find include src -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.c" -o -name "*.cpp" \) | xargs \
		cppcheck -I include -I libs/include --enable=all $(CPPCHECK.PARAMS) --verbose --xml-version=2 --xml 2> $(OUTDIR.analysis)/cppcheck.xml
	$(call YELLOW,"Running cpplint analysis into $(OUTDIR.analysis)/cpplint.xml...")
	$(PRINT)-find include src -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.c" -o -name "*.cpp" \) | xargs \
		python $(SCRIPTS)/cpplint.py 2>&1 | python $(SCRIPTS)/cpplint-cppcheck.py &> $(OUTDIR.analysis)/cpplint.xml
	$(call YELLOW,"Running vera++ analysis into $(OUTDIR.analysis)/vera++.xml...")
	$(PRINT)-find include src -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.c" -o -name "*.cpp" \) | xargs \
		vera++ -r $(SCRIPTS)/vera++ -p custom --parameters $(SCRIPTS)/vera++/parameters $(VERAPP.PARAMS) --checkstyle-report=$(OUTDIR.analysis)/vera++.xml

endif
