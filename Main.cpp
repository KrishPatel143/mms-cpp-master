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
int count = 0;
bool stop = false;
bool Isfounded = false;
bool visit[60][60];
bool visit_stack[60][60];
const int X = 200;
const int Y = 200;
int maze[X][Y];
int food_fill[60][60];
vector<pair<int, int>> s;
int stack_top = 0;
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
    for (int i = 0; i <= 37; i++)
    {
        for (int j = 0; j <= 37; j++)
        {
            cerr << maze[i][j] << ",";
        }
        cerr << "},{";
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
    for (int i = 0; i < s.size(); i++)
    {
        cerr << s[i].first << " " << s[i].second << endl;
    }
}

void chackAt(int i,int j,int a, int b){
    if (curr_poss.first == i & curr_poss.second == j)
    {
        // print_arr();
        // print_stk();
        cerr << a << " " << b << endl;
    }
}

void chackAt(int i,int j,string str){
    if (curr_poss.first == i & curr_poss.second == j)
    {
        // print_arr();
        // print_stk();
        cerr << str << endl;
    }
}

void log(const string &text)
{
    cerr << text << endl;
}

int setdir(int i){
    if(i == -1) {
        return 3;
        }
    if(i >= 4) return (i%4);
    return i;
}

void solveMaze(pair<int,int> poss,string& minans, string move )
{
	if (poss == s[stack_top])
	{
        minans = move;
        Isfounded = true;
        return;
	}
	string path = "0123";
	for (int ind = 0; ind < 4; ind++)
	{
			// cout << nexti << " " << nextj << endl;
		int nexti = poss.first + Horizontal[ind];
		int nextj = poss.second + Vertical[ind];
		if ( !visit[nexti][nextj] && maze[nexti][nextj] == 1 && !Isfounded)
		{
			visit[poss.first][poss.second] = 1;
			solveMaze( {nexti,nextj} ,minans, move + path[ind]);
			visit[poss.first][poss.second] = 0;
		}
	}
}

void findpathinMaze(int a,int b,pair<int,int> poss,string& minans, string move )
{
	if (poss.first == a & poss.second == b )
	{
        if (minans == "" | move <= minans)
        { 
            minans = move;
            Isfounded = true;
            return;
        }
        
	}
	string path = "0123";
	for (int ind = 0; ind < 4; ind++)
	{
			// cout << nexti << " " << nextj << endl;
		int nexti = poss.first + Horizontal[ind];
		int nextj = poss.second + Vertical[ind];
		if ( !visit[nexti][nextj] && maze[nexti][nextj] == 1 && !Isfounded)
		{
			visit[poss.first][poss.second] = 1;
			findpathinMaze(a,b, {nexti,nextj} ,minans, move + path[ind]);
			visit[poss.first][poss.second] = 0;
		}
	}
}

void followPath(string path){
    for (int i = 0; i < path.size(); i += 2)
    { 
        int path_dir = (int)path[i] - 48;
        if(setdir(dir - 1) == path_dir)
        {
            API::turnLeft();
        }
        else if(setdir(dir + 1) == path_dir)
        {
            API::turnRight();
        }
        else if(setdir(dir + 2) == path_dir)
        {
            API::turnRight();
            API::turnRight();
        }
        dir = setdir(path_dir);
        API::moveForward();
        curr_poss = {curr_poss.first + Horizontal[dir] * 2,curr_poss.second + Vertical[dir] * 2};

    }
}

void fastpath(string path){
    for (int i = 0; i < path.size(); i += 2)
    { 
        int path_dir = (int)path[i] - 48;
        if(setdir(dir - 1) == path_dir)
        {
            API::turnLeft();
        }
        else if(setdir(dir + 1) == path_dir)
        {
            API::turnRight();
        }
        else if(setdir(dir + 2) == path_dir)
        {
            API::turnRight();
            API::turnRight();
        }
        dir = setdir(path_dir);
        API::moveForward();
        curr_poss = {curr_poss.first + Horizontal[dir] * 2,curr_poss.second + Vertical[dir] * 2};
        API::setColor(curr_poss.first/2 - 1, curr_poss.second/2 -1, 'r');

    }
}

