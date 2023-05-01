#include <bits/stdc++.h>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
#include "API.h"
using namespace std;

pair<int, int> curr_poss;
int dir = 0;
bool stop = false;
bool visit[60][60];
bool visit_stack[60][60];
const int X = 200;
const int Y = 200;
int maze[X][Y];
int food_fill[60][60];
stack<pair<int, int>> s;

/*
                  N
                  |
                  |
            W----Cell----E
                  |
                  |
                  S

        N = (0,1)
        E = (1,0)
        S = (0,-1)
        W = (-1,0)
*/

int Horizontal[4] = {0, 1, 0, -1};
int Vertical[4] = {1, 0, -1, 0};

void print_arr()
{
    for (int i = 32; i >= 0; i--)
    {
        for (int j = 0; j <= 34; j++)
        {
            cerr << maze[j][i] << " ";
        }
        cerr << endl;
    }

    
}
void print_in()
{
    for (int i = 0; i < 35; i++)
    {
        cerr << i << " ";
    }
cerr << endl;
    
}
void print_stk()
{

    stack<pair<int, int>> teamp = s;

    while (!teamp.empty())
    {
        cerr << teamp.top().first << " " << teamp.top().second << endl;
        teamp.pop();
    }
}
void log(const string &text)
{
    cerr << text << endl;
}

void solveMaze(pair<int,int> poss,string& minans, string move )
{
	if (poss == s.top())
	{
        if(minans == "" | minans.size() > move.size())
            minans = move;
		return;
	}
	string path = "0123";
	for (int ind = 0; ind < 4; ind++)
	{
			// cout << nexti << " " << nextj << endl;
		int nexti = poss.first + Horizontal[ind];
		int nextj = poss.second + Vertical[ind];
		if ( !visit[nexti][nextj] && maze[nexti][nextj] == 1)
		{
			visit[poss.first][poss.second] = 1;
			solveMaze( {nexti,nextj} ,minans, move + path[ind]);
			visit[poss.first][poss.second] = 0;
		}
	}
}

string getpath(){
    string path = "";
    solveMaze(curr_poss,path,"");
    return path;
}

void backTrack(){
    
    string finalpath = "";
    while (s.size())
    {
        if(visit_stack[s.top().first][s.top().second])
            continue;
        
        string mazePath = getpath();
        if(finalpath == "" | finalpath.size() >= mazePath.size())
            finalpath = mazePath;
        // cerr << temp.first  << " " << temp.second << endl;
        s.pop();
    }
    cerr << finalpath << endl;
    
}

int setdir(int i){
    if(i == -1) {
        return 3;
        }
    if(i == 4) return (i%4);
    return i;
}

