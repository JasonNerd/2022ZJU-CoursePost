#ifndef __SHADER_H
#define __SHADER_H

#include "Vec.h"
#include "Model.h"

const float kd = 0.8;//albedo ,ɢ�䷴����

class Shader
{
public:
	//Model model;
	Point3f light_position;//���Դλ��
	Color3f light_color;
	Color3f ambient_color;

	Shader();
	//Shader(Model model);

	void run(Model& model);

};


#endif