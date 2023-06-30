# pip install onnx-simplifier
import onnx
from onnxsim import simplify

in_path = "/root/workspace/UnifiedModelBenchmark/models/fusenet_large-opset16.onnx"
out_path = ""
onnx_model = onnx.load(in_path)  # load onnx model
model_simp, check = simplify(onnx_model)
assert check, "Simplified ONNX model could not be validated"
onnx.save(model_simp, out_path)
print('finished exporting onnx')