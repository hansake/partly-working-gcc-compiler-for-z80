/* Subroutines for gcc2 for z80.
   Copyright (C) 1994, 1995, 1996, 1997, 1999 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include <stdio.h>
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "tree.h"
#include "insn-config.h"
#include "conditions.h"
#include "insn-flags.h"
#include "output.h"
#include "insn-attr.h"
#include "flags.h"
#include "recog.h"
#include "z80-protos.h"


extern char *version_string;

static rtx find_addr_reg (); 
rtx ix_reg;
rtx iy_reg;
rtx stack_push_word;
rtx stack_pop_word;


/* Set to 1 by expand_prologue() when the function is an interrupt handler.  */
int current_function_interrupt;

/* Set to 1 by expand_prologue() when the function is a trap handler.  */
int current_function_trap;
static int register_indirect_p PARAMS((rtx, enum machine_mode, int));
static rtx z80_expand_compare PARAMS((enum rtx_code, rtx, rtx));
static int z80_auto_inc_p PARAMS ((rtx));
/*These are used to store the compare operands of cmp/tsts for later use in branches. */

rtx z80_compare_op0;
rtx z80_compare_op1;

long  z80_min_offset = -128;

/* Max offset that is valid for the indirect addressing mode.  */
long z80_max_offset = 127;

/* Value is 1 if hard register REGNO can hold a value of machine-mode MODE.
    - 8 bit values are stored anywhere (except the SP register and IX, IY).
    - 16 bit values can be stored in any register whose mode is 16
    - 32 bit values can be stored in hl + de registers or in bc + iy  */

      
int
hard_regno_mode_ok (regno, mode)
     int regno;
     enum machine_mode mode;
{
  switch (GET_MODE_SIZE (mode))
    {

    case 4:
	if(mode ==SImode || mode == SFmode)    
    		 return  HL_REGNO_P (regno) || BC_REGNO_P (regno);

    case 2:
      return H_REGNO_P (regno);

    case 1:
      /* Let us not accept IX and IY registers in QI mode. Most of the
	    instructions does not accept ir registers.  Seems to be okay. */

      return G_REGNO_P (regno) && !SP_REGNO_P (regno) && !A_REGNO_P (regno);

    default:
      return 0;
    }
}

/* This function determines whether op is symbolic memory or not. */

int
symbolic_memory_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{

	if (GET_CODE (op) != MEM)
		return 0;
	op = XEXP (op,0);
	switch (GET_CODE (op))
	{
    case SYMBOL_REF:
    case LABEL_REF:
      return 1;
    case CONST:
      op = XEXP (op, 0);
      return (GET_CODE (XEXP (op, 0)) == SYMBOL_REF
	      || GET_CODE (XEXP (op, 0)) == LABEL_REF)
	     && GET_CODE (XEXP (op, 1)) == CONST_INT;

      /* ??? This clause seems to be irrelevant.  */
    case CONST_DOUBLE:
      return GET_MODE (op) == mode;

    case PLUS:
      return symbolic_memory_operand (XEXP (op, 0), mode)
	&& symbolic_memory_operand (XEXP (op, 1), mode);

    default:
      return 0;
    }
}

int
reg_or_some_mem_operand (operand, mode)
     rtx operand;
     enum machine_mode mode;
{
  if (GET_CODE (operand) == MEM)
    {
      rtx op = XEXP (operand, 0);

      if (symbolic_memory_operand (op, mode))
	return 1;

      if (IS_STACK_PUSH (operand))
	return 1;

      if (z80_register_indirect_p (operand, mode))
	return 1;

      return 0;
    }

  return register_operand (operand, mode);
}
	
/* Returns 1 if the operand fits in a z80 indirect mode 
  mem:MODE (REG) or mem: MODE (IX + d)   */

int
z80_small_indexed_indirect_p (operand, mode)
     rtx operand;
     enum machine_mode mode;
{
  rtx base, offset;

  if (GET_CODE (operand) != MEM)
    return 0;

  operand = XEXP (operand, 0);
  /* if (REG_P (operand) && (REG_OK_FOR_BASE_P (operand) || HL_REG_P (operand)))
	   return 1;
	*/

  if (!register_indirect_p (operand, mode,
                            (reload_completed | reload_in_progress)))
    return 0;

  return 1;
}
int 
register_indirect_operand (operand, mode)
		rtx operand;
		enum machine_mode mode;
{
		return register_indirect_p (operand, mode, 1);
}




static int
register_indirect_p (operand, mode, strict)
     rtx operand;
     enum machine_mode mode;
     int strict;
{
  rtx base, offset;

  switch (GET_CODE (operand))
    {
    case POST_INC:
    case PRE_INC:
    case POST_DEC:
    case PRE_DEC:
      return 0;

    case PLUS:
      base = XEXP (operand, 0);
      if (GET_CODE (base) != REG)
	return 0;

      offset = XEXP (operand, 1);
      if ((GET_CODE (offset) != CONST_INT) || !VALID_CONSTANT_OFFSET_P(offset, mode)) 
	return 0;

	  if (strict == 0)
	    return 1;
	 else
	   return REGNO_OK_FOR_BASE_P2 (REGNO (base), strict);
      
    case REG:
/*	return REGNO_OK_FOR_BASE_P2 (REGNO (base), strict); bug, 
             base is not initialized, changed 2020-10-22 */
	return REGNO_OK_FOR_BASE_P2 (REGNO (operand), strict);

    default:
      return 0;
    }
}

