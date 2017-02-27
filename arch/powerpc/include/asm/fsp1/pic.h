/*
 * Copyright (c) International Business Machines Corp., 2001, 2010
 *
 * Module name: pic.h
 *
 * Description:
 *   Combine prioirq related .h from FSP
 */

#ifndef __PPC4XX_PIC_PRIOIRQ_H__
#define __PPC4XX_PIC_PRIOIRQ_H__

#include <asm/ptrace.h>
#include <linux/irq.h>

/* for lack of a better place, CAH
	MCP5 arch/ppc/include/asm/irq.h

	Maybe better is arch/powerpc/include/asm/fsp1/irq ... ?
*/
#define NR_UICS 21

/* UIC0 irq bitmap */
#define FSP1_RDMEM_ERR		0
#define FSP1_WRMEM_ERR		1
#define FSP1_FSIPORT_ERR	2
#define FSP1_FSIMASTER_ERR	3
#define FSP1_UIC1_CASC_CRIT	4
#define FSP1_UIC2_CASC_CRIT	5
#define FSP1_UIC3_CASC_CRIT	6
#define FSP1_UIC4_CASC_CRIT	7
#define FSP1_ECC_UE		8
#define FSP1_WPAPARITY_ERR	9
#define FSP1_WPAPROT_ERR	10
#define FSP1_ECC_CE		11
#define FSP1_SCRUB_ECC_UE	12
#define FSP1_SCRUB_ECC_CE	13
#define FSP1_SCRUB_OC		14
#define FSP1_PLBOPB0_ERR	15
#define FSP1_PLBOPB1_ERR	16
#define FSP1_PLBOPB2_ERR	17
#define FSP1_PLBOPB3_ERR	18
#define FSP1_OPBPLB_ERR		19
#define FSP1_EBC_ERR		20
#define FSP1_UIC1_CASC_NC	21
#define FSP1_UIC2_CASC_NC	22
#define FSP1_UIC3_CASC_NC	23
#define FSP1_UIC4_CASC_NC	24
#define FSP1_RSV01		25
#define FSP1_WR_HDCR		26
#define FSP1_FSI_PLUG		27
#define FSP1_RTC0_CMP		28
#define FSP1_RTC1_CMP		29
#define FSP1_RTC2_CMP		30
#define FSP1_UART		31

/* UIC1.0 irq bitmap
#define FSP1_UIC20_CASC_CRIT	32
#define FSP1_UIC21_CASC_CRIT	33
#define FSP1_UIC22_CASC_CRIT	34
#define FSP1_UIC23_CASC_CRIT	35
#define FSP1_UIC24_CASC_CRIT	36
#define FSP1_UIC25_CASC_CRIT	37
#define FSP1_UIC26_CASC_CRIT	38
#define FSP1_UIC27_CASC_CRIT	39
#define FSP1_UIC28_CASC_CRIT	40
#define FSP1_UIC29_CASC_CRIT	41
#define FSP1_UIC2A_CASC_CRIT	42
#define FSP1_UIC2B_CASC_CRIT	43
#define FSP1_UIC2C_CASC_CRIT	44
#define FSP1_UIC2D_CASC_CRIT	45
#define FSP1_UIC2E_CASC_CRIT	46
#define FSP1_UIC2F_CASC_CRIT	47
#define FSP1_UIC20_CASC_NC	48
#define FSP1_UIC21_CASC_NC	49
#define FSP1_UIC22_CASC_NC	50
#define FSP1_UIC23_CASC_NC	51
#define FSP1_UIC24_CASC_NC	52
#define FSP1_UIC25_CASC_NC	53
#define FSP1_UIC26_CASC_NC	54
#define FSP1_UIC27_CASC_NC	55
#define FSP1_UIC28_CASC_NC	56
#define FSP1_UIC29_CASC_NC	57
#define FSP1_UIC2A_CASC_NC	58
#define FSP1_UIC2B_CASC_NC	59
#define FSP1_UIC2C_CASC_NC	60
#define FSP1_UIC2D_CASC_NC	61
#define FSP1_UIC2E_CASC_NC	62
#define FSP1_UIC2F_CASC_NC	63
*/

/* UIC1.1 irq bitmap */
#define FSP1_PSI_MBOX		64
#define FSP1_PSI_LINK0		65
#define FSP1_PSI_LINK1		66
#define FSP1_PSI_LINK2		67
#define FSP1_PSI_LINK3		68
#define FSP1_RSV20		69
#define FSP1_RSV21		70
#define FSP1_RSV22		71
#define FSP1_RSV23		72
#define FSP1_RSV24		73
#define FSP1_RSV25		74
#define FSP1_RSV26		75
#define FSP1_PSIDMA_CHAN0	76
#define FSP1_PSIDMA_CHAN1	77
#define FSP1_PSIDMA_CHAN2	78
#define FSP1_PSIDMA_CHAN3	79
#define FSP1_FSIDMA3_CHAN0	80
#define FSP1_FSIDMA3_CHAN1	81
#define FSP1_FSIDMA3_CHAN2	82
#define FSP1_FSIDMA3_CHAN3	83
#define FSP1_FSIDMA2_CHAN0	84
#define FSP1_FSIDMA2_CHAN1	85
#define FSP1_FSIDMA2_CHAN2	86
#define FSP1_FSIDMA2_CHAN3	87
#define FSP1_FSIDMA1_CHAN0	88
#define FSP1_FSIDMA1_CHAN1	89
#define FSP1_FSIDMA1_CHAN2	90
#define FSP1_FSIDMA1_CHAN3	91
#define FSP1_FSIDMA0_CHAN0	92
#define FSP1_FSIDMA0_CHAN1	93
#define FSP1_FSIDMA0_CHAN2	94
#define FSP1_FSIDMA0_CHAN3	95

