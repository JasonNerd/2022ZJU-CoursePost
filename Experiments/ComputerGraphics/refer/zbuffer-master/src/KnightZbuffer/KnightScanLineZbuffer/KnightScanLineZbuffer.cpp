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
// Description	:	ɨ����Zbufferʵ��
//						���Դ������������ ֻ���Դ���͹����� 
//						������OpenGL���ֹ���	
//						ģ���˹��������Ĵ���,ʹ��ͬ����в�ͬ������ֵ,�Ա�����������ʾ�������
// Time			:	2008/12/20
// Email		:	KnightLiao@gmail.com	liaoqiqi@cad.zju.edu.cn
//////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "KnightScanLineZbuffer.h"
#include "../KnightGlobal/KnightGlobal.h"
#include "../KnightDebug/KnightDebug.h"


//---��ʹ��,���� ���(������/��߶�)   Ͱ(�����Ͱ/��Ͱ)   ֡����---------------
KnightScanLineZbuffer::KnightScanLineZbuffer()
{
	m_activeList = new KnightActiveList;	// ���
	m_bucket = new KnightBucket;	// Ͱ
	m_frameBuffer = new KnightFrameBuffer<Triple<unsigned char> >; // frame buffer
}


//---��ĳ�ʹ��
//---������ ��������ָ�� ���� ������ָ�� ��������ָ��-----------------------------------
void KnightScanLineZbuffer::InitialZbuffer( int vetex_num,Triple<double> *vertex,
					   int face_num, ObjFace *surface , Triple<double> *xx_normal)
{
	m_vertex_num = vetex_num;	// ������
	m_surface_num = face_num;	// ����
	m_vertex = vertex;			// ��������
	m_surface = surface;		// ������
	m_normal_x = xx_normal;		// ԭ��������
	
	//---�������ݵı���,��Ҫ��������תʱ��������Ҫ�仯,���ֲ���ı�ԭ��������-----
	m_normal.clear();
	m_normal.resize(m_surface_num);
	for(int i=0;i<m_surface_num;++i){
		m_normal[i].x = xx_normal[i].x;
		m_normal[i].y = xx_normal[i].y;
		m_normal[i].z = xx_normal[i].z;
	}

//	m_zBuffer = new KnightZBuffer;	// zbuffer

	//---��ɫ����,��Ҫ���������ɫ---
	m_color.clear();
	m_color.resize(m_surface_num);	
}


//---��������-----------------------------------------------------------------
KnightScanLineZbuffer::~KnightScanLineZbuffer()
{
	//---�������---
	m_normal.clear();
	m_color.clear();

	//---��new������Ӧ
	if(m_activeList){
		delete m_activeList;
		m_activeList = NULL;
	}
	if(m_bucket){
		delete m_bucket;
		m_bucket = NULL;
	}
	if(m_frameBuffer){
		delete m_frameBuffer;
		m_frameBuffer = NULL;
	}
}