constant_memory_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
  return GET_CODE (op) == MEM && CONSTANT_ADDRESS_P (XEXP (op, 0));
}


int
z80_register_indirect_p (operand, mode)
     rtx operand;
     enum machine_mode mode;
{
  if (GET_CODE (operand) != MEM)
    return 0;

  operand = XEXP (operand, 0);
  return register_indirect_p (operand, mode,
                              (reload_completed | reload_in_progress));
}

int
tst_operand (operand, mode)
     rtx operand;
     enum machine_mode mode;
{
  if (GET_CODE (operand) == MEM)
    {
      rtx addr = XEXP (operand, 0);
      if (z80_auto_inc_p (addr))
	return 0;
    }
  return nonimmediate_operand (operand, mode);
}

int
cmp_operand (operand, mode)
     rtx operand;
     enum machine_mode mode;
{
  if (GET_CODE (operand) == MEM)
    {
      rtx addr = XEXP (operand, 0);
      if (z80_auto_inc_p (addr))
	return 0;
    }
  return general_operand (operand, mode);
}

static int
go_if_legitimate_address_internal (operand, mode, strict)
     rtx operand;
     enum machine_mode mode;
     int strict;
{
  if (CONSTANT_ADDRESS_P (operand))
    {
      return 1;
    }
  if (register_indirect_p (operand, mode, strict))
    {
      return 1;
    }
  if (PUSH_POP_ADDRESS_P (operand))
    {
      return 1;
    }
/*  if (symbolic_memory_operand (operand, mode))
    {
      return 1;
    }
*/
  return 0;
}

int
z80_go_if_legitimate_address (operand, mode, strict)
     rtx operand;
     enum machine_mode mode;
     int strict;
{
  int result;

  result = go_if_legitimate_address_internal (operand, mode, strict);

  return result;
}

int
stack_register_operand (operand, mode)
     rtx operand;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
  return SP_REG_P (operand);
}

int
non_stack_register_operand (operand, mode)
     rtx operand;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
		if (GET_CODE (operand) == REG)
				return !SP_REG_P (operand);
		 return general_operand (operand, mode);
}


int
address_register (rtx op)
{
	
	if (GET_CODE (op) == REG && A_REG_P (op))
			  return 1;
	else
			return 0;
}


int
memory_reload_operand (operand, mode)
     rtx operand;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{

	return GET_CODE (operand) == MEM
    && GET_CODE (XEXP (operand, 0)) == PLUS
    && ((GET_CODE (XEXP (XEXP (operand, 0), 0)) == REG
	 && GET_CODE (XEXP (XEXP (operand, 0), 1)) == CONST_INT)
	|| (GET_CODE (XEXP (XEXP (operand, 0), 1)) == REG
	    && GET_CODE (XEXP (XEXP (operand, 0), 0)) == CONST_INT));
}

/* Split a  SI move into several smaller move operations.
   The scratch register 'scratch' is used as a temporary to load
   store intermediate values.  It must be a hard register.  */
void
z80_split_move (to, from, scratch)
     rtx to, from, scratch;
{
  rtx low_to, low_from;
  rtx high_to, high_from;
  enum machine_mode mode;

  mode = GET_MODE (to);
  if (GET_MODE_SIZE (mode) == 4)
    mode = HImode;
  else
	abort ();

  low_to = z80_gen_lowpart (mode, to);
  high_to = z80_gen_highpart (mode, to);

  low_from = z80_gen_lowpart (mode, from);
  high_from = z80_gen_highpart (mode, from);

  if (H_REG_P (to) || H_REG_P (from))
    {
      emit_move_insn (low_to, low_from);
      emit_move_insn (high_to, high_from);
    }
  else
    {
      rtx insn;

      emit_move_insn (scratch, low_from);
      insn = emit_move_insn (low_to, scratch);

      emit_move_insn (scratch, high_from);
      insn = emit_move_insn (high_to, scratch);
    }
}
int
non_push_operand (operand, mode)
     rtx operand;
     enum machine_mode mode;
{
  if (general_operand (operand, mode) == 0)
    return 0;

  if (push_operand (operand, mode) == 1)
    return 0;
  return 1;
}
/* Define this macro if references to a symbol must be treated
   differently depending on something about the variable or function
   named by the symbol (such as what section it is in).

   For the z80, we want to recognize trap handlers so that we
   handle calls to traps in a special manner (by issuing the trap).
   This information is stored in SYMBOL_REF_FLAG.  */
void
z80_encode_section_info (decl)
     tree decl;
{
  tree func_attr;
  int trap_handler;
  rtx rtl;

  if (TREE_CODE (decl) != FUNCTION_DECL)
    return;

  rtl = DECL_RTL (decl);

  func_attr = TYPE_ATTRIBUTES (TREE_TYPE (decl));
  trap_handler = lookup_attribute ("trap", func_attr) != NULL_TREE;
  SYMBOL_REF_FLAG (XEXP (rtl, 0)) = trap_handler;
}


/* Returns true if X is a PRE/POST increment decrement
   (same as auto_inc_p() in rtlanal.c but do not take into
   account the stack).  */
