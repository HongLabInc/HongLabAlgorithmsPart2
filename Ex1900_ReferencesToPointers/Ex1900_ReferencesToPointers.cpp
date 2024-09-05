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

// MyTree는 개념 설명을 위한 것이며 실제로 작동하지는 않습니다.
class MyTree
{
public:
	struct Node
	{
		Node* child = nullptr;
		int value = 0;
	};

	Node* root = nullptr;

	void Insert1()
	{
		if (!root) // if(root == nullptr)
		{
			root = new Node;
			root->value = 123;
		}

		Insert1(root);
	}

	void Insert1(Node* n)
	{
		assert(n);

		// 필요하면 child 생성
		if (!n->child)
		{
			n->child = new Node;
			// ...
		}
	}

	void Insert2()
	{
		Insert2(root);
	}

	void Insert2(Node*& n)
	{
		if (!n)
		{
			n = new Node;
			n->value = 123;
		}

		Insert2(n);
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

	return 0;
}
