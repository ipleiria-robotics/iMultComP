## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,a8fg linker.cmd package/cfg/am335x_app_pa8fg.oa8fg

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/am335x_app_pa8fg.xdl
	$(SED) 's"^\"\(package/cfg/am335x_app_pa8fgcfg.cmd\)\"$""\"/home/valentim/Desktop/iMultComP/Am3359_profibus/.config/xconfig_am335x_app/\1\""' package/cfg/am335x_app_pa8fg.xdl > $@
	-$(SETDATE) -r:max package/cfg/am335x_app_pa8fg.h compiler.opt compiler.opt.defs
