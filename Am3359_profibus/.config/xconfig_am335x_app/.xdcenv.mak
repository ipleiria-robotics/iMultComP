#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /home/valentim/ti/bios_6_76_03_01/packages;/home/valentim/ti/pdk_am335x_1_0_17/packages;/home/valentim/ti/ndk_3_61_01_01/packages;/home/valentim/ti/edma3_lld_2_12_05_30E/packages;/home/valentim/Desktop/iMultComP/Am3359_profibus/.config
override XDCROOT = /home/valentim/ti/xdctools_3_55_02_22_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /home/valentim/ti/bios_6_76_03_01/packages;/home/valentim/ti/pdk_am335x_1_0_17/packages;/home/valentim/ti/ndk_3_61_01_01/packages;/home/valentim/ti/edma3_lld_2_12_05_30E/packages;/home/valentim/Desktop/iMultComP/Am3359_profibus/.config;/home/valentim/ti/xdctools_3_55_02_22_core/packages;..
HOSTOS = Linux
endif
