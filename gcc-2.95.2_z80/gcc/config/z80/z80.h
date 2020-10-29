/* Definitions of target machine for GNU compiler.
   Zilog Z80 (based on Motorola 68HC11 and 68HC12).
   Copyright (C) 1999, 2000 Free Software Foundation, Inc.
   Contributed by Stephane Carrez (stcarrez@worldnet.fr)

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

*/

#include "elfos.h"

/*****************************************************************************
**
** Controlling the Compilation Driver, `gcc'
**
*****************************************************************************/


/* Names to predefine in the preprocessor for this target machine.  */
#define CPP_PREDEFINES		"-Dz80"


#ifndef IN_LIBGCC2
#  include <stdio.h>
#endif

/* Forward type declaration for prototypes definitions.
   rtx_ptr is equivalent to rtx. Can't use the same name. */
struct rtx_def;
typedef struct rtx_def *rtx_ptr;

union tree_node;
typedef union tree_node *tree_ptr;

/* We can't declare enum machine_mode forward nor include 'machmode.h' here.
   Prototypes defined here will use an int instead. It's better than no
   prototype at all. */

typedef int enum_machine_mode;

/*****************************************************************************
**
** Run-time Target Specification
**
*****************************************************************************/

/* Run-time compilation parameters selecting different hardware subsets.  */

extern int target_flags;

extern short *reg_renumber;	/* def in local_alloc.c */


/* Default target_flags if no switches specified.  */
#define TARGET_SWITCHES {{ "", TARGET_DEFAULT}}
#ifndef TARGET_DEFAULT
    #define TARGET_DEFAULT  0
#endif


# define TARGET_Z80 1


/* Print subsidiary information on the compiler version in use.  */
#define TARGET_VERSION		fprintf (stderr, " (z80)")



/* target machine storage layout */

/* Define this if most significant byte of a word is the lowest numbered.  */
#define BYTES_BIG_ENDIAN 	0	

/* Define this if most significant bit is lowest numbered
   in instructions that operate on numbered bit-fields. */
#define BITS_BIG_ENDIAN        1 

/* Define this if most significant word of a multiword number is numbered.  */
#define WORDS_BIG_ENDIAN 		0	

/* Number of bits in an addressible storage unit */
#define BITS_PER_UNIT		8

/* Number of bits in a word */
#define BITS_PER_WORD		16	

/* Width of a word, in units (bytes).  */
#define UNITS_PER_WORD		(BITS_PER_WORD/8)

/* Define if you don't want extended real, but do want to use the
   software floating point emulator for REAL_ARITHMETIC and
   decimal <-> binary conversion.  */
#define REAL_ARITHMETIC

/* Width in bits of a pointer.  See also the macro `Pmode' defined below.  */
#define POINTER_SIZE		16

/* Definition of size_t.  This is really an unsigned short as the
   z80 only handles a 64K address space.  */
#define SIZE_TYPE               "short unsigned int"

/* A C expression for a string describing the name of the data type
   to use for the result of subtracting two pointers.  The typedef
   name `ptrdiff_t' is defined using the contents of the string.
   The z80 only has a 64K address space.  */
#define PTRDIFF_TYPE            "short int"

/* Allocation boundary (bits) for storing pointers in memory.  */
#define POINTER_BOUNDARY	8

/* Normal alignment required for function parameters on the stack, in bits.
   This can't be less than BITS_PER_WORD */
#define PARM_BOUNDARY		(BITS_PER_WORD)

/* Boundary (bits) on which stack pointer should be aligned.  */
#define STACK_BOUNDARY		8

/* Allocation boundary (bits) for the code of a function.  */
#define FUNCTION_BOUNDARY	8

/* Biggest alignment that any data type can require on this machine,
   in bits. */
#define BIGGEST_ALIGNMENT	8

/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY	8

/* Every structure's size must be a multiple of this.  */
#define STRUCTURE_SIZE_BOUNDARY 8

/* Define this if instructions will fail to work if given data not
   on the nominal alignment.  If instructions will merely go slower
   in that case, do not define this macro.  */
#define STRICT_ALIGNMENT	0

/* An integer expression for the size in bits of the largest integer
   machine mode that should actually be used.  All integer machine modes of
   this size or smaller can be used for structures and unions with the
   appropriate sizes.  */
#define MAX_FIXED_MODE_SIZE	32	

/* Floats are checked in a generic way. */
/* #define CHECK_FLOAT_VALUE(MODE, D, OVERFLOW) */



/* target machine storage layout */

/* Size (bits) of the type "int" on target machine
   (If undefined, default is BITS_PER_WORD).  */
#define INT_TYPE_SIZE            16

/* Size (bits) of the type "short" on target machine */
#define SHORT_TYPE_SIZE		16

/* Size (bits) of the type "long" on target machine */
#define LONG_TYPE_SIZE		32

/* Size (bits) of the type "long long" on target machine */
#define LONG_LONG_TYPE_SIZE    	32 

/* Size (bits) of the type "char" on target machine */
#define CHAR_TYPE_SIZE		8

/* A C expression for the size in bits of the type `float' on the
   target machine. If you don't define this, the default is one word.
   Don't use default: a word is only 16.  */
#define FLOAT_TYPE_SIZE         32

/* A C expression for the size in bits of the type double on the target
   machine. If you don't define this, the default is two words.
   Be IEEE compliant.  */
#define DOUBLE_TYPE_SIZE       	32 

#define LONG_DOUBLE_TYPE_SIZE  	32 

/* Define this as 1 if `char' should by default be signed; else as 0.  */
#define DEFAULT_SIGNED_CHAR	0

#define WCHAR_TYPE              "short int"
#define WCHAR_TYPE_SIZE         16

/* Define results of standard character escape sequences.  */
#define TARGET_BELL		007
#define TARGET_BS		010
#define TARGET_TAB		011
#define TARGET_NEWLINE		012
#define TARGET_VT		013
#define TARGET_FF		014
#define TARGET_CR		015


/* Standard register usage.  */

#define HARD_REG_SIZE           (UNITS_PER_WORD)

/* Assign names to real z80 registers.
We allocate hl+de  and bc +iy for 32 bit values.
Z80 has got register pairs HL(h,l),BC (b,c), DE (d,e),IX (ixh,ixl),IY (iyh,iyl)
 But we will use hl,bc,de,ix and iy and the single registers become fixed
registers. 
(I tried with  individual registers general registers and their pair as
fixed, but I feel this looks better approach. (???))
*/
#define HARD_A_REGNUM		0	
#define HARD_HL_REGNUM		1	
#define HARD_DE_REGNUM		2	
#define HARD_BC_REGNUM		3	
#define HARD_IY_REGNUM		4	
#define HARD_IX_REGNUM		5	
#define HARD_SP_REGNUM		6	
#define HARD_PC_REGNUM		7	
#define HARD_H_REGNUM		8	
#define HARD_L_REGNUM		9
#define HARD_D_REGNUM		10
#define HARD_E_REGNUM		11
#define HARD_B_REGNUM		12
#define HARD_C_REGNUM		13
#define HARD_F_REGNUM		14
#define HARD_IXL_REGNUM		15
#define HARD_IXH_REGNUM		16
#define HARD_IYL_REGNUM		17
#define HARD_IYH_REGNUM		18
#define HARD_N_REGS		19	

#define PC_REGNUM		10	


/* Number of actual hardware registers. The hardware registers are assigned
   numbers for the compiler from 0 to just below FIRST_PSEUDO_REGISTER. 
   All registers that the compiler knows about must be given numbers, even
   those that are not normally considered general registers. */
#define FIRST_PSEUDO_REGISTER		HARD_N_REGS

/* 1 for registers that have pervasive standard uses and are not available
   for the register allocator. */
#define FIXED_REGISTERS \
  {0,  0,  0, 0,  0,  0,  1,  1,  1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1 }
