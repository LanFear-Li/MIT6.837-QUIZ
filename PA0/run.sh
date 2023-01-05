#!/bin/bash

rm test.tga
make clean
make
./ifs -input IFS/fern.txt -points 50000 -iters 30 -size 200 -output test.tga
xdg-open test.tga
