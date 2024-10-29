#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

// Sedgewick Algorithm 5.4 Trie symbol table
class Trie
{
public:
	static const int R = 128; // ASCII table 0 이상 127 이하

	struct Node
	{
		vector<Node*> children; // 다음 글자에 대응하는 노드에 대한 포인터 (map<char, Node*> children; 으로도 구현 가능)
		string value;           // 리프(leaf)가 아니어도 (자식이 있어도) 값 저장 가능

		Node()
		{
			children.resize(R, nullptr); // 자식 노드 최대 R개
		}
	};

	Node* root = nullptr;

	~Trie()
	{
		DeleteAll();
	}

	// DeleteAll()과 Size()는 앞에서 공부했던 다른 트리들과 비교해보는 용도입니다.

	void DeleteAll() // 모든 노드 삭제
	{
		DeleteAll(root);
	}

	void DeleteAll(Node*& n)
	{
		if (n) // if(n != nullptr)
		{
			for (int c = 0; c < R; c++)
				DeleteAll(n->children[c]);
			delete n;
			n = nullptr;
		}
	}

	int Size() // 단어가 몇 개 추가되었는 지를 세어서 반환
	{
		return Size(root);
	}

	int Size(Node* n)
	{
		if (!n) return 0; // if(n == nullptr)

		int count = 0;

		if (!n->value.empty()) // 값이 저장된 노드
			count++;

		for (auto* n : n->children)
			count += Size(n);

		return count;
	}

	// 재귀호출을 사용하지 않는 삽입(insert)
	void IterInsert(string key, string val)
	{
		if (!root) root = new Node;

		Node* n = root;

		for (char c : key)
		{
			// TODO: 필요한 경우 새로운 자식 노드 생성

			// n = n->children.at(c); // n = n->children[c];

			// 보충
			// - children.at(c)는 children[c]와 같은 기능을 합니다.
			// - at()은 []와 달리 인덱스의 범위를 확인하기 때문에 디버깅에 유리합니다.
		}

		n->value = val; // 키(key)의 마지막 글자에 해당하는 노드에는 값(value) 저장
	}

	string IterSearch(string key)
	{
		// 키(key)를 찾았을 경우에는 값(value) 문자열을 반환
		// 키(key)를 찾지 못했을 경우에는 빈 문자열(string(""))을 값(value)으로 반환
		// 키의 마지막 글자에 해당하지 않는 노드의 value는 빈 문자열

		return string(""); // TODO:
	}

	// 재귀호출을 사용하는 삽입(Insert)
	void Insert(string key, string val)
	{
		root = Insert(root, key, val, 0);
	}

	Node* Insert(Node*& n, string key, string val, int d)
	{
		if (!n) n = new Node; // Node*& 포인터에 대한 참조 사용

		if (d == key.length())
		{
			// TODO: 해당 노드에 val 저장
			return n;
		}

		// TODO: 다음 글자로 진행 (주의: d + 1)

		return n;
	}

	string Search(string key)
	{
		Node* n = Search(root, key, 0);

		if (!n) return string("");

		return n->value;
	}

	Node* Search(Node* n, string key, int d)
	{
		// 마지막 글자의 노드 포인터 반환
		// 찾지 못했다면 nullptr 반환

		return nullptr; // TODO:
	}

	// 저장되어 있는 모든 키들을 찾아서 반환
	vector<string> Keys()
	{
		// 편의상 KeysWithPrefix()를 사용

		return KeysWithPrefix(string("")); // Prefix가 빈 문자열인 경우
	}

	vector<string> KeysWithPrefix(string pre)
	{
		vector<string> keys;

		// Search()로 키(key)가 pre인 노드를 찾고
		Node* n = Search(root, pre, 0);

		// 그 자식 노드들을 모두 모아서 반환
		Collect(n, pre, keys);

		return keys;
	}

	void Collect(Node* n, string pre, vector<string>& keys)
	{
		if (!n) return;

		if (!n->value.empty()) keys.push_back(pre);

		for (int c = 0; c < R; c++)
		{
			// TODO: Collect(...)
		}
	}

	string LongestPrefixOf(string s)
	{
		int l = SearchPrefix(root, s, 0, 0);
		return s.substr(0, l);
	}

