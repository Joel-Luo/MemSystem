stdbuf -o0 ./MemSystem --conf ./cache_GT.cfg --skip_inst 100000000 --inst_num 500000000 --input /home/luo/spec2006/BenchmarkTrace/400-perlbench_T.dat --output ./result/400-perlbench_GT.out >& ./log/B400-perlbench.txt &

stdbuf -o0 ./MemSystem --conf ./cache_GT.cfg --skip_inst 100000000 --inst_num 500000000 --input /home/luo/spec2006/BenchmarkTrace/401-bzip2_T.dat --output ./result/401-bzip2_GT.out >& ./log/B401-bzip2.txt &

stdbuf -o0 ./MemSystem --conf ./cache_GT.cfg --skip_inst 100000000 --inst_num 500000000 --input /home/luo/spec2006/BenchmarkTrace/403-gcc_T.dat --output ./result/403-gcc_GT.out >& ./log/B403-gcc.txt &

#stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 500000000 -input /home/luo/spec2006/BenchmarkTrace/410-bwaves_T.dat -output ./result/410-bwaves.out -clu ../result/410-bwaves_CLUB.txt >& ./log/B410-bwaves.txt &

#stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 500000000 -input /home/luo/spec2006/BenchmarkTrace/416-gamess_T.dat -output ./result/416-gamess.out -clu ../result/416-gamess_CLUB.txt >& ./log/B416-gamess.txt &

#stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/429-mcf_T.dat -output ./result/429-mcf.out -clu ../result/429-mcf_CLUB.txt >& ./log/B429-mcf.txt &

#stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/433-milc_T.dat -output ./result/433-milc.out -clu ../result/433-milc_CLUS.txt >& ./log/B433-milc.out.txt &

#stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/434-zeusmp_T.dat -output ./result/434-zeusmp.out -clu ../result/434-zeusmp_CLUS.txt >& ./log/B434-zeusmp.txt & 

#stdbuf -o0 ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/435-gromacs_T.dat -output ./result/435-gromacs.out -clu ../result/435-gromacs_CLUS.txt >& ./log/B435-gromacs.txt &

#stdbuf -o0  ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/436-cactusADM_T.dat -output ./result/436-cactusADM.out -clu ../result/436-cactusADM_CLUS.txt >& ./log/B436-cactusADM.txt &
 
#stdbuf -o0  ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/437-leslie3d_T.dat -output ./result/437-leslie3d.out -clu ../result/437-leslie3d_CLUS.txt >& ./log/B437-leslie3d.txt &

#stdbuf -o0  ./MemSystem -conf ./cache_buffer.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/447-dealII_T.dat -output ./result/447-dealII.out -clu ../result/447-dealII_CLUS.txt >& ./log/B447-dealII.txt &

#stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/444-namd_T.dat -output ./result_setting1/444-namd.out -clu ../result/444-namd_CLUS.txt >& ./log/S444-namd.txt &

#stdbuf -o0  ./MemSystem -conf ./cache_setting1.cfg -inst_num 300000000 -input /home/luo/spec2006/BenchmarkTrace/445-gobmk.dat -output ./result_setting1/445-gobmk.out -clu ../result/445-gobmk_CLUS.txt >& ./log/S445-gobmk.txt &
