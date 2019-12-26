rm disk
dd if=/dev/zero of=disk bs=1024 count=1M
make all
dd if=image of=disk conv=notrunc
