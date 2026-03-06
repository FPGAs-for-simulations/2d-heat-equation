// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2025.2 (64-bit)
// Tool Version Limit: 2025.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XHEAT2D_SERIAL_HLS_H
#define XHEAT2D_SERIAL_HLS_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xheat2d_serial_hls_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
#ifdef SDT
    char *Name;
#else
    u16 DeviceId;
#endif
    u64 Ctrl_BaseAddress;
} XHeat2d_serial_hls_Config;
#endif

typedef struct {
    u64 Ctrl_BaseAddress;
    u32 IsReady;
} XHeat2d_serial_hls;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XHeat2d_serial_hls_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XHeat2d_serial_hls_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XHeat2d_serial_hls_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XHeat2d_serial_hls_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
#ifdef SDT
int XHeat2d_serial_hls_Initialize(XHeat2d_serial_hls *InstancePtr, UINTPTR BaseAddress);
XHeat2d_serial_hls_Config* XHeat2d_serial_hls_LookupConfig(UINTPTR BaseAddress);
#else
int XHeat2d_serial_hls_Initialize(XHeat2d_serial_hls *InstancePtr, u16 DeviceId);
XHeat2d_serial_hls_Config* XHeat2d_serial_hls_LookupConfig(u16 DeviceId);
#endif
int XHeat2d_serial_hls_CfgInitialize(XHeat2d_serial_hls *InstancePtr, XHeat2d_serial_hls_Config *ConfigPtr);
#else
int XHeat2d_serial_hls_Initialize(XHeat2d_serial_hls *InstancePtr, const char* InstanceName);
int XHeat2d_serial_hls_Release(XHeat2d_serial_hls *InstancePtr);
#endif


void XHeat2d_serial_hls_Set_num_steps(XHeat2d_serial_hls *InstancePtr, u32 Data);
u32 XHeat2d_serial_hls_Get_num_steps(XHeat2d_serial_hls *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
