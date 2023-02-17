#!/bin/bash

idx=$1
shade_back=$2

scene_names=(
  ""
  "scene6_01_sphere.txt"
  "scene6_02_sphere_triangles.txt"
  "scene6_03_sphere_plane.txt"
  "scene6_04_bunny_mesh_200.txt"
  "scene6_05_bunny_mesh_1k.txt"
  "scene6_06_bunny_mesh_5k.txt"
  "scene6_07_bunny_mesh_40k.txt"
  "scene6_08_scale_translate.txt"
  "scene6_09_rotated_triangles.txt"
  "scene6_10_nested_transformations.txt"
  "scene6_11_mirrored_floor.txt"
  "scene6_12_faceted_gem.txt"
  "scene6_13_checkerboard.txt"
  "scene6_14_glass_sphere.txt"
  "scene6_15_marble_cubes.txt"
  "scene6_16_wood_cubes.txt"
  "scene6_17_marble_vase.txt"
  "scene6_18_6.837_logo.txt"
)

cmake ..
make -j

echo "removing previous image..."
rm *.tga

echo "rendering ${scene_names["10#$idx"]}..."

case "$idx" in
'01')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -stats
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 10 10 10 -stats
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 10 10 10 -visualize_grid
  ;;

'02' | '03')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -stats
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 10 10 10 -stats
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -stats -shadows
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 10 10 10 -stats -shadows
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 10 10 10 -visualize_grid
  ;;

'04')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -stats
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 10 10 7 -stats
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -stats -shadows
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 10 10 7 -stats -shadows
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 10 10 7 -visualize_grid
  ;;

'05')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 15 15 12 -stats -shadows
  ;;

'06')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 20 20 15 -stats -shadows
  ;;

'07')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 40 40 33 -stats -shadows
  ;;

'08')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 15 15 15
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 15 15 15 -visualize_grid
  ;;

'09')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 15 15 9
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 15 15 9 -visualize_grid
  ;;

'10')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 30 30 30
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 30 30 30 -visualize_grid
  ;;

'11')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -shadows -bounces 1 -weight 0.01 -stats
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -shadows -bounces 1 -weight 0.01 -grid 40 10 40 -stats
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 40 10 40 -visualize_grid
  ;;

'12')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -shadows -shade_back -bounces 5 -weight 0.01 -stats
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20 -stats
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 20 20 20 -visualize_grid
  ;;

'13')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -shadows
  ;;

'14')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20
  ;;

'15' | '16')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 300 300
  ;;

'17')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 300 300 -grid 15 30 15 -bounces 1 -shadows
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 300 300 -grid 15 30 15 -visualize_grid
  ;;

'18')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 400 200 -shadows -shade_back -bounces 5 -weight 0.01 -grid 80 30 3
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 400 200 -grid 80 30 3 -visualize_grid
  ;;
esac

xdg-open output6_"$idx".tga
