#include "../shared/draw_helper.h"
#include "Quadtree.h"

using namespace std;
using namespace cv;
using namespace hlab;

void DisplayQuadtree(Mat& image, Quadtree::Node* n)
{
	if (!n) return;

	if (n->HasChild())
	{
		cv::line(image, { n->x, n->y + n->height / 2 }, { n->x + n->width, n->y + n->height / 2 }, Scalar(217, 217, 214), 1, LINE_AA);
		cv::line(image, { n->x + n->width / 2, n->y }, { n->x + n->width / 2 , n->y + n->height }, Scalar(217, 217, 214), 1, LINE_AA);
	}

	for (auto* c : n->children)
		if (c)
			DisplayQuadtree(image, c);

	cv::rectangle(image, Rect(n->x, n->y, n->width + 1, n->height + 1), Scalar(0, 102, 255), 1, LINE_AA); // width/height + 1 주의

	// 쿼드트리에 저장되어 있는 물체들(점들)을 그린다.
	//for (auto& p : n->objects)
	//{
	//	cv::circle(image, { p.x, p.y }, 5, Scalar(200, 200, 200), -1, LINE_AA);
	//}
}

int main(int argc, char** argv)
{
	hlab::initialize(1280, 960);

	vector<Point> points;

	for (int i = 0; i < 100; i++)
		points.push_back(Point(rand() % image.cols / 2 + image.cols / 4, rand() % image.rows / 2 + image.rows / 4));

	//for (auto& p : points)
	//	movable_points.push_back(&p); // 이 예제에서는 점을 움직이면 쿼드트리를 수정해야 합니다.

	hlab::Quadtree qtree(image.cols / 16, image.rows / 16, image.cols / 8 * 7, image.rows / 8 * 7, 5);

	for (Point p : points)
	{
		qtree.Insert(p);
	}

	Point reference{ image.cols / 2, image.rows / 2 };
	movable_points.push_back(&reference);
	int radius = 64;

	while (true)
	{
		hlab::preframe();

		// reference를 중심으로 하고 반지름이 radius인 원과 닿는 모든 리프 노드들을 찾는다.
		vector<Quadtree::Node*> leaves = qtree.FindLeaves(reference, radius);

		for (auto* n : leaves)
			cv::rectangle(image, Rect(n->x, n->y, n->width + 1, n->height + 1), Scalar(150, 255, 255), -1, LINE_AA); // width/height + 1 주의

		// 모든 점들을 회색으로 그리기
		for (Point p : points)
			cv::circle(image, p, 3, Scalar(200, 200, 200), -1, LINE_AA);

		DisplayQuadtree(image, qtree.root);

		// 찾는 영역을 표시하는 원
		cv::circle(image, reference, radius, Scalar(200, 255, 200), 2, LINE_AA);
		if (selected)
			cv::circle(image, *selected, radius, Scalar(0, 255, 0), 2, LINE_AA);

		for (auto* n : leaves) // 위에서 찾은 리프 노드들
			for (auto& p : n->objects) // 그 안에 들어 있는 점들
			{
				// 원 안에 들어온 점들은 빨간색, 그렇지 않은 점들은 파란색
				if (cv::norm(reference - p) <= radius)
					cv::circle(image, p, 4, kPureRed, -1, LINE_AA);
				else
					cv::circle(image, p, 3, kBlue, -1, LINE_AA);
			}

		if (hlab::postframe()) break;
	}

	return 0;
}
