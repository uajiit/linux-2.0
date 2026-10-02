#
# QEMU loads vmlinux directly. This target keeps `make boot` from failing.
#

Image: $(TOPDIR)/vmlinux
	$(CROSS_COMPILE)objcopy -O binary $< Image

clean:
	rm -f Image
