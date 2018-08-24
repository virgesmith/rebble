#!/bin/bash

# TODO can this be integrated into wscript?

# Common headers
ln -s ../common include
# Common sources
sources="Click.c Window.c TextBox.c Timer.c DateTime.c"
cd src/c
for source in $sources; do
  ln -s ../../../common/$source
done