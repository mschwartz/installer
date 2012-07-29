ARCH := $(shell getconf LONG_BIT)

CORE=	main.o global.o console.o process.o net.o fs.o v8.o curl.o builtin.o

V8DIR=	./v8-read-only

# V8LIB_64 := $(V8DIR)/out/x64.release/obj.target/tools/gyp
# V8LIB_32 := $(V8DIR)/out/ia32.release/obj.target/tools/gyp
V8LIB_64 := $(V8DIR)/out/x64.release
V8LIB_32 := $(V8DIR)/out/ia32.release
#V8LIB_DIR := $(V8LIB_$(ARCH))
V8LIB_DIR := $(V8LIB_32)

V8VERSION_64 := x64.release
V8VERSION_32 := ia32.release
# V8VERSION := $(V8VERSION_$(ARCH))
V8VERSION := $(V8VERSION_32)

V8=	$(V8LIB_DIR)/libv8_base.a $(V8LIB_DIR)/libv8_snapshot.a
# V8LIBS = -L$(V8LIB_DIR) -lv8_base -lv8_snapshot
V8LIBS = -L$(V8LIB_DIR) -lv8

INCDIRS=    -I$(V8DIR)/include

LIBDIRS=    -L$(V8LIB_DIR)/ 

CFLAGS = -fexceptions -fomit-frame-pointer -fdata-sections -ffunction-sections -fno-strict-aliasing -fvisibility=hidden -Wall -W  -Wno-unused-function -Wno-unused-parameter -Wnon-virtual-dtor -m32 -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -ansi -fno-strict-aliasing

%.o: %.cpp SilkJS.h 
	g++ $(CFLAGS) -c $(INCDIRS) -o $*.o $*.cpp

%.o: %.s
	nasm -f macho $*.s

installer: $(V8DIR) $(V8) $(CORE) SilkJS.h Makefile
	g++ -m32 -o installer $(CORE) $(OBJ) $(V8) -lssl -lpthread -lcurl -Wl,-rpath,/usr/local/silkjs/src/v8,-rpath,$(V8LIB_DIR)

deps: 
	sudo apt-get -y install libmm-dev libmysqlclient-dev libmemcached-dev libgd2-xpm-dev libncurses5-dev libsqlite3-dev libcurl4-openssl-dev libssh2-1-dev libcairo2-dev

$(V8DIR):
	echo "svn checkout"
#	svn checkout http://v8.googlecode.com/svn/branches/bleeding_edge/ v8-read-only
	svn checkout http://v8.googlecode.com/svn/trunk/ v8-read-only

$(V8):
	cd $(V8DIR) && svn update && make dependencies && GYP_GENERATORS=make make ia32.release -j 4 $(V8VERSION)

update:
	cd $(V8DIR) && svn update && make dependencies && GYP_GENERATORS=make make $(V8VERSION)
	git pull
	
clean:
	rm -rf installer *.o

realclean:
	rm -rf silkjs *.o v8-read-only

$(CORE): SilkJS.h Makefile
