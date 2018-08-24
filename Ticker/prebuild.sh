#!/bin/bash

# TODO can this be integrated into wscript?

# Common headers
ln -s ../common include
# Common sources
sources="Window.c Index.c TextBox.c Timer.c DateTime.c utils.c"
cd src/c
for source in $sources; do
  ln -s ../../../common/$source
done