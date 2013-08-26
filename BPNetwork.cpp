/********************************************************************
 * File			:	BPNetwork.cpp
 * Author		:	Tianjiao Mao
 * Date			:	2011.11.15
 * Description	:	This file implements the BPNetwork class which is
 *					defined in "BPNetwork.h".
 *******************************************************************/
 
#include "BPNetwork.h"
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;
 
BPNetwork::BPNetwork()
{
	m_inputUnits = NULL;
	m_hideUnits = NULL;
	m_outputUnits = NULL;
	m_hideUnitsIn = NULL;
	m_outputUnitsIn = NULL;
	m_deltaHide = NULL;
	m_deltaOut = NULL;
	m_target = NULL;
	m_theta = NULL;
	m_gamma = NULL;
	m_wInToHide = NULL;
	m_wHideToOut = NULL;
	m_pActiveFunc = NULL;
	m_pActiveFuncD = NULL;
	m_maxOut = 1.0;
	m_minOut = 0;
	m_speed = 0.1;
}
 
BPNetwork::~BPNetwork()
{
	/*�ͷ����ж�̬������ڴ�ռ�*/
	if (m_inputUnits != NULL)
		delete []m_inputUnits;
	if (m_hideUnitsIn != NULL)
		delete []m_hideUnitsIn;
	if (m_outputUnitsIn != NULL)
		delete []m_outputUnitsIn;
	if (m_hideUnits != NULL)
		delete []m_hideUnits;
	if (m_outputUnits != NULL)
		delete []m_outputUnits;
	if (m_target != NULL)
		delete []m_target;
	if (m_gamma != NULL)
		delete []m_gamma;
	if (m_theta != NULL)
		delete []m_theta;
	if (m_deltaHide != NULL)
		delete []m_deltaHide;
	if (m_deltaOut != NULL)
		delete []m_deltaOut;
	if (m_wInToHide != NULL)
	{
		for (int i = 0; i < m_nNodeIn; i++)
			delete []m_wInToHide[i];
		
		delete []m_wInToHide;
	}
	if (m_wHideToOut != NULL)
	{
		for (int i = 0; i < m_nNodeHide; i++)
			delete []m_wHideToOut[i];
		
		delete []m_wHideToOut;
	}
}
 
/**���һ��-1��1�������**/
double BPNetwork::GetRandom()
{
	return ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
}


/**�����������Ԫ��������ֵ����Сֵ
 * ����(min, max)ӦΪ�ڵ㼤�����ֵ��
 */
void BPNetwork::SetMaxMinOutput(double max, double min)
{
	m_maxOut = max;
	m_minOut = min;
}

/**���ýڵ㼤���**/
void BPNetwork::SetActiveFunc(Func f)
{
	m_pActiveFunc = f;
}

/**���ýڵ㼤����ĵ���**/
void BPNetwork::SetActiveFuncD(Func f)
{
	m_pActiveFuncD = f;
}


/**�����ʼ�����Ӿ���Ȩֵ
 * ���룺	W	Ȩֵ����ָ��
 * 			m	��������
 *			n	��������
 */
void BPNetwork::RandWeights(double **W, int m, int n)
{
	srand(time(NULL));
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			W[i][j] = GetRandom();
		}
	}
}

/**��(0,1)�����ֵ��ʼ��һά��ֵ����
 * ���룺	th	����ָ��
 * 			m	���鳤��
 */
void BPNetwork::RandThreshold(double *th, int m)
{
	srand(time(NULL));
	for (int i = 0; i < m; i++)
	{
		th[i] = (double)rand() / (double)RAND_MAX;
	}
}

/**����ѧϰ����**/
void BPNetwork::SetLearningSpeed(double speed)
{
	m_speed = speed;
}	
 
/**����BP����,Ϊ���鶯̬�����ڴ棬�������ʼ������Ȩֵ����
 * ����:	nIn		�����ڵ���
 *			nHide	���ز�ڵ���
 *			nOut	�����ڵ���
 */ 
