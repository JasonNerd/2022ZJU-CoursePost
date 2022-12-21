#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "iostream"
#include "fstream"
#include "vector"

#include "MetaClass.h"
#include "Obj.cpp"

using namespace std;
using namespace cv;

#define BUTTON_OUT_UNPRESSED (0)
#define BUTTON_IN_UNPRESSED (1)
#define BUTTON_IN_PRESSED (2)


class Button
{
public:
	Point left_up;
	Point right_down;
	Scalar color;
	string text;
};


string window_name1 = "Depth";
string window_name2 = "Shadow";
vector<Button> buttons;
Obj obj;
Mat img;
Mat imgDepth;

int buttonState[9] = { 0 };

void initButton(vector<Button> &buttons)
{
	buttons.resize(9);
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i].color = Scalar(50, 50, 50);
	}
	buttons[1].left_up = Point(25, 278);
	buttons[1].right_down = Point(85, 313);
	buttons[2].left_up = Point(675, 278);
	buttons[2].right_down = Point(735, 313);
	buttons[3].left_up = Point(370, 70);
	buttons[3].right_down = Point(430, 105);
	buttons[4].left_up = Point(370, 518);
	buttons[4].right_down = Point(430, 553);
	buttons[5].left_up = Point(25, 70);
	buttons[5].right_down = Point(85, 105);
	buttons[6].left_up = Point(675, 70);
	buttons[6].right_down = Point(735, 105);
	buttons[7].left_up = Point(25, 518);
	buttons[7].right_down = Point(85, 553);
	buttons[8].left_up = Point(675, 518);
	buttons[8].right_down = Point(735, 553);

	buttons[1].text = "L Rotate(left key)";
	buttons[2].text = "R Rotate(right key)";
	buttons[3].text = "U Rotate(up key)";
	buttons[4].text = "D rotate(down key)";
	buttons[5].text = "Anti-Clock(Pgup key)";
	buttons[6].text = "Clock(Pgdn key)";
	buttons[7].text = "Enlarge(m)";
	buttons[8].text = "Reduce(n)";
}

void initButtonState()
{
	for (int i = 1; i < 9; i++)
	{
		buttonState[i] = BUTTON_OUT_UNPRESSED;
	}
}

void drawButton(Mat &img, vector<Button> &buttons)
{
	auto font = FONT_HERSHEY_SIMPLEX;
	auto white = Scalar(255, 255, 255);

	Point point = Point(30, 30);
	putText(img, "Click the button; Or press the key. You can change the model. ESC to quit", point, font, 0.6, white);

	for (int i = 1; i < buttons.size(); i++)
	{
		rectangle(img, Rect(buttons[i].left_up, buttons[i].right_down), buttons[i].color, -1);
		point = Point(buttons[i].left_up.x, buttons[i].left_up.y - 5);
		putText(img, buttons[i].text, point, font, 0.4, white);
	}

	point = Point(650, 590);
	putText(img, "made by HQ", point, font, 0.6, white);
}

void drawTime(Mat &img, double load_time, double render_time)
{
	auto font = FONT_HERSHEY_SIMPLEX;
	auto white = Scalar(255, 255, 255);
	char load_str[100];
	char render_str[100];
	sprintf_s(load_str, "LOAD Time: %.0fms", load_time * 1000);
	sprintf_s(render_str, "RENDER Time: %.0fms", render_time * 1000);
	Point point = Point(25, 135);
	putText(img, load_str, point, font, 0.6, white);
	point = Point(25, 165);
	putText(img, render_str, point, font, 0.6, white);

}
void showImg()
{
	Mat img = obj.img;
	Mat imgDepth = obj.imgDepth;
	initButtonState();
	drawButton(img, buttons);
	drawButton(imgDepth, buttons);
	drawTime(img, obj.load_time, obj.render_time);
	drawTime(imgDepth, obj.load_time, obj.render_time);
	imshow(window_name1, imgDepth);
	imshow(window_name2, img);
}

