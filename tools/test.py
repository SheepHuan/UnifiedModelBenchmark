import glob
import os


# files = glob.glob(f"/root/workspace/UnifiedModelBenchmark/tmp/onnx_models/*.onnx")
# filenames = [file.split("/")[-1].replace(".onnx","") for file in files]
# cmds = ["#!/bin/bash"]
# for file,filename in zip(files,filenames):
#     cmd = f"echo {filename} ; "
#     cmd += f"adb push --sync {file} /data/local/tmp/hcp/models ; "
#     cmd += f'adb -s 3a9c4f5 shell "export LD_LIBRARY_PATH=/data/local/tmp/hcp/libs ; ./data/local/tmp/hcp/ort_benchmark \
# --model=/data/local/tmp/hcp/models/{filename}.onnx \
# --backend=arm --num_warmup=10 --num_runs=40 --num_threads=4 --enable_op_profiling=true --prefix=/data/local/tmp/hcp/{filename}" > {filename}.txt 2>&1 ; '
#     # cmd += f"adb pull /data/local/tmp/hcp/{filename}*.json . ; "
#     # cmd += f"adb shell 'rm /data/local/tmp/hcp/{filename}*.json' ; " 
#     cmds.append(cmd)
    
# open("bath_test.sh","w").write('\n'.join(cmds))


s = """
config_0.1_model_2023-07-19_10-11-55.json
config_0.2_model_2023-07-19_10-12-11.json
config_0.3_model_2023-07-19_10-11-04.json
config_0.4_model_2023-07-19_10-10-43.json
config_0.5_model_2023-07-19_10-12-28.json
config_0.6_model_2023-07-19_10-10-17.json
config_0.7_model_2023-07-19_10-11-22.json
config_0.8_model_2023-07-19_10-09-43.json
config_0.9_model_2023-07-19_10-09-08.json
config_1.0_model_2023-07-19_10-08-30.json
"""
filenames = s.split("\n")
filenames = [filename for filename in filenames if filename!=""]
cmd = ""
for filename in filenames:
    cmd += f"adb pull /data/local/tmp/hcp/{filename} . ;\n"
print(cmd)
    
