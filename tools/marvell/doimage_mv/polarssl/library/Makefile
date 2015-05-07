include $(TOPDIR)/config.mk
# Also see "include/polarssl/config.h"

CFLAGS	= -g -I../include -D_FILE_OFFSET_BITS=64
OFLAGS	= -O

# MicroBlaze specific options:
# CFLAGS += -mno-xl-soft-mul -mxl-barrel-shift

# To compile on Plan9:
# CFLAGS += -D_BSD_EXTENSION

# To compile as a shared library:
# CFLAGS += -fPIC

DLEXT=so
# OSX shared library extension:
# DLEXT=dylib

OBJS=	aes.o		arc4.o		base64.o	\
	bignum.o	certs.o		debug.o		\
	des.o		dhm.o		havege.o	\
	md2.o		md4.o		md5.o		\
	net.o		padlock.o	rsa.o		\
	sha1.o		sha2.o		sha4.o		\
	ssl_cli.o	ssl_srv.o	ssl_tls.o	\
	timing.o	x509parse.o	xtea.o		\
	camellia.o  mvMemPool.o

OBJS    := $(addprefix $(obj), $(OBJS))

.SILENT:

all: static

static: $(obj)libpolarssl.a

shared: $(obj)libpolarssl.$(DLEXT)

$(obj)libpolarssl.a: $(OBJS)
	echo "  AR    $@"
	ar r $@ $(OBJS)
	echo "  RL    $@"
	ranlib $@

$(obj)libpolarssl.so: $(obj)libpolarssl.a
	echo "  LD    $@"
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJS)

$(obj)libpolarssl.dylib: $(obj)libpolarssl.a
	echo "  LD    $@"
	$(CC) -dynamiclib -o $@ $(OBJS)

$(obj)%.o: $(obj)%.c
	echo "  CC    $<"
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ -c $<

clean:
	rm -f $(obj)*.o $(obj)libpolarssl.*
