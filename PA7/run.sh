#!/bin/bash

idx=$1
shade_back=$2

scene_names=(
  ""
  "scene7_01_sphere_triangle.txt"
  "scene7_02_checkerboard.txt"
  "scene7_03_marble_vase.txt"
  "scene7_04_6.837_logo.txt"
  "scene7_05_glass_sphere.txt"
  "scene7_06_faceted_gem.txt"
)

cmake ..
make -j -o2

echo "removing previous image..."
rm *.tga

echo "rendering ${scene_names["10#$idx"]}..."

case "$idx" in
'01')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 180 180

  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 9 9 -render_samples output6_"$idx".tga 20 -random_samples 9
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 9 9 -render_samples output6_"$idx".tga 20 -uniform_samples 4
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 9 9 -render_samples output6_"$idx".tga 20 -jittered_samples 1

  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 9 9 -render_filter output6_"$idx".tga 20 -box_filter 2.3
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 9 9 -render_filter output6_"$idx".tga 20 -tent_filter 1.7
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -size 9 9 -render_filter output6_"$idx".tga 20 -gaussian_filter 2.3

  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 12 12 -jittered_samples 9 -gaussian_filter 1.0
  ;;

'02')
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 180 180 -jittered_samples 16 -gaussian_filter 0.6
  ;;

'03')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 15 30 15 -shadows
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 15 30 15 -shadows -jittered_samples 4 -gaussian_filter 0.4
  ;;

'04')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 400 200 -shadows -shade_back -bounces 5 -weight 0.01 -grid 80 30 3
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 40 20 -shadows -shade_back -bounces 5 -weight 0.01 -jittered_samples 9 -gaussian_filter 0.4
  ;;

'05')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 300 300 -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 300 300 -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20 -jittered_samples 4 -gaussian_filter 0.4
  ;;

'06')
  # ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20
  ./raytracer -input ../SCENE/"${scene_names["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20 -jittered_samples 4 -gaussian_filter 0.4
  ;;
esac

xdg-open output6_"$idx".tga