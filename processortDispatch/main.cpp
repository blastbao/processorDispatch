#include<iostream>
#include<fstream>
#include<string>
using namespace std;
#define COUNT 4

struct process
{
	int num;//���̱��
	double submitTime;//�ύʱ��
	double runTime;//��������ʱ��
	bool submit = false;//�ж��Ƿ��ύ�����
	bool pro = false;//�ж��Ƿ���ִ��
	double resRadio = 1;//��Ӧ��
};

void FCFS(process *pr);//�����ȷ����㷨
void SJF(process *pr);//�̽��������㷨
void HRRN(process *pr);//����Ӧ�������㷨
double cyclingT(double submitTime, double endTime);
double weighedCyclingT(double submitTime, double startTime, double endTime);

int main()
{
	cout << "***********���������ϵͳ�������������ʵ��***********" << endl;

	ifstream in;
	string filename = "data.txt";
	in.open(filename);
	if (!in)
	{
		cerr << "���ļ�����" << endl;
		return 1;
	}

	string s;//���ڱ�����������
	process pr[5];
	
	in.seekg(0,ios::beg);
	for (int i = 0; i < COUNT; i++)
	{
		getline(in, s, '\t');
		pr[i].num = stoi(s);
		getline(in, s, '\t');
		pr[i].submitTime = stod(s);
		getline(in, s, '\n');
		pr[i].runTime = stod(s);
	}
	in.close();

	cout << "���̱��" << ' ' << "�ύʱ��" << ' ' << "��������ʱ��" << endl;
	for (int i = 0; i < COUNT; i++)
	{
		cout << pr[i].num << '\t' << pr[i].submitTime << '\t' << pr[i].runTime << endl;
	}

	while (true)
	{
		cout << "1.�����ȷ����㷨" << endl << "2.�̽��������㷨" << endl << "3.�����Ӧ�������㷨" << endl << "4.�����˳�" << endl;
        cout << "ѡ����̵����㷨��" << endl;

		int choice = 4;
		cin >> choice;
		switch (choice)
		{
		case 1:FCFS(pr); cout << "****************************" << endl; break;
		case 2:SJF(pr); cout << "****************************"<<endl; break;
		case 3:HRRN(pr); cout << "****************************" << endl; break;
		case 4:exit(0);
		default:cout << "�����������������롣"; break;
		}
	}
	return 0;
}

void FCFS(process *pr)
{
	for (int i = 0; i < COUNT-1; i++)
	{//���Ȱ����ύʱ��ָ�
		for (int j = i + 1; j < COUNT; j++)
		{
             if (pr[i].submitTime > pr[j].submitTime)
		      {
			     process temp;
			     temp = pr[j];
			     pr[j] = pr[i];
			     pr[i] = temp;
		       }
		}
	}

	double startTime;//����ʱ��
	double endTime;//����ʱ��
	double sumCyclingTime=0;//����תʱ��
	double sumWeighedCyclingTime=0;//�ܴ�Ȩ��תʱ��
	double time=pr[0].submitTime;//��¼����ʱ��

	cout << "���̵���˳��";
	for (int i = 0; i < COUNT; i++)
	{
		if(i!=COUNT-1)
		cout << pr[i].num << "->";
		else cout << pr[i].num<<endl;
	}
	for (int i = 0; i < COUNT; i++)
	{
		startTime = time;
		endTime = startTime + pr[i].runTime;

		sumCyclingTime += cyclingT(pr[i].submitTime, endTime);
		sumWeighedCyclingTime += weighedCyclingT(pr[i].submitTime, startTime, endTime);

		cout << pr[i].num << '\t' << "��ʼʱ�䣺" << startTime << '\t' << "���ʱ��:" << endTime << '\t' << "��תʱ��:" << cyclingT(pr[i].submitTime, endTime) << '\t' << "��Ȩ��תʱ�䣺" << weighedCyclingT(pr[i].submitTime, startTime, endTime) << endl;
		time = endTime;
	}

	cout << "ƽ����תʱ��=" << sumCyclingTime / COUNT<<endl;
	cout << "ƽ����Ȩ��תʱ��=" << sumWeighedCyclingTime / COUNT << endl;
 }

