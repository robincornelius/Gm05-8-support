#!/bin/sh
libtoolize -c --force
autoconf -f
aclocal -I m4 --install --force
automake -a-c
