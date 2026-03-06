# 2026-03-06T11:57:01.577845058
import vitis

client = vitis.create_client()
client.set_workspace(path="LBP")

comp = client.create_hls_component(name = "solver",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

cfg = client.get_config_file(path="/home/mallya/Desktop/LBP/solver/hls_config.cfg")

cfg.set_values(key="syn.file", values=["../slopper/heat2d_hls/src/heat2d_serial_hls.cpp", "../slopper/heat2d_hls/src/heat2d_parallel_hls.cpp"])

cfg.set_values(key="syn.file", values=["../slopper/heat2d_hls/src/heat2d_serial_hls.cpp", "../slopper/heat2d_hls/src/heat2d_parallel_hls.cpp", "../slopper/heat2d_hls/include/heat2d_parallel_hls.hpp", "../slopper/heat2d_hls/include/heat2d_serial_hls.hpp", "../slopper/heat2d_hls/include/hls_config.hpp", "../slopper/heat2d_hls/include/hls_stencil.hpp", "../slopper/heat2d_hls/include/hls_types.hpp"])

cfg.set_values(key="tb.file", values=["../slopper/heat2d_hls/tb/tb_heat2d_hls.cpp", "../slopper/heat2d_hls/tb/tb_utils.hpp"])

comp = client.get_component(name="solver")
comp.run(operation="C_SIMULATION")

cfg.set_values(key="syn.file", values=["../slopper/heat2d_hls/src/heat2d_serial_hls.cpp", "../slopper/heat2d_hls/src/heat2d_parallel_hls.cpp", "../slopper/heat2d_hls/include/heat2d_parallel_hls.hpp", "../slopper/heat2d_hls/include/heat2d_serial_hls.hpp", "../slopper/heat2d_hls/include/hls_config.hpp", "../slopper/heat2d_hls/include/hls_stencil.hpp"])

cfg.set_values(key="syn.blackbox.file", values=[])

cfg.set_values(key="syn.file", values=["../slopper/heat2d_hls/src/heat2d_serial_hls.cpp", "../slopper/heat2d_hls/src/heat2d_parallel_hls.cpp", "../slopper/heat2d_hls/include/heat2d_serial_hls.hpp", "../slopper/heat2d_hls/include/hls_config.hpp", "../slopper/heat2d_hls/include/hls_stencil.hpp"])

cfg.set_values(key="syn.blackbox.file", values=[])

cfg.set_values(key="syn.file", values=["../slopper/heat2d_hls/src/heat2d_serial_hls.cpp", "../slopper/heat2d_hls/src/heat2d_parallel_hls.cpp", "../slopper/heat2d_hls/include/hls_config.hpp", "../slopper/heat2d_hls/include/hls_stencil.hpp"])

cfg.set_values(key="syn.blackbox.file", values=[])

cfg.set_values(key="syn.file", values=["../slopper/heat2d_hls/src/heat2d_serial_hls.cpp", "../slopper/heat2d_hls/src/heat2d_parallel_hls.cpp", "../slopper/heat2d_hls/include/hls_config.hpp"])

cfg.set_values(key="syn.blackbox.file", values=[])

cfg.set_values(key="syn.file", values=["../slopper/heat2d_hls/src/heat2d_serial_hls.cpp", "../slopper/heat2d_hls/src/heat2d_parallel_hls.cpp"])

cfg.set_values(key="syn.blackbox.file", values=[])

cfg.set_values(key="tb.file", values=["../slopper/heat2d_hls/tb/tb_heat2d_hls.cpp", "../slopper/heat2d_hls/tb/tb_utils.hpp", "../slopper/heat2d_hls/include/heat2d_parallel_hls.hpp", "../slopper/heat2d_hls/include/heat2d_serial_hls.hpp", "../slopper/heat2d_hls/include/hls_config.hpp", "../slopper/heat2d_hls/include/hls_stencil.hpp", "../slopper/heat2d_hls/include/hls_types.hpp"])

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

cfg.set_values(key="syn.file_cflags", values=[])

cfg.set_values(key="syn.file", values=["../slopper/heat2d_hls/src/heat2d_serial_hls.cpp", "../slopper/heat2d_hls/src/heat2d_parallel_hls.cpp"])

cfg.set_values(key="syn.file_csimflags", values=[])

cfg.set_value(section="hls", key="syn.csimflags", value="")

cfg.set_value(section="hls", key="syn.cflags", value="-I../slopper/heat2d_hls/include")

cfg.set_value(section="hls", key="syn.cflags", value="")

cfg.set_value(section="hls", key="syn.csimflags", value="")

cfg.set_values(key="syn.file", values=["../slopper/heat2d_hls/src/heat2d_serial_hls.cpp", "../slopper/heat2d_hls/src/heat2d_parallel_hls.cpp"])

cfg.set_values(key="syn.file_cflags", values=[])

cfg.set_values(key="syn.file_csimflags", values=[])

cfg.set_values(key="syn.file_csimflags", values=[])

cfg.set_value(section="hls", key="syn.csimflags", value="")

cfg.set_values(key="syn.file", values=["../slopper/heat2d_hls/src/heat2d_serial_hls.cpp", "../slopper/heat2d_hls/src/heat2d_parallel_hls.cpp", "../slopper/heat2d_hls/include/heat2d_parallel_hls.hpp", "../slopper/heat2d_hls/include/heat2d_serial_hls.hpp", "../slopper/heat2d_hls/include/hls_config.hpp", "../slopper/heat2d_hls/include/hls_stencil.hpp", "../slopper/heat2d_hls/include/hls_types.hpp"])

cfg.set_values(key="syn.file_cflags", values=[])

cfg.set_value(section="hls", key="syn.cflags", value="")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

cfg.set_value(section="hls", key="syn.top", value="heat2d_serial_hls")

comp.run(operation="SYNTHESIS")