/* UIC1.2 irq bitmap
#define FSP1_EN0_WAKE		96
#define FSP1_EN0_EMAC		97
#define FSP1_EN0_MIB		98
#define FSP1_EN0_MALRXEOB	99
#define FSP1_EN0_MALTXEOB	100
#define	FSP1_EN0_MALRXDSRP_ERR	101
#define FSP1_EN0_MALTXDSRP_ERR	102
#define FSP1_EN0_MALSYS_ERR	103
#define FSP1_EN1_WAKE		104
#define FSP1_EN1_EMAC		105
#define FSP1_EN1_MIB		106
#define FSP1_EN1_MALRXEOB	107
#define FSP1_EN1_MALTXEOB	108
#define	FSP1_EN1_MALRXDSRP_ERR	109
#define FSP1_EN1_MALTXDSRP_ERR	110
#define FSP1_EN1_MALSYS_ERR	111
#define	FSP1_RSV32		112
#define	FSP1_RSV33		113
#define	FSP1_RSV34		114
#define	FSP1_RSV35		115
#define	FSP1_RSV36		116
#define	FSP1_RSV37		117
#define	FSP1_RSV38		118
#define	FSP1_RSV39		119
#define	FSP1_EXTIRQ0		120
#define	FSP1_EXTIRQ1		121
#define	FSP1_EXTIRQ2		122
#define	FSP1_EXTIRQ3		123
#define FSP1_USB_INT1		124
#define FSP1_USB_INT2		125
#define	FSP1_RSV3E		126
#define	FSP1_RSV3F		127
*/

/* UIC1.3 irq bitmap (reserved)
#define FSP1_RSV40		128	// shadow of disr bits
#define FSP1_RSV41		129	// shadow of disr bits
#define FSP1_RSV42		130	// shadow of disr bits
#define FSP1_BUDDY_RESET	131
#define FSP1_RSV44		132	// shadow of disr bits
#define FSP1_RSV45		133	// shadow of disr bits
#define FSP1_RSV46		134
#define FSP1_RSV47		135
#define ZHOST_MBX0		136	// on psi link 0
#define ZHOST_MBX1		137	// on psi link 1
#define ZHOST_MBX2		138	// on psi link 2
#define ZHOST_MBX3		139	// on psi link 3
#define FSP1_RSV4C		140
#define FSP1_RSV4D		141
#define FSP1_RSV4E		142
#define FSP1_RSV4F		143
#define FSP1_RSV4G		144
#define FSP1_RSV4H		145
#define FSP1_RSV4I		146
#define FSP1_RSV4J		147
#define FSP1_RSV4K		148
#define FSP1_RSV4L		149
#define FSP1_RSV4M		150
#define FSP1_RSV4N		151
#define FSP1_RSV4O		152
#define FSP1_RSV4P		153
#define FSP1_RSV4Q		154
#define FSP1_RSV4R		155
#define FSP1_WRTOUT		156
#define FSP1_RDTOUT		157
#define FSP1_WRIRT		158
#define FSP1_RDIRQ		159
*/

/* UIC2.x irq bitmap (reserved)
#define FSP1_UIC20_FIRST	160
*/

/* interrupt polarity--first set is for level trig; second for edge trig.
#define FSP_IRQ_POL_NEG			0x00
#define FSP_IRQ_POL_POS			0x01
#define FSP_IRQ_POL_FALLING_EDGE	0x00
#define FSP_IRQ_POL_RISING_EDGE		0x01
*/

/* interrupt trigger mechanism
#define FSP_IRQ_TRIG_LEVEL		0x00
#define FSP_IRQ_TRIG_EDGE		0x02
*/

/*
 * cascaded UIC interrupt
 * indicates if the particular interrupt pin actually fans out further.
 * not sure why we want to designate this.
 */


/*
 * critical IRQ bit
 * indicates if the UIC bit should be set to designate this IRQ as
 * critical, meaning it will come in on a different exception vector
 * to the CPU core..

#define FSP_IRQ_NORMAL			0x00
#define FSP_IRQ_FASTINT			0x04
#define FSP_IRQ_CRITICAL		0x08

#define FSP_IRQ_NOT_CASCADED		0x00
#define FSP_IRQ_CASCADED		0x10

#define FSP1_BASEPRI			0
#define BIT2VEC(bit)			((unsigned long)(0x80000000>>bit))

struct fsp_irq_struct
{
	unsigned short irq;
	unsigned char priority;
	unsigned char attrs;
};
*/

//extern struct hw_interrupt_type *ppc4xx_pic;

/* function prototypes
extern int set_fsp_irq_critical(int irq);
extern int set_fsp_cfam_fastint(int linkno, int cfamid);
extern int reset_fsp_cfam_fastint(int linkno, int cfamid);
extern int clear_uic_sr_cc(int word, int bit);
*/

#endif

