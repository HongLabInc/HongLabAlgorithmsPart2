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

	Mat image;                         // 그림을 그리는 화면, 행렬(2차원 배열) 형식입니다.
	vector<cv::Point*> movable_points; // 마우스로 클릭해서 움직일 수 있는 물체
	bool left_down = false;            // 마우스 왼쪽 버튼이 눌렸는 지
	bool update_flag = false;          // 클릭&드래그가 발생하면 true로 선택, 가시화에서 사용
	Point* selected = nullptr;         // 마우스 클릭으로 선택한 물체

	// 자주 사용하는 색들 (주의: RGB 순서가 아니라 BGR 순서입니다.)
	const Scalar kWhite = Scalar(255, 255, 255);
	const Scalar kPureBlue = Scalar(255, 0, 0);
	const Scalar kPureRed = Scalar(0, 0, 255);
	const Scalar kRed = Scalar(97, 105, 255);
	const Scalar kBlue = Scalar(255, 191, 0);
	const Scalar kBlack = Scalar(30, 30, 30);
	const Scalar kBrightGray = Scalar(200, 200, 200);

	void mouse_callback(int is_event, int x, int y, int flags, void* userdata)
	{
		// 화면에 그려지는 이미지 좌표계는 왼쪽 위가 (0, 0)입니다. 
		// 우리가 수학시간에 사용하는 좌표계는 왼쪽 아래가 (0, 0)이라서 
		// 여기서는 위아래를 뒤집어주고 있습니다.
		y = image.rows - y;

		if (left_down == false && is_event == EVENT_LBUTTONDOWN) // 마우스 왼쪽 버튼이 눌렸을 때, 
		{
			//cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

			left_down = true;

			for (auto* p : movable_points)
			{
				if (cv::norm(*p - cv::Point(x, y)) < 15.0) // 눌린 물체가 뭔지를 찾아서 
				{
					selected = p;                          // 포인터를 selected에 저장
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
			if (left_down && selected != nullptr) // 선택된 물체 이동
			{
				*selected = cv::Point(x, y);
				update_flag = true; // 좌표가 바뀐 물체가 있는지를 가시화하는 부분에서 확인할 때 사용
			}
		}
	}

	void initialize(int width, int height)
	{
		utils::logging::setLogLevel(utils::logging::LogLevel::LOG_LEVEL_SILENT);

		namedWindow("HongLab Algorithms Part2");
		setMouseCallback("HongLab Algorithms Part2", mouse_callback, NULL); // 사용자가 마우스를 조작하면 OS를 통해서 mouse_callback()이 실행됩니다.

		// OpenCV에서는 이미지도 행렬(row x column)입니다.
		image = Mat(height, width, CV_8UC3, Scalar(255, 255, 255)); // 생성자에 height(rows), width(columns) 순서로 넣어줘야 합니다.
	}

	void preframe()
	{
		image = kWhite; // 화면을 흰색으로 초기화. 새로 지우고 시작하는 개념입니다.
	}

	bool postframe() // 종료할 때가 되면 true를 반환합니다.
	{
		cv::flip(image, image, 0); // 왼쪽 아래가 (0, 0)이 되도록 위아래를 뒤집어 줍니다.

		imshow("HongLab Algorithms Part2", image);

		if (waitKey(1) == 27) return true; // Press ESC key to out
		if (cv::getWindowProperty("HongLab Algorithms Part2", cv::WND_PROP_VISIBLE) <= 0) return true;

		return false;
	}
}