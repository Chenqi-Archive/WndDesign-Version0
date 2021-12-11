#include <CShape.h>

#include <math.h>

// x = 0, y = 0 be the center or be the rect's lt vertex?
// or be some other 
// x, y might be negetive

class CPoint : public CShape{
	virtual Generate(){
		x = 0; y = 0;
		post(x, y);
	}
}

class CLine : public CShape{
	UINT end_x;
	UINt end_y;
	
	virtual Generate(){
		double t = 1.0 / max(end_x, end_y);
		for(; t <= 1; ++t){
			x = end_x * t;
			y = end_y * t;
			post(x, y);
		}
	}
}

// Rect
//     -- x -->
// | 0 ....... w
// y . ..      .
// | .     ..  .
// v h ....... .

class CRect : public CShape{
	UINT width;
	UINT height;
	
	// Generate point at the center of the line
	// Render at the next step using the pen style
	virtual Generate(){
		UINT t; // parametric equation
		for(t = 0; t < width; ++t){
			x = t;
			y = 0;
			post(x, y);
		}
		for(t = 0; t < height; ++t){
			x = width - 1;
			y = t;
			post(x, y);
		}
		...
	}
	
}

class CArch : public CShape{
	UINT radius;
	
	CArch()
	CArch(int StartTangent{x, y})
	
	virtual Generate(){
		double t;
		double increment = arctan(1.0 / radius);
		for(t = 0; t < 2 * pi; t += increment)
		{
			x = radius * cos(t) + radius;  // place in the rect
			y = radius * sin(t) + radius;
			post(x, y);
		}
	}
}


class CCircle : public CShape{

}

class CRoundedRect : public Rect{
	UINT round_radius;
	
	CRoundedRect(UINT width, UINT height, UINT radius)
	{
		round_radius = min(radius, width / 2, height / 2);
	}
	
	virtual void Generate(){
		
	}
}

class CCharacter : public CShape{
	wchar c;
	Font font;
	
}


