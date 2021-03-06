#pragma once

enum class Mnemonic
{
	MOV,
	XCH,
	MOVW,
	XCHW,

	ADD,
	ADDC,
	SUB,
	SUBC,
	AND,
	OR,
	XOR,
	CMP,

	ADDW,
	SUBW,
	CMPW,
	MULU,
	DIVUW,
	MULUW,
	DIVUX,

	INC,
	DEC,
	INCW,
	DECW,

	ROR,
	ROL,
	RORC,
	ROLC,
	SHR,
	SHL,
	SHRW,
	SHLW,

	ROR4,
	ROL4,
	ADJ4,

	MOV1,
	AND1,
	OR1,
	ANDI1,
	ORI1,
	XOR1,
	SET1,
	CLR1,
	NOT1,

	CALL,
	CALLF,
	CALLT,
	CALLR,
	BRK,
	RET,
	RETI,

	PUSH,
	PUSHU,
	POP,
	POPU,
	MOVW_,
	INCW_,
	DECW_,

	BR,

	BL,
	BNL,

	BE,
	BNE,
	BPE,
	BPO,
	BN,
	BP,
	BGT,
	BGE,
	BLT,
	BLE,
	BH,
	BNH,
	BT,
	BF,
	BTCLR,
	BFSET,
	DBNZ,
	BRKCS,
	RETCS,

	MOVM,
	MOVBK,
	XCHM,
	XCHBK,
	CMPME,
	CMPBKE,
	CMPMNE,
	CMPBKNE,
	CMPMC,
	CMPBKC,
	CMPMNC,
	CMPBKNC,

	MOV__,
	SWRS,
	SEL,
	NOP,
	EI,
	DI
};

static const char* names[] =
{
	"MOV",
	"XCH",
	"MOVW",
	"XCHW",

	"ADD",
	"ADDC",
	"SUB",
	"SUBC",
	"AND",
	"OR",
	"XOR",
	"CMP",

	"ADDW",
	"SUBW",
	"CMPW",
	"MULU",
	"DIVUW",
	"MULUW",
	"DIVUX",

	"INC",
	"DEC",
	"INCW",
	"DECW",

	"ROR",
	"ROL",
	"RORC",
	"ROLC",
	"SHR",
	"SHL",
	"SHRW",
	"SHLW",

	"ROR4",
	"ROL4",
	"ADJ4",

	"MOV1",
	"AND1",
	"OR1",
	"ANDI1",
	"ORI1",
	"XOR1",
	"SET1",
	"CLR1",
	"NOT1",

	"CALL",
	"CALLF",
	"CALLT",
	"CALLR",
	"BRK",
	"RET",
	"RETI",

	"PUSH",
	"PUSHU",
	"POP",
	"POPU",
	"MOVW_",
	"INCW_",
	"DECW_",

	"BR",

	"BL",
	"BNL",

	"BE",
	"BNE",
	"BPE",
	"BPO",
	"BN",
	"BP",
	"BGT",
	"BGE",
	"BLT",
	"BLE",
	"BH",
	"BNH",
	"BT",
	"BF",
	"BTCLR",
	"BFSET",
	"DBNZ",
	"BRKCS",
	"RETCS",

	"MOVM",
	"MOVBK",
	"XCHM",
	"XCHBK",
	"CMPME",
	"CMPBKE",
	"CMPMNE",
	"CMPBKNE",
	"CMPMC",
	"CMPBKC",
	"CMPMNC",
	"CMPBKNC",

	"MOV__",
	"SWRS",
	"SEL",
	"NOP",
	"EI",
	"DI"
};
