# This toplevel Makefile compiles the vendored rpi-rgb-led-matrix library in lib/.
RGB_LIBDIR=./lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

all : $(RGB_LIBRARY)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

clean:
	$(MAKE) -C lib clean

FORCE:
.PHONY: all clean FORCE