//---��ʹ����Ͱ �����Ͱ
//---�Ѷ������ݶ������� ������д��Ͱ����-----------------------------------------------------
//---YͰֵһ��ͳһȡ��-----------------
void KnightScanLineZbuffer::setBucket()
{
	int height = m_viewport[3];	// ���
	int width = m_viewport[2];	// �߶�

	//---reset Ͱ
	m_bucket->Reset(height+1);
	//---reset ��߱�
	//m_activeList->Reset(m_surface_num+1);
	m_activeList->Reset();

	//---��ÿ����߱���д���,����Y�����YͰ------------------
	for(int i=0;i<(int)m_surface_num;++i){

		//---��ö���ε�Y�仯��------------------------
		double min_y,max_y ;
		min_y = max_y = m_projectionVertex[m_surface[i].vertexIndex[0]].y;	// ��һ�������Yֵ

		//---��ö���ε����Yֵ����Сֵ---
		for(int j =0;j<(int)m_surface[i].vertexIndex.size()-1;++j){
			min_y = min(min_y, m_projectionVertex[m_surface[i].vertexIndex[j+1]].y);
			max_y = max(max_y, m_projectionVertex[m_surface[i].vertexIndex[j+1]].y);
		}
		//---�ü������Ӵ���Ķ����,���ٻ��ƹ���------
		if((max_y)<0 || (min_y)>=height)	
			continue;

		//---ѭ��ÿ����,�����YͰ---------------------------------
		int delta_y;		// Y�ı仯��
		double edge_max_y,edge_min_y;	// �ߵ����Yֵ����СYֵ
		double delta_x;		// X�ı仯��
		//double edge_min_x,edge_max_x;
		double edge_upper;	// �϶˵��Xֵ
		ObjFace *objFacePtr = &m_surface[i];	// ��ָ��

		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
		//t_scanLine << "face "<< i << " : " << objFacePtr->vertexIndex.size() << std::endl;
		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

		//---��ѭ����ʼ-------------
		for(int j =0;j<(int)objFacePtr->vertexIndex.size();++j){

			//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
			//t_scanLine << objFacePtr->vertexIndex[j]  << std::endl;
			//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
			
			//---���һ���������һ������Ҳ�����һ����---
			int next = j+1;
			if(next >=(int)objFacePtr->vertexIndex.size())
				next = 0;

			//----�ü�---,����-----
			bool is_y_maxY = true;
			double zz = 1.0;	// �ߵ������Ϣ
			if(m_projectionVertex[objFacePtr->vertexIndex[j]].y > m_projectionVertex[objFacePtr->vertexIndex[next]].y){
				edge_max_y = m_projectionVertex[objFacePtr->vertexIndex[j]].y;
				edge_min_y =m_projectionVertex[objFacePtr->vertexIndex[next]].y;
				zz = m_projectionVertex[objFacePtr->vertexIndex[j]].z;		// �ñߵ�Zֵ
				edge_upper = m_projectionVertex[objFacePtr->vertexIndex[j]].x;	//ȡ�϶˵�(Yֵ�ϴ�Ķ���)��Xֵ
				is_y_maxY = true;
			}
			else{
				edge_max_y = m_projectionVertex[objFacePtr->vertexIndex[next]].y;
				edge_min_y = m_projectionVertex[objFacePtr->vertexIndex[j]].y;
				zz = m_projectionVertex[objFacePtr->vertexIndex[next]].z;
				edge_upper = m_projectionVertex[objFacePtr->vertexIndex[next]].x;
				is_y_maxY = false;
			}

/*	
			if(m_projectionVertex[objFacePtr->vertexIndex[j]].x > m_projectionVertex[objFacePtr->vertexIndex[next]].x){
				zz = m_projectionVertex[objFacePtr->vertexIndex[j]].z;
			}
			else{
				zz = m_projectionVertex[objFacePtr->vertexIndex[next]].z;
			}
*/
			//edge_max_y = max(m_projectionVertex[objFacePtr->vertexIndex[j]].y,m_projectionVertex[objFacePtr->vertexIndex[j+1]].y);
			//edge_min_y = min(m_projectionVertex[objFacePtr->vertexIndex[j]].y,m_projectionVertex[objFacePtr->vertexIndex[j+1]].y);
			
			//---�ߵ�Yֵ������Ļ��С--------û�д���ɹ�����-------------
			if(edge_max_y >= height){
				
				//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
				//t_scanLine << " @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << edge_max_y << " " << i << "  " << height <<  " " << width <<std::endl;
				//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

				double tmp_yy = (edge_max_y - height + 1);
				double tmp_yyy,tmp_xxx;
				if(is_y_maxY){
					tmp_yyy = m_projectionVertex[objFacePtr->vertexIndex[j]].y - m_projectionVertex[objFacePtr->vertexIndex[next]].y;
					tmp_xxx = m_projectionVertex[objFacePtr->vertexIndex[j]].x - m_projectionVertex[objFacePtr->vertexIndex[next]].x;
				}
				else{
					tmp_yyy	= m_projectionVertex[objFacePtr->vertexIndex[next]].y - m_projectionVertex[objFacePtr->vertexIndex[j]].y;
					tmp_yyy	= m_projectionVertex[objFacePtr->vertexIndex[next]].x - m_projectionVertex[objFacePtr->vertexIndex[j]].x;
				}
				double tmp_xx = (tmp_yy * tmp_xxx)/tmp_yyy;
				double tmp_delta_zx = m_normal[i].x / m_normal[i].z;
				tmp_delta_zx *= (-1.0);
				double tmp_delta_zy = m_normal[i].y / m_normal[i].z;
				double tmp_delta_z = tmp_xx * tmp_delta_zx - tmp_delta_zy * tmp_yy;
				if(is_y_maxY){
					zz = m_projectionVertex[objFacePtr->vertexIndex[j]].z  - tmp_delta_z;
					edge_upper = m_projectionVertex[objFacePtr->vertexIndex[j]].x - tmp_xx;
				}
				else{
					zz = m_projectionVertex[objFacePtr->vertexIndex[next]].z  - tmp_delta_z;
					edge_upper = m_projectionVertex[objFacePtr->vertexIndex[next]].x - tmp_xx;
				}
				
				//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
				t_scanLine << edge_upper <<  "  " <<  zz << std::endl;
				//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

				edge_max_y = height-1;
			}
			if(edge_min_y < 0){

				//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
				//t_scanLine << " $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << edge_min_y << " " << i << "  " << "0" << " " << width << std::endl;
				//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

				double tmp_yy = (edge_min_y - 0.0);
				double tmp_yyy,tmp_xxx;
				if(is_y_maxY){
					tmp_yyy = m_projectionVertex[objFacePtr->vertexIndex[next]].y - m_projectionVertex[objFacePtr->vertexIndex[j]].y;
					tmp_xxx = m_projectionVertex[objFacePtr->vertexIndex[next]].x - m_projectionVertex[objFacePtr->vertexIndex[j]].x;
				}
				else{
					tmp_yyy	= m_projectionVertex[objFacePtr->vertexIndex[j]].y - m_projectionVertex[objFacePtr->vertexIndex[next]].y;
					tmp_yyy	= m_projectionVertex[objFacePtr->vertexIndex[j]].x - m_projectionVertex[objFacePtr->vertexIndex[next]].x;
				}
				double tmp_xx = (tmp_yy * tmp_xxx)/tmp_yyy;
				double tmp_delta_zx = m_normal[i].x / m_normal[i].z;
				tmp_delta_zx *= (-1.0);
				double tmp_delta_zy = m_normal[i].y / m_normal[i].z;
				double tmp_delta_z = tmp_xx * tmp_delta_zx - tmp_delta_zy * tmp_yy;
				if(is_y_maxY){
					zz = m_projectionVertex[objFacePtr->vertexIndex[next]].z  - tmp_delta_z;
					edge_upper = m_projectionVertex[objFacePtr->vertexIndex[next]].x - tmp_xx;
				}
				else{
					zz = m_projectionVertex[objFacePtr->vertexIndex[j]].z  - tmp_delta_z;
					edge_upper = m_projectionVertex[objFacePtr->vertexIndex[j]].x - tmp_xx;
				}

				//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
				//t_scanLine << edge_upper <<  "  " << zz << std::endl;
				//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

				edge_min_y = 0;
			}
			//---�ñߵ�y�ı仯��---------------------------------------
			//---����Y����,һ��ͳһȡ��..
			//---���,Y�ı仯Ӧ���Ƿֱ�ȡ����,�����-------------------------
			delta_y = (int)edge_max_y - (int)edge_min_y;

			//---x�ı仯��
			//edge_min_x= min(m_projectionVertex[objFacePtr->vertexIndex[j]].x,m_projectionVertex[objFacePtr->vertexIndex[next]].x);
			//edge_max_x = max(m_projectionVertex[objFacePtr->vertexIndex[j]].x,m_projectionVertex[objFacePtr->vertexIndex[next]].x); 
			//delta_x = edge_max_x - edge_min_x;
			//---�϶˵��XֵΪ edge_max_x

			//---������ ˮƽ��,�Թ�ˮƽ�ߣ����洢---------------------
			if(delta_y<1e-5){	// ������ʵ����delta_y=0,
				continue;
				//delta_x = 0;
				//delta_y = 0;
			}
			else{	// ��ˮƽ�� Ҫ���X�ı仯��	
				delta_x = (m_projectionVertex[objFacePtr->vertexIndex[j]].x-m_projectionVertex[objFacePtr->vertexIndex[next]].x) / 
					((int)m_projectionVertex[objFacePtr->vertexIndex[j]].y - (int)m_projectionVertex[objFacePtr->vertexIndex[next]].y);
				delta_x = (-1)*delta_x;
			}
			
			//---�ѱ߼����YͰ---------------
			int tmp_y = (int)(edge_max_y);	// edge_max_y����YouͰֵ

			//---�϶˵�Xֵedge_upper    X�ı仯��delta_x    
			//---Y�ı仯��delta_y   YͰtmp_y   Z���ֵzz
			m_bucket->AddEdge(edge_upper,delta_x,delta_y,i,tmp_y,zz);

			//++objFacePtr;		//---��һ����
		}
		//---�����ǶԶ���ε�ÿ���߽��д���---------------------------

		//---���¼��㷨��,��Ϊ��������ת---------------------------------
		m_normal[i] = (m_projectionVertex[m_surface[i].vertexIndex[1]] - m_projectionVertex[m_surface[i].vertexIndex[0]])*
			(m_projectionVertex[m_surface[i].vertexIndex[2]]-m_projectionVertex[m_surface[i].vertexIndex[0]]);
		m_normal[i] /= m_normal[i].VectorLen();
	
		//---�������ɫ,���� ģ���ӵ������ԭ���ߵ������ ��ģ�����-------------------------------------
		double factor = m_modelview[2]*m_normal_x[i].x + m_modelview[6]*m_normal_x[i].y + m_modelview[10]*m_normal_x[i].z;
		if(factor<0)
			factor = -factor;
		Triple<unsigned char> color;
		color.x = (unsigned char)Round(94.0f*factor);
		color.y = (unsigned char)Round(225.0f*factor);
		color.z = (unsigned char)Round(30.0f*factor);
		m_color[i] = color;		// ��i������ε���ɫ����ֵ

		//---�Ѷ���η�������YͰ------------------------------------------
		//---����ε�Y�仯��  �ü����max_y min_y
		if(max_y >=height)
			max_y = height-1;
		if(min_y<0)
			min_y = 0;

		delta_y = (int)max_y - (int)min_y;	// Y�ı仯�� Ҫ��ȡ�������
		int tmp_y = (int)(max_y);	// YͰֵ

		//---�·���m_normal[i]  Y�ı仯��delta_y  ��ɫ������color  YͰtmp_y
		m_bucket->AddPolygon(m_normal[i],i,delta_y,color,tmp_y);
	}
	
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	//m_bucket->printBucketPloy();
	//m_bucket->printBucketEdge();
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
}


