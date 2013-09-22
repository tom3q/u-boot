#
# Copyright (C) 2009 Samsung Electronics
# Minkyu Kang <mk7.kang@samsung.com>
#
# SPDX-License-Identifier:	GPL-2.0+
#

include $(TOPDIR)/config.mk

LIB	= $(obj)libsamsung-common.o

COBJS-y			+= cpu_info.o

ifdef CONFIG_SPL_BUILD
ifndef CONFIG_SPL_FRAMEWORK
	MINIMAL_BUILD  = yes;
endif
endif

ifndef MINIMAL_BUILD
COBJS-y			+= sromc.o 
COBJS-y			+= timer.o
COBJS-y			+= pwm.o
endif

SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS-y) $(SOBJS))

all:	 $(obj).depend $(LIB)

$(LIB):	$(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
