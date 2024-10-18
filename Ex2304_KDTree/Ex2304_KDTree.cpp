#include "../shared/draw_helper.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace cv;
using namespace hlab;

// 구현 기본 틀: https://users.cis.fiu.edu/~weiss/dsaa_c++4/code/KdTree.cpp
class KdTree
{
public:
	struct Node
	{
		vector<int> data; // 점 저장

		Node* left = nullptr;
		Node* right = nullptr;

		// 디버깅 용도
		vector<int> minpos;
		vector<int> maxpos; // minpos <= x < maxpos

		int d = 0; // 어느 차원으로 분할했는지 기록 (이해하기가 쉽습니다.)
	};

	Node* root = nullptr;

	KdTree()
		: root(nullptr)
	{}

	// 한꺼번에 여러 개를 추가하는 방식
	void Insert(vector<vector<int>> v)
	{
		Insert(v, root, 0, { -10000, -10000 }, { 10000, 10000 });
	}

	void Insert(vector<vector<int>> v, Node*& n, int d, vector<int> minpos, vector<int> maxpos)
	{
		if (v.empty()) return;

		if (!n)
		{
			auto m = v.begin() + v.size() / 2;

			std::nth_element(v.begin(), m, v.end(), [&](const vector<int>& a, const vector<int>& b) {return a[d] < b[d]; }); // TODO: root의 d에 맞춰서 정렬

			vector<int> x = v[v.size() / 2];

			n = new Node{ x };
			n->minpos = minpos;
			n->maxpos = maxpos;
			n->d = d;

			vector<vector<int>> v_left(v.begin(), m); // m is not included
			vector<vector<int>> v_right(m + 1, v.end());

			vector<int> left_minpos = minpos, left_maxpos = maxpos;
			vector<int> right_minpos = minpos, right_maxpos = maxpos;
			left_maxpos[d] = n->data[d];
			right_minpos[d] = n->data[d];

			// (d + 1) % int(x.size()) <- 분할하는 방향을 바꿔가면서 재귀진행
			Insert(v_left, n->left, (d + 1) % int(x.size()), left_minpos, left_maxpos);
			Insert(v_right, n->right, (d + 1) % int(x.size()), right_minpos, right_maxpos);
		}
	}

	void Insert(vector<int> x)
	{
		Insert(x, root, 0, { -10000, -10000 }, { 10000, 10000 });
	}

	void Insert(vector<int> x, Node*& n, int d, vector<int> minpos, vector<int> maxpos)
	{
		if (!n) // 새로운 노드 생성
		{
			n = new Node{ x };
			n->minpos = minpos;
			n->maxpos = maxpos;
			n->d = d;
		}
		else // 왼쪽 또는 오른쪽으로 내려가기
		{
			vector<int> new_minpos = minpos;
			vector<int> new_maxpos = maxpos;

			if (x[d] < n->data[d])
			{
				new_maxpos[d] = n->data[d];

				Insert(x, n->left, (d + 1) % int(x.size()), new_minpos, new_maxpos);
			}
			else
			{
				new_minpos[d] = n->data[d];
				Insert(x, n->right, (d + 1) % int(x.size()), new_minpos, new_maxpos);
			}
		}
	}

	vector<Node*> Traverse()
	{
		vector<Node*> visited;

		Traverse(root, visited);

		return visited;
	}

	void Traverse(Node* n, vector<Node*>& visited)
	{
		if (!n) return;

		visited.push_back(n);

		if (n->left) Traverse(n->left, visited);
		if (n->right) Traverse(n->right, visited);
	}

	Node* FindLeaf(vector<int> x)
	{
		if (!root) return nullptr;

		return FindLeaf(root, x);
	}

	Node* FindLeaf(Node* n, vector<int> x)
	{
		if (x[n->d] < n->data[n->d])
		{
			if (!n->left) return n;
			else return FindLeaf(n->left, x);
		}
		else
		{
			if (!n->right) return n;
			else return FindLeaf(n->right, x);
		}
	}

