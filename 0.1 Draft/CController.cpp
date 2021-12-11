



class CController{
	
	// turns the controller on
	virtual void Begin();
	
	// Return value positive indicating input too much,
	//   and negetive means too little.
	virtual int Test(); 
	virtual void Change(Param& param);  // may combine the two functions
	
}


// A controller to reach a steady speed for video player, etc
// Frame rate(fps) is set at the beginning such as 24 frames.
class CSpeedController : public CController{
	const UINT fps;
	UINT start_time;
	
public:
	CSpeedController(UINT FPS = 24): fps(FPS){}

	void Begin(){
		start_time = GetTick();
	}
	
	int Test(UINT frames_played){
		UINT curr_time = GetTick();
		
	}
	
	// returns time(ms) to sleep
	void Change(Param& param){
		
	}
}