//---��ʹ��֡���� Zbuffer����---------------------------------
void KnightScanLineZbuffer::InitialBuffers()
{
	m_zBuffer.ResizeBuffer(m_viewport[2]+1);	// ���Zbuffer����ֵ������������Zbuffer ��С
	m_frameBuffer->ResizeBuffer(m_viewport[2],m_viewport[3]);	// ���֡�����ֵ�����������ô�Сframe buffer size
	m_frameBuffer->Memset(0xA0);		// ��ɫ��Ļ
}


//---�Ѹö������ɨ���ߵĽ���߶Է���߻����-----------------------------------
//---numΪɨ����  poly_numΪ����α��  paraΪ���� a b c
//--func Ϊ0 ��ʾ�ǵ������µĶ����ʱ ������
//--func Ϊ1 ��ʾ�ǵ�����ʧʱ �¼����˱߶Ե����Σ�������Ҫ��delta_ylr����һ
//--err_code Ϊ���صĴ������-----
bool KnightScanLineZbuffer::PutEdgeToAcitive(int num,int poly_num,Triple<double> &para,int func,int &err_code)
{
	
	//---�����������ཻ������-------------------
	std::vector<Edge> edge;	// ���ص��ཻ�߱�
	int ret_num = m_bucket->HaveEdge(num,poly_num,edge);
	//---����ҵ��ıߵ���������ż����,�����
	if(ret_num%2 != 0){
		AfxMessageBox("error in function: KnightScanLineZbuffer::PutEdgeToAcitive ",MB_YESNO|MB_ICONQUESTION);
		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
		//t_scanLine << "error in function: KnightScanLineZbuffer::PutEdgeToAcitive ret_num%2 != 0" <<std::endl;
		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
		err_code = 1;	// �������---
		return false;
	}

	//---�߱��������---------------------------------------
	Edge *edge_left = NULL;		// �߶Ե����
	Edge *edge_right = NULL;	// �߶Ե��ұ�
	
	for(int i=0;i<(int)edge.size();i+=2){	// i+=2 �����ɶ�

		//---���ñ߱���Ϣ                                    
		//---�ж���ߺ��ұ�
		edge_left = &edge[i+1];
		edge_right = &edge[i];
		if(edge_right->num != edge_left->num){
			AfxMessageBox("error in function: KnightScanLineZbuffer::PutEdgeToAcitive ",MB_YESNO|MB_ICONQUESTION);
			//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
			//t_scanLine << "error in function: KnightScanLineZbuffer::PutEdgeToAcitive " <<std::endl;
			//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
			err_code = 2;
			return false;
		}
		if(edge_left->x + edge_left->delta_x  > edge_right->x + edge_right->delta_x){
			edge_left = &edge[i];
			edge_right = &edge[i+1];
		}
		//---edge_leftһ��ָ����ߵı�

		EdgeActive edge_active;
		edge_active.xl = edge_left->x;	// xl
		edge_active.delta_xl = edge_left->delta_x;	// delta xl
		edge_active.delta_yl = edge_left->delta_y;	// delta yl
		edge_active.zl = edge_left->z;	// ���߽�����������Ĵ�����ε����ֵ*****
		
		//���ƽ����XOYƽ��
		if(para.z==0){
			edge_active.delta_zx = 0;
			edge_active.delta_zy = 0;
		}
		else{
			edge_active.delta_zx = -(para.x)/(para.z);
			edge_active.delta_zy = (para.y)/(para.z);
		}

		edge_active.xr = edge_right->x;
		edge_active.delta_yr = edge_right->delta_y;
		edge_active.delta_xr = edge_right->delta_x;
		
		edge_active.num = edge_right->num;

		//---�������µĶ����ʱ,�����±ߵ�����
		if(func==1){
			--edge_active.delta_yr;
			--edge_active.delta_yl;
		}	

		//---�Ѹ� �߶� ���� ���Ա߶Ա�
		//---poly_numΪ����εı��  edge_activeΪҪ����ı߶�
		m_activeList->AddActiveEdge(poly_num,edge_active);
	}

	err_code = 0;	// �޴�
	return true;
}


