# import pandas as pd
# import os
# # df = pd.read_excel('scripts/models.xlsx',sheet_name="Sheet0", usecols=[0,1])

# # print(df)
# # tmp = 'cd /workspace/models && export https_proxy="http://172.16.6.189:7890" && '
# # for index,row in df.iterrows():
# #     data = row.tolist()
# #     # print(data)
# #     model_name = data[0]
# #     url = data[1].replace("/raw/main","")
# #     url = url.replace("https://github.com/onnx/models/","")
# #     # print(url)
# #     cmd=f'git lfs pull --include="{url}" --exclude=""\n'
# #     os.system(f'{tmp} {cmd}')

# # https://cdn.githubjs.cf

# import glob

# dirs = glob.glob("/workspace/models/vision/*")

# models = {
    
# }

# def findPath(dir):
#     pathes= glob.glob(f"{dir}/*")
#     for path in pathes:
#         if os.path.isfile(path):
#             if path.split(".")[-1] == "onnx":
#                 print(path)
#         elif os.path.isdir(path):
#             print("dir",path)
#             findPath(path)

# findPath("/workspace/models/vision")
# # for dir in dirs:
# #     pathes= glob.glob(f"{dir}/*")
# #     for path in pathes:
# #         if os.path.isfile(path):
# #             print("file",path)
# #         elif os.path.isdir(path):
# #             print("dir",path)
# #             for subpath in glob.glob(f"{path}/*"):
# #                 print(subpath)

import glob

model_files = glob.glob(f"/root/workspace/UnifiedModelBenchmark/models/*.onnx")
"""
./main --model_path="/root/workspace/UnifiedModelBenchmark/models/yolov4.onnx" --image_shape="1x3x416x416"

"""
for file in model_files:
    print(file)
    image_shape = ""
    image_path = "/root/workspace/UnifiedModelBenchmark/samples/dog.jpg"
    cmd = f"./main --model_path={file} --image_shape={image_shape} --image_path={image_path}"