int
z80_auto_inc_p (x)
     rtx x;
{
  return GET_CODE (x) == PRE_DEC
    || GET_CODE (x) == POST_INC
    || GET_CODE (x) == POST_DEC || GET_CODE (x) == PRE_INC;
}
rtx
z80_gen_lowpart (mode, x)
     enum machine_mode mode;
     rtx x;
{
  /* We assume that the low part of an auto-inc mode is the same with
     the mode changed and that the caller split the larger mode in the
     correct order.  */
  if (GET_CODE (x) == MEM && z80_auto_inc_p (XEXP (x, 0)))
    {
      return gen_rtx (MEM, mode, XEXP (x, 0));
    }

  /* Note that a CONST_DOUBLE rtx could represent either an integer or a
     floating-point constant.  A CONST_DOUBLE is used whenever the
     constant requires more than one word in order to be adequately
     represented.  */
  if (GET_CODE (x) == CONST_DOUBLE)
    {
      long l[2];

      if (GET_MODE_CLASS (GET_MODE (x)) == MODE_FLOAT)
	{
	  REAL_VALUE_TYPE r;

	  if (GET_MODE (x) == SFmode)
	    {
	      REAL_VALUE_FROM_CONST_DOUBLE (r, x);
	      REAL_VALUE_TO_TARGET_SINGLE (r, l[0]);
	    }
	  else
	    {
	      rtx first, second;

	      split_double (x, &first, &second);
	      return second;
	    }
	  if (mode == SImode)
	    return gen_rtx (CONST_INT, VOIDmode, l[0]);

	  return gen_rtx (CONST_INT, VOIDmode, l[0] & 0x0ffff);
	}
      else
	{
	  l[0] = CONST_DOUBLE_LOW (x);
	}
      if (mode == SImode)
	return gen_rtx (CONST_INT, VOIDmode, l[0]);
      else if (mode == HImode && GET_MODE (x) == SFmode)
	return gen_rtx (CONST_INT, VOIDmode, l[0] & 0x0FFFF);
      else
	abort ();
    }

  if (mode == QImode)  
  {
     if(BC_REG_P (x))
    	return gen_rtx (REG, mode, HARD_C_REGNUM);
     else if(HL_REG_P (x))
	     return gen_rtx(REG, mode, HARD_L_REGNUM);
     else if(DE_REG_P (x))
	     return gen_rtx(REG, mode, HARD_E_REGNUM);
	else if (IX_REG_P (x))
			return gen_rtx_REG (mode, HARD_IXH_REGNUM);
	else if (IY_REG_P (x))
			return gen_rtx_REG (mode, HARD_IYH_REGNUM);

  }
if (mode == HImode)
{
	if (HL_REG_P(x))
		return gen_rtx_REG (HImode, HARD_HL_REGNUM);
	else if (BC_REG_P (x))
			return gen_rtx_REG (HImode, HARD_BC_REGNUM);

}

  
  /* gen_lowpart crashes when it is called with a SUBREG.  */
  if (GET_CODE (x) == SUBREG && SUBREG_WORD (x) != 0)
    {
      if (mode == SImode)
	return gen_rtx_SUBREG (mode, SUBREG_REG (x), SUBREG_WORD (x) + 2);
      else if (mode == HImode)
	return gen_rtx_SUBREG (mode, SUBREG_REG (x), SUBREG_WORD (x) + 1);
      else
	abort ();
    }
  x = gen_lowpart (mode, x);

  /* Return a different rtx to avoid to share it in several insns
     (when used by a split pattern).  Sharing addresses within
     a MEM breaks the Z register replacement (and reloading).  */
  if (GET_CODE (x) == MEM)
    x = copy_rtx (x);
  return x;
}

rtx
z80_gen_highpart (mode, x)
     enum machine_mode mode;
     rtx x;
{
  /* We assume that the high part of an auto-inc mode is the same with
     the mode changed and that the caller split the larger mode in the
     correct order.  */
  if (GET_CODE (x) == MEM && z80_auto_inc_p (XEXP (x, 0)))
    {
      return gen_rtx (MEM, mode, XEXP (x, 0));
    }

  /* Note that a CONST_DOUBLE rtx could represent either an integer or a
     floating-point constant.  A CONST_DOUBLE is used whenever the
     constant requires more than one word in order to be adequately
     represented.  */
  if (GET_CODE (x) == CONST_DOUBLE)
    {
      long l[2];

      if (GET_MODE_CLASS (GET_MODE (x)) == MODE_FLOAT)
	{
	  REAL_VALUE_TYPE r;

	  if (GET_MODE (x) == SFmode)
	    {
	      REAL_VALUE_FROM_CONST_DOUBLE (r, x);
	      REAL_VALUE_TO_TARGET_SINGLE (r, l[1]);
	    }
	  else
	    {
	      rtx first, second;

	      split_double (x, &first, &second);
	      return first;
	    }
	  if (mode == SImode)
	    return gen_rtx (CONST_INT, VOIDmode, l[1]);

	  return gen_rtx (CONST_INT, VOIDmode, (l[1] >> 16) & 0x0ffff);
	}
      else
	{
	  l[1] = CONST_DOUBLE_HIGH (x);
	}

      if (mode == SImode)
	return gen_rtx (CONST_INT, VOIDmode, l[1]);
      else if (mode == HImode && GET_MODE_CLASS (GET_MODE (x)) == MODE_FLOAT)
	return gen_rtx (CONST_INT, VOIDmode, (l[0] >> 16) & 0x0FFFF);
      else
	abort ();
    }
  if (GET_CODE (x) == CONST_INT)
    {
      HOST_WIDE_INT val = INTVAL (x);

      if (mode == QImode)
	{
	  return gen_rtx (CONST_INT, VOIDmode, val >> 8);
	}
      else if (mode == HImode)
	{
	  return gen_rtx (CONST_INT, VOIDmode, val >> 16);
	}
    }

  if (mode == QImode)  
  {
     if(BC_REG_P (x))
    	return gen_rtx (REG, mode, HARD_B_REGNUM);
     else if(HL_REG_P (x))
	     return gen_rtx(REG, mode, HARD_H_REGNUM);
     else if(DE_REG_P (x))
	     return gen_rtx(REG, mode, HARD_D_REGNUM);
	else if (IX_REG_P (x))
			return gen_rtx_REG (mode, HARD_IXH_REGNUM);
	else if (IY_REG_P (x))
			return gen_rtx_REG (mode, HARD_IYH_REGNUM);

  }
if (mode == HImode)
{
		if (HL_REG_P (x))
				return gen_rtx_REG (HImode, HARD_DE_REGNUM);
		else if (BC_REG_P (x))
				return gen_rtx_REG (HImode, HARD_IY_REGNUM);
}

  /* gen_highpart crashes when it is called with a SUBREG.  */
  if (GET_CODE (x) == SUBREG && SUBREG_WORD (x) != 0)
    {
      return gen_rtx (SUBREG, mode, XEXP (x, 0), XEXP (x, 1));
    }
  x = gen_highpart (mode, x);

  /* Return a different rtx to avoid to share it in several insns
     (when used by a split pattern).  Sharing addresses within
     a MEM breaks the Z register replacement (and reloading).  */
  if (GET_CODE (x) == MEM)
    x = copy_rtx (x);
  return x;  
}


