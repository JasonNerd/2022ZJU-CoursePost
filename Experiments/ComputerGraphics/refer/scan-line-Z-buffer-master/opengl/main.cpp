#include "main.h"


bool Collinear(Triple<GLdouble>& a, Triple<GLdouble>& b){//�����жϣ����߷���true�����򷵻�false
	if(b == a) return true;
	Triple<GLdouble> z0(0.0,0.0,0.0);
	if(b == z0 || a == z0) return true;
	GLdouble r = 0.0;
	GLdouble costheta = (b.x*a.x+b.y*a.y+b.z*a.z+0.0)/sqrt((b.x*b.x+b.y*b.y+b.z*b.z)*(a.x*a.x+a.y*a.y+a.z*a.z));
	if(abs(costheta-1)<1e-8 || abs(costheta+1)<1e-8) return true;
	return false;
}

void init_g_zbuffer(){
	g_zbuffer.clear();
	g_zbuffer.assign(g_winWidth, -999999.0);
}

void solveFaceCoffs(vector<GLdouble>& coffs, face f){
	//solve the coefficient of the plane specified by icoords and store the answer in coffs
	Triple<GLdouble> vec1;
	vec1 = f[2] - f[1];
	Triple<GLdouble> vec2;
	vec2 = f[0] - f[1];
	//����ϵ��
	coffs[0] = vec1.y * vec2.z - vec1.z * vec2.y;
	coffs[1] = vec1.z * vec2.x - vec1.x * vec2.z;
	coffs[2] = vec1.x * vec2.y - vec1.y * vec2.x;
	//ϵ����һ
	GLfloat coffs_abssum = abs(coffs[0]) + abs(coffs[1]) + abs(coffs[2]);
	//_ASSERT(coffs_abssum != 0.0);
	if(coffs_abssum == 0.0){
		#ifdef DEBUGGER
		cout<<"normal vector is 0."<<endl;
		#endif
		coffs_abssum = 1.0;
	}
	coffs[0] = coffs[0] / coffs_abssum;
	coffs[1] = coffs[1] / coffs_abssum;
	coffs[2] = coffs[2] / coffs_abssum;
	//����ؾ�
	coffs[3] = 0.0 - coffs[0] * f[0].x - coffs[1] * f[0].y - coffs[2] * f[0].z;
}

bool preProcessf(face& f){//
	//����f������Ԥ����
	//1.������С��0��x&y������нضϣ���С��0����0
	//2.����������겻�ܹ����ı��Σ��򷵻�false�����򷵻�true
	bool flag = false;
	for(GLuint i = 0; i < f.size(); i++){
		if(f[i].x < 0) f[i].x = 0;
		if(f[i].y < 0) f[i].y = 0;
		if(f[i].z < 0) f[i].z = 0;
	}
	//���ж�����Ĭ���ĵ㹲�棬�ж������Ƿ����غ�
	//Triple<GLint> vec1 = f[1] - f[0];
	//Triple<GLint> vec2 = f[2] - f[1];
	//Triple<GLint> vec3 = f[3] - f[2];
	//Triple<GLint> vec4 = f[0] - f[3];
	//Triple<GLint> vecZero(0,0,0);
	//if((vec1!=vecZero)&&(vec2!=vecZero)&&(vec3!=vecZero)&&(vec4!=vecZero)&&
	//	(vec1+vec2+vec3+vec4)==vecZero&&
	//	!vec1.Collinear(vec2)&&!vec2.Collinear(vec3)&&!vec3.Collinear(vec4)&&!vec4.Collinear(vec1))
	//	flag = true;
	return flag;
}

Triple<GLubyte> getPolygonColor(vector<GLdouble> coffs){
	//����ƽ�淨������z�ᣨ���������򣩵ļн�
	//�н���С������ɫ��������
	if(coffs[0] < 0) coffs[0] = 0-coffs[0];
	if(coffs[1] < 0) coffs[1] = 0-coffs[1];
	if(coffs[2] < 0) coffs[2] = 0-coffs[2];
	GLdouble costheta = coffs[2] / sqrt(coffs[0]+coffs[1]+coffs[2]);
	return Triple<GLubyte>(255*costheta,255*costheta,255*costheta);
}