string getpath(){
    string path = "";
    solveMaze(curr_poss,path,"");
    return path;
}

void backTrack(){
    vector<int> remove_ele;
    string finalpath = "";
    Isfounded = false;


    for (stack_top = s.size() - 1; stack_top >= 0; stack_top--)
    {
        if(visit_stack[s[stack_top].first][s[stack_top].second]){ 
            remove_ele.push_back(stack_top);
            continue;
        }
        finalpath = getpath();
        if(finalpath != "" ){
            remove_ele.push_back(stack_top);
            break;
        }
    }
    for (int i = 0; i < remove_ele.size() ; i++)
    {
        s.erase(s.begin() + remove_ele[i]);
    }
    if(s.size() == 0 & finalpath.size() == 0){
        stop = true;  
        return;
    }
    
    
    followPath(finalpath); 
}

void findpath()
{
    char nextsid;
    // log("working.....");
 
    
    cerr << curr_poss.first << " " << curr_poss.second << endl;
    maze[curr_poss.first][curr_poss.second] = 1; 
    visit_stack[curr_poss.first][curr_poss.second] = true;
  
    bool iswall[3];
    iswall[0] = API::wallLeft()  | visit_stack[curr_poss.first + Horizontal[setdir(dir - 1)] * 2][curr_poss.second + Vertical[setdir(dir - 1)] * 2]; 
    iswall[1] = API::wallFront() | visit_stack[curr_poss.first + Horizontal[dir] * 2][curr_poss.second + Vertical[dir] * 2];
    iswall[2] = API::wallRight() | visit_stack[curr_poss.first + Horizontal[setdir(dir + 1)] * 2][curr_poss.second + Vertical[setdir(dir + 1)] * 2];
    
    API::setColor(curr_poss.first/2 - 1, curr_poss.second/2 -1, 'g');
    /*     WALL 
            |
       WALL |  WALL 
            |           
    */
    if (iswall[0] & iswall[1] & iswall[2] ){
        //  serching and go on different point
        //  nextpoint()
        backTrack();
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
         s.push_back({curr_poss.first + Horizontal[dir] *2,curr_poss.second + Vertical[dir] * 2});
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
        s.push_back({curr_poss.first + Horizontal[setdir(dir - 1)]*2 ,curr_poss.second + Vertical[setdir(dir - 1)]*2 });
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
        s.push_back({curr_poss.first + Horizontal[setdir(dir - 1)]*2,curr_poss.second + Vertical[setdir(dir - 1)]*2 });

    }
    else if(!(iswall[0] | iswall[1] | iswall[2] ) ){
        maze[curr_poss.first + Horizontal[setdir(dir + 1)]][curr_poss.second + Vertical[setdir(dir + 1)] ] = 1;
        maze[curr_poss.first + Horizontal[dir]][curr_poss.second + Vertical[dir] ] = 1;
        maze[curr_poss.first + Horizontal[setdir(dir - 1)]][curr_poss.second + Vertical[setdir(dir - 1)] ] = 1;
        maze[curr_poss.first + Horizontal[dir]*2][curr_poss.second + Vertical[dir]*2 ] = 1;
        maze[curr_poss.first + Horizontal[setdir(dir - 1)]*2][curr_poss.second + Vertical[setdir(dir - 1)] *2] = 1;
        
        s.push_back({curr_poss.first + Horizontal[setdir(dir - 1)] *2,curr_poss.second + Vertical[setdir(dir - 1)]*2 });
        s.push_back({curr_poss.first + Horizontal[dir]*2 ,curr_poss.second + Vertical[dir] *2});
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
    print_arr();
    visit_stack[2][2] = 0;
    s.push_back({2,2});
    backTrack();
    while (dir != 0)
    {
        API::turnRight();
        dir = setdir(dir+1);
    }
    cerr << dir << endl;
    Isfounded = false;
    string fpath = "";
    findpathinMaze(18,18,curr_poss,fpath,"");
    fastpath(fpath);
}