/* A, HL, DE, BC, IY, IX, SP, PC, H, L,  B, C, D, E, F, IXL,IXH,IYH,IYL */


/* 1 for registers not available across function calls. For our pseudo
   registers, all are available.  */
#define CALL_USED_REGISTERS \
  {1,  1,  1, 1,  1,  1,  1,  1,  1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1 }
/* A, HL, DE, BC, IY, IX, SP, PC, H, L,  B, C, D, E, F, IXL,IXH,IYH,IYL */


/* Define this macro to change register usage conditional on target flags.
#define CONDITIONAL_REGISTER_USAGE (z80_conditional_register_usage ())
*/

/* List the order in which to allocate registers.  Each register must be
   listed once, even those in FIXED_REGISTERS.  */
#define REG_ALLOC_ORDER						\
{ HARD_A_REGNUM,HARD_HL_REGNUM,HARD_BC_REGNUM,HARD_DE_REGNUM,			\
  HARD_IY_REGNUM,HARD_IX_REGNUM,	HARD_SP_REGNUM,\
  HARD_PC_REGNUM,HARD_H_REGNUM,HARD_L_REGNUM, HARD_B_REGNUM,\
  HARD_C_REGNUM,HARD_D_REGNUM,HARD_E_REGNUM,HARD_F_REGNUM,HARD_IXL_REGNUM,\
  HARD_IXH_REGNUM,HARD_IYL_REGNUM,HARD_IYH_REGNUM,			\
}

/* A C expression for the number of consecutive hard registers,
   starting at register number REGNO, required to hold a value of
   mode MODE.  */
#define HARD_REGNO_NREGS(REGNO, MODE) \
((Q_REGNO_P (REGNO)) ? (GET_MODE_SIZE (MODE)) : \
   ((GET_MODE_SIZE (MODE) + HARD_REG_SIZE - 1) / HARD_REG_SIZE))

/* Value is 1 if hard register REGNO can hold a value of machine-mode MODE.
    - 8 bit values are stored anywhere (except the IX,IY and SP register).
    - 16 bit values can be stored in  all long registers.
    - 32 bit values can be stored in  (hl + de) or (bc + iy) registers 
   lowpart will be hl and high part is de.
   (you have instruction ex hl, de (1 byte).A bit cheaper than (hl + bc).
 */

#define HARD_REGNO_MODE_OK(REGNO, MODE) \
     hard_regno_mode_ok (REGNO,MODE)

/* Value is 1 if it is a good idea to tie two pseudo registers when one has
   mode MODE1 and one has mode MODE2.  If HARD_REGNO_MODE_OK could produce
   different values for MODE1 and MODE2, for any hard reg, then this must be
   0 for correct output.  */
#define MODES_TIEABLE_P(MODE1, MODE2)                   \
     (((MODE1) == (MODE2))                              \
      || ((MODE1) == SImode && (MODE2) == HImode)	\
      || ((MODE1) == HImode && (MODE2) == SImode))


/* Define the classes of registers for register constraints in the
   machine description.  Also define ranges of constants.

   One of the classes must always be named ALL_REGS and include all hard regs.
   If there is more than one class, another class must be named NO_REGS
   and contain no registers.

   The name GENERAL_REGS must be the name of a class (or an alias for
   another name such as ALL_REGS).  This is the class of registers
   that is allowed by "g" or "r" in a register constraint.
   Also, registers outside this class are allocated only when
   instructions express preferences for them.

   The classes must be numbered in nondecreasing order; that is,
   a larger-numbered class must never be contained completely
   in a smaller-numbered class.

   For any two classes, it is very desirable that there be another
   class that represents their union.  
 
  On z80 we have register pairs.
  h+l, d+e, b+c
  ixh+ixl, iyh+iyl.
  We use hl,de, bc, ix and iy registers .We do not use the registers 
  individually . I had problems for reloads of SImode values, 
  when used registers individually. So I moved on to this approach.
  
  QImode values can be stored in hl,de,bc and a registers.
  HImode values can be stored in HL, DE,BC,IY,IX or SP  registers.
  For this we have  register classes HL_REGS, DE_REGS and BC_REGS.
  SImode velues are stored in  hl + de and bc +iy registers.
  For this we have classes HL_OR_DE_REGS and BC_OR_IY_REGS.
  */

enum reg_class
{	NO_REGS,	/* no registers class , gcc insists on this	*/
	DA_REGS,	/* register a					*/
	HL_REGS,	/* 16 bit accumulator  				*/
	DE_REGS,	/*  DE			*/
	BC_REGS,	/*  BC  				*/
	IY_REGS,	/*  IY register 						*/
	IX_REGS,	/*  IX  register 						*/
	HL_OR_DE_REGS,	/* HL or DE register				*/
	BC_OR_IY_REGS,	/* BC or IY register 						*/
	A_REGS,		/* address registers IX, IY			*/
	RR_REGS,	/* registers HL,IX or IY			*/
	SP_REGS,	/* register SP					*/
	SS_REGS,	/* registers BC,DE,HL,SP			*/
	Q_REGS,		/* register A,B,C,D,E,H,L, ixh,ixl, iyh,iyl  	*/
	H_REGS,		/* BC,DE,HL,IX,IY,SP 				*/
	G_REGS,		/* all registers				*/
	ALL_REGS,
	LIM_REG_CLASSES
};

#define GENERAL_REGS    G_REGS		/* alias GENERAL_REGS to G_REGS.*/
#define N_REG_CLASSES (int)LIM_REG_CLASSES
#define REG_CLASS_NAMES \
{	"NO_REGS",	\
	"DA_REGS",	\
	"HL_REGS",	\
	"DE_REGS",	\
	"BC_REGS",	\
	"IY_REGS",	\
	"IX_REGS",	\
	"HL_OR_DE_REGS",	\
	"BC_OR_IY_REGS",	\
	"A_REGS",	\
	"RR_REGS",	\
	"SP_REGS",	\
	"SS_REGS",	\
	"Q_REGS",	\
	"H_REGS",	\
	"G_REGS",	\
	"ALL_REGS" }


/* An initializer containing the contents of the register classes,
   as integers which are bit masks.  The Nth integer specifies the
   contents of class N.  The way the integer MASK is interpreted is
   that register R is in the class if `MASK & (1 << R)' is 1.  */

/*--------------------------------------------------------------
   A		0x00000001
   HL		0x00000002
   DE		0x00000004
   BC		0x00000008
   IY		0x00000010
   IX		0x00000020
   SP		0x00000040
   PC		0x00000080
   H		0x00000100
   L		0x00000200
   D       	0x00000400
   E		0x00000800
   B		0x00001000
   C		0x00002000
   F        0x00004000
   IXL		0x00008000	
   IXH      0x00010000
   IYL      0x00020000
   IYH      0x00040000 

   HL_REGS represents HL. It is used for 16-bits numbers.
   DE_REGS represents DE. It is used for 16-bits numbers.
   BC_REGS represents BC. It is used for 16-bits numbers.
   A_REGS      represents a valid base register for indexing. It represents
	       X,Y.
   HL_OR_DE_REGS represents HL + DE. It is used for 32-bits numbers.
   BC_OR_IY_REGS represents BC + IY. It is used for 32-bits numbers.
--------------------------------------------------------------*/