void findpath()
{
    char nextsid;
    log("working.....");
    // print_stk();
        // cerr << "done" << endl;
    maze[curr_poss.first][curr_poss.second] = 1; 
    visit_stack[curr_poss.first][curr_poss.second] = true;
    bool iswall[3];
    iswall[0] = API::wallLeft()  | visit_stack[curr_poss.first + Horizontal[setdir(dir - 1)] * 2][curr_poss.second + Vertical[setdir(dir - 1)] * 2]; 
    iswall[1] = API::wallFront() | visit_stack[curr_poss.first + Horizontal[dir] * 2][curr_poss.second + Vertical[dir] * 2];
    iswall[2] = API::wallRight() | visit_stack[curr_poss.first + Horizontal[setdir(dir + 1)] * 2][curr_poss.second + Vertical[setdir(dir + 1)] * 2];
    // cerr << dir << " " << iswall[0] << " " << iswall[1]<< " " << iswall[2] << " " << curr_poss.first + Horizontal[setdir(dir + 1)] * 2 <<endl;
    
    API::setColor(curr_poss.first/2 - 1, curr_poss.second/2 -1, 'g');
    /*     WALL 
            |
       WALL |  WALL 
            |           
    */
    if (iswall[0] & iswall[1] & iswall[2] ){
        //  serching and go on different point
        //  nextpoint()

        API::turnRight();
        API::turnRight();
        dir = setdir(dir + 2);
        stop = true;
        return;
    }

    /*
            
            |----
            |
            |           
    */
    else if (iswall[0] & iswall[1] ){
        
         maze[curr_poss.first + Horizontal[setdir(dir + 1)]][curr_poss.second + Vertical[setdir(dir + 1)] ] = 1;
        dir++;
         API::turnRight();
    }

    /*
        ----|
            |
            |           
    */
    else if (iswall[1] & iswall[2] ){
         maze[curr_poss.first + Horizontal[setdir(dir - 1)]][curr_poss.second + Vertical[setdir(dir - 1)] ] = 1;
        setdir(dir--); 
         API::turnLeft();
    }

    /*
            |
            |
            |
            |
            |           
    */
    else if (iswall[0] & iswall[2] ){

         maze[curr_poss.first + Horizontal[dir]][curr_poss.second + Vertical[dir] ] = 1;
    }

    /*
            |
            |
            |----
            |
            |           
    */
    else if (iswall[0]){
         maze[curr_poss.first + Horizontal[setdir(dir + 1)]][curr_poss.second + Vertical[setdir(dir + 1)] ] = 1;
         maze[curr_poss.first + Horizontal[dir]][curr_poss.second + Vertical[dir] ] = 1;
         maze[curr_poss.first + Horizontal[dir]*2][curr_poss.second + Vertical[dir] *2] = 1;
         s.push({curr_poss.first + Horizontal[dir] *2,curr_poss.second + Vertical[dir] * 2});
         dir++;
         
         API::turnRight();

    }

    /*
        ---- ----
            |
            |           
    */
    else if (iswall[1]){
        maze[curr_poss.first + Horizontal[setdir(dir + 1)]][curr_poss.second + Vertical[setdir(dir + 1)] ] = 1;
        maze[curr_poss.first + Horizontal[setdir(dir - 1)]][curr_poss.second + Vertical[setdir(dir - 1)] ] = 1;
        maze[curr_poss.first + Horizontal[setdir(dir - 1)]*2][curr_poss.second + Vertical[setdir(dir - 1)]*2 ] = 1;
        s.push({curr_poss.first + Horizontal[setdir(dir - 1)]*2 ,curr_poss.second + Vertical[setdir(dir - 1)]*2 });
        dir++;
       
        API::turnRight();
    }

    /*
            |
            |
        ----|
            |
            |           
    */
    else if (iswall[2]){
        maze[curr_poss.first + Horizontal[dir]][curr_poss.second + Vertical[dir] ] = 1;
        maze[curr_poss.first + Horizontal[setdir(dir - 1)]][curr_poss.second + Vertical[setdir(dir - 1)] ] = 1;
        maze[curr_poss.first + Horizontal[setdir(dir - 1)]*2][curr_poss.second + Vertical[setdir(dir - 1)] *2] = 1;
        s.push({curr_poss.first + Horizontal[setdir(dir - 1)]*2,curr_poss.second + Vertical[setdir(dir - 1)]*2 });

    }
    else if(!(iswall[0] | iswall[1] | iswall[2] ) ){
        maze[curr_poss.first + Horizontal[setdir(dir + 1)]][curr_poss.second + Vertical[setdir(dir + 1)] ] = 1;
        maze[curr_poss.first + Horizontal[dir]][curr_poss.second + Vertical[dir] ] = 1;
        maze[curr_poss.first + Horizontal[setdir(dir - 1)]][curr_poss.second + Vertical[setdir(dir - 1)] ] = 1;
        maze[curr_poss.first + Horizontal[dir]*2][curr_poss.second + Vertical[dir]*2 ] = 1;
        maze[curr_poss.first + Horizontal[setdir(dir - 1)]*2][curr_poss.second + Vertical[setdir(dir - 1)] *2] = 1;
        
        s.push({curr_poss.first + Horizontal[setdir(dir - 1)] *2,curr_poss.second + Vertical[setdir(dir - 1)]*2 });
        s.push({curr_poss.first + Horizontal[dir]*2 ,curr_poss.second + Vertical[dir] *2});
        dir++;
        
        API::turnRight();
    }
    dir = setdir(dir);
    curr_poss = {curr_poss.first + Horizontal[dir] * 2 , curr_poss.second + Vertical[dir] * 2};
    API::moveForward();
}

int main(int argc, char *argv[])
{
    log("Running...");
    curr_poss = {2, 2};
    dir = 0;
    API::setText(0, 0, "str");
    while (!stop)
    {
        findpath();
    }
    backTrack();
    log("comp...");
    // print_in();
    print_arr();

}
