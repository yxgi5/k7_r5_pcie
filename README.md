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
