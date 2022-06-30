#!/bin/sh
fail() { >&2 echo "failed: $@"; exit 1; }
cd "$(dirname "$0")" || fail cd
[ -d hueless-plymouth ] && fail already made
mkdir -vp hueless-plymouth
which convert >/dev/null 2>&1 || fail no convert
g++ -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-4 -pthread -I/usr/include/AL -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system \
	main.cpp anim.cpp -o anim || fail compiler
./anim || fail exec
cd hueless-plymouth
mv ../animation-* ../throbber-* .
cp ../empty.png ../hueless-plymouth.plymouth .
# Apply glow FX
for i in *.png; do convert $i \( $i -blur 0x8 -alpha on -channel alpha -evaluate set 50% \) -compose Plus -composite $i; done
for i in bullet.png entry.png lock.png; do ln -s empty.png $i; done
