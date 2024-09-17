#include <string>
#include <vector>
#include <iostream>
using namespace std;

// 아래 구현은 CLRS 4th p.974을 유사하게 이해하기 쉽도록 정리한 것입니다.
// C 스타일의 보다 효율적인 구현은 아래 링크 참고하세요.
// https://www.geeksforgeeks.org/finite-automata-algorithm-for-pattern-searching/

void ComputeTransitionFunction(string pat, vector<vector<int>>& TF) // TF reference
{
	int M = int(pat.size());
	int d = int(TF[0].size()); // number of characters (128 in ASCII)

	// 힌트
	// - pat[0]에서 pat[s-1]까지 순서대로 들어왔다면 상태 숫자는 s입니다.
	// - 바꿔말하면 상태가 s번이라는 의미는 글자가 s개 매칭된 상태라는 의미입니다.
	// - 상태의 가짓수는 패턴의 글자수보다 하나 더 많습니다. (하나도 매칭 안된 0번 상태 포함)

	for (int state = 0; state <= M; ++state)     // 상태 state에서
		for (int x = 32; x < d; ++x)             // 글자 x가 들어오면 (ASCII 32 미만은 패턴에 사용되지 않는다고 가정)
		{
			int k = std::min(M, state + 1); // 상태는 M보다 클 수 없다

			while (k > 0) // k == 0 이라면 한 글자도 매칭이 안되었다는 의미
			{
				// 문자열 연산 힌트 substr(), +=
				// string prefix = TODO;     // 패턴의 앞에서부터 k 글자
				// string suffix = TODO; // 현재 상태로 오기위해 입력받은 문자열 + 새로운 글자의 뒷부분 k 글자

				//if (prefix == suffix)
				//	break;

				k = k - 1; // 길이 k인 경우에 매칭 실패,더 짦은 경우에 대해 시도
			}

			TF[state][x] = k;
		}
}

void PrintTransitionFunction(vector<vector<int>>& tf)
{
	cout << "   ";
	for (char i = 'A'; i <= 'D'; i++)
		cout << i;
	cout << endl;

	for (int s = 0; s < tf.size(); s++)
	{
		cout << s << "  ";
		for (char i = 'A'; i <= 'D'; i++)
			cout << tf[s][i];
		cout << endl;
	}
	cout << endl;
}

void FiniteAutomatonMatcher(string pat, string txt)
{
	int M = int(pat.size());
	int N = int(txt.size());
	int d = 128; // number of ASCII characters

	vector<vector<int>> tf(M + 1, vector<int>(d, 0)); // 모두 0으로 초기화 (엉뚱한 글자가 들어오면 첫 상태로 이동)

	// tf[state][character] -> next state
	// 그래프의 인접 행렬(adjacency matrix) 표현으로 생각할 수 있습니다.

	ComputeTransitionFunction(pat, tf);

	PrintTransitionFunction(tf);

	for (int i = 0; i < N; i++)
		cout << i % 10;
	cout << endl << txt << endl;

	string record; // state 기록 디버깅용

	int state = 0;
	for (int i = 0; i < N; i++)
	{
		state = tf[state][txt[i]];

		record.append(to_string(state));

		if (state == M)
		{
			cout << record << endl;
			cout << string(i - M + 1, ' ') << pat << " " << i - M + 1 << endl;
		}
	}
}

int main()
{
	string txt = "AABAACAADAABAAABAA";
	string pat = "AABA";
	//string txt = "ABABABCABABABAB";
	//string pat = "ABAB";

	FiniteAutomatonMatcher(pat, txt); // 중복 매치 가능

	return 0;
}
