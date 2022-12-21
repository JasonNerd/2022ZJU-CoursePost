
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include <gl/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <time.h>
#include "Triple.h"
#include "Obj.h"
#include "FrameBuffer.h"
//#define DEBUGGER

using namespace std;
//typedef GLint COORD_TYPE; 
typedef vector<Triple<GLdouble>> face;
//typedef Triple<GLint> Point;
typedef struct{
	GLint x,y;
	GLdouble z;
} Point;

typedef struct nodeClassifiedPolygon{
	GLdouble a, b, c, d;
	GLint id;
	GLint dy;//跨越的扫描线数目
	Triple<GLubyte> color;
	nodeClassifiedPolygon* next;
}nodeClassifiedPolygon;

typedef struct nodeClassifiedEdge{
	GLdouble x;
	GLdouble dx;
	//COORD_TYPE e_len;//边是水平边时，e_len为非0值，否则e_len=0
	GLint dy;
	GLint id;
	bool used;
	nodeClassifiedEdge* next;
}nodeClassifiedEdge;

typedef struct nodeActivePolygon{
	GLdouble a, b, c, d;
	GLint id;
	GLint dy;
	Triple<GLubyte> color;
	nodeActivePolygon* next;
}nodeActivePolygon;

typedef struct nodeActiveEdgePair{
	GLdouble xl;
	GLdouble dxl;//相邻扫描线交点x坐标之差
	GLint dyl;//靠左的边跨越的扫描线数目
	GLdouble xr;
	GLdouble dxr;
	GLint dyr;
	GLdouble zl;
	GLdouble dzx;
	GLdouble dzy;
	GLint id;
	Triple<GLubyte> color;
	nodeActiveEdgePair* next;
}nodeActiveEdgePair;

/*分类多边形表，元素是多边形指针*/
vector<nodeClassifiedPolygon*> tClassifiedPolygon;
vector<nodeClassifiedEdge*> tClassifiedEdge;
nodeActivePolygon tActivePolygonHead;
nodeActiveEdgePair tActiveEdgePairHead;

FrameBuffer frameBuffer;
int g_winHeight = 900, g_winWidth = 1600;
Obj obj;
GLfloat g_rotate = 0;
int times = 0;
int g_polygon_id = 0;
Triple<GLubyte> g_renderColor(255, 255, 255);
Triple<GLubyte> g_bgColor(0, 0, 0);
vector<GLfloat> g_zbuffer;

/*
	move
**/
GLint mouseDownX, mouseDownY, mouseUpX, mouseUpY;
GLint mouseMidDownX, mouseMidDownY, mouseMidUpX, mouseMidUpY;
bool mouseDataValid = false;
bool mouseMidDataValid = false;
/*
	scale
**/

/*
	rotate
**/
bool bRotate = false;
//bool dataValid = false;
/*
	fps
**/
clock_t t_start;
long frameCount;
void displayInfo();

void deleteStruct();
void clearData();
void activateNewPolygon(GLint y);
void deepthUpdate(GLint y);
void activeEdgeTableUpdate(GLint y);
void activePolygonTableUpdate();
void zbuffer();
void constructDS(vector<face>& vface);
bool Collinear(Triple<GLdouble>& a, Triple<GLdouble>& b);
void init_g_zbuffer();
void solveFaceCoffs(vector<GLdouble>& coffs, face f);
bool preProcessf(face& f);
Triple<GLubyte> getPolygonColor(vector<GLdouble> coffs);
void findEdge(nodeClassifiedEdge** e, GLint y, GLint polygon_id);
void findPolygon(nodeActivePolygon** p, int polygon_id);
void display(void);
void clearData();
void init();
void clearClassifiedPolygon();
void clearClassifiedEdge();
//void deleteStruct();
void clearActivePolygonTable();//清空活化多边形表
void clearActiveEdgeTable();//清空活化边表
void getFrameBuffer();
void reshape(GLsizei width,GLsizei height);
void myMouse(int button,int state,int x,int y);
Point roundVertex(Triple<GLdouble>);