
void Character::Init()
{
	// 테스트용
	pos = { 60, 500 };
	state = DOWN;
	jump_power = 0;
	width = 60;
	height = 60;
}

void Character::Move(HDC h_dc, const TCHAR* map_name)	// 캐릭터 상태(LANDING, DOWN, DOWNJUMP, UP)에 따른 움직임
{
	// 캐릭터 발 위치가 허공일 경우 DOWN상태로 바꾸기
	if ((state == LANDING || state == DOWNJUMP) && MapPixelCollision(map_name, RGB(255, 0, 255), pos.x + width / 2, pos.y + height, h_dc)) {
		state = DOWN;
		if (MapPixelCollision(map_name, !RGB(255, 0, 255), pos.x + width / 2, pos.y + height + 5, h_dc)) {	// 내리막길일 경우
			state = LANDING;
			pos.y += 5;
		}
	}
	// 낙하 중 땅 밟게 되면 LANDING 상태로 바꾸기
	else if (state == DOWN && (MapPixelCollision(map_name, RGB(255, 0, 0), pos.x + width / 2, pos.y + height, h_dc)) || MapPixelCollision(L"map_png\\map1-지형.png", RGB(0, 255, 0), pos.x + width / 2, pos.y + height, h_dc)) {
		state = LANDING;
	}

	if (state == LANDING) {	// 오르막 or DOWN에서 LANDING으로 바뀌었을 시 캐릭터가 땅 바로 위에 서게 하려 만듦 but 부자연스러움 
		while (!MapPixelCollision(map_name, RGB(255, 0, 255), pos.x + width / 2, pos.y + height - 1, h_dc)) {
			pos.y--;
		}
	}

	if (state == UP) {	// JUMP에서 올라가고 있는 상태 jump_power가 0이 되면 상태가 DOWN으로 바뀐다
		pos.y -= jump_power;
		jump_power -= 2;
		if (jump_power <= 0) {
			state = DOWN;
		}
	}
	else if (state == DOWN || state == DOWNJUMP) {
		pos.y += jump_power;
		if (jump_power < 10) {
			jump_power += 2;
		}
	}

}

BOOL Character::MapPixelCollision(const TCHAR* map_name, COLORREF color, int x, int y, HDC h_dc)	// 지형 표시 이미지를 사용해 충돌 확인, 오류 있음
{
	Image map_collision;
	map_collision.Load(map_name);

	int map_width = map_collision.GetWidth();
	int map_height = map_collision.GetHeight();

	HDC map_dc = CreateCompatibleDC(h_dc);
	HBITMAP h_bit = CreateCompatibleBitmap(h_dc, 1280, 800);
	HBITMAP old_bit = (HBITMAP)SelectObject(map_dc, h_bit);

	map_collision.Draw(map_dc, 0, 0, 1280, 800, 0, 0, map_width, map_height);

	COLORREF c = GetPixel(map_dc, x, y);

	map_collision.Destroy();
	SelectObject(h_dc, old_bit);
	DeleteObject(h_bit);
	DeleteDC(map_dc);

	if (color == c) { return TRUE; }
	return FALSE;
}

void Character::Render(HDC& buf_m_dc)
{
	Image image;
	image.Load(L"player - dungreed\\CharIdle0-resources.assets-2445.png");	// 테스트용
	int image_width = image.GetWidth();
	int image_height = image.GetHeight();
	image.Draw(buf_m_dc, pos.x, pos.y, width, height, 0, 0, image_width, image_height);
	image.Destroy();
}

void Character::Update()
{

}
