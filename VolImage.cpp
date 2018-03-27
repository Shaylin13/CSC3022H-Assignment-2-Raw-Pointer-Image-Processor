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

	}

	// populate the object with images in stack and
	// set member variables
	bool VolImage::readImages(string baseName) {
		int numberOfImages;
		vector<string> tokens;
		string word;
		string fileName = baseName + "/MRI.data";
		ifstream file(fileName);

		if (!file) {
			return false;
		}

		while (getline(file, word, ' ')) {
			tokens.push_back(word);
			//cout << tokens.back().c_str()<<endl;
		}

		VolImage::width = atoi(tokens[0].c_str());
		VolImage::height = atoi(tokens[1].c_str());

		numberOfImages = atoi(tokens.back().c_str());
		
		cout << "width: " <<VolImage::width << endl;
		cout << "height: " <<VolImage::height << endl;
		cout << "Number of images: " <<numberOfImages << endl;

		VolImage::slices.resize(numberOfImages);//not neccesary just for assurance, if not used use push_back instead of slices[i]

		for (int i = 0; i < numberOfImages; i++) 
		{//repeats for each slice
			baseName = "brain_mri_raws";
			string rawFileName = "MRI";
			rawFileName = rawFileName + to_string(i) + ".raw";
			//cout << rawFileName << endl;//testing purposes
            string rawtemp = baseName + "/" + rawFileName;

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
			        //cout << (int)rows[j][k] << endl;//testing purposes
			    }
			}
			//end of array population------------------------------------------
			VolImage::slices[i]=rows;//add 2D array to the vector, slices
			
			/*if(i ==(numberOfImages-1))//testing purposes
			{
			    for(int j = 0; j<VolImage::height;j++)//testing
			    {
			    for(int k =0; k<VolImage::width;k++)
			    {
			        cout<<VolImage::slices[122][j][k]<<endl;
			    }
			    }
			}*/
			
			
		}
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
			    /*if(j!=VolImage::height-1)
			    {
			        out<<endl;
			    }*/
			}
			
			out.close();
			cout << (string)output_prefix<<".raw created"<<endl;
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
			    /*if(j!=VolImage::height-1)
			    {
			        out<<endl;
			    }*/
		}
		out.close();//close to prevent memory leaks
		cout << (string)output_prefix<<".raw created"<<endl;
	}

	// number of bytes used to store image data bytes
	// and pointers (ignore vector<> container, dims etc)
	int VolImage::volImageSize(void) {
		return 0;
	}

	int VolImage::getWidth() {
		return VolImage::width;
	}

};

int main(int argc, char* argv[]) {

	//cout << argc << endl;//testing
	//cout << argv[0] << endl;
	//cout << argv[1] << endl;

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
	    //cout << par1 <<endl;//for testing
	    //cout << par2 <<endl;
	    //cout << outputName <<endl;
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

