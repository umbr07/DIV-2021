#pragma once
#include "stdafx.h"
#include "LT.h"
#include "IT.h"
#include "Log.h"

namespace Sem
{
	void CheckParm(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);					//�������� ���������� ������� �� ���������� �� ���� � ����������
	void CheckReturnType(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);				//�������� ���������� ���� ������� � ���� ������������� ��������
	void CheckAssignValue(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);			//�������� ������������ ���� ���������� � �������������� ��������
	void CheckAllowedValue(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);			//�������� ������������� �������� �������� �� ������������
	void CheckFunc(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);				// �������� �������
}
