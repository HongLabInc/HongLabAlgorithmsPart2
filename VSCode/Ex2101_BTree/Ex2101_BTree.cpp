#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

// 참고: https://www.geeksforgeeks.org/introduction-of-b-tree-2/

template<typename T_KEY>
class BTree
{
public:
	struct Node
	{
		vector<T_KEY> keys;     // 최대 2*t - 1개, 루트 외에 최소 t - 1개
		vector<Node*> children; // 최대 2*t개
		bool leaf = false;      // 리프노드라면 true
		int n = 0;              // 현재 저장된 키의 갯수

		// children[0]은 keys[0]보다 작은 키들이 저장된 자식 노드
		// children[n]은 keys[n-1]보다 큰 키들이 저장된 자식 노드

		Node(int t, bool leaf)
			: keys(2 * t - 1, -1), children(2 * t, nullptr), leaf(leaf), n(0)
		{
			// 편의상 삽입/삭제가 이뤄질 때마다 keys와 children의 크기를 매번 바꾸는 대신에
			// 처음에 최대 크기로 초기화한 후에 실제 추가된 갯수는 n으로 추적합니다.
		}

		Node(int t, bool leaf, string init) // 테스트 케이스용 생성자
			: keys(2 * t - 1, -1), children(2 * t, nullptr), leaf(leaf), n(0)
		{
			n = int(init.size());
			for (int i = 0; i < n; i++)
				keys[i] = init[i];
		}
	};

	Node* root = nullptr;
	int t; // 최소 차수 (minimum degree)

	// 최소 차수 규칙
	// - 루트 외의 노드들은 키(key)를 최소 t - 1개 저장
	// - 루트 포함 모든 노드들은 키를 최대 2t - 1개 저장
	// - n = 2t - 1인 노드는 분할 가능

	BTree(int t)
		: t(t)
	{
		root = new Node(t, true); // 처음 만들어진 루트 노드는 자식이 없기 때문에 리프노드

		// DiskWrite(root); 디스크에 저장 (예제에서는 디스크 입출력을 생략하였습니다.)
	}

	~BTree()
	{
		RemoveChildren(root);
	}

	void RemoveChildren(Node*& x)
	{
		if (!x) return;
		//for (int i = 0; i < TODO: 몇 개를 삭제해야 할까요? ; i++)
		//	RemoveChildren(x->children[i]);

		//cout << "Remove: ";
		//for (int i = 0; i < x->n; i++)
		//	cout << x->keys[i] << ", ";
		//cout << endl;

		delete x;
		x = nullptr;
	}

	void Insert(T_KEY k)
	{
		if (root->n == 2 * t - 1)
		{
			SplitRoot();

			InsertNonfull(root, k);
		}
		else
			InsertNonfull(root, k);
	}

	void SplitRoot()
	{
		Node* s = new Node(t, false); // 빈 노드를 새로 루트로 만들고

		s->children[0] = root;        // 원래 루트는 그 노드의 첫 번째 자식으로 지정

		SplitChild(s, 0); // s의 children[0]을 쪼갠다 (결국 예전 root를 쪼개는 것)
		// 쪼갤때 부모로 하나 올려줘야 하기 때문에 부모 포인터를 넣어줍니다.

		root = s;
	}

	void InsertNonfull(Node* x, T_KEY k)
	{
		int i = x->n - 1;    // 맨 뒤에서부터 삽입 위치를 찾아가는 방식

		if (x->leaf == true) // 리프노드라면 현재 노드에 바로 삽입
		{
			// 삽입정렬처럼 삽입
			while (i >= 0 && x->keys[i] > k)
			{
				x->keys[i + 1] = x->keys[i];
				i = i - 1;
			}
			x->keys[i + 1] = k;
			x->n += 1;

			// DiskWrite(x) 바뀐 내용을 디스크에 저장
		}
		else // 리프 노드가 아닐 경우
		{
			while (i >= 0 && x->keys[i] > k) // 새로 삽입될 위치 찾기
				i = i - 1;

			// 예시:
			// ACEF가 저장된 노드에서 k = D가 저장될 자식 찾기
			// 0 1 2 3
			// A C E F
			// i = 3에서 시작해서 하나씩 감소시키면 i = 1일때 break
			// D가 들어갈 위치는 C와 E사이인데, children[0]은 A보다 작은 것들, children[1]은 C보다 작은 것들, children[2]는 E보다 작은 것들, 순서라서
			// children[i + 1]에 저장해야 함
			// 그래서 아래에서는 i + 1 사용

			// DiskRead(x->children[x + 1])  // 디스크에서 읽어들이기

			//if (TODO: 여기 들어갈 조건은? ) // 꽉 찼다면 쪼갠 후에 삽입
			//{
			//	SplitChild(x, i + 1);

			//	// 쪼갠 후에는 x->keys[i+1]이 바뀌었기 때문에
			//	// 저장 위치가 그 키의 전인지 후인지를 업데이트 해야 함
			//	if (x->keys[i + 1] < k)
			//		i = i + 1;
			//}

			// InsertNonfull(x->children[i + 1], k); // 키 삽입
		}
	}

