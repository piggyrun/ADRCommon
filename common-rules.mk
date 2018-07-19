INCLUDES = -I$(top_srcdir) -I$(top_builddir)
MOSTLYCLEANFILES=
BUILT_SOURCES=

CFLAGS+=$(if $(DEBUG), -g -O0)
CXXFLAGS+=$(if $(DEBUG), -g -O0)

CPPFLAGS+=$(if $(OPTIMIZE), -DNDEBUG)
CFLAGS+=$(if $(OPTIMIZE), -O3)
CXXFLAGS+=$(if $(OPTIMIZE), -O3)

CFLAGS+=$(if $(PROFILE), -pg)
CXXFLAGS+=$(if $(PROFILE), -pg)

AM_CPPFLAGS=-D'SVNVERSION="$(SVNVERSION)"'
AM_LDFLAGS=-Wl,-no-undefined
AM_CFLAGS=
AM_CXXFLAGS=
AM_MAKEFLAGS=-j4

DOLOCALE=LC_ALL= LC_CTYPE=zh_CN.gbk LC_MESSAGES=en_US

_SVNVERSION=$(strip $(subst exported, 0, $(shell \
	if $(DOLOCALE) svnversion -nc $(srcdir) | fgrep 'M' &> /dev/null; then \
		$(DOLOCALE) svnversion -n $(srcdir) | sed 's/^.*://' | tr -cd [0-9]; \
	else \
		$(DOLOCALE) svnversion -nc $(srcdir)| sed 's/^.*://' | tr -cd [0-9]; \
	fi \
	)))
_SAVED_SVNVERSION=$(strip $(subst exported, 0, $(shell cat $(srcdir)/SVNVERSION | sed 's/^.*://' | tr -cd [0-9] )))
SVNVERSION=$(if $(_SVNVERSION),$(_SVNVERSION),$(_SAVED_SVNVERSION))
CONFIGURE_DEPENDENCIES=$(subst @@@,,$(subst CONFIGURE_DEPENDENCIES,@,@CONFIGURE_DEPENDENCIES@))
CONFIGURE_DEPENDENCIES+=$(top_srcdir)/acsite.m4


EXTRA_DIST=SVNVERSION SVNINFO

SVNVERSION SVNINFO: dist-svn

dist-svn:
	if test -d $(srcdir)/.svn; then \
		make dist-svn-doit; \
	fi;

dist-svn-doit:
	if $(DOLOCALE) svnversion -nc $(srcdir) | fgrep 'M' &> /dev/null; then \
		$(DOLOCALE) svnversion -nc $(srcdir) > SVNVERSION; \
		echo -n ':' >> SVNVERSION; \
		$(DOLOCALE) svnversion -n $(srcdir) | sed 's/^.*://' | tr -cd [0-9] >> SVNVERSION; \
	else \
		$(DOLOCALE) svnversion -nc $(srcdir) > SVNVERSION; \
	fi; \
	$(DOLOCALE) svn info -R > SVNINFO;

dist: dist-svn

clean-local:
	if [ -d $(srcdir)/.svn ]; then rm -f SVNVERSION SVNINFO; fi

.PHONY: dist-svn dist-svn-doit

