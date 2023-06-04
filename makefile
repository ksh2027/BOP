CC := g++
CFLAGS := -std=c++11 -O3 -Wall
SOFLAG := -fPIC -shared -m64
PYTHON_INCLUDES := `python3 -m pybind11 --includes` `python3-config --includes --ldflags`

SRCDIR := src/C++
INCDIR := inc
BUILDDIR := build

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

SO = _BOP.so
EXE := BOP

.PHONY: all clean

all: $(SO)
# all: $(EXE)

# $(EXE): $(OBJECTS) $(BUILDDIR)/main.o
# 	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $@

# $(BUILDDIR)/main.o: main.cpp
# 	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
# 	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(SO): $(OBJECTS) $(BUILDDIR)/pybind.o
	$(CC) $(CFLAGS) $(SOFLAG) -I$(INCDIR) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(SOFLAG) -I$(INCDIR) -c $< -o $@

$(BUILDDIR)/pybind.o: src/pybind/pybind.cpp
	$(CC) $(CFLAGS) $(SOFLAG) $(PYTHON_INCLUDES) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)/*.o $(EXE) $(SO) main.o