	// x->children[i]를 쪼갠다
	// 쪼개는 과정에서 부모 노드로 하나씩 옮기기 때문에 children[i]의 포인터가 아니라 부모 x의 포인터 전달
	void SplitChild(Node* x, int i)
	{
		Node* y = x->children[i];
		Node* z = new Node(t, y->leaf);

		z->n = t - 1;

		//for (TODO: ) // y의 마지막 t - 1개의 키(key)들을 z로 복사
		//	z->keys[j] = TODO;

		if (y->leaf == false) // 리프노드가 아니라면 자식들도 복사
		{
			//for (TODO:)
			//	z->children[j] = TODO;
		}

		y->n = t - 1; // n의 개수를 줄인다는 것은 z로 옮긴 것들을 y에서 뺀다는 의미

		// 부모 x에 새로운 자식 노드가 추가되어야 하기 때문에 하나씩 밀어서 빈 자리 마련
		//for (TODO:)
		//	x->children[j + 1] = x->children[j];

		x->children[i + 1] = z; // 빈 자리에 z를 새로운 자식으로 추가

		// y의 중간 키를 x로 이동하기 위해 빈 자리 마련
		//for (TODO:)
		//	x->keys[j + 1] = x->keys[j];

		x->keys[i] = y->keys[t - 1]; // y의 가운데 키를 x로 이동

		x->n += 1; // 하나 증가
	}

	void Delete(T_KEY k)
	{
		if (root)
		{
			Delete(root, k);

			if (root->n == 0) // 삭제 후 루트가 비었다면 첫번째 자식을 루트로 교체
			{
				Node* temp = root;
				if (root->leaf)
					root = nullptr;
				else
					root = root->children[0];

				delete temp;
			}
		}
		else
			cout << "Tree is empty" << endl;
	}

	// keys에서 k보다 큰 첫 인덱스
	int FindKey(Node* x, T_KEY k)
	{
		int idx = 0;
		while (idx < x->n && x->keys[idx] < k)
			idx += 1;
		return idx;
	}

	// x->children[idx]가 최소 개수일 경우에는 더 줄일 수 없기 때문에
	// 미리 갯수를 늘려줍니다. Case 3
	void Fill(Node* x, int idx)
	{
		// x->children[idx-1]에 여유가 있다면 거기서 가져옵니다. Case 3a
		//if (idx != 0 && TODO: 조건)
		//	BorrowFromPrev(x, idx);

		//// x->children[idx+1]에 여유가 있다면 거기서 가져옵니다. Case 3a
		//else if (idx != x->n && TODO: 조건)
		//	BorrowFromNext(x, idx);

		//// 그 외의 경우에는 형제 노드와 합칩니다. Case 3b
		//// 위에서 형제들 x->children[idx - 1], x->children[idx + 1]이 크지 않다는 것을 확인했기 때문에 
		//// 합쳐도 최대 조건을 넘지 않습니다.
		//else
		//{
		//	if (idx != x->n)
		//		Merge(x, idx);     // children[idx]와 children[idx+1]을 합칩니다.
		//	else
		//		Merge(x, idx - 1); // children[idx]가 마지막 자식일 경우에는 그 앞의 자식인 children[idx - 1]과 합칩니다.
		//}

		return;
	}

