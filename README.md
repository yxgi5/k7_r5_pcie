# block design

[doc/system.pdf](doc/system.pdf)

# MicroBlaze 最小系统

# Jtag模式

可以选上 program fpga

也可以不选，先下载bitstream，再debug


# xsct cmds

```
connect
targets
targets -set 3
mrd 0x80000000
mrd -size h 0x80000000 10
mrd -size w 0x80000000 10
disconnect
```

```
mrd 0x44A30030

mwr 0x44A30030 0x8A


mwr 0x44A30030 0x04
mrd 0x44A30030

mwr 0x44A30030 0x0101108B
mwr 0x44A300AC 0xA0000000
mwr 0x44A300A8 0x1680
mwr 0x44A300A4 0x1680
mwr 0x44A300A0 0x438

mrd 0x44A30030
mrd 0x44A300AC
mrd 0x44A300A8
mrd 0x44A300A4
mrd 0x44A300A0
mrd 0x44A30034

xsct% 44A30030:   0101100B
xsct% 44A300AC:   A0000000
xsct% 44A300A8:   00001680
xsct% 44A300A4:   00001680
xsct% 44A300A0:   00000438
xsct% 44A30034:   00015810

mwr 0x44A30030 0x0101108B
mwr 0x44A300AC 0x90000000
mwr 0x44A300A8 0x1680
mwr 0x44A300A4 0x1680
mwr 0x44A300A0 0x438


mwr 0x44A30034 0xffffffff
mrd 0x44A30034
```
