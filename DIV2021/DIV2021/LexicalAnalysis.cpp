#include "stdafx.h"
#include "LexicalAnalysis.h"
#include "FST.h"
#include "Graphs.h"
#include "Split.h"
#include "In.h"
#include "Log.h"
#include "LT.h"
#include "IT.h"
#include "Error.h"
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

namespace Lex
{
	int Oct_to_dec(char* s)
	{
		int res = 0;
		for (int i = 2; s[i] != NULL; ++i)
		{
			res *= 8;
			res += (s[i] - '0');
		}
		return res;
	}
	int Bin_to_dec(char* s)
	{
		int res = 0;
		for (int i = 2; s[i] != NULL; ++i)
		{
			res *= 2;
			res += (s[i] - '0');
		}
		return res;
	}

	void ITLog(IT::IdTable it, Log::LOG log)
	{
		int i, numberOP = 0;
		*log.stream << "\nТаблица идентификаторов:\n\n";
		*log.stream << "№   " << " | " << "Идентификатор" << " | " << "Тип данных" << " | " << "Тип идентификатора" << " | " << "Индекс в ТЛ" << " | " << " Значение   " << endl;
		*log.stream << setw(87) << endl;
		for (i = 0; i < it.size; i++)
		{
			*log.stream << setfill('0') << setw(4) << right << i << "    ";
			*log.stream << setfill(' ') << setw(13) << left << it.table[i].id;
			switch (it.table[i].iddatatype)
			{
			case IT::INT: *log.stream << setw(10) << left;
				if (it.table[i].idtype == IT::OP)*log.stream << "-";
				else *log.stream << "integer   "; break;
			case IT::STR: *log.stream << setw(10) << left;
				if (it.table[i].idtype == IT::OP) *log.stream << "-";
				else *log.stream << "string    "; break;
			default: *log.stream << setw(10) << std::left << "unknown"; break;
			}
			switch (it.table[i].idtype)
			{
			case IT::V: *log.stream << left << "переменная     "; break;
			case IT::F: *log.stream << left << "функция        "; break;
			case IT::P: *log.stream << left << "параметр       "; break;
			case IT::L: *log.stream << left << "литерал        "; break;
			case IT::OP: *log.stream << left << "оператор       "; break;
				break;
			default: *log.stream << left << "unknown        "; break;
			}
			*log.stream << setw(11) << left << it.table[i].idxfirstLE;
			if (it.table[i].iddatatype == IT::INT && (it.table[i].idtype == IT::V || it.table[i].idtype == IT::L))
				*log.stream << setw(18) << left << it.table[i].value.vint;
			else if (it.table[i].iddatatype == IT::STR && (it.table[i].idtype == IT::V || it.table[i].idtype == IT::L))
				*log.stream << "[" << it.table[i].value.vstr->len << "]" << " \"" << it.table[i].value.vstr->str << "\" " << "";
			else if (it.table[i].idtype == IT::OP)
				*log.stream << setw(18) << left << it.table[i].value.op;
			else
				*log.stream << "       -       ";
			*log.stream << endl;
		}

		*log.stream << "\nКоличество идентификаторов:\n " << i << endl;

	}


	void LTLog(LT::LexTable lt, Log::LOG log)
	{

		int line = 1;
		*log.stream << "\n";
		*log.stream << "Таблица лексем:\n\n";
		*log.stream << line << "  ";
		for (int i = 0; i < lt.size; i++)
		{
			if (lt.table[i].lexema[0] == '~' || lt.table[i].lexema[0] == '{')
			{
				*log.stream << lt.table[i].lexema[0];
				line++;
				*log.stream << '\n';
				*log.stream << line << "  ";
				continue;
			}

			*log.stream << lt.table[i].lexema[0];
		}
	}

	LEX LexicalAnalysis(Log::LOG log, In::IN in)
	{
		LEX lex;
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);
		IT::Entry entryIT;

		int cHead = 0;
		int line = 1;
		int position = 0;
		int indxLex = 0;
		int indexID = 0;
		int cLit = 1;
		int cOperators = 1;


