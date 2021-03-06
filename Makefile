OBJS_BOOTPACK = bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj \
		int.obj fifo.obj keyboard.obj mouse.obj memory.obj sheet.obj timer.obj \
		mtask.obj window.obj console.obj file.obj core_task.obj \
		shtPutStr.obj semaphore.obj

TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del

# �f�t�H���g����

default :
	$(MAKE) img

# �t�@�C�������K��

ipl10.bin : ipl10.nas Makefile
	$(NASK) ipl10.nas ipl10.bin ipl10.lst

asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

hankaku.bin : hankaku.txt Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin Makefile
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

bootpack.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		$(OBJS_BOOTPACK)
# 3MB+64KB=3136KB

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

haribote.sys : asmhead.bin bootpack.hrb Makefile
	copy /B asmhead.bin+bootpack.hrb haribote.sys

produce0.bim : produce0.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:produce0.bim stack:1k map:produce0.map \
		produce0.obj a_nask.obj

produce0.hrb : produce0.bim Makefile
	$(BIM2HRB) produce0.bim produce0.hrb 0
	
consume0.bim : consume0.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:consume0.bim stack:1k map:consume0.map \
		consume0.obj a_nask.obj

consume0.hrb : consume0.bim Makefile
	$(BIM2HRB) consume0.bim consume0.hrb 0

produce.bim : produce.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:produce.bim stack:1k map:produce.map \
		produce.obj a_nask.obj

produce.hrb : produce.bim Makefile
	$(BIM2HRB) produce.bim produce.hrb 0
	
consume.bim : consume.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:consume.bim stack:1k map:consume.map \
		consume.obj a_nask.obj

consume.hrb : consume.bim Makefile
	$(BIM2HRB) consume.bim consume.hrb 0
	
pacsta.bim : pacsta.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:pacsta.bim stack:1k map:pacsta.map \
		pacsta.obj a_nask.obj

pacsta.hrb : pacsta.bim Makefile
	$(BIM2HRB) pacsta.bim pacsta.hrb 0

#user_produce.bim : user_produce.obj a_nask.obj Makefile
#	$(OBJ2BIM) @$(RULEFILE) out:user_produce.bim stack:1k map:user_produce.map \
		user_produce.obj a_nask.obj

#user_consume.bim : user_consume.obj a_nask.obj Makefile
#	$(OBJ2BIM) @$(RULEFILE) out:user_consume.bim stack:1k map:user_consume.map \
		user_consume.obj a_nask.obj

#user_pacsta.bim : user_pacsta.obj a_nask.obj Makefile
#	$(OBJ2BIM) @$(RULEFILE) out:user_pacsta.bim stack:1k map:user_pacsta.map \
		user_pacsta.obj a_nask.obj

#user_end.bim : user_end.obj a_nask.obj Makefile
#	$(OBJ2BIM) @$(RULEFILE) out:user_end.bim stack:1k map:user_end.map \
		user_end.obj a_nask.obj

mainpro0.bim : mainpro0.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:mainpro0.bim stack:1k map:mainpro0.map \
		mainpro0.obj a_nask.obj

mainpro0.hrb : mainpro0.bim Makefile
	$(BIM2HRB) mainpro0.bim mainpro0.hrb 0
	
maincon0.bim : maincon0.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:maincon0.bim stack:1k map:maincon0.map \
		maincon0.obj a_nask.obj

maincon0.hrb : maincon0.bim Makefile
	$(BIM2HRB) maincon0.bim maincon0.hrb 0

mainpro.bim : mainpro.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:mainpro.bim stack:1k map:mainpro.map \
		mainpro.obj a_nask.obj

mainpro.hrb : mainpro.bim Makefile
	$(BIM2HRB) mainpro.bim mainpro.hrb 0
	
maincon.bim : maincon.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:maincon.bim stack:1k map:maincon.map \
		maincon.obj a_nask.obj

maincon.hrb : maincon.bim Makefile
	$(BIM2HRB) maincon.bim maincon.hrb 0
	
mainpac.bim : mainpac.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:mainpac.bim stack:1k map:mainpac.map \
		mainpac.obj a_nask.obj

mainpac.hrb : mainpac.bim Makefile
	$(BIM2HRB) mainpac.bim mainpac.hrb 0

haribote.img : ipl10.bin haribote.sys Makefile \
		produce.hrb consume.hrb produce0.hrb consume0.hrb pacsta.hrb \
		mainpac.hrb mainpro0.hrb maincon0.hrb mainpro.hrb maincon.hrb
		
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:haribote.sys to:@: \
		copy from:ipl10.nas to:@: \
		copy from:make.bat to:@: \
		copy from:produce0.hrb to:@: \
		copy from:consume0.hrb to:@: \
		copy from:produce.hrb to:@: \
		copy from:consume.hrb to:@: \
		copy from:pacsta.hrb to:@: \
		copy from:mainpac.hrb to:@: \
		copy from:mainpro.hrb to:@: \
		copy from:maincon.hrb to:@: \
		copy from:mainpro0.hrb to:@: \
		copy from:maincon0.hrb to:@: \
		imgout:haribote.img

# ��ʋK��

%.gas : %.c bootpack.h Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

# �R�}���h

img :
	$(MAKE) haribote.img

run :
	$(MAKE) img
	$(COPY) haribote.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) img
	$(IMGTOL) w a: haribote.img

clean :
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.obj
	-$(DEL) *.map
	-$(DEL) *.bim
	-$(DEL) *.hrb
	-$(DEL) haribote.sys

src_only :
	$(MAKE) clean
	-$(DEL) haribote.img
