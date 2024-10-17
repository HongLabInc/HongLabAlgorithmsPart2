#include "../shared/draw_helper.h"

using namespace std;
using namespace cv;
using namespace hlab;

// Introduction to Algorithms, 3rd Edition, Ch 33. p. 1018
int Direction(Point p0, Point p2, Point p1)
{
	return (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
}

bool OnSegment(Point pi, Point pj, Point pk)
{
	if (min(pi.x, pj.x) <= pk.x && pk.x <= max(pi.x, pj.x) &&
		min(pi.y, pj.y) <= pk.y && pk.y <= max(pi.y, pj.y)) return true;
	else return false;
}

bool SegmentsIntersect(Point p1, Point p2, Point p3, Point p4) // Segment-Segment Intersect
{
	int d1 = Direction(p3, p4, p1);
	int d2 = Direction(p3, p4, p2);
	int d3 = Direction(p1, p2, p3);
	int d4 = Direction(p1, p2, p4);

	return false; // TODO: 구현
}

// https://stackoverflow.com/questions/7446126/opencv-2d-line-intersection-helper-function
// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
bool Intersection(Point o1, Point p1, Point o2, Point p2, Point& r)
{
	Point x = o2 - o1;
	Point d1 = p1 - o1;
	Point d2 = p2 - o2;

	return false; // TODO: 구현
}

void DrawArrow(cv::Mat& image, Point line_start, Point line_end, Scalar color)
{
	cv::Point temp = line_end - line_start;
	cv::line(image, line_start - temp * 1000, line_start + temp * 1000, Scalar(200, 200, 200), 1, LINE_AA);
	cv::circle(image, line_start, 5, color, -1, LINE_AA);
	cv::arrowedLine(image, line_start, line_end, color, 1, LINE_AA);
}

int main(int argc, char** argv)
{
	hlab::initialize(1280, 960);

	Point p1 = Point(540, 340);
	Point p2 = Point(540, 500);
	Point p3 = Point(740, 340);
	Point p4 = Point(740, 500);

	movable_points.push_back(&p1);
	movable_points.push_back(&p2);
	movable_points.push_back(&p3);
	movable_points.push_back(&p4);

	while (true)
	{
		hlab::preframe();

		if (selected)
			cv::circle(image, *selected, 17, Scalar(0, 255, 0), 1, LINE_AA);

		Scalar color = kBlack;

		if (SegmentsIntersect(p1, p2, p3, p4) == true)
		{
			color = kRed; // 아래에서 화살표도 빨간색

			Point r;
			if (Intersection(p1, p2, p3, p4, r))
				cv::circle(image, r, 5, color, -1, LINE_AA);
		}

		if (selected)
			cv::circle(image, *selected, 17, Scalar(0, 255, 0), 1, LINE_AA);

		DrawArrow(image, p1, p2, color);
		DrawArrow(image, p3, p4, color);

		if (hlab::postframe()) break;
	}

	return 0;
}