static rtx
z80_expand_compare (code, op0, op1)
     enum rtx_code code;
     rtx op0, op1;
{
  rtx ret = 0;

  if (GET_MODE_CLASS (GET_MODE (op0)) == MODE_FLOAT)
    abort ();
  else
    {
      emit_insn (gen_rtx_SET (VOIDmode, cc0_rtx,
			      gen_rtx_COMPARE (VOIDmode, op0, op1)));
      ret = gen_rtx (code, VOIDmode, cc0_rtx, const0_rtx);
    }

  return ret;
}

/* output the compare and branch instructions.We make use of the operands saved 
 in cmp patterns. For SImode you need to have two compares and branch.
first you need to compare the lowparts 
if the condition is eq or ne you can check for the reverse condition 
and branch. Otherwise compare the highparts and then branch. 
Check for the proper placement of labels here....


Also we do n't have any compare instructions to branch on LE and GT. So when
the condition code is LE or GT we swap the operands and make the condition code
swap_conditioncode.
*/

		
rtx
z80_expand_compare_and_branch (code, op0, op1, label,isout)
     enum rtx_code code;
     rtx op0, op1, label;  
     int isout;
{
  rtx tmp;
  enum machine_mode  mode;
  rtx operands[2];   
  rtx label1;
  tmp = op0;
  if (GET_CODE (op0) == CONST_INT)
		  tmp = op1;

  switch (GET_MODE (tmp))
    {
    case QImode:
    case HImode:
	mode = GET_MODE (tmp);
	if (code == GT || code == LE || code == GTU || code == LEU)
	{
			rtx tmp = op0;
			op0 = op1;
			op1 = tmp;
			code = swap_condition (code);
	}
   if (GET_CODE (op0) != REG)
	    op0 = force_reg (mode, op0);
    if (GET_CODE (op1) != REG)
	   op1 = force_reg (mode, op1);
    
   	if (isout == 1)
      {
      tmp = z80_expand_compare (code, op0, op1);
		debug_rtx (tmp);

      tmp = gen_rtx_IF_THEN_ELSE (VOIDmode, tmp,
				  gen_rtx_LABEL_REF (VOIDmode, label),
				  pc_rtx);
		debug_rtx (tmp);
       tmp = emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx, tmp));

	  }
// This case for outputting a comparision of lowpart of a long value.  
  
	else if (isout == 0)
	{
	 tmp = z80_expand_compare (code, op0, op1);
 	  
	  if (code == EQ || code == NE)
	 {
      tmp = gen_rtx_IF_THEN_ELSE (VOIDmode, tmp,
				  gen_rtx_LABEL_REF (VOIDmode, label),
				  pc_rtx);
      emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx, tmp));
	 }
	}
// This case for outputting a comparision of high part of a long value.
	 
	else if (isout == 2)
	{
			//you are braching for a comparision of the high part of an
			// SImode value.
	emit_insn (gen_rtx_SET (VOIDmode, op0,  
	gen_rtx_MINUS (HImode, gen_rtx_MINUS (HImode,
	gen_rtx_REG (HImode, 14), op0),
	op1)));
   
  tmp  = gen_rtx (code, VOIDmode, cc0_rtx, const0_rtx);
		
      tmp = gen_rtx_IF_THEN_ELSE (VOIDmode, tmp,
				  gen_rtx_LABEL_REF (VOIDmode, label),
				  pc_rtx);
      emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx, tmp));

   }

      return 0;
    case SImode:
      /* Expand SImode branch into multiple compare+branch.  */
      {
	rtx lo[2], hi[2], label2;
	enum rtx_code code1, code2, code3;

	
	lo[0] = z80_gen_lowpart (HImode, op0);
	lo[1] = z80_gen_lowpart (HImode, op1);
	hi[0] = z80_gen_highpart (HImode, op0);
	hi[1] = z80_gen_highpart (HImode, op1);
	
	label2 = gen_label_rtx ();
	code1 = code;
	if (code == EQ || code == NE)
			code1 = reverse_condition (code1);

	z80_expand_compare_and_branch (code1, lo[0], lo[1], label2,0);
	z80_expand_compare_and_branch (code, hi[0], hi[1], label,2);
	
	if (code == EQ || code == NE)
			emit_label (label2);



	return 0;
      }

    default:
     abort ();
    }
  return 0;
}


