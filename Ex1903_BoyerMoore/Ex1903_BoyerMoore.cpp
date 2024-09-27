#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

// Sedgewick Algorithm 5.7를 연속 매칭으로 수정

class BoyerMoore
{
public:
	vector<int> right;
	string pat;

	BoyerMoore(string pat)
		: pat(pat)
	{
		int M = int(pat.size());
		int R = 128;

		right.resize(R);

		for (int c = 0; c < R; c++)
			right[c] = -1;              // 패턴에 포함되지 않는 글자는 -1
		for (int j = 0; j < M; j++)
			right[pat.at(j)] = j;       // 여러번 나타날 경우 가장 뒤에(오른쪽에) 나타나는 글자 위치로 덮어씀

		// 디버깅용 출력
		for (int i = 0; i < pat.size(); i++)
			cout << i % 10;
		cout << endl << pat << endl;
		for (char i = 'A'; i <= 'Z'; i++)
			cout << "  " << i;
		cout << endl;
		for (int i = 'A'; i <= 'Z'; i++)
			cout << setw(3) << right[i];
		cout << endl;
	}

	void Search(string txt)
	{
		int N = int(txt.size());
		int M = int(pat.size());

		for (int i = 0; i < txt.size(); i++)
			cout << i % 10;
		cout << endl << txt << endl;

		int skip;
		for (int i = 0; i <= N - M; i += skip)
		{
			string log(N, ' ');      // 디버깅용 출력

			skip = 0;
			for (int j = M - 1; j >= 0; j--) // 패턴을 역순으로 비교
			{
				log.at(i + j) = pat.at(j); // 디버깅용 출력

				// cout << log << endl;

				if (pat.at(j) != txt.at(i + j)) // 매칭이 되지 않을 경우
				{
					// skip = TODO; // right 표를 사용해서 몇 칸 건너뛸지 결정

					// 힌트
					// 1. 패턴 안에서 현재 몇 번째 글자를 매칭하고 있었는지와 일치하지 않는 글자와의 거리
					// 2. 패턴이 없는 글자라서 right의 값이 -1이라면 완전히 벗어나는 거리만큼 skip

					if (skip < 1) skip = 1;
					// skip < 1 이라는 것은, 패턴 안에서 볼때 현재 위치보다 오른쪽의 위치로 스킵하라는 의미가 된다.
					// 역순으로 진행을 할 수는 없기 때문에 오른쪽으로 한 칸만 옮긴다.

					break;
				}
			}

			cout << log << " ";

			if (skip == 0) // 완전히 매칭되었다는 의미
			{
				cout << "Match at " << i;
				skip = 1; // 계속 매칭 진행 (더 길게 건너뛸 수는 없을까?)
			}
			else cout << "Skip = " << skip;
			cout << endl;
		}
	}
};

int main()
{
	{
		string pat = "AABA";
		string txt = "AABAACAADAABAABA";
		BoyerMoore bm(pat);
		bm.Search(txt);
	}

	{
		string pat = "NEEDLE";
		string txt = "FINDINAHAYSTACKNEEDLEINA";
		BoyerMoore bm(pat);
		bm.Search(txt);
	}

	{
		// https://www.cs.jhu.edu/~langmea/resources/lecture_notes/strings_matching_boyer_moore.pdf
		string pat = "CTTACTTAC";
		string txt = "CGTGCCTACTTACTTACTTACTTACGCGAA";
		BoyerMoore bm(pat);
		bm.Search(txt);
	}

	return 0;
}
