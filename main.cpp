#include "BPNetwork.h"
#include <memory.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>
using namespace std;


/*ѵ�������������ݣ���75�飬ÿ��4���������룩*/
double **trainInput;
double **trainOutput;
	
/*����������������*/
double testInput[75][4];
int testOutput[75];		//�������ݵ�ʵ�����
int testTarget[75];		//�������ݵ�Ŀ�����

/**����������Ӧ�ĵ�����**/
double ActiveFunc1(double x)		//s�ͺ���
{
	return 1.0 / (1.0 + exp(-x));
}
double ActiveFuncD1(double x)
{
	return exp(-x) / pow(1.0 + exp(-x), 2);
}
double ActiveFunc2(double x)		//���Һ���
{
	return sin(x);
}
double ActiveFuncD2(double x)
{
	return cos(x);
}
double ActiveFunc3(double x)		//˫�����к���
{
	return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}
double ActiveFuncD3(double x)
{
	return 4.0 / pow(exp(x) + exp(-x), 2);
}
double ActiveFunc4(double x)		//�����к���
{
	return atan(x);
}
double ActiveFuncD4(double x)
{
	return 1.0 / (1.0 + x * x);
}
////////////////////////////////////////////////////////////////////


/**��ȡѵ������**/
bool readTrainData(char *filePath)
{
	if (freopen(filePath, "r", stdin) == NULL)
		return false;
	string strKind;
	for (int i = 0; i < 75; i++)
	{

		/*��ȡ��������*/
		for (int j = 0; j < 4; j++)
		{
			cin>>trainInput[i][j];
		}
		
		/*��ȡ�������(����iris�������ж����������Ϊ
		(1, 0, 0), (0, 1, 0), (0, 0, 1)����֮һ)*/
		cin>>strKind;
		if (strKind == "setosa")
		{
			trainOutput[i][0] = 1.0;
		}
		else if(strKind == "versicolor")
		{
			trainOutput[i][1] = 1.0;
		}
		else if (strKind == "virginica")
		{
			trainOutput[i][2] = 1.0;
		}
	}
	fclose(stdin);
	
	return true;
}

/**��ȡ��������**/
bool readTestData(char *filePath)
{
	if (freopen(filePath, "r", stdin) == NULL)
		return false;
	string strKind;
	for (int k = 0; k < 75; k++)
	{
		for (int i = 0; i < 4; i++)
			cin>>testInput[k][i];
		
		cin>>strKind;
		if (strKind == "setosa")
			testTarget[k] = 0;
		else if(strKind == "versicolor")
			testTarget[k] = 1;
		else if (strKind == "virginica")
			testTarget[k] = 2;
		
	}
	fclose(stdin);
	
	return true;
}

/**д����������**/
bool writeOutput(char *filePath)
{
	int nError = 0;
	cout<<"������:"<<endl;
	for (int j = 0; j < 75; j++)
	{
		if (testOutput[j] != testTarget[j])
			nError++;
		
		switch (testOutput[j])
		{
		case 0:
			cout<<"setosa"<<endl;
			break;
		case 1:
			cout<<"versicolor"<<endl;
			break;
		case 2:
			cout<<"virginica"<<endl;
			break;
		}
	}
	cout<<"\n\nʶ�����: "<<nError<<"��"<<endl;
	
	return true;
}

int main(int argc, char **argv)
{
	int nHide;		//���ڵ����
	double speed;	//ѧϰ����
	int nFunc;		//���������
	
	trainInput = BPNetwork::Alloc2DArray(75, 4);
	trainOutput = BPNetwork::Alloc2DArray(75, 3);;
	
	cout<<"��ѡѡ�񼤻��:"<<endl;
	cout<<"1. s�ͺ���  2. ���Һ���  3. ˫�����к���  4. �����к���"<<endl;
	cin>>nFunc;
	cout<<"���������ڵ������";
	cin>>nHide;
	cout<<"\n������ѧϰ����(0~1.0):";
	cin>>speed;

	/*��ȡѵ������*/
	if (!readTrainData("train.dat"))
	{
		cout<<"��ȡѵ������ʧ�ܣ�"<<endl;
		return -1;
	}
	
	/*��ȡ��������*/
	if (!readTestData("test.dat"))
	{
		cout<<"��ȡ��������ʧ��!"<<endl;
		return -1;
	}
	
	/*����������*/
	BPNetwork bpn;
	bpn.Create(4, nHide, 3);
	bpn.SetLearningSpeed(speed);
	switch (nFunc)	//ѡ�񼤻��
	{
	case 1:
		bpn.SetActiveFunc(ActiveFunc1);
		bpn.SetActiveFuncD(ActiveFuncD1);
		bpn.SetMaxMinOutput(1.0, 0.0);
		break;
	case 2:
		bpn.SetActiveFunc(ActiveFunc2);
		bpn.SetActiveFuncD(ActiveFuncD2);
		bpn.SetMaxMinOutput(1.0, -1.0);
		break;
	case 3:
		bpn.SetActiveFunc(ActiveFunc3);
		bpn.SetActiveFuncD(ActiveFuncD3);
		bpn.SetMaxMinOutput(1.0, -1.0);
		break;
	case 4:
		bpn.SetActiveFunc(ActiveFunc4);
		bpn.SetActiveFuncD(ActiveFuncD4);
		bpn.SetMaxMinOutput(1.57, -1.57);
		break;
	default:
		break;
	}
	
	/*ѵ��BP������*/
	cout<<"����ѧϰ�У����Ժ�..."<<endl;
	int count = bpn.Train(trainInput, trainOutput, 75);
	
	/*����BP������*/
	for (int k = 0; k < 75; k++)
	{
		testOutput[k] = bpn.Classify(testInput[k]);
	}
	writeOutput("testclassify.dat");
	cout<<"��������: "<<count<<endl;
	
	return 0;
}








