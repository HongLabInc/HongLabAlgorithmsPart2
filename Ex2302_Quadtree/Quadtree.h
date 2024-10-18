#pragma once

#include <vector>
#include <list>
#include <assert.h>
#include <iostream>
#include <opencv2/core/core.hpp> // cv::Point ���

namespace hlab
{
	using namespace std;

	class Quadtree
	{
	public:
		using Point = cv::Point; // ������ Point ����ü�� ���� �� ������, ���⼭�� ���ǻ� OpenCV Point ��� (ū ���� �����)

		//struct Point
		//{
		//	int x, y;
		//	bool operator == (const Point& p) const
		//	{
		//		return x == p.x && y == p.y;
		//	}
		//};

		struct Node
		{
			Node(int _x, int _y, int _width, int _height, int _level) :
				x(_x), y(_y), width(_width), height(_height), level(_level),
				children(4, nullptr) // 4 children
			{
				// children[0] : left bottom child
				// children[1] : right bottom child
				// children[2] : left top child
				// children[3] : right top child
			}

			// [x, x + width - 1] x [y, y + height - 1] ����

			int x, y;
			int width, height; // ���� ���簢�� ���
			int level;
			vector<Node*> children;
			list<Point> objects;

			bool HasChild()
			{
				for (auto* c : children)
					if (c) return true;
				return false;
			}

			bool Intersect(Point p, int r) // p�� �߽��̰� �������� r�� �� ��� �簢���� ����� �ƴ��� �˻�
			{
				int closest_x = max(x, min(p.x, x + width));
				int closest_y = max(y, min(p.y, y + height));

				if (cv::norm(Point(closest_x, closest_y) - p) < r)
					return true;
				else
					return false;
			}
		};

		Node* root = nullptr;
		int max_level = 5;
		int x = 0;
		int y = 0;
		int width = 1280;
		int height = 960;

		Quadtree(int _x, int _y, int _width, int _height, int _max_level)
			: x(_x), y(_y), width(_width), height(_height), max_level(_max_level)
		{
		}

		~Quadtree()
		{
			if (root)
				Delete(root);
		}

		void Delete(Node* n)
		{
			if (!n) return;

			for (auto& c : n->children)
			{
				if (c)
				{
					Delete(c);
					delete c;
					c = nullptr;
				}
			}
		}

		bool Insert(Point p)
		{
			if (!root)
				root = new Node(x, y, width, height, 0);

			bool result = Insert(root, p);

			return result;
		}

		bool Insert(Node* n, Point p)
		{
			// ��Ʈ: ������ �켱���� https://en.cppreference.com/w/cpp/language/operator_precedence
			if (p.x < n->x || p.x >= n->x + n->width || p.y < n->y || p.y >= n->y + n->height)
			{
				cout << "Not included " << p.x << " " << p.y << endl;

				return false; // ������ ����� ������ �� �������� �ʰ� ���� ����
			}

			if (n->level == max_level) // leaf node
			{
				n->objects.push_back(p);
				return true;
			}
			else if (p.y < n->y + n->height / 2)
			{
				return false; // TODO: ����

				//if (p.x < n->x + n->width / 2)
				//{
				//	if (!n->children[0])
				//		n->children[0] = new Node( TODO: );

				//	return Insert(n->children[0], p);
				//}
				//else
				//{
				//	if (!n->children[1])
				//		n->children[1] = new Node( TODO: );

				//	return Insert(n->children[1], p);
				//}
			}
			else
			{
				return false; // TODO: ����

				//if (p.x < n->x + n->width / 2)
				//{
				//	if (!n->children[2])
				//		n->children[2] = new Node( TODO: );

				//	return Insert(n->children[2], p);
				//}
				//else
				//{
				//	if (!n->children[3])
				//		n->children[3] = new Node( TODO: );

				//	return Insert(n->children[3], p);
				//}
			}
		}

		vector<Node*> FindLeaves(Point p, int radius)
		{
			vector<Node*> nodes;
			FindLeaves(root, p, radius, nodes);
			return nodes;
		}

		void FindLeaves(Node* n, Point p, int radius, vector<Node*>& nodes)
		{
			if (n->level == max_level) // leaf node
			{
				nodes.push_back(n);
			}
			else
			{
				for (Node* c : n->children)
				{
					if (c && c->Intersect(p, radius))
						FindLeaves(c, p, radius, nodes);
				}
			}
		}
	};
}
