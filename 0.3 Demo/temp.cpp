#include <Windows.h>

#undef TMP

#ifdef TMP
BOOL SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpszFileName)
{
	HDC hDC; //�豸������ 

	//�����ɫ���С�� λͼ�������ֽڴ�С �� λͼ�ļ���С �� д���ļ��ֽ��� 
	DWORD dwPaletteSize = 0, dwBmBitsSize, dwDIBSize, dwWritten;

	BITMAP Bitmap; //λͼ���Խṹ 
	BITMAPFILEHEADER bmfHdr; //λͼ�ļ�ͷ�ṹ 
	BITMAPINFOHEADER bi;//λͼ��Ϣͷ�ṹ 
	LPBITMAPINFOHEADER lpbi; //ָ��λͼ��Ϣͷ�ṹ 

	HANDLE fh, hDib, hPal;
	HPALETTE hOldPal = NULL;//�����ļ��������ڴ�������ɫ����

	//����λͼ��Ϣͷ�ṹ 
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

	//�����ɫ���С 
	if (Bitmap.bmBitsPixel <= 8) 
		dwPaletteSize = (1 << Bitmap.bmBitsPixel) * sizeof(RGBQUAD);
	dwBmBitsSize = ((Bitmap.bmWidth * Bitmap.bmBitsPixel + 31) / 32) * 4 * Bitmap.bmHeight;

	//Ϊλͼ���ݷ����ڴ� 
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	// �����ɫ�� 
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	// ��ȡ�õ�ɫ�����µ�����ֵ 
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);
	
	//�ָ���ɫ�� 
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}


	//����λͼ�ļ� 
	fh = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE) 
		return FALSE;

	// ����λͼ�ļ�ͷ 
	bmfHdr.bfType = 0x4D42; // "BM" 
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	
	// д��λͼ�ļ�ͷ 
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	
	// д��λͼ�ļ��������� 
	WriteFile(fh, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize, &dwWritten, NULL);
	
	//��� 
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return TRUE;
}

#endif