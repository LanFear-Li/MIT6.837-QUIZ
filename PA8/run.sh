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
  ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -curve_tessellation 4 -gui
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -curve_tessellation 4 -revolution_tessellation 10 -output ../SCENE/torus_low.obj
  # ./curve_editor -input ../SCENE/"${spline_name["10#$idx"]}" -curve_tessellation 30 -revolution_tessellation 60 -output ../SCENE/torus_high.obj
  ;;
esac

# xdg-open output6_"$idx".tga

cd .. || exit