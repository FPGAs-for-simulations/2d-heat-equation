set ModuleHierarchy {[{
"Name" : "heat2d_serial_hls", "RefName" : "heat2d_serial_hls","ID" : "0","Type" : "sequential",
"SubInsts" : [
	{"Name" : "grp_heat2d_serial_hls_Pipeline_LOAD_I_LOAD_J_fu_154", "RefName" : "heat2d_serial_hls_Pipeline_LOAD_I_LOAD_J","ID" : "1","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "LOAD_I_LOAD_J","RefName" : "LOAD_I_LOAD_J","ID" : "2","Type" : "pipeline"},]},
	{"Name" : "grp_heat2d_serial_hls_Pipeline_STORE_I_STORE_J_fu_192", "RefName" : "heat2d_serial_hls_Pipeline_STORE_I_STORE_J","ID" : "3","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "STORE_I_STORE_J","RefName" : "STORE_I_STORE_J","ID" : "4","Type" : "pipeline"},]},],
"SubLoops" : [
	{"Name" : "TIME_LOOP","RefName" : "TIME_LOOP","ID" : "5","Type" : "no",
	"SubInsts" : [
	{"Name" : "grp_heat2d_serial_hls_Pipeline_SWAP_I_SWAP_J_fu_210", "RefName" : "heat2d_serial_hls_Pipeline_SWAP_I_SWAP_J","ID" : "6","Type" : "sequential",
			"SubLoops" : [
			{"Name" : "SWAP_I_SWAP_J","RefName" : "SWAP_I_SWAP_J","ID" : "7","Type" : "pipeline"},]},],
	"SubLoops" : [
	{"Name" : "STENCIL_I","RefName" : "STENCIL_I","ID" : "8","Type" : "no",
		"SubInsts" : [
		{"Name" : "grp_heat2d_serial_hls_Pipeline_STENCIL_J_fu_230", "RefName" : "heat2d_serial_hls_Pipeline_STENCIL_J","ID" : "9","Type" : "sequential",
				"SubLoops" : [
				{"Name" : "STENCIL_J","RefName" : "STENCIL_J","ID" : "10","Type" : "pipeline"},]},]},]},]
}]}