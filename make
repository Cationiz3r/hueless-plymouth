#!/bin/sh
# Bootstrap & check dependencies
fail() { >&2 echo "failed: $@"; exit 1; }
cd "$(dirname "$0")" || fail cd
[ -d hueless-plymouth ] && fail already made
mkdir -vp hueless-plymouth
which convert >/dev/null 2>&1 || fail no convert

# Compiling
echo :: Compiling
g++ -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-4 -pthread -I/usr/include/AL -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system \
	main.cpp anim.cpp -o anim || fail compiler
./anim || fail exec

# Moving stuff
echo :: Moving stuff
cd hueless-plymouth
mv -v ../animation-* ../throbber-* .
cp -v ../empty* ../hueless-plymouth.plymouth .

# Apply glow FX
echo :: Applying glowfx
for i in *.png; do
	echo "postfx: $i"
	convert $i \( $i -blur 0x8 -alpha on -channel alpha -evaluate set 50% \) -compose Plus -composite $i;
done

# Plymouth needs these files for the animation to play
echo :: Linking file for compatability
for i in bullet.png entry.png lock.png; do ln -sv empty.png $i; done

# Freeze at last frame and stutter out
echo :: Generating animation last frames
for i in $(seq 30 34) $(seq 39 41) 44; do           ln -sv animation-029.png   animation-$i.png; done
for i in $(seq 35 38) $(seq 42 43) $(seq 45 49); do ln -sv empty-animation.png animation-$i.png; done

# Optimizing pngs
echo :: Otimizing pngs
if which convert >/dev/null 2>&1; then
	find -type f -name '*.png' -exec optipng {} \;
else >&2 echo warning: skipping, optipng not found
fi

# Correcting permission for install
echo :: Correcting permission
chmod 644 *
chmod 755 .

echo :: Done\!
