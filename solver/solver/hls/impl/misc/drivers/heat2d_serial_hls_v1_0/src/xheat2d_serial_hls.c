// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2025.2 (64-bit)
// Tool Version Limit: 2025.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xheat2d_serial_hls.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XHeat2d_serial_hls_CfgInitialize(XHeat2d_serial_hls *InstancePtr, XHeat2d_serial_hls_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Ctrl_BaseAddress = ConfigPtr->Ctrl_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XHeat2d_serial_hls_Set_num_steps(XHeat2d_serial_hls *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XHeat2d_serial_hls_WriteReg(InstancePtr->Ctrl_BaseAddress, XHEAT2D_SERIAL_HLS_CTRL_ADDR_NUM_STEPS_DATA, Data);
}

u32 XHeat2d_serial_hls_Get_num_steps(XHeat2d_serial_hls *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XHeat2d_serial_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XHEAT2D_SERIAL_HLS_CTRL_ADDR_NUM_STEPS_DATA);
    return Data;
}

