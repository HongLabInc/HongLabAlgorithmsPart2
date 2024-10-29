#include <iostream>
#include <assert.h>

using namespace std;

void Test1(float* f) // 매개변수 f의 자료형이 포인터(float*)
{
	f = new float(123.0f);
}

void Test2(float*& f) // 매개변수 f의 자료형이 포인터의 참조(float*&)
{
	f = new float(456.0f);
}

class MyTree
{
public:
	struct Node
	{
		Node* child = nullptr;
		int value = 0;
	};

	Node* root = nullptr;

	~MyTree()
	{
		DeleteAll();
	}

	void DeleteAll()
	{
		DeleteAll(root);
	}

	void DeleteAll(Node*& n)
	{
		if (n) // if(n != nullptr)
		{
			DeleteAll(n->child);
			delete n;
			n = nullptr;
		}
	}

	void Insert1()
	{
		if (!root) // if(root == nullptr)
		{
			root = new Node;
			root->value = 0;
		}

		Insert1(root, 1, 5); // 0에서 4까지 노드 5개 생성
	}

	void Insert1(Node* n, int d, int l)
	{
		if (d == l) return;

		// 필요하면 child 생성
		if (!n->child)
		{
			n->child = new Node;
			n->child->value = d;
		}

		Insert1(n->child, d + 1, l);
	}

	void Insert2()
	{
		Insert2(root, 0, 5);
	}

	void Insert2(Node*& n, int d, int l) // 0에서 4까지 노드 5개 생성
	{
		if (d == l) return;

		if (!n)
		{
			n = new Node;
			n->value = d;
		}

		Insert2(n->child, d + 1, 5);
	}

	void PrintIter()
	{
		Node* n = root;
		while (n)
		{
			cout << n->value << " ";
			n = n->child;
		}
		cout << endl;
	}

	void PrintRecur()
	{
		PrintRecur(root);
		cout << endl;
	}

	void PrintRecur(Node* n)
	{
		if (!n) return;
		cout << n->value << " ";
		PrintRecur(n->child);
	}
};

int main()
{
	float* f1 = nullptr;

	Test1(f1);

	cout << f1 << endl; // f1은 여전히 nullptr라서 0000000000000000 출력

	float* f2 = nullptr;

	Test2(f2);

	cout << f2 << " " << *f2 << endl; // 0000017E006E67B0 (주소는 달라질 수 있음) 456 <- Test2() 안에서 만들어준 대로 출력

	MyTree t1;
	t1.Insert1();
	t1.PrintIter();   // 0 1 2 3 4
	t1.PrintRecur();  // 0 1 2 3 4

	MyTree t2;
	t2.Insert2();
	t2.PrintIter();   // 0 1 2 3 4
	t2.PrintRecur();  // 0 1 2 3 4

	return 0;
}
