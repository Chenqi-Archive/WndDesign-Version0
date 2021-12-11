

struct WndProperty{
	WndID id;
	Bitmap wndShape;
	Pos offset;
	Style;
}


// Windows as layers are ordered on a list, from front to back
static List<WndID> wndList;



//
static bool BringToFront(wndID){
	
}


// To main thread
extern Output;
extern WndIndex;


static void Render(){
	for(wnd in reverse wndList){
		show(wnd);
		SetRegion(wnd);
	}
}




// Thread 2: Control message loop
//   May be put into another cpp file.




extern queue msgQueue; //synchronized
static bool draw_init(){
	
	queue_init();
	
	while(GetMessage(msgQueue, &msg)){
		DispatchMsg(msg);
	}
}



// Drawing init
// Create blank background
void draw_init(){
	
}

