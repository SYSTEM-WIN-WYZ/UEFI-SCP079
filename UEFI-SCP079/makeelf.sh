echo 编译main.c

aarch64-linux-gnu-gcc -c main.c -o main.o     -I. -I/usr/include/efi     -std=c17 -Wall -Wextra -Wpedantic     -ffreestanding -fshort-wchar -nostdlib     -fno-stack-protector -fno-asynchronous-unwind-tables

echo 成功
echo 链接并构建bootaa64.efl

aarch64-linux-gnu-ld -e efi_main -o BOOTAA64.elf main.o

echo 成功
echo 移位字节

aarch64-linux-gnu-objcopy     --set-section-alignment .text=4096     --set-section-alignment .rodata=4096     --set-section-alignment .data=4096     --set-section-alignment .bss=4096     BOOTAA64.elf BOOTAA64-aligned.elf

echo 成功
echo 转换elf为efi

./elf2efi64 BOOTAA64-aligned.elf bootaa64.efi

echo 成功
echo return 0
