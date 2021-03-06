@ RUN: llvm-mc %s -triple=armv7-unknown-linux-gnueabi -filetype=obj -o - \
@ RUN:   | llvm-readobj -s -sd -sr -t | FileCheck %s

@ Check the .group section for the function in comdat section.

@ In C++, the instantiation of the template will come with linkonce (or
@ linkonce_odr) linkage, so that the linker can remove the duplicated
@ instantiation.  When the exception handling is enabled on those function,
@ we have to group the corresponding .ARM.extab and .ARM.exidx with the
@ text section together.
@
@ This test case will check the content of .group section.  The section index
@ of the grouped sections should be recorded in .group section.

	.syntax unified
	.section	.TEST1,"axG",%progbits,func1,comdat
	.weak	func1
	.align	2
	.type	func1,%function
func1:
	.fnstart
	.save	{r4, lr}
	push	{r4, lr}
	.vsave	{d8, d9, d10, d11, d12}
	vpush	{d8, d9, d10, d11, d12}
	.pad	#24
	sub	sp, sp, #24

	add	sp, sp, #24
	vpop	{d8, d9, d10, d11, d12}
	pop	{r4, pc}

	.globl	__gxx_personality_v0
	.personality __gxx_personality_v0
	.handlerdata
	.fnend



@-------------------------------------------------------------------------------
@ Check the .group section
@-------------------------------------------------------------------------------
@ CHECK: Sections [
@ CHECK:   Section {
@ CHECK:     Index: 1
@ CHECK:     Name: .group
@ CHECK:     Type: SHT_GROUP (0x11)
@ CHECK:     Flags [ (0x0)
@ CHECK:     ]
@ CHECK:     SectionData (
@-------------------------------------------------------------------------------
@ The second, third, and fourth word should correspond to the section index
@ of .TEST1, .ARM.extab.TEST1, and .ARM.exidx.TEST1.
@-------------------------------------------------------------------------------
@ CHECK:       0000: 01000000 05000000 06000000 07000000  |................|
@ CHECK:     )
@ CHECK:   }


@-------------------------------------------------------------------------------
@ Check the .TEST1 section
@-------------------------------------------------------------------------------
@ CHECK:   Section {
@ CHECK:     Index: 5
@ CHECK:     Name: .TEST1
@ CHECK:     Type: SHT_PROGBITS (0x1)
@-------------------------------------------------------------------------------
@ The flags should contain SHF_GROUP.
@-------------------------------------------------------------------------------
@ CHECK:     Flags [ (0x206)
@ CHECK:       SHF_ALLOC (0x2)
@ CHECK:       SHF_EXECINSTR (0x4)
@ CHECK:       SHF_GROUP (0x200)
@ CHECK:     ]
@ CHECK:   }


@-------------------------------------------------------------------------------
@ Check the .ARM.extab.TEST1 section
@-------------------------------------------------------------------------------
@ CHECK:   Section {
@ CHECK:     Index: 6
@ CHECK:     Name: .ARM.extab.TEST1
@ CHECK:     Type: SHT_PROGBITS (0x1)
@-------------------------------------------------------------------------------
@ The flags should contain SHF_GROUP.
@-------------------------------------------------------------------------------
@ CHECK:     Flags [ (0x202)
@ CHECK:       SHF_ALLOC (0x2)
@ CHECK:       SHF_GROUP (0x200)
@ CHECK:     ]
@ CHECK:   }


@-------------------------------------------------------------------------------
@ Check the .ARM.exidx.TEST1 section
@-------------------------------------------------------------------------------
@ CHECK:   Section {
@ CHECK:     Index: 7
@ CHECK:     Name: .ARM.exidx.TEST1
@ CHECK:     Type: SHT_ARM_EXIDX (0x70000001)
@-------------------------------------------------------------------------------
@ The flags should contain SHF_GROUP.
@-------------------------------------------------------------------------------
@ CHECK:     Flags [ (0x282)
@ CHECK:       SHF_ALLOC (0x2)
@ CHECK:       SHF_GROUP (0x200)
@ CHECK:       SHF_LINK_ORDER (0x80)
@ CHECK:     ]
@ CHECK:     Link: 5
@ CHECK:   }
@ CHECK: ]



@-------------------------------------------------------------------------------
@ Check symbol func1.  It should be weak binding, and belong to .TEST1 section.
@-------------------------------------------------------------------------------
@ CHECK: Symbols [
@ CHECK:   Symbol {
@ CHECK:     Name: func1
@ CHECK:     Binding: Weak (0x2)
@ CHECK:     Type: Function (0x2)
@ CHECK:     Section: .TEST1 (0x5)
@ CHECK:   }
@ CHECK: ]
