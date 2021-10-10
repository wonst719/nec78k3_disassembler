#include <stdio.h>
#include <string>
#include <vector>

typedef unsigned char byte;
typedef unsigned short uint16;
typedef unsigned int uint32;

class File
{
	FILE* fp;

public:
	File(std::string path)
	{
		fp = fopen(path.c_str(), "rb");
	}

	~File()
	{
		if (fp)
			fclose(fp);
	}

public:
	byte ReadByte()
	{
		int ch = fgetc(fp);
		return (byte)ch;
	}

	uint16 ReadUInt16()
	{
		byte l = ReadByte();
		byte h = ReadByte();
		return (uint16)h << 8 | l;
	}

	bool IsEof()
	{
		return feof(fp) != 0;
	}

	void Back1()
	{
		long cp = ftell(fp);
		fseek(fp, cp - 1, SEEK_SET);
	}

	long Tell()
	{
		return ftell(fp);
	}

	void Seek(long offset)
	{
		fseek(fp, offset, SEEK_SET);
	}

};

#include "Mnemonic.h"

#include "Operand.h"

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

#include "Opcodes.h"

struct OpcodeInstance
{
	unsigned int pos;
	const Opcode& op;
	byte totalBytes;
	byte bytes[5];

	void PrintString(long pos)
	{
		int block = 7;
		for (int i = 0; i < totalBytes; i++)
		{
			printf("%02x", bytes[i]);
		}
		for (int i = totalBytes; i < block; i++)
		{
			printf("  ");
		}

		op.PrintOpcode();
		PrintOperand(pos);
		printf("\n");
	}

	void PrintDirectRegOp(int type)
	{
		switch (type)
		{
		case OT_A:		printf("a"); break;
		case OT_A_S:	printf("as"); break;
		case OT_AX:		printf("ax"); break;
		case OT_CY:		printf("cy"); break;
		case OT_PSW:	printf("psw"); break;
		case OT_PSWL:	printf("pswl"); break;
		case OT_PSWH:	printf("pswh"); break;
		case OT_SP:		printf("sp"); break;
		case OT_X:		printf("x"); break;

		case OT_DE_A_P:		printf("[de+]"); break;
		case OT_DE_S_P:		printf("[de-]"); break;
		case OT_HL_A_P:		printf("[hl+]"); break;
		case OT_HL_S_P:		printf("[hl-]"); break;
		}
	}

	static uint16 GetWord(int byteOffset, byte bytes[5])
	{
		byte lo = bytes[byteOffset];
		byte hi = bytes[byteOffset + 1];

		uint16 word = lo | (hi << 8);
		return word;
	}

	static uint16 GetWord(const Operand& operand, byte bytes[5])
	{
		return GetWord(operand.byteOffset, bytes);
	}

	static byte GetMaskedByte(const Operand& operand, byte bytes[5])
	{
		byte b = (bytes[operand.byteOffset] & operand.opMask) >> operand.opShift;
		return b;
	}
		
	static uint16 CalcSaddr(byte b)
	{
		uint16 w = b;		// fe20 ~ ff1f
		if (b < 0x20)
		{
			w += 0xff00;
		}
		else
		{
			w += 0xfe00;
		}
		return w;
	}

	// 이건 뭘까...
	static uint16 CalcSaddr_fe(byte b)
	{
		uint16 w = b;		// fe20 ~ ff1f
// 		if (b < 0x20)
// 		{
// 			w += 0xff00;
// 		}
// 		else
		{
			w += 0xfe00;
		}
		return w;
	}

