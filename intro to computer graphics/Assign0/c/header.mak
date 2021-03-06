#
# This header.mak file will set up all necessary options for compiling
# and linking C and C++ programs which use OpenGL, GLFW, and/or GLEW on
# the CS Ubuntu systems.
#
# To use it, place it in the same directory as your source code, and
# run the command
#
#	gmakemake > Makefile
#
# To compile and link your program, just run "make".
#

# locations of important directories if the header files
# and library files aren't in the standard places
INCLUDE =
LIBDIRS =

# common linker options
LDLIBS = -lGL -lm -lGLEW -lglfw

# language-specific linker options
CLDLIBS =
CCLDLIBS =

CFLAGS = -std=c99 -g $(INCLUDE) -DGL_GLEXT_PROTOTYPES
CCFLAGS = $(CFLAGS)
CXXFLAGS = -g $(INCLUDE) -DGL_GLEXT_PROTOTYPES

LIBFLAGS = -g $(LIBDIRS) $(LDLIBS)
CLIBFLAGS = $(LIBFLAGS) $(CLDLIBS)
CCLIBFLAGS = $(LIBFLAGS) $(CCLDLIBS)