#define REG_CLASS_CONTENTS					  \
/* NO_REGS	*/		   {{ 0x00000000},				  \
/* DA_REGS	*/ 		 	{ 0x00000001}, /* A				*/\
/* HL_REGS	*/ 		{ 0x00000002}, /* HL				*/\
/* DE_REGS	*/ 		{ 0x00000004}, /* DE 				*/\
/* BC_REGS	*/ 		{ 0x00000008}, /* BC				*/\
/* IY_REGS	*/ 			{ 0x00000010}, /* IY			*/\
/* IX_REGS*/ 			{ 0x00000020}, /* IX 				*/\
/* HL_OR_DE_REGS*/ 		{ 0x00000006},  /* HL,DE 				*/\
/* BC_OR_IY_REGS*/ 		{ 0x00000018}, 	/* BC,IY 				*/\
/* A_REGS*/ 			{ 0x00000030},  /* IX,IY 				*/\
/* RR_REGS	*/ 			{ 0x00000032},	/* HL,IX,IY 			*/\
/* SP_REGS	*/ 			{ 0x00000040}, 	/* SP				*/\
/* SS_REGS	*/ 			{ 0x0000004E}, /*HL,BC,DE,SP */\
/* Q_REGS	*/ 			{ 0x0007BF00}, /* A,B,C,D,E,H,LIXH,IXL,IYH,IYL	*/\
/* H_REGS	*/ 			{ 0x0000007E}, /* HL,BC,DE,SP, IX OR IY	*/\
/* G_REGS	*/ 			{ 0x0007FFFF}, /*            		*/\
/* ALL_REGS	*/ 			{ 0x0007FFFF}}




#define Q_REGNO_P(REGNO)	((REGNO) == HARD_A_REGNUM	\
                                 ||(REGNO) == HARD_B_REGNUM	\
                                 ||(REGNO) == HARD_C_REGNUM	\
                                 ||(REGNO) == HARD_D_REGNUM	\
                                 ||(REGNO) == HARD_E_REGNUM	\
                                 ||(REGNO) == HARD_H_REGNUM	\
                                 ||(REGNO) == HARD_L_REGNUM \
                                 ||(REGNO) == HARD_IXH_REGNUM	\
                                 ||(REGNO) == HARD_IXL_REGNUM	\
                                 ||(REGNO) == HARD_IYH_REGNUM	\
                                 ||(REGNO) == HARD_IYL_REGNUM)


#define Q_REG_P(X)		(REG_P (X) && Q_REGNO_P (REGNO (X)))

/* HL registers */

#define HL_REGNO_P(REGNO)	((REGNO) == HARD_HL_REGNUM)
#define HL_REG_P(X)		(REG_P (X) && HL_REGNO_P (REGNO (X)))

#define HL_OR_DE_REG_P(X)		(REG_P (X) && HL_OR_DE_REGNO_P (REGNO (X)))

#define HL_OR_DE_REGNO_P(REGNO)	(HL_REGNO_P(REGNO)	\
								||	DE_REGNO_P(REGNO))

#define BC_OR_IY_REG_P(X)		(REG_P (X) && BC_OR_IY_REGNO_P (REGNO (X)))
#define BC_OR_IY_REGNO_P(REGNO)	(BC_REGNO_P(REGNO)	\
								||	IY_REGNO_P(REGNO))

/* BC registers */

#define BC_REGNO_P(REGNO)	((REGNO) == HARD_BC_REGNUM)
#define BC_REG_P(X)		(REG_P (X) && BC_REGNO_P (REGNO (X)))

/* D OR E registers */

#define DE_REGNO_P(REGNO)	((REGNO) == HARD_DE_REGNUM)
#define DE_REG_P(X)		(REG_P (X) && DE_REGNO_P (REGNO (X)))

/* IX OR IY registers */

#define IX_REGNO_P(REGNO)	((REGNO) == HARD_IX_REGNUM)
#define IX_REG_P(X)		(REG_P (X) && IX_REGNO_P (REGNO (X)))

#define IY_REGNO_P(REGNO)	((REGNO) == HARD_IY_REGNUM)
#define IY_REG_P(X)		(REG_P (X) && IY_REGNO_P (REGNO (X)))

#define A_REGNO_P(REGNO)        ((REGNO) == HARD_IX_REGNUM 		\
                                || (REGNO) == HARD_IY_REGNUM)
#define A_REG_P(X)              (REG_P (X) && A_REGNO_P (REGNO (X)))



#define DA_REGNO_P(REGNO)	((REGNO) == HARD_A_REGNUM)
#define DA_REG_P(X)		(REG_P (X) && DA_REGNO_P (REGNO (X)))


#define SP_REGNO_P(REGNO)       ((REGNO) == HARD_SP_REGNUM)
#define SP_REG_P(X)             (REG_P (X) && SP_REGNO_P (REGNO (X)))


/* ALL z80 long registers  BC,DE, HL, IX,IY and SP  */

#define H_REGNO_P(REGNO)		(HL_REGNO_P (REGNO) \
								|| A_REGNO_P (REGNO) \
								|| BC_REGNO_P (REGNO) \
								|| DE_REGNO_P (REGNO)\
							|| SP_REGNO_P (REGNO))
#define H_REG_P(X)              (REG_P (X) && H_REGNO_P (REGNO (X)))

#define G_REGNO_P(REGNO)	(H_REGNO_P (REGNO)	|| SP_REGNO_P (REGNO)\
								|| Q_REGNO_P (REGNO) \
								||((REGNO) == HARD_PC_REGNUM))
#define G_REG_P(X)              (REG_P (X) && G_REGNO_P (REGNO (X)))


#define REGNO_REG_CLASS(REGNO) \
  (DA_REGNO_P (REGNO) ? DA_REGS : \
   (HL_REGNO_P (REGNO) ? HL_REGS : \
   (DE_REGNO_P (REGNO) ? DE_REGS : \
   (BC_REGNO_P (REGNO) ? BC_REGS : \
   (IY_REGNO_P (REGNO) ? IY_REGS : \
   (IX_REGNO_P (REGNO) ? IX_REGS : \
   (HL_OR_DE_REGNO_P (REGNO) ? HL_OR_DE_REGS : \
   (BC_OR_IY_REGNO_P (REGNO) ? BC_OR_IY_REGS : \
     (SP_REGNO_P (REGNO) ? SP_REGS : \
      (H_REGNO_P (REGNO) ? H_REGS : \
        (G_REGNO_P (REGNO) ? G_REGS : ALL_REGS)))))))))))  


/* Get reg_class from a letter in the machine description.  */

#define REG_CLASS_FROM_LETTER(C) \
   ((C) == 'a' ? DA_REGS : \
    (C) == 'A' ? A_REGS : \
    (C) == 'h' ? HL_REGS : \
    (C) == 'b' ? BC_REGS : \
    (C) == 'd' ? DE_REGS : \
    (C) == 'D' ? HL_OR_DE_REGS : \
    (C) == 'u' ? BC_OR_IY_REGS : \
    (C) == 'q' ? Q_REGS : \
    (C) == 'z' ? RR_REGS : \
    (C) == 't' ? H_REGS : \
    (C) == 'w' ? SP_REGS : \
    (C) == 'x' ? IX_REGS : \
    (C) == 'y' ? IY_REGS : NO_REGS)


#define PREFERRED_RELOAD_CLASS(X,CLASS)		(CLASS)

#define SECONDARY_ELOAD_CLASS(CLASS,MODE,IN) \
	(((CLASS) != DA_REGS && MODE == QImode && \
 	 symbolic_memory_operand (X, MODE))\
	 ? DA_REGS: NO_REGS)



#define SMALL_REGISTER_CLASSES 1

/* A C expression whose value is nonzero if pseudos that have been
   assigned to registers of class CLASS would likely be spilled
   because registers of CLASS are needed for spill registers.

   The default value of this macro returns 1 if CLASS has exactly one
   register and zero otherwise.  On most machines, this default
   should be used.  Only define this macro to some other expression
   if pseudo allocated by `local-alloc.c' end up in memory because
   their hard registers were needed for spill registers.  If this
   macro returns nonzero for those classes, those pseudos will only
   be allocated by `global.c', which knows how to reallocate the
   pseudo to another register.  If there would not be another
   register available for reallocation, you should not change the
   definition of this macro since the only effect of such a
   definition would be to slow down register allocation.  */

