# Lix and lixserv
# Simon and Mindless, 2010-08-17 19:01:16

# BUILD SETTINGS ###########################################

TARGETS = $(TARGET) $(TARGET_SERV)

############################################################

CXX := g++
STRIP := strip
ALLEGRO_CONFIG := allegro-config

TARGET := ../lix
SRCS := $(wildcard api/*.cpp) $(wildcard api/button/*.cpp) $(wildcard editor/*.cpp) $(wildcard gameplay/*.cpp) $(wildcard graphic/*.cpp) $(wildcard lix/*.cpp) $(wildcard level/*.cpp) $(wildcard menu/*.cpp) $(wildcard network/*.cpp) $(wildcard other/*.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

TARGET_SERV := ../lixserv
SRCS_SERV := $(wildcard server/*.cpp) network/net_t.cpp ./network/permu.cpp network/server.cpp network/server_c.cpp other/date.cpp other/io.cpp other/globals.cpp
OBJS_SERV := $(SRCS_SERV:.cpp=.o)
DEPS_SERV := $(SRCS_SERV:.cpp=.d)

# FLAGS ####################################################

ifeq ($(MAKECMDGOALS), release)
    EXTRA_CXXFLAGS += -O2 -DNDEBUG
else
    EXTRA_CXXFLAGS += -g3 -O0 #-Werror
endif
EXTRA_CXXFLAGS += -MMD -pedantic -Wall -Wextra

ALLEGRO_CFLAGS := $(shell $(ALLEGRO_CONFIG) --cppflags)
ALLEGRO_LDLIBS := $(shell $(ALLEGRO_CONFIG) --libs)

ENET_LDLIBS := -lenet

ALL_CXXFLAGS += --std=c++98 -I. $(EXTRA_CXXFLAGS) $(ALLEGRO_CFLAGS) $(CXXFLAGS)
ALL_LDFLAGS += $(LDFLAGS)
LDLIBS += $(ALLEGRO_LDLIBS) $(ENET_LDLIBS)

# RULES ####################################################

.PHONY : all release clean

all : $(TARGETS)

release : all
	$(STRIP) $(TARGETS)

clean :
	rm -rf $(OBJS) $(DEPS)
	rm -rf $(OBJS_SERV) $(DEPS_SERV)
	rm -f $(TARGETS)

ifneq ($(MAKECMDGOALS), clean)
    -include $(DEPS) $(DEPS_SERV)
endif

$(TARGET) : $(OBJS)
	$(CXX) -o $@ $(ALL_LDFLAGS) $^ $(LDLIBS)

$(TARGET_SERV) : $(OBJS_SERV)
	$(CXX) -o $@ $(ALL_LDFLAGS) $^ $(LDLIBS)

%.o : %.cpp
	@mkdir -p "$(dir $@)"
	$(CXX) -c -o $@ $(ALL_CXXFLAGS) $<
