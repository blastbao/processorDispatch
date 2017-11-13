#include<iostream>
#include<fstream>
#include<string>
using namespace std;
#define COUNT 4

struct process
{
	int num;//进程编号
	double submitTime;//提交时间
	double runTime;//估计运行时间
	bool submit = false;//判断是否提交处理机
	bool pro = false;//判断是否已执行
	double resRadio = 1;//响应比
};

void FCFS(process *pr);//先来先服务算法
void SJF(process *pr);//短进程优先算法
void HRRN(process *pr);//高响应比优先算法
double cyclingT(double submitTime, double endTime);
double weighedCyclingT(double submitTime, double startTime, double endTime);

int main()
{
	cout << "***********计算机操作系统——处理机调度实验***********" << endl;

	ifstream in;
	string filename = "data.txt";
	in.open(filename);
	if (!in)
	{
		cerr << "打开文件出错。" << endl;
		return 1;
	}

	string s;//用于保存读入的数据
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

	cout << "进程编号" << ' ' << "提交时间" << ' ' << "估计运行时间" << endl;
	for (int i = 0; i < COUNT; i++)
	{
		cout << pr[i].num << '\t' << pr[i].submitTime << '\t' << pr[i].runTime << endl;
	}

	while (true)
	{
		cout << "1.先来先服务算法" << endl << "2.短进程优先算法" << endl << "3.最高响应比优先算法" << endl << "4.程序退出" << endl;
        cout << "选择进程调度算法：" << endl;

		int choice = 4;
		cin >> choice;
		switch (choice)
		{
		case 1:FCFS(pr); cout << "****************************" << endl; break;
		case 2:SJF(pr); cout << "****************************"<<endl; break;
		case 3:HRRN(pr); cout << "****************************" << endl; break;
		case 4:exit(0);
		default:cout << "输入有误，请重新输入。"; break;
		}
	}
	return 0;
}

void FCFS(process *pr)
{
	for (int i = 0; i < COUNT-1; i++)
	{//首先按照提交时间恢复
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

	double startTime;//运行时间
	double endTime;//结束时间
	double sumCyclingTime=0;//总周转时间
	double sumWeighedCyclingTime=0;//总带权周转时间
	double time=pr[0].submitTime;//记录现在时间

	cout << "进程调度顺序：";
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

		cout << pr[i].num << '\t' << "开始时间：" << startTime << '\t' << "完成时间:" << endTime << '\t' << "周转时间:" << cyclingT(pr[i].submitTime, endTime) << '\t' << "带权周转时间：" << weighedCyclingT(pr[i].submitTime, startTime, endTime) << endl;
		time = endTime;
	}

	cout << "平均周转时间=" << sumCyclingTime / COUNT<<endl;
	cout << "平均带权周转时间=" << sumWeighedCyclingTime / COUNT << endl;
 }

void SJF(process *pr)
{
	for (int i = 0; i < COUNT - 1; i++)
	{//按照提交时间恢复
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
	{//重新初始化process，否则会导致不能多次使用算法
		pr[i].pro = false;
		pr[i].resRadio = 1;
		pr[i].submit = false;
	}

	double startTime;//运行时间
	double endTime;//结束时间
	double sumCyclingTime = 0;//总周转时间
	double sumWeighedCyclingTime = 0;//总带权周转时间
	double time = pr[0].submitTime;//记录现在时间
	int queue[COUNT];//保存优先级序列
	int next;
    pr[0].submit = true;

	process *pr0 = pr;
	for (int i = 0; i < COUNT-1; i++)
	{//先按照长短计算进程的优先级序列，保存在数组里
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
	{//按照提交时间恢复pr
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
		//判断调度哪个进程
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

		cout << pr[next].num << '\t' << "开始时间：" << startTime << '\t' << "完成时间:" << endTime << '\t' << "周转时间:" << cyclingT(pr[next].submitTime, endTime) << '\t' << "带权周转时间：" << weighedCyclingT(pr[next].submitTime, startTime, endTime) << endl;
		time = endTime;
		pr[next].pro = true;
	}
	cout << "平均周转时间=" << sumCyclingTime / COUNT << endl;
	cout << "平均带权周转时间=" << sumWeighedCyclingTime / COUNT << endl;
}

void HRRN(process *pr)
{
	for (int i = 0; i < COUNT - 1; i++)
	{//按照提交时间恢复
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
	{//重新初始化process，否则会导致不能多次使用算法
		pr[i].pro = false;
		pr[i].resRadio = 1;
		pr[i].submit = false;
	}

	double startTime;//运行时间
	double endTime;//结束时间
	double sumCyclingTime = 0;//总周转时间
	double sumWeighedCyclingTime = 0;//总带权周转时间
	double time = pr[0].submitTime;//记录现在时间

	for (int i = 0; i < COUNT; i++)
	{
		for (int j = 0; j < COUNT; j++)
		{//判断是否已提交
			if (pr[j].submitTime <= time)
			{
				pr[j].submit = true;
				pr[j].resRadio = (time - pr[j].submitTime + pr[j].runTime) / pr[j].runTime;//计算响应比
			}
		}

		for (int j = 0; j < COUNT-1; j++)
		{//根据响应比优先排序
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

				cout << pr[j].num << '\t' << "开始时间：" << startTime << '\t' << "完成时间:" << endTime << '\t' << "周转时间:" << cyclingT(pr[j].submitTime, endTime) << '\t' << "带权周转时间：" << weighedCyclingT(pr[j].submitTime, startTime, endTime) << endl;
				time = endTime;
				pr[j].pro = true;
				break;
			}
		}
	}

	cout << "平均周转时间=" << sumCyclingTime / COUNT << endl;
	cout << "平均带权周转时间=" << sumWeighedCyclingTime / COUNT << endl;
}

double cyclingT(double submitTime, double endTime)
{
	return endTime - submitTime;
}

double weighedCyclingT(double submitTime, double startTime, double endTime)
{
	return (endTime - submitTime) / (endTime - startTime);//周转时间/服务时间
}

double responseRadio(double waitT, double runT)
{
	return (waitT + runT) / runT;
}