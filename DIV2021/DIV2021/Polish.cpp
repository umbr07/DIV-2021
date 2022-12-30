#include "stdafx.h"
#include "LexicalAnalysis.h"
#include <stack>
#include <queue>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

bool PolishNotation(int i, LT::LexTable& lextable, IT::IdTable& idtable)
{
	stack<LT::Entry> stack;		// ���� ��� ��������
	queue<LT::Entry> queue;		// ������� ��� ���������
	LT::Entry temp;
	temp.idxTI = -1;
	temp.lexema[0] = '#';
	temp.sn = -1;
	LT::Entry func;
	func.lexema[0] = '@';
	LT::Entry commas;
	int countComma = 0;			//������� ���������� �������
	char* comma = new char[1]{ "" };
	int countLex = 0;			// ���������� ��������������� ������
	int posLex = i;				// ���������� ����� ������� ����� ���������������
	bool findFunc = false;


	for (i; lextable.table[i].lexema[0] != LEX_SEPARATOR; i++, countLex++)
	{
		switch (lextable.table[i].lexema[0])
		{
		case LEX_ID:	// ���� �������������
		{
			queue.push(lextable.table[i]);   // �������� � �������
			continue;
		}
		case LEX_LITERAL:		// ���� �������
		{
			queue.push(lextable.table[i]);	// �������� � �������
			continue;
		}
		case LEX_LEFTTHESIS:		// ���� (
		{
			if (idtable.table[lextable.table[i - 1].idxTI].idtype == IT::F)
				findFunc = true;
			stack.push(lextable.table[i]); // �������� �� � ����
			continue;
		}
		case LEX_RIGHTTHESIS:	// ���� )
		{
			if (findFunc)
			{
				itoa(++countComma, comma, 10);
				strcpy(commas.lexema, comma);
				stack.push(commas);
				stack.push(func);
				findFunc = false;
			}
			while (stack.top().lexema[0] != LEX_LEFTTHESIS)	// ���� �� �������� (
			{
				queue.push(stack.top());	// ����������� �� ����� � �������
				stack.pop();

				if (stack.empty())
					return false;
			}
			stack.pop();	// ���������� (
			continue;
		}
		case LEX_OPERATOR:	// ���� ���� ���������
		{
			while (!stack.empty() && lextable.table[i].priority <= stack.top().priority)
				// ���� ��������� �������� ��������� 
				//������ ��� ����� ���������� ��������� � ������� �����
			{
				queue.push(stack.top());	// ����������� �� ����� � �������� ������
				stack.pop();
			}
			stack.push(lextable.table[i]);
			continue;
		}
		case LEX_COMMA:
		{
			countComma++;
			continue;
		}
		}
	}
	while (!stack.empty())	// ���� ���� �� ������
	{
		if (stack.top().lexema[0] == LEX_LEFTTHESIS || stack.top().lexema[0] == LEX_RIGHTTHESIS)
			return false;
		queue.push(stack.top());	// ����������� ��� � �������
		stack.pop();
	}
	while (countLex != 0)		// ������ �������� ��������� � ������� ������ �� ��������� � �����
	{
		if (!queue.empty()) {
			lextable.table[posLex++] = queue.front();
			//cout << lex.idtable.table[queue.front().idxTI].id;	// ����� � �������
			queue.pop();
		}
		else
		{
			lextable.table[posLex++] = temp;			
		}
		countLex--;
	}

	for (int i = 0; i < posLex; i++)		// �������������� �������� ������� ��������� 
											//� ������� ������ � ���������� �� ������� ���������������
	{
		if (lextable.table[i].lexema[0] == LEX_OPERATOR || lextable.table[i].lexema[0] == LEX_LITERAL)
			idtable.table[lextable.table[i].idxTI].idxfirstLE = i;
	}
	return true;
}

void PolishStart(Lex::LEX lex)
{
	for (int i = 0; i < lex.lextable.size; i++)
	{
		if (lex.lextable.table[i].lexema[0] == '=')
		{
			PolishNotation(i + 1, lex.lextable, lex.idtable);
		}
	}
}