	void Delete(Node* x, T_KEY k)
	{
		int idx = FindKey(x, k);

		//if (idx < x->n && TODO: 조건) // x에 k가 저장되어 있는 경우
		//{
		//	if (x->leaf)
		//		DeleteFromLeaf(x, idx);      // 리프노드에서 삭제, Case 1
		//	else
		//		DeleteFromNonLeaf(x, idx);   // 리프가 아닌 노드에서 삭제, Case 2
		//}
		//else
		//{
		//	if (x->leaf) // x가 리프인데 k가 없다면 트리에 없다는 의미 (삭제 불가)
		//	{
		//		cout << "The key " << k << " is does not exist in the tree\n";
		//		return;
		//	}

		//	// 그렇지 않다면 자식 노드에 찾아가서 삭제

		//	// flag는 찾아가야 하는 자식 노드가 마지막 자식 노드인지를 기록
		//	bool flag = ((idx == x->n) ? true : false);

		//	// 삭제 후에 최소 조건을 만족시키지 못할 상황이라면 여분 하나를 미리 증가시켜 준다.
		//	if (TODO: 조건)
		//		Fill(x, idx);

		//	// Fill()에서 마지막 자식은 그 앞의 자식과 합치는 방식
		//	// 그런 경우에는 마지막 자식이 사라졌기 때문에 idx -1 자식으로 재귀호출
		//	if (flag && idx > x->n)
		//		Delete(x->children[idx - 1], k);
		//	else
		//		Delete(x->children[idx], k);
		//}

		return;
	}

	void DeleteFromLeaf(Node* x, int idx) // 리프노드에서 삭제, Case 1
	{
		// idx보다 큰 것들을 하나씩 쉬프트 시켜서 idx 위치가 삭제된 효과 구현
		for (int i = idx + 1; i < x->n; ++i)
			x->keys[i - 1] = x->keys[i];

		x->n -= 1; // 하나 삭제했기 때문에 키의 개수 n 감소

		return;
	}

	void DeleteFromNonLeaf(Node* x, int idx) // 리프 노드가 아닌 노드에서 삭제
	{
		T_KEY k = x->keys[idx];

		// 이 노드의 개수를 유지하기 위해서
		// 삭제하려는 키보다 작은쪽의 서브트리와 바꿔치기하고 그 쪽에서 삭제하게 합니다.
		// 자식 서브트리로 책임을 미루는 느낌입니다. Case 2a
		if (x->children[idx]->n >= t)
		{
			T_KEY pred = GetPred(x, idx);   // 바꿔치기할 값을 가져옵니다.
			x->keys[idx] = pred;            // 삭제할 키 자리에 덮어쓰고
			Delete(x->children[idx], pred); // 내려가면서 그 값을 삭제하도록 합니다.
		}

		// 이 노드의 개수를 유지하기 위해서
		// 삭제하려는 키보다 큰쪽의 서브트리와 바꿔치기하고 그 쪽에서 삭제하게 합니다.
		// 자식 서브트리로 책임을 미루는 느낌입니다. Case 2b
		else if (x->children[idx + 1]->n >= t)
		{
			//TODO;        // 바꿔치기할 값을 가져옵니다.
			//TODO;        // 삭제할 키 자리에 덮어쓰고
			//TODO;        // 내려가면서 그 값을 삭제하도록 합니다.
		}

		// 삭제할 k보다 작은 자식 노드와 큰 자식 노드 두 개를 합칩니다.
		// 자식 노드 두 개가 합치는 과정에서 k가 새로 생긴 자식노드로 넘어갑니다.
		// 그 다음에 그 노드에게 삭제하라고 시킵니다. Case 2c
		// 예시:
		//[                                 P                           ]
		// [      C,       G,       L      ] [         T,       X      ]
		//  [A, B]   [D, E]   [J, K] [N, O]   [Q, R, S]   [U, V] [Y, Z]
		//
		//Delete: G
		//[                              P                           ]
		// [      C,             L      ] [         T,       X      ]
		//  [A, B]   [D, E, J, K] [N, O]   [Q, R, S]   [U, V] [Y, Z]
		// 자식 노드 2개 [D, E]와 [J, K]를 합치면 자식 노드는 [D, E, G, J, K]로 G까지 갖게 되고 
		// 부모 노드인 [C, G, L]에서는 합쳐진 자식으로 G를 보내기 때문에 간단히 [C, L]로 줄어든다.
		// 따라서 G를 삭제하라고 자식 노드에게 시키면 [D, E, G, J, K] -> [D, E, J, K]로 삭제 

		else
		{
			Merge(x, idx);
			Delete(x->children[idx], k);
		}
		return;
	}

