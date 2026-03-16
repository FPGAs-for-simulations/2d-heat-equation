# 2026-03-16T16:33:09.373951888
import vitis

client = vitis.create_client()
client.set_workspace(path="HLS")

comp = client.get_component(name="solver")
comp.run(operation="SYNTHESIS")

