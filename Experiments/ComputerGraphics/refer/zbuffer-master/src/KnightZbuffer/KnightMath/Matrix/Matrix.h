//Matrix.h
#ifndef MATRIX
#define MATRIX

//#define debug_mode

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#define MATRIX_MIN 1e-10														//�趨���㾫��

class Matrix
{
public:
	//ר������ģ���ࣺvector�����������࣬Ŀ�������ƹ��ܵ��ú��ṩ��ȫ�±����
	template <class T>
	class m_vector;
	
	//���캯��:����վ���
	Matrix(int row=0,int col=0,double value=0.0);

	//���캯��:ʹ���������鹹�����
	Matrix(const double* first,const double* last,int row,int col=1);

	//���캯��:ʹ�õ�������ǵ��������乹�����
	template <class Iter>//(Iter�趨��*Iter��++Iter����)						//̫���ˣ�����߻�����дģ�庯��
	Matrix(const Iter first,const Iter last,int row,int col=1);

	//�������þ�������
	void SetRow(int row,double value=0.0);

	//�������þ�������
	void SetCol(int col,double value=0.0);
	
	//�жϾ������ ���ؽ��
	bool operator == (const Matrix& s) const;

	//�жϾ��󲻵� ���ؽ��
	bool operator != (const Matrix& s) const{return !(*this==s);}
	
	//������� ���غ;���
	Matrix operator + (const Matrix& s) const;

	//������� ���ؾ�������
	Matrix& operator += (const Matrix& s);

	//������� ���ز����
	Matrix operator - (const Matrix& s) const;

	//������� ���ؾ�������
	Matrix& operator -= (const Matrix& s);

	//�����볣����� ���س˻�����
	Matrix operator * (double time) const;

	//�����볣����� ���ؾ�������
	Matrix& operator *= (double time);
	
	//�����볣����� ���س˻�����
	friend Matrix operator * (double time,const Matrix& s){return s*time;}

	//������������ ���س˻�����
	Matrix operator * (const Matrix& s) const;

	//������������ ���ؾ�������
	Matrix& operator *= (const Matrix& s);

	//�����ת�� ����ԭ�����ת�þ���
	Matrix Transpose() const; 

	//������� ���ط���������
	Matrix Converse() const;
	
	//������� ���ط���İ������
	Matrix Adjoint() const;
	
	//��ָ��a(i,j)��������ʽ ����double�ͽ��
	double Cofactor(int i,int j) const;//(i,j��1��ʼ)
	
	//������ʽ ���ط��������ʽֵ 
	double Determinate() const;
	
	//���� ���ؾ������
	int Rank() const;
	
	//��λ���� ���ط����Ӧ�ĵ�λ����
	Matrix Unit() const;
	
	//���ȱ任����time���Ծ���ĳһ�У��У�(time!=0) ���ر任��ľ�����
	Matrix& Time(int i,double time=1.0,bool isRow=true);//(i��1��ʼ)
	
	//���ȱ任���������У��У� ���ر任��ľ�����
	Matrix& Exchange(int i,int j,bool isRow=true);//(i,j��1��ʼ)
	
	//���ȱ任������i�У��У���time���ӵ���j�У��У���i!=j�� ���ر任��ľ�����
	Matrix& Add(int i,int j,double time=1.0,bool isRow=true);//(i,j��1��ʼ)

	//���������γɵ�������� ���ظ��������
	Matrix operator | (const Matrix& B) const;

	//ʹ�ô�1��ʼ���кź��кŷ��ʾ���Ԫ�� ���ض�ӦԪ������
	double& operator () (int row,int col=1);

	//ʹ�ô�1��ʼ���кź��кŷ��ʾ���Ԫ�� ���ض�ӦԪ�س�����
	const double& operator () (int row,int col=1) const;

	//ʹ�ô�0��ʼ���кź��кŷ��ʾ���Ԫ�� ���ض�ӦԪ�ص�����
	Matrix::m_vector<double>& operator [] (int rowIndex);//���Ƕ���ֹ�˶�,a[i][j]��jԽ���Ǽ�鲻����.
	/*solved:����̳У������̳У����ײ�vector��������
	���нӿ�ֻ�ṩ�±���ʣ���ȫ�����غ�������
	�����ӿ�����Ԫ�����ԭvector�ӿڲ���Ӱ��*/

	//ʹ�ô�0��ʼ���кź��кŷ��ʾ���Ԫ�� ���ض�ӦԪ�صĳ�����
	const Matrix::m_vector<double>& operator [] (int rowIndex) const;

	//��˹��Ԫ���ⷽ�� ���ط��̽�(������)(���һ��Ϊ�ؽ⣬������Ϊ��ν⣬�վ����ʾ�޽�)
	friend Matrix Gause(const Matrix& A,const Matrix& B);

	//��������� ���ؾ�������
	int Row() const {return m_row;}

	//��������� ���ؾ�������
	int Col() const {return m_col;}
	
#ifdef debug_mode
	//�������ӡ��������Ϊ��Ԫ���������ڵ���ʱ�鿴״̬
	friend ostream& operator << (ostream& out,const Matrix& s);
#endif //debug_mode
protected:
    vector<int> m_gause();//������Ա����:�����б任������ϵ��Ϊ1�Ľ��ݾ���
	vector<m_vector<double> > m_c;//������Ա:�������ݴ洢��
	int m_row;//������Ա:����
	int m_col;//������Ա:����
};

template <class T>
class Matrix::m_vector:protected vector<T>
{//����һ���ֻ࣬Ϊ�±갲ȫ���ʺͶ��������ӿڽ���
//tbc:���ǹ��캯���Ϳ������캯�����޷��������
public:
	m_vector(int dim=0,const T& value=T()):vector<T>(dim,value){}
	T& operator [] (int index)
	{
		if (index>=(int)vector<T>::size())//Pay attention!��int��unsigned int��Ƚ�ʱ�����intǿ��ת����unsigned int
		    throw overflow_error("m_vector::operator [] ():overflow error");
		else if (index<0)
			throw underflow_error("m_vector::operator [] ():underflow error");
		return vector<T>::operator [] (index);
	}
	const T& operator [] (int index) const /*{return (const T&)(*this)[index];}//����*/
	{
		if (index>=(int)vector<T>::size())
		    throw overflow_error("m_vector::operator [] ():overflow error");
		else if (index<0)
			throw underflow_error("m_vector::operator [] ():underflow error");
		return vector<T>::operator [] (index);
	}
	friend class Matrix;
};

template <class Iter>
Matrix::Matrix(const Iter first,const Iter last,int row,int col)
{
	if (row<1 || col<1)
		throw exception("Matrix::Matrix(Iter,Iter,int,int):illegal input");

	//��ʼ��
	m_row = row;												//���г�ʼ��
	m_col = col;
	m_c.resize(row);											//�ռ��ʼ��
	for (int i=0; i<row; i++)
		m_c[i].resize(col,0.0);
		
	//Ϊ����ֵ
	Iter iter=first;
	for (int i=0; i<row; i++)
	{
		for (int j=0; j<col; j++)
		{
			if (iter!=last)
			{
   			    if (fabs((double)(*iter))>=MATRIX_MIN)					//Pay attention!
					m_c[i][j] = *iter;
			    else
			    	m_c[i][j] = 0.0;
				++iter;
			}
			else
				throw exception("Matrix::Matrix(Iter,Iter,int,int):arguements not match");
		}
	}
	if (iter!=last)
		throw exception("Matrix::Matrix(Iter,Iter,int,int):arguements not match");
}

#endif //MATRIX
