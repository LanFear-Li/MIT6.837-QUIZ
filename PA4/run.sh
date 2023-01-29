#!/bin/bash

idx=$1
shade_back=$2

scene_names=(
  ""
  "scene4_01_sphere_shadow.txt"
  "scene4_02_colored_shadows.txt"
  "scene4_03_mirrored_floor.txt"
  "scene4_04_reflective_sphere.txt"
  "scene4_05_transparent_bar.txt"
  "scene4_06_transparent_bars.txt"
  "scene4_07_transparent_sphere_1.0.txt"
  "scene4_08_transparent_sphere_1.1.txt"
  "scene4_09_transparent_sphere_2.0.txt"
  "scene4_10_point_light_distance.txt"
  "scene4_11_point_light_circle.txt"
  "scene4_12_point_light_circle_d_attenuation.txt"
  "scene4_13_point_light_circle_d2_attenuation.txt"
  "scene4_14_faceted_gem.txt"
)

cmake ..
make -j

echo "removing previous image..."
rm *.tga

echo "rendering ${scene_names["10#$idx"]}..."

case "$idx" in
'01')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output4_"$idx".tga -shadows
  ;;

'02')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output4_"$idx".tga -shadows -gui -tessellation 50 25 -gouraud
  ;;

'03')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output4_"$idx".tga -shadows -bounces 1 -weight 0.01 -gui
  ;;

'04')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output4_"$idx".tga -shadows -bounces 0 -weight 0.01
  ;;

'05')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output4_"$idx".tga -shadows -bounces 10 -weight 0.01 -shade_back -gui
  ;;

'06')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output4_"$idx".tga -shadows -bounces 1 -weight 0.01 -shade_back -gui
  ;;

'07' | '08' | '09')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output4_"$idx".tga -shadows -bounces 5 -weight 0.01 -shade_back -gui -tessellation 30 15
  ;;

'10')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output4_"$idx".tga -shadows -gui
  ;;

'11' | '12' | '13')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output4_"$idx".tga -shadows
  ;;

'14')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 300 300 -output output4_"$idx".tga -shadows -shade_back -bounces 0 -weight 0.01
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -shadows -shade_back -bounces 1 -weight 0.01 -gui
  ;;
esac

xdg-open output4_"$idx".tga
