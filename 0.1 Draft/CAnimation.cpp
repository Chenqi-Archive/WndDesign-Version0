
include Contorller


// Is animation be presented by a stream of frames, or by the dynamic
//   behaviour of its contents? (like moving, spinning, scaling, etc)
// 
// Or take objects' behaviour as input and frames as output, scheduling 
//   by a controller?
//
// No! Actions should be directly shown. Actions themselves should be acted
//   with precise timing.


class Animation1{
	
	// Frames need synchronization
	Bitmap frameBuffer;
	
	// Contorller to regulate play speed
	Contorller controller;
	
	// Thread1: Generate frames from (multiple) objects' actions,
	//   objects includes Animation itself
	
	
	
	// Thread2: Show these frames
	
}

class Animation2{
	
	// The space an object acts in. May be multi-dimensional.
	extern Bitmap space;
	extern other sensory information...
	
	Objects objects...
	
	
	Start(){
		all-objects.DoActionIn(space);
	}
	
}


// An object means an agent.

class Agent{
	
	
	
}



