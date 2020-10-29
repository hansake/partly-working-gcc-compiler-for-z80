/* Prototypes for exported functions defined in	z80.c 
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
Boston, MA 02111-1307, USA.  */


extern int hard_regno_mode_ok PARAMS((int, enum machine_mode));

extern int z80_total_frame_size PARAMS((void));

extern void expand_prologue PARAMS((void));
extern void expand_epilogue PARAMS((void));

extern void z80_asm_file_start PARAMS((FILE*, char*));

extern rtx z80_compare_op0;
extern rtx z80_compare_op1;
extern rtx z80_expand_compare_and_branch PARAMS((enum rtx_code,
                                                     rtx, rtx, rtx,int));
extern int z80_go_if_legitimate_address PARAMS((rtx,
                                                    enum machine_mode,
                                                    int));

extern void z80_notice_update_cc PARAMS((rtx, rtx));


extern void print_operand PARAMS((FILE*,rtx,int));
extern void print_operand_address PARAMS((FILE*,rtx));

extern int dead_register_here PARAMS((rtx, rtx));

extern void z80_split_move PARAMS((rtx, rtx,rtx));
extern void z80_split_compare_and_branch PARAMS((enum rtx_code,
                                                     rtx, rtx, rtx));
extern rtx z80_gen_lowpart PARAMS((enum machine_mode, rtx));
extern rtx z80_gen_highpart PARAMS((enum machine_mode, rtx));

extern int z80_small_indexed_indirect_p PARAMS((rtx, enum machine_mode));
extern int tst_operand PARAMS((rtx,enum machine_mode));
extern int cmp_operand PARAMS((rtx,enum machine_mode));
extern int z80_register_indirect_p PARAMS((rtx, enum machine_mode));

extern int symbolic_memory_operand PARAMS((rtx, enum machine_mode));

extern int memory_reload_operand PARAMS((rtx, enum machine_mode));
extern int stack_register_operand PARAMS((rtx, enum machine_mode));
extern int non_push_operand PARAMS((rtx, enum machine_mode));
extern int reg_or_some_mem_operand PARAMS((rtx, enum machine_mode));