bool BPNetwork::Create(int nIn, int nHide, int nOut)
{
	m_nNodeIn = nIn;
	m_nNodeHide = nHide;
	m_nNodeOut = nOut;

	m_inputUnits = new double[nIn];
	m_hideUnits = new double[nHide];
	m_outputUnits = new double[nOut];
	
	m_hideUnitsIn = new double[nHide];
	m_outputUnitsIn = new double[nOut];
	
	m_target = new double[nOut];

	m_deltaHide = new double[nHide];
	m_deltaOut = new double[nOut];
	
	m_theta = new double[nHide];
	m_gamma = new double[nOut];
	
	m_wInToHide = Alloc2DArray(nIn, nHide);
	m_wHideToOut = Alloc2DArray(nHide, nOut);
	
	/*��������������*/
	memset(m_inputUnits, 0, sizeof(double) * nIn);
	memset(m_hideUnitsIn, 0, sizeof(double) * nHide);
	memset(m_outputUnitsIn, 0, sizeof(double) * nOut);
	memset(m_hideUnits, 0, sizeof(double) * nHide);
	memset(m_outputUnits, 0, sizeof(double) * nOut);
	memset(m_target, 0, sizeof(double) * nOut);
	memset(m_deltaHide, 0, sizeof(double) * nHide);
	memset(m_deltaOut, 0, sizeof(double) * nOut);
	
	/*�����ʼ�����Ӿ���Ȩֵ*/
	RandWeights(m_wInToHide, nIn, nHide);
	RandWeights(m_wHideToOut, nHide, nOut);
	
	/*�����ʼ�����ز���������ֵ*/
	RandThreshold(m_theta, nHide);
	RandThreshold(m_gamma, nOut);
	
	return true;
}

/**�����ά˫��������
 * ���룺	m	��������
 *			n	��������
 */
double** BPNetwork::Alloc2DArray(int m, int n)
{
	double **pMatrix;
	pMatrix = new double*[m];
	
	for(int i = 0; i < m; i++)
	{
		pMatrix[i] = new double[n];
		memset(pMatrix[i], 0, sizeof(double) * n);
	}
	return pMatrix;
}


/**ǰ�����
 * ���룺	input	��������
 */
void BPNetwork::CalculateForward()
{
	memset(m_hideUnitsIn, 0, sizeof(double) * m_nNodeHide);
	memset(m_outputUnitsIn, 0, sizeof(double) * m_nNodeOut);
	
	/*�������ز���Ԫ�ļ���ֵ*/
	for (int i = 0; i < m_nNodeHide; i++)
	{
		for (int j = 0; j < m_nNodeIn; j++)
		{
			/*�����Ȩ����*/
			m_hideUnitsIn[i] += m_wInToHide[j][i] * m_inputUnits[j];
		}
		/*���㼤��ֵ*/
		m_hideUnitsIn[i] += m_theta[i];	//�������ز���ֵ
		m_hideUnits[i] = m_pActiveFunc(m_hideUnitsIn[i]); //���㼤��ֵ
	}
	
	
	/*��������㼤��ֵ*/
	for (int k = 0; k < m_nNodeOut; k++)
	{
		for (int i = 0; i < m_nNodeHide; i++)
		{
			m_outputUnitsIn[k] = m_wHideToOut[i][k] * m_hideUnits[i];
		}
		m_outputUnitsIn[k] += m_gamma[k];
		m_outputUnits[k] = m_pActiveFunc(m_outputUnitsIn[k]);
	}
}

/**�������������ز����**/
void BPNetwork::CalculateDelta()
{
	/*������������*/
	for (int i = 0; i < m_nNodeOut; i++)
	{
		m_deltaOut[i] = m_pActiveFuncD(m_outputUnitsIn[i])
						* (m_target[i] - m_outputUnits[i]);
	}
	
	/*�������ز����*/
	for (int j = 0; j < m_nNodeHide; j++)
	{
		m_deltaHide[j] = 0;
		for (int k = 0; k < m_nNodeOut; k++)
		{
			m_deltaHide[j] += m_wHideToOut[j][k] * m_deltaOut[k];
		}
		m_deltaHide[j] *= m_pActiveFuncD(m_hideUnitsIn[j]);	
	}
}

