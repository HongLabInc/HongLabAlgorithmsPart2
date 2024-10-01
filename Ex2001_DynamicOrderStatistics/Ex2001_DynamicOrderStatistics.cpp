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

using Key = int; // 주의: 앞에서는 string이었습니다.
using Value = int;

enum Color { kRed, kBlack };
// C-style로 구현한다면
// #define RED true
// #define BLACK false

class Node
{
public:
	Key key;
	Value val;
	Node* left;
	Node* right;
	int size; // Number of nodes in this subtree
	Color color;

	Node(Key key, Value val, int N, Color color)
		: key(key), val(val), size(N), color(color),
		left(nullptr), right(nullptr) // 널 포인터로 초기화
	{}
};

// RedBlackBST에 k-select 기능을 추가한 것입니다.
// 키가 같은 경우에는 (값을 덮어쓰는 대신에) 오른쪽으로 추가하도록 구현되어 있습니다.
class OrderStatisticTree
{
public:
	Node* root = nullptr;

	Node* Select(int k) // key가 k번째인 Node* 반환
	{
		return Select(root, k);
	}
	Node* Select(Node* n, int k)
	{
		return nullptr;	// TODO: 
	}

	// key가 같은 노드가 여러 개일 경우에는 아무거나 허용
	int Rank(Key key) { return Rank(root, key); }
	int Rank(Node* n, Key key)
	{
		return -1; // TODO:
	}

	// 참고: CLRS 교재의 OS-RANK()는 각 노드가 부모 포인터를 갖고 있는 경우에 대해 구현되어 있어서
	//      이 예제와 다릅니다.

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

	Value Search(Key key) { return Search(root, key); }
	Value Search(Node* x, Key key)
	{
		if (x == nullptr) return -1; // 편의상 못 찾았을 경우 -1 반환
		if (key < x->key) return Search(x->left, key);
		else if (key > x->key) return Search(x->right, key);
		else return x->val;
	}

	bool Contains(Key key) { return Contains(root, key); }
	bool Contains(Node* x, Key key)
	{
		if (!x) return false;
		if (key < x->key) return Contains(x->left, key);
		else if (key > x->key) return Contains(x->right, key);
		else return true;
	}

	Key Min()
	{
		assert(!IsEmpty());
		return Min(root)->key;
	}
	Node* Min(Node* x)
	{
		return x->left == nullptr ? x : Min(x->left);
	}

	// 이진트리와 동일
	Key Max()
	{
		assert(!IsEmpty());
		return Max(root)->key;
	}
	Node* Max(Node* x)
	{
		return (x->right == nullptr) ? x : Max(x->right);
	}

	Node* RotateLeft(Node* h)
	{
		Node* x = h->right; // 회전 후에 부모 자리로 올라갈 노드
		h->right = x->left;
		x->left = h;
		x->color = h->color;
		h->color = Color::kRed; // 일단 레드로 설정 후 나중에 수정
		x->size = h->size;
		h->size = 1 + Size(h->left) + Size(h->right);

		// x->size는 Balance() 마지막에 다시 수정
		assert(h->size == RecurSize(h));

		return x;
	}

