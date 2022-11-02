// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <cmath>


TBitField::TBitField(int len)
{
	if (len > 0)
	{

		BitLen = len;
		MemLen = BitLen / (sizeof(TELEM) * 8) + 1;
		pMem = new TELEM[MemLen];
		memset(pMem, 0, MemLen * sizeof(TELEM));
	}
	else
	{
		throw invalid_argument("Invalid Value of Lenght of BitField");
	}
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % (8 * sizeof(TELEM)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{

	if (n >= BitLen || n < 0)
	{
		throw out_of_range("Incorrect bit num");
	}
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= BitLen || n < 0)
	{
		throw out_of_range("Incorrect bit num");
	}
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= BitLen || n < 0)
	{
		throw out_of_range("Incorrect bit num");
	}
	if (!(pMem[GetMemIndex(n)] & GetMemMask(n)))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (*this == bf)
	{
		return *this;
	}
	delete[] pMem;
	MemLen = bf.MemLen;
	BitLen = bf.BitLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 0;
	if (MemLen == bf.MemLen)
	{
		//int n = 0;
		for (int i = 0; i < MemLen; i++)
		{
			if (pMem[GetMemIndex(i)] != bf.pMem[bf.GetMemIndex(i)]) return 0;
		}
	}
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if (*this == bf)return 0;
	return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	if (BitLen >= bf.BitLen)
	{
		TBitField _temp(*this);
		for (int i = 0; i < bf.MemLen; i++)
		{
			_temp.pMem[_temp.GetMemIndex(i)] = _temp.pMem[_temp.GetMemIndex(i)] | bf.pMem[bf.GetMemIndex(i)];
		}
		return _temp;
	}
	else
	{
		TBitField _temp(bf);
		for (int i = 0; i < MemLen; i++)
		{
			_temp.pMem[_temp.GetMemIndex(i)] = _temp.pMem[_temp.GetMemIndex(i)] | pMem[GetMemIndex(i)];
		}
		return _temp;
	}
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	if (BitLen >= bf.BitLen)
	{
		int i = 0;
		TBitField _temp(*this);
		for (i; i < bf.MemLen; i++)
		{
			_temp.pMem[_temp.GetMemIndex(i)] = _temp.pMem[_temp.GetMemIndex(i)] & bf.pMem[bf.GetMemIndex(i)];
		}
		if (BitLen != bf.BitLen)
		{
			for (i; i < MemLen; i++)
			{
				ClrBit(i);
			}
		}
		cout << _temp << endl;
		
		return _temp;
	}
	else
	{
		int i = 0;
		TBitField _temp(bf);
		for (i; i < MemLen; i++)
		{
			_temp.pMem[_temp.GetMemIndex(i)] = _temp.pMem[_temp.GetMemIndex(i)] & pMem[GetMemIndex(i)];
		}
		for (i; i < bf.MemLen; i++)
		{
			ClrBit(i);
		}
		cout << _temp << endl;
		return _temp;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	for (int i = 0; i < BitLen; i++)
	{
		if (GetBit(i) == 0)
		{
			SetBit(i);
		}
		else
		{
			ClrBit(i);
		}
	}
	return *this;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	unsigned int _temp;
	for (int i = 0; i < bf.GetLength(); i++)
	{
		istr >> _temp;
		if (_temp == 0)
		{
			bf.ClrBit(i);
		}
		else if (_temp == 1)
		{
			bf.SetBit(i);
		}
		else
		{
			throw invalid_argument("Value is non-binary");
		}
	}
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	unsigned int _temp;
	for (int i = 0; i < bf.GetLength(); i++)
	{
		ostr << bf.GetBit(i);

	}
	return ostr;

}