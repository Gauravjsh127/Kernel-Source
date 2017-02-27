/*
 * Copyright (c) International Business Machines Corp., 2007,2010
 *
 * This program is free software;  you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;  without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;  if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
/* Module Specification ***********************************************/
/*                                                                    */
/*  NAME: rtc.h (REAL TIME CLOCK DCR address map)                     */
/*                                                                    */
/*  AUTHOR: Tom Warne, Dean Sanner                                    */
/*                                                                    */
/*  KEY REVIEWER:                                                     */
/*                                                                    */
/*  Description: The FSP0 Base embedded Real Time Clock (RTC) core    */
/*  resides in the Function 0 BAR 0 space of FSP0 Base. The RTC has   */
/*  three time-of-day registers, three alarm registers and four       */
/*  calendar registers all of which are accessible by an 8-bit CPU bus*/
/*                                                                    */
/* End Module Specification *******************************************/

#ifndef __FSP1_RTC
#define __FSP1_RTC

#include <linux/types.h>
#include <linux/rtc.h>
#include <asm/dcr.h>
#include <asm/reg.h>

void ibmfsp1_get_rtc_time(struct rtc_time *);
int ibmfsp1_set_rtc_time(struct rtc_time *);
void ibmfsp1_start_timecal_sync(void);
int ibmfsp1_is_timecal_valid(void);

/**********************************************************************/
/* DEFINITIONS                                                        */
/**********************************************************************/
struct rtc_reg {
		uint8_t rsvd[3];
		uint8_t byte;
};
typedef struct rtc_reg rtc_reg_t;

union rtc_reg_A {
	uint8_t reg;
	struct {
		uint8_t uip : 1,
		div_ctl     : 3,
		rate_select : 4;
	}bits;
};
typedef union rtc_reg_A rtc_reg_A_t;

/* Bit masks for whole byte values. */
#define RTC_REGA_UIP_BIT           0x80
#define RTC_REGA_DIV_CTL_MASK      0x70
#define RTC_REGA_REF_CLCK_4MHZ     0x00
#define RTC_REGA_REF_CLCK_1MHZ     0x10
#define RTC_REGA_REF_CLCK_32KHZ    0x20
#define RTC_REGA_DIV_RESET         0x60
#define RTC_REGA_RATE_SELECT_MASK  0x0F
/* Definitions for div_ctl bit field. */
#define RTC_REGA_BF_REF_CLCK_4MHZ  0x0
#define RTC_REGA_BF_REF_CLCK_1MHZ  0x1
#define RTC_REGA_BF_REF_CLCK_32KHZ 0x2
#define RTC_REGA_BF_DIV_RESET      0x6

union rtc_reg_B {
	uint8_t reg;
	struct {
		uint8_t set  : 1,
		pie  : 1,
		aie  : 1,
		uie  : 1,
		sqwe : 1,
		dm   : 1,
		hr24 : 1,
		dse  : 1;
	}bits;
};
typedef union rtc_reg_B rtc_reg_B_t;

/* Bit masks for whole byte values. */
#define RTC_REGB_SET             0x80
#define RTC_REGB_INT_ENABLE_MASK 0x78
#define RTC_REGB_INT_CHK_MASK    0xF8
#define RTC_REGB_PIE             0x40
#define RTC_REGB_AIE             0x20
#define RTC_REGB_UIE             0x10
#define RTC_REGB_SQWE            0x08
#define RTC_REGB_DM              0x04
#define RTC_REGB_24HR            0x02
#define RTC_REGB_DSE             0x01


union rtc_reg_C {
	uint8_t reg;
	struct {
		uint8_t irqf  : 1,
		pf    : 1,
		af    : 1,
		uf    : 1,
		rsvd2 : 4;
	}bits;
};
typedef union rtc_reg_C rtc_reg_C_t;
/* Bit masks for whole byte values. */
#define RTC_REGC_IQRF  0x80
#define RTC_REGC_PF    0x40
#define RTC_REGC_AF    0x20
#define RTC_REGC_UF    0x10


union rtc_reg_D {
	uint8_t reg;
	struct {
		uint8_t vrt   : 1,
		rsvd  : 7;
	}bits;
};
typedef union rtc_reg_D rtc_reg_D_t;

/* Bit masks for whole byte values. */
#define RTC_REGD_VRT   0x80


union rtc_wrapper_cntl {
	uint32_t reg;
	struct {
		uint32_t rsvd   : 22,
		vtc    : 1,
		ts     : 1,
		synctc : 1,
		euia   : 1,
		asp    : 1,
		nrdp   : 1,
		um     : 1,
		fence  : 1,
		nrst   : 1,
		nmr    : 1;
	}bits;
};
typedef union rtc_wrapper_cntl rtc_wrapper_cntl_t;

/* Bit masks for whole word value. */
#define RTC_WCNTL_NMR    0x00000001
#define RTC_WCNTL_NRST   0x00000002
#define RTC_WCNTL_FENCE  0x00000004
#define RTC_WCNTL_UM     0x00000008
#define RTC_WCNTL_NRDP   0x00000010
#define RTC_WCNTL_ASP    0x00000020
#define RTC_WCNTL_EUIA   0x00000040
#define RTC_WCNTL_SYNCTC 0x00000080
#define RTC_WCNTL_TS     0x00000100
#define RTC_WCNTL_VTC    0x00000200
#define RTC_WCNTL_RSVD   0xFFFFFC00

union rtc64_hi_dd10 {
	uint32_t reg;
	struct {
		uint32_t rsvd    : 6,
		century : 7,
		year    : 7,
		month   : 4,
		day     : 5,
		wday	: 3;
	} bits;
};
typedef union rtc64_hi_dd10 rtc64_hi_dd10_t;