#define CLASS_LIKELY_SPILLED_P(CLASS)					\
  (((CLASS) == DA_REGS)							\
   || ((CLASS) == HL_REGS)                                               \
   || ((CLASS) == BC_REGS)                                               \
   || ((CLASS) == DE_REGS)                                               \
   || ((CLASS) == A_REGS)                                               \
   || ((CLASS) == SP_REGS)                                              \
   || ((CLASS) == HL_OR_DE_REGS)                                          \
   || ((CLASS) == BC_OR_IY_REGS)                                          \
   || ((CLASS) == IX_REGS)                                         \
   || ((CLASS) == IY_REGS))

/* Return the maximum number of consecutive registers needed to represent
   mode MODE in a register of class CLASS.  
   Verify this macro......
 
|| (CLASS) == BC_OR_IY_REGS) ? GET_MODE_SIZE (MODE) \
 */

#define CLASS_MAX_NREGS(CLASS, MODE)				\
( ((CLASS) == DA_REGS || (CLASS) == Q_REGS)?			\
  GET_MODE_SIZE (MODE):						\
  ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD))

/* The letters I, J, K, L and M in a register constraint string
   can be used to stand for particular ranges of immediate operands.
   This macro defines what the ranges are.
   C is the letter, and VALUE is a constant value.
   Return 1 if VALUE is in the range specified by C.

   `L' is for range -65536 to 65536
   `M' is for values whose 16-bit low part is 0
   'N' is for +1 or -1.
   'O' is for 16 (for rotate using swap).
   'P' is for range -8 to 2 (used by addhi_sp)

   'I', 'J', 'K' are not used.  */

#define CONST_OK_FOR_LETTER_P(VALUE, C) \
  ((C) == 'L' ? (VALUE) >= -65536 && (VALUE) <= 65535 : \
   (C) == 'M' ? ((VALUE) & 0x0ffffL) == 0 : \
   (C) == 'N' ? ((VALUE) == 1 || (VALUE) == -1):0)

/* Similar, but for floating constants, and defining letters G and H.
   No floating-point constants are valid on 68HC11.  */
#define CONST_DOUBLE_OK_FOR_LETTER_P(VALUE, C)  0

/* 'U' represents certain kind of memory indexed operand for z80.
  'R' is actually for symbolic memory.
 */

#define EXTRA_CONSTRAINT(OP, C) \
	((C)=='U' ? z80_small_indexed_indirect_p(OP,GET_MODE(OP)):\
	 (C)=='R' ? (GET_CODE (OP) == MEM && !z80_small_indexed_indirect_p(OP,GET_MODE(OP))):0)


/* Stack layout; function entry, exit and calling.  */

/* Define this if pushing a word on the stack
   makes the stack pointer a smaller address.  */
#define STACK_GROWS_DOWNWARD

/* Define this if the nominal address of the stack frame
   is at the high-address end of the local variables;
   that is, each additional local variable allocated
   goes at a more negative offset in the frame.   */

#define FRAME_GROWS_DOWNWARD

/* Define this if successive arguments to a function occupy decreasing 
   addresses in the stack.  */
/* #define ARGS_GROW_DOWNWARD */

/* Offset within stack frame to start allocating local variables at.
   If FRAME_GROWS_DOWNWARD, this is the offset to the END of the
   first local allocated.  Otherwise, it is the offset to the BEGINNING
   of the first local allocated.  */
#define STARTING_FRAME_OFFSET		0  	

/* Offset of first parameter from the argument pointer register value.  */

#define FIRST_PARM_OFFSET(FNDECL)	4	

/* A C expression whose value is RTL representing the location of the
   incoming return address at the beginning of any function, before the
   prologue.  This RTL is either a REG, indicating that the return
   value is saved in REG, or a MEM representing a location in
   the stack.
  
   Before the prologue, RA is at 0(sp). */
#define INCOMING_RETURN_ADDR_RTX \
    gen_rtx_MEM (VOIDmode, gen_rtx_REG (VOIDmode, STACK_POINTER_REGNUM))

/* Before the prologue, the top of the frame is at 2(sp).  */
#define INCOMING_FRAME_SP_OFFSET        2

/* Define this if functions should assume that stack space has been
   allocated for arguments even when their values are passed in
   registers.
  
   The value of this macro is the size, in bytes, of the area reserved for
   arguments passed in registers.
  
   This space can either be allocated by the caller or be a part of the
   machine-dependent stack frame: `OUTGOING_REG_PARM_STACK_SPACE'
   says which. */
/* #define REG_PARM_STACK_SPACE(FNDECL)	2 */

/* Define this macro if REG_PARM_STACK_SPACE is defined but stack
   parameters don't skip the area specified by REG_PARM_STACK_SPACE.
   Normally, when a parameter is not passed in registers, it is placed on
   the stack beyond the REG_PARM_STACK_SPACE area.  Defining this macro  
   suppresses this behavior and causes the parameter to be passed on the
   stack in its natural location.  */
/* #define STACK_PARMS_IN_REG_PARM_AREA */

/* Register to use for pushing function arguments.  */
#define STACK_POINTER_REGNUM		HARD_SP_REGNUM

/* Base register for access to local variables of the function.  */
#define FRAME_POINTER_REGNUM		HARD_IX_REGNUM

/* Base register for access to arguments of the function.  */
#define ARG_POINTER_REGNUM			FRAME_POINTER_REGNUM

/* Register in which static-chain is passed to a function. 
  This needs to be looked in more closely. I am saing the static chain
  should be passed in iy register.
   But actually I am modifying this register in the prologue ??
   To allocate frame --- ld iy, frame_size
   						  add iy, sp
						  ld sp, iy
  This is the combination that I could find for frame setup.

 May be we can pass this in memory. (??)
 --need to look in to this.


 */
#define STATIC_CHAIN_REGNUM	  		HARD_IY_REGNUM  



/* Value should be nonzero if functions must have frame pointers.
   Zero means the frame pointer need not be set up (and parms may be
   accessed via the stack pointer) in functions that seem suitable.
   This is computed in `reload', in reload1.c. */
#define FRAME_POINTER_REQUIRED	0

#define INITIAL_FRAME_POINTER_OFFSET(DEPTH_VAR)			\
{                                                               \
  int offset, regno;                                            \
  offset = get_frame_size();                                    \
  for (regno = 0; regno < 8; regno++)                           \
    if (regs_ever_live[regno] && ! call_used_regs[regno])       \
      offset += 2;                                              \
  (DEPTH_VAR) = offset;                                         \
}

/* LONGJMP_RESTORE_FROM_STACK */


/* Passing Function Arguments on the Stack.  */

/* When a prototype says `char' or `short', really pass an `int'.  
   A guess for z80. We do n't have instructions to push a single
   byte on to stack */


#define PROMOTE_PROTOTYPES 

/* If we generate an insn to push BYTES bytes, this says how many the
   stack pointer really advances by. When ever we push it is interms of word.
*/

#define PUSH_ROUNDING(BYTES) (((BYTES) + 1) & (-2))

/* Value is 1 if returning from a function call automatically pops the
   arguments described by the number-of-args field in the call. FUNTYPE is
   the data type of the function (as a tree), or for a library call it is
   an identifier node for the subroutine name.  */

  
#define RETURN_POPS_ARGS(FUNDECL,FUNTYPE,SIZE)	0

/* Nonzero if type TYPE should be returned in memory.
   Blocks and data types largers than 4 bytes cannot be returned
   in the register (hl + de = 4).  */
#define RETURN_IN_MEMORY(TYPE)				\
    ((TYPE_MODE (TYPE) == BLKmode)			\
     ? (int_size_in_bytes (TYPE) > 4)			\
     : (GET_MODE_SIZE (TYPE_MODE (TYPE)) > 4))


/* Passing Arguments in Registers.  */

/* Define a data type for recording info about an argument list
   during the scan of that argument list.  This data type should
   hold all necessary information about the function itself
   and about the args processed so far, enough to enable macros
   such as FUNCTION_ARG to determine where the next arg should go.  */

#define CUMULATIVE_ARGS	int

