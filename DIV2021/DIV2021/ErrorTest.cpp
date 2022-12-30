#include "IT.h"
#include "LT.h"

#define ER120r 120
#define ER121r 121
#define ER122r 122
#define ER123r 123
namespace ErrorTest
{
	void Test()
	{
#ifdef ER120
		LT::LexTable lextable = LT::Create(LT_MAXSIZE + 1);								//превышение размера тл
#endif
#ifdef ER122
		IT::IdTable idtable = IT::Create(TI_MAXSIZE + 1);								//превышение размера ти	
#endif
#ifdef ER121
		LT::LexTable lextable = LT::Create(2);										//переполнение тл
		LT::Entry entryLT1 = WriteEntry(entryLT1, '1', LT_TI_NULLIDX, 1);
		LT::Add(lextable, entryLT1);
		LT::Entry entryLT2 = WriteEntry(entryLT2, '2', LT_TI_NULLIDX, 2);
		LT::Add(lextable, entryLT2);
		LT::Entry entryLT3 = WriteEntry(entryLT3, '3', LT_TI_NULLIDX, 3);
		LT::Add(lextable, entryLT3);
#endif
#ifdef ER123
		IT::IdTable idtable = IT::Create(2);											//переполнение ти
		IT::Entry entryIT1;
		IT::Add(idtable, entryIT1);
		IT::Entry entryIT2;
		IT::Add(idtable, entryIT2);
		IT::Entry entryIT3;
		IT::Add(idtable, entryIT3);
#endif
	}
}