	void PrintRegOp(long pc, Operand operand, byte bytes[5])
	{
		switch (operand.opType)
		{
		case OT_R:
			{
				const char* pRegId = "xacbxacb8901edlh";
				byte b = GetMaskedByte(operand, bytes);
				if (b > 0xf)
				{
					printf("r_error");
				}
				else
				{
					//printf("(r)");
					if (b == 8)
					{
						printf("vpl");
					}
					else if (b == 9)
					{
						printf("vph");
					}
					else if (b == 10)
					{
						printf("upl");
					}
					else if (b == 11)
					{
						printf("uph");
					}
					else
					{
						printf("%c", pRegId[b & 0xf]);
						if (b >= 0x4 && b <= 0xb)
						{
							printf("'");
						}
					}
				}
			}
			break;
		case OT_R1:
			{
				const char* pRegId = "xacbxacb";
				byte b = GetMaskedByte(operand, bytes);
				if (b > 0x7)
				{
					printf("r1_error");
				}
				else
				{
					//printf("(r1)");
					printf("%c", pRegId[b & 0x7]);
					if (b >= 0x4)
					{
						printf("'");
					}
				}
			}
			break;

		case OT_R2:
			{
				const char* pRegId = "cb";
				byte b = GetMaskedByte(operand, bytes);
				if (b > 0x1)
				{
					printf("r2_error");
				}
				else
				{
					//printf("(r2)");
					printf("%c", pRegId[b & 0x1]);
				}
			}
			break;

		case OT_RP:
			{
				//const char* pRegId = "ax bc p2 p3 vp up de hl";
				const char* pRegId = "axbcaxbcvpupdehl"; // 01234567
				byte b = GetMaskedByte(operand, bytes);
				if (b > 0x7)
				{
					printf("rp_error");
				}
				else
				{
					//printf("(r1)");
					printf("%c", pRegId[(b & 0x7) * 2]);
					printf("%c", pRegId[(b & 0x7) * 2 + 1]);
					if (b == 2 || b == 3)
					{
						printf("'");
					}
				}
			}
			break;

		case OT_RP1:
			{
				//const char* pRegId = "ax bc p2 p3 vp up de hl";
				const char* pRegId = "axvpbcupaxdebchl"; // 04152637
				byte b = GetMaskedByte(operand, bytes);
				if (b > 0x7)
				{
					printf("rp1_error");
				}
				else
				{
					//printf("(r1)");
					printf("%c", pRegId[(b & 0x7) * 2]);
					printf("%c", pRegId[(b & 0x7) * 2 + 1]);
					if (b == 4 || b == 6)
					{
						printf("'");
					}
				}
			}
			break;

		case OT_RP2:
			{
				const char* pRegId = "vpupdehl";
				byte b = GetMaskedByte(operand, bytes);
				if (b > 0x7)
				{
					printf("rp2_error");
				}
				else
				{
					//printf("(rp2)");
					printf("%c", pRegId[(b & 0x7) * 2]);
					printf("%c", pRegId[(b & 0x7) * 2 + 1]);
				}
			}
			break;

		case OT_MEM:
			{
				if (operand.isMemMod)
				{
					byte mod = (bytes[0] & 0x1f);
					byte b = GetMaskedByte(operand, bytes);

					if (mod == 0b10110)
					{
						//printf("_RI_");
						switch (b)
						{
						case 0: printf("[de+]"); break;
						case 1: printf("[hl+]"); break;
						case 2: printf("[de-]"); break;
						case 3: printf("[hl-]"); break;
						case 4: printf("[de]"); break;
						case 5: printf("[hl]"); break;
						case 6: printf("[vp]"); break;
						case 7: printf("[up]"); break;
						}
					}
					else if (mod == 0b10111)
					{
						//printf("_BI_");
						switch (b)
						{
						case 0: printf("[de + a]"); break;
						case 1: printf("[hl + a]"); break;
						case 2: printf("[de + b]"); break;
						case 3: printf("[hl + b]"); break;
						case 4: printf("[vp + de]"); break;
						case 5: printf("[vp + hl]"); break;
						}
					}
					else if (mod == 0b00110)
					{
						byte lo = bytes[2];
						//printf("_BS_");
						switch (b)
						{
						case 0: printf("[de + 0x%02x]", lo); break;
						case 1: printf("[sp + 0x%02x]", lo); break;
						case 2: printf("[hl + 0x%02x]", lo); break;
						case 3: printf("[up + 0x%02x]", lo); break;
						case 4: printf("[vp + 0x%02x]", lo); break;
						}
					}
					else if (mod == 0b01010)
					{
						uint16 word = GetWord(2, bytes);
						switch (b)
						{
						case 0: printf("[0x%04x + de]", word); break;
						case 1: printf("[0x%04x + a]", word); break;
						case 2: printf("[0x%04x + hl]", word); break;
						case 3: printf("[0x%04x + b]", word); break;
						}
					}
				}
			}
			break;

		case OT_MEM_S:
			{
				byte b = GetMaskedByte(operand, bytes);
				if (b > 0x5)
				{
					printf("mem_sp_error");
				}
				else
				{
					switch (b)
					{
					case 0: printf("[de+]"); break;
					case 1: printf("[hl+]"); break;
					case 2: printf("[de-]"); break;
					case 3: printf("[hl-]"); break;
					case 4: printf("[de]"); break;
					case 5: printf("[hl]"); break;
					}
				}
			}
			break;

		case OT_BYTE:
			{
				byte b = GetMaskedByte(operand, bytes);
				printf("#0x%02x", b);
			}
			break;

		case OT_WORD:
			{
				uint16 word = GetWord(operand, bytes);
				printf("#0x%04x", word);
			}
			break;

		case OT_NUM:
			{
				byte b = GetMaskedByte(operand, bytes);
				printf("%d", b);
			}
			break;

		case OT_SFR:
			{
				byte b = GetMaskedByte(operand, bytes);
				printf("sfr[0x%02x]", b);
			}
			break;

		case OT_SADDR:
			{
				byte b = GetMaskedByte(operand, bytes);
				uint16 w = CalcSaddr(b);
				printf("0x%04x", w);
			}
			break;
			
		case OT_SADDR_FE:
			{
				byte b = GetMaskedByte(operand, bytes);
				uint16 w = CalcSaddr_fe(b);
				printf("0x%04x", w);
			}
			break;

		case OT_ADDR16:
			{
				uint16 word = GetWord(operand, bytes);
				printf("0x%04x", word);
			}
			break;

		case OT_ADDR5:
			{
				byte b = (bytes[operand.byteOffset] & 0x1f);
				printf("[0x00%02x]", b & 0x1f);
			}
			break;

		case OT_ADDR16R:
			{
				byte lo = bytes[operand.byteOffset];
				unsigned short word = (unsigned short)pc + op.bytes;
				word += (char)lo;
				//printf("%x / %x / %d", lo, pc, op.bytes);
				printf("0x%04x", word);
			}
			break;

		case OT_RB_NUM:
			{
				byte n = GetMaskedByte(operand, bytes);
				printf("rb%d", n);
			}
			break;

		case OT_SADDRP:
			{
				byte b = GetMaskedByte(operand, bytes);
				uint16 w = CalcSaddr(b);
				printf("0x%04x.w", w);
			}
			break;

		case OT_SADDRP_FE:
			{
				byte b = GetMaskedByte(operand, bytes);
				uint16 w = CalcSaddr_fe(b);
				printf("0x%04x.w", w);
			}
			break;

		case OT_SADDRPP:
			{
				byte b = GetMaskedByte(operand, bytes);
				uint16 w = CalcSaddr(b);
				printf("[0x%04x]", w);
			}
			break;

		case OT_SFRP:
			{
				byte b = GetMaskedByte(operand, bytes);
				printf("sfr[0x%02x].w", b);
			}
			break;

		case OT_POST_BYTE:
		case OT_POST_BYTE_U:
			{
				const char* pRegId = "axbcaxbcvpupdehl";	// FIXME: ?? 일단 맞추긴 했는데
				byte b = GetMaskedByte(operand, bytes);

				int cnt = 0;
				for (int i = 7; i >= 0; i--)
				{
					if (b & (1 << i))
					{
						if (cnt > 0)
							printf(", ");

						if (operand.opType == OT_POST_BYTE_U && i == 5)
						{
							printf("psw");
						}
						else
						{
							printf("%c", pRegId[i * 2]);
							printf("%c", pRegId[i * 2 + 1]);
							if (i >= 0x2 && i <= 0x3)
							{
								printf("'");
							}
						}
						cnt++;
					}
				}
			}
			break;

		default:
			printf("_unk_%d", operand.opType);
		}
	}