/* A C expression that indicates when an argument must be passed by reference.
   If nonzero for an argument, a copy of that argument is made in memory and a
   pointer to the argument is passed instead of the argument itself.
   The pointer is passed in whatever way is appropriate for passing a pointer
   to that type.  */

 
#define FUNCTION_ARG_PASS_BY_REFERENCE(CUM, MODE, TYPE, NAMED)		\
  ((TYPE) && int_size_in_bytes (TYPE) > 8)


/* If defined, a C expression which determines whether, and in which direction,
   to pad out an argument with extra space.  The value should be of type
   `enum direction': either `upward' to pad above the argument,
   `downward' to pad below, or `none' to inhibit padding.

   Structures are stored left shifted in their argument slot.  
  The default definition should suffice.
 */

/* #define FUNCTION_ARG_PADDING(MODE, TYPE)  */


/* Initialize a variable CUM of type CUMULATIVE_ARGS for a call to a
   function whose data type is FNTYPE. For a library call, FNTYPE is 0. */
#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,INDIRECT)	\
 ((CUM)= 0)

/* Update the data in CUM to advance over an argument of mode MODE and data
   type TYPE. (TYPE is null for libcalls where that information may not be
   available.) 
  This does not matter if all the arguments are passed on stack.
 */
#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED) \
 ((CUM) += (GET_MODE_SIZE (MODE) + 1) & (-2)) 

/* Define where to put the arguments to a function.
   Value is zero to push the argument on the stack,
   or a hard register in which to store the argument.

   MODE is the argument's machine mode.
   TYPE is the data type of the argument (as a tree).
    This is null for libcalls where that information may
    not be available.
   CUM is a variable of type CUMULATIVE_ARGS which gives info about
    the preceding args and about the function being called.
   NAMED is nonzero if this argument is a named parameter
    (otherwise it is an extra parameter matching an ellipsis).  

  As a first hand approach let me push all the arguments on to stack.
  May be in the next round I can pass them in registers. 
 */
#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED)	0


/* Define the profitability of saving registers around calls.

   Disable this because the saving instructions generated by
   caller-save need a reload and the way it is implemented,
   it forbids all spill registers at that point.  Enabling
   caller saving results in spill failure.  
#define CALLER_SAVE_PROFITABLE(REFS,CALLS) 0
*/
#define DEFAULT_CALLER_SAVES

/* For an arg passed partly in registers and partly in memory,
   this is the number of registers used.
   For args passed entirely in registers or entirely in memory, zero.

   Passing an arg partly in register and memory does not work at all.
   Don't do that.  */
#define FUNCTION_ARG_PARTIAL_NREGS(CUM, MODE, TYPE, NAMED) (0)

/* 1 if N is a possible register number for function argument passing.
   HL is for 16-bit values, HL + DE is for 32-bit  */
#define FUNCTION_ARG_REGNO_P(N)	(0)

/* All return values are in the D or X+D registers:
    - 8 and 16-bit values are returned in hl.
      BLKmode are passed in HL as pointer.
    - 32-bit values are returned in hl + de. 
      The high part is passed in de and the low part in hl. */

#define FUNCTION_VALUE(VALTYPE, FUNC)					\
     gen_rtx (REG, TYPE_MODE (VALTYPE),					\
		    HARD_HL_REGNUM)

#define LIBCALL_VALUE(MODE)						\
     gen_rtx (REG, MODE,HARD_HL_REGNUM)

/* 1 if N is a possible register number for a function value.  */
#define FUNCTION_VALUE_REGNO_P(N) \
     ((N) == HARD_HL_REGNUM)

/* Register in which address to store a structure value is passed to a
   function.  */
#define STRUCT_VALUE_REGNUM		0


/* EXIT_IGNORE_STACK should be nonzero if, when returning from a function,
   the stack pointer does not matter.  The value is tested only in functions
   that have frame pointers. No definition is equivalent to always zero.  */
#define EXIT_IGNORE_STACK	0


/* Generating Code for Profiling.  */

/* Output assembler code to FILE to increment profiler label # LABELNO
   for profiling a function entry.  */
#define FUNCTION_PROFILER(FILE, LABELNO)	;

/* Save all registers which may be clobbered by a function call.
   MACHINE_STATE_SAVE and MACHINE_STATE_RESTORE are target-code macros,
   used in libgcc2.c.  They may not refer to TARGET_* macros !!!

   We don't need to save the CCR nor the soft registers because
   they will be saved by gcc.  */
#define MACHINE_STATE_SAVE(id) \
  {			       \
    asm ("push\thl");	       \
    asm ("push\tde");	       \
    asm ("push\tbc");	       \
    asm ("push\tix");	       \
    asm ("push\tiy");	       \
  }

#define MACHINE_STATE_RESTORE(id) \
  {			       \
    asm ("pop\tiy");	       \
    asm ("pop\tix");	       \
    asm ("pop\tbc");	       \
    asm ("pop\tde");	       \
    asm ("pop\thl");	       \
  }

/* Output assembler code for a block containing the constant parts
   of a trampoline, leaving space for the variable parts.  */
#define TRAMPOLINE_TEMPLATE(FILE) { \
  fprintf (FILE, "\t.bogus\t\t; TRAMPOLINE_TEMPLATE unimplemented\n"); }

/* Length in units of the trampoline for entering a nested function.  */
#define TRAMPOLINE_SIZE		0

/* A C statement to initialize the variable parts of a trampoline.
   ADDR is an RTX for the address of the trampoline; FNADDR is an
   RTX for the address of the nested function; STATIC_CHAIN is an
   RTX for the static chain value that should be passed to the
   function when it is called.  */
#define INITIALIZE_TRAMPOLINE(TRAMP, FNADDR, CXT) { \
	}



/* `INIT_TARGET_OPTABS'
     Define this macro as a C statement that declares additional library
     routines renames existing ones. `init_optabs' calls this macro
     after initializing all the normal library routines.
*/

#define MODHI3_LIBCALL "__modhi3"
#define DIVHI3_LIBCALL "__divhi3"
#define MULHI3_LIBCALL "__mulhi3"
/*
#define MODQI3_LIBCALL "__modqi3"
#define DIVQI3_LIBCALL "__divqi3"
#define MULQI3_LIBCALL "__mulqi3"
*/

#define INIT_TARGET_OPTABS \
  do { \
    smod_optab->handlers[(int) HImode].libfunc		\
      = gen_rtx (SYMBOL_REF, Pmode, MODHI3_LIBCALL);	\
    sdiv_optab->handlers[(int) HImode].libfunc		\
      = gen_rtx (SYMBOL_REF, Pmode, DIVHI3_LIBCALL);	\
    smul_optab->handlers[(int) HImode].libfunc				\
      = gen_rtx_SYMBOL_REF (Pmode, MULHI3_LIBCALL);			\
  } while (0)


/* Addressing modes, and classification of registers for them.  */
/*
#define HAVE_POST_INCREMENT 
#define HAVE_PRE_INCREMENT 
#define HAVE_POST_DECREMENT 
#define HAVE_PRE_DECREMENT 
*/

/* The class value for base registers. For z80 we define it as A_REGS (IX or IY)
  registers */
#define BASE_REG_CLASS		A_REGS


/* The class value for index registers.  This is NO_REGS for z80.  */

#define INDEX_REG_CLASS		NO_REGS

#define REG_VALID_FOR_BASE_P(REGNO)             \
     ((REGNO) < FIRST_PSEUDO_REGISTER               \
          && ( (REGNO == HARD_IX_REGNUM) || (REGNO == HARD_IY_REGNUM)))
#define REGNO_OK_FOR_INDEX_P(REGNO)	0

#define REGNO_OK_FOR_BASE_P(REGNO)                  \
    ((REGNO) < FIRST_PSEUDO_REGISTER ? REG_VALID_FOR_BASE_P (REGNO) \
          : (reg_renumber && REG_VALID_FOR_BASE_P (reg_renumber[REGNO])))

