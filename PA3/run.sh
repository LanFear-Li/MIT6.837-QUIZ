#!/bin/bash

idx=$1
shade_back=$2

scene_names=(
  ""
  "scene3_01_cube_orthographic.txt"
  "scene3_02_cube_perspective.txt"
  "scene3_03_bunny_mesh_200.txt"
  "scene3_04_bunny_mesh_1k.txt"
  "scene3_05_axes_cube.txt"
  "scene3_06_crazy_transforms.txt"
  "scene3_07_plane.txt"
  "scene3_08_sphere.txt"
  "scene3_09_exponent_variations.txt"
  "scene3_10_exponent_variations_back.txt"
  "scene3_11_weird_lighting_diffuse.txt"
  "scene3_12_weird_lighting_specular.txt"
)

cmake ..
make -j

echo "removing previous image..."
rm *.tga

echo "rendering ${scene_names["10#$idx"]}..."

case "$idx" in
'01' | '02' | '03' | '04' | '05' | '06')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output3_"$idx".tga -gui
  ;;

'07')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output3_"$idx".tga -gui -tessellation 10 5
  ;;

'08')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output3_"$idx".tga -gui -tessellation 10 5
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output3_"$idx".tga -gui -tessellation 20 10
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output3_"$idx".tga -gui -tessellation 10 5 -gouraud
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output3_"$idx".tga -gui -tessellation 20 10 -gouraud
  ;;

'09' | '10')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 300 300 -output output3_"$idx".tga -gui -tessellation 100 50 -gouraud
  ;;

'11' | '12')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output3_"$idx".tga -gui -tessellation 100 50 -gouraud
  ;;
esac

xdg-open output3_"$idx".tga
