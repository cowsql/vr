ACLOCAL_AMFLAGS = -I m4

# Not clobbering the base pointer helps bpftrace construct backtraces
AM_CFLAGS += -fno-omit-frame-pointer

include_HEADERS = include/vr.h

vrincludedir = $(includedir)/vr
vrinclude_HEADERS =

lib_LTLIBRARIES = libvr.la
libvr_la_CFLAGS = $(AM_CFLAGS) $(CODE_COVERAGE_CFLAGS) -fvisibility=hidden
libvr_la_LDFLAGS = -version-info 0:0:0
libvr_la_SOURCES = \
  src/vr.c

bin_PROGRAMS =

if BACKTRACE_ENABLED
libvr_la_CFLAGS += -DVR_ASSERT_WITH_BACKTRACE
libvr_la_LDFLAGS += -lbacktrace
endif # BACKTRACE_ENABLED

if DEBUG_ENABLED
  AM_CFLAGS += -Werror -Wall
else
  AM_CFLAGS += -DNDEBUG
endif

if SANITIZE_ENABLED
  AM_CFLAGS += -fsanitize=address
endif

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = @PACKAGE_NAME@.pc
