EXENAME=openreader
DEFINES=$(options) HAVE_W32API_H __WXMSW__ _UNICODE WXUSINGDLL NOPCH

ifeq ($(OS),Windows_NT)
EXT_EXE=.exe
EXT_DLL=.dll
else
EXT_EXE=
EXT_DLL=.so
endif

ifeq ($(mode),release)
NAME_SUFFIX=
DEFINES += RELEASE
CXXOPTFLAGS=-s -O3
else
NAME_SUFFIX=d
DEFINES += DEBUG
CXXOPTFLAGS=-g
endif

EXECUTABLE=$(EXENAME)$(NAME_SUFFIX)$(EXT_EXE)
OBJDIR=obj$(NAME_SUFFIX)/

CXX=g++
WINDRES=windres
WINDRESFLAGS=$(addprefix -D,$(DEFINES)) -I"C:\MinGW-W64\mingw32\i686-w64-mingw32\include"
CXXFLAGS=-std=gnu++17 -Wextra $(addprefix -D,$(DEFINES)) -mthreads
LDFLAGS=-lwxbase31u -lwxbase31u_xml -lwxmsw31u_core -lwxmsw31u_webview -L. -lUniversalSpeech -lxqilla -ltidy.dll -mthreads -mwindows

SRCS=$(wildcard *.cpp)
RCSRCS=$(wildcard *.rc)
OBJS=$(addprefix $(OBJDIR),$(SRCS:.cpp=.o))
RCOBJS=$(addprefix $(OBJDIR)rsrc/,$(RCSRCS:.rc=.o))
PERCENT=%

all: $(EXECUTABLE)

.PHONY: $(EXECUTABLE)

clean:
	rm -r $(OBJDIR)

$(EXECUTABLE): $(RCOBJS) $(OBJS)
	@$(CXX) $(CXXFLAGS) $(CXXOPTFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)%.o: %.cpp $(wildcard %.hpp)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(CXXOPTFLAGS) -c -o $@ $<

$(OBJDIR)rsrc/%.o: %.rc
	@mkdir -p $(dir $@)
	@$(WINDRES) $(WINDRESFLAGS) -o $@ $<