//---������ε�YͰ��������µĶ�����漰��ɨ���ߣ�������������λ����
//---����������֮�� ҲҪ�Ѹö�����뵱ǰɨ�����ཻ�ı߶� ���뵽��߶Ա�-----------------
void KnightScanLineZbuffer::CheckPolygonBucket_PutItToActive(int num,int &err_code)
{
	//---�õ�����α�
	std::vector<KnightPolygon> poly ;	// ���������
	if(m_bucket->HaveBucketPoly(num,poly)==-1){
		err_code = 0;	// û���ཻ�Ķ����,�޴�
		return ;
	}
	
	//---�Ѷ���η�����-----------------------
	for(int i=0;i<(int)poly.size();++i){

		// �������ε����ݾ���һ��ֱ����,�����������,Ҳ��������
		if(poly[i].delta_y == 0)
			continue;		
		m_activeList->AddActivePoly(poly[i]);
		int poly_num = poly[i].num;	// ����α��

		//---�Ѹö������ɨ���ߵĽ���߶Է���߻��
		//--numΪɨ���� poly_num����α�� 
		//--normalΪ�������� 0Ϊ����ѡ�� err_codeΪ�������
		if(!PutEdgeToAcitive(num,poly_num,m_normal[poly_num],0,err_code)){
			return;
		}
	}	

	err_code = 0;	// �޴���
	return;
}


