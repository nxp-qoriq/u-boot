/** @file cs_rtos.c
 ****************************************************************************
 *
 * @brief
 *    This contains all the RTOS(like system calls) and environment      *
 *    related macro's and stub utilities which should be modified or     *
 *    filled in as suited to the customer environment. It is important   *
 *    that this customization or porting of the driver is done BEFORE    *
 *    making any attempt to compile or use the driver.                   *
 *
 ****************************************************************************
 *  * @author
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
 *    Copyright (C) 2018 NXP Semiconductors 
 *
 *    API Version Number: 3.7.8
 ****************************************************************************/
#include "cs_rtos.h"
#include <phy.h>


// TODO: support for multiple dies 
static struct phy_device *cs4223_phydev;
static struct mii_dev *cs4223_bus;

/* Include any necessary library files when building the driver */
#ifndef CS_DONT_USE_STDLIB
#    include <stdlib.h>	       /* for malloc(), free(), abs() */
#    include <string.h>        /* for memcpy()                */
#    include <stdarg.h>        /* for variable args           */
#    include <stdio.h>         /* for printf variants         */
#    if !defined(_WINDOWS) && !defined(_WIN32) && !defined(_WIN64)
#        include <arpa/inet.h> /* for ntohs, htons            */
#    else
#        include <WinSock2.h>
#    endif
#endif /* CS_DONT_USE_STDLIB */

/* ANSI C doesn't declare these methods */
int usleep(unsigned int usecs);



void CS_UDELAY(int usecs)
{
	udelay(usecs);
}

void CS_MDELAY(int msecs)
{
    CS_UDELAY(msecs * 1000);
}

unsigned int CS_ABS(int value)
{
#ifdef CS_DONT_USE_STDLIB
    return (unsigned int) (value < 0 ? -value : value);
#else
    return (unsigned int) abs(value);
#endif
}

void *CS_MEMSET(void *p, int c, int n)
{
  char *pb = (char *) p;
  char *pbend = pb + n;
  while (pb != pbend) *pb++ = c;
  return p;
}


void *CS_MEMCPY(void *dst, const void *src, int n)
{
#ifdef CS_DONT_USE_STDLIB
    void *ret = dst;

    while (n--)
    {
      *(char *)dst = *(char *)src;
      dst = (char *) dst + 1;
      src = (char *) src + 1;
    }

    return ret;
#else
    return memcpy(dst, src, n);
#endif
}


int CS_STRLEN(const char *s)
{
  const char *eos = s;
  while (*eos++);
  return (int) (eos - s - 1);
}


char *CS_STRCAT(char *dest, const char *source)
{ 
  char *d = dest;
  while (*d) ++d;
  while ((*d++ = *source++) != '\0') ;
  return (dest);
}


char *CS_STRNCPY(char *dest, const char *source, int count)
{
  char *start = dest;

  while (count && (*dest++ = *source++)) count--;
  if (count) while (--count) *dest++ = '\0';
  return start;
}

#ifndef CS_DONT_USE_STDLIB
#ifndef CS_TRACE_FUNCTION
/** Don't use this function directly, use CS_TRACE((x)) instead
 * @private
 */
void CS_TRACE_FUNCTION(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(CS_TRACE_STREAM, fmt, ap);
    va_end(ap);
}
#endif
#endif

void cs4223_glue_phydev_set(struct phy_device *phydev)
{
	cs4223_phydev = phydev;
	cs4223_bus = phydev->bus;
}

cs_status cs4224_reg_set(cs_uint32 slice, cs_uint32 addr, cs_uint16 data)
{
    cs4223_bus->write(cs4223_bus, 0, 0, addr, data);
    return CS_OK;
}

cs_status cs4224_reg_get(cs_uint32 slice, cs_uint32 addr, cs_uint16* data)
{

    *data = cs4223_bus->read(cs4223_bus, 0, 0, addr);

    return CS_OK;
}
