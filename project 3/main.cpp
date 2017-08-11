#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include <sstream>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <queue>
#include <stack>
#include <vector>
//#include "twoOpt.h"

using namespace std;

#define DEBUG 1
#define DBG_PRNT_LN if(DEBUG)

////
//////REF: https://github.com/beckysag/traveling-salesman
////
struct coords{
	int id;
	 int x;
	 int y;	
};


// Full n x n cost matrix of distances between each city
 int **graph;

// Adjacency list
// Array of n dynamic arrays, each holding a list of nodes it's index is attached to
vector< int> *adjlist;

// n x 2 array to store length of TSP path starting at each node
// col 0 => starting index   col 1 => path length from that node
 int **path_vals;
	
// List of odd nodes
vector<int>odds;

// euler circuit
vector<int>eLoop;
	
// Current shortest path length
int pathLen;

vector<coords> cityList;

//==============================================================
// calculate distances using all integers
// take the sum of the difference between the distances
// and square them. For the square root part,
// shift our sum to the left to evaluate the decimal_point
// point to the left and do floor or ceil based on value.
//==============================================================
 int getDist(coords a, coords b){
   float temp = 0.0;
   
   temp = pow((a.x - b.x),2) + pow((a.y - b.y),2);

  DBG_PRNT_LN cout<<"temp value before sqrt is ->"<<temp<<endl;
  temp = sqrt(temp);
  DBG_PRNT_LN cout<<"temp value after sqrt is ->"<<temp<<endl;
 
  return int (floor(temp + 0.5));
}
//==============================================================
//
//
//
//==============================================================
int initialize_graph_state(string filename){
coords city;
string line;
int n;
//read cities from file into a list 
//create empty graph, path and adj matrix
	ifstream inputFile(filename);
	
	while(getline(inputFile, line)){
			
		stringstream stream(line);
		stream >> city.id;
		stream >> city.x;
		stream >> city.y;
		
		cityList.push_back(city);
		
		DBG_PRNT_LN cout<<"city [x: "<<city.x<<", y: "<<city.y<<"]"<<endl;
     }
	
	DBG_PRNT_LN cout<<"number of cities read ->"<<cityList.size()<<endl;

	n = cityList.size();
	 
	// Allocate memory
	graph = new  int*[n];
	for (int i = 0; i < n; i++) {
		graph[i] = new  int[n];
		for (int j = 0; j < n; j++) graph[i][j] = 0;
	}

	path_vals = new  int*[n];
	for (int i = 0; i < n; i++) {
		path_vals[i] = new  int[n];
	}

	// Adjacency list
	adjlist = new vector< int> [n];	

	inputFile.close();
	
	return n;
}

//==============================================================
//
//
//
//==============================================================
void calcDistMatrix(int n){
// fill matrix with distances from every city to every other city

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			graph[i][j] = graph[j][i] =  getDist(cityList[i], cityList[j]);
			DBG_PRNT_LN cout<<"Dst["<<i<<","<<j<<"]"<<graph[i][j]<<" || ";
		}
		DBG_PRNT_LN cout<<endl;
	}	
}

//==============================================================
//
//
//
//==============================================================
void cleanUp(int n){
	for (int i = 0; i < n; i++) {
		delete [] graph[i];
		delete [] path_vals[i];
	}
	delete [] path_vals;
	delete [] graph;
	delete [] adjlist;	
	
}

//==============================================================
// Borrowed verbatim from REF:
//
//
//==============================================================
// findMST helper function
int minKey( int key[], bool mstSet[], int n) {
	// Initialize min value
	 int min = std::numeric_limits<int>::max();
	 int min_index;
	for (int v = 0; v < n; v++)
		if (mstSet[v] == false && key[v] < min) {
			min = key[v];
			min_index = v;
		}
	return min_index;
}

