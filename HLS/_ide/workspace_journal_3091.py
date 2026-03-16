# 2026-03-16T12:55:52.703895614
import vitis

client = vitis.create_client()
client.set_workspace(path="HLS")

comp = client.get_component(name="solver")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

cfg = client.get_config_file(path="/home/mallya/Desktop/Heat_eqn_solver/HLS/solver/hls_config.cfg")

cfg.set_value(section="hls", key="syn.rtl.reset_level", value="low")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