Point roundVertex(Triple<GLdouble> v){//���㸡�㶥��v�����������p
	Point p;
	p.x = v.x + ((v.x < 0)?-0.5:0.5);
	p.y = v.y + ((v.y < 0)?-0.5:0.5);
	p.z = v.z;
	return p;
}

void constructDS(vector<face>& vface){
	//construct classified polygon and edge table
	//edge table
	for(GLuint i = 0; i < vface.size(); i++){
		g_polygon_id++;//ÿ�������ۻ������Ҫ��ţ���ֹ��Ż���
		//if(g_polygon_id == 129)
		//	g_polygon_id =g_polygon_id;
		//cout<<g_polygon_id<<endl;
		//if(!preProcessf(vface[i])) continue;
		//���ν��ÿ����
		vector<GLdouble> coffs(4);
		solveFaceCoffs(coffs, vface[i]);
		if(coffs[2] < 1e-8 && coffs[2] > -1e-8) continue;//��ֱ��ͶӰxOy����治����

		int polygon_maxy = (vface[i][0].y>0?vface[i][0].y:0), polygon_miny = polygon_maxy; 
		for(GLuint j = 0; j < vface[i].size(); j++){
			//���ν��ÿ�����ÿ����
			Point a = roundVertex(vface[i][j]);
			Point b = roundVertex(vface[i][(j+1)%vface[i].size()]);//%--���һ����͵�һ����Ҳ�ܹ��ɱ�
			//a.x += 
			if(a.y < b.y){//��a��Ϊ��ǰ����ߵ��϶���
				Point t = a;
				a = b;
				b = t;
			}
			if(a.y < 0) continue;//ˮƽ���µı߲�����
			if(a.y - b.y < 1e-8 && a.y - b.y > -1e-8) continue;//ˮƽ�߲����ǣ���Ӱ����������Ϊ���Ʒ�ˮƽ�ߵ�ʱ���Ѿ�������ˮƽ��
			
			//�������߱���
			nodeClassifiedEdge* pCE = (nodeClassifiedEdge*)malloc(sizeof(nodeClassifiedEdge));
			pCE->x = a.x;
			pCE->dx = (b.x - a.x + .0) / (a.y - b.y);//��������ɨ���߽����x�����dx (-1/k)
			pCE->dy = a.y - b.y + 1;
			pCE->id = g_polygon_id;
			pCE->used = false;
			pCE->next = NULL;
			//�������߱�
			if(a.y >= tClassifiedEdge.size()) continue;
			if(tClassifiedEdge[a.y] == NULL) {
				tClassifiedEdge[a.y] = pCE;
			}else{
				nodeClassifiedEdge* p = tClassifiedEdge[a.y];
				while(p->next) p = p->next;
				p->next = pCE;
			}
			if(a.y > polygon_maxy) polygon_maxy = a.y;
			if(b.y < polygon_miny) polygon_miny = b.y;
		}//end for(int j = 0;...)

		nodeClassifiedPolygon* pCP = (nodeClassifiedPolygon*)malloc(sizeof(nodeClassifiedPolygon));		
		pCP->a = coffs[0];
		pCP->b = coffs[1];
		pCP->c = coffs[2];
		pCP->d = coffs[3];
		pCP->dy = polygon_maxy - polygon_miny + 1;
		pCP->id = g_polygon_id;
		pCP->color = getPolygonColor(coffs);
		pCP->next = NULL;
		if(polygon_maxy >=  tClassifiedPolygon.size()) continue;
		if(tClassifiedPolygon[polygon_maxy] == NULL){
			tClassifiedPolygon[polygon_maxy] = pCP;
		}else{
			nodeClassifiedPolygon* p = tClassifiedPolygon[polygon_maxy];
			while(p->next) p = p->next;
			p->next = pCP;
		}		
	}
}

