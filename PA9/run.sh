#!/bin/bash

cd build || exit

idx=$1

system_name=(
  ""
  "system9_01_hose.txt"
  "system9_02_hose_gravity.txt"
  "system9_03_hose_force.txt"
  "system9_04_circle_euler.txt"
  "system9_05_circle_midpoint.txt"
  "system9_06_circle_rungekutta.txt"
  "system9_07_wave.txt"
  "system9_08_fire.txt"
  "system9_09_wind.txt"
)

cmake ..
make -j -o2

case "$idx" in
'01')
  ./particle_system -input ../SYSTEM/"${system_name["10#$idx"]}" -refresh 0.1  -dt 0.1
  ./particle_system -input ../SYSTEM/"${system_name["10#$idx"]}" -refresh 0.05 -dt 0.05
  ./particle_system -input ../SYSTEM/"${system_name["10#$idx"]}" -refresh 0.1  -dt 0.1  -motion_blur
  ./particle_system -input ../SYSTEM/"${system_name["10#$idx"]}" -refresh 0.05 -dt 0.05 -motion_blur
  ;;

'02' | '03')
  ./particle_system -input ../SYSTEM/"${system_name["10#$idx"]}" -refresh 0.05  -dt 0.05 -draw_vectors 0.1
  ./particle_system -input ../SYSTEM/"${system_name["10#$idx"]}" -refresh 0.05  -dt 0.05 -motion_blur
  ;;

'04' | '05' | '06')
  ./particle_system -input ../SYSTEM/"${system_name["10#$idx"]}" -refresh 0.1  -dt 0.1 -integrator_color -draw_vectors 0.02
  ./particle_system -input ../SYSTEM/"${system_name["10#$idx"]}" -refresh 0.05  -dt 0.05 -integrator_color -motion_blur
  ./particle_system -input ../SYSTEM/"${system_name["10#$idx"]}" -refresh 0.01  -dt 0.01 -integrator_color -motion_blur
  ;;

'07')
  ./particle_system -input ../SYSTEM/"${system_name["10#$idx"]}" -refresh 0.2  -dt 0.2 -integrator_color -motion_blur
  ./particle_system -input ../SYSTEM/"${system_name["10#$idx"]}" -refresh 0.01  -dt 0.05 -integrator_color -motion_blur
  ./particle_system -input ../SYSTEM/"${system_name["10#$idx"]}" -refresh 0.01  -dt 0.01 -integrator_color -motion_blur
  ;;

'08' | '09')
  ./particle_system -input ../SYSTEM/"${system_name["10#$idx"]}" -refresh 0.05  -dt 0.05 -motion_blur
  ;;
esac

cd .. || exit