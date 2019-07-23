#include <iostream>
#include <string>
#include "LCS.hpp"

using namespace std;

template <typename Seq>
void printSeq(const Seq &seq)
{
    for (auto &item: seq)
        cout << item << " ";
}

int main(int argc, char *argv[])
{
	string x = "ABCBDAB";
	string y = "BDCABA";

    vector<char> seqX(x.begin(), x.end());
    vector<char> seqY(y.begin(), y.end());

	LCS<vector<char>> lcs(seqX, seqY);

    cout << "idx:  "; for (int i = 0; i < seqX.size(); i++) {cout << i << " ";} cout << endl;
    cout << "seqX: "; printSeq(seqX); cout << endl;
    cout << "seqY: "; printSeq(seqY); cout << endl;
    int lcsLen = lcs.length();
    vector<int> subX;
    vector<int> subY;
    lcs.getLCS(subX, subY);
    cout << "lcs length: " << lcsLen << endl;
    cout << "subX: "; printSeq(subX); cout << endl;
    cout << "subY: "; printSeq(subY); cout << endl;

    assert(subX.size() == subY.size());
    assert(subX.size() == lcsLen);
    cout << "lcs:  ";
    for (int i = 0; i < subX.size(); i++) {
        assert(seqX[subX[i]] == seqY[subY[i]]);
        cout << seqX[subX[i]] << " ";
    }
    cout << endl;

	return 0;
}
