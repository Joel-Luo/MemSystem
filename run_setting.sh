stdbuf -o0 ./MemSystem -conf ./cache_setting1.cfg -inst_num 500000000 -input /home/luo/spec2006/BenchmarkTrace/400-perlbench_T.dat -output ./result_setting1/400-perlbench.out >& ./log/S400-perlbench.txt &

stdbuf -o0 ./MemSystem -conf ./cache_setting1.cfg -inst_num 500000000 -input /home/luo/spec2006/BenchmarkTrace/401-bzip2_T.dat -output ./result_setting1/401-bzip2.out >& ./log/S401-bzip2.txt &

stdbuf -o0 ./MemSystem -conf ./cache_setting1.cfg -inst_num 100000000 -input /home/luo/spec2006/BenchmarkTrace/403-gcc_T.dat -output ./result_setting1/403-gcc.out >& ./log/S403-gcc.txt &

stdbuf -o0 ./MemSystem -conf ./cache_setting1.cfg -inst_num 500000000 -input /home/luo/spec2006/BenchmarkTrace/410-bwaves_T.dat -output ./result_setting1/410-bwaves.out >& ./log/S410-bwaves.txt &

stdbuf -o0 ./MemSystem -conf ./cache_setting1.cfg -inst_num 500000000 -input /home/luo/spec2006/BenchmarkTrace/416-gamess_T.dat -output ./result_setting1/416-gamess.out >& ./log/S416-gamess.txt &

stdbuf -o0 ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/429-mcf_T.dat -output ./result_setting1/429-mcf.out >& ./log/S429-mcf.txt &

stdbuf -o0 ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/433-milc_T.dat -output ./result_setting1/433-milc.out >& ./log/S433-milc.out.txt &

stdbuf -o0 ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/434-zeusmp_T.dat -output ./result_setting1/434-zeusmp.out >& ./log/S434-zeusmp.txt & 

stdbuf -o0 ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/435-gromacs_T.dat -output ./result_setting1/435-gromacs.out >& ./log/S435-gromacs.txt &

stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/436-cactusADM_T.dat -output ./result_setting1/436-cactusADM.out >& ./log/S436-cactusADM.txt &
 
stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/437-leslie3d_T.dat -output ./result_setting1/437-leslie3d.out >& ./log/S437-leslie3d.txt &

#stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/447-dealII_T.dat -output ./result_setting1/447-dealII.out >& ./log/S447-dealII.txt &

#stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/444-namd_T.dat -output ./result_setting1/444-namd.out >& ./log/S444-namd.txt &

#stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/445-gobmk.dat -output ./result_setting1/445-gobmk.out >& ./log/S445-gobmk.txt &

#stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/447-dealII_T.dat -output ./result_setting1/447-dealII.out >& ./log/S447-dealII.txt &