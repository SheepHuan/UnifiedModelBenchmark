import glob

path = "/root/workspace/UnifiedModelBenchmark/3rd-party/onnxruntime/build/Android/Debug"

files = glob.glob(f"{path}/libonnxruntime*.a")

files = [file.split("/")[-1] for file in files]
files = [file.replace(".a","") for file in files]
files = [file.replace("lib","") for file in files]
print("\n".join(files))