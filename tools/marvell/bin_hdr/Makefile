#;/*******************************************************************************
#;Copyright (C) Marvell International Ltd. and its affiliates
#;
#;This software file (the "File") is owned and distributed by Marvell
#;International Ltd. and/or its affiliates ("Marvell") under the following
#;alternative licensing terms.  Once you have made an election to distribute the
#;File under one of the following license alternatives, please (i) delete this
#;introductory statement regarding license alternatives, (ii) delete the two
#;license alternatives that you have not elected to use and (iii) preserve the
#;Marvell copyright notice above.
#;
#;********************************************************************************
#;Marvell Commercial License Option
#;
#;If you received this File from Marvell and you have entered into a commercial
#;license agreement (a "Commercial License") with Marvell, the File is licensed
#;to you under the terms of the applicable Commercial License.
#;
#;********************************************************************************
#;Marvell GPL License Option
#;
#;If you received this File from Marvell, you may opt to use, redistribute and/or
#;modify this File in accordance with the terms and conditions of the General
#;Public License Version 2, June 1991 (the "GPL License"), a copy of which is
#;available along with the File in the license.txt file or by writing to the Free
#;Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or
#;on the worldwide web at http://www.gnu.org/licenses/gpl.txt.
#;
#;THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED
#;WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY
#;DISCLAIMED.  The GPL License provides additional details about this warranty
#;disclaimer.
#;********************************************************************************
#;Marvell BSD License Option
#;
#;If you received this File from Marvell, you may opt to use, redistribute and/or
#;modify this File under the following licensing terms.
#;Redistribution and use in source and binary forms, with or without modification,
#;are permitted provided that the following conditions are met:
#;
#;   *   Redistributions of source code must retain the above copyright notice,
#;	    this list of conditions and the following disclaimer.
#;
#;    *   Redistributions in binary form must reproduce the above copyright
#;        notice, this list of conditions and the following disclaimer in the
#;        documentation and/or other materials provided with the distribution.
#;
#;    ;*   Neither the name of Marvell nor the names of its contributors may be
#;        used to endorse or promote products derived from this software without
#;        specific prior written permission.
#;
#;THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#;ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#;WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#;DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
#;ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#;(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#;LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
#;ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#;(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#;SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#;
#;*******************************************************************************/
#.SECONDARY:
include $(TOPDIR)/config.mk
include ./base.mk

SETPARAMS   = setparams
PARAMSFILE  = params
LDSCRIPT    = linker.scr
LDSECSCRIPT = linker_sec.scr
DRAMREGS    = ./dramregs.txt
MAIN = main

ifneq ($(obj),)
$(shell mkdir -p $(obj)$(MAIN))
endif

LDFLAGSBIN    = -T $(LDSCRIPT) $(LDFLAGS)
LDSECFLAGSBIN = -T $(LDSECSCRIPT) $(LDFLAGS)

HOSTCPPFLAGS = $(CPPFLAGS)

TGT = bin_hdr

ASRC = $(MAIN)/bin_entry.s
CSRC = $(MAIN)/mvdispacher.c

AOBJ = $(subst .s,.o,$(ASRC))
AOBJ := $(addprefix $(obj), $(AOBJ))
COBJ = $(subst .c,.o,$(CSRC))
COBJ := $(addprefix $(obj), $(COBJ))
CUART_OBJ = $(subst .c,.uart.o,$(CSRC))
CUART_OBJ := $(addprefix $(obj), $(CUART_OBJ))

LIB_PHY = $(obj)phy_$(BOARD).a
LIB_DDR = $(obj)ddr_$(BOARD).a
LIB_PLAT = $(obj)plat.a
LIB_SWUP = $(obj)suspendWUP.a
LIB_GENERAL_INIT = $(obj)generalInit.a

LIB_PHY_UART = $(obj)phy_$(BOARD).uart.a
LIB_DDR_UART = $(obj)ddr_$(BOARD).uart.a
LIB_PLAT_UART = $(obj)plat.uart.a
LIB_SWUP_UART = $(obj)suspendWUP.uart.a
LIB_GENERAL_INIT_UART = $(obj)generalInit.uart.a

LIBGCC = $(MAIN)/libgcc.lib

ifeq ($(DDRTYPE),)
DDRTYPE = ddr3
endif

TLIB = ./src_ddr/lib/$(DDRTYPE)_training_$(LIBNAME).lib

COMPONENT_SUBDIRS = platform src_phy src_ddr src_pm src_init
HDR_COMPONENTS = $(LIB_PLAT) $(LIB_GENERAL_INIT) $(LIB_PHY) $(LIB_DDR) $(TLIB) $(LIB_SWUP)
HDR_COMPONENTS_UART = $(LIB_PLAT_UART) $(LIB_GENERAL_INIT_UART) $(LIB_PHY_UART) $(LIB_DDR_UART) $(TLIB) $(LIB_SWUP_UART)

#how to add new component to binary header:
#EXAMPLE_COMPONENT=no
#ifeq ($(EXAMPLE_COMPONENT),yes)
#COMPONENT_SUBDIRS += src_example
#HDR_COMPONENTS += component_example.a
#HDR_COMPONENTS_UART += component_example.uart.a
#CFLAGS += -DCONFIG_EXAMPLE_COMPONENT
#endif