//**************************************************************************************************************************
//=====����ÿһ��ɨɨ����============================================================================
//--numΪɨ���� error_codeΪ�������---
void KnightScanLineZbuffer::DealWithActiveEdge(int num,int &error_code)
{
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	//t_scanLine << "trace in DealWithActiveEdge\n" ;
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

	//---�õ���߶Ա�---
	std::list<EdgeActive> * edge_activeList;
	edge_activeList = m_activeList->getEdgeActive();
	
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	//m_activeList->printEdge();
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

	//---Ŀǰû�л��
	if(edge_activeList->size()==0){
		error_code = 0;		// �޴�
		return ;
	}
	
	//---����ÿһ���߶�----------------------------------------------	
	double zx;
	Iterator it = edge_activeList->begin();
	int cout_tmp = (int)edge_activeList->size();	// Ҫ����Ĵ���
	for(int i=0; i<cout_tmp;){

		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
		//t_scanLine << " ���ڼ������ " << (*it).num << "����εı�" << std::endl;
		//m_activeList->printEdgeInfo((*it).num);
		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

		//---��xl<=x<=xr�����ؽ��д���--------------
		zx = (*it).zl;
		for(int x=(int)(*it).xl+1; x<=(int)(*it).xr; x++){
			
			zx += (*it).delta_zx;
			//---��Zbuffer��ֵ���жԱ�
			if(zx < m_zBuffer[x]){		// ȡ��Сֵ
				m_zBuffer[x] = zx;
				//---����֡���� ��num�У���x�� , ���óɵ�(*it).num������ε���ɫ
				*m_frameBuffer->Pixel(num,x) = m_color[(*it).num];
			}
		}
		
		//---���±߶���Ϣ
		(*it).delta_yl -= 1;
		(*it).delta_yr -= 1;

		//---�߶�ͬʱ��ʧ
		if((*it).delta_yl<0 && (*it).delta_yr<0){

			//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
			//t_scanLine << "�߶�ͬʱ��ʧ" << std::endl;
			//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

			//---���ñ߶�����������Ƿ���Ӧ���ڻ����α���
			int delta_y_tmp = m_activeList->getPolyDeltaY((*it).num);
			if(delta_y_tmp<=0){	// �ö�߻��б�δ����
				m_activeList->erasePolyActive((*it).num);	// ɾ������
				m_activeList->eraseEdgeActive(it++);		// �ӻ�߱�ɾ���ñ߶�	
			}
			else{
				//---����Ļ���Ҫ�����߶�
				//--numΪɨ���� (*it).numΪ����α�� m_normalΪ������Ϣ 
				//-- 1Ϊ����ѡ��,��ʾҪ�������ߵ�Y�仯����һ,error_codeΪ�������
				if(!PutEdgeToAcitive(num,(*it).num,m_normal[(*it).num],1,error_code))
					return ;
				// �ӻ�߱�ɾ���ñ߶� it����ָ����һ��Ҫ����ı߶�
				m_activeList->eraseEdgeActive(it++);		
			}
			++i;		// ��һ��
			continue;	// �Ѿ�����øñ߶� 
		}
		else if((*it).delta_yl <0  || (*it).delta_yr<0){
		//---������һ������ʧ��-----

			//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
			//t_scanLine << "��һ������ʧ" << std::endl ;
			//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

			//---Ѱ���µı�-------------------
			int poly_num = (*it).num;	// ��poly_num�Ŷ����
			//��num----num+delta_y֮��Ѱ���µı�
			//---�ж�һ������ߵ�delta_y �����ұߵ�delta_y
			int delta_tmp = (*it).delta_yl;
			if((*it).delta_yl <0)
				delta_tmp = (*it).delta_yr;
	
			//---�õ����ʵı������---
			//--numΪɨ���� delta_tmpΪ����ɨ������ poly_numΪ����� edge_tmpΪ�õ����±�------------
			Edge edge_tmp ;
			if(!m_bucket->getProperEdge(num,delta_tmp,poly_num,edge_tmp)){
				AfxMessageBox("error in function: KnightScanLineZbuffer::DealWithActiveEdge �޷����������!!!",MB_YESNO|MB_ICONQUESTION);
				//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
				//t_scanLine << "error in function: KnightScanLineZbuffer::DealWithActiveEdge " <<std::endl;
				//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
				error_code = 1;	// �������1
				return;	// �������ǲ�Ӧ�ó��ֵ�����!!!!!!!!!!!!!!!!!!!
			}

			//---�����ʧ���ұ߻���
			if((*it).delta_yl<0 && (*it).delta_yr>0){
				//---�������ñ߶�-----
				(*it).xl = edge_tmp.x;
				(*it).delta_yl = edge_tmp.delta_y -1 ;
				(*it).delta_xl = edge_tmp.delta_x;

				// ����ƽ���������,�����ʧ��,�²���ıߵ�zֵ ��ԭ����ֵ���
				(*it).zl = edge_tmp.z;	
			}
			else{
				//---��߻��ڣ��ұ���ʧ
				//---�������ñ߶�,zl����,��Ϊ���û����ʧ-----
				(*it).xr = edge_tmp.x;
				(*it).delta_yr = edge_tmp.delta_y -1 ;
				(*it).delta_xr = edge_tmp.delta_x;
			}
		}

		//----������һ��ɨ���߽���-------------����-------
		(*it).xl = (*it).xl + (*it).delta_xl;
		(*it).xr = (*it).xr + (*it).delta_xr;
		//----�Ǽ���delta_zy,�����Ǽ�ȥ,���Ͻ����д���
		(*it).zl = (*it).zl + (*it).delta_zx * (*it).delta_xl + (*it).delta_zy;
	
		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
		//m_activeList->printEdgeInfo((*it).num);
		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

		++it;	//-----��һ���߶�
		++i;	// ��һ��
	}
	//---�����Ѿ���������еı߶���Ϣ----------------------------------------------------

	//---�������е�ÿһ������ε� delta_y ��1
	m_activeList->PolySubDeltaY();
	
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	//m_activeList->printEdge();
	//m_activeList->printPoly();
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

	//---ɾ�������Զ������delta_yС��0�Ķ����-----------
	m_activeList->PolyUpdate();

	error_code = 0;	// �޴�
	return;
}


