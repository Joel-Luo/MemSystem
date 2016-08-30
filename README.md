# README #

* This is a cache simulator system. 

### What is this repository for? ###

* This system base on pin tool.
* [pintool](https://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool)

### How do I get set up? ###

* git src
* make 
* ./memsystem -inst_num "500000000" -conf "config file path" -inst_num "numofinstruction" -input "input file path" -output "output result file path"

e.x. ./MemSystem -conf ./cache_buffer.cfg -inst_num 500000000 -input /home/luo/spec2006/BenchmarkTrace/400-perlbench_T.dat -output ./result/400-perlbench.out
 
* ./memsystem -h   // if you need to help.