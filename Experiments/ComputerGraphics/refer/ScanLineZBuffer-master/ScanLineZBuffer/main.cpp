#include <iostream>
#include "Vec.h"
#include "utils.h"
#include "Model.h"
#include "Shader.h"
#include "ScanLineZBuffer.h"
#include "Display.h"
#include <ctime>


using namespace std;

int main()
{
	
	Model model("../models/al.obj");
	//���ڴ�С
	int width = 800, height = 600;
	//int width = 400, height = 300;

	//ɨ����zbuffer����õ���Ҫ���л��Ƶ���Ļ�����Ӧ����Ԫid
	ScanLineZBuffer slzBuffer;
	slzBuffer.setSize(width, height);
	slzBuffer.run(model);

	Display display(&model, &slzBuffer);
	display.run();
	
	//system("pause");
	return 0;
}