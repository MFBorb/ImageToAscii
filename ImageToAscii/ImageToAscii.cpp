#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

// Prototype functions
void attemptAsciiSave(string asciiString);


/////////////////////////////////////////////////////////////////////


// Main function

int main(int argc, char** argv)
{
	// Make sure user is inputting the appropriate amount of arguments.
	if (argc != 2)
	{
		cout << " Usage: " << argv[0] << " ImageToLoadAndDisplay" << endl;
		return -1;
	}

	// Read in the image.
	Mat img = imread(argv[1], IMREAD_COLOR);
	int numX = img.cols;
	int numY = img.rows;

	// Scale the image to fit a specific region.
	// TODO (testing without doing this)

	// Apply an edge detection filter.
	// TODO (Instead will apply a grayscale filter to each pixel and quantize it into 8 colors).
	char grayscaleChars[] = {' ', '.', ':', '-', '=', '#', '%', '@'};
	vector<vector<char>> asciiPixels;

	// For each pixel, find the corresponding grayscale value and quantize it to one of the grayscale characters above.
	for (int i = 0; i < numX; i++) {

		vector<char> row;

		for (int j = 0; j < numY; j++) {
			Vec3b intensity = img.at<Vec3b>(j, i);

			// GIMP finds grayscale by dotting the picture with the <0.21, 0.72, 0.07> vector in RGB order.
			// OpenCV stores in BGR order so we have to take the end of the intensities first.
			double grayscaleValue = (0.21 * intensity[2]) + (0.72 * intensity[1]) + (0.07 * intensity[0]);

			// Divide by 255 to get it in a scale from 0 to 1, then multiply by 8 to find which quantized value it is.
			// Assigning it to an int will effectively round it down (which we need!)
			int quantizedValue = grayscaleValue / 255.0 * 8;

			char grayscaleChar = grayscaleChars[quantizedValue];

			row.push_back(grayscaleChar);

		}

		// Append the asciiPixels with the row.
		asciiPixels.push_back(row);
	}

	string completedString = "";
	// Print each ascii letter out into a text file.
	for (int i = 0; i < numY; i += 2) {
		string completedRowString = "";

		for (int j = 0; j < numX; j += 1) {
			completedRowString += asciiPixels[j][i];
		}

		completedRowString += "\n";
		completedString += completedRowString;
	}

	cout << completedString;

	// Save text file.
	attemptAsciiSave(completedString);
	
	return 0;
}


///////////////////////////////////////////////////////////////////////


// Worker functions
void attemptAsciiSave(string asciiString) {

	// Ask user if they want to save the ascii.
	cout << "\nDo you want to save the ascii to a text file (y/n)?: ";

	// Get the user's response. If it isn't a yes or no, keep repeating
	// until they input yes or no.
	char userResponse = ' ';
	cin.get(userResponse);

	while (userResponse != 'y' && userResponse != 'n') {
		cout << "Not a valid response. Please input y or n: ";
		cin.get(userResponse);
	}

	if (userResponse == 'y') {
		ofstream asciiFile("ascii.txt", ofstream::out);

		asciiFile << asciiString;

		asciiFile.close();
	}
}