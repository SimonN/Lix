# Lix Makefile
# See ./doc/linux.txt if you want to compile yourself.

CXX      ?= g++
CXXFLAGS ?= -O2
LD       = libtool --tag=CXX --mode=link $(CXX)
PKG_CONFIG ?= pkg-config

LDALLEG  = $(shell allegro-config --libs)
LDENET   = $(shell $(PKG_CONFIG) --libs libenet 2>/dev/null || echo "-L/usr/local/lib -lenet")
LDPNG    = $(shell $(PKG_CONFIG) --libs libpng zlib 2>/dev/null || echo "-lpng -lz")
CPPFLAGS += $(shell $(PKG_CONFIG) --cflags libpng zlib libenet 2>/dev/null) $(shell allegro-config --cflags)

STRIP    = strip

DEPGEN   = $(CXX) -MM
RM       = rm -rf
MKDIR    = mkdir -p

SRCDIR   = src
OBJDIR   = obj
DEPDIR   = $(OBJDIR)
BINDIR   = bin

# verbosity switch
V=0
ifeq ($(V),0)
Q=@
endif

CLIENT_BIN  = $(BINDIR)/lix
CLIENT_CSRC = $(wildcard src/graphic/png/*.c)
CLIENT_SRCS = $(wildcard src/api/*.cpp) \
              $(wildcard src/api/button/*.cpp) \
              $(wildcard src/editor/*.cpp) \
              $(wildcard src/gameplay/gui/*.cpp) \
              $(wildcard src/gameplay/*.cpp) \
              $(wildcard src/graphic/*.cpp) \
              $(wildcard src/lix/*.cpp) \
              $(wildcard src/level/*.cpp) \
              $(wildcard src/menu/*.cpp) \
              $(wildcard src/network/*.cpp) \
              $(wildcard src/other/*.cpp) \
              $(wildcard src/other/file/*.cpp)
CLIENT_OBJS = $(subst $(SRCDIR)/,$(OBJDIR)/,$(CLIENT_CSRC:%.c=%.o)) \
              $(subst $(SRCDIR)/,$(OBJDIR)/,$(CLIENT_SRCS:%.cpp=%.o))
CLIENT_DEPS = $(subst $(SRCDIR)/,$(DEPDIR)/,$(CLIENT_CSRC:%.c=%.d)) \
              $(subst $(SRCDIR)/,$(DEPDIR)/,$(CLIENT_SRCS:%.cpp=%.d))

SERVER_BIN  = $(BINDIR)/lixd
SERVER_SRCS = $(wildcard src/daemon/*.cpp) \
               src/network/net_t.cpp        src/network/permu.cpp \
               src/network/server.cpp       src/network/server_c.cpp
SERVER_OBJS = $(subst $(SRCDIR)/,$(OBJDIR)/,$(SERVER_SRCS:%.cpp=%.o))
SERVER_DEPS = $(subst $(SRCDIR)/,$(DEPDIR)/,$(SERVER_SRCS:%.cpp=%.d))



###############################################################################

.PHONY: all clean

all: $(CLIENT_BIN) $(SERVER_BIN)

clean:
	$(RM) $(CLIENT_BIN)
	$(RM) $(SERVER_BIN)
	$(RM) $(OBJDIR)
	$(RM) $(DEPDIR)

$(CLIENT_BIN): $(CLIENT_OBJS)
	$(Q)$(MKDIR) $(BINDIR)
	@echo Linking the game \`$(CLIENT_BIN)\' with \
		$(LDALLEG) $(LDENET) $(LDPNG)
	$(Q)$(LD) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDALLEG) $(LDENET) $(LDPNG) $(CLIENT_OBJS) -o $(CLIENT_BIN) \
		> /dev/null
	$(Q)$(STRIP) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_OBJS)
	$(Q)$(MKDIR) $(BINDIR)
	@echo Linking the server daemon \`$(SERVER_BIN)\' with \
		$(LDENET)
	$(Q)$(LD) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDENET) $(SERVER_OBJS) -o $(SERVER_BIN) \
		> /dev/null
	$(Q)$(STRIP) $(SERVER_BIN)

define MAKEFROMSOURCE
$(Q)$(MKDIR) `dirname $@` `dirname $(DEPDIR)/$*.d`
@echo $<
$(Q)$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
@printf "%s/%s" `dirname $@` "`$(DEPGEN) $<`" > $(DEPDIR)/$*.d
endef

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(MAKEFROMSOURCE)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(MAKEFROMSOURCE)

-include $(CLIENT_DEPS)
-include $(SERVER_DEPS)

