#include "../shared/draw_helper.h"

#include <string>

using namespace std;
using namespace cv;
using namespace hlab;

struct LineSegment
{
	Point p1, p2;
	string name; // 디버깅/가시화 용도

	//void Print() // 디버깅용
	//{
	//	cout << "{ {" << p1.x << ", " << p1.y << "}, {" << p2.x << ", " << p2.y << "}, \"" << name << "\" })" << endl;
	//}
};

// 양수면 왼쪽, 음수면 오른쪽, 0이면 직선 위에
int Direction(Point p0, Point p2, Point p1)
{
	return (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
}

bool OnSegment(Point pi, Point pj, Point pk)
{
	if (min(pi.x, pj.x) <= pk.x && pk.x <= max(pi.x, pj.x) && min(pi.y, pj.y) <= pk.y && max(pi.y, pj.y)) return true;
	else return false;
}

// p1-p2는 선분이 아니라 직선(길이 무한)
bool LineAndSegmentIntersect(Point p1, Point p2, Point p3, Point p4)
{
	int d1 = Direction(p3, p4, p1);
	int d2 = Direction(p3, p4, p2);
	int d3 = Direction(p1, p2, p3);
	int d4 = Direction(p1, p2, p4);

	if ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)) return true;

	return false;
}

bool SegmentsIntersect(Point p1, Point p2, Point p3, Point p4)
{
	int d1 = Direction(p3, p4, p1);
	int d2 = Direction(p3, p4, p2);
	int d3 = Direction(p1, p2, p3);
	int d4 = Direction(p1, p2, p4);

	// 서로 교차하는지 확인
	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) return true;
	//if ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)) return true; // Line-Segment Interect
	else if (d1 == 0 && OnSegment(p3, p4, p1)) return true;
	else if (d2 == 0 && OnSegment(p3, p4, p2)) return true;
	else if (d3 == 0 && OnSegment(p1, p2, p3)) return true;
	else if (d4 == 0 && OnSegment(p1, p2, p4)) return true;
	else return false;
}

// https://stackoverflow.com/questions/7446126/opencv-2d-line-intersection-helper-function
bool Intersection(Point o1, Point p1, Point o2, Point p2, Point& r)
{
	Point x = o2 - o1;
	Point d1 = p1 - o1;
	Point d2 = p2 - o2;

	double cross = d1.x * d2.y - d1.y * d2.x;
	if (abs(cross) < /*EPS*/1e-8)
		return false;

	double t1 = (x.x * d2.y - x.y * d2.x) / cross;
	r = o1 + d1 * t1;
	return true;
}

class BSPTree
{
public:
	struct Node
	{
		LineSegment l;

		Node* left = nullptr;
		Node* right = nullptr;
	};

	void Insert(LineSegment l)
	{
		if (!root)
		{
			root = new Node{ l };
			cout << "Root " << l.name << endl;
		}
		else
		{
			Insert(root, l);
		}
	}

	void Insert(Node* node, LineSegment l)
	{
		// cross product를 이용해서 왼쪽이면 왼쪽으로 오른쪽이면 오른쪽으로
		// 걸쳐 있다면 잘라서 넣는다.

		int d1 = Direction(node->l.p1, node->l.p2, l.p1);
		int d2 = Direction(node->l.p1, node->l.p2, l.p2);

		// p1-p2 직선과 완전히 겹치는 선분은 입력으로 들어오지 않는다고 가정
		// if(d1 == 0 && d2 == 0) { 발생하지 않는다고 가정 } 

		// 1. 전부 왼쪽일 경우
		if (d1 >= 0 && d2 >= 0)
		{
			if (!node->left)
				node->left = new Node{ l };
			else
				Insert(node->left, l);
		}

		// 2. 전부 오른쪽일 경우
		else if (d1 <= 0 && d2 <= 0)
		{
			if (!node->right)
				node->right = new Node{ l };
			else
				Insert(node->right, l);
		}
		// 3. 걸쳐있을 때는 잘라서 (p1은 왼쪽)
		else if (d1 >= 0 && d2 <= 0) // p1은 왼쪽, p2는 오른쪽
		{
			Point r;
			Intersection(node->l.p1, node->l.p2, l.p1, l.p2, r);

			if (!node->left)
				node->left = new Node{ {l.p1, r, l.name + "1"} };
			else
				Insert(node->left, { l.p1, r, l.name + "1" });

			if (!node->right)
				node->right = new Node{ {r, l.p2 , l.name + "2"} };
			else
				Insert(node->right, { r, l.p2 , l.name + "2" });
		}
		// 4. 걸쳐있을 때 잘라서 (p1은 오른쪽)
		else if (d1 <= 0 && d2 >= 0) // p1은 오른쪽, p2는 왼쪽
		{
			Point r;
			Intersection(node->l.p1, node->l.p2, l.p1, l.p2, r);

			if (!node->right)
				node->right = new Node{ {l.p1, r, l.name + "1" } };
			else
				Insert(node->right, { l.p1, r, l.name + "1" });

			if (!node->left)
				node->left = new Node{ {r, l.p2, l.name + "2"} };
			else
				Insert(node->left, { r, l.p2, l.name + "2" });
		}
	}

