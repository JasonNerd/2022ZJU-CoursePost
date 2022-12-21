#pragma once
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "iostream"
#include "fstream"
#include "vector"
#include "set"
#include "ctime"

#include "MetaClass.h"
using namespace std;

class Poly
{
public:
	int faceIndex;
	double a = 0, b =0,  c = 0, d = 0;
	int dy;
	Color color;
};

class Edge
{
public:
	double x;
	int polyIndex;
	double dx;
	int dy;
	bool operator < (const Edge &a)
	{
		return x < a.x;
	}
};

class Obj
{
public:
	int height, width;
	int numOfVertex = 0, numOfFace = 0;
	cv::Mat img;
	cv::Mat imgDepth;
	Vertex3f center;
	double scaleRate = 1.0f;
	double minX = FLT_MAX;
	double maxX = -FLT_MAX;
	double minY = FLT_MAX;
	double maxY = -FLT_MAX;
	double minZ = FLT_MAX;
	double maxZ = -FLT_MAX;
	double load_time = 0;
	double render_time = 0;
	set<int> InOrOut;
	vector<Face> faces;
	vector<Poly> PT;
	vector<vector<Edge>> ET;
	vector<Edge> AET;
	constexpr static int LEFT = 0;
	constexpr static int RIGHT = 1;
	constexpr static int UP = 2;
	constexpr static int DOWN = 3;
	constexpr static int ANTI_CLOCK= 4;
	constexpr static int CLOCK = 5;
	constexpr static int ENLARGE = 6;
	constexpr static int REDUCE = 7;

	void setFrameSize(int w, int h)
	{
		height = h + 1;
		width = w + 1;
		img = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
		imgDepth = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
	}

	void setColor() {
		Color light = Color(3, 0, 3);
		Vertex3f light_vertex = Vertex3f(width / 2, height / 2, maxZ + 5 * (maxZ - minZ));

		for (Face &face : faces) {
			for (Vertex3f &v : face.vertexes) {
				Vector3f direction = Normalize(Vertex3f(light_vertex.x - v.x, light_vertex.y - v.y, light_vertex.z - v.z));
				Normal &normal = face.normal;
				double thera = std::abs(direction.x * normal.x + direction.y * normal.y + direction.z * normal.z);
				face.color = Color(thera * light.r, thera * light.g, thera * light.b);
			}

			face.color.r /= face.vertexes.size();
			face.color.g /= face.vertexes.size();
			face.color.b /= face.vertexes.size();
			if (face.color.r < 0.0f)
				face.color.r = 0.0f;
			if (face.color.r > 1.0f)
				face.color.r = 1.0f;
			if (face.color.g < 0.0f)
				face.color.g = 0.0f;
			if (face.color.g > 1.0f)
				face.color.g = 1.0f;
			if (face.color.b < 0.0f)
				face.color.b = 0.0f;
			if (face.color.b > 1.0f)
				face.color.b = 1.0f;
			face.color.r *= 255;
			face.color.g *= 255;
			face.color.b *= 255;
		}
	}

	void change(int action)
	{
		switch (action)
		{
		case LEFT:
		case RIGHT:
		case UP:
		case DOWN:
		case ANTI_CLOCK:
		case CLOCK:
			rotate(action);
			break;
		case ENLARGE:
			scaleRate *= 1.1f;
			makeScale(scaleRate);
			break;
		case REDUCE:
			scaleRate *= 0.9f;
			makeScale(scaleRate);
			break;
		default:
			break;
		}

	}

	void makeScale()
	{
		makeScale(scaleRate);
	}

	void makeScale(double rate)
	{
		if (rate > 1.45f)
			rate = 1.45f;
		scaleRate = rate;
		center.x = (maxX + minX) / 2.0;
		center.y = (maxY + minY) / 2.0;
		center.z = (maxZ + minZ) / 2.0;
		double dis_x = maxX - minX;
		double dis_y = maxY - minY;
		double dis_z = maxZ - minZ;

		double scale = min(height, width) / (max(max(dis_x, dis_y), dis_z) * 1.414f) * 2 / 3;
		// scale = 5;
		scale *= rate;
		for (Face &face : faces)
		{
			for (Vertex3f &v : face.vertexes)
			{
				v.x = (v.x - center.x) * scale + width / 2;
				v.y = (v.y - center.y) * scale + height / 2;
				v.z = (v.z - center.z) * scale;
			}
		}
		minX = (minX - center.x) * scale + width / 2;
		maxX = (maxX - center.x) * scale + width / 2;
		minY = (minY - center.y) * scale + height / 2;
		maxY = (maxY - center.y) * scale + height / 2;
		minZ = (minZ - center.z) * scale;
		maxZ = (maxZ - center.z) * scale;
		center = Vertex3f(width / 2, height / 2, 0);
	}

