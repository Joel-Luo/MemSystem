stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 500000000 -input /home/luo/spec2006/BenchmarkTrace/400-perlbench_T.dat -output ./result/400-perlbench.out >& ./log/B400-perlbench.txt &

stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 500000000 -input /home/luo/spec2006/BenchmarkTrace/401-bzip2_T.dat -output ./result/401-bzip2.out >& ./log/B401-bzip2.txt &

stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 100000000 -input /home/luo/spec2006/BenchmarkTrace/403-gcc_T.dat -output ./result/403-gcc.out >& ./log/B403-gcc.txt &

stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 500000000 -input /home/luo/spec2006/BenchmarkTrace/410-bwaves_T.dat -output ./result/410-bwaves.out >& ./log/B410-bwaves.txt &

stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 500000000 -input /home/luo/spec2006/BenchmarkTrace/416-gamess_T.dat -output ./result/416-gamess.out >& ./log/B416-gamess.txt &

stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/429-mcf_T.dat -output ./result/429-mcf.out >& ./log/B429-mcf.txt &

stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/433-milc_T.dat -output ./result/433-milc.out >& ./log/B433-milc.out.txt &

stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/434-zeusmp_T.dat -output ./result/434-zeusmp.out >& ./log/B434-zeusmp.txt & 

stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/435-gromacs_T.dat -output ./result/435-gromacs.out >& ./log/B435-gromacs.txt &

stdbuf -o0  ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/436-cactusADM_T.dat -output ./result/436-cactusADM.out >& ./log/B436-cactusADM.txt &
 
stdbuf -o0  ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/437-leslie3d_T.dat -output ./result/437-leslie3d.out >& ./log/B437-leslie3d.txt &

stdbuf -o0  ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/447-dealII_T.dat -output ./result/447-dealII.out >& ./log/B447-dealII.txt &

#stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/444-namd_T.dat -output ./result_setting1/444-namd.out >& ./log/S444-namd.txt &

#stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/445-gobmk.dat -output ./result_setting1/445-gobmk.out >& ./log/S445-gobmk.txt &

#stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/447-dealII_T.dat -output ./result_setting1/447-dealII.out >& ./log/S447-dealII.txt &