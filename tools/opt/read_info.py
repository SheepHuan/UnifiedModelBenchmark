import onnxruntime as ort


# Load the ONNX model
model_path = '/root/workspace/UnifiedModelBenchmark/tmp/models/sim-stereonetcmn-opset16.onnx'
sess = ort.InferenceSession(model_path)

# Get the input information
inputs = sess.get_inputs()

for input in inputs:
    input_name = input.name
    input_shape = input.shape
    print(f"[{input_name},{'x'.join([str(i) for i in input_shape])},float32]")
