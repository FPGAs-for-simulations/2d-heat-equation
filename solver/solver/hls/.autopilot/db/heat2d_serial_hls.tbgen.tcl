set moduleName heat2d_serial_hls
set isTopModule 1
set isCombinational 0
set isDatapathOnly 0
set isPipelined 0
set isPipelined_legacy 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set restart_counter_num 0
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set hasInterrupt 0
set DLRegFirstOffset 0
set DLRegItemOffset 0
set svuvm_can_support 1
set cdfgNum 7
set C_modelName {heat2d_serial_hls}
set C_modelType { void 0 }
set ap_memory_interface_dict [dict create]
dict set ap_memory_interface_dict u_in { MEM_WIDTH 64 MEM_SIZE 8192 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict u_out { MEM_WIDTH 64 MEM_SIZE 8192 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
set C_modelArgList {
	{ u_in int 64 regular {array 1024 { 1 3 } 1 1 }  }
	{ u_out int 64 regular {array 1024 { 0 3 } 0 1 }  }
	{ num_steps int 32 regular {axi_slave 0}  }
}
set hasAXIMCache 0
set l_AXIML2Cache [list]
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "u_in", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "u_out", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "num_steps", "interface" : "axi_slave", "bundle":"ctrl","type":"ap_none","bitwidth" : 32, "direction" : "READONLY", "offset" : {"in":16}, "offset_end" : {"in":23}} ]}
# RTL Port declarations: 
set portNum 30
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst_n sc_in sc_logic 1 reset -1 active_low_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ u_in_address0 sc_out sc_lv 10 signal 0 } 
	{ u_in_ce0 sc_out sc_logic 1 signal 0 } 
	{ u_in_q0 sc_in sc_lv 64 signal 0 } 
	{ u_out_address0 sc_out sc_lv 10 signal 1 } 
	{ u_out_ce0 sc_out sc_logic 1 signal 1 } 
	{ u_out_we0 sc_out sc_logic 1 signal 1 } 
	{ u_out_d0 sc_out sc_lv 64 signal 1 } 
	{ s_axi_ctrl_AWVALID sc_in sc_logic 1 signal -1 } 
	{ s_axi_ctrl_AWREADY sc_out sc_logic 1 signal -1 } 
	{ s_axi_ctrl_AWADDR sc_in sc_lv 5 signal -1 } 
	{ s_axi_ctrl_WVALID sc_in sc_logic 1 signal -1 } 
	{ s_axi_ctrl_WREADY sc_out sc_logic 1 signal -1 } 
	{ s_axi_ctrl_WDATA sc_in sc_lv 32 signal -1 } 
	{ s_axi_ctrl_WSTRB sc_in sc_lv 4 signal -1 } 
	{ s_axi_ctrl_ARVALID sc_in sc_logic 1 signal -1 } 
	{ s_axi_ctrl_ARREADY sc_out sc_logic 1 signal -1 } 
	{ s_axi_ctrl_ARADDR sc_in sc_lv 5 signal -1 } 
	{ s_axi_ctrl_RVALID sc_out sc_logic 1 signal -1 } 
	{ s_axi_ctrl_RREADY sc_in sc_logic 1 signal -1 } 
	{ s_axi_ctrl_RDATA sc_out sc_lv 32 signal -1 } 
	{ s_axi_ctrl_RRESP sc_out sc_lv 2 signal -1 } 
	{ s_axi_ctrl_BVALID sc_out sc_logic 1 signal -1 } 
	{ s_axi_ctrl_BREADY sc_in sc_logic 1 signal -1 } 
	{ s_axi_ctrl_BRESP sc_out sc_lv 2 signal -1 } 
}
set NewPortList {[ 
	{ "name": "s_axi_ctrl_AWADDR", "direction": "in", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "ctrl", "role": "AWADDR" },"address":[{"name":"num_steps","role":"data","value":"16"}] },
	{ "name": "s_axi_ctrl_AWVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "ctrl", "role": "AWVALID" } },
	{ "name": "s_axi_ctrl_AWREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "ctrl", "role": "AWREADY" } },
	{ "name": "s_axi_ctrl_WVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "ctrl", "role": "WVALID" } },
	{ "name": "s_axi_ctrl_WREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "ctrl", "role": "WREADY" } },
	{ "name": "s_axi_ctrl_WDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "ctrl", "role": "WDATA" } },
	{ "name": "s_axi_ctrl_WSTRB", "direction": "in", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "ctrl", "role": "WSTRB" } },
	{ "name": "s_axi_ctrl_ARADDR", "direction": "in", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "ctrl", "role": "ARADDR" },"address":[] },
	{ "name": "s_axi_ctrl_ARVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "ctrl", "role": "ARVALID" } },
	{ "name": "s_axi_ctrl_ARREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "ctrl", "role": "ARREADY" } },
	{ "name": "s_axi_ctrl_RVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "ctrl", "role": "RVALID" } },
	{ "name": "s_axi_ctrl_RREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "ctrl", "role": "RREADY" } },
	{ "name": "s_axi_ctrl_RDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "ctrl", "role": "RDATA" } },
	{ "name": "s_axi_ctrl_RRESP", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "ctrl", "role": "RRESP" } },
	{ "name": "s_axi_ctrl_BVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "ctrl", "role": "BVALID" } },
	{ "name": "s_axi_ctrl_BREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "ctrl", "role": "BREADY" } },
	{ "name": "s_axi_ctrl_BRESP", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "ctrl", "role": "BRESP" } }, 
 	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst_n", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst_n", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "u_in_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":10, "type": "signal", "bundle":{"name": "u_in", "role": "address0" }} , 
 	{ "name": "u_in_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_in", "role": "ce0" }} , 
 	{ "name": "u_in_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_in", "role": "q0" }} , 
 	{ "name": "u_out_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":10, "type": "signal", "bundle":{"name": "u_out", "role": "address0" }} , 
 	{ "name": "u_out_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_out", "role": "ce0" }} , 
 	{ "name": "u_out_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_out", "role": "we0" }} , 
 	{ "name": "u_out_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_out", "role": "d0" }}  ]}

set ArgLastReadFirstWriteLatency {
	heat2d_serial_hls {
		u_in {Type I LastRead 0 FirstWrite -1}
		u_out {Type O LastRead -1 FirstWrite 9}
		num_steps {Type I LastRead 1 FirstWrite -1}}
	heat2d_serial_hls_Pipeline_LOAD_I_LOAD_J {
		u_in {Type I LastRead 0 FirstWrite -1}
		u_a {Type O LastRead -1 FirstWrite 9}
		u_a_1 {Type O LastRead -1 FirstWrite 9}
		u_a_2 {Type O LastRead -1 FirstWrite 9}
		u_a_3 {Type O LastRead -1 FirstWrite 9}
		u_a_4 {Type O LastRead -1 FirstWrite 9}
		u_a_5 {Type O LastRead -1 FirstWrite 9}
		u_a_6 {Type O LastRead -1 FirstWrite 9}
		u_a_7 {Type O LastRead -1 FirstWrite 9}
		u_a_8 {Type O LastRead -1 FirstWrite 9}
		u_a_9 {Type O LastRead -1 FirstWrite 9}
		u_a_10 {Type O LastRead -1 FirstWrite 9}
		u_a_11 {Type O LastRead -1 FirstWrite 9}
		u_b {Type O LastRead -1 FirstWrite 1}
		u_b_1 {Type O LastRead -1 FirstWrite 1}
		u_b_2 {Type O LastRead -1 FirstWrite 1}
		u_b_3 {Type O LastRead -1 FirstWrite 1}}
	heat2d_serial_hls_Pipeline_STORE_I_STORE_J {
		u_out {Type O LastRead -1 FirstWrite 9}
		u_a {Type I LastRead 8 FirstWrite -1}
		u_a_1 {Type I LastRead 8 FirstWrite -1}
		u_a_2 {Type I LastRead 8 FirstWrite -1}
		u_a_3 {Type I LastRead 8 FirstWrite -1}
		u_a_4 {Type I LastRead 8 FirstWrite -1}
		u_a_5 {Type I LastRead 8 FirstWrite -1}
		u_a_6 {Type I LastRead 8 FirstWrite -1}
		u_a_7 {Type I LastRead 8 FirstWrite -1}
		u_a_8 {Type I LastRead 8 FirstWrite -1}
		u_a_9 {Type I LastRead 8 FirstWrite -1}
		u_a_10 {Type I LastRead 8 FirstWrite -1}
		u_a_11 {Type I LastRead 8 FirstWrite -1}}
	heat2d_serial_hls_Pipeline_SWAP_I_SWAP_J {
		u_a {Type O LastRead -1 FirstWrite 8}
		u_a_1 {Type O LastRead -1 FirstWrite 8}
		u_a_2 {Type O LastRead -1 FirstWrite 8}
		u_a_3 {Type O LastRead -1 FirstWrite 8}
		u_a_4 {Type O LastRead -1 FirstWrite 8}
		u_a_5 {Type O LastRead -1 FirstWrite 8}
		u_a_6 {Type O LastRead -1 FirstWrite 8}
		u_a_7 {Type O LastRead -1 FirstWrite 8}
		u_a_8 {Type O LastRead -1 FirstWrite 8}
		u_a_9 {Type O LastRead -1 FirstWrite 8}
		u_a_10 {Type O LastRead -1 FirstWrite 8}
		u_a_11 {Type O LastRead -1 FirstWrite 8}
		u_b {Type I LastRead 7 FirstWrite -1}
		u_b_1 {Type I LastRead 7 FirstWrite -1}
		u_b_2 {Type I LastRead 7 FirstWrite -1}
		u_b_3 {Type I LastRead 7 FirstWrite -1}}
	heat2d_serial_hls_Pipeline_STENCIL_J {
		mul_ln92 {Type I LastRead 0 FirstWrite -1}
		u_a {Type I LastRead 2 FirstWrite -1}
		mul_ln104 {Type I LastRead 0 FirstWrite -1}
		mul_ln93 {Type I LastRead 0 FirstWrite -1}
		u_a_1 {Type I LastRead 2 FirstWrite -1}
		u_a_2 {Type I LastRead 2 FirstWrite -1}
		u_a_3 {Type I LastRead 2 FirstWrite -1}
		u_a_4 {Type I LastRead 2 FirstWrite -1}
		u_a_5 {Type I LastRead 2 FirstWrite -1}
		u_a_6 {Type I LastRead 2 FirstWrite -1}
		u_a_7 {Type I LastRead 2 FirstWrite -1}
		u_a_8 {Type I LastRead 2 FirstWrite -1}
		u_a_9 {Type I LastRead 2 FirstWrite -1}
		u_a_10 {Type I LastRead 2 FirstWrite -1}
		u_a_11 {Type I LastRead 2 FirstWrite -1}
		i_1 {Type I LastRead 0 FirstWrite -1}
		u_b {Type O LastRead -1 FirstWrite 32}
		u_b_1 {Type O LastRead -1 FirstWrite 32}
		u_b_2 {Type O LastRead -1 FirstWrite 32}
		u_b_3 {Type O LastRead -1 FirstWrite 32}
		empty {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "5173", "Max" : "31022071"}
	, {"Name" : "Interval", "Min" : "5174", "Max" : "31022072"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	u_in { ap_memory {  { u_in_address0 mem_address 1 10 }  { u_in_ce0 mem_ce 1 1 }  { u_in_q0 mem_dout 0 64 } } }
	u_out { ap_memory {  { u_out_address0 mem_address 1 10 }  { u_out_ce0 mem_ce 1 1 }  { u_out_we0 mem_we 1 1 }  { u_out_d0 mem_din 1 64 } } }
}

set maxi_interface_dict [dict create]

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
