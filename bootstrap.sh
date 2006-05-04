#!/bin/sh
libtoolize --force
aclocal
autoconf
automake -a