#define REGNO_OK_FOR_BASE_P2(REGNO,STRICT)          \
      ((STRICT) ? (REGNO_OK_FOR_BASE_P (REGNO)):REGNO_OK_FOR_BASE_NONSTRICT_P (REGNO))

extern short *reg_renumber; /* def in local_alloc.c */

/* Internal macro, the nonstrict definition for REGNO_OK_FOR_BASE_P.  */
#define REGNO_OK_FOR_BASE_NONSTRICT_P(REGNO) \
           ((REGNO) >= FIRST_PSEUDO_REGISTER \
             || REG_VALID_FOR_BASE_P (REGNO) \
             || (REGNO) == FRAME_POINTER_REGNUM \
            || (REGNO) == ARG_POINTER_REGNUM \
            || (reg_renumber && REG_VALID_FOR_BASE_P (reg_renumber[REGNO])))




/* see PUSH_POP_ADDRESS_P() below for an explanation of this.  */
#define IS_STACK_PUSH(operand) \
    ((GET_CODE (operand) == MEM) \
     && (GET_CODE (XEXP (operand, 0)) == PRE_DEC) \
     && (SP_REG_P (XEXP (XEXP (operand, 0), 0))))

#define IS_STACK_POP(operand) \
    ((GET_CODE (operand) == MEM) \
     && (GET_CODE (XEXP (operand, 0)) == POST_INC) \
     && (SP_REG_P (XEXP (XEXP (operand, 0), 0))))

/* 1 if X is an rtx for a constant that is a valid address.  */
#define CONSTANT_ADDRESS_P(X)	(CONSTANT_P (X))

/* Maximum number of registers that can appear in a valid memory address */
#define MAX_REGS_PER_ADDRESS	2

/* GO_IF_LEGITIMATE_ADDRESS recognizes an RTL expression that is a
   valid memory address for an instruction. The MODE argument is the
   machine mode for the MEM expression that wants to use this address.  */

/*--------------------------------------------------------------
   Valid addresses are either direct or indirect (MEM) versions
   of the following forms:
	constant		N
	register		,X
	indexed			N+X
--------------------------------------------------------------*/

/* The range of index that is allowed by indirect addressing. */

#define VALID_MIN_OFFSET z80_min_offset
#define VALID_MAX_OFFSET z80_max_offset

/* The offset values which are allowed by the n,x and n,y addressing modes.
*/

#define VALID_CONSTANT_OFFSET_P(X,MODE)		\
((GET_CODE (X) == CONST_INT) &&			\
 ((INTVAL (X) >= VALID_MIN_OFFSET)		\
    && ((INTVAL (X) <= VALID_MAX_OFFSET	\
		- (HOST_WIDE_INT) (GET_MODE_SIZE (MODE) + 1)))))

/* This is included to allow stack push/pop operations. Special hacks in the
   md and z80.c files exist to support this.  */
#define PUSH_POP_ADDRESS_P(X) \
  (((GET_CODE (X) == PRE_DEC) || (GET_CODE (X) == POST_INC)) \
	&& SP_REG_P (XEXP (X, 0)))

/* Go to ADDR if X is a valid address. */

#ifndef REG_OK_STRICT
#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR) \
{ \
  if (z80_go_if_legitimate_address ((X), (MODE), 0)) goto ADDR; \
}
#else
#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR)		 \
{							 \
  if (z80_go_if_legitimate_address ((X), (MODE), 1)) goto ADDR; \
}
#endif

/* The macros REG_OK_FOR..._P assume that the arg is a REG rtx and check its
   validity for a certain class.  We have two alternate definitions for each
   of them.  The usual definition accepts all pseudo regs; the other rejects
   them unless they have been allocated suitable hard regs.  The symbol
   REG_OK_STRICT causes the latter definition to be used.
  
   Most source files want to accept pseudo regs in the hope that they will
   get allocated to the class that the insn wants them to be in. Source files
   for reload pass need to be strict. After reload, it makes no difference,
   since pseudo regs have been eliminated by then.  */

#ifndef REG_OK_STRICT
/* Nonzero if X is a hard reg that can be used as a base reg.  */
#define REG_OK_FOR_BASE_P(X)   REGNO_OK_FOR_BASE_NONSTRICT_P(REGNO(X))
#define REG_OK_FOR_INDEX_P(X) 	(0)   

/* Nonzero if X is a hard reg that can be used as an index.  */
#else
#define REG_OK_FOR_BASE_P(X)   REGNO_OK_FOR_BASE_P(REGNO(X))
#define REG_OK_FOR_INDEX_P(X) 	(0)  
#endif


/* Try machine-dependent ways of modifying an illegitimate address
   to be legitimate.  If we find one, return the new, valid address.
   This macro is used in only one place: `memory_address' in explow.c.
  
   OLDX is the address as it was before break_out_memory_refs was called.
   In some cases it is useful to look at this to decide what needs to be done.
  
   MODE and WIN are passed so that this macro can use
   GO_IF_LEGITIMATE_ADDRESS.
  
   It is always safe for this macro to do nothing.
   It exists to recognize opportunities to optimize the output.  
 
 When the offset goes out of range in indexed addressing mode,
  we will force the displacement in to a register and add it to the ix register.
 May be that we can better handle this case in LEGITIMIZE_RELOAD_ADDRESS.
 
 */
#define LEGITIMIZE_ADDRESS(X, OLDX, MODE, WIN)               \
{                                    \
  if (GET_CODE (X) == PLUS                                              \
      && GET_CODE (XEXP (X, 0)) == REG                                  \
      && GET_CODE (XEXP (X, 1)) == CONST_INT                            \
      && !VALID_CONSTANT_OFFSET_P (XEXP (X, 1), MODE))                  \
    {                                                                   \
      rtx xop0 = XEXP (X, 0);                        \
      rtx xop1 = XEXP (X, 1);                        \
      xop1 = force_reg (Pmode, xop1);                 \
 	 (X) = force_reg (Pmode, gen_rtx_PLUS (Pmode, xop0, xop1)); \
      goto WIN;                         \
    }                                \
}

/* Go to LABEL if ADDR (a legitimate address expression)
   has an effect that depends on the machine mode it is used for.  */
#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR,LABEL)  \
{									\
  if (GET_CODE (ADDR) == PRE_DEC || GET_CODE (ADDR) == POST_DEC		\
      || GET_CODE (ADDR) == PRE_INC || GET_CODE (ADDR) == POST_INC)	\
    goto LABEL;								\
}

/* Nonzero if the constant value X is a legitimate general operand.
   It is given that X satisfies CONSTANT_P or is a CONST_DOUBLE.  */

#define LEGITIMATE_CONSTANT_P(X)	1


/* Tell final.c how to eliminate redundant test instructions.  
 need to check this. 
 This needs to be improved. (??)
 
 */

#define NOTICE_UPDATE_CC(EXP, INSN)                 \
{ if (GET_CODE (EXP) == SET)                        \
    {                                   \
      notice_update_cc_on_set(EXP, INSN);               \
    }                                   \
  else if (GET_CODE (EXP) == PARALLEL                   \
           && GET_CODE (XVECEXP (EXP, 0, 0)) == SET)            \
    {                                   \
      notice_update_cc_on_set(XVECEXP (EXP, 0, 0), INSN);       \
    }                                   \
  else if (GET_CODE (EXP) == CALL)                  \
    { /* all bets are off */ CC_STATUS_INIT; }              \
  if (cc_status.value1 && GET_CODE (cc_status.value1) == REG        \
      && cc_status.value2                       \
      && reg_overlap_mentioned_p (cc_status.value1, cc_status.value2))  \
    {                                   \
      printf ("here!\n");                       \
      cc_status.value2 = 0;                     \
    }                                   \
}

/* Cost macros : These are not fully relevant to z80. I have given some default
 values to these macros.  These needs to be defined correctly. The
 confusion here is we have instructions to operate on one byte operands and
 instructions to operate on 2 byte (size) operands, which have differsnt costs
 in themselves. */



