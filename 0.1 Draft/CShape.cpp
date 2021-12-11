
struct PointInfo{
	UINT x;
	UINT y;
	UINT info;
}InvalidPoint({-1, -1, -1});

class ColorInfo{
	BYTE red;
	BYTE green;
	BYTE blue;
	BYTE transparency;  //alpha
	
	ColorInfo(const char*);
}

class CPen{
	UINT size;
	ColorInfo color;
	
	// transparent, scatter, color，shades
	Style style;
	
	public:
	CPen(UINT Size, UINT Color):size(Size), color(Color){}
	
}DefaultPen(1, "#000000");



class CShape{
private:
	 
	 
public:
	virtual BOOL Generate(PointInfo* Point) = 0;
}

class CRect : public CShape{
private:
	UINT width;
	UINT height;
	
private:
	// 3 states: idling, starting, finishing
	
	// GenerateStart;
    // GenerateNext;
	static void generate_thread(){
		for(int i = 0; i < width; ++i){
			for(int j = 0; j < height; ++j){
				if(i == 0 || j == 0 || i == width - 1 || j == height - 1){
					buffer[curr_pos++] = {i, j, color};
				}
			}
		}
	}
	
public:
	virtual BOOL Generate(PointInfo* Point, const CPen& Pen = DefaultPen){
		static PointInfo* buffer;
		static UINT curr_pos;
		// fork a new thread to fill in buffer
				
		
		if(curr_pos == PosEnd){
			return False;
		}
		Point = buffer[curr_pos];
		return True;
	}
}


class CBitmap : public CShape{
	
	
	void Draw(CPen& pen, CShape& shape){
		
	}
}