/**�������Ӿ���Ȩֵ**/
void BPNetwork::AdjustWeights()
{
	/*�������ز㵽��������Ӿ���Ȩֵ*/
	for (int i = 0; i < m_nNodeHide; i++)
	{
		for (int j = 0; j < m_nNodeOut; j++)
		{
			m_wHideToOut[i][j] += m_speed * m_hideUnits[i] * m_deltaOut[j];
		}
	}
	
	/*��������㵽���ز����Ӿ���Ȩֵ*/
	for (int k = 0; k < m_nNodeIn; k++)
	{
		for (int j = 0; j < m_nNodeHide; j++)
		{
			m_wInToHide[k][j] += m_speed * m_inputUnits[k] * m_deltaHide[j];
		}
	}
}

/**�������ز����������ֵ**/
void BPNetwork::AdjustThreshold()
{
	for (int i = 0; i < m_nNodeOut; i++)
		m_gamma[i] += m_speed * m_deltaOut[i];
		
	for (int j = 0; j < m_nNodeHide; j++)
		m_theta[j] += m_speed * m_deltaHide[j];
}

/**�жϷ������Ƿ���ȷ
 * ���룺	output	Ԥ��������
 */
bool BPNetwork::IsCorrect(double *output)
{
	int max = 0;		//������ڵ�����λ��
	
	/*�������������ҵ�������Ľڵ㣬��Ϊ��������*/
	for (int i = 1; i < m_nNodeOut; i++)
	{
		if (m_outputUnits[max] < m_outputUnits[i])
		{
			max = i;
		}
	}

	/*��׼���������ʽΪ(1,0,0������),(0,1,0������),(0,0,1������),
	���� '1' ����һά��Ϊ������*/
	if (output[max] == 1.0)
		return true;
	
	return false;
}

/**�趨Ŀ������
 * ���룺 pTarget	Ŀ������
 * ˵���������Ŀ��������ʽΪ(1,0,0��),(0,1,0��),(0,0,1��)����,
 *       ����ת����Ϊ(m_maxOut,m_minOut,m_minOut��),(m_minOut,m_maxOut,m_minOut��)����
 *       �Ա���õ�ѧϰ.
 */
void BPNetwork::SetTargetOutput(double *pTarget)
{
	int n;
	for (int i = 0; i < m_nNodeOut; i++)
	{
		if (pTarget[i] == 1.0)
			n = i;
	}
	for (int j = 0; j < m_nNodeOut; j++)
	{
		m_target[j] = m_minOut;
	}
	m_target[n] = m_maxOut;
}


/**ѵ��BP������
 * ���룺	input	���뼯
 * 			output	Ŀ�������
 *			size	ѵ�����Ĵ�С
 * ����ֵ��	count	��������
 */
int BPNetwork::Train(double **input, double **output, int size)
{
	bool isDone = false;
	int count = 0;	//��������
	while (!isDone)
	{
		isDone = true;
		count++;
		for (int i = 0; i < size; i++)
		{
			/*����������ֱ�Ӵ���������Ԫ*/
			memcpy(m_inputUnits, input[i], sizeof(double) * m_nNodeIn);
			/*�趨Ŀ������*/
			SetTargetOutput(output[i]);
			//memcpy(m_target, output[i], sizeof(double) * m_nNodeOut);
			
			
			/*����ǰ�����*/
			CalculateForward();
			
			/*�������������ȷ������ѵ��*/
			if (!IsCorrect(output[i]))
			{
				isDone = false;
				
				/*�������*/
				CalculateDelta();
				
				/*�������Ӿ���Ȩֵ*/
				AdjustWeights();
				
				/*�������ز����������ֵ*/
				AdjustThreshold();
			}	
		}
	}
	return count;
}


/**ʹ��ѵ���õ�������з���
 * ���룺input	��������
 * ����ֵ��	0	����Ϊsetosa
 *			1	����Ϊversicolor
 * 			2	����Ϊvirginica
 *			>2	����
 */
int BPNetwork::Classify(double *input)
{
	int kind = 0;
	
	/*����������ֱ�Ӵ���������Ԫ*/
	memcpy(m_inputUnits, input, sizeof(double) * m_nNodeIn);
	
	/*����ǰ�����*/
	CalculateForward();
	
	/*�������������ҵ�������Ľڵ㣬��Ϊ��������*/
	for (int i = 0; i < m_nNodeOut - 1; i++)
	{
		if (m_outputUnits[i] < m_outputUnits[i + 1])
		{
			kind = i + 1;
		}
	}
		
	return kind;
}