#pragma once

#include <stdio.h>
#include <string>
#include "Types.h"

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
