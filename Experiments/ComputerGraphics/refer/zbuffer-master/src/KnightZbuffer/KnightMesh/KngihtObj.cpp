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
// Description	:	OBJ����ģ��
//					�ɴ���������Ƭ
// Time			:	2008/12/20
// Email		:	KnightLiao@gmail.com	liaoqiqi@cad.zju.edu.cn
//////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "KnightObj.h"
#include "../KnightGlobal/KnightGlobal.h"
#include "../KnightDebug/KnightDebug.h"
#include "../KnightMath/KnightMath.h"


//=====��һ��ɨ��OBJ�ļ� ͳ�ƶ�����Ŀ ����Ŀ
bool KnightObj::ReloadObjFile(const char* file_path, int& vertex_number,int& surface_number) {

//	t.print("trace in KnightObj::ReloadObjFile\n");

	FILE *pFile;
	//---���а�ȫ���ƵĴ򿪺���---
	if(fopen_s(&pFile,file_path,"r"))
		return false;
	vertex_number = surface_number = 0;
	//---����һ�������65535���ַ�
	static const int MAXLEN = 65535;
	static char line[MAXLEN];
	//---���ñ�־λ ������һλΪ0,������޸�,��˵���л��岻��,����---
	line[MAXLEN-2] = 0;
	while(fgets(line,MAXLEN,pFile)){
		//---��� һ�г���65535 ��˵������
		if(line[MAXLEN-2]){
			fclose(pFile);
			return false;
		}
		switch(*line){
			case 'f':	// ����Ϣ
				//---һ��һ����Ƭ---��������ƬҲ�Ƕ������Ƭ
				surface_number++ ;
				break;
			case 'v':	// ������Ϣ v vt����  vn����
				// ÿһ�д���һ������
				if(*(line+1)==' ')
					++vertex_number;
				break;
			default :
				break;
		}
	}
	fclose(pFile);
	return vertex_number && surface_number;
}


//---��������1:ȥ�������ַ�,�����ո�(0x20)---
inline const char* ridOfCtrl(const char* s) {
	while (*s && (*s<0x21 || *s>0x7e)) ++s;
	return s;
}


//----��������2:��ȡ���һ��������Ϣ---
inline const char* getVerInfo(const char* is, char* os) {
	const char *old=is=ridOfCtrl(is);
	//---�ѷǿ��Ʒ�������,�������ո�---
	while (*is>0x20 && *is<0x7f) 
		*(os++)=*(is++); 
	*os=0;
	return old==is ? 0 : is;
}


//---��������3:��ȡ����һ������Ķ���������Ϣ---
inline unsigned int getVerIndex(const char* s) {
	//---ȥ�����ܳ��ֵ�+��---
	if (*(s=ridOfCtrl(s))=='+') 
		++s;
	unsigned int ret=0;
	while (*s>='0' && *s<='9') 
		ret=ret*10+*(s++)-'0';
	return ret;
}


//=====���������Ƭ=====
bool KnightObj::ReadPolygonal(const char* file_path, int& vertex_number,int& surface_number) {

	//---����ָ��---
	Triple<double>* vertex_ptr = &m_vertex[0];	 
	ObjFace* surface_ptr = &m_surface[0];	// ��Ƭָ��

	FILE *pFile;
	if(fopen_s(&pFile,file_path,"r"))
		return false;
	static const int MAXLEN = 65536;
	static char line[MAXLEN];
	vertex_number = surface_number = 0;
	line[MAXLEN-2] = 0;
	while(fgets(line,MAXLEN,pFile)){
		if(line[MAXLEN-2]){
			fclose(pFile);
			return false;
		}
		static char tmp[MAXLEN];
		const char *p;
		switch(*line){
		case 'v':	
			//---����
			if(*(line+1) == ' '){	
				//---���붥�������---
				sscanf_s(line+1,"%lf %lf %lf", &vertex_ptr->x, &vertex_ptr->y, &vertex_ptr->z);
				//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++ 		
//				t_stream << vertex_ptr->x << " " << vertex_ptr->y << " "<<vertex_ptr->z << std::endl;
				//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
				++vertex_ptr;
				++vertex_number;
			}		
			break;
		case 'f':
			//---����---
			p=line+1;
			//---�����ж�������ѹ��ջ---
			while(p=getVerInfo(p,tmp)){
				surface_ptr->vertexIndex.push_back(getVerIndex(tmp)-1);//---ע��,������ͼ�1��
			}
			//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
//			for(int j=0;j<(int)surface_ptr->vertexIndex.size();++j)
//				t_stream << surface_ptr->vertexIndex[j] << " " ; 
//			t_stream << " end " << std::endl;
			//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
			++surface_ptr;
			++surface_number;
			break;
		default:
			break;
		}
	}
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	t_stream << "surface num: " << surface_number << "\nvertex num: " << vertex_number << std::endl;
	//t_stream.offFile_stream();
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	fclose(pFile);
	return true;
}


//====����ģ��������Ϣ=====
// ���ĵ� ģ�ʹ�С ��ķ�����Ϣ
void KnightObj::SetProperties() {
	m_center.x = m_center.y = m_center.z = 0.0 ;
	Triple<double> min_xyz = m_vertex[0],max_xyz=m_vertex[0];
	for(int i=0; i< (int)m_vertex.size(); ++i){
		m_center += m_vertex[i];
		min_xyz.x = min(min_xyz.x,m_vertex[i].x);
		min_xyz.y = min(min_xyz.y,m_vertex[i].y);
		min_xyz.z = min(min_xyz.z,m_vertex[i].z);
		max_xyz.x = max(max_xyz.x,m_vertex[i].x);
		max_xyz.y = max(max_xyz.y,m_vertex[i].y);
		max_xyz.z = max(max_xyz.z,m_vertex[i].z);
	}
	//---����λ��---
	m_center/=(float)m_vertex.size();
	//---������---
	m_normal.resize(m_surface.size());
	//---ģ�͵Ĵ�С----
	m_scale = (max_xyz-min_xyz).VectorLen();
	
	//---����ķ�����Ϣ---
	for(int i=0;i<(int)m_surface.size();++i){
		m_normal[i] = (m_vertex[m_surface[i].vertexIndex[1]] - m_vertex[m_surface[i].vertexIndex[0]])*
					(m_vertex[m_surface[i].vertexIndex[2]]-m_vertex[m_surface[i].vertexIndex[0]]);
		m_normal[i] /= m_normal[i].VectorLen();
	}
	
}