	vector<LineSegment> FindLeaf(Point p)
	{
		vector<LineSegment> visited;

		FindLeaf(root, p, visited);

		return visited;
	}

	void FindLeaf(Node* n, Point p, vector<LineSegment>& visited)
	{
		if (!n) return;

		visited.push_back(n->l);

		int dir = Direction(n->l.p1, n->l.p2, p);

		if (dir >= 0) // 0인 경우는 왼쪽으로
			FindLeaf(n->left, p, visited);
		else
			FindLeaf(n->right, p, visited);
	}

	vector<LineSegment> Traverse(Point p)
	{
		vector<LineSegment> visited;

		Traverse(root, p, visited);

		return visited;
	}

	void Traverse(Node* n, Point p, vector<LineSegment>& visited)
	{
		if (!n) return;

		// cout << n->l.name << " ";

		if (Direction(n->l.p1, n->l.p2, p) >= 0)// 0인 경우는 왼쪽으로 (결국 모두 방문하기 때문에 충돌 체크 결과는 동일)
		{
			//cout << "Left of " << n->l.name << endl;

			Traverse(n->right, p, visited); // right먼저 방문
			visited.push_back(n->l);        // Inorder traversal
			Traverse(n->left, p, visited);
		}
		else
		{
			Traverse(n->left, p, visited); // left먼저 방문
			visited.push_back(n->l);
			Traverse(n->right, p, visited);
		}
	}

	bool Collision(LineSegment ray, Point& result) // 0보다 작은 값을 반환하면 충돌 없음
	{
		return Collision(root, ray, result);
	}

	bool Collision(Node* n, LineSegment ray, Point& result)
	{
		if (!n) return false;

		int d1 = Direction(n->l.p1, n->l.p2, ray.p1);
		int d2 = Direction(n->l.p1, n->l.p2, ray.p2);

		if (d1 >= 0 && d2 >= 0) {
			return Collision(n->left, ray, result);
		}
		else if (d1 <= 0 && d2 <= 0) {
			return Collision(n->right, ray, result);
		}
		else if (d1 >= 0 && d2 <= 0) // 교차할 경우 (p1은 왼쪽, p2는 오른쪽), d1과 d2가 둘 다 0인 경우는 없다고 가정
		{
			// 가장 가까운 교차점을 찾는 것이 문제 (불필요한 비교를 피해야 함)

			Point r;
			bool check = Intersection(n->l.p1, n->l.p2, ray.p1, ray.p2, r); // 직선-선분 교차

			if (Collision(n->left, { ray.p1, r }, result))
			{
				return true; // 더 가까운 곳에서 충돌이 발생했기 때문에 더 할일이 없음
			}
			else
			{
				if (SegmentsIntersect(n->l.p1, n->l.p2, ray.p1, ray.p2)) // 선분-선분 교차일 경우 
				{
					result = r;
					return true;
				}
				else
				{
					return Collision(n->right, { r, ray.p2 }, result);
				}
			}
		}
		else // if(d1 <= 0 && d2 >= 0) // 교차할 경우 (p1은 오른쪽, p2는 왼쪽), d1과 d2가 둘 다 0인 경우는 없다고 가정
		{
			Point r;
			bool check = Intersection(n->l.p1, n->l.p2, ray.p1, ray.p2, r);

			if (Collision(n->right, { ray.p1, r }, result))
			{
				return true; // 더 가까운 곳에서 충돌이 발생했기 때문에 더 할일이 없음
			}
			else
			{
				if (SegmentsIntersect(n->l.p1, n->l.p2, ray.p1, ray.p2)) // 선분-선분 교차
				{
					result = r;
					return true;
				}
				else
				{
					return Collision(n->left, { r, ray.p2 }, result);
				}
			}
		}
	}

	Node* root = nullptr;

	// 디버깅 편의 도구

	// 루트만 있을 경우 높이 0
	// https://en.wikipedia.org/wiki/Tree_(data_structure)
	int Height() { return Height(root); }
	int Height(Node* node)
	{
		if (!node) return -1;
		return 1 + std::max(Height(node->left), Height(node->right));
	}

