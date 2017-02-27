/*
 *   Copyright (c) International Business Machines Corp., 2005, 2010
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;	without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <asm/fsp1/rtc.h>

int ibmfsp1_is_timecal_valid(void)
{
	uint32_t wrapper_cntl = rtc_get_word(DCRN_RTC_WRAPPER_CNTL);
	uint32_t reg_B = rtc_get_word(DCRN_RTC_REG_B);

	/* Be paranoid and check other bits besides timecal valid this
	   includes reg B bits and update interrupt enable */
	int valid = (reg_B == (RTC_REGB_UIE | RTC_REGB_DM | RTC_REGB_24HR)) &&
		((wrapper_cntl & (RTC_WCNTL_VTC | RTC_WCNTL_EUIA)) ==
		 (RTC_WCNTL_VTC | RTC_WCNTL_EUIA));

	return valid;
}

void ibmfsp1_start_timecal_sync(void)
{
	uint32_t wrapper_cntl;

	/* Be paranoid and use NRST to clear active interrupt requests
	   if starting sync.  This may be redundant in some cases, but
	   needed in others */
	wrapper_cntl = rtc_get_word(DCRN_RTC_WRAPPER_CNTL);
	wrapper_cntl &= ~(RTC_WCNTL_NRST | RTC_WCNTL_EUIA);
	rtc_put_word(DCRN_RTC_WRAPPER_CNTL, wrapper_cntl);
	wrapper_cntl |= RTC_WCNTL_NRST; /* finish the reset */
	rtc_put_word(DCRN_RTC_WRAPPER_CNTL, wrapper_cntl);

	/* Initiate a TIMECALENDAR Bus Synchronization. */
	wrapper_cntl = rtc_get_word(DCRN_RTC_WRAPPER_CNTL);

	/* Start a synchronization cycle by setting the SYNCTC bit, then
	   enable the update ended interrupt access bit*/
	rtc_put_word(DCRN_RTC_WRAPPER_CNTL, (wrapper_cntl | RTC_WCNTL_SYNCTC));
	rtc_put_word(DCRN_RTC_WRAPPER_CNTL, (wrapper_cntl | RTC_WCNTL_EUIA));

	/* Enable the update ended interrupt, preserve binary and 24
	   hour modes. */
	rtc_put_word(DCRN_RTC_REG_B, ( RTC_REGB_UIE |
				       RTC_REGB_DM  |
				       RTC_REGB_24HR));
}

int ibmfsp1_set_rtc_time(struct rtc_time *set_tm)
{
	uint8_t century;
	uint32_t wrapper_cntl;
	struct rtc_time null_tm = {0,};
	struct rtc_time *tm = set_tm;

	/* Handle case where we need to set time to "0" */
	if (!tm) {
		tm = &null_tm;
	}

	/* Break down the time. */
	century = tm->tm_year / 100;

	/* Stop and reset the prescaler, clear any rate select. */
	rtc_put_word(DCRN_RTC_REG_A, RTC_REGA_DIV_RESET);

	/*
	 * Freeze the user copy of the time registers, clear all
	 * interrupt enables, set binary and 24 hour modes, reset
	 * daylight savings enable.
	 */
	rtc_put_word(DCRN_RTC_REG_B, (RTC_REGB_SET |
				      RTC_REGB_DM  |
				      RTC_REGB_24HR));

	/* Assert and clear the NRST line to clear active interrupt
	   requests. */
	wrapper_cntl = rtc_get_word(DCRN_RTC_WRAPPER_CNTL);
	wrapper_cntl |= RTC_WCNTL_NMR;
	wrapper_cntl &= ~(RTC_WCNTL_NRST | RTC_WCNTL_EUIA);
	rtc_put_word(DCRN_RTC_WRAPPER_CNTL, wrapper_cntl);

	/*finish the reset*/
	wrapper_cntl |= RTC_WCNTL_NRST;
	rtc_put_word(DCRN_RTC_WRAPPER_CNTL, wrapper_cntl);

	/* Write the time to the user copy of the counter registers. */
	rtc_put_word(DCRN_RTC_CENTURY, century);
	rtc_put_word(DCRN_RTC_YEAR, tm->tm_year % 100);
	rtc_put_word(DCRN_RTC_MONTH, tm->tm_mon);
	rtc_put_word(DCRN_RTC_DAY_OF_MONTH, tm->tm_mday);
	rtc_put_word(DCRN_RTC_HOURS, tm->tm_hour);
	rtc_put_word(DCRN_RTC_MINUTES, tm->tm_min);
	rtc_put_word(DCRN_RTC_SECONDS, tm->tm_sec);

	/* Transfer the user copy into the counter registers, set binary and
	 * 24 hour modes, reset daylight savings enable.*/
	rtc_put_word(DCRN_RTC_REG_B, RTC_REGB_DM | RTC_REGB_24HR);

	/* Allow the RTC to count again */
	rtc_put_word(DCRN_RTC_REG_A, RTC_REGA_REF_CLCK_32KHZ);

	ibmfsp1_start_timecal_sync();

	return 0;
}

