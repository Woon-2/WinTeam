#include "Player.h"

void Player::KeyMove(HDC h_dc, const TCHAR* map_name)
{
	POINT pos = GetPos();
	int state = GetState();
	int jump_power;
	int width = GetWidth();
	int height = GetHeight();

	if (GetAsyncKeyState('A') & 0x8000) {
		if (!MapPixelCollision(map_name, RGB(255, 0, 0), pos.x - 10, pos.y, h_dc)) {
			pos.x -= 10;
		}
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		if (!MapPixelCollision(map_name, RGB(255, 0, 0), pos.x + width + 10, pos.y, h_dc)) {
			pos.x += 10;
		}
	}
	
	if ((GetAsyncKeyState('S') & 0x8000) && (GetAsyncKeyState(VK_SPACE) & 0x8000)) {
		if (MapPixelCollision(map_name, RGB(0, 255, 0), pos.x, pos.y + height, h_dc)) {
			state = DOWNJUMP;
		}
	}
	else if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		if (state == LANDING) {
			state = UP;
			jump_power = 25;
			SetJumpPower(jump_power);
		}
	}

	SetPos(pos);
	SetState(state);
}