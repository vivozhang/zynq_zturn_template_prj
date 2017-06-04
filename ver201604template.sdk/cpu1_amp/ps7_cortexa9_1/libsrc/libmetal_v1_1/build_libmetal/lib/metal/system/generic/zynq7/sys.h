/*
 * Copyright (c) 2015, Xilinx Inc. and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Xilinx nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * @file	generic/zynq7/sys.h
 * @brief	generic zynq7 system primitives for libmetal.
 */

#ifndef __METAL_GENERIC_SYS__H__
#error "Include metal/sys.h instead of metal/generic/zynq7/sys.h"
#endif

#include "xscugic.h"

#ifndef __METAL_GENERIC_ZYNQ7_SYS__H__
#define __METAL_GENERIC_ZYNQ7_SYS__H__

#ifdef __cplusplus
extern "C" {
#endif


#if !defined(MAX_IRQS)
#define MAX_IRQS	((int)XSCUGIC_MAX_NUM_INTR_INPUTS)          /**< maximum number of irqs */
#endif

static inline void sys_irq_enable(unsigned int vector)
{
        XScuGic_EnableIntr(XPAR_SCUGIC_0_DIST_BASEADDR, vector);
}

static inline void sys_irq_disable(unsigned int vector)
{
        XScuGic_DisableIntr(XPAR_SCUGIC_0_DIST_BASEADDR, vector);
}


#ifdef __cplusplus
}
#endif

#endif /* __METAL_GENERIC_ZYNQ7_SYS__H__ */