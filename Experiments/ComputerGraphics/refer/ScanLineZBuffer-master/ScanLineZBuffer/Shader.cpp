#include "Shader.h"
#include <omp.h>

using namespace std;

Shader::Shader()
{
	//this->model = model;
	//light_position = Point3f(0.0f, 500.0f, 0.0f);
	//light_color = Color3f(0.6, 0.6, 0.6);
	light_color = Color3f(0.3, 0.3, 0.3);
	ambient_color = Color3f(0.3, 0.3, 0.3);

	//light_position = Point3f(400.0f, 600.0f, 0.0f);
	light_position = Point3f(400.0f, 600.0f, 500.0f);
	//���ݹ�Դλ�ü���ģ�͸���Ԫ����ɫ���ŵ����߳��н���
	//render(model);
}


//�򵥵ĵ��Դ�����µĻ���
void Shader::run(Model& model)
{
	int face_num = model.faces.size();

	//�˴�����openmp
#pragma omp parallel for
	for (int i = 0; i < face_num; ++i)
	{
		Face& face = model.faces[i];
		int face_vertex_num = face.vertexIdx.size();
		for (int j = 0; j < face_vertex_num; ++j)
		{
			Vertex face_vertex = model.vertexes[face.vertexIdx[j]];
			Vec3f ray_direction = normalize(light_position - face_vertex.point);//�������䷴����
			Vec3f normal = face.normalIdx[j] >= 0 ?
				model.normals[face.normalIdx[j]]:face.normal;
			float cosine = dot(ray_direction, normal);//��������䷴�������淨��/���㷨�ߵļн�cos
			if (cosine>0.0)face.color += kd*cosine*light_color;//���Դɢ����ɫ
			face.color += ambient_color;//���ӻ�����ɫ
		}
		face.color /= face.vertexIdx.size();///�����С�����ɫȡ�����ƽ����ɫ

		//������ɫȡֵ��Χ��0.0��1.0֮��
		if (face.color.r > 1.0f)face.color.r = 1.0f;
		if (face.color.r < 0.0f)face.color.r = 0.0f;
		if (face.color.g > 1.0f)face.color.g = 1.0f;
		if (face.color.g < 0.0f)face.color.g = 0.0f;
		if (face.color.b > 1.0f)face.color.b = 1.0f;
		if (face.color.b < 0.0f)face.color.b = 0.0f;
	}
	
	
}






