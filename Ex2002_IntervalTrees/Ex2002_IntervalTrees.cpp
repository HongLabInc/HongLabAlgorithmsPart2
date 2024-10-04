#include <iostream>
#include <string>
#include <assert.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

// Sedgewick 교재의 자바 구현 예시
// https://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html

//using Key = int;   // 범위 [low, high]에서 low를 key로 사용
//using Value = int; // 생략

enum Color { kRed, kBlack };
// C-style로 구현한다면
// #define RED true
// #define BLACK false

class Node
{
public:
	int low;      // 이 값을 key로 사용
	int high;
	int max_high; // 이 노드를 루트로 하는 서브트리에서 가장 큰 high
	// Key key;   // 범위 [low, high]에서 low를 key로 사용
	// Value val; // 생략
	Node* left;
	Node* right;
	int size;     // 이 노드를 루트로 하는 서브트리의 크기(노드의 수)
	Color color;

	Node(int low, int high, int N, Color color)
		: low(low), high(high),
		size(N), color(color),
		max_high(high), // <- 새로 추가
		left(nullptr), right(nullptr)
	{
		assert(low <= high);
	}
};

class IntervalTree
{
public:
	Node* root = nullptr;

	bool IsEmpty() { return root == nullptr; }

	bool IsRed(Node* x)
	{
		if (x == nullptr) return false; // 널노드는 블랙
		return x->color == Color::kRed;
	}

	int Size() { return Size(root); }
	int Size(Node* x)
	{
		if (x == nullptr) return 0;
		else return x->size;
	}

	int MaxHigh(Node* n)
	{
		if (!n) return -1000; // 편의상 무한소 대신에 -1000 사용
		else return n->max_high;
	}

	// [l1, h1]과 [l2, h2]가 겹치는지 확인
	bool Overlap(int l1, int h1, int l2, int h2)
	{
		assert(l1 <= h1);
		assert(l2 <= h2);

		// if ( TODO: ) return true;
		return false;
	}

	Node* SearchOne(int low, int high)
	{
		Node* n = root;

		//while (n != nullptr && !Overlap(n->low, n->high, low, high))
		//{
		//	if (n->left != nullptr && TODO: 조건 추가)
		//		n = n->left;
		//	else
		//		n = n->right;
		//}

		// 편의상 여기서 출력
		cout << "[" << low << "," << high << "] overlaps: ";
		if (n)
			cout << "[" << n->low << "," << n->high << "] " << endl;
		else
			cout << "None" << endl;

		return n;
	}

	vector<Node*> SearchAll(int low, int high)
	{
		vector<Node*> overlapped;

		SearchAll(root, low, high, overlapped);

		// 편의상 여기서 출력
		cout << "[" << low << "," << high << "] overlaps:" << endl;
		if (overlapped.empty()) cout << "None" << endl;
		else
		{
			for (Node* n : overlapped)
				cout << "[" << n->low << "," << n->high << "] ";
			cout << endl;
		}

		return overlapped;
	}

	// CLRS 3판 14.3-4, 4판 기준 17.3-3.
	// https://sites.math.rutgers.edu/~ajl213/CLRS/Ch14.pdf (풀이에 오류 있음, right subtree의 low의 최소값은 n의 low 이상이어야 함)
	void SearchAll(Node* n, int low, int high, vector<Node*>& overlapped)
	{
		assert(low <= high);

		//cout << "Begin " << "[" << n->low << "," << n->high << "]" << endl;

		if (Overlap(n->low, n->high, low, high))
			overlapped.push_back(n);

		// 힌트:
		// n->left를 루트로 하는 서브트리의 
		// - 최소는 n->low보다 작다는 것 외에는 범위를 정확히 알 수 없음 (고려할 필요 없음)
		// - 최대는 max_high
		//if (n->left && TODO: 조건 추가)
		//	SearchAll(n->left, low, high, overlapped);

		// 힌트:
		// n->right를 루트로 하는 서브트리의
		// - 최소는 n->low (이진탐색트리의 성질)
		// - 최대는 n->right->max_high
		//if (n->right && TODO: && TODO: )
		//	SearchAll(n->right, low, high, overlapped);

		//cout << "End " << "[" << n->low << "," << n->high << "]" << endl;
	}

	Node* RotateLeft(Node* h)
	{
		Node* x = h->right;
		h->right = x->left;
		x->left = h;
		x->color = h->color;
		h->color = Color::kRed;
		x->size = h->size;
		h->size = 1 + Size(h->left) + Size(h->right);
		assert(h->size == RecurSize(h));

		// h->max_high = TODO: max_high 업데이트
		// assert(h->max_high == RecurMaxHigh(h));

		// x->size, x->max_high는 Balance() 마지막에 다시 수정

		return x;
	}

	Node* RotateRight(Node* h)
	{
		Node* x = h->left;
		h->left = x->right;
		x->right = h;
		x->color = h->color;
		h->color = Color::kRed;

		x->size = h->size;
		h->size = 1 + Size(h->left) + Size(h->right);
		assert(h->size == RecurSize(h));

		// h->max_high = TODO: max_high 업데이트
		// assert(h->max_high == RecurMaxHigh(h));

		// x->size, x->max_high는 Balance() 마지막에 다시 수정

		return x;
	}

	void FlipColor(Color& c)
	{
		if (c == Color::kRed)
			c = Color::kBlack;
		else
			c = Color::kRed;
	}

	void FlipColors(Node* h)
	{
		FlipColor(h->color);
		FlipColor(h->left->color);
		FlipColor(h->right->color);
	}

