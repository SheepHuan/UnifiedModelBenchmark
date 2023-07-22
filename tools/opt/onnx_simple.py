# pip install onnx-simplifier
import onnx
import onnxruntime as ort
from onnxsim import simplify

in_path = "/root/workspace/UnifiedModelBenchmark/tmp/models/stereonetcmn-opset16.onnx"
out_path = "/root/workspace/UnifiedModelBenchmark/tmp/models/sim-stereonetcmn-opset16.onnx"
onnx_model = onnx.load(in_path)  # load onnx model
model_simp, check = simplify(onnx_model)
assert check, "Simplified ONNX model could not be validated"
onnx.save(model_simp, out_path)
print('finished exporting onnx')

