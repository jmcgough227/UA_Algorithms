/----------Algorithms Project 2: Huffman Encoding----------\
Program and readme by: Jason McGough
Email: jrm43@zips.uakron.edu

This readme will explain how to run my program for this project, as well as describe 
the components of said program.
Test cases are not included with the submitted files. It is assumed that the 
professor and grader have access to these.

Running the Huffman Encoding program:

	This project was designed in Visual Studio 2010 and was not tested under 
	any other compilers. Therefore, I recommend loading the huffman.cpp file 
	into Visual Studio 2010, building it there, and running it.

	When the program loads, it will place you at the programs menu. A menu 
	header reading "-------Huffman Encoding-------" will appear with a list of 
	options available underneath it. Each option is preset to load a particular 
	test case that has been provided for this project. Option 7, however, will 
	allow you to load a file by whatever name you choose. Option 8 will exit 
	the program. After each selection, the program will perform operations to 
	build and display the Huffman codes and then return to the menu for further 
	testing or to allow the option to exit the program.

Description of program components:

	Node, Internal_Node, and Leaf_Node - This program constructs it's Huffman 
		code tree using a system of node classes. Node is the generalized 
		representation of nodes found in the tree and where a nodes 
		frequency is stored. Internal_Node and Leaf_Node both inherit from 
		Node, but have distinct properties. Internal_Node has two pointers, 
		left and right, that point further down the tree. Leaf_Node stores 
		a character and has no children.

	BoolQueue - This class is used soley for encoding, and holds a static queue 
			of bools for forming bytes.

	encode() - Takes in a reference to an unsigned char vector stored in main 
			(to store the encoded data), the mapped Huffman codes, and 
			the original data to be encoded. Forms bytes by pushing bits 
			onto the BoolQueue and then taking those bytes and placing 
			them into the unsigned char vector.

	file_read() - Reads in data from the selected file. The size of the file is 
			found in this function and set to a reference found in the 
			main() function.

	file_write() - Takes the data encoded by the encode() function and writes it 
			to an hzip file.

	build_tree() - Builds a Huffman coding tree from a Node type root using a 
			priority queue. Unique characters are assigned to Leaf_Node 
			type nodes and are connected by Internal_Node type nodes. 
			The top of the tree is then returned.

	gen_codes() - Generates codes recursively. Base case is to find a Leaf_Node 
			type node within the Huffman coding tree and matches its 
			character on the Huffman code map to it's respective code 
			prefix. 

	output_tree() - This function outputs the post_order tree traversal of the 
			Huffman code tree recursively. The base case sought is a 
			node of Leaf_Node type, in which it's character and 
			frequency are then output. It first recurses left, and 
			then right. After going as far as it can both left then 
			right, an "@" is output to denote an internal node.

	main() - The programs menu is stored directly within main, contained by a 
			while loop and controlled by a switch statement. Each case 
			in the switch statement is an option on the menu. If invalid 
			data is entered for a selection, then the program will assert 
			and intentionally crash the program.
		
		 After each selection, any previous data is cleared out of variables. 
			The selected file is then read, and the Huffman coding tree 
			is built from the data extracted from the file. Huffman codes 
			are then output, as well as file sizes and the post-order tree 
			traversal. The data is then encoded and written into a hzip 
			file by the same name as the file in which the data 
			was read from. The program then loops back to the menu.