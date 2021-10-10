#pragma once

struct Opcode
{
	Mnemonic Mnemonic;
	byte bytes;

	byte op;
	byte opMask;
	byte subOp;
	byte subOpMask;

	Operand operand1;
	Operand operand2;

	bool specialOnebyteInstr;

	Operand operand3;

	void PrintOpcode() const
	{
		char name[32];
		strcpy(name, names[(int)Mnemonic]);
		int nameLen = strlen(name);
		for (int i = 0; i < nameLen; i++)
		{
			name[i] = tolower(name[i]);
		}
		printf("%-8s", name);
	}
};
