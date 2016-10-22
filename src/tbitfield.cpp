// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) { throw "ne to chislo!"; }
	BitLen = len;
	MemLen = (len + sizeof(int)*8 - 1)/(sizeof(int)*8);
	pMem = new ui[MemLen];
	for (int i = 0; i < MemLen; i++) { pMem[i] = 0; }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new ui[MemLen];
	if (pMem != NULL)
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];

}

TBitField::~TBitField()
{
	delete[]pMem;
	pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n < 0) || (n >= BitLen)) { throw "ne to chislo!"; }
	return (n / (sizeof(int)* 8));
}

ui TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n < 0) || (n >= BitLen))  { throw "ne to chislo!"; }
	int j, m;
	m = n % (sizeof(int)* 8);
	j = 1 << m;
	return j;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= BitLen))  { throw "ne to chislo!"; }
	if ((n >= 0) && (n< BitLen)) pMem[GetMemIndex(n)] |= GetMemMask(n); 
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n>-1) && (n<BitLen))
		pMem[GetMemIndex(n)] &= ~GetMemMask(n); else throw "Error";

	//if ((n < 0) || (n >= BitLen))  { throw "ne to chislo!"; }	if ((n >= 0) && (n< BitLen)) pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= BitLen))  { throw "ne to chislo!"; }
	else return (pMem[GetMemIndex(n)] & GetMemMask(n));
	return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf) return*this;
	BitLen = bf.BitLen;
	if (bf.pMem == NULL)
	{
		delete pMem; BitLen = bf.BitLen; MemLen = bf.MemLen; return*this;
	}
	if (MemLen != bf.MemLen)
	{
		if (pMem != NULL)delete pMem; MemLen = bf.MemLen; pMem = new ui[MemLen];
	}
	BitLen = bf.BitLen;
	for (int i = 0; i<bf.MemLen; i++)pMem[i] = bf.pMem[i]; return*this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 0;
	else for (int i = 0; i < MemLen; i++)
	if (pMem[i] != bf.pMem[i]) return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 1;
	else for (int i = 0; i < MemLen; i++)
	if (pMem[i] != bf.pMem[i]) return 1;
	return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = BitLen;
	if (bf.BitLen> len) len = bf.BitLen;
	TBitField bf2(len);
	for (int i = 0; i < MemLen; i++) bf2.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++) bf2.pMem[i] |= bf.pMem[i];
		return bf2;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = BitLen;
	if (bf.BitLen> len) len = bf.BitLen;
	TBitField bf2(len);
	for (int i = 0; i < MemLen; i++) bf2.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++) bf2.pMem[i] &= bf.pMem[i];
	return bf2;
}

TBitField TBitField::operator~(void) // отрицание
{
	int len = BitLen;
	int l = 0;  ui k = 0;
	TBitField bf2(len),bf3(len);
	for (int i = 0; i < MemLen; i++) { bf2.pMem[i] = ~pMem[i]; }
	l = (len % (MemLen*(sizeof(int) * 8) ));
	if (l != 0)
	{ for (k = len-1; k < (MemLen*(sizeof(int) * 8)); k++) bf2.ClrBit(k); }
	
	bf3 = bf2;
	
	return bf2;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	char v;
	do {istr >> v;} while ( v != 0);
	while (1) { istr >> v;
	if (v == '0') bf.ClrBit(i++);
	else if (v == '1') bf.SetBit(i++); else break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len = bf.GetLength();
	for ( int i=0;i < len; i++) if (bf.GetBit(i)) ostr << '1'; else ostr << '0';
	return ostr;
}