void SJF(process *pr)
{
	for (int i = 0; i < COUNT - 1; i++)
	{//�����ύʱ��ָ�
		for (int j = i + 1; j < COUNT; j++)
		{
			if (pr[i].submitTime > pr[j].submitTime)
			{
				process temp;
				temp = pr[j];
				pr[j] = pr[i];
				pr[i] = temp;
			}
		}
	}

	for (int i = 0; i < COUNT; i++)
	{//���³�ʼ��process������ᵼ�²��ܶ��ʹ���㷨
		pr[i].pro = false;
		pr[i].resRadio = 1;
		pr[i].submit = false;
	}

	double startTime;//����ʱ��
	double endTime;//����ʱ��
	double sumCyclingTime = 0;//����תʱ��
	double sumWeighedCyclingTime = 0;//�ܴ�Ȩ��תʱ��
	double time = pr[0].submitTime;//��¼����ʱ��
	int queue[COUNT];//�������ȼ�����
	int next;
    pr[0].submit = true;

	process *pr0 = pr;
	for (int i = 0; i < COUNT-1; i++)
	{//�Ȱ��ճ��̼�����̵����ȼ����У�������������
		for (int j = i + 1; j < COUNT; j++)
		{
			if (pr0[i].runTime > pr0[j].runTime)
			{
				process temp;
				temp = pr0[j];
				pr0[j] = pr0[i];
				pr0[i] = temp;
			}
		}
	}
	
	for (int i = 0; i < COUNT; i++)
	{
		queue[i] = pr0[i].num-1;
	}

	for (int i = 0; i < COUNT - 1; i++)
	{//�����ύʱ��ָ�pr
		for (int j = i + 1; j < COUNT; j++)
		{
			if (pr[i].submitTime > pr[j].submitTime)
			{
				process temp;
				temp = pr[j];
				pr[j] = pr[i];
				pr[i] = temp;
			}
		}
	}

	for (int i = 0; i < COUNT; i++)
	{
		//�жϵ����ĸ�����
		for (int j = 0; j < COUNT; j++)
		{
			if (pr[j].submitTime <= time)
			{
				pr[j].submit = true;
			}
		}

		for (int k = 0; k < COUNT; k++)
		{
			if (pr[queue[k]].submit == true && pr[queue[k]].pro == false)
			{
				next = queue[k];
				break;
			}
		}

		startTime = time;
		endTime = time + pr[next].runTime;

		sumCyclingTime += cyclingT(pr[next].submitTime, endTime);
		sumWeighedCyclingTime += weighedCyclingT(pr[next].submitTime, startTime, endTime);

		cout << pr[next].num << '\t' << "��ʼʱ�䣺" << startTime << '\t' << "���ʱ��:" << endTime << '\t' << "��תʱ��:" << cyclingT(pr[next].submitTime, endTime) << '\t' << "��Ȩ��תʱ�䣺" << weighedCyclingT(pr[next].submitTime, startTime, endTime) << endl;
		time = endTime;
		pr[next].pro = true;
	}
	cout << "ƽ����תʱ��=" << sumCyclingTime / COUNT << endl;
	cout << "ƽ����Ȩ��תʱ��=" << sumWeighedCyclingTime / COUNT << endl;
}

void HRRN(process *pr)
{
	for (int i = 0; i < COUNT - 1; i++)
	{//�����ύʱ��ָ�
		for (int j = i + 1; j < COUNT; j++)
		{
			if (pr[i].submitTime > pr[j].submitTime)
			{
				process temp;
				temp = pr[j];
				pr[j] = pr[i];
				pr[i] = temp;
			}
		}
	}
	for (int i = 0; i < COUNT; i++)
	{//���³�ʼ��process������ᵼ�²��ܶ��ʹ���㷨
		pr[i].pro = false;
		pr[i].resRadio = 1;
		pr[i].submit = false;
	}

	double startTime;//����ʱ��
	double endTime;//����ʱ��
	double sumCyclingTime = 0;//����תʱ��
	double sumWeighedCyclingTime = 0;//�ܴ�Ȩ��תʱ��
	double time = pr[0].submitTime;//��¼����ʱ��

	for (int i = 0; i < COUNT; i++)
	{
		for (int j = 0; j < COUNT; j++)
		{//�ж��Ƿ����ύ
			if (pr[j].submitTime <= time)
			{
				pr[j].submit = true;
				pr[j].resRadio = (time - pr[j].submitTime + pr[j].runTime) / pr[j].runTime;//������Ӧ��
			}
		}

		for (int j = 0; j < COUNT-1; j++)
		{//������Ӧ����������
			for (int k = j+1; k < COUNT; k++)
			{
				if (pr[j].resRadio < pr[k].resRadio)
				{
					process temp;
					temp = pr[k];
					pr[k] = pr[j];
					pr[j] = temp;
				}
			}
		}

		for (int j = 0; j < COUNT; j++)
		{
			if (pr[j].submit == true && pr[j].pro == false)
			{
				startTime = time;
				endTime = time + pr[j].runTime;

				sumCyclingTime += cyclingT(pr[j].submitTime, endTime);
				sumWeighedCyclingTime += weighedCyclingT(pr[j].submitTime, startTime, endTime);

				cout << pr[j].num << '\t' << "��ʼʱ�䣺" << startTime << '\t' << "���ʱ��:" << endTime << '\t' << "��תʱ��:" << cyclingT(pr[j].submitTime, endTime) << '\t' << "��Ȩ��תʱ�䣺" << weighedCyclingT(pr[j].submitTime, startTime, endTime) << endl;
				time = endTime;
				pr[j].pro = true;
				break;
			}
		}
	}

	cout << "ƽ����תʱ��=" << sumCyclingTime / COUNT << endl;
	cout << "ƽ����Ȩ��תʱ��=" << sumWeighedCyclingTime / COUNT << endl;
}

double cyclingT(double submitTime, double endTime)
{
	return endTime - submitTime;
}

double weighedCyclingT(double submitTime, double startTime, double endTime)
{
	return (endTime - submitTime) / (endTime - startTime);//��תʱ��/����ʱ��
}

double responseRadio(double waitT, double runT)
{
	return (waitT + runT) / runT;
}