void findEdge(nodeClassifiedEdge** e, GLint y, GLint polygon_id){
	//����polygon_idѰ�����ڻ����α��еĻ��
	//e - ����ߵ�ָ��ĵ�ַ
	//y - ��ǰɨ����λ��
	//polygon_id - �����id
	*e = NULL;
	nodeActivePolygon* _pAP = (tActivePolygonHead.next);
	//��������Ƿ��ڻ����α���
	while(_pAP && (_pAP->id != polygon_id || _pAP->dy <= 1)) //_pAP->dy <= 1 �����Ķ���μ������Ƴ�������Բ��迼��
		_pAP = _pAP->next;
	if(_pAP != NULL){//����λ��ڻ����
		if(!tClassifiedEdge[y]) {
			//����������Ҳ�п���������������Ϊ�п���ɨ�赽�������ײ�ʱ������λ�û���Ƴ�
			//�����α�
			#ifdef DEBUGGER
			cout<<"function:findedge accur logic error(1)"<<endl;
			#endif
			return;
		}
		nodeClassifiedEdge* _pCE = tClassifiedEdge[y];
		while(_pCE && (_pCE->id != polygon_id || _pCE->used))
			_pCE = _pCE->next;
		if(!_pCE){
			#ifdef DEBUGGER
			cout<<"function:findedge not find"<<endl;
			#endif
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

void activateNewPolygon(GLint y){
	//std::cout<<y<<std::endl;
	if(y < 0 || y >= tClassifiedPolygon.size() ) return;
	if(tClassifiedPolygon[y]){
	//����ǰɨ����ɨ�赽�����ж���μ�������α�
		nodeClassifiedPolygon* pCP = tClassifiedPolygon[y];
		while(pCP){//1������εĴ���,��һ������μ�������α�(��ֱ��xOy���ƽ�治����)
			//if(pCP->c < 1e-6 && pCP->c > -1e-6){//��һ�α�֤��ֱ��xOy���ƽ�治����
			//	pCP = pCP->next;
			//	continue;
			//}
			//���ƽ�治��ֱ��xOy�棬��������������α�
				
			nodeClassifiedPolygon *t_pCP = (nodeClassifiedPolygon*)malloc(sizeof(nodeClassifiedPolygon));
			*t_pCP = *pCP;
			pCP = t_pCP;//pcp��nextָ����Ȼ����ָ��������α��ָ��
			t_pCP = pCP->next;
			pCP->next = NULL;
			nodeActivePolygon *pAP = tActivePolygonHead.next;
			if(!pAP) tActivePolygonHead.next = (nodeActivePolygon*)pCP;
			else{
				while(pAP->next) pAP = pAP->next;
				pAP->next = (nodeActivePolygon*)pCP;
			}
			//if(tActivePolygonHead.next == NULL){
			//	tActivePolygonHead.next = (nodeActivePolygon*)pCP;//�����α���Ľṹ�ͷ������α�Ľṹ��ͬ
			//	tailAP = (nodeActivePolygon*)pCP;
			//}else{
			//	tailAP->next = (nodeActivePolygon*)pCP;
			//	tailAP = tailAP->next;
			//}
			//����μ�������α��Ժ��������߼����߱�
			//nodeClassifiedEdge* pCE = tClassifiedEdge[i];
			//while(pCE){
			//����߱���ṹת��Ϊ��߱���ṹ
			//�ҵ��߶Ե���ߺ��ұ�
				
			nodeClassifiedEdge *l, *r;//�˴����ܳ���bug
			findEdge(&l, y, pCP->id);
			findEdge(&r, y, pCP->id);
			if(!l || !r) {
				#ifdef DEBUGGER
				cout<<__LINE__<<":find a polygon, but not find it's corresponding l & r edges."<<endl;
				#endif
				nodeClassifiedPolygon* next_pCP = pCP->next;
				pCP->next = NULL;
				pCP = next_pCP;
				continue;
			}
			if(l->x > r->x || (l->x == r->x && l->dx > r->dx)){
				nodeClassifiedEdge* _t = l;
				l = r;
				r = _t;
			}
			//l->used = true; r->used = true;
			//�����߱���
			nodeActiveEdgePair* pAEP = (nodeActiveEdgePair*)malloc(sizeof(nodeActiveEdgePair));
			pAEP->xl = l->x;
			pAEP->dxl = l->dx;
			pAEP->dyl = l->dy;
			pAEP->xr = r->x;
			pAEP->dxr = r->dx;
			pAEP->dyr = r->dy;
			pAEP->zl = (- pCP->d - l->x * pCP->a - y * pCP->b) / pCP->c;
			pAEP->dzx = (- pCP->a) / pCP->c;
			pAEP->dzy = pCP->b / pCP->c;
			pAEP->id = l->id;
			pAEP->color = pCP->color;
			pAEP->next = NULL;
			//����߱�������߱�
			if(tActiveEdgePairHead.next == NULL){
				tActiveEdgePairHead.next = pAEP;
			}else{
				//find active edge table's tail
				nodeActiveEdgePair* _pAEP = tActiveEdgePairHead.next;
				if(_pAEP == NULL) tActiveEdgePairHead.next = pAEP;
				else{
					while(_pAEP->next) _pAEP = _pAEP->next;
					_pAEP->next = pAEP;
				}
			}

			//��������ε��ϱ߽���
			GLdouble zx = pAEP->zl;
			int x = pAEP->xl ;
			//����Ѱ���ϱ߽����߽�
			while(x < 0){
				zx += pAEP->dzx;
				x ++;
			}
			while(x < pAEP->xr){//�˴������Ǳ߽�
				if(x < g_zbuffer.size() && zx > g_zbuffer[x]){
					g_zbuffer[x] = zx;
					//cout<<"x:"<<x<<", y:"<<y<<endl;
					frameBuffer.SetPixel(x, y, g_bgColor);
				}
				zx += pAEP->dzx;
				x ++;
			}
			//}//while(pCE)
			//����ӷ������α��п���������ָ��next
			//nodeClassifiedPolygon* next_pCP = pCP->next;
			//pCP->next = NULL;
			pCP = t_pCP;
		}//while(pCP)	
	}//if(tClassifiedPolygon[i])
}

void deepthUpdate(GLint y){
	//����ʽ��ȸ���
	nodeActiveEdgePair* pAEP = tActiveEdgePairHead.next;
	while(pAEP){
		GLdouble zx = pAEP->zl;
		int x = pAEP->xl ;
		//init_g_zbuffer();

		//���ұ߽��ű���ɫ
		//����Ѱ����߽�
		while(x < 0){
			zx += pAEP->dzx;
			x ++;
		}
		//��߽���ɫ
		if(x < g_zbuffer.size() && zx > g_zbuffer[x]){
			g_zbuffer[x] = zx;
			frameBuffer.SetPixel(x, y, g_bgColor);
		}
		zx += pAEP->dzx;
		x ++;
		//��߽���ɫ���

		while(x < g_zbuffer.size() && x </*=*/ pAEP->xr){//�˴������Ǳ߽�
			if(zx > g_zbuffer[x]){
				g_zbuffer[x] = zx;
				//cout<<"x:"<<x<<", y:"<<y<<endl;
				frameBuffer.SetPixel(x, y, pAEP->color);
			}
			zx += pAEP->dzx;
			x ++;
		}

		//�ұ߽���ɫ
		//if(zx > g_zbuffer[x]){
		//	g_zbuffer[x] = zx;
		//	frameBuffer.SetPixel(x, y, g_bgColor);
		//}
		//�ұ߽���ɫ���

		pAEP = pAEP->next;
	}
}

void activeEdgeTableUpdate(GLint y){//��߱�Ԫ���޸�
	nodeActiveEdgePair* pAEP_pre = &tActiveEdgePairHead;
	nodeActiveEdgePair* pAEP = pAEP_pre->next;
	while(pAEP){
		pAEP->dyl --;
		pAEP->dyr --;
		if(pAEP->dyl <= 0 && pAEP->dyr <= 0){//��������ͬʱɨ����
			nodeClassifiedEdge *l, *r;
			findEdge(&l, y, pAEP->id);
			if(l == NULL){
				pAEP_pre->next = pAEP->next;
				free(pAEP);
				pAEP = pAEP_pre->next;
			}else{//����ҵ�����ߣ����ݶ���εķ���ԣ�һ�������ҵ��ұ�
				findEdge(&r, y, pAEP->id);
				if(l->x > r->x || (l->x == r->x && l->dx > r->dx)){
					nodeClassifiedEdge* _t = l;
					l = r;
					r = _t;
				}
				nodeActivePolygon *pAP;
				findPolygon(&pAP, pAEP->id);
				pAEP->xl = l->x;
				pAEP->dxl = l->dx;
				pAEP->dyl = l->dy;
				pAEP->xr = r->x;
				pAEP->dxr = r->dx;
				pAEP->dyr = r->dy;
				pAEP->zl = (- pAP->d - l->x * pAP->a - y * pAP->b) / pAP->c;
				pAEP->dzx = (- pAP->a) / pAP->c;
				pAEP->dzy = pAP->b / pAP->c;

				pAEP_pre = pAEP;
				pAEP = pAEP->next;
			}
		}
		else{
			if(pAEP->dyl <= 0){//���ɨ����
				nodeClassifiedEdge* l;
				findEdge(&l, y, pAEP->id);
				if(l){//pAEP��Ϊ�ձ�ʾ��������ڻ������б���
					//nodeActivePolygon *pAP;
					//findPolygon(&pAP, pAEP->id);
					pAEP->xl = l->x;
					pAEP->dxl = l->dx;
					pAEP->dyl = l->dy;
					//pAEP->zl = (- pAP->d - l->x * pAP->a - i * pAP->b) / pAP->c;
					//pAEP->dzx = (- pAP->a) / pAP->c;
					//pAEP->dzy = pAP->b / pAP->c;
				}
				else{
					pAEP_pre->next = pAEP->next;
					free(pAEP);
					pAEP = pAEP_pre->next;
					continue;
				}
			}
			else{//���δɨ����
				pAEP->xl += pAEP->dxl;
				pAEP->zl = pAEP->zl + pAEP->dxl * pAEP->dzx + pAEP->dzy;
			}
		
			if(pAEP->dyr <= 0){//�ұ�ɨ����
				nodeClassifiedEdge* r;
				findEdge(&r, y, pAEP->id);
				if(r){
					pAEP->xr = r->x;
					pAEP->dxr = r->dx;
					pAEP->dyr = r->dy;
				}
				else{
					pAEP_pre->next = pAEP->next;
					free(pAEP);
					pAEP = pAEP_pre->next;
					continue;
				}
			}
			else{//�ұ�δɨ����
				pAEP->xr += pAEP->dxr;
			}
			pAEP_pre = pAEP;
			pAEP = pAEP->next;
		}
	}	
}

void activePolygonTableUpdate(){//���»����α�
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

void zbuffer(){
	tActivePolygonHead.next = NULL;
	tActiveEdgePairHead.next = NULL;

	//nodeActivePolygon* tailAP = NULL;
	//nodeActiveEdgePair* tailEP = NULL;
	//scan
	for(GLint y = g_winHeight-1; y >= 0; y--){
		init_g_zbuffer();
		activateNewPolygon(y);
		deepthUpdate(y);
		activeEdgeTableUpdate(y);//��߱�Ԫ���޸�
		activePolygonTableUpdate();//���»����α�
	}//end for(int i = 4;...)
	
}

void getFrameBuffer(){
	vector<face> vface;
	Triple<GLdouble> p(1,1,1);
	g_polygon_id = 0;
	if(bRotate){
		vface = obj.getObj().rotateObj(1,p).vfs;		
	}
	else
		vface = obj.getObj().vfs;
	//if(!dataValid){
	constructDS(vface);
	//	dataValid = true;
	//}
	zbuffer();
	clearData();
}

void displayInfo(){
	time_t cur_end = clock();
	frameCount++;

	GLfloat fps = frameCount * CLOCKS_PER_SEC / (cur_end - t_start + 0.0);
	GLint nVer = obj.vVertex.size()-1;
	GLint nface = obj.vfs.size();

	system("cls");
	printf("������:%d\n��Ƭ��:%d\n֡��:%.2f\n", nVer, nface, fps);
}

void display(void)
{
	/*if(times++ <= 10000) return;
	times = 0;*/
    glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/);
	//glDrawBuffer(GL_FRONT_AND_BACK);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	frameBuffer.Memset(g_bgColor.x);

	getFrameBuffer();

	glRasterPos2i(0,0);
	glDrawPixels(frameBuffer.getBufferWidth(), frameBuffer.getBufferHeight(), 
	GL_RGB, GL_UNSIGNED_BYTE, frameBuffer.getPixelAddr(0,0));
	glutSwapBuffers();

	displayInfo();
}

void clearData(){
	clearActivePolygonTable();//��ջ����α�
	clearActiveEdgeTable();//��ջ�߱�
	clearClassifiedPolygon();
	clearClassifiedEdge();
}

void init()
{
    glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,g_winWidth<<6,0,g_winHeight<<6);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	g_zbuffer.clear();
	g_zbuffer.assign(g_winWidth<<6, -999999.0);

	frameBuffer.ResizeBuffer(g_winWidth, g_winHeight);

	tClassifiedPolygon.clear();
	tClassifiedPolygon.assign(g_winHeight<<6, NULL);

	tClassifiedEdge.clear();
	tClassifiedEdge.assign(g_winHeight<<6, NULL);
}

void clearActiveEdgeTable(){//��ջ�߱�
	nodeActiveEdgePair *p = tActiveEdgePairHead.next, *q;
	while(p){
		q = p->next;
		free(p);
		p = q;
	}
	tActiveEdgePairHead.next = NULL;
}

void clearActivePolygonTable(){//��ջ����α�
	nodeActivePolygon *p = tActivePolygonHead.next, *q;
	while(p){
		q = p->next;
		free(p);
		p = q;
	}
	tActivePolygonHead.next = NULL;
}

void clearClassifiedPolygon(){
	nodeClassifiedPolygon *p, *q; /*int _cnt = 0;*/
	for(GLuint i = 0; i < tClassifiedPolygon.size(); i++){
		if(tClassifiedPolygon[i]){
			p = tClassifiedPolygon[i];
			while(p){
				q = p->next;
				free(p);
				p = q;
				//_cnt++;
			}
			//free(p);
			tClassifiedPolygon[i] = NULL;
		}
	}
	//cout<<"del cnt: " <<_cnt <<endl;
}

void clearClassifiedEdge(){
	nodeClassifiedEdge *p, *q;
	for(GLuint i = 0; i < tClassifiedEdge.size(); i++){
		if(tClassifiedEdge[i]){
			p = tClassifiedEdge[i];
			while(p){
				q = p->next;
				free(p);
				p = q;
			}
			tClassifiedEdge[i] = NULL;
		}
	}
}

void reshape(GLsizei width,GLsizei height) {
	g_winWidth = width;
	g_winHeight = height;
	init();
	display();
}

void myMouse(int button,int state,int x,int y)
{
	bool smaller = true;
	switch (button)
	{
	case GLUT_LEFT_BUTTON://��������ק
		switch (state)
		{
		case GLUT_DOWN:
			//cout<<"GLUT_DOWN:"<<" x:"<<x<<" ,y:"<<y<<endl;
			mouseDownX = x;
			mouseDownY = y;
			mouseDataValid = true;
			break;
		case GLUT_UP:
			//cout<<"GLUT_UP:"<<" x:"<<x<<" ,y:"<<y<<endl;
			if(!mouseDataValid) return;
			mouseDataValid = false;
			//Triple<GLdouble> p1(x,mouseDownY,60);//��Ļ����ϵ�뻺������ϵ�ǶԳƵ�
			//Triple<GLdouble> p2(mouseDownX,y,0);
			obj.translateObj(Triple<GLdouble>(x-mouseDownX,mouseDownY-y,0));
			display();
			break;
		default:
			break;
		}
		break;
	case GLUT_MIDDLE_BUTTON://������ʱû��ʵ��
		switch (state)
		{
		case GLUT_DOWN:
			mouseMidDownX = x;
			mouseMidDownY = y;
			mouseMidDataValid = true;
			break;
		case GLUT_UP:
			if(!mouseMidDataValid) return;
			mouseMidDataValid = false;
			if(y > mouseMidDownY) smaller = true;
			else smaller = false;
			obj.scaleObj(Triple<GLdouble>(x,mouseDownY,0),Triple<GLdouble>(mouseDownX,y,0),smaller);
			display();
		default:
			break;
		}
		//cout<<"GLUT_MIDDLE_BUTTON:"<<" x:"<<x<<" ,y:"<<y<<endl;
		break;
	case GLUT_RIGHT_BUTTON:
		bRotate = !bRotate;
		display();
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);
	glutInitWindowSize(g_winWidth, g_winHeight);
	
    glutCreateWindow("zbuffer");
	init();

	//��ʼ��ʱ
	t_start = clock();
	frameCount = 0;//֡��ͳ��


    glutDisplayFunc(display);
	glutReshapeFunc(reshape);
    glutIdleFunc(display);
	glutMouseFunc(myMouse);
	//glutPassiveMotionFunc(myMouse);

    glutMainLoop();
    return 0;
}