	void PrintBitOp(Operand operand, byte bytes[5])
	{
// 		if (operand.opType == OT_N_BIT)
		{
			byte b = GetMaskedByte(operand, bytes);
			printf(".%d", b & 0x7);
		}
// 		else
// 		{
// 			printf(".bit_unk");
// 		}
	}

	void PrintOperand(long pc)
	{
		int type1 = op.operand1.opType;
		int type2 = op.operand2.opType;
		int type3 = op.operand3.opType;
		if (type1 != OT_NONE)
		{
			if (_OT_BEGIN_DIRREG <= type1 && type1 <= _OT_END_DIRREG)
			{
				PrintDirectRegOp(type1);
			}
			else if (_OT_BEGIN_REG <= type1 && type1 <= _OT_END_REG)
			{
				PrintRegOp(pc, op.operand1, this->bytes);
			}
			else if (type1 == OT_CY_BIT)
			{
				printf("cy");
			}
			else
			{
				printf("_unk_%d", type1);
			}
		}

		if (type2 == OT_CY_BIT)
		{
			PrintBitOp(op.operand2, bytes);
		}

		if (type3 == OT_N_BIT)
		{
			PrintBitOp(op.operand3, bytes);
		}

		if (type2 != OT_NONE)
		{
			if (type2 == OT_N_BIT)
			{
				PrintBitOp(op.operand2, bytes);
			}
			else
			{
				printf(", ");

				if (_OT_BEGIN_DIRREG <= type2 && type2 <= _OT_END_DIRREG)
				{
					PrintDirectRegOp(type2);
				}
				else if (_OT_BEGIN_REG <= type2 && type2 <= _OT_END_REG)
				{
					PrintRegOp(pc, op.operand2, this->bytes);
				}
				else if (type2 == OT_CY_BIT)
				{
					printf("cy");
				}
				else
				{
					printf("_unk_%d", type2);
				}
			}
		}

		if (type1 == OT_CY_BIT)
		{
			PrintBitOp(op.operand1, bytes);
		}
	}
};

