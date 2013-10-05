
CC=g++
PWD=`pwd`

# external libraries used for this interpreter
INCDIRS := ./include/ ../../include/
LIBDIRS := ../../build/lib/
LIBS := ww

SOURCES = ./src/main.cpp

ifdef DEBUG
	OPTARG := -g
	TDIR   := debug
else
	OPTARG := -O4
	TDIR   := release
endif

ifdef PROFILE
	PROFARG := -pg
else
	PROFARG :=
endif

OBJECTS := $(addprefix build/obj/$(TDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

CPPFLAGS := -pthread $(INCDIRS:%=-I%) $(OPTARG) $(PROFARG) -m64 -Wall -Wno-deprecated
LIBTEXT  := $(addprefix -L, $(LIBDIRS)) $(addprefix -l, $(LIBS))

build/bin/peval : dirs $(OBJECTS)
	$(CC) $(CPPFLAGS) $(OBJECTS) $(LIBTEXT) -o build/bin/peval

build/obj/$(TDIR)/%.o:%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

build/obj/$(TDIR)/%.o:%.mm
	$(CC) $(CPPFLAGS) -c $< -o $@

build/obj/$(TDIR)/%.o:%.c
	$(CC) $(CPPFLAGS) -c $< -o $@

build/obj/$(TDIR)/%.o:%.m
	$(CC) $(CPPFLAGS) -c $< -o $@

dirs:
	mkdir -p build/obj/$(TDIR)/
	mkdir -p build/bin/
	find . -type d | grep -v "build" | grep -v '\.$$' | awk '{print "build/obj/$(TDIR)/" $$0}' | xargs mkdir -p

clean:
	rm -rf build
