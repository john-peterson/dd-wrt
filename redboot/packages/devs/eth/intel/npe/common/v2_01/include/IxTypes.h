/**
 * @file IxTypes.h (Replaced by OSAL)
 *
 * @date 28-NOV-2001

 * @brief This file contains basic types used by the IXP400 software
 *
 * Design Notes:
 *    This file shall only include fundamental types and definitions to be
 *    shared by all the IXP400 components.
 *    Please DO NOT add component-specific types here.
 *
 * 
 * @par
 * IXP400 SW Release version  2.0
 * 
 * -- Intel Copyright Notice --
 * 
 * @par
 * Copyright 2002-2005 Intel Corporation All Rights Reserved.
 * 
 * @par
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel Corporation
 * or its suppliers or licensors.  Title to the Material remains with
 * Intel Corporation or its suppliers and licensors.
 * 
 * @par
 * The Material is protected by worldwide copyright and trade secret laws
 * and treaty provisions. No part of the Material may be used, copied,
 * reproduced, modified, published, uploaded, posted, transmitted,
 * distributed, or disclosed in any way except in accordance with the
 * applicable license agreement .
 * 
 * @par
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you by
 * disclosure or delivery of the Materials, either expressly, by
 * implication, inducement, estoppel, except in accordance with the
 * applicable license agreement.
 * 
 * @par
 * Unless otherwise agreed by Intel in writing, you may not remove or
 * alter this notice or any other notice embedded in Materials by Intel
 * or Intel's suppliers or licensors in any way.
 * 
 * @par
 * For further details, please see the file README.TXT distributed with
 * this software.
 * 
 * @par
 * -- End Intel Copyright Notice --
 */

/**
 * @defgroup IxTypes IXP400 Types (IxTypes) 
 *
 * @brief Basic data types used by the IXP400 project
 *
 * @{
 */

#ifndef IxTypes_H

#ifndef __doxygen_HIDE

#define IxTypes_H

#endif /* __doxygen_HIDE */


/* WR51880: Undefined data types workaround for backward compatibility */
#ifdef __linux
#ifndef __INCvxTypesOldh
typedef int (*FUNCPTR)(void);
typedef int STATUS;
#define OK (0)
#define ERROR (-1)
#endif
#endif

#include "IxOsalBackward.h"

#endif /* IxTypes_H */

/**
 * @} addtogroup IxTypes
 */