	void rotate(int direction)
	{
		double PI = 3.1415926;
		double dtheta = PI / 64;
		double Rot[3][3] = { 0 };
		switch (direction) {
		case LEFT:
			dtheta = -dtheta;
		case RIGHT:
			Rot[1][1] = 1;
			Rot[0][0] = cos(dtheta);
			Rot[0][2] = sin(dtheta);
			Rot[2][0] = -sin(dtheta);
			Rot[2][2] = cos(dtheta);
			break;
		case UP:
			dtheta = -dtheta;
		case DOWN:
			Rot[0][0] = 1;
			Rot[1][1] = cos(dtheta);
			Rot[1][2] = -sin(dtheta);
			Rot[2][1] = sin(dtheta);
			Rot[2][2] = cos(dtheta);
			break;
		case ANTI_CLOCK:
			dtheta = -dtheta;
		case CLOCK:
			dtheta = -dtheta;
			Rot[2][2] = 1;
			Rot[0][0] = cos(dtheta);
			Rot[0][1] = -sin(dtheta);
			Rot[1][0] = sin(dtheta);
			Rot[1][1] = cos(dtheta);
			break;
		default:
			break;
		}

		minX = FLT_MAX;
		maxX = -FLT_MAX;
		minY = FLT_MAX;
		maxY = -FLT_MAX;
		minZ = FLT_MAX;
		maxZ = -FLT_MAX;
		for (Face &face : faces) {
			face.normal.leftDot(Rot);
			for (Vertex3f &v : face.vertexes) {
				RotateAxes(v, Rot, center);
				minX = min(minX, v.x);
				maxX = max(maxX, v.x);
				minY = min(minY, v.y);
				maxY = max(maxY, v.y);
				minZ = min(minZ, v.z);
				maxZ = max(maxZ, v.z);
			}
		}
	}

	void changeInOrOut(int index)
	{
		set<int>::iterator  it = InOrOut.find(index);
		if (InOrOut.end() == it)
		{
			InOrOut.insert(index);
		}
		else
		{
			InOrOut.erase(index);
		}
	}

	int load(string filename)
	{
		clock_t start = clock();
		ifstream file(filename);
		if (!file.is_open())
			return 0;
		string type;
		vector<Vertex3f> vertexes;

		while (file >> type)
		{
			if (type == "v")
			{
				Vertex3f v;
				file >> v.x >> v.y >> v.z;
				vertexes.push_back(v);
				numOfVertex++;

				minX = min(minX, v.x);
				maxX = max(maxX, v.x);
				minY = min(minY, v.y);
				maxY = max(maxY, v.y);
				minZ = min(minZ, v.z);
				maxZ = max(maxZ, v.z);
			}
			else if (type == "f")
			{
				char str[1000];
				file.getline(str, 1000);
				string pattern(str);
				pattern = pattern + " ";
				int vIndex, tIndex, nIndex;
				vector<int> vertex_ids;

				string::size_type v_pos1, v_pos2;
				string::size_type i_pos1, i_pos2;
				v_pos1 = 1;
				v_pos2 = pattern.find(" ", v_pos1);
				string sub_pattern;
				string temp;
				while (string::npos != v_pos2)
				{
					vIndex = tIndex = nIndex = 0;
					sub_pattern = pattern.substr(v_pos1, v_pos2 - v_pos1);
					v_pos1 = v_pos2 + 1;
					v_pos2 = pattern.find(" ", v_pos1);

					i_pos1 = 0;
					i_pos2 = sub_pattern.find("/");
					temp = sub_pattern.substr(i_pos1, i_pos2 - i_pos1);
					if (temp.size())
						vIndex = stoi(temp);
					if (vIndex)
						vertex_ids.push_back(vIndex - 1);

					if (string::npos == i_pos2)
						continue;
					i_pos1 = i_pos2 + 1;
					i_pos2 = sub_pattern.find("/", i_pos1);
					temp = sub_pattern.substr(i_pos1, i_pos2 - i_pos1);
					if (temp.size())
						tIndex = stoi(temp);

					if (string::npos == i_pos2)
						continue;
					i_pos1 = i_pos2 + 1;
					i_pos2 = sub_pattern.find("/", i_pos1);
					temp = sub_pattern.substr(i_pos1, i_pos2 - i_pos1);
					if (temp.size())
						nIndex = stoi(temp);
				}

				if (vertex_ids.size() > 2)
				{
					Face face;
					for (int i = 0; i < vertex_ids.size(); i++)
					{
						face.vertexes.push_back(vertexes[vertex_ids[i]]);
					}

					Vertex3f &a = face.vertexes[0];
					Vertex3f &b = face.vertexes[1];
					Vertex3f &c = face.vertexes[2];

					Vertex3f normal = Cross(Vertex3f(a.x - b.x, a.y - b.y, a.z - b.z),
						Vertex3f(b.x - c.x, b.y - c.y, b.z - c.z));

					face.normal = Normalize(normal);

					faces.push_back(face);
					numOfFace++;
				}
			}
		}
		clock_t end = clock();
		load_time = (double)(end - start) / CLOCKS_PER_SEC;
		return 1;
	}

