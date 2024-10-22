#pragma once

#include <iostream> 
#include <opencv2/core/core.hpp> 
#include <opencv2/imgproc.hpp> // Drawing shapes 
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/core/utils/logger.hpp> // loger options
#include <vector>

namespace hlab
{
	using namespace std;
	using namespace cv;

	Mat image;                         // �׸��� �׸��� ȭ��, ���(2���� �迭) �����Դϴ�.
	vector<cv::Point*> movable_points; // ���콺�� Ŭ���ؼ� ������ �� �ִ� ��ü
	bool left_down = false;            // ���콺 ���� ��ư�� ���ȴ� ��
	bool update_flag = false;          // Ŭ��&�巡�װ� �߻��ϸ� true�� ����, ����ȭ���� ���
	Point* selected = nullptr;         // ���콺 Ŭ������ ������ ��ü

	// ���� ����ϴ� ���� (����: RGB ������ �ƴ϶� BGR �����Դϴ�.)
	const Scalar kWhite = Scalar(255, 255, 255);
	const Scalar kPureBlue = Scalar(255, 0, 0);
	const Scalar kPureRed = Scalar(0, 0, 255);
	const Scalar kRed = Scalar(97, 105, 255);
	const Scalar kBlue = Scalar(255, 191, 0);
	const Scalar kBlack = Scalar(30, 30, 30);
	const Scalar kBrightGray = Scalar(200, 200, 200);

	void mouse_callback(int is_event, int x, int y, int flags, void* userdata)
	{
		// ȭ�鿡 �׷����� �̹��� ��ǥ��� ���� ���� (0, 0)�Դϴ�. 
		// �츮�� ���нð��� ����ϴ� ��ǥ��� ���� �Ʒ��� (0, 0)�̶� 
		// ���⼭�� ���Ʒ��� �������ְ� �ֽ��ϴ�.
		y = image.rows - y;

		if (left_down == false && is_event == EVENT_LBUTTONDOWN) // ���콺 ���� ��ư�� ������ ��, 
		{
			//cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

			left_down = true;

			for (auto* p : movable_points)
			{
				if (cv::norm(*p - cv::Point(x, y)) < 15.0) // ���� ��ü�� ������ ã�Ƽ� 
				{
					selected = p;                          // �����͸� selected�� ����
					break;
				}
			}

			if (selected != nullptr)
				*selected = cv::Point(x, y);
		}
		else if (is_event == EVENT_RBUTTONDOWN)
		{
			//cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		}
		else if (is_event == EVENT_LBUTTONUP)
		{
			left_down = false;
			selected = nullptr;
			update_flag = false;
		}
		else if (is_event == EVENT_MOUSEMOVE)
		{
			if (left_down && selected != nullptr) // ���õ� ��ü �̵�
			{
				*selected = cv::Point(x, y);
				update_flag = true; // ��ǥ�� �ٲ� ��ü�� �ִ����� ����ȭ�ϴ� �κп��� Ȯ���� �� ���
			}
		}
	}

	void initialize(int width, int height)
	{
		utils::logging::setLogLevel(utils::logging::LogLevel::LOG_LEVEL_SILENT);

		namedWindow("HongLab Algorithms Part2");
		setMouseCallback("HongLab Algorithms Part2", mouse_callback, NULL); // ����ڰ� ���콺�� �����ϸ� OS�� ���ؼ� mouse_callback()�� ����˴ϴ�.

		// OpenCV������ �̹����� ���(row x column)�Դϴ�.
		image = Mat(height, width, CV_8UC3, Scalar(255, 255, 255)); // �����ڿ� height(rows), width(columns) ������ �־���� �մϴ�.
	}

	void preframe()
	{
		image = kWhite; // ȭ���� ������� �ʱ�ȭ. ���� ����� �����ϴ� �����Դϴ�.
	}

	bool postframe() // ������ ���� �Ǹ� true�� ��ȯ�մϴ�.
	{
		cv::flip(image, image, 0); // ���� �Ʒ��� (0, 0)�� �ǵ��� ���Ʒ��� ������ �ݴϴ�.

		imshow("HongLab Algorithms Part2", image);

		if (waitKey(1) == 27) return true; // Press ESC key to out
		if (cv::getWindowProperty("HongLab Algorithms Part2", cv::WND_PROP_VISIBLE) <= 0) return true;

		return false;
	}
}