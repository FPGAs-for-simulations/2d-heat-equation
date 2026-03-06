
set TopModule "heat2d_serial_hls"
set ClockPeriod 10
set ClockList ap_clk
set AxiliteClockList {}
set HasVivadoClockPeriod 0
set CombLogicFlag 0
set PipelineFlag 0
set DataflowTaskPipelineFlag 1
set TrivialPipelineFlag 0
set noPortSwitchingFlag 0
set FloatingPointFlag 1
set FftOrFirFlag 0
set NbRWValue 0
set intNbAccess 0
set NewDSPMapping 1
set HasDSPModule 0
set ResetLevelFlag 0
set ResetStyle control
set ResetSyncFlag 1
set ResetRegisterFlag 0
set ResetVariableFlag 0
set ResetRegisterNum 0
set FsmEncStyle onehot
set MaxFanout 0
set RtlPrefix {}
set RtlSubPrefix heat2d_serial_hls_
set ExtraCCFlags {}
set ExtraCLdFlags {}
set SynCheckOptions {}
set PresynOptions {}
set PreprocOptions {}
set RtlWriterOptions {}
set CbcGenFlag 0
set CasGenFlag 0
set CasMonitorFlag 0
set AutoSimOptions {}
set ExportMCPathFlag 0
set SCTraceFileName mytrace
set SCTraceFileFormat vcd
set SCTraceOption all
set TargetInfo xczu5eg:-sfvc784:-1-i
set SourceFiles {sc {} c {../../../slopper/heat2d_hls/src/heat2d_parallel_hls.cpp ../../../slopper/heat2d_hls/src/heat2d_serial_hls.cpp}}
set SourceFlags {sc {} c {{} {}}}
set DirectiveFile {}
set TBFiles {verilog {/home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_types.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_stencil.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_config.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/heat2d_serial_hls.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/heat2d_parallel_hls.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/tb/tb_utils.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/tb/tb_heat2d_hls.cpp} bc {/home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_types.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_stencil.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_config.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/heat2d_serial_hls.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/heat2d_parallel_hls.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/tb/tb_utils.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/tb/tb_heat2d_hls.cpp} vhdl {/home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_types.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_stencil.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_config.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/heat2d_serial_hls.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/heat2d_parallel_hls.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/tb/tb_utils.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/tb/tb_heat2d_hls.cpp} sc {/home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_types.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_stencil.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_config.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/heat2d_serial_hls.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/heat2d_parallel_hls.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/tb/tb_utils.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/tb/tb_heat2d_hls.cpp} cas {/home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_types.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_stencil.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/hls_config.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/heat2d_serial_hls.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/include/heat2d_parallel_hls.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/tb/tb_utils.hpp /home/mallya/Desktop/LBP/slopper/heat2d_hls/tb/tb_heat2d_hls.cpp} c {}}
set SpecLanguage C
set TVInFiles {bc {} c {} sc {} cas {} vhdl {} verilog {}}
set TVOutFiles {bc {} c {} sc {} cas {} vhdl {} verilog {}}
set TBTops {verilog {} bc {} vhdl {} sc {} cas {} c {}}
set TBInstNames {verilog {} bc {} vhdl {} sc {} cas {} c {}}
set XDCFiles {}
set ExtraGlobalOptions {"area_timing" 1 "clock_gate" 1 "impl_flow" map "power_gate" 0}
set TBTVFileNotFound {}
set AppFile {}
set ApsFile hls.aps
set AvePath ../../.
set DefaultPlatform DefaultPlatform
set multiClockList {}
set SCPortClockMap {}
set intNbAccess 0
set PlatformFiles {{DefaultPlatform {xilinx/zynquplus/zynquplus}}}
set HPFPO 0
