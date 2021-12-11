*使用c++11新特性
*使用opengl进行GPU渲染合成
*取消多余的窗口类型，自动分层绘制
*多线程
*渲染与操作分离（渲染由单独的帧进行，操作会合并）

board改名为canvas


class Style{
	Size size;  // size = {800, 480};
	Pos pos; // {0, 0} or center ...
	short angle; // 45 degree
	
	byte opacity;  // transparent = 0; opaque = 255; 
	
}

class TextBoxStyle : public Style{
	
}