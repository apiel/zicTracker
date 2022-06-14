#!/bin/sh

mkdir -p opk
cp ./zicServer.elf opk/zicServer.elf
cp ./icon.png opk/icon.png

# https://unix.stackexchange.com/questions/219268/how-to-add-new-lines-when-using-echo
print()
	case    ${IFS- } in
	(\ *)   printf  %b\\n "$*";;
	(*)     IFS=\ $IFS
	printf  %b\\n "$*"
	IFS=${IFS#?}
esac

# Create GmenuNx entry file plus other things

print '[Desktop Entry]
Type=Application
Name=Zic
Comment=Zic synth
Exec=zicServer.elf
Icon=icon
Terminal=false
Type=Application
Categories=audio;
X-OD-NeedsDownscaling=true' > opk/default.gcw0.desktop

mksquashfs ./opk zicServer.opk -all-root -noappend -no-exports -no-xattrs

rm -r opk