/* Compute the cost of computing a constant rtl expression RTX whose rtx-code
   is CODE.  The body of this macro is a portion of a switch statement.  If
   the code is computed here, return it with a return statement. Otherwise,
   break from the switch.  */

/* The cost macros are given some default values or not exact values.
 These needs to be improved.
 
 Trying to fix these.....
 */

#define CONST_COSTS(RTX,CODE,OUTER_CODE) \
 case CONST_INT:			 \
    if (RTX == const0_rtx) return 0;	 \
 case CONST:				 \
    return 0;                            \
 case LABEL_REF:			 \
 case SYMBOL_REF:			 \
   return 1;				 \
 case CONST_DOUBLE:			 \
   return 0;

#define DEFAULT_RTX_COSTS(X,CODE,OUTER_CODE)		\
    return z80_rtx_costs (X, CODE, OUTER_CODE);

/* An expression giving the cost of an addressing mode that contains
   ADDRESS.  If not defined, the cost is computed from the ADDRESS
   expression and the `CONST_COSTS' values.  */

#define ADDRESS_COST(RTX) \
  ((CONSTANT_P (RTX)						\
    || (GET_CODE (RTX) == PLUS && CONSTANT_P (XEXP (RTX, 1))	\
	&& REG_P (XEXP (RTX, 0)))) ? 0				\
   : REG_P (RTX) ? 4						\
   : 2)

#define REGISTER_MOVE_COST(CLASS1, CLASS2)		\
		(((CLASS1) == Q_REGS && (CLASS2) == Q_REGS)?2:4) 

#define MEMORY_MOVE_COST(MODE,CLASS,IN)	\
	((MODE) == QImode ?4:6)

/* A C expression for the cost of a branch instruction.  A value of 1
   is the default; other values are interpreted relative to that.

   Pretend branches are cheap because GCC generates sub-optimal code
   for the default value.  */
#define BRANCH_COST 0

/* Nonzero if access to memory by bytes is slow and undesirable.  */
#define SLOW_BYTE_ACCESS	0

/* It is as good to call a constant function address as to call an address
   kept in a register.  */
#define NO_FUNCTION_CSE



/* Defining the Output Assembler Language.  */

/* A default list of other sections which we might be "in" at any given
   time.  For targets that use additional sections (e.g. .tdesc) you
   should override this definition in the target-specific file which
   includes this file.  */

/* Output before read-only data.  */
#define TEXT_SECTION_ASM_OP	("\t.section\t.text")

/* Output before writable data.  */
#define DATA_SECTION_ASM_OP	("\t.section\t.data")

/* Output before uninitialized data.  */
#define BSS_SECTION_ASM_OP 	("\t.section\t.bss")

/* This is how to begin an assembly language file.  Most svr4 assemblers want
   at least a .file directive to come first, and some want to see a .version
   directive come right after that.  Here we just establish a default
   which generates only the .file directive.  If you need a .version
   directive for any specific target, you should override this definition
   in the target-specific file which includes this one.  */

#undef ASM_FILE_START
#define ASM_FILE_START(FILE)                            \
    z80_asm_file_start ((FILE), main_input_filename)

/* Comment character */
#define ASM_COMMENT_START	"#"

/* Output to assembler file text saying following lines
   may contain character constants, extra white space, comments, etc.  */
#define ASM_APP_ON 		"# Begin inline assembler code\n#APP\n"

/* Output to assembler file text saying following lines
   no longer contain unusual constructs.  */
#define ASM_APP_OFF 		"# End of inline assembler code\n#NO_APP\n"

/* Output #ident as a .ident.  */

/* This is how to output a `long double' extended real constant.  */

#define ASM_OUTPUT_LONG_DOUBLE(FILE,VALUE)  				\
    ASM_OUTPUT_DOUBLE(FILE,VALUE)

/* This is how to output an assembler line defining a `double' constant.  */

#define ASM_OUTPUT_DOUBLE(FILE,VALUE)					\
do { long l[2];								\
     REAL_VALUE_TO_TARGET_DOUBLE (VALUE, l);				\
     fprintf (FILE, "\t%s\t0x%lx,0x%lx\n", ASM_LONG, l[0], l[1]);	\
   } while (0)

/* This is how to output an assembler line defining a `float' constant.  */

#define ASM_OUTPUT_FLOAT(FILE,VALUE)			\
do { long l;						\
     REAL_VALUE_TO_TARGET_SINGLE (VALUE, l);		\
     fprintf ((FILE), "\t%s\t0x%lx\n", ASM_LONG, l);	\
   } while (0)

/* This is how to output an assembler line defining a `long' constant.  */
#define ASM_OUTPUT_INT(FILE,VALUE)		        \
( fprintf (FILE, "\t%s\t", ASM_LONG),			\
  output_addr_const (FILE, (VALUE)),			\
  fprintf (FILE, "\n"))

/* Likewise for `char' and `short' constants.  */
#define ASM_OUTPUT_SHORT(FILE,VALUE)			\
( fprintf (FILE, "\t%s\t", ASM_SHORT),			\
  output_addr_const (FILE, (VALUE)),			\
  fprintf (FILE, "\n"))

/* This is how to output an assembler line for a numeric constant byte.  */
#define ASM_OUTPUT_CHAR(FILE,VALUE)			\
( fprintf (FILE, "\t%s\t", ASM_BYTE_OP),		\
  output_addr_const (FILE, (VALUE)),			\
  putc ('\n', FILE))

#define ASM_OUTPUT_BYTE(FILE,VALUE)			\
  fprintf ((FILE), "%s 0x%x\n", ASM_BYTE_OP, (VALUE))


/* Define the parentheses used to group arithmetic operations in assembler
 * code.  
 */
#define ASM_OPEN_PAREN		"("
#define ASM_CLOSE_PAREN		")"

/* This is how to output the definition of a user-level label named NAME,
   such as the label on a static function or variable NAME.  */

#define ASM_OUTPUT_LABEL(FILE,NAME)	\
  do { assemble_name (FILE, NAME); fputs (":\n", FILE); } while (0)


/* This is how to output a command to make the user-level label named NAME
   defined for reference from other files.  */

#define ASM_GLOBALIZE_LABEL(FILE,NAME)	\
  do { fprintf (FILE, "%s ", GLOBAL_ASM_OP);		\
       assemble_name (FILE, NAME);			\
       fputs ("\n", FILE);} while (0)

/* output external reference */
#define ASM_OUTPUT_EXTERNAL(FILE,DECL,NAME) \
  {fputs ("\t# extern\t", FILE); \
  assemble_name (FILE, NAME); \
  fputs ("\n", FILE);}



/* Store in OUTPUT a string (made with alloca) containing
   an assembler-name for a local static variable named NAME.
   LABELNO is an integer which is different for each call.  */

#define ASM_FORMAT_PRIVATE_NAME(OUTPUT, NAME, LABELNO)	\
( (OUTPUT) = (char *) alloca (strlen ((NAME)) + 10),	\
  sprintf ((OUTPUT), "%s.%d", (NAME), (LABELNO)))

/* How to refer to registers in assembler output.  This sequence is indexed
   by compiler's hard-register-number (see above).  */
#define REGISTER_NAMES						\
{ "a", "hl", "de", "bc", "iy", "ix", "sp", "pc", "h",		\
  "l", "d", "e", "b", "c", "f","ixl", "ixh", "iyl", "iyh"}


/* Output a float value (represented as a C double) as an immediate operand.
   This macro is a 68k-specific macro.  */

#define ASM_OUTPUT_FLOAT_OPERAND(CODE,FILE,VALUE)		\
 do {								\
      long l;							\
      REAL_VALUE_TO_TARGET_SINGLE (VALUE, l);			\
      asm_fprintf ((FILE), "%I0x%lx", l);			\
     } while (0)