	double Dist(vector<int> p1, vector<int> p2)
	{
		int sum = 0;
		for (int i = 0; i < p1.size(); i++)
			sum += (p1[i] - p2[i]) * (p1[i] - p2[i]); // (p1.x - p2.x) * (p1.y - p2.y) = (p1[0] - p2[0]) * (p1[1] - p2[2])
		return std::sqrt(sum);
	}

	vector<int> FindNearest(vector<int> x, vector<Node*>& visited) // visited는 가시화 용도
	{
		if (!root) return {};

		vector<int> nearest = { 10000, 10000 };
		double dist = Dist(x, nearest);

		FindNearest(root, x, nearest, dist, visited);

		return nearest;
	}

	// x와 nearest는 크기가 2인 vector<int>입니다. 2차원의 좌표(Point)를 의미합니다.
	// x와 가장 가까운 점을 찾아서 nearest에 저장하고 x와 nearest의 거리도 dist에 저장합니다.
	// visited는 찾는 과정에서 방문한 노드들을 기록해서 가시화할때 사용합니다.
	void FindNearest(Node* n, vector<int> x, vector<int>& nearest, double& dist, vector<Node*>& visited)
	{
		visited.push_back(n);

		double temp = Dist(n->data, x);

		if (temp < dist) // 더 가까운 점을 발견하면 덮어쓰기
		{
			nearest = n->data;
			dist = temp;
		}

		if (x[n->d] < n->data[n->d]) // x가 왼쪽(d = 0인 경우) 또는 아래쪽(d = 1인 경우)
		{
			if (n->left)
				FindNearest(n->left, x, nearest, dist, visited);

			// 경계선을 건너서 더 가까운 것이 존재할 가능성이 있을 때는 반대편(n->right)도 탐색
			if (n->right) // TODO: 현재는 모든 노드를 찾는 방식, 조건을 추가해서 탐색 범위 줄이기
				FindNearest(n->right, x, nearest, dist, visited);
		}
		else
		{
			if (n->right)
				FindNearest(n->right, x, nearest, dist, visited);

			// 경계선을 건너서 더 가까운 것이 존재할 가능성이 있을 때는 반대편(n->left)도 탐색
			if (n->left) // TODO: 현재는 모든 노드를 찾는 방식, 조건을 추가해서 탐색 범위 줄이기
				FindNearest(n->left, x, nearest, dist, visited);
		}
	}

	/*
	 * Print items satisfying
	 * low[ 0 ] <= x[ 0 ] <= high[ 0 ] and
	 * low[ 1 ] <= x[ 1 ] <= high[ 1 ]
	 */
	void PrintRange(vector<int> low, vector<int> high) const
	{
		PrintRange(low, high, root, 0);
	}

	void PrintRange(vector<int>& low, vector<int>& high, Node* n, int d) const
	{
		if (n != nullptr)
		{
			// 현재 노드 n이 가지고 있는 데이터가 범위에 포함된다면 출력
			if (low[0] <= n->data[0] && high[0] >= n->data[0] &&
				low[1] <= n->data[1] && high[1] >= n->data[1])
				cout << "(" << n->data[0] << "," << n->data[1] << ")" << endl;

			if (low[d] <= n->data[d])
				PrintRange(low, high, n->left, (d + 1) % int(low.size()));

			if (n->data[d] <= high[d])
				PrintRange(low, high, n->right, (d + 1) % int(low.size()));
		}
	}
};

