
#define VARIABLE
#define FUNCTION


typedef struct{UINT x, UINT y}Pos, Size;


class Wnd;

struct SubWndInfo{
	Wnd* wnd;
	Pos pos;  // Left top coordinate of sub window
	Style style; // show, hide, 
}

class Wnd{
	
///////////////////////////////////////////////////////////////
// For subwindows
private VARIABLE:
	// A list of subwindows, backmost at front, topmost at rear.
	List<SubWndInfo> subWnds;
	
private FUNCTION:
	// Create a new subwindow and put it at front level
	bool CreateSubWnd(Pos pos = 0, Size size = 0, Style style = STYLE_DEFAULT){
		// If pos and size are both 0, then automatically find a place.
		// Or use commands 
		FindAvailablePlace(&pos, &size);
		
		SubWndInfo subWnd;
		subWnd.wnd = new Wnd(size);  //
		subWnd.pos = pos;
		subWnd.style = style;
		
		if({PutFront(style)){
			subWnds.push_back(subWnd);
			Draw(subWnd);
		}
		
	}
	
	// Insert a created subwindow
	bool InsertSubWnd(Wnd* wnd){
		
	}
	
	// Destroy a subwindow
	bool DestroySubWnd(Wnd* wnd){
		
	}


///////////////////////////////////////////////////////////////
// For displaying
private VARIABLE:
	Bitmap background;
	
	Size size;
	Bitmap display;

private FUNCTION:
	void Show(Wnd* subWnd = NULL){  // The subwindow to be lift up
		
	}

///////////////////////////////////////////////////////////////
// For message handlers
private FUNCTION:
	void OnClick(){
		
	}
	
	void OnDestroy(){
		// Delete all subwindows
	}
	
public FUNCTION:
	MsgProc(UINT msg){
		
	}
}