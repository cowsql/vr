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
  src/heap.c \
  src/vr.c

bin_PROGRAMS =

check_PROGRAMS = \
  test/integration/core
TESTS = $(check_PROGRAMS)

check_LTLIBRARIES = libtest.la

libtest_la_CFLAGS = $(AM_CFLAGS) -DMUNIT_TEST_NAME_LEN=60 -Wno-unused-result -Wno-conversion
libtest_la_SOURCES = \
  test/lib/munit.c

test_integration_core_SOURCES = \
  test/integration/main_core.c \
  test/integration/test_step.c
test_integration_core_CFLAGS = $(AM_CFLAGS) -Wno-conversion
test_integration_core_LDFLAGS = -no-install
test_integration_core_LDADD = libtest.la libvr.la

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
