set SynModuleInfo {
  {SRCNAME heat2d_serial_hls_Pipeline_LOAD_I_LOAD_J MODELNAME heat2d_serial_hls_Pipeline_LOAD_I_LOAD_J RTLNAME heat2d_serial_hls_heat2d_serial_hls_Pipeline_LOAD_I_LOAD_J
    SUBMODULES {
      {MODELNAME heat2d_serial_hls_mul_6ns_8ns_13_1_1 RTLNAME heat2d_serial_hls_mul_6ns_8ns_13_1_1 BINDTYPE op TYPE mul IMPL auto LATENCY 0 ALLOW_PRAGMA 1}
      {MODELNAME heat2d_serial_hls_urem_6ns_3ns_2_10_1 RTLNAME heat2d_serial_hls_urem_6ns_3ns_2_10_1 BINDTYPE op TYPE urem IMPL auto LATENCY 9 ALLOW_PRAGMA 1}
      {MODELNAME heat2d_serial_hls_flow_control_loop_pipe_sequential_init RTLNAME heat2d_serial_hls_flow_control_loop_pipe_sequential_init BINDTYPE interface TYPE internal_upc_flow_control INSTNAME heat2d_serial_hls_flow_control_loop_pipe_sequential_init_U}
    }
  }
  {SRCNAME heat2d_serial_hls_Pipeline_STENCIL_J MODELNAME heat2d_serial_hls_Pipeline_STENCIL_J RTLNAME heat2d_serial_hls_heat2d_serial_hls_Pipeline_STENCIL_J
    SUBMODULES {
      {MODELNAME heat2d_serial_hls_dadd_64ns_64ns_64_5_full_dsp_1 RTLNAME heat2d_serial_hls_dadd_64ns_64ns_64_5_full_dsp_1 BINDTYPE op TYPE dadd IMPL fulldsp LATENCY 4 ALLOW_PRAGMA 1}
      {MODELNAME heat2d_serial_hls_dsub_64ns_64ns_64_5_full_dsp_1 RTLNAME heat2d_serial_hls_dsub_64ns_64ns_64_5_full_dsp_1 BINDTYPE op TYPE dsub IMPL fulldsp LATENCY 4 ALLOW_PRAGMA 1}
      {MODELNAME heat2d_serial_hls_dmul_64ns_64ns_64_5_max_dsp_1 RTLNAME heat2d_serial_hls_dmul_64ns_64ns_64_5_max_dsp_1 BINDTYPE op TYPE dmul IMPL maxdsp LATENCY 4 ALLOW_PRAGMA 1}
      {MODELNAME heat2d_serial_hls_sparsemux_7_2_64_1_1 RTLNAME heat2d_serial_hls_sparsemux_7_2_64_1_1 BINDTYPE op TYPE sparsemux IMPL onehotencoding_realdef}
      {MODELNAME heat2d_serial_hls_sparsemux_9_2_64_1_1 RTLNAME heat2d_serial_hls_sparsemux_9_2_64_1_1 BINDTYPE op TYPE sparsemux IMPL compactencoding_dontcare}
    }
  }
  {SRCNAME heat2d_serial_hls_Pipeline_SWAP_I_SWAP_J MODELNAME heat2d_serial_hls_Pipeline_SWAP_I_SWAP_J RTLNAME heat2d_serial_hls_heat2d_serial_hls_Pipeline_SWAP_I_SWAP_J
    SUBMODULES {
      {MODELNAME heat2d_serial_hls_urem_5ns_3ns_2_9_1 RTLNAME heat2d_serial_hls_urem_5ns_3ns_2_9_1 BINDTYPE op TYPE urem IMPL auto LATENCY 8 ALLOW_PRAGMA 1}
      {MODELNAME heat2d_serial_hls_mul_5ns_7ns_11_1_1 RTLNAME heat2d_serial_hls_mul_5ns_7ns_11_1_1 BINDTYPE op TYPE mul IMPL auto LATENCY 0 ALLOW_PRAGMA 1}
    }
  }
  {SRCNAME heat2d_serial_hls_Pipeline_STORE_I_STORE_J MODELNAME heat2d_serial_hls_Pipeline_STORE_I_STORE_J RTLNAME heat2d_serial_hls_heat2d_serial_hls_Pipeline_STORE_I_STORE_J}
  {SRCNAME heat2d_serial_hls MODELNAME heat2d_serial_hls RTLNAME heat2d_serial_hls IS_TOP 1
    SUBMODULES {
      {MODELNAME heat2d_serial_hls_urem_5ns_3ns_2_9_seq_1 RTLNAME heat2d_serial_hls_urem_5ns_3ns_2_9_seq_1 BINDTYPE op TYPE urem IMPL auto_seq LATENCY 8 ALLOW_PRAGMA 1}
      {MODELNAME heat2d_serial_hls_u_a_RAM_AUTO_1R1W RTLNAME heat2d_serial_hls_u_a_RAM_AUTO_1R1W BINDTYPE storage TYPE ram IMPL auto LATENCY 2 ALLOW_PRAGMA 1}
      {MODELNAME heat2d_serial_hls_u_b_RAM_AUTO_1R1W RTLNAME heat2d_serial_hls_u_b_RAM_AUTO_1R1W BINDTYPE storage TYPE ram IMPL auto LATENCY 2 ALLOW_PRAGMA 1}
      {MODELNAME heat2d_serial_hls_ctrl_s_axi RTLNAME heat2d_serial_hls_ctrl_s_axi BINDTYPE interface TYPE interface_s_axilite}
    }
  }
}
