#include <iostream>
#include <string>
using namespace std;

// CLRS 4th 32.1
void NaiveStringMatcher(string& pat, string& txt)
{
	int M = pat.size();
	int N = txt.size();

	cout << txt << " " << pat << endl;
	for (int i = 0; i < N; i++)
		cout << i % 10;
	cout << endl;

	// for (TODO)
	//{
	//}

	cout << endl;
}

int main()
{
	string txt1 = "acaabc";
	string pat1 = "aab";
	NaiveStringMatcher(pat1, txt1);

	string txt2 = "AABAACAADAABAABA";
	string pat2 = "AABA";
	NaiveStringMatcher(pat2, txt2);

	return 0;
}
