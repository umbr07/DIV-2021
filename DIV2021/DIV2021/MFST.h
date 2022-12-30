#pragma once
#pragma once
#include <stack>
#include "Greibach.h"
#include "MFST_trace.h"
#include "LT.h"
#include <iomanip>

class my_stack_SHORT :public std::stack<short>
{
public:
	using std::stack<short>::c; // ���� ��������
};


#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3
static int FST_TRACE_n = -1;
static char rbuf[205], sbuf[205], lbuf[1024];




typedef  my_stack_SHORT MFSTSTSTACK;
namespace MFST
{
	struct MfstState					//��������� �������� ��� ����������
	{
		short lenta_position;			//��������� �������� ��� ����������
		short nrule;					//����� �������� �������
		short nrulechain;				//����� ������� �������
		MFSTSTSTACK st;					//���� ��������
		MfstState();
		MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain);	//(������� �� �����;���� ��������; ����� ������� ������� �������� �������)
		MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain);//������� �� �����;���� ��������; ����� �������� �������; ����� ������� ������� �������� �������)

	};

	struct Mfst							//���������� �������
	{
		enum RC_STEP {					//��� �������� ������� step
			NS_OK,					//������� ������� � �������, ������� �������� � ����
			NS_NORULE,				//�� ������� ������� ����������(������ � ����������)
			NS_NORULECHAIN,			//�� ������� ���������� ������� ������(������ � ��o������ ����)
			NS_ERROR,				//����������� �������������� ������ ����������
			TS_OK,					//���. ������ �����==������� �����, ������������ �����, pop �����
			TS_NOK,					//���. ������ ����� != ������� �����, ������������� ��������� 
			LENTA_END,				//������� ������� ����� >= lenta_size
			SURPRISE,				//����������� ��� �������� (������ � step)
		};
		struct MfstDiagnosis		//�����������
		{
			short lenta_position;		//������� �� �����
			RC_STEP rc_step;			//��� ���������� ����
			short nrule;				//����� �������
			short nrule_chain;			//����� ������� �������
			MfstDiagnosis();			//==
			MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain);//(������� �� �����, ��� ���������� ����, ����� �������, ����� ������� �������)

		}diagnosis[MFST_DIAGN_NUMBER]; 		// ��������� ����� �������� ���������

		class my_stack_MfstState :public std::stack<MfstState>
		{
		public:
			using std::stack<MfstState>::c;
		};

		GRBALPHABET* lenta;					//���������������� (TN/NS) ����� (�� LEX)
		short lenta_position;				//������� ������� �� �����
		short nrule;						//����� �������� �������
		short nrulechain;					//����� ������� �������, �������� �������
		short lenta_size;					//������ �����
		GRB::Greibach grebach;				//���������� �������
		LT::LexTable lex;					//��������� ������ ������������ �����������
		MFSTSTSTACK st;						//���� ��������
		my_stack_MfstState storestate;		//���� ��� ���������� ���������
		Mfst();
		Mfst(LT::LexTable& plex, GRB::Greibach pgrebach);		//(��������� ������ ����. �����������, ���������� �������)
		char* getCSt(char* buf);								//�������� ���������� �����
		char* getCLenta(char* buf, short pos, short n = 25);	//�����: n �������� � pos
		char* getDiagnosis(short n, char* buf);					//�������� n-� ������ ����������� ��� 0�00
		bool savestate(Log::LOG& log);										//��������� ��������� ��������
		bool resetstate(Log::LOG& log);										//������������ ��������� ��������
		bool push_chain(GRB::Rule::Chain chain);				//��������� ������� ������� � ���� (������� �������)
		RC_STEP step(Log::LOG& log);											//��������� ��� ��������
		bool start(Log::LOG& log);											//��������� �������
		bool savediagnosis(RC_STEP pprc_step);					//(��� ���������� ����)
		void printrules(Log::LOG& log);										//����� ������������������ ������

		struct Deducation
		{
			short size;
			short* nrules;
			short* nrulechains;
			Deducation()
			{
				size = 0;
				nrules = 0;
				nrulechains = 0;
			};
		}deducation;

		bool savededucation();
	};
}

