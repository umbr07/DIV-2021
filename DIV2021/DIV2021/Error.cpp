#include "Error.h"

namespace Error
{
	ERROR error[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "������������ ��� ������"),
		ERROR_ENTRY(1, "��������� ����"),
		ERROR_ENTRY(2, "Sem: ����������� ����� ����� head"),
		ERROR_ENTRY(3, "Sem: ��������� ������ 1 ����� ����� head"),
		ERROR_ENTRY(4, "Sem: ���������� ��� ������� ������ ����� ������������ ��� ������"),
		ERROR_ENTRY(5, "Lex: ������� �� ����������"),
		ERROR_ENTRY(6, "Sem: ��������� ���������� ���������� ���������"),ERROR_ENTRY_NODEF(7),
		ERROR_ENTRY_NODEF(8),ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10),
		ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30),
		ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60),
		ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "�������� -in ������ ���� �����"),
		ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "��������� ����� �������� ���������"), ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106),
		ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "������ ��� �������� ����� � �������� ����� (-in)"),
		ERROR_ENTRY(111, "������������ ������ � �������� ����� (-in)"),
		ERROR_ENTRY(112, "������ ��� �������� ����� ��������� (-log)"),
		ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
		ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118),
		ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY(120, "Lex: �������� ������ ������� ������"),
		ERROR_ENTRY(121, "Lex: ������� ������ �����������"),
		ERROR_ENTRY(122, "Lex: �������� ������ ������� ���������������"),
		ERROR_ENTRY(123, "Lex: ������� ��������������� �����������"),
		ERROR_ENTRY_NODEF(124), ERROR_ENTRY_NODEF(125), ERROR_ENTRY_NODEF(126),
		ERROR_ENTRY_NODEF(127),	ERROR_ENTRY_NODEF(128), ERROR_ENTRY_NODEF(129),
		ERROR_ENTRY(130, "Sem: ���������� ���������� ������� �� ���������."),
		ERROR_ENTRY(131, "Sem: ���� ���������� � ����������� ���������� ������� �� ���������."),
		ERROR_ENTRY(132,"Sem: ��� ������� � ��� ������������� �������� �� ���������."),
		ERROR_ENTRY(133, "Sem: ������������ ��� �������������� ��������"),
		ERROR_ENTRY(134,"Sem: �������� ��� ����������� ������ ����� �����������."),
		ERROR_ENTRY(135,"Sem: ����� ������� �� ������� ����������� ���������."),
		ERROR_ENTRY(136,"Sem: ������������ �������� �� ��������."),
		ERROR_ENTRY(137,"Sem: ������������� �� ����� ���� ����������� �� ��� ����������"),
		ERROR_ENTRY(138, "Sem: ����������� ������� �� ������� ����������."),
		ERROR_ENTRY_NODEF(139),
		ERROR_ENTRY_NODEF10(140),ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170),
		ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400),
		ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "Syn: �������� ��������� ���������"),
		ERROR_ENTRY(601, "Syn: ��������� ��������"),
		ERROR_ENTRY(602, "Syn: ������ � ���������"),
		ERROR_ENTRY(603, "Syn: ������ � ���������� �������"),
		ERROR_ENTRY(604, "Syn: ������ � ���������� ���������� �������"),
		ERROR_ENTRY(605, "������ ��������������� �������"),ERROR_ENTRY_NODEF(606),ERROR_ENTRY_NODEF(607),ERROR_ENTRY_NODEF(608),
		ERROR_ENTRY_NODEF(609),
		ERROR_ENTRY_NODEF100(700),
		ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900),
	};

	ERROR geterror(int id)
	{
		ERROR newError;
		if (id<0 || id>ERROR_MAX_ENTRY) newError.id = 0;
		else newError.id = id;
		for (int i = 0; i < ERROR_MAXSIZE_MESSAGE; i++)
			newError.message[i] = error[newError.id].message[i];

		return newError;
	}
	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		ERROR newError = geterror(id);
		newError.inext.col = col;
		newError.inext.line = line;
		return newError;
	}
};