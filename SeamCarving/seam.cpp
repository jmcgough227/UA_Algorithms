/*************************************************************
* Project: Seam Carving
* Name: Jason McGough
* Email: jrm43@zips.uakron.edu
* Class: Algorithms
**************************************************************/

//check to make sure element access is working properly

#include <string>
#include <iostream>
#include <fstream>
//#include <assert.h>
#include <cmath>
#include <time.h>

using namespace std;

//Stores image in a one dimensional array.
//Example element access: element(Column, Row) = array[Row*max_col+Column]
class Pixel_Array
{
private:
	int max_row, max_col;
	int* arr;

public:
	Pixel_Array() : max_row(0), max_col(0), arr(NULL) {}
	Pixel_Array(int, int);
	~Pixel_Array() { delete arr; }

	//Calls explicit constructor to build array.
	//Useful if default construction was used.
	void build_array(int y, int x) { Pixel_Array(y, x); }

	int get_max_col() { return max_col; }
	int get_max_row() { return max_row; }

	//element accessor
	int get_element(int col, int row)
		{ return arr[row*max_col+col]; }

	void set_element(int col, int row, int val) 
		{ arr[row*max_col+col] = val; }
};

Pixel_Array::Pixel_Array(int y, int x)
{
	max_row = y;
	max_col = x;
	arr = new int[max_row*max_col];
}

/****************************************************************************/

//-----------------------INCOMPLETE
//load pgm file
void file_load(string& f_name, Pixel_Array& pic, int& total_elements)
{
	FILE *file = fopen(f_name.c_str(), "rb");



	fclose(file);
}

//-----------------------INCOMPLETE
//save file in format "original_image_file_name_processed.pgm"
void file_save(string& f_name, Pixel_Array& processed_pic, 
				int& total_elements)
{


}

//-----------------------INCOMPLETE
int calc_energy(Pixel_Array& pix_val, int i, int j)
{
	if(i == 0 && j == 0)
	{

	}

	return abs(pix_val.get_element(i, j) - pix_val.get_element(i-1, j))
		 + abs(pix_val.get_element(i, j) - pix_val.get_element(i+1, j))
		 + abs(pix_val.get_element(i, j) - pix_val.get_element(i, j-1))
		 + abs(pix_val.get_element(i, j) - pix_val.get_element(i, j+1));

	//return 1;
}

//-----------------------INCOMPLETE
//find pixel energy
//e(i,j) = |v(i,j)-v(i-1,j)|+ |v(i,j)-v(i+1,j)|+ |v(i,j)-v(i,j-1)|+ |v(i,j)-v(i,j+1)|,
// v(i,j) = pixel value at (i,j)
void set_energy(Pixel_Array& pix_val, Pixel_Array& pix_energy)
{
	for(int j = 0; j < pix_val.get_max_row(); ++j)
	{
		for(int i = 0; i < pix_val.get_max_col(); ++i)
		{
			pix_energy.set_element(i, j, calc_energy(pix_val, i, j));
			cout << pix_energy.get_element(i, j) << "\t";
		}
		cout << "\n\n";
	}
}

//-----------------------INCOMPLETE
//find vert seam
void find_seam_vert(Pixel_Array& pix_energy)
{
	


}

//-----------------------INCOMPLETE
//find horiz seam
void find_seam_horiz(Pixel_Array& pix_energy)
{



}

//-----------------------INCOMPLETE
//removes located low energy seam from image, horizontal or vertical
void remove_seam()
{

}

//-----------------------INCOMPLETE
//the menu. don't want this cluttering up main...
void menu()
{

}

//-----------------------INCOMPLETE
int main(int argc, char* argv[])
{
	// if(argc >= 4)

	string file_name = (string)(const char *)argv[1];
	int h_seams = *argv[2];
	int v_seams = *argv[3];

	Pixel_Array image(10, 10);
	Pixel_Array energy(10, 10);
	int total_elements = 10*10;

	srand((unsigned)time(NULL));

	//file_load(file_name + ".pgm", image, total_elements);
	
	//populate image with random pixel values
	for(int j = 0; j < image.get_max_row(); ++j)
	{
		for(int i = 0; i < image.get_max_col(); ++i)
		{
			image.set_element(i, j, rand() % 255);
			cout << image.get_element(i, j) << "\t";
		}
		cout << "\n\n";
	}
	
	//add some spacing between value and energy display
	cout << "\n\n";

	set_energy(image, energy);

	//hold command window open
	int wait;
	cin >> wait;

	return 0;
}