/* Finds backward in the instructions to see if register 'reg' is
   dead.  This is used when generating code to see if we can use 'reg'
   as a scratch register.  This allows us to choose a better generation
   of code when we know that some register dies or can be clobbered.  */

int
dead_register_here (x, reg)
     rtx x;
     rtx reg;
{
  rtx x_reg;
  rtx p;

  if (HL_REG_P (reg))
    x_reg = gen_rtx (REG, SImode, HARD_HL_REGNUM);
  else
    x_reg = 0;

  for (p = PREV_INSN (x); p && GET_CODE (p) != CODE_LABEL; p = PREV_INSN (p))
    if (GET_RTX_CLASS (GET_CODE (p)) == 'i')
      {
	rtx body;

	body = PATTERN (p);

	if (GET_CODE (body) == CALL_INSN)
	  break;
	if (GET_CODE (body) == JUMP_INSN)
	  break;

	if (GET_CODE (body) == SET)
	  {
	    rtx dst = XEXP (body, 0);

	    if (GET_CODE (dst) == REG && REGNO (dst) == REGNO (reg))
	      break;
	    if (x_reg && rtx_equal_p (dst, x_reg))
	      break;

	    if (find_regno_note (p, REG_DEAD, REGNO (reg)))
	      return 1;
	  }
	else if (reg_mentioned_p (reg, p)
		 || (x_reg && reg_mentioned_p (x_reg, p)))
	  break;
      }

  /* Scan forward to see if the register is set in some insns and never
     used since then. */
  for (p = x /*NEXT_INSN (x) */ ; p; p = NEXT_INSN (p))
    {
      rtx body;

      if (GET_CODE (p) == CODE_LABEL
	  || GET_CODE (p) == JUMP_INSN
	  || GET_CODE (p) == CALL_INSN || GET_CODE (p) == BARRIER)
	break;

      if (GET_CODE (p) != INSN)
	continue;

      body = PATTERN (p);
      if (GET_CODE (body) == SET)
	{
	  rtx src = XEXP (body, 1);
	  rtx dst = XEXP (body, 0);

	  if (GET_CODE (dst) == REG
	      && REGNO (dst) == REGNO (reg) && !reg_mentioned_p (reg, src))
	    return 1;
	}

      /* Register is used (may be in source or in dest). */
      if (reg_mentioned_p (reg, p)
	  || (x_reg != 0 && GET_MODE (p) == SImode
	      && reg_mentioned_p (x_reg, p)))
	break;
    }
  return p == 0 ? 1 : 0;
}
	
static rtx
find_addr_reg (addr)
     rtx addr;
{
  while (GET_CODE (addr) == PLUS)
    {
      if (GET_CODE (XEXP (addr, 0)) == REG)
	addr = XEXP (addr, 0);
      if (GET_CODE (XEXP (addr, 1)) == REG)
	addr = XEXP (addr, 1);
      if (CONSTANT_P (XEXP (addr, 0)))
	addr = XEXP (addr, 1);
      if (CONSTANT_P (XEXP (addr, 1)))
	addr = XEXP (addr, 0);
    }
  if (GET_CODE (addr) == REG)
    return addr;
  return 0;
}


/* Print the name of register 'regno' in the assembly file.  */
static void
asm_print_register (file, regno)
     FILE *file;
     int regno;
{
  const char *name = reg_names[regno];

  asm_fprintf (file, "%s", name);
}

/* Returns true if the operand 'op' must be printed with parenthesis
   arround it.  This must be done only if there is a symbol whose name
   is a processor register.  */
static int
must_parenthesize (op)
     rtx op;
{
  const char *name;

  switch (GET_CODE (op))
    {
    case SYMBOL_REF:
      name = XSTR (op, 0);
      /* Avoid a conflict between symbol name and a possible
         register.  */
      return (strcasecmp (name, "x") == 0
	      || strcasecmp (name, "y") == 0) ?1:0;

    case PLUS:
    case MINUS:
      return must_parenthesize (XEXP (op, 0))
	|| must_parenthesize (XEXP (op, 1));

    case MEM:
    case CONST:
    case ZERO_EXTEND:
    case SIGN_EXTEND:
      return must_parenthesize (XEXP (op, 0));

    case CONST_DOUBLE:
    case CONST_INT:
    case LABEL_REF:
    case CODE_LABEL:
    default:
      return 0;
    }
}


/* A C compound statement to output to stdio stream STREAM the
   assembler syntax for an instruction operand that is a memory
   reference whose address is ADDR.  ADDR is an RTL expression.  */

