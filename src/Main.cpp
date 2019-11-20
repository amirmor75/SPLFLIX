#include <iostream>
#include "../include/Session.h"
using namespace std;

int main(int argc, char** argv){

    std::vector<int> myvector (3,100);
    std::vector<int>::iterator it;

    it = myvector.begin();
    it = myvector.insert ( it , 200 );

    myvector.insert (it,2,300);

    // "it" no longer valid, get a new one:
    it = myvector.begin();

    std::vector<int> anothervector (2,400);
    myvector.insert (it+2,anothervector.begin(),anothervector.end());

    int myarray [] = { 501,502,503 };
    myvector.insert (myvector.begin(), myarray, myarray+3);

    std::cout << "myvector contains:";
    for (it=myvector.begin(); it<myvector.end(); it++)
        std::cout << ' ' << *it;
    std::cout << '\n';















    /*



//	if(argc!=2)
//	{
//		cout << "usage splflix input_file" << endl;
//		return 0;
//	}
//	Session s(argv[1]);
//	s.start();

    std:string str="/home/tal/CLionProjects/SPLFLIX/src/config1.json";
    Session s(str);
	return 0;

     */
}
