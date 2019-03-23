# Folder where to find other compiled files to include
INCLUDEDIR = ./fusion-c/include/
# Folder where to find Fusion.lib       
LIBDIR = ./fusion-c/lib/

# Name of the target to make
TARGETNAME = roboplay

# Destination of final file
DEST := dsk

# standrd crt0 
# INC1 = $(INCLUDEDIR)crt0_msxdos.rel
# use this crt0 if you want to pass parameters to your program
INC1 = $(INCLUDEDIR)crt0_msxdos_advanced.rel
INC2 = $(LIBDIR)fusion.lib
#INC3 = $(INCLUDEDIR)
#INC4 = $(INCLUDEDIR)
#INC5 = $(INCLUDEDIR)
#INC6 = $(INCLUDEDIR)
#INC7 = $(INCLUDEDIR)
#INC8 = $(INCLUDEDIR)
#INC9 = $(INCLUDEDIR)
#INCA = $(INCLUDEDIR)
#INCB = $(INCLUDEDIR)
#INCC = $(INCLUDEDIR)
#INCD = $(INCLUDEDIR)
#INCE = $(INCLUDEDIR)
#INCF = $(INCLUDEDIR)

# Standard Adress code 
# ADDR_CODE = 0x107
# use this adress code if you are using crt0_msxdos_advanced
ADDR_CODE = 0x180
ADDR_DATA = 0x0

CFLAGS = --disable-warning 196 -mz80 --no-std-crt0 --opt-code-size
LDFLAGS = --code-loc $(ADDR_CODE) --data-loc $(ADDR_DATA) -L $(LIBDIR) $(INC1) $(INC2) $(INC3) $(INC4) $(INC5) $(INC6) $(INC7) $(INC8) $(INC9) $(INCA) $(INCB) $(INCC) $(INCD) $(INCE) $(INCF)

REL = 

all: $(TARGETNAME).com dro2.ply sop.ply clean

$(TARGETNAME).com: $(REL)
	sdcc $(CFLAGS) $(TARGETNAME).c $(REL) $(LDFLAGS)
	hex2bin -e com $(TARGETNAME).ihx
	copy $(TARGETNAME).com $(DEST)\$(TARGETNAME).com /y

%.ply: %.ihx
	hex2bin -e ply $<
	copy $@ $(DEST)\$@ /y

%.ihx: %.c player.rel
	sdcc $(CFLAGS) $< player.rel --code-loc 0x4030 --data-loc 0x0

%.rel: %.c
	sdcc $(CFLAGS) -c $<

%.rel: %.s
	sdasz80 -o $<	

clean:
	@del /Q *.com *.asm *.lst *.sym *.bin *.ihx *.lk *.map *.noi *.rel *.o *.ply
