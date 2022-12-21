#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <fstream>
#include <iostream>
using namespace std;
#define MAXY 100
#define SIZE_ZBUFFER_LINE 500
#define POLYGON 4
#define ZBUFFER 
#define INFINITY 999999
#define BUFFER_OFFSIT_X 3
static GLfloat g_rotate = 10;
static int times = 0;
static int g_polygon_id = 0;

const static GLfloat g_renderColor[] = {1.0, 0.0, 0.0};

typedef GLdouble COORD_TYPE; 
//typedef COORD_TYPE point3d [3];

typedef struct{
	COORD_TYPE x, y, z;
}point3d;

void pt3dAdd(point3d* p, point3d* p1, point3d* p2){
	p->x = p1->x + p2->x;
	p->y = p1->y + p2->y;
	p->z = p1->z + p2->z;
}

void pt3dSub(point3d* p, point3d* p1, point3d* p2){
	point3d t;
	t.x = - p2->x;
	t.y = - p2->y;
	t.z = - p2->z;
	pt3dAdd(p, p1, &t);
}

typedef struct nodeClassifiedPolygon{
	GLfloat a, b, c, d;
	GLint id;
	GLint dy;//跨越的扫描线数目
	GLfloat color[3];
	nodeClassifiedPolygon* next;
}nodeClassifiedPolygon;

typedef struct nodeClassifiedEdge{
	COORD_TYPE x;
	COORD_TYPE dx;
	COORD_TYPE e_len;//边是水平边时，e_len为非0值，否则e_len=0
	GLint dy;
	GLint id;
	bool used;
	nodeClassifiedEdge* next;
}nodeClassifiedEdge;

typedef struct nodeActivePolygon{
	GLfloat a, b, c, d;
	GLint id;
	GLint dy;
	GLfloat color[3];
	nodeActivePolygon* next;
}nodeActivePolygon;

typedef struct nodeActiveEdgePair{
	COORD_TYPE xl;
	COORD_TYPE dxl;//相邻扫描线交点x坐标之差
	GLint dyl;//靠左的边跨越的扫描线数目
	GLfloat xr;
	COORD_TYPE dxr;
	GLint dyr;
	GLfloat zl;
	GLfloat dzx;
	GLfloat dzy;
	GLint id;
	GLfloat* color;
	nodeActiveEdgePair* next;
}nodeActiveEdgePair;

nodeClassifiedPolygon* tClassifiedPolygon[1000];
nodeClassifiedEdge* tClassifiedEdge[1000];
nodeActivePolygon tActivePolygonHead;
nodeActiveEdgePair tActiveEdgePairHead;

//point3d pts[8] = {{0, 0, 0}, {0, 1, 0}, {1, 0, 0}, {1, 1, 0}, 
//		{0, 0, 1}, {0, 1, 1}, {1, 0, 1}, {1, 1, 1}};
point3d pts[8] = {{-2, 3, 0}, {-2, 1, 0}, {2, 1, 0}, {2, 3, 0},
{-1, 4, 1}, {-1, 0, 1}, {1, 0, 1}, {1, 4, 1}};
//GLint iFaces[6][4] = {{1, 3, 2, 0}, {5, 4, 6, 7}, {6, 2, 3, 7}, {5, 1, 0, 4}, {4, 0, 2, 6}, {7, 3, 1, 5}};
GLint iFaces[2][4] = {{0, 1, 2, 3}, {4, 5, 6, 7}};
GLfloat g_zbuffer[SIZE_ZBUFFER_LINE];

void init_g_zbuffer(){
	for(int j = 0; j < SIZE_ZBUFFER_LINE; j++){
		g_zbuffer[j] = -999999.0;
	}
}