	vector<string> screen;
	void PrintLine(int x, string s, string& line) {
		for (const auto c : s) line[x++] = c;
	}
	void Print2D() { Print2D(root); }
	void Print2D(Node* root) {
		if (!root) cout << "Empty" << endl;
		else {
			int h = Height(root) + 1, w = 4 * int(pow(2, h - 1));
			screen.clear();
			screen.resize(h * 2, string(w, ' '));
			Print2D(root, w / 2 - 2, 0, h - 1);
			for (const auto& l : screen) cout << l << endl;
		}
	}
	void Print2D(Node* n, int x, int level, int s)
	{
		PrintLine(x, " " + n->l.name, screen[2 * level]);
		x -= int(pow(2, s));
		if (n->left) {
			PrintLine(x, "  /", screen[2 * level + 1]);
			Print2D(n->left, x, level + 1, s - 1);
		}
		if (n->right)
		{
			PrintLine(x + 2 * int(pow(2, s)), "\\", screen[2 * level + 1]);
			Print2D(n->right, x + 2 * int(pow(2, s)), level + 1, s - 1);
		}
	}
};

void DrawArrow(cv::Mat& image, Point line_start, Point line_end, Scalar color)
{
	if (selected)
		cv::circle(image, *selected, 17, Scalar(0, 255, 0), 1, LINE_AA);

	cv::Point temp = line_end - line_start;
	//cv::line(image, line_start - temp * 1000, line_start + temp * 1000, Scalar(200, 200, 200), 1, LINE_AA);
	cv::circle(image, line_start, 2, color, -1, LINE_AA);
	cv::arrowedLine(image, line_start, line_end, color, 1, LINE_AA, 0, 0.02);
}

int main(int argc, char** argv)
{
	hlab::initialize(1280, 960);

	vector<LineSegment> segments;
	segments.push_back(LineSegment{ {350, 80}, {1050, 80}, "A" });
	segments.push_back(LineSegment{ {1050, 80}, {1050, 400}, "B" });
	segments.push_back(LineSegment{ {1050, 400}, {630, 320}, "C" });
	segments.push_back(LineSegment{ {630, 320}, {560, 560}, "D" });
	segments.push_back(LineSegment{ {560, 560}, {979, 560}, "E" });
	segments.push_back(LineSegment{ {979, 560}, {909, 800}, "F" });
	segments.push_back(LineSegment{ {909, 800}, {420, 720}, "G" });
	segments.push_back(LineSegment{ {420, 720}, {350, 80}, "H" });

	LineSegment ray{ {700, 700}, {700, 160} };

	movable_points.push_back(&ray.p1);
	movable_points.push_back(&ray.p2);

	BSPTree tree;

	for (LineSegment s : segments)
	{
		tree.Insert(s);
	}

	tree.Print2D();

	while (true)
	{
		hlab::preframe();

		if (selected)
		{
			cv::circle(image, *selected, 17, Scalar(0, 255, 0), 1, LINE_AA);

			auto visited = tree.FindLeaf(*selected);

			for (auto& l : visited)
			{
				int d = Direction(l.p1, l.p2, *selected);

				for (int c = 0; c < image.cols; c++)
					for (int r = 0; r < image.rows; r++)
					{
						int d2 = Direction(l.p1, l.p2, Point(c, r));
						if ((d < 0 && d2 > 0) || (d > 0 && d2 < 0))
							image.at<Vec3b>(r, c) = Vec3b(200, 200, 200); // 밝은 회색

						// visit 한 곳들을 회색으로 모두 칠했기 때문에
						// selected를 둘러싼 지점만 흰색으로 남게 됩니다.
					}
			}
		}

		int count = 0;
		auto visited = tree.Traverse(ray.p1);
		for (LineSegment s : visited)
		{
			// cout << s.name << " ";
			count += 1;
			DrawArrow(image, s.p1, s.p2, Scalar(30, 30, 30));
			cv::putText(image, s.name + "_" + std::to_string(count), (s.p1 + s.p2) / 2, cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(30, 30, 30), 1, LINE_AA, true);
		}

		cv::arrowedLine(image, ray.p1, ray.p2, Scalar(255, 125, 125), 3, LINE_AA, 0, 0.05);

		Point collision_point;
		if (tree.Collision(ray, collision_point))
			cv::circle(image, collision_point, 8, Scalar(30, 30, 255), -1, LINE_AA);

		if (hlab::postframe()) break;
	}

	return 0;
}