/* Output a double value (represented as a C double) as an immediate operand.
   This macro is a 68k-specific macro.  */
#define ASM_OUTPUT_DOUBLE_OPERAND(FILE,VALUE)				\
 do { char dstr[30];							\
      REAL_VALUE_TO_DECIMAL (VALUE, "%.20g", dstr);			\
      asm_fprintf (FILE, "%I0r%s", dstr);				\
    } while (0)

/* Note, long double immediate operands are not actually
   generated by m68k.md.  */
#define ASM_OUTPUT_LONG_DOUBLE_OPERAND(FILE,VALUE)			\
 do { char dstr[30];							\
      REAL_VALUE_TO_DECIMAL (VALUE, "%.20g", dstr);			\
      asm_fprintf (FILE, "%I0r%s", dstr);				\
    } while (0)

/* Print an instruction operand X on file FILE. CODE is the code from the
   %-spec for printing this operand. If `%z3' was used to print operand
   3, then CODE is 'z'.  */

#define PRINT_OPERAND(FILE, X, CODE)					\
{									\
	if (CODE == '#') 						\
		fprintf (FILE, "#");					\
	else if (CODE == 'h')						\
	{								\
	if (GET_CODE (X) == REG && GET_MODE (X) == QImode && !Q_REG_P(X))\
		X = z80_gen_highpart (QImode, X);			\
	fprintf (FILE, "%s", reg_names[REGNO (X)]);            		\
	}								\
	else if (CODE == 'H')						\
	{								\
	if (GET_CODE (X) == REG && H_REG_P (X))\
		X = z80_gen_highpart (QImode, X);			\
	fprintf (FILE, "%s", reg_names[REGNO (X)]);            		\
	}								\
	else if (GET_CODE (X) == REG)					\
	{								\
		if (!Q_REG_P (X)  && GET_MODE (X) ==QImode)    	        \
			X = z80_gen_lowpart (QImode, X);      	        \
		fprintf (FILE, "%s", reg_names[REGNO (X)]);		\
	}               						\
	else if (GET_CODE (X) == PLUS){					\
		print_operand_address (FILE, X); \
	}\
	else if (GET_CODE (X) == MEM)					\
		output_address (XEXP (X, 0));				\
	else if (GET_CODE (X) == CONST_DOUBLE && GET_MODE (X) != SImode)\
	{								\
		union { double d; int i[2]; } u;			\
		u.i[0]=CONST_DOUBLE_LOW (X); u.i[1]=CONST_DOUBLE_HIGH (X);\
		fprintf (FILE, "%.20e", u.d);				\
	}								\
	else								\
	{								\
		output_addr_const (FILE, X);		\
	}								\
}


/* Print a memory operand whose address is X, on file FILE. */
#define PRINT_OPERAND_ADDRESS(FILE, ADDR) \
  print_operand_address (FILE, ADDR)

/* This is how to output an insn to push/pop a register on the stack.
   It need not be very fast code.  
*/


#define ASM_OUTPUT_REG_PUSH(FILE,REGNO)			\
  fprintf (FILE, "\tpush\t%s\n", reg_names[REGNO])

#define ASM_OUTPUT_REG_POP(FILE,REGNO)			\
  fprintf (FILE, "\tpop\t%s\n", reg_names[REGNO])

/* This is how to output an element of a case-vector that is relative. */

#define ASM_OUTPUT_ADDR_DIFF_ELT(FILE, BODY, VALUE, REL) \
  asm_fprintf (FILE, "\t%s\tL%d-L%d\n", ASM_SHORT, VALUE, REL)

/* This is how to output an element of a case-vector that is absolute. */
#define ASM_OUTPUT_ADDR_VEC_ELT(FILE, VALUE) \
  asm_fprintf (FILE, "\t%s\t.L%d\n", ASM_SHORT, VALUE)

#undef ASM_OUTPUT_LABELREF
#define ASM_OUTPUT_LABELREF(FILE, NAME)	          \
  do {                                            \
  char* real_name;                                \
  STRIP_NAME_ENCODING (real_name, (NAME));        \
  fprintf (FILE, "_%s", real_name);               \
  } while (0)           

/* This is how to output an assembler line that says to advance the
   location counter to a multiple of 2**LOG bytes.  */
#define ASM_OUTPUT_ALIGN(FILE,LOG)			\
  do {                                                  \
      if ((LOG) > 1)                                    \
          asm_fprintf ((FILE), "\t%s\n", ALIGN_ASM_OP); \
  } while (0)


/* Assembler Commands for Exception Regions.  */

/* Default values provided by GCC should be ok. Assumming that DWARF-2
   frame unwind info is ok for this platform. */

/* How to renumber registers for dbx and gdb. */
#define DBX_REGISTER_NUMBER(REGNO) \
 ((REGNO))

#undef PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

/* The prefix for local labels.  You should be able to define this as
   an empty string, or any arbitrary string (such as ".", ".L%", etc)
   without having to make any other changes to account for the specific
   definition.  Note it is a string literal, not interpreted by printf
   and friends. */
#define LOCAL_LABEL_PREFIX "."

/* The prefix for immediate operands.  */
//#define IMMEDIATE_PREFIX "#"
#define GLOBAL_ASM_OP   ".globl"
#define ASM_LONG        ".long"
#define ASM_SHORT       ".word"


/* Miscellaneous Parameters.  */

/* Define the codes that are matched by predicates in m68hc11.c.  */
#define PREDICATE_CODES \
{"stack_register_operand",   {SUBREG, REG}},				\
{"hard_reg_operand",         {SUBREG, REG}},				\
{"non_push_operand",         {SUBREG, REG, MEM}},			\
{"reg_or_some_mem_operand",  {SUBREG, REG, MEM}},			\
{"tst_operand",              {SUBREG, REG, MEM}},			\
{"cmp_operand",              {SUBREG, REG, MEM, SYMBOL_REF, LABEL_REF,	\
			     CONST_INT, CONST_DOUBLE}},

/* Specify the machine mode that this machine uses
   for the index in the tablejump instruction.  */
#define CASE_VECTOR_MODE	Pmode

/* Specify the tree operation to be used to convert reals to integers. */
#define IMPLICIT_FIX_EXPR	FIX_ROUND_EXPR

/* This flag, if defined, says the same insns that convert to a signed fixnum
   also convert validly to an unsigned one.  */
#define FIXUNS_TRUNC_LIKE_FIX_TRUNC

/* This is the kind of divide that is easiest to do in the general case.  */
#define EASY_DIV_EXPR		TRUNC_DIV_EXPR

/* Max number of bytes we can move from memory to memory in one
   reasonably fast instruction.  */
#define MOVE_MAX 		2

/* MOVE_RATIO is the number of move instructions that is better than a
   block move.  Make this small on 6811, since the code size grows very
   large with each move.  */
#define MOVE_RATIO		3

/* Define if shifts truncate the shift count which implies one can omit
   a sign-extension or zero-extension of a shift count.  */
#define SHIFT_COUNT_TRUNCATED	1

/* Value is 1 if truncating an integer of INPREC bits to OUTPREC bits
   is done just by pretending it is already truncated.  */
#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC)	1

/* Specify the machine mode that pointers have. After generation of rtl, the
   compiler makes no further distinction between pointers and any other
   objects of this machine mode.  */
#define Pmode			HImode

/* A function address in a call instruction is a byte address (for indexing
   purposes) so give the MEM rtx a byte's mode.  */
#define FUNCTION_MODE		QImode

/* define SCCS_DIRECTIVE if SCCS directives should be ignored */
#define SCCS_DIRECTIVE		1

/* Allow $ in identifiers */
#define DOLLARS_IN_IDENTIFIERS	1

extern long z80_min_offset;
extern long z80_max_offset;
extern int current_function_interrupt;
extern int current_function_trap;
extern rtx_ptr z80_compare_op0;
extern rtx_ptr z80_compare_op1;