void solvePlaneCoffs(GLint icoords[], GLfloat coffs[]){
	//solve the coefficient of the plane specified by icoords and store the answer in coffs
	point3d vec1;
	pt3dSub(&vec1, &pts[icoords[2]], &pts[icoords[1]]);
	point3d vec2;
	pt3dSub(&vec2, &pts[icoords[0]], &pts[icoords[1]]);

	coffs[0] = vec1.y * vec2.z - vec1.z * vec2.y;
	coffs[1] = vec1.z * vec2.x - vec1.x * vec2.z;
	coffs[2] = vec1.x * vec2.y - vec1.y * vec2.x;
	//系数归一
	GLfloat coffs_sum = coffs[0] + coffs[1] + coffs[2];
	coffs[0] = coffs[0] / coffs_sum;
	coffs[1] = coffs[1] / coffs_sum;
	coffs[2] = coffs[2] / coffs_sum;
	coffs[3] = 0.0 - coffs[0] * pts[icoords[0]].x - coffs[1] * pts[icoords[0]].y - coffs[2] * pts[icoords[0]].z;
}


void add2Struct(GLint icoords[]){
	//construct classified polygon and edge table
	//edge table
	COORD_TYPE edge_maxy;
	COORD_TYPE polygon_maxy = pts[0].y, polygon_miny = pts[0].y;
	GLint _i, _pre, _next;
	for(_i = 0; _i < POLYGON; _i++){
		
		nodeClassifiedEdge* nodeCE = (nodeClassifiedEdge*)malloc(sizeof(nodeClassifiedEdge));
		_pre = _i;
		_next = (_i + 1) % POLYGON;
		COORD_TYPE dy = pts[icoords[_pre]].y - pts[icoords[_next]].y;//y方向增量
		COORD_TYPE dx = pts[icoords[_pre]].x - pts[icoords[_next]].x;//x方向增量
		cout<<"polygon:"<<g_polygon_id<<", "<<"sp:"<<icoords[_pre]<<", ep:"<<icoords[_next]<<endl;
		if(dy > 1e-8 || (-dy) > 1e-8)
			nodeCE->dx = (-GLfloat(dx)) / GLfloat(dy);//相邻扫描线的x坐标之差
		else{
			nodeCE->dx = INFINITY;
			nodeCE->e_len = dx + 0.01;
		}
		if(dy > 0){//边的上端点为pts[icoords[_pre]]
			nodeCE->x = pts[icoords[_pre]].x;
			edge_maxy = pts[icoords[_pre]].y;//计算边的最大y值以便更新边所在的多边形的最大y值
			if(edge_maxy > polygon_maxy) polygon_maxy = edge_maxy;//当前边的最大y值大于多边形当前的最大y值，则更新多边形当前的最大y值
			if(pts[icoords[_next]].y < polygon_miny) polygon_miny = pts[icoords[_next]].y;//最小y值(最小y值是为了计算多边形跨越的扫描线数目)
		}
		else{
			nodeCE->x = pts[icoords[_next]].x;
			edge_maxy = pts[icoords[_next]].y;
			if(edge_maxy > polygon_maxy) polygon_maxy = edge_maxy;
			if(pts[icoords[_pre]].y < polygon_miny) polygon_miny = pts[icoords[_pre]].y;
		}
		nodeCE->dy = abs(dy) * 100 + 1;//当前边跨越的扫描线数目
		nodeCE->id = g_polygon_id;
		nodeCE->used = false;
		nodeCE->next = NULL;
		//将当前边加入分类边表

		nodeClassifiedEdge* pCE = tClassifiedEdge[(GLint)(edge_maxy*100)];
		if(!pCE) tClassifiedEdge[(GLint)(edge_maxy*100)] = nodeCE;
		else{
			while(pCE->next) pCE = pCE->next;
			pCE->next = nodeCE;
		}
	}

	//polygon table
	GLfloat coffs[POLYGON];
	solvePlaneCoffs(icoords, coffs);

	nodeClassifiedPolygon* nodeCP = (nodeClassifiedPolygon*)malloc(sizeof(nodeClassifiedPolygon));//多边形表的表结点
	nodeCP->a = coffs[0];
	nodeCP->b = coffs[1];
	nodeCP->c = coffs[2];
	nodeCP->d = coffs[3];
	//color
	nodeCP->color[0] = 0.0;
	nodeCP->color[1] = 0.0;
	nodeCP->color[2] = 0.0;
	nodeCP->color[g_polygon_id%3] = 1.0;
	nodeCP->dy = (polygon_maxy - polygon_miny)*100 + 1;
	nodeCP->id = g_polygon_id ++;
	//nodeCP->del = false;
	nodeCP->next = NULL;
	if(polygon_miny < 0) { //场景的所有物体均在上半平面
		std::cout<<"don't support negative y"<<std::endl;\
		//return;
	}
	nodeClassifiedPolygon* pCP = tClassifiedPolygon[(GLint)(polygon_maxy*100)];
	if(!pCP) tClassifiedPolygon[(GLint)(polygon_maxy*100)] = nodeCP;
	else{
		while(pCP->next) pCP = pCP->next;
		pCP->next = nodeCP;
	}
}