//----------ɨ�������㷨���--------------------------------------------------------------------------------------------------
bool KnightScanLineZbuffer::MainAlgorithm()
{
	int width = m_viewport[2];
	int height = m_viewport[3];

	//-------���Ͽ�ʼ����ɨ��--------------------------
	for(int i=height-1 ; i>0 ; --i){
		
		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
		//t_scanLine << "ɨ����" << i << " *************************************************************" << std::endl;
		//m_activeList->printEdge();
		//m_activeList->printPoly();
		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

		//---reset zbuffer-------------
		for(int j=0;j<width; ++j)	
  			m_zBuffer[j] = 1;		// ���ó���С��ֵ
		
		int err_code = 0;

		//---�������YͰ��������µĶ�����漰ɨ���ߣ�������������λ����-----
		//---������µĶ���μ������λ����Ҫ�Ѹö������OXYƽ���ϵ�ͶӰ��ɨ�����ཻ�ı߶Լ���߻����
		CheckPolygonBucket_PutItToActive(i, err_code);
		if(err_code!=0)
			return false;

		//---ȡ���߶�������------------------------------------------------
		//---�����Щ��������ɨ���ߴ������ˣ��������ڵĶ�������ڶ���λ���ڣ���ɴӱߵ�YͰ���ҵ��ö������OXYƽ����
		//---��ͶӰ��ɨ�����ཻ���µı߻�߶ԣ��޸Ļ�ӵ��ߵĻ����ȥ��
		DealWithActiveEdge(i,err_code);	
		if(err_code !=0 )
			return false;	//����
	}

	return true;
}