int main(int argc, const char** argv)
{
	if (argc < 2)
		return -1;

	const char* fileName = argv[0];

	std::vector<OpcodeInstance> o;

	File file(fileName);

	while (!file.IsEof())
	{
		long pos = file.Tell();
		byte b = file.ReadByte();
		if (file.IsEof())
			break;

		bool ok = false;
		Opcode* pSelectedOp = nullptr;

		int modBytes = 0;

		for (auto& op : opps)
		{
			if ((b & op.opMask) == op.op)
			{
				if (op.specialOnebyteInstr)
				{
					byte mem = b & 0x07;
					if (mem >= 6)
						continue;
				}

				if (op.subOpMask != 0)
				{
					ok = false;
					byte b2 = file.ReadByte();
					if ((b2 & op.subOpMask) == op.subOp)
					{
						pSelectedOp = &op;

						// mod!
						if (pSelectedOp->bytes == 0)
						{
							byte mod = b & 0x1f;

							if (mod == 0b10110)
							{
								//printf("_RI_");
								modBytes = 2;
							}
							else if (mod == 0b10111)
							{
								//printf("_BI_");
								modBytes = 2;
							}
							else if (mod == 0b00110)
							{
								//printf("_BS_");
								modBytes = 3;
							}
							else if (mod == 0b01010)
							{
								//printf("_IX_");
								modBytes = 4;
							}
							else
							{
								file.Back1();
								continue;
							}
						}

						ok = true;
						file.Back1();

						break;
					}
					else
					{
						file.Back1();
					}
				}
				else
				{
					pSelectedOp = &op;
					ok = true;

					break;
				}
			}
		}

		if (ok)
		{
			int totBytes = (pSelectedOp->bytes + modBytes);
			OpcodeInstance inst = { (unsigned int)pos, *pSelectedOp, totBytes };

			file.Seek(pos);

			for (int i = 0; i < totBytes; i++)
			{
				inst.bytes[i] = file.ReadByte();
			}
			//file.Seek(pos + totBytes);

			o.push_back(inst);

			printf("%06x ", pos/*file.Tell() - totBytes*/);
			inst.PrintString(pos);
		}
		else
		{
			printf("%06x ", file.Tell() - 1);
			printf("%02x            ", b);
			printf("????    \n");
		}
	}

	return 0;
}

