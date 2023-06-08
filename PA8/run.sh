#!/bin/bash

cd build || exit

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
  # ./curve_editor -input ../SCENE/output8_01_bspline.txt -gui -curve_tessellation 30
  # ./curve_editor -input ../SCENE/output8_02_bezier.txt -gui -curve_tessellation 30
  ./curve_editor -input ../SCENE/output8_02_bspline.txt -gui -curve_tessellation 30
  ;;

'01')
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -gui -curve_tessellation 30

  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -output_bezier ../SCENE/output8_01_bezier.txt
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -output_bspline ../SCENE/output8_01_bspline.txt
  ;;

'02')
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -gui -curve_tessellation 30

  ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -output_bezier ../SCENE/output8_02_bezier.txt
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -output_bspline ../SCENE/output8_02_bspline.txt
  ;;

'03' | '04' | '05')
  ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -gui -curve_tessellation 30
  ;;

'06')
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -curve_tessellation 4 -gui

  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -curve_tessellation 4 -revolution_tessellation 10 -output ../MODEL/torus_low.obj
  ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -curve_tessellation 30 -revolution_tessellation 60 -output ../MODEL/torus_high.obj

  # ./raytracer -input ../SCENE/scene8_06_torus_low.txt -gui -size 300 300
  ./raytracer -input ../SCENE/scene8_06_torus_high.txt -gui -size 300 300
  ;;

'07')
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -curve_tessellation 4 -output_bspline ../SCENE/output8_07_edit.txt -gui

  # ./curve_editor -input ../SCENE/output8_07_edit.txt -curve_tessellation 4 -revolution_tessellation 10 -output ../MODEL/vase_low.obj
  # ./curve_editor -input ../SCENE/output8_07_edit.txt -curve_tessellation 10 -revolution_tessellation 60 -output ../MODEL/vase_high.obj
  ./curve_editor -input ../SCENE/output8_07_edit.txt -curve_tessellation 20 -revolution_tessellation 100 -output ../MODEL/vase_very_high.obj

  # ./raytracer -input ../SCENE/scene8_07_vase_low.txt -gui -size 300 300
  # ./raytracer -input ../SCENE/scene8_07_vase_high.txt -gui -size 300 300
  ;;

'08')
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -gui

  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -curve_tessellation 4 -patch_tessellation 4 -output ../MODEL/patch_low.obj
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -curve_tessellation 10 -patch_tessellation 10 -output ../MODEL/patch_med.obj
  ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -curve_tessellation 40 -patch_tessellation 40 -output ../MODEL/patch_high.obj

  # ./raytracer -input ../SCENE/scene8_08_bezier_patch_low.txt -gui -size 300 300
  # ./raytracer -input ../SCENE/scene8_08_bezier_patch_med.txt -gui -size 300 300
  ./raytracer -input ../SCENE/scene8_08_bezier_patch_high.txt -gui -size 300 300
  ;;

'09')
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -gui

  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -patch_tessellation 4 -curve_tessellation 4 -revolution_tessellation 10 -output ../MODEL/teapot_low.obj
  ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -patch_tessellation 30 -curve_tessellation 30 -revolution_tessellation 100 -output ../MODEL/teapot_high.obj

  # ./raytracer -input ../SCENE/scene8_09_teapot_low.txt -gui -size 300 300
  ./raytracer -input ../SCENE/scene8_09_teapot_high.txt -gui -size 300 300
  ;;

'10')
./raytracer -input ../SCENE/scene8_10_transparent_vase.txt -output output8_10.tga -grid 30 30 30 -size 300 300 -bounces 4 -shade_back -jittered_samples 9 -tent_filter 1.0 -shadows
  ;;

'11')
./raytracer -input ../SCENE/scene8_11_reflective_teapot.txt -output output8_11.tga -grid 50 30 30 -size 300 300 -bounces 4 -shade_back -jittered_samples 9 -tent_filter 1.0 -shadows
  ;;
esac

xdg-open output8_"$idx".tga

cd .. || exit