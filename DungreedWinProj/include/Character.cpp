#include "Character.h"

void Character::ForceGravity(const Dungeon* dungeon)	// 캐릭터 상태(LANDING, DOWN, DOWNJUMP, UP)에 따른 움직임
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });

	dungeon->dungeon_terrain_image->Draw(dc_set.buf_dc, dc_set.bit_rect);

	// 캐릭터 발 위치가 허공일 경우 DOWN상태로 바꾸기
	if ((state == State::LANDING || state == State::DOWNJUMP) && MapPixelCollision(dc_set.buf_dc, RGB(255, 0, 255), POINT{ pos.x + width / 2, pos.y + height })) {
		state = State::DOWN;
	}
	// 낙하 중 땅 밟게 되면 LANDING 상태로 바꾸기
	else if (state == State::DOWN && (MapPixelCollision(dc_set.buf_dc, RGB(255, 0, 0), POINT{ pos.x + width / 2, pos.y + height }) || MapPixelCollision(dc_set.buf_dc, RGB(0, 255, 0), POINT{ pos.x + width / 2, pos.y + height }))) {
		state = State::LANDING;
		jump_power = 0;
	}

	if (state == State::LANDING) {	// 오르막 or DOWN에서 LANDING으로 바뀌었을 시 캐릭터가 땅 바로 위에 서게 하려 만듦
		while (MapPixelCollision(dc_set.buf_dc, RGB(255, 0, 0), POINT{ pos.x + width / 2, pos.y + height - 1 })
			|| MapPixelCollision(dc_set.buf_dc, RGB(0, 255, 0), POINT{ pos.x + width / 2, pos.y + height - 1 })) {
			MovePos(dungeon, Direction::UP, 1);
		}
	}

	if (state == State::UP) {	// JUMP에서 올라가고 있는 상태 jump_power가 0이 되면 상태가 DOWN으로 바뀐다
		MovePos(dungeon, Direction::UP, jump_power);
		// jump_power를 나누는 분모를 키우면 중력을 덜 받고 튀어오르는 느낌이 강해진다.
		// 자연스러운 움직임을 만드려면 두 분모의 곱은 최대한 유지한다. ex) 50.0f : 2000.0f, 250.0f : 4000.0f
		jump_power -= jump_power / 50.0f + dungeon->camera_y_half_range / 2000.0f;
		if (jump_power <= 0) {
			state = State::DOWN;
		}
	}
	else if (state == State::DOWN || state == State::DOWNJUMP) {
		MovePos(dungeon, Direction::DOWN, jump_power);
		if (jump_power < dungeon->camera_y_half_range / 20.0f) {
			// jump_power를 나누는 분모를 키우면 중력이 약한 느낌이 강해진다.
			// 자연스러운 움직임을 위해선 후자의 분모를 500~1500 사이로 설정한다. 후자는 떨어지게는 하기 위한 최소한의 수치다.
			// 이 값들을 중력 관련 변수로 설정하면 맵 마다 다른 중력 구현이 가능해 수중맵도 구현이 가능할 것이다.
			jump_power += jump_power / 500.0f + dungeon->camera_y_half_range / 1500.0f;
		}
	}
}

void Character::MovePos(const Dungeon* dungeon, Direction direction, const int px)
{
	switch (direction) {
	case Direction::LEFT:
		pos.x -= px;
		break;
	case Direction::UP:
		pos.y -= px;
		break;
	case Direction::RIGHT:
		pos.x += px;
		break;
	case Direction::DOWN:
		pos.y += px;
		break;
	}
}

bool Character::MapPixelCollision(HDC terrain_dc, const COLORREF& val, const POINT& pt)	// 지형 표시 이미지를 사용해 충돌 확인, 오류 있음
{
	if (pt.x < client.left || pt.y > client.right)
		return false;
	if (pt.y < client.top || pt.y > client.bottom)
		return false;

	if (GetPixel(terrain_dc, pt.x, pt.y) == val)
		return true;
	else
		return false;
}

void Character::Render(HDC scene_dc, const RECT& bit_rect) const
{
	Image image(L"player - dungreed\\CharIdle0-resources.assets-2445.png");
	int image_width = image.GetWidth();
	int image_height = image.GetHeight();
	if (looking_direction) {
		image.Draw(scene_dc, pos.x, pos.y, width, height, 0, 0, image_width, image_height);
	}
	else {
		FlipImage(scene_dc, bit_rect, &image, pos.x, pos.y, width, height);
	}
}

void Character::Look(const POINT& target)
{
	if (pos.x < target.x)
		looking_direction = TRUE;
	else
		looking_direction = FALSE;
}