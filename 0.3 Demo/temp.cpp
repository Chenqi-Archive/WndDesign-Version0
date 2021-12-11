#include <Windows.h>

#undef TMP

#ifdef TMP
BOOL SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpszFileName)
{
	HDC hDC; //设备描述表 

	//定义调色板大小， 位图中像素字节大小 ， 位图文件大小 ， 写入文件字节数 
	DWORD dwPaletteSize = 0, dwBmBitsSize, dwDIBSize, dwWritten;

	BITMAP Bitmap; //位图属性结构 
	BITMAPFILEHEADER bmfHdr; //位图文件头结构 
	BITMAPINFOHEADER bi;//位图信息头结构 
	LPBITMAPINFOHEADER lpbi; //指向位图信息头结构 

	HANDLE fh, hDib, hPal;
	HPALETTE hOldPal = NULL;//定义文件，分配内存句柄，调色板句柄

	//设置位图信息头结构 
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = Bitmap.bmBitsPixel;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	//计算调色板大小 
	if (Bitmap.bmBitsPixel <= 8) 
		dwPaletteSize = (1 << Bitmap.bmBitsPixel) * sizeof(RGBQUAD);
	dwBmBitsSize = ((Bitmap.bmWidth * Bitmap.bmBitsPixel + 31) / 32) * 4 * Bitmap.bmHeight;

	//为位图内容分配内存 
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	// 处理调色板 
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	// 获取该调色板下新的像素值 
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);
	
	//恢复调色板 
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}


	//创建位图文件 
	fh = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE) 
		return FALSE;

	// 设置位图文件头 
	bmfHdr.bfType = 0x4D42; // "BM" 
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	
	// 写入位图文件头 
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	
	// 写入位图文件其余内容 
	WriteFile(fh, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize, &dwWritten, NULL);
	
	//清除 
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return TRUE;
}

#endif