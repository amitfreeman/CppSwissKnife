# Compile Binary for mysql ODBC connection & build binary

CXX=g++
BUILD_DIR := ../BinaryArea
SRCS := $(shell ls *.cpp)
OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))
BINARY := $(BUILD_DIR)/mysql_odbc_demo
LDFLAGS := -g -O2 -L/usr/lib/x86_64-linux-gnu/odbc 
LDLIBS := -lodbc
CPPFLAGS := -c -O2

$(BINARY): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(BINARY) $(OBJS) $(LDLIBS)

#below will replace for each target .o in $@ - automatic variable 
#with each pre-requisite $<
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<


clean:
	rm -f $(BINARY) $(OBJS)
