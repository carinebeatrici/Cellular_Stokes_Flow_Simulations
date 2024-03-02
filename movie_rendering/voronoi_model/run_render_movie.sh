#!/bin/bash

echo "Creating povray files"
python gera_image.py

echo "Rendering Images"
for i in *.pov
do
  povray $1 +W3200 +H2400 -D 2>/dev/null;
  echo $i
done

echo "Rendering Movie"
ffmpeg  -i vor%4d.png -r 10 output.avi


