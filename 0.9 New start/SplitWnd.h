

union SplitNode{		
		struct{
			SplitNode* firstNode, *secondNode;
			uint firstSize, secondSize, lineSize;
			Line* LineStyle;
			enum{horizontal, vertical, wnd}SplitMode;
		};
		Wnd* wnd;
};