	// 현재 삽입하려는 키 보다 작은 키들 중에서 가장 큰 것을 찾습니다.
	T_KEY GetPred(Node* x, int idx)
	{
		Node* cur = x->children[idx];
		while (!cur->leaf)
			cur = cur->children[cur->n];
		return cur->keys[cur->n - 1];
	}

	// 현재 삽입하려는 키보다 큰 키들 중에서 가장 작은 것을 찾습니다.
	T_KEY GetSucc(Node* x, int idx)
	{
		Node* cur = x->children[idx + 1];
		while (!cur->leaf)
			cur = cur->children[0];
		return cur->keys[0];
	}

	// x->children[idx - 1]에서 가장 큰 키를 x로 옮기고
	// 대신에 x에서 하나를 x->children[idx]의 가장 작은 키로 이동
	// 이진트리에서 보던 회전과 비슷
	void BorrowFromPrev(Node* x, int idx)
	{
		Node* child = x->children[idx];
		Node* sibling = x->children[idx - 1];

		// x->children[idx]의 맨 앞에 빈 자리 마련 (원래 갖고 있던 것들을 뒤로 하나씩 미룸)
		for (int i = child->n - 1; i >= 0; --i)
			child->keys[i + 1] = child->keys[i];

		// 리프 노드가 아닐 경우에는 자식 포인터들도 하나씩 쉬프트 (위에서 키를 쉬프트한 것과 일관성 유지)
		if (!child->leaf)
		{
			for (int i = child->n; i >= 0; --i)
				child->children[i + 1] = child->children[i];
		}

		// x에서 하나 가져와서 x->children[idx]의 가장 작은 키로 채움
		child->keys[0] = x->keys[idx - 1];

		// 자식 포인터는 x->children[idx-1]에서 가져옴
		if (!child->leaf)
			child->children[0] = sibling->children[sibling->n];

		// x->children[idx - 1]에서 가장 큰 키를 부모 x로 이동
		x->keys[idx - 1] = sibling->keys[sibling->n - 1];

		child->n += 1;    // x->children[idx] 하나 증가
		sibling->n -= 1;  // x->children[idx - 1] 하나 감소

		return;
	}

	// BorrowFromPrev()와 좌우 대칭
	void BorrowFromNext(Node* x, int idx)
	{
		Node* child = x->children[idx];
		Node* sibling = x->children[idx + 1];

		child->keys[(child->n)] = x->keys[idx];

		if (!child->leaf)
			child->children[(child->n) + 1] = sibling->children[0];

		x->keys[idx] = sibling->keys[0];

		for (int i = 1; i < sibling->n; ++i)
			sibling->keys[i - 1] = sibling->keys[i];

		if (!sibling->leaf)
		{
			for (int i = 1; i <= sibling->n; ++i)
				sibling->children[i - 1] = sibling->children[i];
		}

		child->n += 1;
		sibling->n -= 1;

		return;
	}

	// x->children[idx]과 x->children[idx + 1]을 합칩니다.
	void Merge(Node* x, int idx)
	{
		Node* child = x->children[idx];
		Node* sibling = x->children[idx + 1];

		// 부모 x의 키들 중에서 두 자식에 대한 포인터 사이에 껴있는 것도 자식으로 옮깁니다.
		child->keys[t - 1] = x->keys[idx];

		// x->children[idx + 1]의 키들을 모두 x->children[idx]로 복사합니다.
		for (int i = 0; i < sibling->n; ++i)
			child->keys[i + t] = sibling->keys[i];

		// 리프 노드가 아니라면 자식 포인터도 복사합니다.
		if (!child->leaf)
		{
			for (int i = 0; i <= sibling->n; ++i)
				child->children[i + t] = sibling->children[i];
		}

		// 부모 x의 키들 중에서 하나를 자식으로 옮겼기 때문에 빈 자리를 메웁니다.
		for (int i = idx + 1; i < x->n; ++i)
			x->keys[i - 1] = x->keys[i];

		// 부모 x의 자식들 2개가 하나로 합쳐지기 때문에 자식 포인터를 줄입니다.
		for (int i = idx + 2; i <= x->n; ++i)
			x->children[i - 1] = x->children[i];

		child->n += sibling->n + 1; // 개수 조정 (뒤의 + 1개는 부모 x에서 가져온 하나)
		x->n -= 1;                  // 부모 x에서는 하나 감소

		delete sibling; // 메모리 반환

		return;
	}