//==============================================================
// use function verbatim from REF:https://github.com/beckysag/traveling-salesman
//
//
//==============================================================
void findMST(int n) {
	/////////////////////////////////////////////////////
	// In each iteration, we choose a minimum-weight
	// edge (u, v), connecting a vertex v in the set A to
	// the vertex u outside of set A
	/////////////////////////////////////////////////////

	 int key[n];   // Key values used to pick minimum weight edge in cut
	bool in_mst[n];  // To represent set of vertices not yet included in MST
	 int parent[n];

	// For each vertex v in V
	for (int v = 0; v < n; v++) {
		// Initialize all keys to infinity
		key[v] = std::numeric_limits<int>::max();

		// Mark as not being in mst yet
		in_mst[v] = false;
	}

	// Node 0 is the root node so give it the lowest distance (key)
	key[0] = 0;
	parent[0] = -1; // First node is always root of MST

	for (int i = 0; i < n - 1; i++) {
		// Find closest remaining (not in tree) vertex
		// TO DO : This would be better represented by heap/pqueue
		 int v = minKey(key, in_mst, n);

		// Add vertex v to the MST
		in_mst[v] = true;

		// Look at each vertex u adjacent to v that's not yet in mst
		for (int u = 0; u < n; u++) {
			if (graph[v][u] && in_mst[u] == false && graph[v][u] < key[u]) {
				// Update parent index of u
				parent[u] = v;

				// Update the key only if dist is smaller than key[u]
				key[u] = graph[v][u];
			}
		}
	}

	// map relations from parent array onto matrix
	for (int v1 = 0; v1 < n; v1++) {
		// there is an edge between v1 and parent[v1]
		 int v2 = parent[v1];
		if (v2 != -1) {
			adjlist[v1].push_back(v2);
			adjlist[v2].push_back(v1);
		}
	}
}

//==============================================================
// use function verbatim from REF:https://github.com/beckysag/traveling-salesman
//
//
//==============================================================
void findOdds(int n) {
	/////////////////////////////////////////////////////
	// Find nodes with odd degrees in T to get subgraph O
	/////////////////////////////////////////////////////

	// store odds in new vector for now

	for (int r = 0; r < n; r++) {
		//cities[r].isOdd = ((adjlist[r].size() % 2) == 0) ? 0 : 1;
		if ((adjlist[r].size() % 2) != 0 ) {
			odds.push_back(r);
		}
	}
}
//==============================================================
// use function verbatim from REF:https://github.com/beckysag/traveling-salesman
//
//
//==============================================================
void perfect_matching(int n) {
	/////////////////////////////////////////////////////
	// find a perfect matching M in the subgraph O using greedy algorithm
	// but not minimum
	/////////////////////////////////////////////////////
	int closest, length; //int d;
	std::vector<int>::iterator tmp, first;

	// Find nodes with odd degrees in T to get subgraph O
	findOdds(n);

	// for each odd node
	while (!odds.empty()) {
		first = odds.begin();
		vector<int>::iterator it = odds.begin() + 1;
		vector<int>::iterator end = odds.end();
		length = std::numeric_limits<int>::max();

		for (; it != end; ++it) {
			// if this node is closer than the current closest, update closest and length
			if (graph[*first][*it] < length) {
				length = graph[*first][*it];
				closest = *it;
				tmp = it;
			}
		}	// two nodes are matched, end of list reached

		adjlist[*first].push_back(closest);
		adjlist[closest].push_back(*first);
		odds.erase(tmp);
		odds.erase(first);
	}
}

//==============================================================
// use function verbatim from REF:https://github.com/beckysag/traveling-salesman
//
//
//==============================================================
// Take reference to a path vector
// so can either modify actual euler path or a copy of it
void euler (int pos, vector<int> &path, int n) {
	/////////////////////////////////////////////////////////
	// Based on this algorithm:
	//	http://www.graph-magics.com/articles/euler.php
	// we know graph has 0 odd vertices, so start at any vertex
	// O(V+E) complexity
	/////////////////////////////////////////////////////////

	// make copy of original adjlist to use/modify
	vector<int> *temp = new vector<int> [n];
	for (int i = 0; i < n; i++) {
		temp[i].resize(adjlist[i].size());
		temp[i] = adjlist[i];
	}

	path.clear();

	// Repeat until the current vertex has no more neighbors and the stack is empty.
	stack<int> stk;
	while (!stk.empty() || temp[pos].size() > 0 ) {
		// If current vertex has no neighbors -
		if (temp[pos].size() == 0) {
			// add it to circuit,
			path.push_back(pos);
			// remove the last vertex from the stack and set it as the current one.
			int last = stk.top();
			stk.pop();
			pos = last;
		}
		// Otherwise (in case it has neighbors)
		else {
			// add the vertex to the stack,
			stk.push(pos);
			// take any of its neighbors,
			int neighbor = temp[pos].back();
			// remove the edge between selected neighbor and that vertex,
			temp[pos].pop_back();
	        for (unsigned int i = 0; i < temp[neighbor].size(); i++)
	            if (temp[neighbor][i] == pos) { // find position of neighbor in list
	        	    temp[neighbor].erase (temp[neighbor].begin() + i); // remove it
	                break;
	            }
			// and set that neighbor as the current vertex.
	        pos = neighbor;
		}
	}
	path.push_back(pos);
}

