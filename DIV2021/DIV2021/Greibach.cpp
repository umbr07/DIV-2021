#include "stdafx.h"
#include <cstring>
#include "Rule.h"
#include "Greibach.h"

namespace GRB
{

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)	// конструктор
	{
		nt = new GRBALPHABET[size = psize];  //цепочка терминалов
		int* p = (int*)&s; //присваиваем символ указателю п
		for (short i = 0; i < psize; ++i)
			nt[i] = (GRBALPHABET)p[i]; //заполняем цепочку терминалов
	};

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) // конструктор правила
	{
		nn = pnn;  //нетерминал
		iderror = piderror; //идентификатор
		chains = new Chain[size = psize]; //место для цепочки
		Chain* p = &c;
		for (int i = 0; i < size; i++)
			chains[i] = p[i]; //заполняем множество цепочек
	};

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...) //конструктор гграматики Грейбаха(стартовый символ, дно стека, количество правил, правила...)
	{
		startN = pstartN;  //стартовый символ
		stbottomT = pstbottom; //дно стека
		rules = new Rule[size = psize];  //выделяем память
		Rule* p = &r;
		for (int i = 0; i < size; i++)
			rules[i] = p[i]; //заполняем правила
	};

	Greibach getGreibach() { return greibach; };

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule) // получить правило, возвращается номер правила или -1
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn) k++; //пока К меньше количества правил и пока левый символ правила не равен парметру ф-ции
		if (k < size) prule = rules[rc = k]; //возвращаемое правило граматики равно правилу с индексом К
		return rc;  //возвращается номер правила или -1
	};

	Rule Greibach::getRule(short n)	//получить правило по номеру
	{
		Rule rc;      //создаём правило рц
		if (n < size) rc = rules[n];   //присваиваем правилу рц правило н
		return rc;  //возвращаем созданное правило
	};

	char* Rule::getCRule(char* b, short nchain)		// полчить правило в виде N->цепочка
	{
		char bchain[200]; //строка
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-'; b[2] = '>'; b[3] = 0x00; //терминал ->
		chains[nchain].getCChain(bchain); //получает правую сторонц правила
		strcat_s(b, sizeof(bchain) + 5, bchain); //добавляем строку (куда, с какого элемента, строку)
		return b;
	};

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) //получить следующую за j подходящую цепочку, вернуть ее номер или -1
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t) ++j;		// цикл по всем цепочкам правила
		rc = (j < size ? j : -1);
		if (rc >= 0) pchain = chains[rc];
		return rc;
	};

	char* Rule::Chain::getCChain(char* b)	// получить правую сторону правила (цепочку)
	{
		for (int i = 0; i < size; i++)
			b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};

}