	Node* RotateRight(Node* h)
	{
		Node* x = h->left; // 회전 후에 부모 자리로 올라갈 노드
		h->left = x->right;
		x->right = h;
		x->color = h->color;
		h->color = Color::kRed; // 일단 레드로 설정 후 나중에 수정

		x->size = h->size;
		h->size = 1 + Size(h->left) + Size(h->right);

		// x->size는 Balance() 마지막에 다시 수정
		assert(h->size == RecurSize(h));

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

	Node* Balance(Node* h) 	// restore red-black tree invariant
	{
		assert(h != nullptr);

		if (IsRed(h->right) && !IsRed(h->left)) h = RotateLeft(h);
		if (IsRed(h->left) && IsRed(h->left->left)) h = RotateRight(h);
		if (IsRed(h->left) && IsRed(h->right)) FlipColors(h);

		h->size = Size(h->left) + Size(h->right) + 1;
		assert(h->size == RecurSize(h));

		return h;
	}

	void Insert(Key key, Value val)
	{
		root = Insert(root, key, val);
		root->color = Color::kBlack; // 루트는 블랙
	}

	Node* Insert(Node* h, Key key, Value val)
	{
		if (h == nullptr)
			return new Node(key, val, 1, Color::kRed); // 새 노드는 일단 레드

		if (key < h->key) h->left = Insert(h->left, key, val);
		else h->right = Insert(h->right, key, val); // 키가 같으면 오른쪽에 추가
		//else if (key > h->key) h->right = Insert(h->right, key, val);
		//else h->val = val; // <- 원래는 키가 같으면 값(value)을 덮어쓰는 방식

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

	void DeleteMin()
	{
		assert(!IsEmpty());

		if (!IsRed(root->left) && !IsRed(root->right)) // 루트가 가운데인 4-노드로 임시 변경
			root->color = Color::kRed;

		root = DeleteMin(root);

		if (!IsEmpty()) // 루트는 항상 블랙
			root->color = Color::kBlack;
	}

	Node* DeleteMin(Node* h)
	{
		if (h->left == nullptr) // h가 가장 작은 노드라면 삭제하고 반환
		{
			delete h; // 자바에서는 가비지 컬렉터 사용
			return nullptr;
		}

		if (!IsRed(h->left) && !IsRed(h->left->left))
		{
			h = MoveRedLeft(h);
			Print2D(h);
		}

		h->left = DeleteMin(h->left); // 계속 찾아 내려감

		return Balance(h);
	}

	void DeleteMax()
	{
		if (!IsRed(root->left) && !IsRed(root->right))
			root->color = Color::kRed;;
		root = DeleteMax(root);
		if (!IsEmpty()) root->color = Color::kBlack;
	}

	Node* DeleteMax(Node* h)
	{
		if (IsRed(h->left))
			h = RotateRight(h);

		if (h->right == nullptr)
		{
			delete h; // 자바는 가비지 컬렉터 사용
			return nullptr;
		}
		if (!IsRed(h->right) && !IsRed(h->right->left))
			h = MoveRedRight(h);
		h->right = DeleteMax(h->right);
		return Balance(h);
	}

	void Delete(Key key)
	{
		if (!Contains(key)) return;

		if (!IsRed(root->left) && !IsRed(root->right))
			root->color = Color::kRed;

		root = Delete(root, key);

		if (!IsEmpty()) root->color = Color::kBlack;
	}

	Node* Delete(Node* h, Key key)
	{
		if (key < h->key)
		{
			if (!IsRed(h->left) && !IsRed(h->left->left))
				h = MoveRedLeft(h);

			h->left = Delete(h->left, key);
		}
		else
		{
			if (IsRed(h->left))
				h = RotateRight(h);

			if ((key == h->key) && (h->right == nullptr))
			{
				delete h; // 자바는 가비지 컬렉터 사용
				return nullptr;
			}

			if (!IsRed(h->right) && !IsRed(h->right->left))
				h = MoveRedRight(h);

			if (key == h->key)
			{
				Node* x = Min(h->right);
				h->key = x->key;
				h->val = x->val;
				h->right = DeleteMin(h->right);
			}
			else
			{
				h->right = Delete(h->right, key);
			}
		}

		return Balance(h);
	}

	int RecurSize() { return RecurSize(root); } // 재귀호출로 다시 계산하는 방식, 디버깅용
	int RecurSize(Node* n)
	{
		if (!n) return 0;
		return 1 + RecurSize(n->left) + RecurSize(n->right);
	}

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
			int h = max(5, Height(root) + 1), w = 6 * int(pow(2, h - 1));
			screen.clear();
			screen.resize(h * 2, string(w, ' '));
			Print2D(root, w / 2 - 2, 0, h - 1);
			for (const auto& l : screen) cout << l << endl;
		}
	}
	void Print2D(Node* n, int x, int level, int s)
	{
		PrintLine(x, (IsRed(n) ? "*" : " ") + to_string(n->key) + "(" + to_string(RecurSize(n)) + ")", screen[2 * level]);
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

int main()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 99);

	OrderStatisticTree bst;

	// key를 추가하는 순서에 따라서 트리 구조가 달라집니다.
	vector<int> keys = { 26, 17, 41, 14, 21, 30, 47, 10, 16, 19 };
	// vector<int> keys = { 26, 17, 41, 14, 21, 30, 47, 10, 16, 19, 21, 28, 38, 7, 12, 14, 20, 35, 39, 3 };

	for (int key : keys)
	{
		bst.Insert(key, key);
	}

	bst.Print2D();

	sort(keys.begin(), keys.end());

	cout << "Rank: ";
	for (int r = 1; r <= keys.size(); r++)
		cout << setw(3) << r;
	cout << endl << "Key : ";

	for (int k : keys)
		cout << setw(3) << k;
	cout << endl;


	cout << "Test: ";
	for (int k = 1; k <= keys.size(); k++) // 1부터 시작
	{
		Node* temp = bst.Select(k);
		if (!temp)
		{
			cout << "k = " << k << " not found" << endl;
			exit(-1);
		}
		else
		{
			cout << setw(3) << temp->key;

			if (keys[bst.Rank(temp->key) - 1] != temp->key)
			{
				cout << "Wrong rank of " << temp->key << " , " << bst.Rank(temp->key) << " vs " << keys[bst.Rank(temp->key) - 1] << endl;
				exit(-1);
			}
		}
	}
	cout << endl;

	return 0;

	// 아래는 난수로 여러 경우에 대해 테스트하는 코드
	for (int i = 0; i < 1000; i++)
	{
		OrderStatisticTree bst;

		vector<int> keys(20);
		std::generate(keys.begin(), keys.end(), [&] { return dis(gen); });

		for (int key : keys)
		{
			bst.Insert(key, key);
		}

		bst.Print2D();

		sort(keys.begin(), keys.end());

		cout << "Rank: ";
		for (int r = 1; r <= keys.size(); r++)
			cout << setw(3) << r;
		cout << endl << "Key : ";

		for (int k : keys)
			cout << setw(3) << k;
		cout << endl;


		cout << "Test: ";
		for (int k = 1; k <= keys.size(); k++) // 1부터 시작
		{
			Node* temp = bst.Select(k);
			if (!temp)
			{
				cout << "k = " << k << " not found" << endl;
				exit(-1);
			}
			else
			{
				cout << setw(3) << temp->key;

				if (keys[bst.Rank(temp->key) - 1] != temp->key)
				{
					cout << "Wrong rank of " << temp->key << " , " << bst.Rank(temp->key) << " vs " << keys[bst.Rank(temp->key) - 1] << endl;
					exit(-1);
				}
			}
		}
		cout << endl;
	}
}