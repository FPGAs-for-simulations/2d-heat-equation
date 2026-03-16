# 2026-03-16T00:50:06.356888767
import vitis

client = vitis.create_client()
client.set_workspace(path="HLS")

vitis.dispose()

