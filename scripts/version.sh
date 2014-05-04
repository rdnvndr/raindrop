#!/bin/sh

TVER=`cat version.h | grep '#define VER_REV'`
VER=`git describe --tags --always`

if [ "$TVER" != "#define VER_REV \"$VER\"" ]; then
  DATE_REV=`git show -s --format=format:"%ci" $VER`
  echo "#ifndef VERSION_H" > version.h
  echo "#define VERSION_H" >> version.h
  echo "#define VER_REV \"$DATE_REV ($VER)\"" >> version.h
  echo "#endif" >> version.h
fi