	int SearchPrefix(Node* n, string s, int d, int l)
	{
		// l은 지금까지 찾은 가장 긴 prefix의 길이

		if (!n) return l;

		// TODO: n->value가 비어있지 않다면 key의 마지막 글자라는 의미니까 l에 d를 기록

		// TODO: d와 s.length()가 같다면 더 진행할 필요가 없으니까 l 반환

		char c = s.at(d); // s[d]

		return SearchPrefix(n->children[c], s, d + 1, l);
	}

	vector<string> KeysThatMatch(string pat)
	{
		vector<string> keys;

		CollectMatch(root, string(""), pat, keys);

		return keys;
	}

	void CollectMatch(Node* n, string pre, string pat, vector<string>& keys)
	{
		if (!n) return;

		int d = int(pre.length());

		if (d == pat.length() && !n->value.empty())
			keys.push_back(pre);

		if (d == pat.length()) return;

		char next = pat.at(d);

		if (next == '?')
		{
			// TODO: ? 자리에 어떤 글자든지 들어올 수 있다 -> 모든 글자에 대해 재귀 호출
		}
		else if (next == '*')
		{
			// 참고: https://www.geeksforgeeks.org/wildcard-character-matching/  
			//      https://github.com/TeodorDyakov/wildcard-trie/blob/master/src/main/java/trie/Trie.java

			// 난이도가 높은 도전 문제입니다. 

			// 힌트: 3가지 경우에 대해 CollectMatch()를 재귀호출합니다.
			// 1. * 자리에 글자가 없는 경우 (예: wor*d로 word를 찾음)
			// 2. * 자리에 ?처럼 어떤 글자든지 들어갈 수 있는 경우
			// 3. * 자리에 여러 글자가 들어가는 경우 (예: wor*d로 world, worried 등을 찾음)

			// TODO:
		}
		else
		{
			// TODO: 그 외의 일반적인 알파벳일 경우
		}
	}

	void Delete(string key)
	{
		root = Delete(root, key, 0); // root가 삭제될 경우 새로운 root 대입
	}

	bool IsEmpty(Node* n) // 자식이 하나도 없으면 true
	{
		for (int i = 0; i < R; i++)
			if (n->children[i])
				return false;
		return true;
	}

	Node* Delete(Node* n, string key, int d)
	{
		if (!n) return nullptr;

		if (d == key.length())
		{
			n->value = string(""); // 키(key)와 값(value) 삭제
		}
		else
		{
			char c = key.at(d);
			n->children[c] = Delete(n->children[c], key, d + 1);
		}

		//if ( TODO: 자식이 하나도 없고 value도 비어있다면)
		//{
		//	// TODO: 메모리 삭제
		//	n = nullptr;
		//}

		return n;
	}

	// 아래는 디버깅을 위한 트리구조 출력

	void Display2D()
	{
		vector<string> screen(Height(), string(Width(root) * 6, ' '));
		Display2D(root, 0, 0, screen);
		for (auto& s : screen) cout << s << endl;
	}

	void Display2D(Node* n, int x, int level, vector<string>& screen)
	{
		if (!n) return;
		for (int c = 0; c < R; c++)
			if (n->children[c])
			{
				screen[level][3 * x] = char(c);
				if (!n->children[c]->value.empty())
					screen[level][3 * x + 1] = '*';
				Display2D(n->children[c], x, level + 1, screen);
				x += Width(n->children[c]);
			}
	}

	int Height()
	{
		return Height(root);
	}

	int Height(Node* n)
	{
		if (!n) return 0;
		int cnt = 0;
		for (auto* c : n->children)
			cnt = std::max(cnt, Height(c));
		return 1 + cnt;
	}

	int Width(Node* n)
	{
		vector<int> w(Height(n), 0);
		WidthHelper(n, w, 0);
		return *std::max_element(w.begin(), w.end());
	}

	void WidthHelper(Node* n, vector<int>& w, int level)
	{
		for (int c = 0; c < R; c++)
			if (n->children[c])
			{
				w[level] += 1;
				WidthHelper(n->children[c], w, level + 1);
			}
		w[level] = std::max(1, w[level]);
	}
};

void run_dict(); // 함수 구현은 main() 아래에

