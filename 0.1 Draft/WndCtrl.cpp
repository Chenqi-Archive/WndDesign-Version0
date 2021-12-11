

// This layer seperate Wnd and System

// No need! Just create a system wnd, without displaying, just as a handler!

// Mapping between wnd and hWnd
static class map(Wnd, HWND);

static insertmap();
static deletemap();

static getVal(UINT hWnd)


// Create a new Wnd object
Wnd* NewWnd((UINT)hWnd){
	Wnd* wnd = new Wnd;
	map(hWnd, wnd);
	
}


bool DeleteWnd((UINT)hWnd){
	
}


bool sendMsg((UINT)hWnd, MSG){
	
}

