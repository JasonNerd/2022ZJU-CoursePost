#pragma once
#include <fstream>
#include <string>
#include <Windows.h>
#include <windef.h>
#include <io.h>
#include "Triple.h"
#include "Util.h"
class Obj
{
public:
	std::vector<Triple<GLdouble>> vVertex;
	//std::vector<Triple<GLdouble>> vVertexErr;//误差修正
	//GLdouble m_errAccuracy;//误差修正精度
	GLdouble m_offsetX, m_offsetY, m_offset;
	GLdouble m_scaleFactor;
	std::vector<std::vector<Triple<GLdouble>>> vfs;//面
	std::vector<std::vector<GLint>> ifaces;
	//vector<vector<string>> model;
	std::string objpath;
	GLint nVertex;//顶点数
	GLint nFace;//面数
	GLint m_winWidth, m_winHeight;
public:
	Obj(){
		m_winWidth = 1600;
		m_winHeight = 900;
		string moduleDir = getModueDir();
		vector<string> files;
		//cout<<"moduleDir:"<<moduleDir<<endl;
		getFiles(moduleDir, files);
		int item = getItem(files);
		objpath = files[item-1];
		loadObjFile();
		vfs.resize(nFace);
	}
	~Obj(){}

	int getItem(vector<string>& files){
		//cout<<"size:"<<files.size()<<endl;
		for(int i = 0; i <files.size(); i++)
			cout<<i+1<<"."<<files[i]<<endl;
		cout<<"chose one to perform:";
		int j;
		cin>>j;
		while(j<=0||j>files.size()){
			cout<<"error,again:";
			cin>>j;
		}
		return j;
	}
	void getFiles(string path, vector<string>& files ){
		//文件句柄
		long   hFile   =   0;
		//文件信息
		struct _finddata_t fileinfo;
		string p;
		if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
		{
			do
			{
				//如果是目录,迭代之
				//如果不是,加入列表
				if((fileinfo.attrib &  _A_SUBDIR))
				{
					if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
						getFiles( p.assign(path).append("\\").append(fileinfo.name), files );
				}
				else
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
				}
			}while(_findnext(hFile, &fileinfo)  == 0);
			_findclose(hFile);
		}
	}
	string getModueDir(){
		string exe = getEXEDir();
		int pos = exe.find_last_of("\\\\");
		pos = exe.substr(0,pos).find_last_of("\\\\");
		if(pos > 0){
			return exe.substr(0,pos) + "\\\\model";
		}
		return "";
	}
	string getEXEDir(){
		 TCHAR exeFullPath[MAX_PATH]; // MAX_PATH在WINDEF.h中定义了，等于260
		 memset(exeFullPath,0,MAX_PATH);

		 GetModuleFileName(NULL,exeFullPath,MAX_PATH);
		 return string(exeFullPath);
	}

	void loadObjFile(){
		/*第一遍扫描记录xy坐标的最小值和最大值，有如下两个作用：
		1.如果这两个值跨度太小，说明模型太小，为了显示模型，需要对坐标值进行放大
		2.对原始坐标进行偏移，使得模型基本可以完全展示出来*/
		GLdouble min_x = 999999.99, max_x = -999999.99;
		GLdouble min_y = 999999.99, max_y = -999999.99;
		Util util;
		std::ifstream ifs(objpath);
		std::string line;
		vVertex.push_back(Triple<GLdouble>(0,0,0));
		//vVertexErr.push_back(Triple<GLdouble>(0.0,0.0,0.0));
		//int debugger = 0;
		while(getline(ifs,line)){			
			vector<string> vline;
			//model[i].clear();
			util.split(vline, line);
			if(vline.size() == 0) continue;
			if("v" == vline[0]){
				//_ASSERT(vline.size() > 3);
				std::string s;
				GLdouble vertex[3];
				GLdouble d;//当前读取的坐标值
				for(int i = 0; i < 3; i++){
					s = vline[i+1];
					sscanf_s(s.c_str(), "%lf", &vertex[i]);
				}
				if(vertex[0] < min_x) min_x = vertex[0];
				if(vertex[0] > max_x) max_x = vertex[0];
				if(vertex[1] < min_y) min_y = vertex[1];
				if(vertex[1] > max_y) max_y = vertex[1];
				vVertex.push_back(Triple<GLdouble>(vertex));
				//vVertexErr.push_back(Triple<GLdouble>(0.0,0.0,0.0));
			}else if("f" == vline[0]){
				//_ASSERT(vline.size() > 3);
				std::vector<GLint> ele;
				int d;
				std::string s;
				for(GLuint i = 0; i < 4 && (i+1) < vline.size(); i++){
					s = vline[i+1];
					//丢弃材质等信息
					int slash_pos = s.find_first_of('/');
					if(slash_pos >= 0) s = s.substr(0, slash_pos);
					//...不支持负的顶点索引
					sscanf_s(s.c_str(), "%d", &d);
					ele.push_back(d);
				}
				ifaces.push_back(ele);
			}
		}
		//offset

		//m_offset = m_offsetX > m_offsetY ? m_offsetX : m_offsetY;

		//scale
		GLdouble spanx = max_x - min_x, spany = max_y - min_y;
		//GLdouble span = (spanx<spany?spanx:spany);
		//m_scaleFactor = (m_winHeight*100) / span / span;
		if(spanx <= 1 || spany <= 1) m_scaleFactor = 1000;
		else if(spanx < 5 || spany < 5) m_scaleFactor = 80;
		else if(spanx < 10 || spany < 10) m_scaleFactor = 40;
		else if(spanx < 20 || spany < 20) m_scaleFactor = 20;
		else if(spanx < 40 || spany < 40) m_scaleFactor = 10;
		else m_scaleFactor = 1;
		m_offsetX = (m_winWidth >> 1) - ((max_x + min_x)*m_scaleFactor / 2);//abs(min_x) + 0.5;
		m_offsetY = (m_winHeight >> 1) - ((max_y + min_y)*m_scaleFactor / 2);

		//m_offset += 10;
		//第二遍扫描
		//std::ifstream ifs(objpath);
		//std::string line;

		nVertex = vVertex.size() - 1;
		nFace = ifaces.size();
		//model.clear();
		//ifs.close();
	}
	
	Obj& getObj(){
		translateObj(Triple<GLdouble>(0,0,0));
		return *this;
	};


	void Matrix_Multip(GLdouble *A_matrix,GLdouble *B_matrix,GLdouble *AB_matrix,
		int A_Rows,int A_Colunms,int B_Rows,int B_Colunms)
	{
		//矩阵乘法 double*AB_matrix最后的结果
		for (int i=0;i<A_Rows;i++)
		{
			for (int j=0;j<B_Colunms;j++)
			{
				AB_matrix[i*B_Colunms+j]=0.0;
				for (int k=0;k<A_Colunms;k++)
				{
					AB_matrix[i*B_Colunms+j]=AB_matrix[i*B_Colunms+j]+A_matrix[i*A_Colunms+k]*B_matrix[k*B_Colunms+j];
				}   
			}
		}
	}

	Obj& rotateObj(GLdouble angle, Triple<GLdouble>& p/*1, Triple<GLdouble>& p2*/){
		/*
		###test
		*/
		/*GLdouble test1[3][3]={1,0,0,0,1,0,0,0,1};
		GLdouble test2[3][3]={1,0,0,0,1,0,0,0,1};
		GLdouble test3[3][3];
		Matrix_Multip((GLdouble*)test1,(GLdouble*)test2,(GLdouble*)test3, 3,3,3,3);*/
		/*end*/
		//Triple<GLdouble> rotVec = p1 - p2;//终点-始点，p2看向p1，p2-->p1
		//GLdouble modRotateAxis = sqrt(rotVec.x*rotVec.x+rotVec.y*rotVec.y+rotVec.z*rotVec.z);
		//_ASSERT(modRotateAxis > 0.0);
		GLdouble a = 1.0;//rotVec.x / modRotateAxis;
		GLdouble b = 1.0;//rotVec.y / modRotateAxis;
		GLdouble c = 1.0;//rotVec.z / modRotateAxis;
		//GLdouble d = sqrt(b*b+c*c);
		GLdouble theta = 3.1415926536*angle/180;
		GLdouble sintheta = sin(theta);
		GLdouble costheta = cos(theta);
		GLdouble transMat[4][4] = {
			1.0,0.0,0.0,m_offsetX,
			0.0,1.0,0.0,m_offsetY,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0
		};
		GLdouble scaleMat[4][4] = {
			m_scaleFactor,0.0,0.0,0.0,
			0.0,m_scaleFactor,0.0,0.0,
			0.0,0.0,m_scaleFactor,0.0,
			0.0,0.0,0.0,1.0
		};
		//GLdouble transMatInv[4][4] = {
		//	1,0,0,p2.x/m_axisScale,
		//	0,1,0,p2.y/m_axisScale,
		//	0,0,1,p2.z/m_axisScale
		//};
		GLdouble rotateMat[4][4] = {
			(1-costheta)*a*a+costheta,	(1-costheta)*b*a-sintheta*c,		(1-costheta)*a*c+sintheta*b,0,
			(1-costheta)*a*b+sintheta*c,(1-costheta)*b*b+costheta,		(1-costheta)*b*c-sintheta*a,0,
			(1-costheta)*a*c-sintheta*b,(1-costheta)*b*c+sintheta*a,		(1-costheta)*c*c+costheta,	0,
			0,							0,								0,							1
		};
		//GLint nVertex = vVertex.size()-1;
		//GLdouble* vertices = (GLdouble*)malloc(sizeof(GLdouble)*4*nVertex);
		GLdouble* vertices = (GLdouble*)malloc(sizeof(GLdouble)*4*nVertex);//减1是由于顶点数组第一个顶点是占位元素
		for(int i = 0; i < nVertex; i++){
			*(vertices + i) = vVertex[i+1].x/* + vVertexErr[i+1].x*/;
			*(vertices + i + nVertex) = vVertex[i+1].y/* + vVertexErr[i+1].y*/;
			*(vertices + i + (nVertex<<1)) = vVertex[i+1].z/* + vVertexErr[i+1].z*/;
			*(vertices + i + ((nVertex<<1)+nVertex)) = 1.0;
		}

		
		GLdouble* res = (GLdouble*)malloc(sizeof(GLdouble)*4*nVertex);
		//GLdouble* res2 = (GLdouble*)malloc(sizeof(GLdouble)*4*nVertex);
		//先旋转
		Matrix_Multip(reinterpret_cast<GLdouble*>(rotateMat), vertices, res, 4, 4, 4, nVertex);
		//原始数据保存
		for(int i = 0; i < nVertex; i++){
			vVertex[i+1].x = *(res + i)/* + (*(res + i)>=0?0.5:-0.5)*/;//大于0向上取整
			vVertex[i+1].y = *(res + i + nVertex)/* + (*(res + i + nVertex)>=0?0.5:-0.5)*/;
			vVertex[i+1].z = *(res + i + (nVertex<<1))/* + (*(res + i + (nVertex<<1))>=0?0.5:-0.5)*/;
		}
		//再缩放
		Matrix_Multip(reinterpret_cast<GLdouble*>(scaleMat), res, vertices, 4, 4, 4, nVertex);
		//平移至屏幕中心
		Matrix_Multip(reinterpret_cast<GLdouble*>(transMat), vertices, res, 4, 4, 4, nVertex);

		for(GLuint i = 0; i < nFace; i++){
			vfs[i].clear();
			for(GLuint j = 0; j < ifaces[i].size(); j++)
				vfs[i].push_back(Triple<GLdouble>(*(res + ifaces[i][j]-1),
				*(res + ifaces[i][j]-1 + nVertex),
				*(res + ifaces[i][j]-1 + (nVertex<<1))));
		}

		free(vertices);
		free(res);
		//free(res2);
		return *this;
	}

	Obj& translateObj(Triple<GLdouble>& p = Triple<GLdouble>(0,0,0)){
		//GLdouble transX,transY;
		m_offsetX = m_offsetX + p.x;
		m_offsetY = m_offsetY + p.y;
		GLdouble transMat[4][4] = {
			1.0,0.0,0.0,m_offsetX,
			0.0,1.0,0.0,m_offsetY,
			0.0,0.0,1.0,0,
			0.0,0.0,0,1.0
		};
		GLdouble scaleMat[4][4] = {
			m_scaleFactor,0.0,0.0,0.0,
			0.0,m_scaleFactor,0.0,0.0,
			0.0,0.0,m_scaleFactor,0.0,
			0.0,0.0,0.0,1.0
		};

		GLdouble* vertices = (GLdouble*)malloc(sizeof(GLdouble)*4*nVertex);
		for(int i = 0; i < nVertex; i++){
			*(vertices + i) = vVertex[i+1].x;
			*(vertices + i + nVertex) = vVertex[i+1].y;
			*(vertices + i + (nVertex<<1)) = vVertex[i+1].z;
			*(vertices + i + ((nVertex<<1)+nVertex)) = 1.0;
		}
		GLdouble* res = (GLdouble*)malloc(sizeof(GLdouble)*4*nVertex);

		//先缩放
		Matrix_Multip(reinterpret_cast<GLdouble*>(scaleMat), vertices, res, 4, 4, 4, nVertex);
		//再平移
		Matrix_Multip(reinterpret_cast<GLdouble*>(transMat), res, vertices, 4, 4, 4, nVertex);
		//vfs.clear();
		vfs.resize(nFace);
		for(GLuint i = 0; i < nFace; i++){
			vfs[i].clear();
			for(GLuint j = 0; j < ifaces[i].size(); j++)
				vfs[i].push_back(Triple<GLdouble>(*(vertices + ifaces[i][j]-1),//x
				*(vertices + ifaces[i][j]-1 + nVertex),//y
				*(vertices + ifaces[i][j]-1 + (nVertex<<1))));//z
		}
		
		free(vertices);
		free(res);
		return *this;
	}

	Obj& scaleObj(Triple<GLdouble>& p1, Triple<GLdouble>& p2, bool smaller){
		//double _scaleFactor = 0.0;
		if(smaller) m_scaleFactor = m_winHeight/abs(p1.y-p2.y + m_winHeight);
		else m_scaleFactor = abs(p1.y-p2.y + m_winHeight)/m_winHeight;
		GLdouble scaleMat[4][4] = {
			m_scaleFactor,0,0,0,
			0,m_scaleFactor,0,0,
			0,0,m_scaleFactor,0,
			0,0,0,1
		};
		GLdouble transMat[4][4] = {
			1.0,0.0,0.0,m_offsetX,
			0.0,1.0,0.0,m_offsetY,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0
		};
		GLdouble* vertices = (GLdouble*)malloc(sizeof(GLdouble)*4*nVertex);
		for(int i = 0; i < nVertex; i++){
			*(vertices + i) = vVertex[i+1].x + 0.0;
			*(vertices + i + nVertex) = vVertex[i+1].y + 0.0;
			*(vertices + i + (nVertex<<1)) = vVertex[i+1].z + 0.0;
			*(vertices + i + ((nVertex<<1)+nVertex)) = 1.0;
		}
		GLdouble* res = (GLdouble*)malloc(sizeof(GLdouble)*4*nVertex);
		//放缩
		Matrix_Multip(reinterpret_cast<GLdouble*>(scaleMat), vertices, res, 4, 4, 4, nVertex);
		//保存原始数据
		for(int i = 0; i < nVertex; i++){
			vVertex[i+1].x = *(res + i);
			vVertex[i+1].y = *(res + i + nVertex);
			vVertex[i+1].z = *(res + i + (nVertex<<1));
		}
		//平移
		Matrix_Multip(reinterpret_cast<GLdouble*>(transMat), res, vertices, 4, 4, 4, nVertex);
		for(GLuint i = 0; i < nFace; i++){
			vfs[i].clear();
			for(GLuint j = 0; j < ifaces[i].size(); j++)
				vfs[i].push_back(Triple<GLdouble>(*(vertices + ifaces[i][j]-1),
				*(vertices + ifaces[i][j]-1 + nVertex),
				*(vertices + ifaces[i][j]-1 + (nVertex<<1))));
		}
		
		free(vertices);
		free(res);
		return *this;
	}
};

