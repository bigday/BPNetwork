/*********************************************************************
 * File			:	BPNetwork.h
 * Author		:	TianJiao Mao
 * Date			:	2011.11.14
 * Description	:	This file defines a class that represent a 3-layer 
 *					BP Network which can be used to deal with classify 
 *					problems.
 ********************************************************************/

#ifndef _BPNETWORK_H_
#define _BPNETWORK_H_


/**
 *	BP�������࣬��ʾһ������BP����
 */
class BPNetwork
{
	typedef double (*Func)(double x);	//����һ������ָ������
public:
	BPNetwork();
	~BPNetwork();
	
	/**����BP������**/
	bool Create(int nIn, int nHide, int nOut);	
	
	/**����ѧϰ����**/
	void SetLearningSpeed(double speed);
	
	/**���ýڵ㼤���**/
	void SetActiveFunc(Func f);
	
	/**���ýڵ㼤����ĵ���**/
	void SetActiveFuncD(Func f);
	
	/**�����������Ԫ��������ֵ����Сֵ(�������ֵ��)**/
	void SetMaxMinOutput(double max, double min);
	
	/**ѵ��BP������**/
	int Train(double **input, double **output, int size);
	
	/**ʹ��ѵ���õ�������з���**/
	int Classify(double *input);
	
	/**�����ά˫��������**/
	static double **Alloc2DArray(int m, int n);
	
private:
	/**���һ��-1��1�������**/
	double GetRandom();		
	
	/**�趨Ŀ������**/
	void SetTargetOutput(double *pTarget);
	
	/**�����ʼ�����Ӿ���Ȩֵ**/
	void RandWeights(double **W, int m, int n);
	
	/**�����ʼ��һά��ֵ����**/
	void RandThreshold(double *th, int m);
	
	/**ǰ�����**/
	void CalculateForward();
	
	/**�������������ز����**/
	void CalculateDelta();
	
	/**�������Ӿ���Ȩֵ**/
	void AdjustWeights();
	
	/**�������ز����������ֵ**/
	void AdjustThreshold();
	
	/**�жϷ������Ƿ���ȷ**/
	bool IsCorrect(double *output);

private:
	int m_nNodeIn;			//�����ڵ���Ŀ
	int m_nNodeHide;		//���ز�ڵ���Ŀ
	int m_nNodeOut;			//�����ڵ���Ŀ
	double *m_theta;		//���ڵ����ֵ����
	double *m_gamma;		//����ڵ����ֵ����
	double m_speed;			//ѧϰ����
	double m_maxOut;		//�����Ԫ��������ֵ
	double m_minOut	;		//�����Ԫ�������Сֵ

	double *m_inputUnits;	//�������Ԫ��ֵ
	double *m_hideUnits;	//���ز���Ԫ�����ֵ
	double *m_outputUnits;	//�������Ԫ�����ֵ
	double *m_hideUnitsIn;	//���ز���Ԫ������ֵ
	double *m_outputUnitsIn;//�������Ԫ������ֵ
	
	double *m_target;		//Ŀ������
	
	double *m_deltaHide;	//���ز����
	double *m_deltaOut;		//��������
	
	double **m_wInToHide;	//����㵽���ز������Ȩֵ����
	double **m_wHideToOut;	//���ز㵽����������Ȩֵ����
			
	Func m_pActiveFunc;		//�����
	Func m_pActiveFuncD;	//������ĵ���

};
#endif //#ifndef _BPNETWORK_H_