//==============================================================
// use function verbatim from REF:https://github.com/beckysag/traveling-salesman
//
//
//==============================================================
void make_hamilton(vector<int> &path, int &path_dist, int n) {
	// remove visited nodes from Euler tour
	bool visited[n]; // boolean value for each node if it has been visited yet
	memset(visited, 0, n * sizeof(bool));

	path_dist = 0;

	int root = path.front();
	vector<int>::iterator curr = path.begin();
	vector<int>::iterator next = path.begin()+1;
	visited[root] = true;

	// loop until the end of the circuit list is reached
	while ( next != path.end() ) {
		// if we haven't been to the next city yet, go there
		if (!visited[*next]) {
			path_dist += graph[*curr][*next];
			curr = next;
			visited[*curr] = true;
			next = curr + 1;
		}else {
			next = path.erase(next); // remove it
		}
	}

	// add the distance back to the root
	path_dist += graph[*curr][*next];
}

//==============================================================
// use function verbatim from REF:https://github.com/beckysag/traveling-salesman
//
//
//==============================================================
// Does euler and hamilton but doesn't modify any variables
// Just finds path length from the node specified and returns it
int find_best_path (int pos, int n) {
	// create new vector to pass to euler function
	vector<int>path;
	euler(pos, path, n);

	// make it hamiltonian, pass copy of vars
	int length;

	make_hamilton(path, length, n);

	// Optimize
/*
	twoOpt(graph, path, length, n);
	twoOpt(graph, path, length, n);
	twoOpt(graph, path, length, n);
	twoOpt(graph, path, length, n);
	twoOpt(graph, path, length, n);
*/
	return length;
}
//==============================================================
//
//
//
//==============================================================
void create_tour(int pos, int n){
	// call euler with actual circuit vector
	euler(pos, eLoop, n);

	// make it hamiltonian
	// pass actual vars
	make_hamilton(eLoop, pathLen, n);
}

//==============================================================
//
//
//
//==============================================================
void best_path_search(int n){
int bestIndex, best = INT_MAX;	

	for(int i = 0; i < n; i++){
		int length = find_best_path (i, n);
		path_vals[i][0] = i;
		path_vals[i][1] = length;
		
	   if (path_vals[i][1] < best) {
			bestIndex = path_vals[i][0];
			best = path_vals[i][1];
			
			DBG_PRNT_LN cout<<"best length: "<<best<<"from ->"<<bestIndex;
		 }
		
		DBG_PRNT_LN cout<<"Node: "<<i<<", len: "<<length<<endl;
     }

	create_tour(bestIndex, n);
/*	twoOpt(graph, eLoop, pathLen, n);
	twoOpt(graph, eLoop, pathLen, n);
	twoOpt(graph, eLoop, pathLen, n);
	twoOpt(graph, eLoop, pathLen, n);
	twoOpt(graph, eLoop, pathLen, n);	
*/	
}
//==============================================================
//
//
//
//==============================================================
int main(void){
string filename;
int capacity;

	cout<<"Enter a tour instance filename: ";
	cin>>filename;
	
 	ofstream outputFile(filename + ".tour");
	
	capacity = initialize_graph_state(filename);
	calcDistMatrix(capacity);
	findMST(capacity);
	perfect_matching(capacity);
	best_path_search(capacity);
	
	outputFile<<pathLen<<endl;
	DBG_PRNT_LN cout<<"pathLen: "<<pathLen<<endl;
	for(int i = 0; i < eLoop.size(); i++){
	    outputFile<<eLoop[i]<<endl;
		DBG_PRNT_LN cout<<"["<<eLoop[i]<<"] ";
	  }
	 
//good housekeeping	
	cleanUp(capacity);
	outputFile.close();
	
  return 0;	
}