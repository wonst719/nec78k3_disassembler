#include <vector>

#include "Types.h"
#include "File.h"

#include "Mnemonic.h"

#include "Operand.h"

#include "Opcode.h"
#include "Opcodes.h"

#include "OpcodeInstance.h"

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
