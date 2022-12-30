#pragma once
#include "Greibach.h"
#define GRB_ERROR_SERIES 600
#define NS(n)	GRB::Rule::Chain::N(n)
#define TS(n)	GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)

namespace GRB
{
	Greibach greibach(
		NS('S'), TS('$'),                     // ��������� ������, ��� �����
		6,									  // ���������� ������
		Rule(
			NS('S'), GRB_ERROR_SERIES + 0,    // �������� ��������� ���������
			4,                                // 
			Rule::Chain(8, TS('h'), TS('{'), NS('N'), TS('r'), NS('E'), TS('~'), TS('}'), TS('~')),
			Rule::Chain(14, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS('~'), TS('}'), TS('~'), NS('S')),
			Rule::Chain(13, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS('~'), TS('}'), TS('~')),
			Rule::Chain(9, TS('h'), TS('{'), NS('N'), TS('r'), NS('E'), TS('~'), TS('}'), TS('~'), NS('S'))
		),
		Rule(
			NS('N'), GRB_ERROR_SERIES + 1,    // ����������� � ��������
			18,                               //
			Rule::Chain(4, TS('n'), TS('t'), TS('i'), TS('~')),
			Rule::Chain(5, TS('n'), TS('t'), TS('i'), TS('~'), NS('N')),
			Rule::Chain(6, TS('n'), TS('t'), TS('i'), TS('='), NS('E'), TS('~')),
			Rule::Chain(7, TS('n'), TS('t'), TS('i'), TS('='), NS('E'), TS('~'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS('~'), NS('N')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS('~')),
			Rule::Chain(8, TS('n'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('~')),
			Rule::Chain(9, TS('n'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('~'), NS('N')),
			Rule::Chain(4, TS('p'), TS('i'), TS('~'), NS('N')),
			Rule::Chain(4, TS('p'), TS('l'), TS('~'), NS('N')),
			Rule::Chain(4, TS('p'), NS('E'), TS('~'), NS('N')),
			Rule::Chain(3, TS('p'), NS('E'), TS('~')),
			Rule::Chain(3, TS('p'), TS('i'), TS('~')),
			Rule::Chain(3, TS('p'), TS('l'), TS('~')),
			Rule::Chain(3, TS('r'), NS('E'), TS('~')),
			Rule::Chain(4, TS('r'), NS('E'), TS('~'), NS('N')),
			Rule::Chain(11, TS('u'), TS('('), TS('i'), TS('='), NS('E'), TS('~'), TS(')'), TS('{'), NS('N'), TS('}'), TS('~')),
			Rule::Chain(12, TS('u'), TS('('), TS('i'), TS('='), NS('E'), TS('~'), TS(')'), TS('{'), NS('N'), TS('}'), TS('~'), NS('N'))
		),
		Rule(
			NS('E'), GRB_ERROR_SERIES + 2,    // ������ � ���������
			11,
			Rule::Chain(4, TS('i'), TS('('), NS('F'), TS(')')),
			Rule::Chain(5, TS('i'), TS('('), NS('F'), TS(')'), NS('M')),
			Rule::Chain(6, TS('i'), TS('('), NS('F'), TS(')'), NS('M'), NS('E')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))

		),
		Rule(
			NS('F'), GRB_ERROR_SERIES + 3,    // ������ � ���������� �������
			6,
			Rule::Chain(3, TS('i'), TS(','), NS('F')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(3, TS('l'), TS(','), NS('F')),
			Rule::Chain(2, TS('l')),
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(
			NS('W'), GRB_ERROR_SERIES + 4,    //������ � ���������� ���������� �������
			4,                                // 
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		),
		Rule(
			NS('M'), GRB_ERROR_SERIES + 5,    //��������
			2,								  //
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M'))
		)
	);
}