void ibmfsp1_get_rtc_time(struct rtc_time *tm)
{
	uint8_t reg_A, reg_B, century = 0;
	uint8_t day, wday, mon, year;
	uint32_t wrapper_cntl;
	rtc64_hi_t rtc64_hi;
	rtc64_lo_t rtc64_lo;

	/* Get the RTC control registers for validity checking. */
	reg_A = rtc_get_word(DCRN_RTC_REG_A);
	reg_B = rtc_get_word(DCRN_RTC_REG_B);
	wrapper_cntl = rtc_get_word(DCRN_RTC_WRAPPER_CNTL);

	/* check for validity of timecalendar bus.  If valid then use
	   RTC64, otherwise fallback to the tried and true method of
	   busy polling base RTCCore */
	if(ibmfsp1_is_timecal_valid()) {
		/* The RTC TIMECALENDAR bus is synchronized. Use the
		   RTC64 register to read the RTC. rtc64_hi must be read
		   first to latch, then rtc64_lo unlatches*/
		rtc64_hi.reg = rtc_get_cmu_word(CMUN_RTC64_HI);
		rtc64_lo.reg = rtc_get_cmu_word(CMUN_RTC64_LO);

		rtc64hi_get_fields(rtc64_hi, &day, &wday,
				   &mon, &year, &century);
		tm->tm_mday = day;
		tm->tm_mon = mon;
		tm->tm_year = year;
		tm->tm_sec = rtc64_lo.bits.seconds;
		tm->tm_min = rtc64_lo.bits.minutes;
		tm->tm_hour = rtc64_lo.bits.hours;
	} else {
#ifndef CONFIG_FSP1
#error loop_cnt must be revisited on new platforms
#endif
		/* Single loop takes ~200 tb ticks on FSP1, so 4000 loops
		 * should take ~4ms.  Updates to RTC take about 2ms, so 4ms
		 * window will allow us to get a good read. */
		unsigned long loop_cnt = 4000;

		do {
			uint8_t tmp_A, tmp_sec;

			/* Re-read the RTC registers until no update is in
			   progress. */
			if (rtc_get_word(DCRN_RTC_REG_A) & RTC_REGA_UIP_BIT)
				continue;

			tm->tm_sec = rtc_get_word(DCRN_RTC_SECONDS);
			tm->tm_min = rtc_get_word(DCRN_RTC_MINUTES);
			tm->tm_hour = rtc_get_word(DCRN_RTC_HOURS);
			tm->tm_mday = rtc_get_word(DCRN_RTC_DAY_OF_MONTH);
			tm->tm_mon = rtc_get_word(DCRN_RTC_MONTH);
			tm->tm_year = rtc_get_word(DCRN_RTC_YEAR);
			century = rtc_get_word(DCRN_RTC_CENTURY);

			tmp_A = rtc_get_word(DCRN_RTC_REG_A);
			tmp_sec = rtc_get_word(DCRN_RTC_SECONDS);

			/* Break out of loop if we were able to read all the
			 * registers while:
			 * 1) The UIP bit stayed off and
			 * 2) the seconds field did not change */
			if (!(tmp_A & RTC_REGA_UIP_BIT) &&
					tm->tm_sec == tmp_sec) {
				break;
			}
		} while (--loop_cnt > 0);
	}

	/* Check the validity of the time stamp and control registers. */
	if (((reg_A & ~RTC_REGA_UIP_BIT) != RTC_REGA_REF_CLCK_32KHZ) ||
	    /* wrong ref clock freq*/
	    ((reg_B & ~RTC_REGB_INT_ENABLE_MASK) !=
	     (RTC_REGB_DM | RTC_REGB_24HR)) || /* BIN, 24 hours, no DSE */
	    ((wrapper_cntl & (RTC_WCNTL_NMR | RTC_WCNTL_NRST)) !=
	     (RTC_WCNTL_NMR | RTC_WCNTL_NRST)) || /* Not in Reset */
	    (tm->tm_sec > 59)  ||
	    (tm->tm_min > 59)  ||
	    (tm->tm_hour > 23) ||
	    (tm->tm_mday > 31) ||
	    (tm->tm_mon == 0)  ||
	    (tm->tm_mon > 12)  ||
	    (tm->tm_year > 99) ||
	    (century < 19)    ||
	    (century > 20)) {
		/* Either the register checks failed or time is invalid.
		 * Set the returned time to the epoch but leave the RTC alone
		 * so that raw registers may be dumped if desired.
		 */
		tm->tm_sec = 0;
		tm->tm_min = 0;
		tm->tm_hour = 0;
		tm->tm_mday = 1;
		tm->tm_mon = 1;
		tm->tm_year = 70;
		century = 19;
	}

	/* Add the century to the year */
	tm->tm_year += century * 100;

}

