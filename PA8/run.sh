#!/bin/bash

idx=$1

spline_name=(
  ""
  "spline8_01_bezier.txt"
  "spline8_02_bspline.txt"
  "spline8_03_bezier.txt"
  "spline8_04_bspline.txt"
  "spline8_05_bspline_dups.txt"
  "spline8_06_torus.txt"
  "spline8_07_vase.txt"
  "spline8_08_bezier_patch.txt"
  "spline8_09_teapot.txt"
)

cmake ..
make -j -o2

case "$idx" in
'00')
  # ./curve_editor -input ../SCENE/output8_01_bezier.txt -gui -curve_tessellation 30
  ./curve_editor -input ../SCENE/output8_01_bspline.txt -gui -curve_tessellation 30
  # ./curve_editor -input ../SCENE/output8_02_bezier.txt -gui -curve_tessellation 30
  # ./curve_editor -input ../SCENE/output8_02_bspline.txt -gui -curve_tessellation 30
  ;;

'01')
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -gui -curve_tessellation 30

  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -output_bezier ../SCENE/output8_01_bezier.txt
  ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -output_bspline ../SCENE/output8_01_bspline.txt
  ;;

'02')
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -gui -curve_tessellation 30

  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -output_bezier ../SCENE/output8_02_bezier.txt
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -output_bspline ../SCENE/output8_02_bspline.txt
  ;;

'03')
  # ./raytracer -input ../SCENE/"${spline_name["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 15 30 15 -shadows
  ./raytracer -input ../SCENE/"${spline_name["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -grid 15 30 15 -shadows -jittered_samples 4 -gaussian_filter 0.4
  ;;

'04')
  # ./raytracer -input ../SCENE/"${spline_name["10#$idx"]}" -output output6_"$idx".tga -size 400 200 -shadows -shade_back -bounces 5 -weight 0.01 -grid 80 30 3
  ./raytracer -input ../SCENE/"${spline_name["10#$idx"]}" -output output6_"$idx".tga -size 40 20 -shadows -shade_back -bounces 5 -weight 0.01 -jittered_samples 9 -gaussian_filter 0.4
  ;;

'05')
  # ./raytracer -input ../SCENE/"${spline_name["10#$idx"]}" -output output6_"$idx".tga -size 300 300 -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20
  ./raytracer -input ../SCENE/"${spline_name["10#$idx"]}" -output output6_"$idx".tga -size 300 300 -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20 -jittered_samples 4 -gaussian_filter 0.4
  ;;

'06')
  # ./raytracer -input ../SCENE/"${spline_name["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20
  ./raytracer -input ../SCENE/"${spline_name["10#$idx"]}" -output output6_"$idx".tga -size 200 200 -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20 -jittered_samples 4 -gaussian_filter 0.4
  ;;
esac

# xdg-open output6_"$idx".tga