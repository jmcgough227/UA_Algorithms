/************************************************************
* Name: Jason McGough
* Email: jrm43@zips.uakron.edu
* Project: Huffman Coding
* File : huffman.cpp
*************************************************************/

//#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <iterator>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <assert.h>

using namespace std;

//global variables used throughout program to store huffman codes
typedef vector<bool> h_codes;
typedef map<char, h_codes> h_map;

//tracks unique characters
const int unique_chars = 1 << (CHAR_BIT - 1);

//Base node structure
class Node
{
public:
	int freq;

	virtual ~Node() {}

protected:
	Node(int f) : freq(f) {}
};

//Internal node structure
class Internal_Node : public Node
{
public:
	Node* left;
	Node* right;

	Internal_Node(Node* l, Node* r) : Node(l->freq + r->freq), left(l), right(r) {}

	Internal_Node() : Node(0), left(NULL), right(NULL){}
	Internal_Node(int f) : Node(f), left(NULL), right(NULL){}
	~Internal_Node() {delete left, right;}
};

//Leaf node structure
class Leaf_Node : public Internal_Node
{
public:
	char ch;

	Leaf_Node(int f, char c) : Internal_Node(f), ch(c){}

	Leaf_Node() : ch(0){}
};

struct Compare_Nodes
{
	bool operator() (const Node* l, const Node* r) const {return l->freq > r->freq;}
};

//****************************************************************************************

// Class for holding a static queue of bools
class BoolQueue
{
public:
	bool data[100];
	int front;
	int back;
	int sz;

	BoolQueue() : front(0), back(0), sz(0){}

	void push(bool b)
	{
		data[(back++)%100] = b;
		sz++;
	}

	bool pop()
	{
		sz--;
		return data[(front++)%100];
	}

	int size() {return sz;}
};

//encodes text using the huffman codes
void encode(vector<unsigned char> &out_data, h_map &codes, string &str)
{
	BoolQueue enc;

	int i;
	bool added = true;
	unsigned char byte = 0;

	for(i = 0;i < str.length();i++)
	{
		if(!added)
		{
			out_data.push_back(byte);
			byte = 0;
		}
		
		added = true;
		vector<bool> &code = codes[str[i]];

		//push the bits into the queue
		for(int d = 0;d < code.size();d++)
		{
			enc.push(code[d]);
		}

		//pack the bits into bytes
		while(enc.size() >= 8)
		{
			if(!added)
			{
				out_data.push_back(byte);
				byte = 0;
			}

			added = false;

			for(int d = 0;d < 8;d++)
			{
				byte = (byte << 1) | (int)enc.pop();
			}
		}
	}

	//handle anything left in the queue
	while(enc.size() >= 8)
	{
		if(!added)
		{
			out_data.push_back(byte);
			byte = 0;
		}

		added = false;

		for(int d = 0;d < 8;d++)
		{
			byte = (byte << 1) | (int)enc.pop();
		}
	}

	if(!added)
	{
		out_data.push_back(byte);
		byte = 0;
	}


	//special case of when the file size is not a multiple of 8
	while(((i++) % 8) != 0)
	{
		byte<<=1;
	}

	out_data.push_back(byte);
}

//****************************************************************************************

//read sample data from file
char* file_read(string file_n, int& size)
{
	char *data;
	FILE *file = fopen(file_n.c_str(), "rb");

	if(!file) return NULL;

	//Gets the size of the file
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	data = new char[size+1];

	//read file into data
	fread(data, 1, size, file);

	data[size] = 0;

	fclose(file);

	return data;
}

//write huffman code to file
bool file_write(string file_n, const unsigned char* data, int size)
{
	FILE *file = fopen(file_n.c_str(), "wb");

	if(!file) return false;

	//write into hzip file
	fwrite(data, 1, size, file);

	fclose(file);

	return true;
}

//builds the Huffman code tree
Node* build_tree(int freqs[unique_chars])
{
	priority_queue<Node*, vector<Node*>, Compare_Nodes> h_tree;

	for(int i = 0; i < unique_chars; i++)
	{
		if(freqs[i] != 0)
		{
			h_tree.push(new Leaf_Node(freqs[i], (char)i));
		}

	}

	while (h_tree.size() > 1)
	{
		Node* node_R = h_tree.top();
		h_tree.pop();
		Node* node_L = h_tree.top();
		h_tree.pop();

		Node* node = new Internal_Node(node_R, node_L);
		h_tree.push(node);
	}

	return h_tree.top();
}