int main()
{
	hlab::initialize(1280, 960);

	KdTree t;

	vector<Point> points;

	//points.push_back({ 500, 500 });
	//points.push_back({ 300, 500 });
	//points.push_back({ 510, 800 });
	//points.push_back({ 800, 400 });
	//points.push_back({ 1000, 900 });
	//points.push_back({ 1300, 700 });
	//points.push_back({ 1800, 300 });

	for (int i = 0; i < 30; i++)
		points.push_back({ rand() % image.cols, rand() % image.rows });

	// kd-tree에서는 임의의 차원(dimension)에 대해 구현하기 위해서
	// Point대신에 vector<int>를 사용합니다.
	// Point(123, 456)과 vector<int>{123, 456}은 둘 다 2차원 포인트를 의미합니다.

	vector<vector<int>> v;
	for (auto& p : points)
		v.push_back({ p.x, p.y });

	t.Insert(v);

	vector<int> low(2), high(2);
	low[0] = 300;
	low[1] = 300;
	high[0] = 800;
	high[1] = 800;

	t.PrintRange(low, high);

	vector<KdTree::Node*> visited = t.Traverse();

	Point reference(image.cols / 2, image.rows / 2);
	movable_points.push_back(&reference);

	while (true)
	{
		hlab::preframe();

		auto* leaf = t.FindLeaf({ reference.x, reference.y });

		vector<KdTree::Node*> nearest_visited;
		vector<int> nearest = t.FindNearest({ reference.x, reference.y }, nearest_visited);

		double dist = 1000000;
		vector<int> temp;
		for (auto p : points)
		{
			if (cv::norm(p - reference) < dist)
			{
				temp = { p.x, p.y };
				dist = cv::norm(p - reference);
			}
		}

		if (nearest[0] != temp[0] || nearest[1] != temp[1])
		{
			cout << "Wrong nearest" << endl;
			exit(-1);
		}

		if (leaf)
		{
			auto minpos = leaf->minpos;
			auto maxpos = leaf->maxpos;

			if (leaf->d == 0)
			{
				if (reference.x < leaf->data[0])
					maxpos[0] = leaf->data[0];
				else
					minpos[0] = leaf->data[0];
			}
			else
			{
				if (reference.y < leaf->data[1])
					maxpos[1] = leaf->data[1];
				else
					minpos[1] = leaf->data[1];
			}

			Rect r(minpos[0], minpos[1], maxpos[0] - minpos[0],
				maxpos[1] - minpos[1]);

			cv::rectangle(image, r, kBrightGray, -1, LINE_AA);
		}

		for (auto* n : visited)
		{
			if (n->d == 0)
				cv::line(image, Point(n->data[0], n->minpos[1]), Point(n->data[0], n->maxpos[1]), Scalar(200, 200, 200), 1, LINE_AA);
			else
				cv::line(image, Point(n->minpos[0], n->data[1]), Point(n->maxpos[0], n->data[1]), Scalar(200, 200, 200), 1, LINE_AA);
		}

		for (Point p : points)
		{
			cv::circle(image, p, 5, Scalar(100, 100, 100), -1, LINE_AA);
		}

		for (auto* n : nearest_visited) // 방문한 곳들만 노란색으로 (모두 방문하는 것이 아님)
		{
			if (n->d == 0)
				cv::line(image, Point(n->data[0], n->minpos[1]), Point(n->data[0], n->maxpos[1]), Scalar(0, 255, 255), 1, LINE_AA);
			else
				cv::line(image, Point(n->minpos[0], n->data[1]), Point(n->maxpos[0], n->data[1]), Scalar(0, 255, 255), 1, LINE_AA);

			cv::circle(image, { n->data[0], n->data[1] }, 5, Scalar(0, 255, 255), -1, LINE_AA);
		}

		if (!nearest.empty())
		{
			cv::circle(image, Point(nearest[0], nearest[1]), 8, Scalar(0, 0, 0), -1, LINE_AA);
		}

		if (leaf) // 빨간 점은 leaf node의 점, 더 큰 검은 점은 가장 가까운 점 (둘이 일치하지 않을 수 있다는 것 강조)
		{
			cv::circle(image, Point(leaf->data[0], leaf->data[1]), 5, Scalar(100, 100, 255), -1, LINE_AA);
		}

		cv::circle(image, reference, 8, Scalar(255, 100, 100), -1, LINE_AA);
		if (selected)
			cv::circle(image, *selected, 8, Scalar(0, 255, 0), 1, LINE_AA);

		if (hlab::postframe()) break;
	}

	return 0;
}