//---ͼ�λ��ƺ���-------------------------------------------
void KnightScanLineZbuffer::DrawLine()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,m_viewport[2],0.0,m_viewport[3]);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(0,0);
	glDrawPixels(m_viewport[2],m_viewport[3],GL_RGB,GL_UNSIGNED_BYTE,m_frameBuffer->Pixel(0,0));
	glPopMatrix();
}

//==============================================================================================================================
//====ɨ����Zbuffer�㷨========================================================================================================
bool KnightScanLineZbuffer::StartAlgorithm()
{
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
//	t_scanLine.on_stream();
//	t_stream.on_stream();
	t_scanLine.setPrecision(10);
	t_stream.setPrecision(10);
	t_scanLine.off_stream();	// �ر����е���
	t_stream.off_stream();		// �ر����е���
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

	//---����͸��ͶӰ���õ��ӿڡ�ģ�͡�ͶӰ����----
	SetScene();
	//---��ģ��ͶӰ����Ļ��-----------
	ProjectScene();
	//---���ú� �����YͰ YͰ ���Ͱ ���߱�Ͱ
	setBucket();

	//===============��ʼ�㷨==========================================
	//---���ú� ֡���桢Zbuffer
	InitialBuffers();
	//---���㷨
	if(!MainAlgorithm())
		return false;	// ����
	//---draw
	DrawLine();

	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	//for(int i= 0;i<m_surface_num;++i)
	//	t_stream << m_normal[i].x  << " " << m_normal[i].y << " " << m_normal[i].z <<std::endl;
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

	return true;
}
//===============================================================================================================================


