/*******************************************************************************
LCA.h

Data structure for LCA computations on a binary tree
Implementation of the RMQ-based methods of Bender and Farach-Colton

Copyright 2010 Chris Whidden
cwhidden@dal.ca
http://kiwi.cs.dal.ca/Software/RSPR
April 28, 2010
Version 1.02

This file is part of rspr.

rspr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

rspr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with rspr.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#ifndef INCLUDE_CSTDIO
	#define INCLUDE_CSTDIO
	#include <cstdio>
#endif
#ifndef INCLUDE_STRING
	#define INCLUDE_STRING
	#include <string>
#endif
#ifndef INCLUDE_IOSTREAM
	#define INCLUDE_IOSTREAM
	#include <iostream>
#endif
#ifndef INCLUDE_NODE
	#define INCLUDE_NODE
	#include "Node.h"
#endif
#ifndef INCLUDE_VECTOR
	#define INCLUDE_VECTOR
	#include <vector>
#endif
#ifndef INCLUDE_MATH
	#define INCLUDE_MATH
	#include <cmath>
#endif
using namespace std;

int mylog2 (int val) {
    int ret = -1;
    while (val != 0) {
        val >>= 1;
        ret++;
    }
    return ret;
}

class LCA {
	private:
	Node *tree;
	vector<int> E;		// preorder numbers of euler tour
	vector<int> L;		// levels of euler tour
	vector<int> H;		// first occurence of a preorder number in E
	vector<Node *> N;	// preorder to node mapping
	vector<vector<int> > RMQ;	// precomputed RMQ values

	public:
	LCA(Node *tree) {
		this->tree = tree;
		euler_tour(tree, 0);
		precompute_rmq();
	}
	void euler_tour(Node *node, int depth) {
		// First visit
		int preorder_number = N.size();
		int euler_number = E.size();
		N.push_back(node);
		node->set_preorder_number(preorder_number);

		cout << preorder_number << "\t";
		node->print_subtree();
		H.push_back(euler_number);
		L.push_back(depth);
		E.push_back(preorder_number);

		// Left child
		if(node->lchild() != NULL) {
			euler_tour(node->lchild(), depth+1);
			// Middle visit
			L.push_back(depth);
			E.push_back(preorder_number);
		}


		// Right child
		if(node->rchild() != NULL) {
			euler_tour(node->rchild(), depth+1);
			// Last visit
			L.push_back(depth);
			E.push_back(preorder_number);
		}

	}

	void precompute_rmq() {
		// E gives queries of length 1
		RMQ.push_back(E);
		for(int length = 1; length < E.size(); length *= 2) {
			vector<int> V = vector<int>();
			for(int start = 0; start < E.size() - length; start++) {
				V.push_back(rmq(start, start + length));
			}
			RMQ.push_back(V);
		}
	}
	//vector<vector<int> > RMQ;	// precomputed RMQ values

	// find the index of the rmq between indices i and j of E
	int rmq(int i, int j) {
		if (i == j)
			return E[i];
		int length = j - i - 1;
		//cout << "  " << i << " " << j << endl;
		//cout << "  " << length << endl;
		//cout << "  " << mylog2(length) << endl;
		length = (mylog2(length));
		//cout << "  " << length << endl;
		//cout << "  " << j - (1 << (length)) << endl;
		int rmq1 = RMQ[length+1][i];
		int rmq2 = RMQ[length+1][j - (1 << (length))];
		if (rmq1 < rmq2)
			return rmq1;
		return rmq2;
	}
	Node *get_lca(Node *a, Node *b) {
		int preorder_a = a->get_preorder_number();
		int preorder_b = b->get_preorder_number();
		int lca_index;
		if (preorder_a <= preorder_b)
			lca_index = rmq(H[preorder_a], H[preorder_b]);
		else
			lca_index = rmq(H[preorder_b], H[preorder_a]);
		return N[lca_index];
	}
	/* copy constructor
	LCA(const LCA &n) {
	}
	*/
	void debug() {
		for(int i = 0; i < E.size(); i++) {
			cout << " " << E[i];
		}
		cout << endl;
		cout << endl;
		for(int i = 0; i < L.size(); i++) {
			cout << " " << L[i];
		}
		cout << endl;
		cout << endl;
		for(int i = 0; i < H.size(); i++) {
			cout << " " << H[i];
		}
		cout << endl;
		cout << endl;
		cout << endl;
		for(int i = 0; i < RMQ.size(); i++) {
			for(int j = 0; j < RMQ[i].size(); j++) {
			cout << " " << RMQ[i][j];
			}
			cout << endl;
			cout << endl;
		}
	}
};