int getButtonOfMouse(vector<Button> &buttons, int x, int y)
{
	for (int i = 1; i < buttons.size(); i++)
	{
		if (x < buttons[i].left_up.x)
			continue;
		if (y < buttons[i].left_up.y)
			continue;
		if (x > buttons[i].right_down.x)
			continue;
		if (y > buttons[i].right_down.y)
			continue;
		return i;
	}
	return 0;
}

void handleButton(int buttonIndex)
{
	switch (buttonIndex)
	{
	case 1:
		obj.change(Obj::LEFT);
		break;
	case 2:
		obj.change(Obj::RIGHT);
		break;
	case 3:
		obj.change(Obj::UP);
		break;
	case 4:
		obj.change(Obj::DOWN);
		break;
	case 5:
		obj.change(Obj::ANTI_CLOCK);
		break;
	case 6:
		obj.change(Obj::CLOCK);
		break;
	case 7:
		obj.change(Obj::ENLARGE);
		break;
	case 8:
		obj.change(Obj::REDUCE);
		break;
	default:
		break;
	}
	obj.render();
	showImg();
}

void onMouse(int event, int x, int y, int flags, void* userdata)
{
	vector<Button> *buttons = (vector<Button> *)userdata;
	int buttonIndexOfMouse = getButtonOfMouse(*buttons, x, y);
	if (!buttonIndexOfMouse)
	{
		initButtonState();
		return;
	}
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		buttonState[buttonIndexOfMouse] = BUTTON_IN_PRESSED;
		break;
	case EVENT_LBUTTONUP:
		if (buttonState[buttonIndexOfMouse] == BUTTON_IN_PRESSED)
		{
			buttonState[buttonIndexOfMouse] = BUTTON_IN_UNPRESSED;
			handleButton(buttonIndexOfMouse);
		}
		break;
	default:
		break;
	}
}

int main(int argc, const char** argv)
{
	int width = 800;
	int height = 600;
	if (argc < 2)
	{
		cout << "You should use this pram by this format:" << endl;
		cout << "interval_scan_line example.obj" << endl;
		system("pause");
		return 0;
	}
	string filename(argv[1]);

	obj.setFrameSize(800, 600);
	int state = obj.load(filename);
	if (!state)
	{
		cout << "[File Wrong]: " << filename <<  " open wrong!" << endl;
		system("pause");
		return 0;
	}

	namedWindow(window_name1);
	namedWindow(window_name2);
	initButton(buttons);
	setMouseCallback(window_name1, onMouse, &buttons);
	setMouseCallback(window_name2, onMouse, &buttons);

	cout << "Interval Scan Line" << endl;
	cout << "Control Key Map: Or you can click the button" << endl;
	cout << "¡û: left rotate\n¡ú: right rotate" << endl;
	cout << "¡ü: up rotate \n¡ý: down rotate" << endl;
	cout << "Pgup: anti-clock rotate \nPgdn: clock rotate" << endl;
	cout << "m: enlarge \nn: reduce" << endl;
	cout << "ESC: quit" << endl;
	cout << "FACEs: " << obj.numOfFace << "\tVERTEXs: " << obj.numOfVertex << endl;
	obj.makeScale();
	obj.render();
	showImg();

	while (1)
	{
		int key = waitKeyEx(0);
		if (key == 27)
		{
			destroyAllWindows();
			exit(0);
			break;
		}
		else if (key == 2424832) // left
		{
			obj.change(Obj::LEFT);
		}
		else if (key == 2555904) // right
		{
			obj.change(Obj::RIGHT);
		}
		else if (key == 2490368) // up
		{
			obj.change(Obj::UP);
		}
		else if (key == 2621440) // down
		{
			obj.change(Obj::DOWN);
		}
		else if (key == 2162688) // Pgup
		{
			obj.change(Obj::ANTI_CLOCK);
		}
		else if (key == 2228224) // Pgdn
		{
			obj.change(Obj::CLOCK);
		}
		else if (key == 109) // m
		{
			obj.change(Obj::ENLARGE);
		}
		else if (key == 110) // n
		{
			obj.change(Obj::REDUCE);
		}
		else
		{
			continue;
		}
		obj.render();
		showImg();
	}
	system("pause");
	return 0;
}
