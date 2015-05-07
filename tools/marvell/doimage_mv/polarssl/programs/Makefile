include $(TOPDIR)/config.mk
# To compile on SunOS: add "-lsocket -lnsl" to LDFLAGS
# To compile on MinGW: add "-lws2_32" to LDFLAGS

CFLAGS	= -g -I../include -D_FILE_OFFSET_BITS=64
OFLAGS	= -O
LDFLAGS	= -L$(obj)../library -lpolarssl

APPS =	aes/aescrypt2		hash/hello		\
	hash/md5sum		hash/sha1sum		\
	hash/sha2sum		pkey/dh_client		\
	pkey/dh_genprime	pkey/dh_server		\
	pkey/mpi_demo		pkey/rsa_genkey		\
	pkey/rsa_sign		pkey/rsa_verify		\
	ssl/ssl_client1		ssl/ssl_client2		\
	ssl/ssl_server		test/benchmark		\
	test/selftest		test/ssl_test
APPS := $(addprefix $(obj),$(APPS))

ifneq ($(obj),)
$(shell mkdir -p $(obj)aes)
$(shell mkdir -p $(obj)hash)
$(shell mkdir -p $(obj)pkey)
$(shell mkdir -p $(obj)ssl)
$(shell mkdir -p $(obj)test)
endif

.SILENT:

all: $(APPS)

$(obj)aes/aescrypt2: aes/aescrypt2.c $(obj)../library/libpolarssl.a
	echo   "  CC    aes/aescrypt2.c"
	$(CC) $(CFLAGS) $(OFLAGS) aes/aescrypt2.c    $(LDFLAGS) -o $@

$(obj)hash/hello: hash/hello.c $(obj)../library/libpolarssl.a
	echo   "  CC    hash/hello.c"
	$(CC) $(CFLAGS) $(OFLAGS) hash/hello.c       $(LDFLAGS) -o $@

$(obj)hash/md5sum: hash/md5sum.c $(obj)../library/libpolarssl.a
	echo   "  CC    hash/md5sum.c"
	$(CC) $(CFLAGS) $(OFLAGS) hash/md5sum.c      $(LDFLAGS) -o $@

$(obj)hash/sha1sum: hash/sha1sum.c $(obj)../library/libpolarssl.a
	echo   "  CC    hash/sha1sum.c"
	$(CC) $(CFLAGS) $(OFLAGS) hash/sha1sum.c     $(LDFLAGS) -o $@

$(obj)hash/sha2sum: hash/sha2sum.c $(obj)../library/libpolarssl.a
	echo   "  CC    hash/sha2sum.c"
	$(CC) $(CFLAGS) $(OFLAGS) hash/sha2sum.c     $(LDFLAGS) -o $@

$(obj)pkey/dh_client: pkey/dh_client.c $(obj)../library/libpolarssl.a
	echo   "  CC    pkey/dh_client.c"
	$(CC) $(CFLAGS) $(OFLAGS) pkey/dh_client.c   $(LDFLAGS) -o $@

$(obj)pkey/dh_genprime: pkey/dh_genprime.c $(obj)../library/libpolarssl.a
	echo   "  CC    pkey/dh_genprime.c"
	$(CC) $(CFLAGS) $(OFLAGS) pkey/dh_genprime.c $(LDFLAGS) -o $@

$(obj)pkey/dh_server: pkey/dh_server.c $(obj)../library/libpolarssl.a
	echo   "  CC    pkey/dh_server.c"
	$(CC) $(CFLAGS) $(OFLAGS) pkey/dh_server.c   $(LDFLAGS) -o $@

$(obj)pkey/mpi_demo: pkey/mpi_demo.c $(obj)../library/libpolarssl.a
	echo   "  CC    pkey/mpi_demo.c"
	$(CC) $(CFLAGS) $(OFLAGS) pkey/mpi_demo.c    $(LDFLAGS) -o $@

$(obj)pkey/rsa_genkey: pkey/rsa_genkey.c $(obj)../library/libpolarssl.a
	echo   "  CC    pkey/rsa_genkey.c"
	$(CC) $(CFLAGS) $(OFLAGS) pkey/rsa_genkey.c  $(LDFLAGS) -o $@

$(obj)pkey/rsa_sign: pkey/rsa_sign.c $(obj)../library/libpolarssl.a
	echo   "  CC    pkey/rsa_sign.c"
	$(CC) $(CFLAGS) $(OFLAGS) pkey/rsa_sign.c    $(LDFLAGS) -o $@

$(obj)pkey/rsa_verify: pkey/rsa_verify.c $(obj)../library/libpolarssl.a
	echo   "  CC    pkey/rsa_verify.c"
	$(CC) $(CFLAGS) $(OFLAGS) pkey/rsa_verify.c  $(LDFLAGS) -o $@

$(obj)ssl/ssl_client1: ssl/ssl_client1.c $(obj)../library/libpolarssl.a
	echo   "  CC    ssl/ssl_client1.c"
	$(CC) $(CFLAGS) $(OFLAGS) ssl/ssl_client1.c  $(LDFLAGS) -o $@

$(obj)ssl/ssl_client2: ssl/ssl_client2.c $(obj)../library/libpolarssl.a
	echo   "  CC    ssl/ssl_client2.c"
	$(CC) $(CFLAGS) $(OFLAGS) ssl/ssl_client2.c  $(LDFLAGS) -o $@

$(obj)ssl/ssl_server: ssl/ssl_server.c $(obj)../library/libpolarssl.a
	echo   "  CC    ssl/ssl_server.c"
	$(CC) $(CFLAGS) $(OFLAGS) ssl/ssl_server.c   $(LDFLAGS) -o $@

$(obj)test/benchmark: test/benchmark.c $(obj)../library/libpolarssl.a
	echo   "  CC    test/benchmark.c"
	$(CC) $(CFLAGS) $(OFLAGS) test/benchmark.c   $(LDFLAGS) -o $@

$(obj)test/selftest: test/selftest.c $(obj)../library/libpolarssl.a
	echo   "  CC    test/selftest.c"
	$(CC) $(CFLAGS) $(OFLAGS) test/selftest.c    $(LDFLAGS) -o $@

$(obj)test/ssl_test: test/ssl_test.c $(obj)../library/libpolarssl.a
	echo   "  CC    test/ssl_test.c"
	$(CC) $(CFLAGS) $(OFLAGS) test/ssl_test.c    $(LDFLAGS) -o $@

clean:
	rm -f $(APPS)
