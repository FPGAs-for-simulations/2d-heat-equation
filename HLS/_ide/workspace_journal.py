# 2026-03-15T17:37:16.737580629
import vitis

client = vitis.create_client()
client.set_workspace(path="HLS")

comp = client.create_hls_component(name = "solver",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

cfg = client.get_config_file(path="/home/mallya/Desktop/Heat_eqn_solver/HLS/solver/hls_config.cfg")

cfg.set_values(key="syn.file", values=["solver.cpp"])

cfg.set_values(key="tb.file", values=["solver_tb.cpp"])

