stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 500000000 -input /home/luo/spec2006/BenchmarkTrace/416-gamess_T.dat -output ./result/416-gamess.out >& ./log/job1.txt &

stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/434-zeusmp_T.dat -output ./result/434-zeusmp.out >& ./log/job2.txt & 

stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/435-gromacs_T.dat -output ./result/435-gromacs.out >& ./log/job3.txt &



stdbuf -o0  ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/436-cactusADM_T.dat -output ./result/436-cactusADM.out >& ./log/B436-cactusADM.txt &
 
stdbuf -o0  ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/437-leslie3d_T.dat -output ./result/437-leslie3d.out >& ./log/B437-leslie3d.txt &

stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/436-cactusADM_T.dat -output ./result_setting1/436-cactusADM.out >& ./log/S436-cactusADM.txt &
 
stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/437-leslie3d_T.dat -output ./result_setting1/437-leslie3d.out >& ./log/S437-leslie3d.txt &



stdbuf -o0  ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/444-namd_T.dat -output ./result/444-namd.out >& ./log/B444-namd.txt &

stdbuf -o0  ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/445-gobmk.dat -output ./result/445-gobmk.out >& ./log/B445-gobmk.txt &

stdbuf -o0  ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/447-dealII_T.dat -output ./result/447-dealII.out >& ./log/B447-dealII.txt &

stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/444-namd_T.dat -output ./result_setting1/444-namd.out >& ./log/S444-namd.txt &

stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/445-gobmk.dat -output ./result_setting1/445-gobmk.out >& ./log/S445-gobmk.txt &

stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/447-dealII_T.dat -output ./result_setting1/447-dealII.out >& ./log/S447-dealII.txt &