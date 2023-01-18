#!/bin/bash

idx=$1
shade_back=$2

scene_names=(
  ""
  "scene2_01_diffuse.txt"
  "scene2_02_ambient.txt"
  "scene2_03_colored_lights.txt"
  "scene2_04_perspective.txt"
  "scene2_05_inside_sphere.txt"
  "scene2_06_plane.txt"
  "scene2_07_sphere_triangles.txt"
  "scene2_08_cube.txt"
  "scene2_09_bunny_200.txt"
  "scene2_10_bunny_1k.txt"
  "scene2_11_squashed_sphere.txt"
  "scene2_12_rotated_sphere.txt"
  "scene2_13_rotated_squashed_sphere.txt"
  "scene2_14_axes_cube.txt"
  "scene2_15_crazy_transforms.txt"
  "scene2_16_t_scale.txt"
)

cmake ..
make -j

echo "removing previous image..."
rm *.tga

echo "rendering ${scene_names["10#$idx"]}..."

case "$idx" in
'01' | '02')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output2_"$idx".tga
  ;;

'03' | '04')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output2_"$idx".tga -normals normals2_"$idx".tga
  ;;

'05')
  if [ $# -eq 2 ] &&  [ "$shade_back" -eq 1 ]; then
    ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output2_"$idx".tga -depth 9 11 depth2_"$idx".tga -normals normals2_"$idx".tga -shade_back
  else
    ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output2_"$idx".tga
  fi
  ;;

'06')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output2_"$idx".tga -depth 8 20 depth2_"$idx".tga -normals normals2_"$idx".tga
  ;;

'07')
  if [ $# -eq 2 ] &&  [ "$shade_back" -eq 1 ]; then
    ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output2_"$idx".tga -depth 9 11 depth2_"$idx".tga -normals normals2_"$idx".tga -shade_back
  else
    ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output2_"$idx".tga
  fi
  ;;

'08' | '09' | '10')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output2_"$idx".tga
  ;;

'11' | '12' | '13')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output2_"$idx".tga -normals normals2_"$idx".tga
  ;;

'14' | '15')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output2_"$idx".tga
  ;;

'16')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output2_"$idx".tga -depth 2 7 depth2_"$idx".tga
  ;;
esac

xdg-open output2_"$idx".tga
xdg-open normals2_"$idx".tga
