#include "Triple.h"
class FrameBuffer{
public:

	FrameBuffer():m_width(0),m_height(0)/*,m_centerX(0),m_centerY(0),m_center(0)*/{}

	//----更新高度和宽度，并重新设置其大小
	void ResizeBuffer(int width,int height){
		if(m_width!=width || m_height!=height){
			m_width = width;
			m_height = height;
			m_buffer.clear();
			m_buffer.resize(m_width * m_height * 3);	// 帧缓存大小
			//m_centerX = m_width >> 1;
			//m_centerY = m_height >> 1;
			//m_center = m_width * m_centerY + m_centerX;
		}
	}

	//---初使化帧缓存----
	void Memset(int value){
		memset(&m_buffer[0],value,m_buffer.size());
	}

	//---返回要写入的帧缓存位置-----
	void SetPixel(int x,int y,Triple<GLubyte>& color){
		if(x<0 || x>=m_width || y <0 || y>=m_height) return;
		*(&m_buffer[0]+(m_width*y+x)*3) = color.x;
		*(&m_buffer[0]+(m_width*y+x)*3+1) = color.y;
		*(&m_buffer[0]+(m_width*y+x)*3+2) = color.z;
		//std::cout<<"x:"<<x<<" y:"<<y<<std::endl;
		
			//memcpy(&m_buffer[m_center]+(m_width*y+x)*3, &color[0], 3);//debugger
		//}
	}

	Triple<GLubyte>* getPixelAddr(int x,int y){
		if(y>=0 && y<m_height && x>=0 && x<m_width)
			return ((Triple<GLubyte>*)(&m_buffer[0]+(m_width*y+x)*3));
		std::cout<<"getPixelAddr error"<<std::endl;
		return NULL;
	}

	int getBufferWidth(){
		return m_width;
	}
	int getBufferHeight(){
		return m_height;
	}
public:
	std::vector<GLubyte> m_buffer;	// 帧缓存
	int m_width,m_height;	// 宽度/高度
	//int m_centerX, m_centerY, m_center;
};