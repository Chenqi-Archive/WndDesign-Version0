

class CBitmap : public CShape{
	UINT width;
	UINT height;
	
	CColorInfo* content;
	
	
	static Generate(){
        for(int i = 0; i < width; ++i){
			for(int j = 0; j < height; ++j){
				if(i == 0 || j == 0 || i == width - 1 || j == height - 1){
					buffer[curr_pos++] = {i, j, content[i][j]};
				}
			}
		}
	}
	
	
	void LoadFromFile(FileName){
		
		// May pack file reader in another cpp
		buffer = sys_read_file(...);
		
		// Read picture files .bmp .jpg .png .ico ...
		
	}
	
	void SaveToFile()
}