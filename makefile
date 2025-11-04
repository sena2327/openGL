# Compiler
CXX ?= g++
CXXFLAGS ?= -O2 -Wall

# Detect OpenCV (prefer opencv4, fallback to opencv)
OPENCV_PKG := $(shell pkg-config --exists opencv4 && echo opencv4 || (pkg-config --exists opencv && echo opencv))
ifeq ($(OPENCV_PKG),)
$(error OpenCV not found. Install libopencv-dev and ensure pkg-config files are present)
endif

OPENCV_CFLAGS := $(shell pkg-config --cflags $(OPENCV_PKG))
OPENCV_LIBS   := $(shell pkg-config --libs   $(OPENCV_PKG))

# OpenGL / GLUT libs + OpenCV
LDLIBS  := -lglut -lGLU -lGL $(OPENCV_LIBS)
CXXFLAGS += $(OPENCV_CFLAGS)

# Targets
.PHONY: all clean
all: test5

test5: test5.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS)

clean:
	rm -f test5
