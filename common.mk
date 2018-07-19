include $(top_srcdir)/common-rules.mk
AM_CPPFLAGS+=-DSS_DOCID_BITS=128

collectlib_DIR=$(top_builddir)/Library/Linux
collectbin_DIR=$(top_builddir)/Binary
include $(top_srcdir)/collect.mk