	int getClosestPoly(int x, int y)
	{
		if (InOrOut.empty())
			return -1;
		set<int>::iterator it = InOrOut.begin();
		int max_depth_i = *it;
		double max_depth;
		Poly p = PT[max_depth_i];
		if (std::abs(p.c) <= 1.0e-3f)
			max_depth = getVerticalPolyDepth(max_depth_i, x, y);
		else
			max_depth = -(p.a * x + p.b * y + p.d) / p.c;
		double depth;
		for (it++; it != InOrOut.end(); it++)
		{
			p = PT[*it];
			if (std::abs(p.c) <= 1.0e-3f)
				depth = getVerticalPolyDepth(*it, x, y);
			else
				depth = -(p.a * x + p.b * y + p.d) / p.c;
			if (depth > max_depth)
			{
				max_depth_i = *it;
				max_depth = depth;
			}
		}
		return max_depth_i;
	}

	double getVerticalPolyDepth(int index, int x, int y)
	{
		int faceIndex = PT[index].faceIndex;
		Face &face = faces[faceIndex];
		double depth = -FLT_MAX;
		vector<Vertex3f> &vertexes = face.vertexes;
		Vertex3f *p, *q;
		for (int i = 0; i < vertexes.size(); i++)
		{
			int j = (i == vertexes.size() - 1) ? 0 : (i + 1);
			p = &vertexes[i];
			q = &vertexes[j];
			int x1 = (int)(p->x + 0.5);
			int x2 = (int)(q->x + 0.5);
			int y1 = (int)(p->y + 0.5);
			int y2 = (int)(q->y + 0.5);
			if (x1 == x2 && x1 == x)
			{
				if (y1 == y2)
					depth = max(depth, max(p->z, q->z));
				else
					depth = max(depth, ((q->z - p->z) / (q->y - p->y) * (y - p->y) + p->y));
			}
		}
		return max(depth, -1.0);
	}

	void calImg(int x1, int x2, int y, vector<double> &z_buffer)
	{
		if (InOrOut.empty())
			return;
		if (x1 < 0)
			x1 = 0;
		if (x2 < 0)
			x2 = 0;
		if (x1 >= width)
			x1 = width - 1;
		if (x2 >= width)
			x2 = width - 1;
		int index_left = getClosestPoly(x1, y);
		int index_right = getClosestPoly(x2, y);
		Poly &p = PT[index_left];
		Poly &q = PT[index_right];
		if (index_left == index_right)
		{
			double old_z = z_buffer[x1];
			if (std::abs(p.c) <= 1.0e-3f)
				z_buffer[x1] = getVerticalPolyDepth(index_left, x1, y) - minZ;
			else
				z_buffer[x1] = -(p.a * x1 + p.b * y + p.d) / p.c - minZ;
			double zdx = -p.a / p.c;
			for (int x = x1 + 1; x <= x2; x++)
			{
				z_buffer[x] = z_buffer[x - 1] + zdx;
			}
			z_buffer[x1] = max(z_buffer[x1], old_z);
			Color &color = faces[p.faceIndex].color;
			cv::line(img,cv::Point(x1, height - y - 1), cv::Point(x2, height - y - 1),cv::Scalar(color.r, color.g, color.b));
			
		}
		else
		{
			int middle_x = (int)(-(((q.b*p.c - q.c * p.b) * y + p.c*q.d - q.c*p.d) / (q.a*p.c - q.c*p.a)) + 0.5);
			if (middle_x < x1)
				middle_x = x1;
			if (middle_x > x2)
				middle_x = x2;
			double zdxl = -p.a / p.c;
			double zdxr = q.a / q.c;
			double old_z = z_buffer[x1];
			if (std::abs(p.c) <= 1.0e-3f)
				z_buffer[x1] = getVerticalPolyDepth(index_left, x1, y) - minZ;
			else
				z_buffer[x1] = -(p.a * x1 + p.b * y + p.d) / p.c - minZ;
			for (int x = x1 + 1; x <= middle_x; x++)
			{
				z_buffer[x] = z_buffer[x - 1] + zdxl;
			}
			z_buffer[x1] = max(z_buffer[x1], old_z);
			Color color = faces[p.faceIndex].color;
			cv::line(img,cv::Point(x1, height - y - 1), cv::Point(middle_x, height - y - 1),cv::Scalar(color.r, color.g, color.b));
			old_z = z_buffer[x2];
			if (std::abs(q.c) <= 1.0e-3f)
				z_buffer[x2] = getVerticalPolyDepth(index_right, x2, y) - minZ;
			else
				z_buffer[x2] = -(q.a * x2 + q.b * y + q.d) / q.c - minZ;
			for (int x = x2 - 1; x > middle_x; x--)
			{
				z_buffer[x] = z_buffer[x + 1] + zdxr;
			}
			z_buffer[x2] = max(z_buffer[x2], old_z);
			color = faces[q.faceIndex].color;
			cv::line(img,cv::Point(middle_x, height - y - 1), cv::Point(x2, height - y - 1),cv::Scalar(color.r, color.g, color.b));
		}
		for (int x = x1; x <= x2; x++)
		{
			imgDepth.at<uchar>(height - y - 1, x) = int((z_buffer[x]) / (maxZ - minZ) * 180 + 20);
		}
	}