int main()
{
	// 기본 사례
	{
		// Trie에 추가할 단어들
		vector<string> words = { "and", "ant", "do", "dad", "ball", "she", "shells" };

		Trie trie;

		// 키(key) 추가
		for (auto w : words)
			trie.IterInsert(w, w + "_value"); //trie.Insert(w, w + "_value"); // _value는 디버깅을 위한 임시 값(value) (뒤에서는 사전의 내용)

		// 모든 키 출력
		// and ant ball dad do she shells <- 알파벳 순서로 정렬되어서 출력됩니다.
		cout << "Keys()" << endl;
		for (const auto& k : trie.Keys())
			cout << k << " ";
		cout << endl << endl;

		// 트리 구조 확인
		cout << "Display2D()" << endl;
		trie.Display2D();
		cout << endl;

		// 키 탐색
		// do : do_value     <- do를 찾아서 do_value 출력
		// dad : dad_value
		// hello : Not found <- 삽입하지 않았기 때문에 찾지 못함

		cout << "Search" << endl;
		for (auto w : vector<string>{ "do", "dad", "hello" })
		{
			string value = trie.IterSearch(w); // string value = trie.Search(w);
			if (value.empty()) value = "Not found"; // 반환 문자열이 비어있다면 못 찾은 상황
			cout << w << " : " << value << endl;
		}
		cout << endl;

		// 특정 문자열로 시작하는 키 검색
		// dad do <- "d"로 시작하는 단어들이 모두 출력
		cout << "KeysWithPrefix()" << endl;
		for (const auto& k : trie.KeysWithPrefix("d"))
		{
			cout << k << " ";
		}
		cout << endl << endl;

		// 앞 부분이 겹치는 가장 긴 단어 출력
		cout << "LongestPrefixOf()" << endl;
		cout << trie.LongestPrefixOf("she") << endl;       // she
		cout << trie.LongestPrefixOf("shell") << endl;     // she
		cout << trie.LongestPrefixOf("shellsort") << endl; // shells
		cout << trie.LongestPrefixOf("shallow") << endl;   // 공백(찾지 못함)
		cout << endl;

		// 와일드카드(wildcard) ? 테스트
		// ? 자리에 어떤 글자든지 들어갈 수 있음
		// "an?" 에서 ? 자리가 각각 d와 t인 "and" 와 "ant" 출력
		cout << "KeysThatMatch()" << endl;
		for (const auto& k : trie.KeysThatMatch("an?"))
		{
			cout << k << " ";
		}
		cout << endl << endl;

		// 키 삭제
		cout << "Delete()" << endl;
		for (auto w : vector<string>{ "and", "ant", "ball", "she" })
		{
			cout << "Delete: " << w << endl;
			trie.Delete(w);
			trie.Display2D(); // 트리구조가 어떻게 달라지는 지 실행 예시에서 보세요
		}
		cout << endl << endl;

		// 삭제 후 키 출력 (삭제된 단어들은 출력되지 않음)
		cout << "Keys() after Delete()" << endl;
		for (const auto& k : trie.Keys())
			cout << k << " ";
		cout << endl << endl;
	}

	// 와일드카드 * 테스트
	// * 자리에는 어떤 문자열이든지 들어갈 수 있습니다. 아무 글자도 들어가지 않을 수도 있습니다.
	// 예시) "ab*cd"에서 *자리에 
	//      아무 글자도 들어가지 않으면 abcd 
	//      "1"이 들어가면 "ab1cd" 
	//      "12"가 들어가면 "ab12cd" 
	//      "123"이 들어가면 "ab123cd"
	{
		Trie trie;

		for (string w : {"abcd", "ab1cd", "ab12cd", "ab123cd"})
			trie.IterInsert(w, w + "_value");

		cout << "KeysThatMatch(\"ab*cd\")" << endl;
		for (const auto& k : trie.KeysThatMatch("ab*cd"))
		{
			cout << k << " ";
		}
		cout << endl << endl;
	}

	// 영어 사전 (사전 파일을 읽어들이는 데에 시간이 약간 걸립니다.)
	// run_dict();

	return 0;
}

void run_dict()
{
	Trie trie;

	ifstream ifile("../eng_dic.txt");
	int count = 0;
	while (ifile)
	{
		string key, value; // char key[1024], value[1024];
		getline(ifile, key); //ifile.getline(key, sizeof(key));
		getline(ifile, value); //ifile.getline(value, sizeof(value));

		if (!key.empty())
		{
			trie.Insert(key, value); // trie에 추가
			count++;
		}
	}
	ifile.close();
	cout << "Inserted " << count << " keys" << endl;

	while (true)
	{
		string word; // char word[1024];
		cout << "Input a word : ";
		cin >> word;

		vector<string> keys = trie.KeysThatMatch(word);

		if (!keys.empty())
			for (string k : keys)
				cout << k << " : " << trie.Search(k) << endl;
		else
			cout << "Not found" << endl;
		cout << endl;
	}
}