void
print_operand_address (file, addr)
     FILE *file;
     rtx addr;
{
  rtx base;
  rtx offset;
  int need_parenthesis = 0;
  
  switch (GET_CODE (addr))
    {
    case REG:
      if (!REG_P (addr))
	abort ();

      asm_print_register (file, REGNO (addr));
      //asm_fprintf (file, "+0");
      break;

    case MEM:
      base = XEXP (addr, 0);
      output_addr_const (file, base);
      break;

    case PLUS:
      base = XEXP (addr, 0);
      offset = XEXP (addr, 1);
      if (!G_REG_P (base) && G_REG_P (offset))
	{
	  base = XEXP (addr, 1);
	  offset = XEXP (addr, 0);
	}
      if ((CONSTANT_ADDRESS_P (base)) && (CONSTANT_ADDRESS_P (offset)))
	{
	  need_parenthesis = must_parenthesize (addr);

	  output_addr_const (file, base);
	 if (INTVAL (offset) >=0)   
	  asm_fprintf (file, "+");       
	  output_addr_const (file, offset);
	}
      else if (REG_P (base)  && REG_OK_FOR_BASE_P (base))
	{
	  need_parenthesis = must_parenthesize (addr);

	      asm_print_register (file, REGNO (base));
	      if (INTVAL(offset) >= 0)   
	      asm_fprintf (file, "+");
	      
	      output_addr_const (file, offset);
	}
      else
	{
	 abort ();
	}
      break;

    default:
      if (GET_CODE (addr) == CONST_INT
	  && INTVAL (addr) < 0x8000 && INTVAL (addr) >= -0x8000)
	{
	  asm_fprintf (file, "%d", INTVAL (addr));
	}
      else
	{
	 output_addr_const (file, addr);
	}
      break;
    }
}




void
notice_update_cc_on_set(exp, insn)
  rtx exp;
  rtx insn;
{
    if (GET_CODE (SET_DEST (exp)) == CC0)
    { 
	cc_status.flags = 0;					
	cc_status.value1 = SET_DEST (exp);			
	cc_status.value2 = SET_SRC (exp);			

/*
	if (GET_MODE(SET_SRC(exp)) == DFmode)
	    cc_status.flags |= CC_IN_FPU;
*/	
    }							
    else if ((GET_CODE (SET_DEST (exp)) == REG		
	      || GET_CODE (SET_DEST (exp)) == MEM)		
	     && GET_CODE (SET_SRC (exp)) != PC		
	     && (GET_MODE (SET_DEST(exp)) == HImode		
		 || GET_MODE (SET_DEST(exp)) == QImode)	
		&& (GET_CODE (SET_SRC(exp)) == PLUS		
		    || GET_CODE (SET_SRC(exp)) == MINUS	
		    || GET_CODE (SET_SRC(exp)) == AND	
		    || GET_CODE (SET_SRC(exp)) == IOR	
		    || GET_CODE (SET_SRC(exp)) == XOR	
		    || GET_CODE (SET_SRC(exp)) == NOT	
		    || GET_CODE (SET_SRC(exp)) == NEG	
			|| GET_CODE (SET_SRC(exp)) == REG	
		    || GET_CODE (SET_SRC(exp)) == MEM))	
    { 
	cc_status.flags = 0;					
	cc_status.value1 = SET_SRC (exp);   			
	cc_status.value2 = SET_DEST (exp);			
	
	if (cc_status.value1 && GET_CODE (cc_status.value1) == REG	
	    && cc_status.value2					
	    && reg_overlap_mentioned_p (cc_status.value1, cc_status.value2))
    	    cc_status.value2 = 0;					
	if (cc_status.value1 && GET_CODE (cc_status.value1) == MEM	
	    && cc_status.value2					
	    && GET_CODE (cc_status.value2) == MEM)			
	    cc_status.value2 = 0; 					
    }							
    else if (GET_CODE (SET_SRC (exp)) == CALL)		
    { 
	CC_STATUS_INIT; 
    }
    else if (GET_CODE (SET_DEST (exp)) == REG)       		
	/* what's this ? */					
    { 
	if ((cc_status.value1					
	     && reg_overlap_mentioned_p (SET_DEST (exp), cc_status.value1)))
	    cc_status.value1 = 0;				
	if ((cc_status.value2					
	     && reg_overlap_mentioned_p (SET_DEST (exp), cc_status.value2)))
	    cc_status.value2 = 0;				
    }							
    else if (SET_DEST(exp) == pc_rtx)
    { 
	/* jump */
    }
    else /* if (GET_CODE (SET_DEST (exp)) == MEM)	*/	
    {  
	/* the last else is a bit paranoiac, but since nearly all instructions 
	   play with condition codes, it's reasonable! */

	CC_STATUS_INIT; /* paranoia*/ 
    }		        
}


	

int
dead_p (rtx insn, rtx x)
{
  rtx link;
  unsigned int regno;
  regno = REGNO (x);

  for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
  {
      if (REG_NOTE_KIND (link) != REG_DEAD || GET_CODE (XEXP (link, 0)) != 
REG)
        continue;

      if (regno == REGNO (XEXP (link, 0)))
          return 1;
  }
  return 0;
}


#define COSTS_N_INSNS(N) ((N) * 4 - 2)
	
int
z80_memory_move_cost (mode, class, in)
     enum machine_mode mode;
     enum reg_class class;
     int in ATTRIBUTE_UNUSED;
{
    switch (GET_MODE_SIZE (mode))
	{
			case 1:
					return 4;
			case 2:
				return 5;
			default:
				return 8;
	}
}