long ibmfsp1_time_init(void)
{
	uint8_t	 reg_A, reg_B, reg_C, reg_D;
	uint32_t wrapper_cntl;

	/* Read the RTC control/status registers. */
	reg_A = rtc_get_word(DCRN_RTC_REG_A);
	reg_B = rtc_get_word(DCRN_RTC_REG_B);
	reg_C = 0; /* Reading reg_C may disturb rtc64 counter rollover */
	reg_D = rtc_get_word(DCRN_RTC_REG_D);
	wrapper_cntl = rtc_get_word(DCRN_RTC_WRAPPER_CNTL);

	printk("RTC Init: Present date/time: "
	       "%02d%02d/%02d/%02d %02d:%02d:%02d\n",
	       rtc_get_word(DCRN_RTC_CENTURY),
	       rtc_get_word(DCRN_RTC_YEAR),
	       rtc_get_word(DCRN_RTC_MONTH),
	       rtc_get_word(DCRN_RTC_DAY_OF_MONTH),
	       rtc_get_word(DCRN_RTC_HOURS),
	       rtc_get_word(DCRN_RTC_MINUTES),
	       rtc_get_word(DCRN_RTC_SECONDS));
	printk("\t Regs A-D, wrapper_cntl: "
	       "0x%02x 0x%02x 0x%02x 0x%02x 0x%08x\n",
	       reg_A, reg_B, reg_C, reg_D, wrapper_cntl);

	/* Verify that the RTC is operational and set up as needed. */
	/* don't check (reg_D != RTC_REGD_VRT)) see bugzilla 873*/
	if (((reg_A & ~RTC_REGA_UIP_BIT) != RTC_REGA_REF_CLCK_32KHZ) ||
	    /* wrong ref clock freq, or rate bits set */
	    ((reg_B & ~RTC_REGB_INT_ENABLE_MASK) !=
	     (RTC_REGB_DM | RTC_REGB_24HR)) ||
	    /* Bin,24hr mode not set, or SET,DSE set. */
	    ((wrapper_cntl & (RTC_WCNTL_NMR | RTC_WCNTL_NRST)) !=
	     (RTC_WCNTL_NMR | RTC_WCNTL_NRST))) {
		/* Reset lines must not be low.	 Bug 18036 - don't
		   check TS bit */
		printk("RTC - ERROR\n");

		/* The RTC is either not set up correctly or lost power.
		 * Set up register A and then call the set_rtc_time function
		 * to set the RTC to start counting at 01/01/1970. */

		/* Bug 18036 need to leave external Clock Mode at POR
		   default and treat wrapper control as read - modify -
		   write.  FSP-1 1.0 will have TS set to 0 DD 2.0 and
		   beyond will have it POR to 0b1*/

		/* Issue an NMR and NRST reset, due to bug 18036 also
		   force everything else except TS */
		wrapper_cntl &= ~(RTC_WCNTL_NMR | RTC_WCNTL_NRST |
				  RTC_WCNTL_UM	| RTC_WCNTL_NRDP |
				  RTC_WCNTL_ASP | RTC_WCNTL_EUIA |
				  RTC_WCNTL_SYNCTC);
		rtc_put_word(DCRN_RTC_WRAPPER_CNTL, wrapper_cntl);

		/* Release NMR and NRST to finish the reset*/
		wrapper_cntl |= RTC_WCNTL_NMR | RTC_WCNTL_NRST;
		rtc_put_word(DCRN_RTC_WRAPPER_CNTL, wrapper_cntl);

		/* initialize time, calendar, and alarm registers. */
		ibmfsp1_set_rtc_time(NULL);

		reg_D = rtc_get_word(DCRN_RTC_REG_D);
		printk("RTC: Read VRT at step 7: %x\n",reg_D);
		reg_D = rtc_get_word(DCRN_RTC_REG_D);
		printk("RTC: Read VRT twice at step 7: %x\n",reg_D);

		printk("\t Resetting RTC.\n");
		printk("\t New date/time: %02d%02d/%02d/%02d %02d:%02d:%02d\n",
		       rtc_get_word(DCRN_RTC_CENTURY),
		       rtc_get_word(DCRN_RTC_YEAR),
		       rtc_get_word(DCRN_RTC_MONTH),
		       rtc_get_word(DCRN_RTC_DAY_OF_MONTH),
		       rtc_get_word(DCRN_RTC_HOURS),
		       rtc_get_word(DCRN_RTC_MINUTES),
		       rtc_get_word(DCRN_RTC_SECONDS));
		printk("\t Regs A-D, wrapper_cntl: %x %x %x %x %x\n",
		       rtc_get_word(DCRN_RTC_REG_A),
		       rtc_get_word(DCRN_RTC_REG_B),
		       0, /* Reading reg_C may disturb rtc64 counter
			     rollover */
		       rtc_get_word(DCRN_RTC_REG_D),
		       rtc_get_word(DCRN_RTC_WRAPPER_CNTL));
	}
	else if(!ibmfsp1_is_timecal_valid()) {
		ibmfsp1_start_timecal_sync();
	}

	/* Always return 0, since time_init() in time.c uses it to set
	   time_offset. */
	return 0;
}

/* FIXME extern qualifier removed when porting to MCP6.x, why was it needed */
time_t last_rtc_update;	/*this is a hack.. FIXME*/

EXPORT_SYMBOL(last_rtc_update);
EXPORT_SYMBOL(ibmfsp1_get_rtc_time);
EXPORT_SYMBOL(ibmfsp1_start_timecal_sync);
EXPORT_SYMBOL(ibmfsp1_is_timecal_valid);
EXPORT_SYMBOL(ibmfsp1_set_rtc_time);
EXPORT_SYMBOL(ibmfsp1_time_init);
