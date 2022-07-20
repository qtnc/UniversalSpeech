LIBNAME=UniversalSpeech
DEFINES=$(options)

ifeq ($(OS),Windows_NT)
EXT_EXE=.exe
EXT_LIB=.dll
LDFLAGS=-Wl,--add-stdcall-alias -Wl,--enable-stdcall-fixup -lole32 -loleaut32 -luuid -lpsapi -lversion
DLLDEFS=src/windows/main.def
SRCS=$(wildcard src/windows/*.c)
else
EXT_EXE=
EXT_LIB=.so
LDFLAGS=
DLLDEFS=
SRCS=
endif

ifeq ($(mode),release)
NAME_SUFFIX=
DEFINES += RELEASE
CCOPTFLAGS=-s -O3
else
NAME_SUFFIX=d
DEFINES += DEBUG
CCOPTFLAGS=-g
endif

LIBRARY=$(LIBNAME)$(NAME_SUFFIX)$(EXT_LIB)
OBJDIR=obj$(NAME_SUFFIX)/

CC=gcc
CCFLAGS=-std=gnu99 -Wextra $(addprefix -D,$(DEFINES)) -mthreads
LDFLAGSB=-shared -static-libgcc -Wl,--out-implib,lib$(LIBNAME)$(NAME_SUFFIX).a $(LDFLAGS)

SRCS+=$(wildcard src/*.c) $(wildcard src/java/*.c)
OBJS=$(addprefix $(OBJDIR),$(SRCS:.c=.o))
PERCENT=%

all: $(LIBRARY)

.PHONY: $(LIBRARY)

clean:
	rm -r $(OBJDIR)

$(LIBRARY): $(OBJS) $(DLLDEFS)
	@$(CC) $(CCFLAGS) $(CCOPTFLAGS) -o $@ $^ $(LDFLAGSB)

$(OBJDIR)%.o: %.c $(wildcard %.h) $(wildcard include/*.h)
	@mkdir.exe -p $(dir $@)
	@$(CC) $(CCFLAGS) $(CCOPTFLAGS) -c -o $@ $<

