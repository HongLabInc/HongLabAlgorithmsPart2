#include "../shared/draw_helper.h"

#include <list>

using namespace std;
using namespace cv;
using namespace hlab;

// https://matthias-research.github.io/pages/tenMinutePhysics/11-hashing.pdf
int Hash(int x, int y, int size)
{
	int h = (x * 92837111) ^ (y * 689287499);
	return abs(h) % size;
}

int main(int argc, char** argv)
{
	hlab::initialize(1280, 960);

	vector<Point> points;

	for (int i = 0; i < 1000; i++)
		points.push_back(Point(rand() % image.cols / 2 + image.cols / 4, rand() % image.rows / 2 + image.rows / 4));

	//for (auto& p : points)
	//	movable_points.push_back(&p); // 이 예제에서는 점을 움직이면 해시맵을 수정해야 하기 때문에 이동 금지

	Point reference{ image.cols / 2, image.rows / 2 };
	movable_points.push_back(&reference);

	int radius = 64;

	vector<vector<list<Point*>>> uniform_grid(image.cols / radius, vector<list<Point*>>(image.rows / radius));
	for (auto& p : points)
		uniform_grid[p.x / radius][p.y / radius].push_back(&p);

	vector<list<Point*>> hashmap(500); // 500은 충돌을 피하기에 적당한 크기
	// TODO: hashmap을 사용하도록 수정

	while (true)
	{
		hlab::preframe();

		// 원의 반경에 해당하는 셀들을 노란색으로
		for (int c = std::max(0, int(reference.x / radius) - 1); c <= std::min(image.cols - 1, int(reference.x / radius) + 1); c++)
		{
			for (int r = std::max(0, int(reference.y / radius) - 1); r <= std::min(image.rows - 1, int(reference.y / radius) + 1); r++)
			{
				cv::rectangle(image, Rect(c * radius, r * radius, radius, radius), Scalar(150, 255, 255), -1, LINE_AA);
			}
		}

		// 찾는 영역을 표시하는 원
		cv::circle(image, reference, radius, Scalar(200, 255, 200), -1, LINE_AA);
		if (selected)
			cv::circle(image, *selected, radius, Scalar(0, 255, 0), 1, LINE_AA);

		// 전체 격자(바둑판) 구조 그리기
		for (int c = 0; c < image.cols; c += radius)
			cv::line(image, { c, 0 }, { c, image.rows - 1 }, Scalar(217, 217, 214), 1, LINE_AA);
		for (int r = 0; r < image.rows; r += radius)
			cv::line(image, { 0, r }, { image.cols - 1, r }, Scalar(217, 217, 214), 1, LINE_AA);

		// 모든 점들을 회색으로 그리기
		for (Point p : points)
			cv::circle(image, p, 3, Scalar(200, 200, 200), -1, LINE_AA);

		for (int c = max(0, int(reference.x / radius) - 1); c <= min(image.cols - 1, int(reference.x / radius) + 1); c++)
		{
			for (int r = max(0, int(reference.y / radius) - 1); r <= min(image.rows - 1, int(reference.y / radius) + 1); r++)
			{
				for (Point* p : uniform_grid[c][r]) // TODO: uniform_grid 대신에 hashmap을 사용하도록 수정
				{
					// 원 안에 들어온 점들은 빨간색, 그렇지 않은 점들은 파란색
					if (cv::norm(reference - *p) < radius)
						cv::circle(image, *p, 4, kPureRed, -1, LINE_AA);
					else
						cv::circle(image, *p, 3, kBlue, -1, LINE_AA);
				}
			}
		}

		if (hlab::postframe()) break;
	}

	return 0;
}
