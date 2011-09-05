# Lix makefile copied from towerhack makefile

CXX      := g++
CXXFLAGS := -s -O2

LD       := g++
LDDIRS   := -L/usr/local/lib
LDALLEG  := $(shell allegro-config --libs)
LDENET   := -lenet

DEPGEN   := g++ -MM
RM       := rm -rf
MKDIR    := mkdir -p

SRCDIR   := src
OBJDIR   := obj
DEPDIR   := $(OBJDIR)
BINDIR   := bin

CLIENT_BIN  := $(BINDIR)/lix
CLIENT_SRCS := $(wildcard src/api/*.cpp) \
               $(wildcard src/api/button/*.cpp) \
               $(wildcard src/editor/*.cpp) \
               $(wildcard src/gameplay/gui/*.cpp) \
               $(wildcard src/gameplay/*.cpp) \
               $(wildcard src/graphic/*.cpp) $(wildcard src/lix/*.cpp) \
               $(wildcard src/level/*.cpp)   $(wildcard src/menu/*.cpp) \
               $(wildcard src/network/*.cpp) $(wildcard src/other/*.cpp)
CLIENT_OBJS := $(subst $(SRCDIR)/,$(OBJDIR)/,$(CLIENT_SRCS:%.cpp=%.o))
CLIENT_DEPS := $(subst $(SRCDIR)/,$(DEPDIR)/,$(CLIENT_SRCS:%.cpp=%.d))

SERVER_BIN  := $(BINDIR)/lixd
SERVER_SRCS := $(wildcard src/daemon/*.cpp) \
                src/network/net_t.cpp        src/network/permu.cpp \
                src/network/server.cpp       src/network/server_c.cpp
SERVER_OBJS := $(subst $(SRCDIR)/,$(OBJDIR)/,$(SERVER_SRCS:%.cpp=%.o))
SERVER_DEPS := $(subst $(SRCDIR)/,$(DEPDIR)/,$(SERVER_SRCS:%.cpp=%.d))



###############################################################################

.PHONY: all clean

all: $(CLIENT_BIN) $(SERVER_BIN)

clean:
	$(RM) $(CLIENT_BIN)
	$(RM) $(SERVER_BIN)
	$(RM) $(OBJDIR)
	$(RM) $(DEPDIR)

$(CLIENT_BIN): $(CLIENT_OBJS)
	@$(MKDIR) $(BINDIR)
	@echo Linking the game binary \`$(CLIENT_BIN)\'.
	@echo Linker flags: $(LDDIRS) $(LDALLEG) $(LDENET)
	@$(LD) $(LDDIRS) $(LDALLEG) $(LDENET) $(CLIENT_OBJS) -o $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_OBJS)
	@$(MKDIR) $(BINDIR)
	@echo Linking the server daemon \`$(SERVER_BIN)\'.
	@echo Linker flags: $(LDDIRS) $(LDENET)
	@$(LD) $(LDDIRS) $(LDENET) $(SERVER_OBJS) -o $(SERVER_BIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@$(MKDIR) `dirname $@` `dirname $(DEPDIR)/$*.d`
	@echo $<
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@printf "%s/%s" `dirname $@` "`$(DEPGEN) $<`" > $(DEPDIR)/$*.d

-include $(CLIENT_DEPS)
-include $(SERVER_DEPS)

