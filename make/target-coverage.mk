###########################
# File: 	target-coverage.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_COVERAGE.MK
TARGET_COVERAGE.MK := INCLUDED

.SECONDEXPANSION:

.PHONY: coverage $$(addsuffix -coverage, $$(TGTS))
coverage: override BUILD := coverage
coverage: $$(addsuffix -coverage, $$(TGTS))
	$(PRINT)if [ -d "$(OBJDIR.coverage)" ]; then \
		lcov --zerocounters --quiet --directory $(OBJDIR.coverage) --base-directory $(ROOT); \
		lcov --capture -initial --directory $(OBJDIR.coverage) --base-directory $(ROOT) --output-file $(OBJDIR.coverage)/base.lcov; \
		TRACES=""; \
		for i in `find $(OBJDIR.coverage) -type f -name "*-base.lcov" -print | sort` ; do \
			test -s $$i && TRACES="$$TRACES --add-tracefile $$i"; \
		done; \
		if [ -n "$$TRACES" ]; then \
			lcov $$TRACES -output-file $(OBJDIR.coverage)/base.lcov; \
		fi \
	fi

.PHONY: coverage-report $$(addsuffix -coverage-report, $$(TGTS))
coverage: override BUILD := coverage
coverage-report: $$(addsuffix -coverage-report, $$(TGTS))
	$(PRINT)if [ -d "$(OBJDIR.coverage)" ]; then \
		$(RM) $(OUTDIR.coverage); \
		$(MKDIR) $(OUTDIR.coverage); \
		lcov --capture --directory $(OBJDIR.coverage) --base-directory $(ROOT) --output-file $(OBJDIR.coverage)/run.lcov || true; \
		TRACES=""; \
		for i in `find $(OBJDIR.coverage) -type f -name "*-run.lcov" -print | sort` ; do \
		    test -s $$i && TRACES="$$TRACES --add-tracefile $$i"; \
		done; \
		if [ -n "$$TRACES" ]; then \
			lcov $$TRACES -output-file $(OBJDIR.coverage)/run.lcov; \
			lcov --add-tracefile $(OBJDIR.coverage)/base.lcov --add-tracefile $(OBJDIR.coverage)/run.lcov --output-file $(OBJDIR.coverage)/data.lcov && \
			lcov --remove $(OBJDIR.coverage)/data.lcov "*include*" -o $(OBJDIR.coverage)/data.lcov && \
			lcov --remove $(OBJDIR.coverage)/data.lcov "*boost*" -o $(OBJDIR.coverage)/data.lcov && \
			lcov --remove $(OBJDIR.coverage)/data.lcov "*ACE_wrappers*" -o $(OBJDIR.coverage)/data.lcov && \
			python /$(SCRIPTS)/lcov-cobertura.py $(OBJDIR.coverage)/data.lcov --base-dir $(ROOT) --output $(OUTDIR.coverage)/lcov.xml && \
			genhtml --highlight --legend --output-directory $(OUTDIR.coverage) $(OBJDIR.coverage)/data.lcov; \
			$(RM) $(OBJDIR.coverage)/base.lcov $(OBJDIR.coverage)/run.lcov $(OBJDIR.coverage)/data.lcov; \
		fi \
	fi

endif