/* Cost of moving data from a register of class 'from' to on in class 'to'.
   Reload does not check the constraint of set insns when the two registers
   have a move cost of 2.  Setting a higher cost will force reload to check
   the constraints.  */
int
z80_register_move_cost (from, to)
     enum reg_class from;
     enum reg_class to;
{
/*  if (from >= S_REGS && to >= S_REGS)
    {
      return COSTS_N_INSNS (3);
    }
  if (from <= S_REGS && to <= S_REGS)
    {
      return COSTS_N_INSNS (1) + (reload_completed | reload_in_progress);
    }*/
		return COSTS_N_INSNS (2);

}


/* Provide the costs of an addressing mode that contains ADDR.
   If ADDR is not a valid address, its cost is irrelevant.  */

int
z80_address_cost (addr)
     rtx addr;
{
  int cost = 4;

  switch (GET_CODE (addr))
    {
    case REG:
      /* Make the cost of hard registers and specially SP, FP small. */
      if (REGNO (addr) < FIRST_PSEUDO_REGISTER)
	cost = 0;
      else
	cost = 1;
      break;

    case SYMBOL_REF:
      cost = 8;
      break;

    case LABEL_REF:
    case CONST:
      cost = 0;
      break;

    case PLUS:
      {
	register rtx plus0 = XEXP (addr, 0);
	register rtx plus1 = XEXP (addr, 1);

	if (GET_CODE (plus0) != REG)
	  break;

	switch (GET_CODE (plus1))
	  {
	  case CONST_INT:
	    if (INTVAL (plus1) >= 2 * z80_max_offset
		|| INTVAL (plus1) < z80_min_offset)
	      cost = 3;
	    else if (INTVAL (plus1) >= z80_max_offset)
	      cost = 2;
	    else
	      cost = 0;
	    if (REGNO (plus0) < FIRST_PSEUDO_REGISTER)
	      cost += 0;
	    else
	      cost += 1;
	    break;

	  case SYMBOL_REF:
	    cost = 8;
	    break;

	  case CONST:
	  case LABEL_REF:
	    cost = 0;
	    break;

	  default:
	    break;
	  }
	break;
      }
    case PRE_DEC:
    case PRE_INC:
      if (SP_REG_P (XEXP (addr, 0)))
	cost = 1;
      break;

    default:
      break;
    }
  return cost;
}

void
z80_asm_file_start (out, main_file)
     FILE *out;
     char *main_file;
{
  fprintf (out, "/*;;;-----------------------------------------\n");
  fprintf (out, ";;; Start z80 gcc assembly output\n");
  fprintf (out, ";;; gcc compiler %s\n", version_string);
  fprintf (out, ";;;-----------------------------------------\n*/\n");
  output_file_directive (out, main_file);
}
z80_rtx_costs (x, code, outer_code)
     rtx x;
     enum rtx_code code, outer_code;
{
  enum machine_mode mode = GET_MODE (x);
  int extra_cost = 0;
  int total;

  switch (code)
    {
    case MEM:
      return z80_address_cost (XEXP (x, 0)) + 4;

    case ROTATE:
    case ROTATERT:
    case ASHIFT:
    case LSHIFTRT:
    case ASHIFTRT:
      if (GET_CODE (XEXP (x, 1)) == CONST_INT)
	{
	  int val = INTVAL (XEXP (x, 1));
	  int cost;

	  /* 8 or 16 shift instructions are fast.
	     Others are proportional to the shift counter.  */
	  if (val == 8 || val == 16 || val == -8 || val == -16)
	    {
	      val = 0;
	    }
	  cost = COSTS_N_INSNS (val + 1);
	  cost += rtx_cost (XEXP (x, 0), outer_code);
	  if (GET_MODE_SIZE (mode) >= 4 && val)
	    {
	      cost *= 4;
	    }
	  return cost;
	}
      total = rtx_cost (XEXP (x, 0), outer_code);
      if (GET_MODE_SIZE (mode) >= 4)
	{
	  total += COSTS_N_INSNS (16);
	}
      else
	{
	  total += COSTS_N_INSNS (8);
	}
      return total;

    case MINUS:
    case PLUS:
    case AND:
    case XOR:
    case IOR:
      extra_cost = 0;

      total = rtx_cost (XEXP (x, 0), outer_code)
	+ rtx_cost (XEXP (x, 1), outer_code);
      if (GET_MODE_SIZE (mode) <= 2)
	{
	  total += COSTS_N_INSNS (2);
	}
      else
	{
	  total += COSTS_N_INSNS (4);
	}
      return total;

    case DIV:
    case MOD:
      if (mode == QImode || mode == HImode)
	{
	  return 30;
	}
      else if (mode == SImode)
	{
	  return 100;
	}
      else
	{
	  return 150;
	}

    case MULT:
      if (mode == QImode)
	{
	  return  2;
	}
      if (mode == HImode)
	{
	  return  4;
	}
      if (mode == SImode)
	{
	  return  20;
	}
      return 150;

    case NEG:
    case SIGN_EXTEND:
      extra_cost = COSTS_N_INSNS (2);

      /* Fall through */
    case NOT:
    case COMPARE:
    case ABS:
    case ZERO_EXTEND:
      total = rtx_cost (XEXP (x, 0), outer_code);
      if (mode == QImode)
	{
	  return total + extra_cost + COSTS_N_INSNS (1);
	}
      if (mode == HImode)
	{
	  return total + extra_cost + COSTS_N_INSNS (2);
	}
      if (mode == SImode)
	{
	  return total + extra_cost + COSTS_N_INSNS (4);
	}
      return total + extra_cost + COSTS_N_INSNS (8);

    case IF_THEN_ELSE:
      if (GET_CODE (XEXP (x, 1)) == PC || GET_CODE (XEXP (x, 2)) == PC)
	return COSTS_N_INSNS (1);

      return COSTS_N_INSNS (1);

    default:
      return COSTS_N_INSNS (4);
    }
}         

