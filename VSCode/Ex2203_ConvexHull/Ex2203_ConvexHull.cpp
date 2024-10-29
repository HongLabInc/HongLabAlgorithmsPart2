#include "../shared/draw_helper.h"

#include <stack>
#include <deque>

using namespace std;
using namespace cv;
using namespace hlab;

/*
* 위키피디아 - https://en.wikipedia.org/wiki/Graham_scan
* 유튜브 잘 설명된 영상 - https://www.youtube.com/watch?v=B2AJoQSZf4M
*/

int Direction(Point p0, Point p1, Point p2)
{
	int x0 = p0.x;
	int x1 = p1.x;
	int x2 = p2.x;
	int y0 = p0.y;
	int y1 = p1.y;
	int y2 = p2.y;

	return (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
}

// O(nlog(n))
vector<Point> ConvexHullGrahamScan(vector<Point> points)
{
	int m = int(points.size()); // CLRS에서 수도코드에서 m 사용

	// 가장 아래에 있는 점(y가 작은 점)을 찾는다.
	int min_y = 0;
	for (int i = 1; i < m; i++)
		if (points[i].y < points[min_y].y)
			min_y = i;

	swap(points[min_y], points[0]); // 시작점을 인덱스 0 위치가 되도록 교환

	sort(points.begin() + 1, points.end(), [&](Point a, Point b) {
		int dir = Direction(points[0], a, b);
		if (dir == 0) return cv::norm(a - points[0]) > cv::norm(b - points[0]); // 더 먼쪽이 우선
		return  dir > 0;
		});

	vector<Point> hull; // 편의상 vector를 stack처럼 사용

	// TODO: CLRS 수도코드 대로 구현

	return hull;
}

// 참고: https://www.geeksforgeeks.org/convex-hull-using-jarvis-algorithm-or-wrapping/
// O(m * n), n은 모든 점들의 수, m은 헐에 속하는 점들의 수
vector<Point> ConvexHullJarvisWrapping(vector<Point>& points)
{
	vector<Point> hull;

	int n = int(points.size());

	// 가장 아래에 있는 점(y가 작은 점)을 찾는다.
	int start = 0;
	for (int i = 1; i < n; i++)
		if (points[i].y < points[start].y)
			start = i;

	hull.push_back(points[start]); // 시작점 추가

	int p = start, q;
	while (true)
	{
		q = (p + 1) % n;

		// TODO:

		if (p == start) break;

		hull.push_back(points[p]);
	}

	return hull;
}

int main(int argc, char** argv)
{
	using namespace cv;
	using namespace std;

	hlab::initialize(1280, 960);

	vector<Point> points;

	for (int i = 0; i < 10; i++)
		points.push_back(Point(rand() % (image.cols / 2) + image.cols / 4, rand() % (image.rows / 2) + image.rows / 4));

	assert(points.size() > 3); // 최소 3개는 있어야 컨벡스헐을 찾을 수 있다.

	for (auto& p : points)
		movable_points.push_back(&p);

	while (true)
	{
		hlab::preframe();

		for (Point p : points)
		{
			cv::circle(image, p, 15, Scalar(200, 200, 200), -1, LINE_AA);
		}

		if (selected)
			cv::circle(image, *selected, 17, Scalar(0, 255, 0), 1, LINE_AA);


		vector<Point> hull = ConvexHullJarvisWrapping(points);
		//vector<Point> hull = ConvexHullGrahamScan(points);

		for (int i = 0; i < hull.size(); i++)
		{
			cv::line(image, hull[i], hull[(i + 1) % hull.size()], kBlack, 1, LINE_AA);
			cv::putText(image, to_string(i % hull.size()), hull[i], cv::FONT_HERSHEY_SIMPLEX, 1, kBlack, 1, LINE_AA, true);
		}

		if (hlab::postframe()) break;
	}

	return 0;
}
