#include "stdafx.h"
#include <iostream>
#include "IT.h"
#include <stack>
#include "SemanticAnalysis.h"
#include "LexicalAnalysis.h"
#include "LT.h"
#include "Error.h"
using namespace std;

namespace Sem
{
	void CheckParm(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		stack<LT::Entry> called;
		stack<LT::Entry> buf;
		stack<LT::Entry> call;
		char calledBuf[10];
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].idxTI != TI_NULLIDX && lextable.table[i].lexema[0] == LEX_ID)
			{
				if (idtable.table[lextable.table[i].idxTI].idtype == IT::F && lextable.table[i - 1].lexema[0] == LEX_FUNCTION)
				{
					strcpy(calledBuf, idtable.table[lextable.table[i].idxTI].id);
					i++;
					while (lextable.table[i].lexema[0] != LEX_RIGHTTHESIS)
					{
						if (lextable.table[i].lexema[0] == LEX_ID || lextable.table[i].lexema[0] == LEX_LITERAL)
						{
							called.push(lextable.table[i]);
						}
						i++;
					}

					for (int j = i; j < lextable.size; j++)
					{
						if (lextable.table[j].lexema[0] == LEX_ID && strcmp(idtable.table[lextable.table[j].idxTI].id, calledBuf) == 0)
						{
							j++;
							while (lextable.table[j].lexema[0] != LEX_RIGHTTHESIS)
							{
								if (lextable.table[j].lexema[0] == LEX_ID || lextable.table[j].lexema[0] == LEX_LITERAL)
									call.push(lextable.table[j]);
								j++;
							}

							while (!called.empty() && !call.empty())
							{
								if (idtable.table[called.top().idxTI].iddatatype != idtable.table[call.top().idxTI].iddatatype)
								{
									throw ERROR_THROW_IN(131, lextable.table[j].sn, -1);
								}
								buf.push(called.top());
								called.pop();
								call.pop();
							}
							if (called.size() != call.size())
							{
								throw ERROR_THROW_IN(130, lextable.table[j].sn, -1);
							}
							else {
								while (!call.empty())
									call.pop();
								while (!called.empty())
									called.pop();
								while (!buf.empty())
								{
									called.push(buf.top());
									buf.pop();
								}
							}
						}
					}
				}
			}
		}
	}


	void CheckReturnType(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		bool error = false;
		int buf = IT::NO; bool findHead = false, findLocalFunc = false;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_HEAD)
				findHead = true;
			if (lextable.table[i].lexema[0] == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::F && lextable.table[i - 2].lexema[0] == 't')
			{
				findLocalFunc = true;
				if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::INT)
					buf = IT::INT;
				else { buf = IT::STR; }
				continue;
			}
			if (lextable.table[i - 1].lexema[0] == LEX_RETURN)
			{
				if (findHead && !findLocalFunc)
					continue;
				if (idtable.table[lextable.table[i].idxTI].iddatatype != buf)
				{
					throw ERROR_THROW_IN(132, lextable.table[i].sn, i - 1);
					error = true;
				}
				findLocalFunc = false;
			}
		}
	}


	void CheckAssignValue(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		bool error = false;
		stack<LT::Entry> st;
		int buf = 0; bool findFunc = false, findOperator = false;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i - 1].lexema[0] == LEX_EQUAL && lextable.table[i - 2].lexema[0] == LEX_ID)
			{

				buf = idtable.table[lextable.table[i - 2].idxTI].iddatatype;
				while (lextable.table[i].lexema[0] != LEX_SEPARATOR)
				{
					if (lextable.table[i].lexema[0] == LEX_OPERATOR)
						findOperator = true;
					if (lextable.table[i - 1].lexema[0] == LEX_ID && idtable.table[lextable.table[i - 1].idxTI].idtype == IT::F && lextable.table[i].lexema[0] == LEX_LEFTTHESIS)
					{
						while (lextable.table[i].lexema[0] != LEX_RIGHTTHESIS)
							i++;
					}
					if (lextable.table[i].lexema[0] == LEX_ID || lextable.table[i].lexema[0] == LEX_LITERAL)
						st.push(lextable.table[i]);
					i++;
				}
				while (!st.empty())
				{
					if (idtable.table[st.top().idxTI].iddatatype != buf)
					{
						if (findOperator)
						{
							throw ERROR_THROW_IN(134, lextable.table[i].sn, -1);

						}
						else {
							throw ERROR_THROW_IN(133, lextable.table[i].sn, 3);
						}
					}
					st.pop();
				}
				findOperator = false;
			}
		}
	}

	void CheckAllowedValue(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		bool findStr = false, findOp = false;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_LITERAL && idtable.table[lextable.table[i].idxTI].value.vint == 2147483647)
				throw ERROR_THROW_IN(135, lextable.table[i].sn, i);

			if (lextable.table[i].lexema[0] == LEX_EQUAL)
			{
				while (lextable.table[i].lexema[0] != LEX_SEPARATOR)
				{
					if (lextable.table[i].lexema[0] == LEX_ID && idtable.table[lextable.table[i].idxTI].iddatatype == IT::STR)
						findStr = true;
					if (findStr && lextable.table[i].lexema[0] == LEX_OPERATOR && idtable.table[lextable.table[i].idxTI].value.op != '+')
						throw ERROR_THROW_IN(136, lextable.table[i].sn, -1);
					i++;
				}
			}
			if (lextable.table[i].lexema[0] == 'i' && lextable.table[i].idxTI == TI_NULLIDX && lextable.table[i - 1].lexema[0] != 't' && lextable.table[i - 1].lexema[0] != 'f')
				throw ERROR_THROW_IN(137, lextable.table[i].sn, -1);
		}
	}

	void CheckFunc(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_ID && (idtable.table[lextable.table[i].idxTI].id == STANDART1 || idtable.table[lextable.table[i].idxTI].id == STANDART2))
			{
				int j = i;
				while (j >= i - 3)
				{
					if (lextable.table[i].lexema[0] == LEX_NEW || lextable.table[i].lexema[0] == LEX_FUNCTION || lextable.table[i].lexema[0] == 't')
						throw ERROR_THROW_IN(138, lextable.table[i].sn, -1);
					j--;
				}
			}
		}
	}
}