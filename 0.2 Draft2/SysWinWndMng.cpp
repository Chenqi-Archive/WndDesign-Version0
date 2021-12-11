
#include <Windows.h>

// The only one windows main wnd message controller
//   without backgrund, display 
// No need to be a class!

class WinWnd : public Wnd{
	
private:
	// HWND mapping
	List<struct{HWND, CWnd*}> wndMap;
	CWnd* GetSubWnd(HWND hWnd){
		
	}
	
private:
	CreateSubWnd(){
		
		//
	}
	
public:
	static RegisterBlankWnd(){
		
	}
	
	static LPRESULT WndProc(HWND hWnd, ...){
		
	}
	
	bool Init(){
		// Create first real window
		CreateSubWnd()
	}
}