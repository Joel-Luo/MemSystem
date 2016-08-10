#!/bin/bash

declare -a list=( 400-perlbench_CLUS.txt  401-bzip2_CLUS.txt  403-gcc_CLUS.txt 410-bwaves_CLUS.txt  416-gamess_CLUS.txt  429-mcf_CLUS.txt ) 

for i in "${list[@]}" 
  do 
  str=$i
  echo ./CellTimingFreq ../../result/${str} ../../result/${str:0:${#str}-4}_output.txt
  ./CellTimingFreq ../../result/${str} ../../result/${str:0:${#str}-4}_output.txt
done