		bool findFunc = false;
		bool findInt = false;
		bool findString = false;
		bool findParm = false;
		bool findHead = false;
		bool findSameID = false;
		bool findNew = false;
		bool findBlock = false;
		bool findStandart = false;
		bool findUntil = false;

		char emptystr[] = "";
		char operators[] = "=+-*/%";

		char* L = new  char[2]{ "L" };
		char* OP = new  char[3]{ "OP" };
		char* bufL = new  char[TI_STR_MAXSIZE];
		char* nameLiteral = new  char[10]{ "" };
		char* charclit = new char[10]{ "" };
		char* prefix = new  char[10]{ "" };
		char* bufprefix = new  char[10]{ "" };
		char* oldprefix = new  char[10]{ "" };


		char** word = new char* [max_word];
		for (int i = 0; i < max_word; i++)
			word[i] = new char[size_word] {NULL};

		for (int i = 0; i < max_word; i++)
			strcpy(word[i], SplitText((char*)in.text, in.size)[i]);

		for (int i = 0; i < max_word; i++)
		{
			if (word[i][0] == '|')
			{
				position = 0;
				line++;
				continue;
			}
			position++;

			FST::FST fstReturn(word[i], FST_RETURN);
			if (FST::execute(fstReturn))
			{
				LT::Entry entryLT = WriteEntry(entryLT, LEX_RETURN, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				indxLex++;
				continue;
			}

			FST::FST fstPrint(word[i], FST_PRINT);
			if (FST::execute(fstPrint))
			{
				LT::Entry entryLT = WriteEntry(entryLT, LEX_PRINT, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				indxLex++;
				continue;
			}

			FST::FST fstUntil(word[i], FST_UNTIL);
			if (FST::execute(fstUntil))
			{
				LT::Entry entryLT = WriteEntry(entryLT, LEX_UNTIL, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findUntil = true;
				indxLex++;
				continue;
			}

			FST::FST fstComment(word[i], FST_COMMENT);
			if (FST::execute(fstComment))
			{
				continue;
			}

			FST::FST fstFunction(word[i], FST_FUNCTION);
			if (FST::execute(fstFunction))
			{
				LT::Entry entryLT = WriteEntry(entryLT, LEX_FUNCTION, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				indxLex++;
				findFunc = true;
				continue;
			}

			FST::FST fstNew(word[i], FST_NEW);
			if (FST::execute(fstNew))
			{
				LT::Entry entryLT = LT::WriteEntry(entryLT, LEX_NEW, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				findNew = true;
				indxLex++;
				continue;
			}

			FST::FST fstHead(word[i], FST_HEAD);
			if (FST::execute(fstHead))
			{
				indxLex++;
				strcpy(oldprefix, prefix);
				strcpy(prefix, word[i]);
				LT::Entry entryLT = LT::WriteEntry(entryLT, LEX_HEAD, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				findHead = true;
				cHead++;
				continue;
			}

			FST::FST fstTypeInteger(word[i], FST_INTEGER);
			if (FST::execute(fstTypeInteger))
			{
				LT::Entry entryLT = WriteEntry(entryLT, LEX_INTEGER, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				indxLex++;
				findInt = true;
				continue;
			}

			FST::FST fstTypeString(word[i], FST_STRING);
			if (FST::execute(fstTypeString))
			{
				LT::Entry entryLT = WriteEntry(entryLT, LEX_STRING, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				indxLex++;
				findString = true;
				continue;
			}

			FST::FST fstLeftBrace(word[i], FST_LEFTBRACE);
			if (FST::execute(fstLeftBrace))
			{
				LT::Entry entryLT = LT::WriteEntry(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				findBlock = true;
				indxLex++;
				continue;
			}

			FST::FST fstBraceLet(word[i], FST_BRACELET);
			if (FST::execute(fstBraceLet))
			{
				LT::Entry entryLT = LT::WriteEntry(entryLT, LEX_BRACELET, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				if (!findUntil)
					findBlock = false;
				else findUntil = false;
				indxLex++;
				continue;
			}

			FST::FST fstLeftThesis(word[i], FST_LEFTTHESIS);
			if (FST::execute(fstLeftThesis))
			{
				LT::Entry entryLT = LT::WriteEntry(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line);
				entryLT.priority = 0;
				LT::Add(lextable, entryLT);

				if (idtable.table[indexID - 1].idtype == IT::F)
					findParm = true;
				indxLex++;
				continue;
			}

			FST::FST fstRightThesis(word[i], FST_RIGHTTHESIS);
			if (FST::execute(fstRightThesis))
			{
				LT::Entry entryLT = LT::WriteEntry(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line);
				entryLT.priority = 0;
				LT::Add(lextable, entryLT);

				if (findStandart) {				// если найдена стандартная функция и закрытая скобка, меняем префикс на предыдущий
					strcpy(prefix, oldprefix);
					findStandart = false;
				}
				findNew = false;
				findParm = false;
				indxLex++;
				continue;
			}

			FST::FST fstPlus(word[i], FST_PLUS);
			FST::FST fstMinus(word[i], FST_MINUS);
			if (FST::execute(fstPlus) || FST::execute(fstMinus))
			{
				_itoa_s(cOperators++, charclit, sizeof(char) * 10, 10);
				strcpy(bufL, OP);
				nameLiteral = strcat(bufL, (char*)charclit);

				if (word[i][0] == '-' && lextable.table[lextable.size - 1].lexema[0] != 'i' && lextable.table[lextable.size - 1].lexema[0] != 'l')
					continue;
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.op == word[i][0])
					{
						findSameID = true;
						LT::Entry entryLT = WriteEntry(entryLT, LEX_OPERATOR, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID) 	// если найден такой же стр. литерал
				{
					findSameID = false; continue;
				}
				entryIT.value.op = word[i][0];
				strcpy(entryIT.id, nameLiteral);
				entryIT.idxfirstLE = indxLex;
				entryIT.iddatatype = IT::STR;
				entryIT.idtype = IT::OP;

				IT::Add(idtable, entryIT);
				indexID++;
				LT::Entry entryLT = WriteEntry(entryLT, LEX_OPERATOR, idtable.size - 1, line);
				entryLT.priority = 2;
				LT::Add(lextable, entryLT);


				indxLex++;
				continue;
			}


			FST::FST fstStar(word[i], FST_STAR);
			FST::FST fstDirslash(word[i], FST_DIRSLASH);
			FST::FST fstProc(word[i], FST_PROC);
			if (FST::execute(fstStar) || FST::execute(fstDirslash) || FST::execute(fstProc))
			{
				_itoa_s(cOperators++, charclit, sizeof(char) * 10, 10);
				strcpy(bufL, OP);
				nameLiteral = strcat(bufL, (char*)charclit);

				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.op == word[i][0])
					{
						findSameID = true;
						LT::Entry entryLT = WriteEntry(entryLT, LEX_OPERATOR, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID)
				{
					findSameID = false; continue;
				}
				entryIT.value.op = word[i][0];
				strcpy(entryIT.id, nameLiteral);
				entryIT.idxfirstLE = indxLex;
				entryIT.iddatatype = IT::STR;
				entryIT.idtype = IT::OP;

				IT::Add(idtable, entryIT);
				indexID++;
				LT::Entry entryLT = WriteEntry(entryLT, LEX_OPERATOR, idtable.size - 1, line);
				entryLT.priority = 3;
				LT::Add(lextable, entryLT);


				indxLex++;
				continue;
			}


			FST::FST fstEqual(word[i], FST_EQUAL);
			if (FST::execute(fstEqual))
			{
				LT::Entry entryLT = WriteEntry(entryLT, LEX_EQUAL, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				indxLex++;
				continue;
			}


			FST::FST fstSeparator(word[i], FST_SEPARATOR);
			if (FST::execute(fstSeparator))
			{
				LT::Entry entryLT = WriteEntry(entryLT, LEX_SEPARATOR, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				indxLex++;
				continue;
			}

			FST::FST fstComma(word[i], FST_COMMA);
			if (FST::execute(fstComma))
			{
				LT::Entry entryLT = WriteEntry(entryLT, LEX_COMMA, LT_TI_NULLIDX, line);
				entryLT.priority = 1;
				LT::Add(lextable, entryLT);

				indxLex++;
				continue;
			}


			FST::FST fstIdentif(word[i], FST_ID);
			if (FST::execute(fstIdentif))
			{

				if (strlen(word[i]) > 7)
				{
					for (int j = strlen(word[i]) - 1; j > 5; j--)
					{
						word[i][j] = NULL;
						sizeof(word[i])--;
					}
				}
				indxLex++;

				int idx = IT::IsId(idtable, word[i]);
				if (findFunc)
				{
					strcpy(oldprefix, prefix);
					strcpy(prefix, word[i]);
					if (!findFunc)
						findStandart = true;

				}
				else {
					if ((findBlock || findParm) && (strcmp(word[i], STANDART1) != 0 && strcmp(word[i], STANDART2) != 0))
					{
						if (!(idx != TI_NULLIDX && word[i + 1][0] == '('))     //если это не вызов функции
						{
							strcpy(bufprefix, prefix);
							word[i] = strcat(bufprefix, word[i]);
						}
					}
				}

				idx = IT::IsId(idtable, word[i]);
				if (idx != TI_NULLIDX)
				{
					if (findNew == true)				//Проверка на повторное объявление
						throw ERROR_THROW_IN(6, line, position);

					LT::Entry entryLT = WriteEntry(entryLT, LEX_ID, idx, line);
					LT::Add(lextable, entryLT);
					continue;
				}
				else {

					if (findNew && !findInt && !findString)
						throw ERROR_THROW_IN(4, line, position);

					if (strcmp(word[i], STANDART1) == 0 || strcmp(word[i], STANDART2) == 0 || findFunc)
					{
						entryIT.idxfirstLE = indxLex;
						strcpy(entryIT.id, word[i]);
						if (findString || strcmp(word[i], STANDART1) == 0 || strcmp(word[i], STANDART2) == 0)
							entryIT.iddatatype = IT::STR;
						else { entryIT.iddatatype = IT::INT; }
						entryIT.idtype = IT::F;
						entryIT.value.vstr->len = 0;
						strcpy(entryIT.value.vstr->str, emptystr);

						IT::Add(idtable, entryIT);
						idx = IT::IsId(idtable, word[i]);
						indexID++;

						LT::Entry entryLT = WriteEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);

						findFunc = false;
						findInt = false; //опускаем флаги, активированные при объявлении типа возвращаемого значения функции
						findString = false;
						continue;
					}

					if (findInt || findString)
					{
						if (findParm)
						{
							entryIT.idtype = IT::P;
						}
						else { entryIT.idtype = IT::V; }

						entryIT.idxfirstLE = indxLex;
						strcpy(entryIT.id, word[i]);
						if (findInt)
						{
							entryIT.iddatatype = IT::INT;
							entryIT.value.vint = 0;
						}
						else {
							entryIT.iddatatype = IT::STR;
							entryIT.value.vstr->len = 0;
							strcpy(entryIT.value.vstr->str, emptystr);
						}

						IT::Add(idtable, entryIT);
						int idx = IT::IsId(idtable, word[i]);
						indexID++;

						LT::Entry entryLT = WriteEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);


						if (findInt)findInt = false;
						else findString = false;
						findNew = false;
						continue;
					}

				}
			}

			FST::FST fstIntLit(word[i], FST_INTLIT);
			if (FST::execute(fstIntLit))
			{
				indxLex++;
				_itoa_s(cLit++, charclit, sizeof(char) * 10, 10);
				strcpy(bufL, L);
				nameLiteral = strcat(bufL, (char*)charclit);

				int value = atoi((char*)word[i]);
				if (word[i - 1][0] == '-' && lextable.table[lextable.size - 1].lexema[0] != 'l' && lextable.table[lextable.size - 1].lexema[0] != 'i')
					value = -value;
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vint == value)
					{
						findSameID = true;
						LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID)
				{
					findSameID = false; continue;
				}

				entryIT.value.vint = value;
				strcpy(entryIT.id, nameLiteral);
				entryIT.idxfirstLE = indxLex;
				entryIT.iddatatype = IT::INT;
				entryIT.idtype = IT::L;

				IT::Add(idtable, entryIT);
				indexID++;
				LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, idtable.size - 1, line);
				LT::Add(lextable, entryLT);


				continue;
			}

			FST::FST fstBinLit(word[i], FST_BIN);
			if (FST::execute(fstBinLit))
			{
				indxLex++;
				_itoa_s(cLit++, charclit, sizeof(char) * 10, 10);
				strcpy(bufL, L);
				nameLiteral = strcat(bufL, (char*)charclit);

				int value = Bin_to_dec(word[i]);
				if (word[i - 1][0] == '-' && lextable.table[lextable.size - 1].lexema[0] != 'l' && lextable.table[lextable.size - 1].lexema[0] != 'i')
					value = -value;
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vint == value)
					{
						findSameID = true;
						LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID)
				{
					findSameID = false; continue;
				}
				entryIT.value.vint = value;
				strcpy(entryIT.id, nameLiteral);
				entryIT.idxfirstLE = indxLex;
				entryIT.iddatatype = IT::INT;
				entryIT.idtype = IT::L;

				IT::Add(idtable, entryIT);
				indexID++;
				LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, idtable.size - 1, line);
				LT::Add(lextable, entryLT);
				continue;
			}


			FST::FST fstOctLit(word[i], FST_OCT);
			if (FST::execute(fstOctLit))
			{
				indxLex++;
				_itoa_s(cLit++, charclit, sizeof(char) * 10, 10);
				strcpy(bufL, L);
				nameLiteral = strcat(bufL, (char*)charclit);

				int value = Oct_to_dec(word[i]);
				if (word[i - 1][0] == '-' && lextable.table[lextable.size - 1].lexema[0] != 'l' && lextable.table[lextable.size - 1].lexema[0] != 'i')
					value = -value;
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vint == value)
					{
						findSameID = true;
						LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID)
				{
					findSameID = false; continue;
				}
				entryIT.value.vint = value;
				strcpy(entryIT.id, nameLiteral);
				entryIT.idxfirstLE = indxLex;
				entryIT.iddatatype = IT::INT;
				entryIT.idtype = IT::L;

				IT::Add(idtable, entryIT);
				indexID++;
				LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, idtable.size - 1, line);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstStrLit(word[i], FST_STRLIT);
			if (FST::execute(fstStrLit))
			{
				indxLex++;

				int length = strlen(word[i]);
				for (int k = 0; k < length; k++)
					word[i][k] = word[i][k + 1];
				word[i][length - 2] = 0;


				for (int k = 0; k < idtable.size; k++)
				{
					if (strcmp(idtable.table[k].value.vstr->str, word[i]) == 0)
					{
						findSameID = true;
						LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID)
				{
					findSameID = false; continue;
				}

				_itoa_s(cLit++, charclit, sizeof(char) * 10, 10);
				strcpy(bufL, L);
				nameLiteral = strcat(bufL, (char*)charclit);

				entryIT.idxfirstLE = indxLex;
				strcpy(entryIT.id, nameLiteral);
				entryIT.iddatatype = IT::STR;
				entryIT.idtype = IT::L;

				entryIT.value.vstr->len = length - 2;
				strcpy(entryIT.value.vstr->str, word[i]);
				IT::Add(idtable, entryIT);
				indexID++;

				int idx = IT::IsId(idtable, word[i]);
				LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, idtable.size - 1, line);
				LT::Add(lextable, entryLT);

				continue;
			}
			if (strcmp(word[i], emptystr) != 0)throw ERROR_THROW_IN(5, line, position);
		}
		if (!findHead) throw ERROR_THROW(2);
		if (cHead > 1) throw ERROR_THROW(3);

		lex.lextable = lextable;
		lex.idtable = idtable;
		return lex;
	}
}