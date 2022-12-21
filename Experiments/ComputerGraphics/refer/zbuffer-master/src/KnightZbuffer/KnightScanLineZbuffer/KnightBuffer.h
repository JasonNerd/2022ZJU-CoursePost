/////////////////////////////////////////////////////////////////////////////////
//       ___          ___                     ___          ___
//      /__/|        /__/\       ___         /  /\        /__/\         ___
//     |  |:|        \  \:\     /  /\       /  /:/_       \  \:\       /  /\
//     |  |:|         \  \:\   /  /:/      /  /:/ /\       \__\:\     /  /:/
//   __|  |:|     _____\__\:\ /__/::\     /  /:/_/::\  ___ /  /::\   /  /:/
//  /__/\_|:|____/__/::::::::\\__\/\:\__ /__/:/__\/\:\/__/\  /:/\:\ /  /::\
//  \  \:\/:::::/\  \:\~~\~~\/   \  \:\/\\  \:\ /~~/:/\  \:\/:/__\//__/:/\:\
//   \  \::/~~~~  \  \:\  ~~~     \__\::/ \  \:\  /:/  \  \::/     \__\/  \:\
//    \  \:\       \  \:\         /__/:/   \  \:\/:/    \  \:\          \  \:\
//     \  \:\       \  \:\        \__\/     \  \::/      \  \:\          \__\/
//      \__\/        \__\/                   \__\/        \__\/
//
//
// author		:	KNIGHT
// Description	:	Zbuffer���� �� ֡����
// Time			:	2008/12/20
// Email		:	KnightLiao@gmail.com	liaoqiqi@cad.zju.edu.cn
//////////////////////////////////////////////////////////////////////////////////


#ifndef _KNIGHT_BUFFER_H_
#define _KNIGHT_BUFFER_H_


//===֡������==========================================
template <typename T>
class KnightFrameBuffer{
public:

	KnightFrameBuffer():m_width(0),m_height(0){}

	//----���¸߶ȺͿ�ȣ��������������С
	void ResizeBuffer(int width,int height){
		if(m_width!=width || m_height!=height){
			m_width = width;
			m_height = height;
			m_effectWidth = (m_width * sizeof(T)+3)/4*4;	// ��Ч�Ŀ��
			m_buffer.clear();
			m_buffer.resize(m_effectWidth * m_height);	// ֡�����С
		}
	}

	//---��ʹ��֡����----
	void Memset(int value){
		memset(&m_buffer[0],value,m_buffer.size());
	}

	//---����Ҫд���֡����λ��-----
	T* Pixel(int row,int col){
		if(row>=0 && row<m_height && col>=0 && col < m_width)
			return ((T*)(&m_buffer[0]+m_effectWidth*row))+col;
		else{		
			return &m_dummy;	// ����δ֪����
		}
	}
private:
	std::vector<char> m_buffer;	// ֡����
	T m_dummy;	// tmp
	int m_width,m_height;	// ���/�߶�
	int m_effectWidth;	// ��Ч�Ŀ��
	KnightFrameBuffer(const KnightFrameBuffer&);	// ����
	KnightFrameBuffer& operator=(const KnightFrameBuffer &);	// ����
};


//===Zbuffer����========================================================
class KnightZBuffer{
public:

	KnightZBuffer(){}

	//---дZbuffer
	double& operator[](int index){
		if((index >=0) &&  (index < (int)m_Zbuffer.size()))
			return m_Zbuffer[index];
		else
			return m_dummy;
	}

	//---�������ô�С
	void ResizeBuffer(int n){
		m_Zbuffer.clear();
		m_Zbuffer.resize(n);
	}

private:
	std::vector<double> m_Zbuffer;
	double m_dummy;
	// �������º���
	KnightZBuffer(const KnightZBuffer&);
	KnightZBuffer& operator=(const KnightZBuffer&);
};

#endif