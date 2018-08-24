#!/bin/bash

# TODO can this be integrated into wscript?

# Common headers
ln -s ../common include
# Common sources
sources="Timer.c Window.c"
cd src/c
for source in $sources; do
  ln -s ../../../common/$source
done