//------------------------------------------------------------------------
void KnightScanLineZbuffer::SetScene(){
	//---�����ɫ����--------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//---�õ��ӿ�
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//---͸��ͶӰ
	gluPerspective(60.0, m_viewport[2] / (double)m_viewport[3], 1e-2, 1e6);
	//---�õ�ģ�ͱ任����
	glGetDoublev(GL_MODELVIEW_MATRIX,m_modelview);
	//---�õ�ͶӰ����
	glGetDoublev(GL_PROJECTION_MATRIX,m_projection);
}

//---����ͶӰ����----------------------------------------
void KnightScanLineZbuffer::ProjectScene()
{	
	// ����ͶӰ����
	m_projectionVertex.resize(m_vertex_num);
	for(int i=0;i<(int)m_vertex_num;++i){
		gluProject(m_vertex[i].x, m_vertex[i].y, m_vertex[i].z, m_modelview, m_projection, m_viewport,
			&m_projectionVertex[i].x,&m_projectionVertex[i].y,&m_projectionVertex[i].z);
	}
/*
	//---���¼��㷨��
	for(int i=0;i<m_surface_num;++i){
		m_normal[i] = (m_projectionVertex[m_surface[i].vertexIndex[1]] - m_projectionVertex[m_surface[i].vertexIndex[0]])*
			(m_projectionVertex[m_surface[i].vertexIndex[2]]-m_projectionVertex[m_surface[i].vertexIndex[0]]);
		m_normal[i] /= m_normal[i].VectorLen();
	}
	*/
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
/*	t_stream << "888888888888888888888888888888888888888888888 " << std::endl;
	for(int i=0;i<m_surface_num;++i){
		t_stream << "face " << i << " size: " << m_surface[i].vertexIndex.size() << "*******************" << std::endl;
		for(int j=0;j<(int)m_surface[i].vertexIndex.size();++j){
			t_stream << m_surface[i].vertexIndex[j] << " " << std::endl;
			t_stream << m_projectionVertex[m_surface[i].vertexIndex[j]].x << " "
				<< m_projectionVertex[m_surface[i].vertexIndex[j]].y << " "
				<< m_projectionVertex[m_surface[i].vertexIndex[j]].z << " " << std::endl;
		}
		t_stream << " " << std::endl;
	}
*/	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
}