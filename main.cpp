// main
/*
Compile with:
g++ -std=c++11 `pkg-config --cflags opencv` -o GA main.cpp draw.cpp `pkg-config --libs opencv`
*/
#include "draw.h" // renderPolyImage(), score()
#include "polygon.h"
#include "member.h"
#include <opencv2/highgui/highgui.hpp> // imread(), imwrite(), imshow()
#include <iostream> // cerr, cout
#include <sstream> // ostringstream
#include <string> // string
using namespace cv;
using namespace std;

bool sorter (member A, member B) {
	return (A.get_score() > B.get_score());
}

int main(int argc, char** argv) {
	if (argc < 5) {
		cerr << "Not enough arguments. Usage: ./GA imagename.jpg N K E\n";
		return 0;
	}

	// Interpret argv[1] as an image file name,
	// read in the file using the OpenCV imread funciton
	Mat referenceImage = imread(argv[1]);
	//Interpret argv[2] as N
	int N = atoi(argv[2]);
	//Interpret argv[3] as K
	int K = atoi(argv[3]);
	//Interpret argv[4] as E
	int E = atoi(argv[4]);
	
	// Create a random generator, seed it with the value 1
	// Found in the c++ standard library <random>
	default_random_engine randEngine(1);

	// Create two uniform distributions, one for randomly generating
	// 	x-values of vertices, one for randomly generating y-values of vertices,
	//	within the bounds of the image.
	uniform_int_distribution<int> xValGen(0, referenceImage.cols-1);
	uniform_int_distribution<int> yValGen(0, referenceImage.rows-1);

	// Create distributions for generating color and opacity values
	uniform_int_distribution<int> BGRgen(0,255);
	uniform_real_distribution<double> alphaGen(0,1);
	
	//create parents and children generations
	vector <member> Parents;
	vector <member> Children;
	//vector to store images of current generation
	vector<Mat> polyImages;
	
	//generate N members of population
	for (int i = 0; i < N; i++) {
		vector<polygon> Polygons;
		//generate all 100 polygons
		for (int j = 0; j < 100; j++) {
			Polygons.push_back(polygon(BGRgen, alphaGen, xValGen, yValGen));
			//generate random color and points for the polygon
			Polygons[j].genRandColor(randEngine());
			Polygons[j].genRandPoints(randEngine());
		}
		//create a member object for the polygons
		member Member = member(Polygons);
		//add the member to the Parents
		Parents.push_back(Member);
	}
	
	







	int numChildren = 0;
	int lastNumChildren = 0;
	int numGenerations = 0;
	//number of generations
	while (E > (N + numGenerations * K)) {
		//do mutations if only one member of population
		if (N == 1) {
			Children.clear();
			//iterate through K times for K pairs of parents
			for (int i = 0; i < K; i++) {
				//add the parent to the Children generation
				Children.push_back(Parents[0]);
				numChildren++;
				//make a random number
				int r = randEngine() % 6;
				int p = randEngine() % 100;
				if (r == 0)
					Children[i].get_polygon(p).mutateColor(randEngine());
				else if (r == 1)
					Children[i].get_polygon(p).replaceRandom(randEngine());
				else if (r == 2)
					Children[i].swapPolygons(p);
				else if (r == 3)
					Children[i].get_polygon(p).removeVertex(randEngine());
				else if (r == 4)
					Children[i].get_polygon(p).addVertex(randEngine());
			}
			Children.push_back(Parents[0]);
			numChildren++;
		}
		//do mutations if N is more than 1
		else {
			Children.clear();
			int firstParent;
			int secondParent;
			//iterate through K times for K pairs of parents
			for (int i = 0; i < K; i++) {
				int p1Num = randEngine() % (4 * Parents.size());
				if (p1Num < 2 * Parents.size())
					firstParent = 0;
				else
					firstParent = 1;
				int p2Num = randEngine() % (4 * Parents.size());
				if (p2Num < 2 * Parents.size())
					secondParent = 0;
				else
					secondParent = 1;
				//choose random split point
				Children.push_back(Parents[firstParent]);
				numChildren++;
				int split = randEngine() % 100;
				for (int j = split; j < 100; j++)
					Children[i].set_polygon(Parents[secondParent].get_polygon(j), j);
				//mutation
				//make a random number
				int r = randEngine() % 6;
				int p = randEngine() % 100;
				if (r == 0)
					Children[i].get_polygon(p).mutateColor(randEngine());
				else if (r == 1)
					Children[i].get_polygon(p).replaceRandom(randEngine());
				else if (r == 2)
					Children[i].swapPolygons(p);
				else if (r == 3)
					Children[i].get_polygon(p).removeVertex(randEngine());
				else if (r == 4)
					Children[i].get_polygon(p).addVertex(randEngine());
			}
			Children.push_back(Parents[0]);
			numChildren++;
		}
				
				
			 
		
		
		polyImages.clear();
		
		//get the best score
		double highScore = 0;
		int highScoreMember = 0;
		//iterate over each member to calculate score
		for (int b = 0; b < Children.size(); b++) {
	
			///////////////////////convert from useful vector format to opencv array format
			int vertexCounts[100];
			Scalar colors[100];
			Point polyArr[100][6];
			for (int i = 0; i < 100; i++) {
				vertexCounts[i] = Children[b].get_polygon(i).get_vertexCount();
				colors[i] = Children[b].get_polygon(i).get_color();
				for (int j = 0; j < vertexCounts[i]; j++)		
					polyArr[i][j] = Children[b].get_polygon(i).getPoint(j);
			}
	
			const Point* polygons[100]; 
			//Array of constant pointers to arrays of Points
			for (int i = 0; i < 100; i++)
				// Initialize each pointer to point to a polygon
				polygons[i] = &polyArr[i][0];

			//render the image
			Mat polyImage = renderPolyImage(
				referenceImage.cols, 
				referenceImage.rows, 
				100, 
				polygons, 
				vertexCounts, 
				colors
				);
			
			polyImages.push_back(polyImage);
			/////////////////////////////////////////////////////////////////////////////////
	
	
			// Display the reference image and the rendered polygon image during runtime
			//imshow("Reference Image", referenceImage);
			//imshow("PolyImage", polyImage);
			//Another openCV function that pauses execution until the user presses the given key
			//waitKey(0);

			// Score the randomly generated image
			Children[b].set_score(score(polyImage, referenceImage));
			//cout << Children[b].get_score() << "\n";
			//get the new high score
			if (Children[b].get_score() > highScore) {
				highScore = Children[b].get_score();
				highScoreMember = b;
			}
		}
		
		if (N == 1) {
			Parents.clear();
			Parents.push_back(Children[highScoreMember]);
		}
		
		else {
			sort(Children.begin(), Children.end(), sorter);
			Parents.clear();
			if (N < Children.size()) {
				for (int i = 0; i < N; i++)
					Parents.push_back(Children[i]);
			}
			else {
				for (int i = 0; i < Children.size(); i++)
					Parents.push_back(Children[i]);
			}
		}
		
		if (numChildren - lastNumChildren > 0) {
			lastNumChildren += 1000;
			cout << "Fitness score of best individual in generation " << numGenerations << ": ";
			cout << highScore << '\n';
////////////////////////////////////////////////////////////////////////
//Uncomment the below section to output images as well as fitness values
/*

			// Create the ouput file name
			ostringstream fileOut;
			fileOut << "000000" << numGenerations << ".png";
			string filename = fileOut.str();

			// Use the OpenCV imwrite function to write the generated image to a file
			imwrite(filename, polyImages[highScoreMember]); //Extension determines write format
			cout << "Wrote image file " << filename << '\n';
			
*/
/////////////////////////////////////////////////////////////////////////
		}
			
		numGenerations++;
	}
	
	return 0;
}
