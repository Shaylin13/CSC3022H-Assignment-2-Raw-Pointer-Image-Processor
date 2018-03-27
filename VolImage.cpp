// VolImage.cpp - implementation file for database() functions

#include "VolImage.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
// for testing
#include <vector>
#include <cmath>

using namespace PDYSHA009;
using namespace std;

namespace PDYSHA009 {

	// default constructor
	VolImage::VolImage() 
	{
		VolImage::width = 0;
		VolImage::height = 0;
		std::vector<unsigned char**> slices;
		slices.resize(0);
	}

	// destructor
	VolImage::~VolImage() 
	{
	    for(int i=0; i< VolImage::slices.size();i++)
	    {
            for(int j =0; j<VolImage::height;j++)
            {
                delete [] VolImage::slices[i][j];//delete the inner arrays
            }
            delete [] VolImage::slices[i];//delete the "rows" outer arrays
        }
       
	}

	// populate the object with images in stack and
	// set member variables
	bool VolImage::readImages(string baseName) {
		int numberOfImages;
		vector<string> tokens;
		string word;

		string fileName =  "raws/"+baseName+".data";
		ifstream file(fileName);

		if (!file) {
			return false;
		}

		while (getline(file, word, ' ')) {
			tokens.push_back(word);
		}

		VolImage::width = atoi(tokens[0].c_str());
		VolImage::height = atoi(tokens[1].c_str());

		numberOfImages = atoi(tokens.back().c_str());

		VolImage::slices.resize(numberOfImages);//not neccesary just for assurance, if not used use push_back instead of slices[i]

		for (int i = 0; i < numberOfImages; i++) //loop repeats for each slice
		{

			string rawFileName = baseName;//"MRI";
			rawFileName = rawFileName + to_string(i) + ".raw";

            string rawtemp = /*baseName*/ + "raws/" + rawFileName;

			ifstream in;
			in.open(rawtemp, ios::binary);
			
			//populate 3D array will char pixel values------------------------
			unsigned char ** rows = new unsigned char * [VolImage::height];
			for(int j=0; j<VolImage::height;j++)
			{
			    rows[j]= new unsigned char[VolImage::width];
			    for(int k =0; k < VolImage::width; k++)
			    {
			        rows[j][k] = (unsigned char)(in.get());
			    }
			}
			//end of array population------------------------------------------
			VolImage::slices[i]=rows;//add 2D array to the vector, slices
			
		}
		//Print Number of images and bytes needed as required
		cout << "\nNumber of images: " <<numberOfImages << endl;
		
		cout << "Bytes needed: " << VolImage::volImageSize()*numberOfImages << "\n" <<endl;
		
		return true;
	}
	
	

	// compute difference map and write out to 2D char array
	void VolImage::diffmap(int sliceI, int sliceJ, string output_prefix) {
	        cout << "diffmap() Initiated.." << endl;//indicate to console diffmap() method called
	        ofstream out;
	        out.open(output_prefix+".raw");
	        
            unsigned char ** rows = new unsigned char * [VolImage::height];
			for(int j=0; j<VolImage::height;j++)
			{
			    rows[j]= new unsigned char[VolImage::width];
			    for(int k =0; k < VolImage::width; k++)
			    {
			        rows[j][k] = (unsigned char)(abs((float)VolImage::slices[sliceI][j][k] - (float)VolImage::slices[sliceJ][j][k])/2);
			        out << rows[j][k];//write to file while populating array

			    }
			    
			}
			out.close();//close to prevent memory leaks
			cout << (string)output_prefix<<".raw created"<<endl;
			
			//although not neccessary to create an array it was good for practice
			//cleaning up memory to prevent leaks
			for(int i=0;i<VolImage::height;i++)
			{
                delete [] rows[i];
            }
            delete [] rows;
	}

	
	
	// extract slice sliceId and write to output
	void VolImage::extract(int sliceId, string output_prefix) {
        cout << "extract() Initiated.." << endl;
        ofstream out;//create the header file
        out.open(output_prefix+".dat");
        out << VolImage::width << " " << VolImage::height << " 1";
        out.close();//to prevent memory leaks
        cout << (string)output_prefix<<".dat created"<<endl;
        
        //create the .raw file
        out.open(output_prefix+".raw");
        for(int j = 0; j<VolImage::height;j++)
		{
			    for(int k =0; k<VolImage::width;k++)
			    {
			        out<<VolImage::slices[sliceId][j][k];
			    }
		}
		out.close();//close to prevent memory leaks
		cout << (string)output_prefix<<".raw created"<<endl;
		
	}

	// number of bytes used to store image data bytes
	// and pointers (ignore vector<> container, dims etc)
	int VolImage::volImageSize(void) {
	    //(below) 4 bytes for each array as they are "pointer" and 1 byte for each char.
		int amountBytes = ((VolImage::height*VolImage::width)+(VolImage::height*4)+4);
		return amountBytes;
	}

	int VolImage::getWidth() {
		return VolImage::width;
	}

};

int main(int argc, char* argv[]) {

	string baseName = argv[1];

	VolImage vol1;

	if (!vol1.VolImage::readImages(baseName)) {
		cout << "Couldn't open file" << endl;
	}
	//if parameters for diffmap are entered
	if(argc == 6)
	{
	    int par1 = atoi(argv[3]);
	    int par2 = atoi(argv[4]);
	    string outputName = argv[5];
	   
	    vol1.VolImage::diffmap(par1,par2,outputName);
	}
	//if parameters for extract are entered
	else if(argc == 5)
	{
	    int par1 = atoi(argv[3]);
	    string outputName = argv[4];
	    vol1.VolImage::extract(par1,outputName);
	}
	

	return 0;
}