void
create_regs_rtx ()
{
  /*  regs_inited = 1; */
  ix_reg = gen_rtx (REG, HImode, HARD_IX_REGNUM);
  iy_reg = gen_rtx (REG, HImode, HARD_IY_REGNUM);

  stack_push_word = gen_rtx (MEM, HImode, 
			     gen_rtx (PRE_DEC, HImode,
				      gen_rtx (REG, HImode, HARD_SP_REGNUM)));
  stack_pop_word = gen_rtx (MEM, HImode,
			    gen_rtx (POST_INC, HImode,
				     gen_rtx (REG, HImode, HARD_SP_REGNUM)));

}

/* Function prologue and epilogue.  */

/* Emit a move after the reload pass has completed.  This is used to
   emit the prologue and epilogue.  */
static void
emit_move_after_reload (to, from, scratch)
     rtx to, from, scratch;
{
  rtx insn;

  if ( H_REG_P (to) || H_REG_P (from))
    {
      insn = emit_move_insn (to, from);
    }
  else
    {
      emit_move_insn (scratch, from);
      insn = emit_move_insn (to, scratch);
    }

  /* Put a REG_INC note to tell the flow analysis that the instruction
     is necessary.  */
  if (IS_STACK_PUSH (to))
    {
      REG_NOTES (insn) = gen_rtx_EXPR_LIST (REG_INC,
					    XEXP (XEXP (to, 0), 0),
					    REG_NOTES (insn));
    }
  else if (IS_STACK_POP (from))
    {
      REG_NOTES (insn) = gen_rtx_EXPR_LIST (REG_INC,
					    XEXP (XEXP (from, 0), 0),
					    REG_NOTES (insn));
    }
}

int
z80_total_frame_size ()
{
  int size;
  int regno;

  size = get_frame_size ();
  if (frame_pointer_needed)
    size += HARD_REG_SIZE;

  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
    if (regs_ever_live[regno] && ! call_used_regs[regno])
      size += HARD_REG_SIZE;

  return size;
}


void
expand_prologue ()
{
  tree func_attr;
  int size;
  int regno;
  rtx scratch;

  if (reload_completed != 1)
    abort ();

  size = get_frame_size ();

  create_regs_rtx ();

  /* Generate specific prologue for interrupt handlers.  */
  func_attr = TYPE_ATTRIBUTES (TREE_TYPE (current_function_decl));
  current_function_interrupt = lookup_attribute ("interrupt",
						 func_attr) != NULL_TREE;
  current_function_trap = lookup_attribute ("trap", func_attr) != NULL_TREE;

  /* Save current stack frame.  */
  if (frame_pointer_needed)
    emit_move_after_reload (stack_push_word,frame_pointer_rtx, scratch);

  /* Create the frame pointer.  */
  if (frame_pointer_needed){
		  
    emit_move_after_reload (frame_pointer_rtx,
			    const0_rtx, scratch);
		   emit_insn (gen_rtx_SET (VOIDmode,
				 frame_pointer_rtx, 
				 gen_rtx_PLUS (HImode,
					       frame_pointer_rtx,
					      stack_pointer_rtx)));  

	}
   
  if (size > 0)
    {
      rtx insn;

    emit_move_insn (iy_reg, GEN_INT(-size));
		   emit_insn (gen_rtx_SET (VOIDmode, iy_reg, 
				 gen_rtx_PLUS (HImode, iy_reg, stack_pointer_rtx)));  
    emit_move_insn (stack_pointer_rtx, iy_reg);
	}


   	
	for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
      if (regs_ever_live[regno] && !call_used_regs[regno])
	{
	  emit_move_after_reload (stack_push_word,
				  gen_rtx (REG, HImode, regno), scratch);
	}
}



void
expand_epilogue ()
{
  int size;
  register int regno;
  int return_size;
  rtx scratch;

  if (reload_completed != 1)
    abort ();

  size = get_frame_size ();

  /* If we are returning a value in two registers, we have to preserve the
     X register and use the Y register to restore the stack and the saved
     registers.  Otherwise, use X because it's faster (and smaller).  */
  if (current_function_return_rtx == 0)
    return_size = 0;
  else if (GET_CODE (current_function_return_rtx) == MEM)
    return_size = HARD_REG_SIZE;
  else
    return_size = GET_MODE_SIZE (GET_MODE (current_function_return_rtx));

  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
    if (regs_ever_live[regno] && ! call_used_regs[regno])
    {
	  emit_move_after_reload (gen_rtx (REG, HImode, regno),
				  stack_pop_word, scratch);
	}

	 emit_move_insn (stack_pointer_rtx,ix_reg);


  /* Restore previous frame pointer.  */
  if (frame_pointer_needed)
    emit_move_insn (frame_pointer_rtx, stack_pop_word);


  emit_jump_insn (gen_return ());
}