//
void gen_codes(const Node* node, h_codes &pre, h_map& codes)
{
	//base case - node is a leaf node
	if(const Leaf_Node *l = dynamic_cast<const Leaf_Node*>(node))
	{
		codes[l->ch] = pre;
	}

	//if node is an internal node... recurse until base case
	else if(const Internal_Node* in = dynamic_cast<const Internal_Node*>(node))
	{
		h_codes left_pre = pre;
		left_pre.push_back(true);
		gen_codes(in->left, left_pre, codes);

		h_codes right_pre = pre;
		right_pre.push_back(false);
		gen_codes(in->right, right_pre, codes);
	}
}


//Outputs Huffman code tree in post-order traversal
void output_tree(Node* node)
{
	//base case - node is a leaf node
	if(node == dynamic_cast<const Leaf_Node*>(node))
	{
		cout << dynamic_cast<const Leaf_Node*>(node)->ch << node->freq;
		return;
	}

	//left first
	output_tree(dynamic_cast<const Internal_Node*>(node)->left);

	//then right
	output_tree(dynamic_cast<const Internal_Node*>(node)->right);

	//denotes internal node
	cout << "@" << node->freq;
}

int main()
{
	bool menu = true;
	string file_name = "";
	int file_size = 0;

	//Ye Olde Menu
	while(menu)
	{
		cout << "-------Huffman Encoding-------\n";
		cout << "Please choose a file to load: \n\n";
		cout << " [1] Test case 1 (a.txt) \n";
		cout << " [2] Test case 2 (ab.txt) \n";
		cout << " [3] Test case 3 (abc.txt) \n";
		cout << " [4] Test case 4 (m_lines.txt) \n";
		cout << " [5] Test case 5 (m_lines2.txt) \n";
		cout << " [6] Test case 6 (bible.txt) \n";
		cout << " [7] Test different cases \n";
		cout << " [8] Exit program \n\n";

		int select;
		cin >> select;
		cout << "\n\n";

		string test_file = "";

		//select file by option rather than typing in file name
		switch(select)
		{
			case 1: file_name = "a";
					break;

			case 2: file_name = "ab";
					break;

			case 3: file_name = "abc";
					break;

			case 4: file_name = "m_lines";
					break;

			case 5: file_name = "m_lines2";
					break;

			case 6: file_name = "bible";
					break;

			//unless, of course, you want to type in the file name of something else
			case 7: cout << " Please Input the name of the file you wish to test ('.txt' will be appended automatically): \n";
					cin >> test_file;
					cout << "\n\n";
					
					file_name = test_file;
					test_file = "";
					break;

			//exit program
			case 8: menu = false;
					return 0;

			//exit program if input is not appropriate to menu
			default: assert(select > 8 || select < 1);
		}

		//set character frequencies to the empty set
		int freqs[unique_chars] = {0};


		//reset frequencies to 0 iteratively
		for(int i = 0; i < unique_chars; i++) freqs[i] = 0;

		const char* data = file_read(file_name + ".txt", file_size);

		//calculate length of file data string
		int length = strlen(data);

		for(int i = 0; i < length; i++)
		{
			++freqs[data[i]];
		}

		//build Huffman Code tree
		Node* root = build_tree(freqs);

		//prepare map for codes
		h_map codes;
		gen_codes(root, h_codes(), codes);

		//Output list of characters with code lengths and codes
		for(h_map::const_iterator it = codes.begin(); it != codes.end(); ++it)
		{
			cout << it->first << "(" << it->second.size() << "): \t";
			copy(it->second.begin(), it->second.end(), ostream_iterator<bool>(cout));
			cout << endl;
		}

		cout << "\nFile size is: " << file_size << " before encoding.\n\n";

		//Output post-order traversal of huffman coding tree
		output_tree(root);
		cout << "\n\n";

		//data is then encoded into a vector
		vector<unsigned char> data_encoded;
		encode(data_encoded, codes, (string)data);

		int encoded_size = data_encoded.size();

		cout << "\nFile size is: " << encoded_size << " after encoding.\n\n";

		//encoded data is then written to a hzip file of the same name as the file
		file_write(file_name + ".hzip", &data_encoded[0], encoded_size);

		//clean up our huffman tree
		delete root;
	}

	return 0;
}

