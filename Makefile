CC=gcc
CFLAGS=-I. -I./include -I./LibUfdt/include -I./LibUfdt/sysdeps -I./LibUfdt/sysdeps/include -I./FdtLib/ -g -O2 -fshort-wchar -fno-strict-aliasing -Wall -Wno-error -Wno-array-bounds -DMAX_UINT32="((unsigned int)0xFFFFFFFF)" -DUINT32="unsigned int" -Dbool="int" -DDTO_ENABLE_DEBUG
DEPS = libufdt.h  ufdt_overlay.h  ufdt_types.h

objects = ufdt_overlay_test_app.o  ./include/util.o LibUfdt/ufdt_convert.o  LibUfdt/ufdt_node.o  LibUfdt/ufdt_node_dict.o  LibUfdt/ufdt_overlay.o  LibUfdt/sysdeps/libufdt_sysdeps_posix.o FdtLib/fdt.o  FdtLib/fdt_empty_tree.o  FdtLib/fdt_ro.o  FdtLib/fdt_rw.o  FdtLib/fdt_strerror.o  FdtLib/fdt_sw.o  FdtLib/fdt_wip.o

uefi_dtc_overlay : $(objects)
	$(CC)  -o $@ $(objects)



%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

LibUfdt/sysdeps/libufdt_sysdeps_posix.o:LibUfdt/sysdeps/libufdt_sysdeps_posix.c
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY : clean
clean :
	rm  uefi_dtc_overlay $(objects)