	void TraversePrint()
	{
		TraversePrint(root);
		cout << endl;
	}

	void TraversePrint(Node* x) // 모든 노드 방문
	{
		int i;
		for (i = 0; i < x->n; i++)
		{
			if (x->leaf == false)
				TraversePrint(x->children[i]);
			cout << " " << x->keys[i];
		}

		if (x->leaf == false)
			TraversePrint(x->children[i]);
	}

	int Height(Node* x)
	{
		if (!x) return 0;
		int max_height = 0;
		for (int i = 0; i < x->n + 1; i++)
			max_height = std::max(max_height, Height(x->children[i]));
		return 1 + max_height;
	}

	int Size(Node* x) // 모든 키(key)들의 갯수
	{
		if (!x) return 0;
		int num = x->n;
		for (int i = 0; i < x->n + 1; i++)
			num += Size(x->children[i]);
		return num;
	}

	void DisplayTree() // Inorder traverse
	{
		if (!root)
		{
			cout << "Empty" << endl;
			cout << endl;
			return;
		}

		vector<string> log(Height(root) + 1, string(Size(root) * 4, ' '));
		int start = 0;
		DisplayTree(root, 0, start, log);

		for (string s : log)
			cout << s << endl;
	}

	string KeyToString(int k)
	{
		return to_string(k);
	}

	string KeyToString(char k)
	{
		return string(1, k);
	}

	void DisplayTree(Node* x, int level, int& start, vector<string>& log)
	{
		log[level][start++] = '[';

		int i;
		for (i = 0; i < x->n; i++)
		{
			if (x->leaf == false)
				DisplayTree(x->children[i], level + 1, start, log);

			string temp = KeyToString(x->keys[i]);
			if (i != x->n - 1) temp += ", ";
			for (char c : temp)
			{
				log[level][start++] = c;
			}
		}

		if (x->leaf == false)
			DisplayTree(x->children[i], level + 1, start, log);

		log[level][start++] = ']';
	}
};

int main()
{
	BTree<char> bt(3);
	// BTree<char> bt(5);
	for (char k : string("ABCDEFGHIJKLMNOPQRSTUV"))
	{
		cout << "Insert: " << k << endl;
		bt.Insert(k);
		bt.DisplayTree();
	}

	// GeeksForGeeks 예제 (키가 정수형)
	{
		// BTree<int> bt(2);
		BTree<int> bt(3);

		for (int k : vector<int>{ 1, 3, 7, 10, 11, 13, 14, 15, 18, 16, 19, 24, 25, 26, 21, 4, 5, 20, 22, 2, 17, 12, 6 })
			//for (int k : vector<int>{ 1, 3, 7, 10 })
		{
			cout << "Insert: " << k << endl;
			bt.Insert(k);
			//bt.TraversePrint();
			bt.DisplayTree();
		}

		for (int k : vector<int>{ 1, 3, 7, 10, 11, 13, 14, 15, 18, 16, 19, 24, 25, 26, 21, 4, 5, 20, 22, 2, 17, 12, 6 })
			//for (int k : vector<int>{ 6, 13, 7, 4, 2, 16 })
		{
			cout << "Delete: " << k << endl;
			bt.Delete(k);
			//bt.TraversePrint();
			bt.DisplayTree();
		}
	}

	// CLRS 예제 (키가 문자형)
	{
		BTree<char> bt(3);
		bt.root = new BTree<char>::Node(bt.t, false, "GMPX"); // CLRS 예제와 같은 상황을 만들기 위해 수동으로 트리 초기화
		bt.root->children[0] = new BTree<char>::Node(bt.t, true, "ACDE");
		bt.root->children[1] = new BTree<char>::Node(bt.t, true, "JK");
		bt.root->children[2] = new BTree<char>::Node(bt.t, true, "NO");
		bt.root->children[3] = new BTree<char>::Node(bt.t, true, "RSTUV");
		bt.root->children[4] = new BTree<char>::Node(bt.t, true, "YZ");

		bt.DisplayTree();

		// CLRS Figure 18.8 삭제 예제
		for (char k : string("BQLF"))
		{
			cout << "Insert: " << k << endl;
			bt.Insert(k);
			bt.DisplayTree();
		}

		for (char k : string("FMGDB"))
		{
			cout << "Delete: " << k << endl;
			bt.Delete(k);
			bt.DisplayTree();
		}
	}

	return 0;
}
