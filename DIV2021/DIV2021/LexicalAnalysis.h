#pragma once
#include "stdafx.h"
#include "LT.h"
#include "IT.h"
#include "Log.h"
#include "In.h"

#define STANDART1 "joinst"
#define STANDART2 "copystr"

namespace Lex
{
	struct LEX
	{
		LT::LexTable lextable;
		IT::IdTable idtable;
	};

	LEX LexicalAnalysis(Log::LOG log, In::IN in);
	void LTLog(LT::LexTable lt, Log::LOG log);
	void ITLog(IT::IdTable it, Log::LOG log);
};

