# Inferred from performance & pipeline pragmas/directives
set_directive_loop_flatten heat2d_serial_hls/LOAD_I
set_directive_array_partition heat2d_serial_hls/u_a_0 -dim=1 -type=cyclic -factor=3 u_a_0
set_directive_loop_flatten heat2d_serial_hls/SWAP_I
set_directive_loop_flatten heat2d_serial_hls/STORE_I
