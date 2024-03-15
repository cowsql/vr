AC_PREREQ(2.60)
AC_INIT([vr], [0.0.1])
AC_LANG([C])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([ac])

AM_INIT_AUTOMAKE([subdir-objects -Wall -Wno-portability foreign])
AM_SILENT_RULES([yes])
AC_USE_SYSTEM_EXTENSIONS # Defines _GNU_SOURCE and similar

LT_INIT

AC_ARG_ENABLE(backtrace, AS_HELP_STRING([--enable-backtrace[=ARG]], [print backtrace on assertion failure [default=no]]))
AM_CONDITIONAL(BACKTRACE_ENABLED, test "x$enable_backtrace" = "xyes")

# Whether to enable debugging code.
AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug[=ARG]], [enable debugging [default=no]]))
AM_CONDITIONAL(DEBUG_ENABLED, test "x$enable_debug" = "xyes")

# Whether to enable memory sanitizer.
AC_ARG_ENABLE(sanitize, AS_HELP_STRING([--enable-sanitize[=ARG]], [enable code sanitizers [default=no]]))
AM_CONDITIONAL(SANITIZE_ENABLED, test x"$enable_sanitize" = x"yes")

# Enable large file support. This is mandatory in order to interoperate with
# libuv, which enables large file support by default, making the size of 'off_t'
# on 32-bit architecture be 8 bytes instead of the normal 4.
AC_SYS_LARGEFILE

CC_CHECK_FLAGS_APPEND([AM_CFLAGS],[CFLAGS],[ \
  -std=c11 \
  -g \
  -pipe \
])

AS_IF([test "x$enable_hardening" != "xno"],
      [CC_CHECK_FLAGS_APPEND([AM_CFLAGS],[CFLAGS],[ \
        -fcf-protection \
        --param=ssp-buffer-size=4 \
        -fdiagnostics-color \
        -fexceptions \
        -fstack-clash-protection \
        -fstack-protector-strong \
        -fasynchronous-unwind-tables \
        -fdiagnostics-show-option \
        -Wall \
        -Wextra \
        -Wpedantic \
        -Wimplicit-fallthrough=5 \
        -Wcast-align \
        -Wstrict-prototypes \
        -Wlogical-op \
        -Wmissing-include-dirs \
        -Wold-style-definition \
        -Winit-self \
        -Wfloat-equal \
        -Wsuggest-attribute=noreturn \
        -Wformat=2 \
        -Wendif-labels \
        -Wdate-time \
        -Wnested-externs \
        -Wconversion \
        -Werror=implicit-function-declaration \
        -Wunused-but-set-variable \
        -Werror=return-type \
        -Werror=incompatible-pointer-types \
        -Wshadow \
        -Werror=overflow \
        -Werror=shift-count-overflow \
        -Werror=shift-overflow=2 \
        -Warray-bounds \
        -Wrestrict \
        -Wreturn-local-addr \
        -Wstringop-overflow \
      ])],
      [])

AC_SUBST(AM_CFLAGS)

AC_SUBST(AM_LDLAGS)

AC_CONFIG_FILES([vr.pc Makefile])
AC_OUTPUT