	Node* Balance(Node* h)
	{
		assert(h != nullptr);

		if (IsRed(h->right) && !IsRed(h->left)) h = RotateLeft(h);
		if (IsRed(h->left) && IsRed(h->left->left)) h = RotateRight(h);
		if (IsRed(h->left) && IsRed(h->right)) FlipColors(h);

		h->size = Size(h->left) + Size(h->right) + 1;
		assert(h->size == RecurSize(h));

		// h->max_high = TODO: max_high 업데이트
		// assert(h->max_high == RecurMaxHigh(h));

		return h;
	}

	void Insert(int low, int high)
	{
		root = Insert(root, low, high);
		root->color = Color::kBlack; // 루트는 블랙
	}

	Node* Insert(Node* h, int low, int high)
	{
		if (h == nullptr)
			return new Node(low, high, 1, Color::kRed); // 새 노드는 일단 레드

		if (low < h->low) h->left = Insert(h->left, low, high);
		else h->right = Insert(h->right, low, high); // 키가 같으면 오른쪽에 추가

		return Balance(h); // 자식들의 색을 보고 Balance 수정
	}

	Node* MoveRedLeft(Node* h) // 삭제할 때 사용됨
	{
		FlipColors(h);

		if (IsRed(h->right->left))
		{
			h->right = RotateRight(h->right);
			h = RotateLeft(h);
			FlipColors(h);
		}

		return h;
	}

	Node* MoveRedRight(Node* h) // 삭제할 때 사용됨 (MoreRedLeft와 좌우대칭)
	{
		FlipColors(h);

		if (IsRed(h->left->left))
		{
			h = RotateRight(h);
			FlipColors(h);
		}

		return h;
	}

	int RecurSize() { return RecurSize(root); } // 재귀호출로 다시 계산하는 방식, 디버깅용
	int RecurSize(Node* n)
	{
		if (!n) return 0;
		return 1 + RecurSize(n->left) + RecurSize(n->right);
	}

	int RecurMaxHigh() { return RecurMaxHigh(root); }; // 재귀호출로 계산하는 방식, 디버깅용
	int RecurMaxHigh(Node* n)
	{
		if (!n) return -1000; // 편의상 무한소 대신에 -1000 사용
		return std::max(n->high, std::max(RecurMaxHigh(n->left), RecurMaxHigh(n->right)));
	}

	// Delete 관련 함수들 생략

	// 루트만 있을 경우 높이 0
	// https://en.wikipedia.org/wiki/Tree_(data_structure)
	int Height() { return Height(root); }
	int Height(Node* node)
	{
		if (!node) return -1;
		return 1 + std::max(Height(node->left), Height(node->right));
	}

	// 디버깅 편의 도구
	vector<string> screen;
	void PrintLine(int x, string s, string& line) {
		for (const auto c : s) line[x++] = c;
	}
	void Print2D() { Print2D(root); }
	void Print2D(Node* root) {
		if (!root) cout << "Empty" << endl;
		else {
			int h = max(5, Height(root) + 1), w = 8 * int(pow(2, h - 1));
			screen.clear();
			screen.resize(h * 2, string(w, ' '));
			Print2D(root, w / 2 - 2, 0, h - 1);
			for (const auto& l : screen) cout << l << endl;
		}
	}
	void Print2D(Node* n, int x, int level, int s)
	{
		PrintLine(x, (IsRed(n) ? "*[" : " [") + to_string(n->low) + "," + to_string(n->high) + "]" + to_string(RecurMaxHigh(n)), screen[2 * level]);
		x -= int(pow(2, s));
		if (n->left) {
			PrintLine(x, "      /", screen[2 * level + 1]);
			Print2D(n->left, x, level + 1, s - 1);
		}
		if (n->right)
		{
			PrintLine(x + 2 * int(pow(2, s)), "    \\", screen[2 * level + 1]);
			Print2D(n->right, x + 2 * int(pow(2, s)), level + 1, s - 1);
		}
	}
};

int main()
{
	// key를 추가하는 순서에 따라서 트리 구조가 달라집니다.

	// CLRS 예제
	{
		vector<pair<int, int>> intervals = { {16, 21}, {8, 9}, {25, 30}, {5, 8}, {15, 23}, {17, 19}, {26, 26}, { 0, 3 }, {6, 10}, {19, 20} };

		IntervalTree itree;
		for (pair<int, int> i : intervals)
		{
			itree.Insert(i.first, i.second);
		}

		itree.Print2D();

		itree.SearchOne(22, 25); // [15,23]
		itree.SearchOne(11, 14); // Not found

		itree.SearchAll(0, 30);  // [16,21] [8,9] [5,8] [0,3] [6,10] [15,23] [25,30] [19,20] [17,19] [26,26] (전체 범위)
		itree.SearchAll(15, 20); // [16,21] [15,23] [19,20] [17,19]
		itree.SearchAll(5, 8);   // [8,9] [5,8] [6,10]
		itree.SearchAll(26, 26); // [25,30] [26,26]
		itree.SearchAll(8, 17);  // [16,21] [8,9] [5,8] [6,10] [15,23] [17,19]
	}

	// https://www.geeksforgeeks.org/interval-tree/ 예제
	{
		vector<pair<int, int>> intervals = { { 15, 20 }, { 10, 30 }, { 17, 19 }, { 5, 20 }, { 12, 15 }, { 30, 40 } };

		IntervalTree itree;
		for (pair<int, int> i : intervals)
		{
			itree.Insert(i.first, i.second);
		}

		itree.Print2D();

		itree.SearchOne(6, 7); // [5,20]
		itree.SearchAll(16, 19); // [15,20] [10,30] [5,20] [17,19]
	}

	return 0;
}