#include <Windows.h>
#include <math.h>

#define PI 3.141592654   

HBITMAP GetRotatedBitmap(HDC hdc, HBITMAP hBitmap, int source_x, int source_y, int dest_width, int dest_height, float angle, COLORREF bkColor)
{
	HDC sourceDC = CreateCompatibleDC(hdc);                                      // 회전할 비트맵 원본을 선택할 DC   
	HDC destDC = CreateCompatibleDC(hdc);                                      // 회전할 비트맵을 출력받을 DC   
	HBITMAP hbmResult = CreateCompatibleBitmap(hdc, dest_width, dest_height);       // 회전할 비트맵을 출력받을 메모리비트맵 핸들   

	HBITMAP hbmOldSource = (HBITMAP)SelectObject(sourceDC, hBitmap);                   // 회전할 비트맵 원본을 DC에 선택   
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(destDC, hbmResult);                   // 회전할 비트맵을 출력받을 DC에, 회전할 비트맵을 출력받을 메모리비트맵 선택   

	HBRUSH hbrBack = CreateSolidBrush(bkColor);                                  // 회전으로 인한, 공백영역을 칠할 브러시핸들 생성   
	HBRUSH hbrOld = (HBRUSH)SelectObject(destDC, hbrBack);                      // 브러시핸들 선택   
	PatBlt(destDC, 0, 0, dest_width, dest_height, PATCOPY);                           // 선택된 브러시로, 회전할 비트맵을 출력받을 DC에, 미리 색상을 채워 지움   
	DeleteObject(SelectObject(destDC, hbrOld));                                     // 브러시 해제   

	angle = (float)(angle * PI / 180);                                      // 회전할 각도(360분법)를, 라디안단위의 각도로 변경   
	float cosine = (float)cos(angle);                                              // 회전이동변환 행렬에 들어갈 cos세타 값을 구함          
	float sine = (float)sin(angle);                                              // 회전이동변환 행렬에 들어갈 sin세타 값을 구함   
	SetGraphicsMode(destDC, GM_ADVANCED);                                               // 윈도우 좌표계의 회전을 위하여, 그래픽모드를 확장모드로 변경



	XFORM xform;                                                                        // 방정식을 표현하는 3행3열의 행렬 선언   
	xform.eM11 = cosine;                                                                // 1행 1열 성분 설정 (회전성분)   
	xform.eM12 = sine;                                                                  // 1행 2열 성분 설정 (회전성분)   
	xform.eM21 = -sine;                                                                 // 2행 1열 성분 설정 (회전성분)   
	xform.eM22 = cosine;                                                                // 2행 2열 성분 설정 (회전성분)   
	xform.eDx = (FLOAT)dest_width / 2.0f;                                               // 3행 1열 성분 설정 (X축 이동 성분)   
	xform.eDy = (FLOAT)dest_height / 2.0f;                                              // 3행 2열 성분 설정 (Y축 이동 성분)   
																						// 설정한 행렬로, 인수DC의 윈도우즈 좌표계를 변환.   
																						// 인수DC는 출력대상으로 메모리비트맵을 가지므로,
																						// 선택하고 있는 메모리비트맵의 좌표체계가 회전된다.   
	SetWorldTransform(destDC, &xform);
	// 회전된 메모리DC에, 회전할 이미지를 출력   
	BitBlt(destDC, -(dest_width / 2), -(dest_height / 2), dest_width, dest_height, sourceDC, source_x, source_y, SRCCOPY);
	// 사용된 자원 해제   
	SelectObject(sourceDC, hbmOldSource);
	SelectObject(destDC, hbmOldDest);
	DeleteObject(sourceDC);
	DeleteObject(destDC);
	// 회전된 비트맵 핸들 리턴   
	return hbmResult;
}



void RotateBlt(HDC hdc, HDC hOffScreen, int dest_x, int dest_y, int dest_width, int dest_height,
	HBITMAP hBit, int source_x, int source_y, DWORD dFlag, float angle, COLORREF bkColor)
{
	static HDC hTempMemDC;              // 회전한 비트맵을 선택할 메모리DC   
	HBITMAP hTempBitmap, hOldBitmap;    // 회전한 비트맵을 저장할, 비트맵 핸들   
										// 최초 한번만 비트맵DC를 생성합니다.   
	if (!hTempMemDC) hTempMemDC = CreateCompatibleDC(hdc);
	// 비트맵을 회전하는 함수를 호출하여, 리턴되는 회전된 비트맵 핸들을 저장   
	hTempBitmap = GetRotatedBitmap(hdc, hBit, source_x, source_y, dest_width, dest_height, angle, bkColor);
	// 회전된 비트맵 핸들을 선택하고,
	hOldBitmap = (HBITMAP)SelectObject(hTempMemDC, hTempBitmap);
	// 오프스크린DC에, 회전된 비트맵 핸들을 선택한, 메모리DC의 이미지를 출력합니다.   
	// 안의 주석을 해제하면, 출력좌표를 중심으로 하는 회전이미지가 됩니다.   
	TransparentBlt(hOffScreen, dest_x/* - dest_width/2*/, dest_y/* - dest_height/2*/, dest_width, dest_height, hTempMemDC, 0, 0, dest_width, dest_height, RGB(255, 255, 255));
	SelectObject(hTempMemDC, hOldBitmap);
	DeleteObject(hTempBitmap);
}