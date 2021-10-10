#pragma once

enum OperandType
{
	OT_NONE,
	OT_UNK,

	_OT_BEGIN_DIRREG,

	OT_A,
	OT_A_S,
	OT_AX,
	OT_CY,

	OT_PSW,			// REG

	OT_PSWL,
	OT_PSWH,

	OT_SP,

	OT_X,

	OT_DE_A_P,
	OT_DE_S_P,
	OT_HL_A_P,
	OT_HL_S_P,

	_OT_END_DIRREG,

	_OT_BEGIN_REG,
	OT_RP,
	OT_RP1,
	OT_RP1P,		// []
	OT_RP2,
	OT_RPL,
	OT_R2,			// C/B

	OT_R1,
	OT_R,

	OT_SADDR,
	OT_SADDR_FE,
	OT_SADDR_I,
	OT_BYTE,		// #

	OT_SFR,
	OT_SFR_I,
	OT_MEM,
	OT_MEM_S,

	OT_SADDRP,		// []
	OT_SADDRP_FE,
	OT_SADDRPP,		// [[]]

	OT_ADDR16,		// !
	OT_ADDR11,		// !
	OT_ADDR5,		// []

	OT_ADDR16R,		// JDISP (RELATIVE ADDR16: PC + JDISP)

	OT_WORD,		// #
	OT_SFRP,

	OT_NUM,

	OT_POST_BYTE,
	OT_POST_BYTE_U,

	OT_RB_NUM,

	_OT_END_REG,

	_OT_BEGIN_BIT,

	OT_CY_BIT,		// #1: CY <- ??.BIT / #2: ??.BIT <- CY
	OT_N_BIT,		// #2: ??.BIT

	_OT_END_BIT
};

/*

Coding Types

2 nnnnnRRR / DATA
3 nnnnnnnn / SADDR / DATA
3 nnnnnnnn / SFR / DATA
2 nnnnnnnn / RRRR0RRR
1 nnnnnRRR
2 nnnnnnnn / SADDR
2 nnnnnnnn / SADDR / SADDR
1 nnnnnMEM
? nnnMODMO / nMEMnnnn / L / H
4 nnnnnnnn / nnnnnnnn / L / H
3 nnnnnnnn / nnnnnnnn / DATA
.
3 nnnnnnnn / nnnnnnnn / SFR

3 nnnnnnnn / L / H
4 nnnnnnnn / SADDR / L / H
4 nnnnnnnn / SFR / L / H

2 nnnnnnnn / PPPnnQQQ <- ?

2 nnnnnnnn / nnNNNRRR

*/

struct Operand
{
	OperandType opType;// = OperandType::OT_NONE;
	int byteOffset;
	byte opMask;
	byte opShift;
	bool isMemMod;
};