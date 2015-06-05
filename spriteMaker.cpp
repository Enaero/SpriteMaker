#include <CImg.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

using namespace cimg_library;

int main (int argc, char** argv)
{
	if (argc == 2)
	{
		cout << "[directory]: where the files are found.\n"
			 << "[source file]: text file containing the names of all the source images.\n"
			 << "\tThe file format should look like this:\n\t[number of rows] [number of columns] [list of images].\n"
			 << "[destination name]: the name of the output image.\n";
		return 2;
	}

	else if (argc != 4)
	{
		cerr << "Usage: " << argv[0] << " [directory] [source file] [destination name]";
		cerr << endl << "Use: " << argv[0] << " --help for more details." << endl;
		return 1;
	}

	string dir_source = string(argv[1]) + string("/") + string(argv[2]);
	ifstream sources ( dir_source.c_str() );

	unsigned int num_rows;
	unsigned int num_columns;

	if (!sources.is_open())
	{
		cerr << "Could not open source file.\n";
		return -2;
	}
	sources >> num_rows >> num_columns;

	vector<string> files;
	string temp;

	while (sources >> temp)
		files.push_back (temp);

	sources.close();

	if ( !files.size() )
	{
		cerr << "Source file is empty.\n";
		return -3;
	}

	for (int i = 0; i < files.size(); ++i)
		files[i] = string(argv[1]) + string("/") + files[i];

	CImg<unsigned char> source;
	CImg<unsigned char> dest;

	source.assign (files[0].c_str());
	dest.assign (source.width()*num_columns, source.height()*num_rows, source.depth(), source.spectrum(), 255);

	for (int row = 0; row < num_rows; ++row)
		for (int frame = 0; frame < num_columns; ++frame)
		{
			if ( !(source.depth() && dest.depth() ) )
			{
				cerr << files[frame + num_columns*row] << " could not be loaded.";
				return -4;
			}
			source.assign (files[frame + num_columns*row].c_str());

			dest.draw_image (source.width()*frame, source.height()*row, source);
		}

	dest.save (argv[3]);

	return 0;
}