void findEdge(nodeClassifiedEdge** e, COORD_TYPE y, GLint polygon_id){
	//根据polygon_id寻找仍在活化多边形表中的活化边
	//e - 结果边的指针的地址
	//y - 当前扫描线位置
	//polygon_id - 多边形id
	*e = NULL;
	nodeActivePolygon* _pAP = (tActivePolygonHead.next);
	//检查多边形是否还在活化多边形表中
	while(_pAP && _pAP->id != polygon_id) _pAP = _pAP->next;
	if(_pAP != NULL){
		if(!tClassifiedEdge[(GLint)(y*100)]) {
			cout<<"function:findedge accur logic error(1)"<<endl;
			return;
		}
		nodeClassifiedEdge* _pCE = tClassifiedEdge[(GLint)(y*100)];
		while(_pCE && (_pCE->id != polygon_id || ( _pCE->id == polygon_id && _pCE->used)))
			_pCE = _pCE->next;
		if(!_pCE){
			cout<<"function:findedge not find"<<endl;
			return;
		}
		_pCE->used = true;
		*e = _pCE;
	}
}

void findPolygon(nodeActivePolygon** p, int polygon_id){
	*p = tActivePolygonHead.next;
	while(*p && (*p)->id != polygon_id) *p = (*p)->next;
}

