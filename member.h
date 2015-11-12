#include <opencv2/core/core.hpp> // Mat, Point, Scalar
#include <vector>
using namespace cv;
using namespace std;

class member {
	double score;
	vector<polygon> Polygons;
	
  public:
  	member(vector<polygon> Polygons_in) : Polygons(Polygons_in), score(0) {}
  	
  	//set the score
  	void set_score(double score_in) {score = score_in;}
  	//set polygon
  	void set_polygon(polygon P, int i) {Polygons[i] = P;}
	
	//get the score
	double get_score() const {return score;}
	//get the ith polygon
	polygon& get_polygon(int i) {return Polygons[i];}
	
	//swap two polygons
	void swapPolygons(int i) {
		polygon copy = Polygons[i];
		if (i > 0) {
			Polygons[i] = Polygons[i - 1];
			Polygons[i - 1] = copy;
		}
	}
};
