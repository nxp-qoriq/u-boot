#ifndef __CS_CONFIG_H__
#define __CS_CONFIG_H__
/** @file cs_config.h
 ****************************************************************************
 *
 * @brief
 *     This module allows individual features in the API to be compiled
 *     in or out to manage code space.
 *
 ****************************************************************************
 * @author
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * 
 *    Copyright (C) 2006-2015 Inphi Corporation, Inc. All rights reserved.
 *
 *    API Version Number: 3.7.8
 ***************************************************************************/

/* Set to enable compile in debug features such as register dump */
#define CS_HAS_DEBUG_LOOPBACKS      1
#define CS_HAS_DEBUG_PRBS           1
#define CS_HAS_DEBUG_REGISTER_DUMPS 1
#define CS_HAS_DEBUG_STATUS_DUMPS   1

/* Set to define where the CS_TRACE output goes (stderr or stdout)
 * This is ignored if CS_DONT_USE_STDLIB is defined */
#define CS_TRACE_STREAM stdout

/* Set to allow the use of floating-point math */
#define CS_HAS_FLOATING_POINT 

/* Set to include the Interrupt handling code */
/* #define CS_HAS_INTERRUPTS  */

/* Set to not use stdlib */
/* #define CS_DONT_USE_STDLIB */

/* Set to not use inlining */
/* #define CS_DONT_USE_INLINE */

/* Set to include filesystem  */
/* #define CS_HAS_FILESYSTEM  */

/* Set to not load the ucode automatically (dangerous) */
/* #define CS_SKIP_UCODE_DOWNLOAD 1 */

/* Set to enable experimental maddr masking (dangerous) */
/* #define CS_MULTI_CHIP_UCODE_PRGM */

#endif /* __CS_CONFIG_H__ */