	void render() {
		clock_t start = clock();
		imgDepth = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
		img = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
		setColor();
		vector<double> z_buffer;
		Edge e1, e2;
		buildPTAndET();
		for (int y = height - 1; y >= 0; y--)
		{
			z_buffer.clear();
			z_buffer.resize(width);
			updateAET(y);
			InOrOut.clear();
			if (AET.empty())
				continue;
			e1 = AET[0];
			vector<Edge>::iterator it = AET.begin() + 1;
			for (; it != AET.end(); it++)
			{
				changeInOrOut(e1.polyIndex);
				e2 = *it;
				calImg((int)(e1.x + 0.5), (int)(e2.x + 0.5), y, z_buffer);
				e1 = e2;
			}
		}
		clock_t end = clock();
		render_time = (double)(end - start) / CLOCKS_PER_SEC;
	};

	void buildPTAndET() {
		PT.clear();
		ET.clear();
		// PT.resize(height);
		ET.resize(height);
		vector<Face>::iterator face = faces.begin();
		int current_poly_index = 0;
		for (; face != faces.end(); face++)
		{
			double poly_ymax = -FLT_MAX;
			double poly_ymin = FLT_MAX;
			Poly poly;
			poly.faceIndex = (int)(face - faces.begin());
			poly.a = (*face).normal.x;
			poly.b = (*face).normal.y;
			poly.c = (*face).normal.z;
			Vertex3f v = (*face).vertexes[0];
			poly.d = -(poly.a * v.x + poly.b * v.y + poly.c * v.z);

			vector<Vertex3f> &vertexes = (*face).vertexes;
			Vertex3f *p, *q;
			for (int i = 0; i < vertexes.size(); i++)
			{
				int j = (i == vertexes.size() - 1) ? 0 : (i + 1);
				p = &vertexes[i];
				q = &vertexes[j];
				if (p->y < q->y)
					std::swap(p, q);
				Edge edge;
				int ymax = (int)(p->y + 0.5);
				int ymin = (int)(q->y + 0.5);
				edge.x = p->x;
				edge.dy = ymax - ymin;
				if (edge.dy <= 0)
					continue;
				edge.dx = -(p->x - q->x) / edge.dy;
				edge.polyIndex = current_poly_index;
				ET[(int)(ymax + 0.5)].push_back(edge);
				poly_ymax = poly_ymax > ymax ? poly_ymax : ymax;
				poly_ymin = poly_ymin < ymin ? poly_ymin : ymin;
			}
			poly.dy = (int)(poly_ymax - poly_ymin + 0.5);
			// PT[poly_ymax].push_back(poly);
			PT.push_back(poly);
			current_poly_index++;
		}
	};

	void updateAET(int y) {
		vector<Edge>::iterator it = AET.begin();
		for(; it != AET.end();)
		{
			if ((*it).dy == 1)
			{
				it = AET.erase(it);
			}
			else
			{
				it->x += it->dx;
				it->dy--;
				it++;
			}
		}
		AET.insert(AET.end(), ET[y].begin(), ET[y].end());
		std::sort(AET.begin(), AET.end());
	};

	void updateIPT() {};

};
