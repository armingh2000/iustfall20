# Boot loader

The files of this repositry is based on Tuesday's session. 

You can download the slides here.


# Generate ISO file

to generate ISO file please run following command.

``` $ generate_iso.sh name_of_file.asm ```

You can run this ISO file using VMWare or VirtualBox.

# RUN using QEmu

You can also run the bootloader files using QEmu.

```
$ nasm name_of_file.asm -f bin -o output.bin

$ qemu output.bin
```


