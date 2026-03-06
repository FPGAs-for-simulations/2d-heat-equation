set moduleName heat2d_serial_hls_Pipeline_LOAD_I_LOAD_J
set isTopModule 0
set isCombinational 0
set isDatapathOnly 0
set isPipelined 1
set isPipelined_legacy 1
set pipeline_type loop_auto_rewind
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
set C_modelName {heat2d_serial_hls_Pipeline_LOAD_I_LOAD_J}
set C_modelType { void 0 }
set ap_memory_interface_dict [dict create]
dict set ap_memory_interface_dict u_in { MEM_WIDTH 64 MEM_SIZE 8192 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict u_a { MEM_WIDTH 64 MEM_SIZE 704 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_a_1 { MEM_WIDTH 64 MEM_SIZE 704 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_a_2 { MEM_WIDTH 64 MEM_SIZE 704 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_a_3 { MEM_WIDTH 64 MEM_SIZE 704 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_a_4 { MEM_WIDTH 64 MEM_SIZE 704 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_a_5 { MEM_WIDTH 64 MEM_SIZE 704 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_a_6 { MEM_WIDTH 64 MEM_SIZE 704 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_a_7 { MEM_WIDTH 64 MEM_SIZE 704 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_a_8 { MEM_WIDTH 64 MEM_SIZE 704 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_a_9 { MEM_WIDTH 64 MEM_SIZE 704 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_a_10 { MEM_WIDTH 64 MEM_SIZE 704 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_a_11 { MEM_WIDTH 64 MEM_SIZE 704 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_b { MEM_WIDTH 64 MEM_SIZE 2048 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_b_1 { MEM_WIDTH 64 MEM_SIZE 2048 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_b_2 { MEM_WIDTH 64 MEM_SIZE 2048 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict u_b_3 { MEM_WIDTH 64 MEM_SIZE 2048 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
set C_modelArgList {
	{ u_in int 64 regular {array 1024 { 1 3 } 1 1 }  }
	{ u_a double 64 regular {array 88 { 0 3 } 0 1 }  }
	{ u_a_1 double 64 regular {array 88 { 0 3 } 0 1 }  }
	{ u_a_2 double 64 regular {array 88 { 0 3 } 0 1 }  }
	{ u_a_3 double 64 regular {array 88 { 0 3 } 0 1 }  }
	{ u_a_4 double 64 regular {array 88 { 0 3 } 0 1 }  }
	{ u_a_5 double 64 regular {array 88 { 0 3 } 0 1 }  }
	{ u_a_6 double 64 regular {array 88 { 0 3 } 0 1 }  }
	{ u_a_7 double 64 regular {array 88 { 0 3 } 0 1 }  }
	{ u_a_8 double 64 regular {array 88 { 0 3 } 0 1 }  }
	{ u_a_9 double 64 regular {array 88 { 0 3 } 0 1 }  }
	{ u_a_10 double 64 regular {array 88 { 0 3 } 0 1 }  }
	{ u_a_11 double 64 regular {array 88 { 0 3 } 0 1 }  }
	{ u_b double 64 regular {array 256 { 0 3 } 0 1 }  }
	{ u_b_1 double 64 regular {array 256 { 0 3 } 0 1 }  }
	{ u_b_2 double 64 regular {array 256 { 0 3 } 0 1 }  }
	{ u_b_3 double 64 regular {array 256 { 0 3 } 0 1 }  }
}
set hasAXIMCache 0
set l_AXIML2Cache [list]
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "u_in", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "u_a", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_a_1", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_a_2", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_a_3", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_a_4", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_a_5", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_a_6", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_a_7", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_a_8", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_a_9", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_a_10", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_a_11", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_b", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_b_1", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_b_2", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "u_b_3", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} ]}
# RTL Port declarations: 
set portNum 73
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ u_in_address0 sc_out sc_lv 10 signal 0 } 
	{ u_in_ce0 sc_out sc_logic 1 signal 0 } 
	{ u_in_q0 sc_in sc_lv 64 signal 0 } 
	{ u_a_address0 sc_out sc_lv 7 signal 1 } 
	{ u_a_ce0 sc_out sc_logic 1 signal 1 } 
	{ u_a_we0 sc_out sc_logic 1 signal 1 } 
	{ u_a_d0 sc_out sc_lv 64 signal 1 } 
	{ u_a_1_address0 sc_out sc_lv 7 signal 2 } 
	{ u_a_1_ce0 sc_out sc_logic 1 signal 2 } 
	{ u_a_1_we0 sc_out sc_logic 1 signal 2 } 
	{ u_a_1_d0 sc_out sc_lv 64 signal 2 } 
	{ u_a_2_address0 sc_out sc_lv 7 signal 3 } 
	{ u_a_2_ce0 sc_out sc_logic 1 signal 3 } 
	{ u_a_2_we0 sc_out sc_logic 1 signal 3 } 
	{ u_a_2_d0 sc_out sc_lv 64 signal 3 } 
	{ u_a_3_address0 sc_out sc_lv 7 signal 4 } 
	{ u_a_3_ce0 sc_out sc_logic 1 signal 4 } 
	{ u_a_3_we0 sc_out sc_logic 1 signal 4 } 
	{ u_a_3_d0 sc_out sc_lv 64 signal 4 } 
	{ u_a_4_address0 sc_out sc_lv 7 signal 5 } 
	{ u_a_4_ce0 sc_out sc_logic 1 signal 5 } 
	{ u_a_4_we0 sc_out sc_logic 1 signal 5 } 
	{ u_a_4_d0 sc_out sc_lv 64 signal 5 } 
	{ u_a_5_address0 sc_out sc_lv 7 signal 6 } 
	{ u_a_5_ce0 sc_out sc_logic 1 signal 6 } 
	{ u_a_5_we0 sc_out sc_logic 1 signal 6 } 
	{ u_a_5_d0 sc_out sc_lv 64 signal 6 } 
	{ u_a_6_address0 sc_out sc_lv 7 signal 7 } 
	{ u_a_6_ce0 sc_out sc_logic 1 signal 7 } 
	{ u_a_6_we0 sc_out sc_logic 1 signal 7 } 
	{ u_a_6_d0 sc_out sc_lv 64 signal 7 } 
	{ u_a_7_address0 sc_out sc_lv 7 signal 8 } 
	{ u_a_7_ce0 sc_out sc_logic 1 signal 8 } 
	{ u_a_7_we0 sc_out sc_logic 1 signal 8 } 
	{ u_a_7_d0 sc_out sc_lv 64 signal 8 } 
	{ u_a_8_address0 sc_out sc_lv 7 signal 9 } 
	{ u_a_8_ce0 sc_out sc_logic 1 signal 9 } 
	{ u_a_8_we0 sc_out sc_logic 1 signal 9 } 
	{ u_a_8_d0 sc_out sc_lv 64 signal 9 } 
	{ u_a_9_address0 sc_out sc_lv 7 signal 10 } 
	{ u_a_9_ce0 sc_out sc_logic 1 signal 10 } 
	{ u_a_9_we0 sc_out sc_logic 1 signal 10 } 
	{ u_a_9_d0 sc_out sc_lv 64 signal 10 } 
	{ u_a_10_address0 sc_out sc_lv 7 signal 11 } 
	{ u_a_10_ce0 sc_out sc_logic 1 signal 11 } 
	{ u_a_10_we0 sc_out sc_logic 1 signal 11 } 
	{ u_a_10_d0 sc_out sc_lv 64 signal 11 } 
	{ u_a_11_address0 sc_out sc_lv 7 signal 12 } 
	{ u_a_11_ce0 sc_out sc_logic 1 signal 12 } 
	{ u_a_11_we0 sc_out sc_logic 1 signal 12 } 
	{ u_a_11_d0 sc_out sc_lv 64 signal 12 } 
	{ u_b_address0 sc_out sc_lv 8 signal 13 } 
	{ u_b_ce0 sc_out sc_logic 1 signal 13 } 
	{ u_b_we0 sc_out sc_logic 1 signal 13 } 
	{ u_b_d0 sc_out sc_lv 64 signal 13 } 
	{ u_b_1_address0 sc_out sc_lv 8 signal 14 } 
	{ u_b_1_ce0 sc_out sc_logic 1 signal 14 } 
	{ u_b_1_we0 sc_out sc_logic 1 signal 14 } 
	{ u_b_1_d0 sc_out sc_lv 64 signal 14 } 
	{ u_b_2_address0 sc_out sc_lv 8 signal 15 } 
	{ u_b_2_ce0 sc_out sc_logic 1 signal 15 } 
	{ u_b_2_we0 sc_out sc_logic 1 signal 15 } 
	{ u_b_2_d0 sc_out sc_lv 64 signal 15 } 
	{ u_b_3_address0 sc_out sc_lv 8 signal 16 } 
	{ u_b_3_ce0 sc_out sc_logic 1 signal 16 } 
	{ u_b_3_we0 sc_out sc_logic 1 signal 16 } 
	{ u_b_3_d0 sc_out sc_lv 64 signal 16 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "u_in_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":10, "type": "signal", "bundle":{"name": "u_in", "role": "address0" }} , 
 	{ "name": "u_in_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_in", "role": "ce0" }} , 
 	{ "name": "u_in_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_in", "role": "q0" }} , 
 	{ "name": "u_a_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":7, "type": "signal", "bundle":{"name": "u_a", "role": "address0" }} , 
 	{ "name": "u_a_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a", "role": "ce0" }} , 
 	{ "name": "u_a_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a", "role": "we0" }} , 
 	{ "name": "u_a_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_a", "role": "d0" }} , 
 	{ "name": "u_a_1_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":7, "type": "signal", "bundle":{"name": "u_a_1", "role": "address0" }} , 
 	{ "name": "u_a_1_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_1", "role": "ce0" }} , 
 	{ "name": "u_a_1_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_1", "role": "we0" }} , 
 	{ "name": "u_a_1_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_a_1", "role": "d0" }} , 
 	{ "name": "u_a_2_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":7, "type": "signal", "bundle":{"name": "u_a_2", "role": "address0" }} , 
 	{ "name": "u_a_2_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_2", "role": "ce0" }} , 
 	{ "name": "u_a_2_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_2", "role": "we0" }} , 
 	{ "name": "u_a_2_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_a_2", "role": "d0" }} , 
 	{ "name": "u_a_3_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":7, "type": "signal", "bundle":{"name": "u_a_3", "role": "address0" }} , 
 	{ "name": "u_a_3_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_3", "role": "ce0" }} , 
 	{ "name": "u_a_3_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_3", "role": "we0" }} , 
 	{ "name": "u_a_3_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_a_3", "role": "d0" }} , 
 	{ "name": "u_a_4_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":7, "type": "signal", "bundle":{"name": "u_a_4", "role": "address0" }} , 
 	{ "name": "u_a_4_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_4", "role": "ce0" }} , 
 	{ "name": "u_a_4_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_4", "role": "we0" }} , 
 	{ "name": "u_a_4_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_a_4", "role": "d0" }} , 
 	{ "name": "u_a_5_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":7, "type": "signal", "bundle":{"name": "u_a_5", "role": "address0" }} , 
 	{ "name": "u_a_5_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_5", "role": "ce0" }} , 
 	{ "name": "u_a_5_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_5", "role": "we0" }} , 
 	{ "name": "u_a_5_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_a_5", "role": "d0" }} , 
 	{ "name": "u_a_6_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":7, "type": "signal", "bundle":{"name": "u_a_6", "role": "address0" }} , 
 	{ "name": "u_a_6_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_6", "role": "ce0" }} , 
 	{ "name": "u_a_6_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_6", "role": "we0" }} , 
 	{ "name": "u_a_6_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_a_6", "role": "d0" }} , 
 	{ "name": "u_a_7_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":7, "type": "signal", "bundle":{"name": "u_a_7", "role": "address0" }} , 
 	{ "name": "u_a_7_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_7", "role": "ce0" }} , 
 	{ "name": "u_a_7_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_7", "role": "we0" }} , 
 	{ "name": "u_a_7_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_a_7", "role": "d0" }} , 
 	{ "name": "u_a_8_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":7, "type": "signal", "bundle":{"name": "u_a_8", "role": "address0" }} , 
 	{ "name": "u_a_8_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_8", "role": "ce0" }} , 
 	{ "name": "u_a_8_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_8", "role": "we0" }} , 
 	{ "name": "u_a_8_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_a_8", "role": "d0" }} , 
 	{ "name": "u_a_9_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":7, "type": "signal", "bundle":{"name": "u_a_9", "role": "address0" }} , 
 	{ "name": "u_a_9_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_9", "role": "ce0" }} , 
 	{ "name": "u_a_9_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_9", "role": "we0" }} , 
 	{ "name": "u_a_9_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_a_9", "role": "d0" }} , 
 	{ "name": "u_a_10_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":7, "type": "signal", "bundle":{"name": "u_a_10", "role": "address0" }} , 
 	{ "name": "u_a_10_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_10", "role": "ce0" }} , 
 	{ "name": "u_a_10_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_10", "role": "we0" }} , 
 	{ "name": "u_a_10_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_a_10", "role": "d0" }} , 
 	{ "name": "u_a_11_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":7, "type": "signal", "bundle":{"name": "u_a_11", "role": "address0" }} , 
 	{ "name": "u_a_11_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_11", "role": "ce0" }} , 
 	{ "name": "u_a_11_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_a_11", "role": "we0" }} , 
 	{ "name": "u_a_11_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_a_11", "role": "d0" }} , 
 	{ "name": "u_b_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "u_b", "role": "address0" }} , 
 	{ "name": "u_b_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_b", "role": "ce0" }} , 
 	{ "name": "u_b_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_b", "role": "we0" }} , 
 	{ "name": "u_b_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_b", "role": "d0" }} , 
 	{ "name": "u_b_1_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "u_b_1", "role": "address0" }} , 
 	{ "name": "u_b_1_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_b_1", "role": "ce0" }} , 
 	{ "name": "u_b_1_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_b_1", "role": "we0" }} , 
 	{ "name": "u_b_1_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_b_1", "role": "d0" }} , 
 	{ "name": "u_b_2_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "u_b_2", "role": "address0" }} , 
 	{ "name": "u_b_2_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_b_2", "role": "ce0" }} , 
 	{ "name": "u_b_2_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_b_2", "role": "we0" }} , 
 	{ "name": "u_b_2_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_b_2", "role": "d0" }} , 
 	{ "name": "u_b_3_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "u_b_3", "role": "address0" }} , 
 	{ "name": "u_b_3_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_b_3", "role": "ce0" }} , 
 	{ "name": "u_b_3_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "u_b_3", "role": "we0" }} , 
 	{ "name": "u_b_3_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "u_b_3", "role": "d0" }}  ]}

set ArgLastReadFirstWriteLatency {
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
		u_b_3 {Type O LastRead -1 FirstWrite 1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "1034", "Max" : "1034"}
	, {"Name" : "Interval", "Min" : "1025", "Max" : "1025"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	u_in { ap_memory {  { u_in_address0 mem_address 1 10 }  { u_in_ce0 mem_ce 1 1 }  { u_in_q0 mem_dout 0 64 } } }
	u_a { ap_memory {  { u_a_address0 mem_address 1 7 }  { u_a_ce0 mem_ce 1 1 }  { u_a_we0 mem_we 1 1 }  { u_a_d0 mem_din 1 64 } } }
	u_a_1 { ap_memory {  { u_a_1_address0 mem_address 1 7 }  { u_a_1_ce0 mem_ce 1 1 }  { u_a_1_we0 mem_we 1 1 }  { u_a_1_d0 mem_din 1 64 } } }
	u_a_2 { ap_memory {  { u_a_2_address0 mem_address 1 7 }  { u_a_2_ce0 mem_ce 1 1 }  { u_a_2_we0 mem_we 1 1 }  { u_a_2_d0 mem_din 1 64 } } }
	u_a_3 { ap_memory {  { u_a_3_address0 mem_address 1 7 }  { u_a_3_ce0 mem_ce 1 1 }  { u_a_3_we0 mem_we 1 1 }  { u_a_3_d0 mem_din 1 64 } } }
	u_a_4 { ap_memory {  { u_a_4_address0 mem_address 1 7 }  { u_a_4_ce0 mem_ce 1 1 }  { u_a_4_we0 mem_we 1 1 }  { u_a_4_d0 mem_din 1 64 } } }
	u_a_5 { ap_memory {  { u_a_5_address0 mem_address 1 7 }  { u_a_5_ce0 mem_ce 1 1 }  { u_a_5_we0 mem_we 1 1 }  { u_a_5_d0 mem_din 1 64 } } }
	u_a_6 { ap_memory {  { u_a_6_address0 mem_address 1 7 }  { u_a_6_ce0 mem_ce 1 1 }  { u_a_6_we0 mem_we 1 1 }  { u_a_6_d0 mem_din 1 64 } } }
	u_a_7 { ap_memory {  { u_a_7_address0 mem_address 1 7 }  { u_a_7_ce0 mem_ce 1 1 }  { u_a_7_we0 mem_we 1 1 }  { u_a_7_d0 mem_din 1 64 } } }
	u_a_8 { ap_memory {  { u_a_8_address0 mem_address 1 7 }  { u_a_8_ce0 mem_ce 1 1 }  { u_a_8_we0 mem_we 1 1 }  { u_a_8_d0 mem_din 1 64 } } }
	u_a_9 { ap_memory {  { u_a_9_address0 mem_address 1 7 }  { u_a_9_ce0 mem_ce 1 1 }  { u_a_9_we0 mem_we 1 1 }  { u_a_9_d0 mem_din 1 64 } } }
	u_a_10 { ap_memory {  { u_a_10_address0 mem_address 1 7 }  { u_a_10_ce0 mem_ce 1 1 }  { u_a_10_we0 mem_we 1 1 }  { u_a_10_d0 mem_din 1 64 } } }
	u_a_11 { ap_memory {  { u_a_11_address0 mem_address 1 7 }  { u_a_11_ce0 mem_ce 1 1 }  { u_a_11_we0 mem_we 1 1 }  { u_a_11_d0 mem_din 1 64 } } }
	u_b { ap_memory {  { u_b_address0 mem_address 1 8 }  { u_b_ce0 mem_ce 1 1 }  { u_b_we0 mem_we 1 1 }  { u_b_d0 mem_din 1 64 } } }
	u_b_1 { ap_memory {  { u_b_1_address0 mem_address 1 8 }  { u_b_1_ce0 mem_ce 1 1 }  { u_b_1_we0 mem_we 1 1 }  { u_b_1_d0 mem_din 1 64 } } }
	u_b_2 { ap_memory {  { u_b_2_address0 mem_address 1 8 }  { u_b_2_ce0 mem_ce 1 1 }  { u_b_2_we0 mem_we 1 1 }  { u_b_2_d0 mem_din 1 64 } } }
	u_b_3 { ap_memory {  { u_b_3_address0 mem_address 1 8 }  { u_b_3_ce0 mem_ce 1 1 }  { u_b_3_we0 mem_we 1 1 }  { u_b_3_d0 mem_din 1 64 } } }
}
