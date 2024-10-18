#pragma once

#include <vector>
#include <list>
#include <assert.h>
#include <iostream>
#include <set>
#include <opencv2/core/core.hpp> // cv::Point 사용

namespace hlab
{
	using namespace std;

	class BalancedQuadtree
	{
	public:
		using Point = cv::Point; // 별도의 Point 구조체를 만들 수 있으나, 여기서는 편의상 OpenCV Point 사용 (큰 차이 없어요)

		struct Node
		{
			Node(int _x, int _y, int _width, int _height, int _level)
				:
				x(_x), y(_y), width(_width), height(_height), level(_level)
				, children(4, nullptr), index(0) // 4 children
			{
				// children[0] : left bottom child
				// children[1] : right bottom child
				// children[2] : left top child
				// children[3] : right top child
			}

			// [x, x + width - 1] 범위

			int x, y;
			int width, height; // 보통 정사각형 사용
			int level;
			int index;
			vector<Node*> children;
			set<Node*> neighbors; // 리프 노드들만 포함
			list<Point> objects;

			bool HasChild()
			{
				for (auto* c : children)
					if (c) return true;
				return false;
			}

			Point Center()
			{
				return Point{ x + width / 2, y + height / 2 };
			}
		};

		Node* root = nullptr;
		int max_level = 5;
		vector<Node*> leaves;
		int x = 0;
		int y = 0;
		int width = 1280;
		int height = 960;

		BalancedQuadtree(int _x, int _y, int _width, int _height, int _max_level)
			: x(_x), y(_y), width(_width), height(_height), max_level(_max_level)
		{
		}

		~BalancedQuadtree()
		{
			DeleteAll();
		}

		void DeleteAll()
		{
			DeleteAll(root);
		}

		void DeleteAll(Node* n)
		{
			if (!n) return;

			for (auto& c : n->children)
			{
				if (c)
				{
					DeleteAll(c);
					delete c;
					c = nullptr;
				}
			}
		}

		void SplitMax()
		{
			SplitMax(root);
		}

		void SplitMax(Node* n)
		{
			if (n->level == max_level)
				return;

			if (!n->children[0]) // 아직 자식 노드가 없다면 4개 모두 생성 (0번만 체크)
			{
				n->children[0] = new Node(n->x, n->y, n->width / 2, n->height / 2, n->level + 1);
				n->children[1] = new Node(n->x + n->width / 2, n->y, n->width / 2, n->height / 2, n->level + 1);
				n->children[2] = new Node(n->x, n->y + n->height / 2, n->width / 2, n->height / 2, n->level + 1);
				n->children[3] = new Node(n->x + n->width / 2, n->y + n->height / 2, n->width / 2, n->height / 2, n->level + 1);
			}

			for (auto* c : n->children)
			{
				SplitMax(c);
			}
		}

		bool Insert(Point p, bool split_only)
		{
			if (!root)
				root = new Node(x, y, width, height, 0);

			bool result = Insert(root, p, max_level, split_only);

			// cout << (result ? "added" : "not added") << endl; // 디버깅용

			return result;
		}

		bool Insert(Node* n, Point p, int target_level, bool split_only) // split_only = true이면 쪼개기만 하고 point를 추가하지는 않음
		{
			// 힌트: 연산자 우선순위 https://en.cppreference.com/w/cpp/language/operator_precedence
			if (p.x < n->x || p.x >= n->x + n->width || p.y < n->y || p.y >= n->y + n->height)
			{
				// cout << "Not included " << p.x << " " << p.y << endl;
				return false; // 영역을 벗어나기 때문에 조기 종료
			}

			if (n->level == target_level) // leaf node
			{
				if (!split_only)
					n->objects.push_back(p);

				return true;
			}

			if (!n->children[0]) // 아직 자식 노드가 없다면 4개 모두 생성 (0번만 체크)
			{
				n->children[0] = new Node(n->x, n->y, n->width / 2, n->height / 2, n->level + 1);
				n->children[1] = new Node(n->x + n->width / 2, n->y, n->width / 2, n->height / 2, n->level + 1);
				n->children[2] = new Node(n->x, n->y + n->height / 2, n->width / 2, n->height / 2, n->level + 1);
				n->children[3] = new Node(n->x + n->width / 2, n->y + n->height / 2, n->width / 2, n->height / 2, n->level + 1);

				// 이웃 노드와의 밸런스 맞춰주기
				vector<Point> samples =
				{
					{n->x - 1, n->y - 1}, {n->x + n->width, n->y + n->height}, {n->x - 1, n->y + n->height}, {n->x + n->width, n->y - 1},
					{n->x - 1, n->y}, {n->x, n->y - 1},{n->x + n->width, n->y}, {n->x, n->y + n->height}
				};
				for (Point& s : samples)
				{
					Node* adj = FindLeaf(s);
					if (adj && adj->level < n->level - 1)
						Insert(adj, s, n->level - 1, true);
				}
			}

			if (p.y < n->y + n->height / 2)
			{
				if (p.x < n->x + n->width / 2)
					return Insert(n->children[0], p, target_level, split_only);
				else
					return Insert(n->children[1], p, target_level, split_only);
			}
			else
			{
				if (p.x < n->x + n->width / 2)
					return Insert(n->children[2], p, target_level, split_only);
				else
					return Insert(n->children[3], p, target_level, split_only);
			}
		}

		Node* FindLeaf(Point p)
		{
			return FindLeaf(root, p);
		}

		Node* FindLeaf(Node* n, Point p)
		{
			if (p.x < n->x || p.x >= n->x + n->width || p.y < n->y || p.y >= n->y + n->height)
			{
				// cout << "Not included " << p.x << " " << p.y << endl;
				return nullptr; // 영역을 벗어나기 때문에 조기 종료
			}

			if (n->HasChild())
			{
				if (p.y < n->y + n->height / 2)
				{
					if (p.x < n->x + n->width / 2)
						return FindLeaf(n->children[0], p);
					else
						return FindLeaf(n->children[1], p);
				}
				else
				{
					if (p.x < n->x + n->width / 2)
						return FindLeaf(n->children[2], p);
					else
						return FindLeaf(n->children[3], p);
				}
			}
			else return n;
		}

		void FindNeighbors()
		{
			leaves.clear();

			FindNeighbors(root);
		}

		void FindNeighbors(Node* n) // 리프 노드들의 이웃들을 찾는다.
		{
			if (n->children[0])
			{
				for (auto& c : n->children)
					FindNeighbors(c);
			}
			else // leaf node
			{
				n->index = int(leaves.size());
				leaves.push_back(n);

				// 주의: 화면 해상도 대비 max_level이 너무 크면 한 노드의 크기가 너무 작아져서 문제가 생깁니다.
				vector<Point> samples = {
					{n->x - 1, n->y - 1}, {n->x + n->width, n->y + n->height}, {n->x - 1, n->y + n->height}, {n->x + n->width, n->y - 1},
					{n->x - 1, n->y}, {n->x, n->y - 1},{n->x + n->width, n->y}, {n->x, n->y + n->height}
				};

				for (auto& s : samples)
				{
					auto* adj = FindLeaf(s);
					if (adj && adj->level <= n->level)
					{
						n->neighbors.insert(adj);
						adj->neighbors.insert(n);
					}
				}
			}
		}
	};
}
