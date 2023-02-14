#!/bin/bash

idx=$1
shade_back=$2

scene_names=(
  ""
  "scene5_01_sphere.txt"
  "scene5_02_spheres.txt"
  "scene5_03_offcenter_spheres.txt"
  "scene5_04_plane_test.txt"
  "scene5_05_sphere_triangles.txt"
  "scene5_06_bunny_mesh_200.txt"
  "scene5_07_bunny_mesh_1k.txt"
  "scene5_08_bunny_mesh_5k.txt"
  "scene5_09_bunny_mesh_40k.txt"
  "scene5_10_scale_translate.txt"
  "scene5_11_rotated_triangles.txt"
  "scene5_12_nested_transformations.txt"
)

cmake ..
make -j

echo "removing previous image..."
rm *.tga

echo "rendering ${scene_names["10#$idx"]}..."

case "$idx" in
'01')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -grid 1 1 1 -visualize_grid
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -grid 5 5 5 -visualize_grid
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -grid 15 15 15 -visualize_grid
  ;;

'02')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -grid 15 15 15 -visualize_grid
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -grid 15 15 3 -visualize_grid

  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -gui -grid 8 8 8 -visualize_grid
  ;;

'03')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -grid 20 20 20 -visualize_grid
  ;;

'04')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -tessellation 30 15 -gouraud
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -grid 15 15 15 -visualize_grid
  ;;

'05')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -tessellation 30 15 -gouraud
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -grid 20 20 10 -visualize_grid
  ;;

'06')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -grid 10 10 7 -visualize_grid
  ;;

'07')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -grid 15 15 12 -visualize_grid
  ;;

'08')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -grid 20 20 15 -visualize_grid
  ;;

'09')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_"$idx".tga -gui -grid 40 40 33 -visualize_grid
  ;;

'10')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -gui -tessellation 30 15 -gouraud
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output output5_10.tga -gui -grid 15 15 15 -visualize_grid
  ;;

'11')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output -gui
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output -output output5_11.tga -gui -grid 15 15 9 -visualize_grid
  ;;

'12')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output -gui
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 200 200 -output -output output5_11.tga -gui -grid 30 30 30 -visualize_grid
  ;;
esac

xdg-open output5_"$idx".tga
