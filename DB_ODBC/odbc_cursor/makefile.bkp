# Compile Binary for mysql ODBC connection & build binary

CXX=g++-12.2
BUILD_DIR := .
SRCS := $(shell ls *.cpp)
OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))
BINARY := $(BUILD_DIR)/odbc_cursor_demo
LDFLAGS := -g -O2 -L-L/opt/quest/platform/rh-40-amd64/gcc-12.2/contrib/unixODBC-2.3.11/lib64 
LDLIBS := -lodbc
CPPFLAGS := -c -O2
IDIR := -I/opt/quest/platform/rh-40-amd64/gcc-12.2/contrib/unixODBC-2.3.11/include

$(BINARY): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(BINARY) $(OBJS) $(LDLIBS)

#below will replace for each target .o in $@ - automatic variable 
#with each pre-requisite $<
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(IDIR) -o $@ $<


clean:
	rm -f $(BINARY) $(OBJS)