union rtc64_hi {
	uint32_t reg;
	struct {
		uint32_t rsvd    : 9,
		century : 7,
		year    : 7,
		month   : 4,
		day     : 5;
	} bits;
};
typedef union rtc64_hi rtc64_hi_t;

union rtc64_lo {
	uint32_t reg;
	struct {
		uint32_t hours      : 5,
		minutes    : 6,
		seconds    : 6,
		one32768th : 15;
	} bits;
};
typedef union rtc64_lo rtc64_lo_t;

union rtc64_alrm_cntl {
	uint32_t reg;
	struct {
		uint32_t alarm_0_int  : 1,
		alarm_1_int  : 1,
		alarm_2_int  : 1,
		rsvd_bit28   : 1,
		alarm_0_cmpr : 1,
		alarm_1_cmpr : 1,
		alarm_2_cmpr : 1,
		rsvd_24_to_7 : 18,
		alarm_0_int_enbl : 1,
		alarm_1_int_enbl : 1,
		alarm_2_int_enbl : 1,
		rsvd_bit3    : 1,
		alarm_0_enbl : 1,
		alarm_1_enbl : 1,
		alarm_2_enbl : 1;
	} bits;
};
typedef union rtc64_alrm_cntl rtc64_alrm_cntl_t;

#define RTC64_ALRM0_IRQ			0x80000000		/* Bit 31 */
#define RTC64_ALRM1_IRQ			0x40000000		/* Bit 30 */
#define RTC64_ALRM2_IRQ			0x20000000		/* But 29 */
#define RTC64_RW_ALARM_BITS		0x00000077
#define RTC64_ALRM0_ENABLE		0x00000004
#define RTC64_ALRM1_ENABLE		0x00000002
#define RTC64_ALRM2_ENABLE		0x00000001
#define RTC64_ALRM0_INT_ENABLE	0x00000040
#define RTC64_ALRM1_INT_ENABLE	0x00000020
#define RTC64_ALRM2_INT_ENABLE	0x00000010


/* All hardware registers, used for data capture. */
struct rtc_hw_regs {
		uint8_t  seconds;
		uint8_t  seconds_alarm;
		uint8_t  minutes;
		uint8_t  minutes_alarm;
		uint8_t  hours;
		uint8_t  hours_alarm;
		uint8_t  day_of_week;
		uint8_t  day_of_month;
		uint8_t  month;
		uint8_t  year;
		uint8_t  reg_A;
		uint8_t  reg_B;
		uint8_t  reg_C;
		uint8_t  reg_D;
		uint8_t  century;
		uint8_t  rsvd; /*Maintains 4 byte boundary */
		uint32_t wrapper_cntl;
		uint32_t rtc64_hi;
		uint32_t rtc64_lo;
		uint32_t rtc64AL_ctl;
		uint32_t rtc64AL0_hi;
		uint32_t rtc64AL0_lo;
		uint32_t rtc64AL1_hi;
		uint32_t rtc64AL1_lo;
		uint32_t rtc64AL2_hi;
		uint32_t rtc64AL2_lo;
} __attribute__((packed));
typedef struct rtc_hw_regs rtc_hw_regs_t;




/* Access functions for whole word of a DCR register */
#define rtc_get_word(addr)		mfdcr(addr)
#define rtc_put_word(addr, data)	mtdcr(addr, data)

/* Access functions for whole word of a CMU register */
inline static uint32_t rtc_get_cmu_word(unsigned int addr)
{
		uint32_t data = 0;

		mtdcr(DCRN_CMU_ADDR, addr);
		data = mfdcr(DCRN_CMU_DATA);

		return data;
}

inline static void rtc_put_cmu_word(unsigned int addr, uint32_t data)
{
		mtdcr(DCRN_CMU_ADDR, addr);
		mtdcr(DCRN_CMU_DATA, data);
}

/* Settor/Gettor functions for union rtc64_hi  because the HW
 bit definitions changed between DD 1.0 and DD 2.x*/
inline static void rtc64hi_get_fields(rtc64_hi_t hi, uint8_t *day, uint8_t *wday,
					uint8_t *mon, uint8_t *year, uint8_t *cen)
{
	if(mfspr(SPRN_PVR) == PVR_FSP1_PASS0){
		rtc64_hi_dd10_t hi_dd10;
		hi_dd10.reg = hi.reg;
		*wday = hi_dd10.bits.wday;
		*day =  hi_dd10.bits.day;
		*mon =  hi_dd10.bits.month;
		*year = hi_dd10.bits.year;
		*cen =  hi_dd10.bits.century;
	}else{
		*wday = 1;
		*day = hi.bits.day;
		*mon = hi.bits.month;
		*year = hi.bits.year;
		*cen = hi.bits.century;
	}
}

inline static void rtc64hi_set_fields(rtc64_hi_t *hi, uint8_t day, uint8_t wday,
					uint8_t mon, uint8_t year, uint8_t cen)
{
	if(mfspr(SPRN_PVR) == PVR_FSP1_PASS0){
		rtc64_hi_dd10_t *hi_dd10 = (rtc64_hi_dd10_t*)(hi);
		hi_dd10->bits.wday = wday;
		hi_dd10->bits.day = day;
		hi_dd10->bits.month = mon;
		hi_dd10->bits.year = year;
		hi_dd10->bits.century = cen;
	}else{
		hi->bits.day = day;
		hi->bits.month = mon;
		hi->bits.year = year;
		hi->bits.century = cen;
	}
}

#endif /* __FSP1_RTC */
