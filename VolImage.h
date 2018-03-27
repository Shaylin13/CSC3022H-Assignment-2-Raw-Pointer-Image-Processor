// database.h - header file: this should be included in any source file that needs database() functions

#ifndef VOLIMAGE_H
#define VOLIMAGE_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace PDYSHA009 {
	class VolImage {

	private: // private members

		int width, height;			// width and height of image stack
		std::vector<unsigned char**> slices;	// data for each slice, in order

	public: // public members

		VolImage();	// default constructor
		~VolImage();	// destructor

		// populate the object with images in stack and
		// set member variables
		bool readImages(std::string baseName);

		// compute difference map and write out
		void diffmap(int sliceI, int sliceJ, std::string output_prefix);

		// extract slice sliceId and write to output
		void extract(int sliceId, std::string output_prefix);

		// number of bytes used to store image data bytes
		// and pointers (ignore vector<> container, dims etc)
		int volImageSize(void);

		int getWidth();

	};
}

#endif
