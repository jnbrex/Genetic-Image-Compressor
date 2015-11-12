#include <opencv2/core/core.hpp> // Mat, Point, Scalar
#include <random>
#include <vector>
using namespace cv;
using namespace std;

class polygon {
	//random
	default_random_engine randEngine;
	
	//color
	Scalar color;
	uniform_int_distribution<int> BGRgen;
	uniform_real_distribution<double> alphaGen;
	
	//points
	int vertexCount;
	vector<Point> polyArr;
	uniform_int_distribution<int> xValGen;
	uniform_int_distribution<int> yValGen;
	
  public:
  	////////////////////////////////////constructor/////////////////////////////////////////////
  	////////////////////////////////////////////////////////////////////////////////////////////
  	polygon(uniform_int_distribution<int> BGRgen_in, uniform_real_distribution<double> alphaGen_in, uniform_int_distribution<int> xValGen_in, uniform_int_distribution<int> yValGen_in) : BGRgen(BGRgen_in), alphaGen(alphaGen_in), xValGen(xValGen_in), yValGen(yValGen_in) {}
  	
  	////////////////////////////initial polygon generation//////////////////////////////////////
  	////////////////////////////////////////////////////////////////////////////////////////////
  	//function to generate random color and alpha values
	void genRandColor(int seed) {
		randEngine.seed(seed);
		color = Scalar(BGRgen(randEngine), BGRgen(randEngine),
						BGRgen(randEngine), alphaGen(randEngine));
	}
	
	//function to generate random vertices
	void genRandPoints(int seed) {
		randEngine.seed(seed);
		vertexCount = 3; //number of vertices for the polygon
		for (int i = 0; i < vertexCount; i++)
			//Initialize each point randomly
			polyArr.push_back(Point(xValGen(randEngine), yValGen(randEngine)));
	}
	
	////////////////////////////////////mutations////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	//color
	//mutates the color of polygon
	void mutateColor(int random) {
			int r = random % 10;
			if (r < 1)
				color.val[0] += 5;
			else if (r >= 1 && r < 2)
				color.val[0] -= 5;
			else if (r >= 2 && r < 3)
				color.val[1] += 5;
			else if (r >= 3 && r < 4)
				color.val[1] -= 5;
			else if (r >= 4 && r < 5)
				color.val[2] += 5;
			else if (r >= 5 && r < 6)
				color.val[2] -= 5;
			else if (r >= 6 && r < 8)
				color.val[3] += 0.02;
			else if (r >= 8 && r < 10)
				color.val[3] -= 0.02;
	}
	
	//replaces this polygon with a random triangle
	void replaceRandom(int seed) {
		randEngine.seed(seed);
		vertexCount = 3; //number of vertices for the polygon
		polyArr.clear();
		for (int i = 0; i < vertexCount; i++)
			//Initialize each point randomly
			polyArr.push_back(Point(xValGen(randEngine), yValGen(randEngine)));
	}
	
	//remove a vertex
	void removeVertex(int rand) {
		if (vertexCount > 3) {
			rand = rand % vertexCount;
			vertexCount = vertexCount - 1;
			polyArr.erase(polyArr.begin() + rand);
		}
	}
	
	void addVertex(int seed) {
		if (vertexCount < 6) {
			randEngine.seed(seed);
			vertexCount = vertexCount + 1;
			polyArr.push_back(Point(xValGen(randEngine), yValGen(randEngine)));
		}
	}
	
	//getters
	//returns color of polygon
	Scalar get_color() {return color;}
	//returns number of vertices of polygon
	int get_vertexCount() {return vertexCount;}
	//returns the ith coordinate of the polygon
	Point getPoint(int i) {return polyArr[i];}
};