void setPixel(COORD_TYPE x, COORD_TYPE y, const GLfloat color[] = g_renderColor){
	glColor3fv(color);
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

void zbuffer(){
	tActivePolygonHead.next = NULL;
	tActiveEdgePairHead.next = NULL;

	nodeActivePolygon* tailAP = NULL;
	nodeActiveEdgePair* tailEP = NULL;
	//scan
	for(GLfloat i = 4.0; i >= 1e-6; i-=0.01){
		init_g_zbuffer();
		cout<<"scanline:"<<i<<endl;
		//测试当前扫描线是否有需要新加入的多边形
		if(tClassifiedPolygon[(GLint)(i*100)]){
			//将当前扫描线扫描到的所有多边形加入活化多边形表
			nodeClassifiedPolygon* pCP = (nodeClassifiedPolygon*)malloc(sizeof(nodeClassifiedPolygon));
			*pCP = *tClassifiedPolygon[(GLint)(i*100)];
			while(pCP){//1个多边形的处理,将一个多边形加入活化多边形表
				if(tActivePolygonHead.next == NULL){
					tActivePolygonHead.next = (nodeActivePolygon*)pCP;//活化多边形表结点的结构和分类多边形表的结构相同
					tailAP = (nodeActivePolygon*)pCP;
				}else{
					tailAP->next = (nodeActivePolygon*)pCP;
					tailAP = tailAP->next;
				}
				//多边形加入活化多边形表以后，其两条边加入活化边表
				nodeClassifiedEdge* pCE = tClassifiedEdge[(GLint)(i*100)];
				while(pCE){
					//分类边表结点结构转化为活化边表结点结构
					//找到边对的左边和右边
					nodeClassifiedEdge *l = pCE, *r = pCE->next;
					if(pCE->dx == INFINITY){//如果是水平直线
						//r = l;
						r = (nodeClassifiedEdge*)malloc(sizeof(nodeClassifiedEdge));
						*r = *l;
						r->x = l->x + r->e_len - 0.01;
						pCE = pCE->next;
					}
					else{
						pCE = pCE->next->next;
					}
					if(l->x > r->x){
						nodeClassifiedEdge* _t = l;
						l = r;
						r = _t;
					}
					l->used = true; r->used = true;
					//构造活化边表结点
					nodeActiveEdgePair* nodeAEP = (nodeActiveEdgePair*)malloc(sizeof(nodeActiveEdgePair));
					nodeAEP->xl = l->x;
					nodeAEP->dxl = l->dx;
					nodeAEP->dyl = l->dy;
					nodeAEP->xr = r->x;
					nodeAEP->dxr = r->dx;
					nodeAEP->dyr = r->dy;
					nodeAEP->zl = (- pCP->d - l->x * pCP->a - i * pCP->b) / pCP->c;
					nodeAEP->dzx = (- pCP->a) / pCP->c;
					nodeAEP->dzy = pCP->b / pCP->c;
					nodeAEP->id = l->id;
					nodeAEP->color = pCP->color;
					nodeAEP->next = NULL;
					//将活化边表结点加入活化边表
					if(tActiveEdgePairHead.next == NULL){
						tActiveEdgePairHead.next = nodeAEP;
					}else{
						//find active edge table's tail
						nodeActiveEdgePair* pAEP = tActiveEdgePairHead.next;
						if(pAEP == NULL) tActiveEdgePairHead.next = nodeAEP;
						else{
							while(pAEP->next) pAEP = pAEP->next;
							pAEP->next = nodeAEP;
						}
					}
				}//while(pCE)
				pCP = pCP->next;
			}//while(pCP)	
		}//if(tClassifiedPolygon[i])
		else if(tClassifiedEdge[(GLint)(i*100)])
		{//是否有需要加入的新边对
			
				nodeClassifiedEdge* pCE = tClassifiedEdge[(GLint)(i*100)];
				while(pCE){
					//分类边表结点结构转化为活化边表结点结构
					//找到边对的左边和右边
					nodeClassifiedEdge *l = pCE, *r = pCE->next;
					if(pCE->dx == INFINITY){//如果是水平直线
						//r = l;
						r = (nodeClassifiedEdge*)malloc(sizeof(nodeClassifiedEdge));
						*r = *l;
						r->x = l->x + r->e_len - 0.01;
						pCE = pCE->next;
						//l->used = true;
					}
					else{
						pCE = pCE->next->next;
					}
					if(l->x > r->x){
						nodeClassifiedEdge* _t = l;
						l = r;
						r = _t;
					}
					l->used = true; r->used = true;
					//构造活化边表结点
					nodeActiveEdgePair* nodeAEP = (nodeActiveEdgePair*)malloc(sizeof(nodeActiveEdgePair));
					nodeAEP->xl = l->x;
					nodeAEP->dxl = l->dx;
					nodeAEP->dyl = l->dy;
					nodeAEP->xr = r->x;
					nodeAEP->dxr = r->dx;
					nodeAEP->dyr = r->dy;

					nodeActivePolygon* pAP;
					findPolygon(&pAP, l->id);

					nodeAEP->zl = (- pAP->d - l->x * pAP->a - i * pAP->b) / pAP->c;
					nodeAEP->dzx = (- pAP->a) / pAP->c;
					nodeAEP->dzy = pAP->b / pAP->c;
					nodeAEP->id = l->id;
					nodeAEP->color = pAP->color;
					nodeAEP->next = NULL;
					//将活化边表结点加入活化边表
					if(tActiveEdgePairHead.next == NULL){
						tActiveEdgePairHead.next = nodeAEP;
					}else{
						//find active edge table's tail
						nodeActiveEdgePair* pAEP = tActiveEdgePairHead.next;
						if(pAEP == NULL) tActiveEdgePairHead.next = nodeAEP;
						else{
							while(pAEP->next) pAEP = pAEP->next;
							pAEP->next = nodeAEP;
						}
					}
				}//while(pCE)
		}
		////测试是否有边对处理结束
		//{
		//	nodeActiveEdgePair* pAEP = tActiveEdgePairHead.next;
		//	while(pAEP){
		//		/*if(pAEP->dyl <= 0 && pAEP->dyr <= 0){}
		//		else */if(pAEP->dyl <= 0){
		//			nodeClassifiedEdge* pCE;
		//			findEdge(&pCE, i, pAEP->id);
		//			if(pCE != NULL){//pAEP不为空表示多边形仍在活化多边形行表中
		//				pAEP->xl = pCE->x;
		//				pAEP->dxl = pCE->dx;
		//				pAEP->dyl = pCE->dy;
		//			}
		//		}
		//		/*else*/ if(pAEP->dyr <= 0){
		//			nodeClassifiedEdge* pCE;
		//			findEdge(&pCE, i, pAEP->id);
		//			pAEP->xr = pCE->x;
		//			pAEP->dxr = pCE->dx;
		//			pAEP->dyr = pCE->dy;
		//		}
		//		pAEP = pAEP->next;
		//	}
		//}
		{//增量式深度更新
			COORD_TYPE y = i;
			nodeActiveEdgePair* pAEP = tActiveEdgePairHead.next;
			while(pAEP){
				GLfloat zx = pAEP->zl;
				COORD_TYPE x = pAEP->xl;
				int p_x = (((int)(x * 10))*10 + BUFFER_OFFSIT_X*100);
				int p_xr = (((int)(pAEP->xr * 10))*10 + BUFFER_OFFSIT_X*100);
				while(p_x <= p_xr){
					if(zx > g_zbuffer[p_x]){
						g_zbuffer[p_x] = zx;
						cout<<"x:"<<p_x<<", y:"<<y<<endl;
						setPixel(p_x, y, pAEP->color);
					}
					zx += pAEP->dzx;
					p_x++;
				}
				pAEP = pAEP->next;
			}
		}
		{//活化边表元素修改
			nodeActiveEdgePair* pAEP_pre = &tActiveEdgePairHead;
			nodeActiveEdgePair* pAEP = pAEP_pre->next;
			while(pAEP){
				pAEP->dyl --;
				pAEP->dyr --;
				if(pAEP->dyl <= 0 && pAEP->dyr <= 0){
					//左右两边同时扫描完，接下来找到的边或者是空边，或者是一条水平边
					nodeClassifiedEdge* pCE;
					findEdge(&pCE, i, pAEP->id);
					if(pCE){//水平边
						nodeClassifiedEdge *l, *r;
						l = pCE;
						r = (nodeClassifiedEdge*)malloc(sizeof(nodeClassifiedEdge));
						*r = *l;
						r->x = l->x + r->e_len - 0.01;

						nodeActivePolygon* pAP = tActivePolygonHead.next;
						while(pAP && pAP->id != l->id) pAP = pAP->next;
						pAEP->xl = l->x;
						pAEP->dxl = l->dx;
						pAEP->dyl = l->dy;
						pAEP->xr = r->x;
						pAEP->dxr = r->dx;
						pAEP->dyr = r->dy;
						pAEP->zl = (- pAP->d - l->x * pAP->a - i * pAP->b) / pAP->c;
						pAEP->dzx = (- pAP->a) / pAP->c;
						pAEP->dzy = pAP->b / pAP->c;
						pAEP->id = l->id;
						//nodeAEP->next = NULL;
					}
				}
				if(pAEP->dyl <= 0){//左边扫描完
					nodeClassifiedEdge* pCE;
					findEdge(&pCE, i, pAEP->id);
					if(pCE){//pAEP不为空表示多边形仍在活化多边形行表中
						pAEP->xl = pCE->x;
						pAEP->dxl = pCE->dx;
						pAEP->dyl = pCE->dy;
					}
					else{
						pAEP_pre->next = pAEP->next;
						free(pAEP);
						pAEP = pAEP_pre->next;
						continue;
					}
				}
				else{//左边未扫描完
					pAEP->xl += pAEP->dxl;
					pAEP->zl = pAEP->zl + pAEP->dxl * pAEP->dzx + pAEP->dzy;
				}
				
				if(pAEP->dyr <= 0){//右边扫描完
					nodeClassifiedEdge* pCE;
					findEdge(&pCE, i, pAEP->id);
					if(pCE){
						pAEP->xr = pCE->x;
						pAEP->dxr = pCE->dx;
						pAEP->dyr = pCE->dy;
					}
					else{
						pAEP_pre->next = pAEP->next;
						free(pAEP);
						pAEP = pAEP_pre->next;
						continue;
					}
				}
				else{//右边未扫描完
					pAEP->xr += pAEP->dxr;
				}
				pAEP_pre = pAEP;
				pAEP = pAEP->next;
			}
		}
		{//更新活化多边形表
			nodeActivePolygon* pAP = tActivePolygonHead.next;
			nodeActivePolygon* pAP_pre = &tActivePolygonHead;
			while(pAP){
				pAP->dy = pAP->dy - 1;
				if(pAP->dy <= 0){
					pAP_pre->next = pAP->next;
					free(pAP);
					pAP = pAP_pre->next;
				}
				else{
					pAP_pre = pAP;
					pAP = pAP_pre->next;
				}
			}
		}
	}
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	//glPointSize(3);
	GLfloat _c[] = {1.0, 1.0, 1.0};
	setPixel(0, 0, _c);

	//glRotatef(g_rotate, 0, 1, 0);
	//g_rotate += 0.1;
	//if(g_rotate >= 180) g_rotate = 0;

	for(GLint i = 0; i < 2; i++){
		GLint* icoords= iFaces[i];
		/*glBegin(GL_QUADS);
		glVertex3i(pts[icoords[0]].x, pts[icoords[0]].y, pts[icoords[0]].z);
		glVertex3i(pts[icoords[1]].x, pts[icoords[1]].y, pts[icoords[1]].z);
		glVertex3i(pts[icoords[2]].x, pts[icoords[2]].y, pts[icoords[2]].z);
		glVertex3i(pts[icoords[3]].x, pts[icoords[3]].y, pts[icoords[3]].z);
		glEnd();*/
	#ifdef ZBUFFER
		add2Struct(icoords);
	#endif
	}

#ifdef ZBUFFER
	zbuffer();
#endif
#ifdef ZBUFFER //清空数据结构
	{//清空活化多边形表
		nodeActivePolygon *p = tActivePolygonHead.next, *q;
		while(p){
			q = p->next;
			free(p);
			p = q;
		}
		tActivePolygonHead.next = NULL;
	}
	{//清空活化边表
		nodeActiveEdgePair *p = tActiveEdgePairHead.next, *q;
		while(p){
			q = p->next;
			free(p);
			p = q;
		}
		tActiveEdgePairHead.next = NULL;
	}
	{
		g_polygon_id = 0;
	}
	void deleteStruct();
	deleteStruct();
#endif

	glutSwapBuffers();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    //gluPerspective(45.0, 1, 1, 100);
	//gluLookAt(0, 0, 5, 0, 0, 0, 0, 1.0, 0);
	gluOrtho2D(-10, 10, -10, 10);
	//gluLookAt(1, 0, 5, 0, 0, 0, 0, 1.0, 0);
    glMatrixMode(GL_MODELVIEW);

	glColor3f(1.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//glEnable(GL_CULL_FACE);
}

void deltClassifiedPolygon(){
	nodeClassifiedPolygon *p, *q; int _cnt = 0;
	for(GLint i = 0; i < 1000; i++){
		if(tClassifiedPolygon[i]){
			p = tClassifiedPolygon[i];
			while(p){
				q = p;
				p = p->next;
				free(q);
				_cnt++;
			}
			//free(p);
			tClassifiedPolygon[i] = NULL;
		}
	}
	cout<<"del cnt: " <<_cnt <<endl;
}

void deltClassifiedEdge(){
	nodeClassifiedEdge *p, *q;
	for(GLint i = 0; i < 1000; i++){
		if(tClassifiedEdge[i]){
			p = tClassifiedEdge[i];
			while(p->next){
				q = p;
				p = p->next;
				free(q);
			}
			free(p);
			tClassifiedEdge[i] = NULL;
		}
	}
}

void deleteStruct(){
	deltClassifiedPolygon();
	deltClassifiedEdge();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(400, 400);
    glutInitWindowSize(800, 800);
    glutCreateWindow(argv[0]);
	init();

    glutDisplayFunc(display);
    glutIdleFunc(display);
	
    glutMainLoop();
    return 0;
}