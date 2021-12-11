

struct SubWindowInfo{
	CWindow* subWindow;
	UINT off_x, off_y;
	UINT show_info;  // show, hide ,...
}


class CWindow : public CBitmap{
private:
	CBitmap background;
	CBitmap<SubWindowInfo> show_info;
	
private:
	CList<SubWindowsInfo> subWindows; // from back to front

	
	
public:
	CWindow();
	
	void Refresh(){
		background.Generate();
		for(subWindowInfo in subWindows){
			subWindowInfo.subWindow.Generate();
		}
	}
		
	void OnClick(UINT pos_x, UINT pos_y){
		SubWindowInfo subWindow = show_info[pos_x][pos_y];
		LiftAndRefresh(subWindow);
	}
	
	virtual void ReactOn(UINT msg){
		switch(msg){
			
		}
	}
}


class CButton : public CWindow{
	
	
	{
		subWindows = Empty();
	}
	
	
}