all:   subdirs $(obj)$(TGT).bin $(obj)$(TGT)_sec.bin $(obj)$(TGT).uart.bin  $(obj)$(TGT).dis $(obj)$(TGT)_sec.dis $(obj)$(TGT).uart.dis $(obj)$(TGT).srec $(obj)$(TGT)_sec.srec $(obj)$(TGT).uart.srec

$(AOBJ): $(ASRC)
	$(CC) -c -o $@ $<

$(obj)%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(obj)%.uart.o: %.c
	$(CC) $(CFLAGS) -DNOT_USE_UART -DMV_NO_INPUT -DMV_NO_PRINT  $(CPPFLAGS) -c -o  $@ $<

$(obj)$(TGT).bin: $(obj)$(TGT).elf
	$(OBJCOPY) -S -O binary $< $(addsuffix .tmp, $@)
	$(CAT) $(PARAMSFILE).raw $(addsuffix .tmp, $@) > $@
	$(RM) $(addsuffix .tmp, $@)

$(obj)$(TGT)_sec.bin: $(obj)$(TGT)_sec.elf
	$(OBJCOPY) -S -O binary $< $(addsuffix .tmp, $@)
	$(CAT) $(PARAMSFILE)_sec.raw $(addsuffix .tmp, $@) > $@
	$(RM) $(addsuffix .tmp, $@)

$(obj)$(TGT).uart.bin: $(obj)$(TGT).uart.elf
	$(OBJCOPY) -S -O binary $< $(addsuffix .tmp, $@)
	$(CAT) $(PARAMSFILE).raw $(addsuffix .tmp, $@) > $@
	$(RM) $(addsuffix .tmp, $@)

$(obj)$(TGT).srec: $(obj)$(TGT).elf
	$(OBJCOPY) -O srec $< $@

$(obj)$(TGT)_sec.srec: $(obj)$(TGT)_sec.elf
	$(OBJCOPY) -O srec $< $@

$(obj)$(TGT).dis:  $(obj)$(TGT).elf
	$(OBJDUMP) -lhaD -S $<  > $@

$(obj)$(TGT)_sec.dis:  $(obj)$(TGT)_sec.elf
	$(OBJDUMP) -lhaD -S $<  > $@

$(obj)$(TGT).uart.srec: $(obj)$(TGT).uart.elf
	$(OBJCOPY) -O srec $< $@

$(obj)$(TGT).uart.dis:  $(obj)$(TGT).uart.elf
	$(OBJDUMP) -lhaD -S $<  > $@

$(obj)$(TGT).elf: subdirs  $(AOBJ) $(COBJ) $(SETPARAM) $(LDSCRIPT)
	$(CC) $(LDFLAGSBIN) $(AOBJ) $(COBJ) $(HDR_COMPONENTS) $(LIB_PLAT) $(LIBGCC) -o $@

$(obj)$(TGT)_sec.elf: subdirs  $(AOBJ) $(COBJ) $(SETPARAM) $(LDSECSCRIPT)
	$(CC) $(LDSECFLAGSBIN) $(AOBJ) $(COBJ) $(HDR_COMPONENTS) $(LIB_PLAT) $(LIBGCC) -o $@

$(obj)$(TGT).uart.elf: $(AOBJ) $(CUART_OBJ) subdirs
	$(CC) $(LDFLAGSBIN) $(AOBJ) $(CUART_OBJ) $(HDR_COMPONENTS_UART) $(LIB_PLAT_UART) $(LIBGCC) -o $@

$(obj)$(LDSCRIPT): $(obj)$(SETPARAMS)
	$(obj)./$(SETPARAMS) -B $(BOARD) -P $(PARAMSFILE).txt -R $(DRAMREGS) -d $(DEBUG_MODE_FLAG) $(PARAMSFILE).raw $@

$(obj)$(LDSECSCRIPT): $(obj)$(SETPARAMS)
	$(obj)./$(SETPARAMS) -S -B $(BOARD) -P $(PARAMSFILE).txt -R $(DRAMREGS) -d $(DEBUG_MODE_FLAG) $(PARAMSFILE)_sec.raw $@

$(obj)$(SETPARAMS):
	$(MAKE) $(obj)$(SETPARAMS) BOARD="$(BOARD)"  HOSTOS=$(HOSTOS) HOSTARCH=$(HOSTARCH) HOSTCFLAGS="$(HOSTCFLAGS)" HOST_LDFLAGS="$(HOST_LDFLAGS)"  -C ./platform

subdirs:
	@for dir in $(COMPONENT_SUBDIRS) ; do \
	    $(MAKE) all \
		HOSTOS=$(HOSTOS) \
		HOSTARCH=$(HOSTARCH) \
		HOSTCFLAGS="$(HOSTCFLAGS)" \
		HOST_LDFLAGS="$(HOST_LDFLAGS)" \
		BOARD="$(BOARD)" \
		-C $$dir || exit 1 ; \
	done

clean:
	$(RM) $(obj)*.raw $(obj)*.elf $(obj)*.srec $(obj)*.dis $(obj)*.bin $(obj)*.a $(obj)*.uart.bin $(obj)*.uart.elf $(obj)./src/*.o  $(obj)$(LDSCRIPT) $(obj)$(LDSECSCRIPT) $(obj)$(SETPARAMS)
	@for dir in $(COMPONENT_SUBDIRS) ; do \
	    $(MAKE) clean -s -C $$dir || exit 1 ; \
	done


