This file describes the gcc z80 port status:

I worked with the source tree gcc-2.95.2. This work is done as a part my 
learning. The starting point for this work is m68hc11 port. (m68hc11's architecture is close to z80.). Hope we do n't need to provide any special copyright 
notice for this.

Z80 port status:
----------------
1.)Porting of binutils is not attempted yet.

2.)Currently I am working with gcc. The basic port is up. It's able to generate
code for all the basic operations. (Except for multiplication and division
operations which are library calls in libgcc1. It makes a call to the corresponding function, whose code is not provided yet.). 

3.) Port description:

      Register allocation:
	 ---------------------
		We have registers A,B,C,D,E,H,L,IXH,IXL,IYH,IYL,SP,PC.
		and we can use BC (B+C),DE(D+E),HL(H+L),IX(IXH+IXL) and IY(IYH+IYL).

		We use register as A,HL,BC,DE,SP,IX,IY,SP and PC. rest of them are
		 referred as fixed registers. Initially I started using individual
        registers A,H,L,B,C ......  and used <n> consecutive registers reqired 
		for a mode. But I felt it does not make a big difference. So, I switched		to this approach. (ANY COMMENTS ON THIS ARE WELCOME.).

			---The MAX_FIXED_MODE_SIZE in z80.h is 32.
			---Registers HL+DE  and BC + IY are used for storing 32 bit values.
			---HImode values can go in any register except register A.
			---(??) QImode values can go in any register except IX and IY
			registers. I arrived on to this conclusion because
			most z80 instructions does not allow ixh,ixl, iyh and iyl as
			the operands, but they allow all other registers. I think I need to 
			define LIMIT_RELOAD_CLASS macro to suit my requirement.
			May be I need to allow them, if there are any reload problems. 
				(ANY COMMENTS ON THIS ARE WELCOME.).
			
			--- Register IX is being used as FRAME_POINTER.
			--- Regsiter IY is declared as STATIC_CHAIN_REGNUM. But this 
				needs to be changed to STATIC_CHAIN. Because in the prologue
				I am using IY to allocate frame.

4.) Work needs to be carried out:
		    --- COST macros need to be implemented yet.
			--- The constraints whatever I mentioned in the md file may need
				to be modified, because I arrived on them on a trail/error
				basis.
			---Some improvements for branch/comapre patterns.
				When ever I compile a program with -Os option which involves 
				a SImode comparision (long values), It falis .
				(It fails in cse.c 6037.)
			---If applicable need to add peephole optimization.


5.) How to merge the patch with the source tree.

  This port is based on 2.95.2 version of gcc source.

  Steps to expand and apply the patch:
  ------------------------------------
		1) Expand the patch
				gzip -d gcc-2.95.2-z80-patch.gz
		2) cd gcc-2.95.2
		3) patch -NP1 < ../gcc-2.95.2-z80-patch


6.) How to build gcc for z80.
	1) You make a build directory  at the same level of your source directory.
			$mkdir build
			$mkdir exec

	2) configuring:

			../gcc-2.95.2/configure --target=z80-zilog-linux --prefix=<exec path> -v
	3) make all 
			It stops at libgcc1.
	4) make -i install



