set dataset=Dataset/DS8.txt
set iterations=10000
set max_evaluation_tims=10000
set per_evaluation_tims=1000
set runs=2
set init_sol_path=init_sol/init_08.txt

main.exe hc %dataset% %iterations% %max_evaluation_tims% %per_evaluation_tims% %runs% %init_sol_path%

pause