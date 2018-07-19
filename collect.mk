#collectlib_DIR=$(top_builddir)/lib
#collectbin_DIR=$(top_builddir)/bin

collectLIBRARIES=$(lib_LIBRARIES) $(pkglib_LIBRARIES)
collectLTLIBRARIES=$(lib_LTLIBRARIES) $(pkglib_LTLIBRARIES)
collectPROGRAMS=$(bin_PROGRAMS) $(sbin_PROGRAM) $(pkglib_PROGRAMS) $(libexec_PROGRAMS)

AM_LDFLAGS+=-L$(collectlib_DIR)

all-local: collectlib-local collectbin-local

collectlib-local: $(addprefix $(collectlib_DIR)/, $(collectLIBRARIES) $(collectLTLIBRARIES))
collectbin-local: $(addprefix $(collectbin_DIR)/, $(collectPROGRAMS))

$(collectlib_DIR)/%.a: %.a
	mkdir -p $(collectlib_DIR)
	ln -sf --target-directory=$(collectlib_DIR)/ $(addprefix $(CURDIR)/, $<)

$(collectlib_DIR)/%.la: %.la
	mkdir -p $(collectlib_DIR)/.libs
	rm -f $(collectlib_DIR)/$(<:%.la=%.so.*)
	@rm -f $(collectlib_DIR)/.libs/$(<:%.la=%.so.*)
if ENABLE_STATIC
	ln -sf $(CURDIR)/.libs/$(<:%.la=%.a) $(collectlib_DIR)/
	@ln -sf $(CURDIR)/.libs/$(<:%.la=%.a) $(collectlib_DIR)/.libs/
endif
	ln -sf $(CURDIR)/.libs/$(<:%.la=%.so*) $(collectlib_DIR)/
	@ln -sf $(CURDIR)/.libs/$(<:%.la=%.so*) $(collectlib_DIR)/.libs/
	ln -sf $(CURDIR)/$< $(collectlib_DIR)/

$(collectbin_DIR)/%: %
	mkdir -p $(collectbin_DIR)
	ln -sf --target-directory=$(collectbin_DIR)/ $(addprefix $(CURDIR)/, $<)

MOSTLYCLEANFILES+=$(foreach t, $(collectLTLIBRARIES), $(collectlib_DIR)/$(t))
MOSTLYCLEANFILES+=$(foreach t, $(collectLTLIBRARIES), $(collectlib_DIR)/$(t:%.la=%.so*))
MOSTLYCLEANFILES+=$(foreach t, $(collectLTLIBRARIES), $(collectlib_DIR)/$(t:%.la=%.a))
MOSTLYCLEANFILES+=$(foreach t, $(collectLTLIBRARIES), $(collectlib_DIR)/.libs/$(t:%.la=%.so*))
MOSTLYCLEANFILES+=$(foreach t, $(collectLTLIBRARIES), $(collectlib_DIR)/.libs/$(t:%.la=%.a))
MOSTLYCLEANFILES+=$(foreach t, $(collectLIBRARIES), $(collectlib_DIR)/$(t))
MOSTLYCLEANFILES+=$(foreach t, $(collectPROGRAMS), $(collectbin_DIR)/$(t))

#UILT_SOURCES+=$(collectlib_DIR) $(collectbin_DIR)

.PHONY: mkdir-collect

all: mkdir-collect

mkdir-collect: $(collectlib_DIR) $(collectbin_DIR)

$(collectlib_DIR) $(collectbin_DIR):
	mkdir -p $@

