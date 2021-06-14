#include "Character.h"

void Character::ForceGravity(const Dungeon* dungeon)	// 캐릭터 상태(LANDING, DOWN, DOWNJUMP, UP)에 따른 움직임
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });

	dungeon->dungeon_terrain_image.Draw(dc_set.buf_dc, dc_set.bit_rect);

	// 캐릭터 발 위치가 허공일 경우 DOWN상태로 바꾸기
	if ((state == State::STANDING || state == State::MOVING || state == State::DOWNJUMP) && MapPixelCollision(dc_set.buf_dc, RGB(255, 0, 255), POINT{ pos.x + width / 2, pos.y + height })) {
		state = State::DOWN;
		MovePos(Direction::DOWN, 5);
	}
	// 낙하 중 땅 밟게 되면 STANDING 상태로 바꾸기
	else if (state == State::DOWN && (MapPixelCollision(dc_set.buf_dc, RGB(255, 0, 0), POINT{ pos.x + width / 2, pos.y + height }) || MapPixelCollision(dc_set.buf_dc, RGB(0, 255, 0), POINT{ pos.x + width / 2, pos.y + height }))) {
		state = State::STANDING;
		jump_power = 0;
	}

	if (state == State::STANDING || state == State::MOVING) {	// 오르막 or DOWN에서 착지했을 시 캐릭터가 땅 바로 위에 서게 하려 만듦
		while (MapPixelCollision(dc_set.buf_dc, RGB(255, 0, 0), POINT{ pos.x + width / 2, pos.y + height - 1 })
			|| MapPixelCollision(dc_set.buf_dc, RGB(0, 255, 0), POINT{ pos.x + width / 2, pos.y + height - 1 })) {
			MovePos(Direction::UP, 1);
		}
	}

	if (state == State::UP) {	// JUMP에서 올라가고 있는 상태 jump_power가 0이 되면 상태가 DOWN으로 바뀐다
		MovePos(Direction::UP, jump_power);
		// jump_power를 나누는 분모를 키우면 중력을 덜 받고 튀어오르는 느낌이 강해진다.
		// 자연스러운 움직임을 만드려면 두 분모의 곱은 최대한 유지한다. ex) 50.0f : 2000.0f, 25.0f : 4000.0f
		jump_power -= jump_power / 25.0f + dungeon->camera_y_half_range / 4000.0f;
		if (jump_power <= 0) {
			state = State::DOWN;
		}
	}
	else if (state == State::DOWN || state == State::DOWNJUMP) {
		MovePos(Direction::DOWN, jump_power);
		if (jump_power < dungeon->camera_y_half_range / 80.0f) {
			// jump_power를 나누는 분모를 키우면 중력이 약한 느낌이 강해진다.
			// 자연스러운 움직임을 위해선 후자의 분모를 500~1500 사이로 설정한다. 후자는 떨어지게는 하기 위한 최소한의 수치다.
			// 이 값들을 중력 관련 변수로 설정하면 맵 마다 다른 중력 구현이 가능해 수중맵도 구현이 가능할 것이다.
			jump_power += jump_power / 500.0f + dungeon->camera_y_half_range / 1000.0f;
		}
		else if (jump_power < dungeon->camera_y_half_range / 40.0f) {
			jump_power += jump_power / 50.0f;
		}
	}
}

void Character::Stand()
{
	state = State::STANDING;
}

void Character::RunLeft()
{
	if (state == State::STANDING)
		state = State::MOVING;

	MovePos(Direction::LEFT, x_move_px);
}

void Character::RunRight()
{
	if (state == State::STANDING)
		state = State::MOVING;

	MovePos(Direction::RIGHT, x_move_px);
}

void Character::DownJump()
{
	state = State::DOWNJUMP;
}

void Character::Jump()
{
	state = State::UP;
	// 분모가 커지면 점프력이 약해진다.
	jump_power = jump_start_power;
}

bool Character::CanGoLeft(const HDC terrain_dc)
{
	if (CanGoToPos(terrain_dc, POINT{ pos.x - x_move_px, pos.y + height / 2 }))
		return true;
	return false;
}

bool Character::CanGoRight(const HDC terrain_dc)
{
	if (CanGoToPos(terrain_dc, POINT{ pos.x + width + x_move_px, pos.y + height / 2 }))
		return true;
	return false;
}

bool Character::CanDownJump(const HDC terrain_dc)
{
	if (CanGoToPos(terrain_dc, POINT{ pos.x,  pos.y + height }))
		return true;
	return false;
}

bool Character::CanJump(const State state)
{
	if (state == State::STANDING || state == State::MOVING)
		return true;
	return false;
}

bool Character::IsOut_Left(const Dungeon* dungeon) const
{
	if (pos.x < 0)
		return true;
	else
		return false;
}

bool Character::IsOut_Right(const Dungeon* dungeon) const
{
	if (pos.x + width > dungeon->dungeon_width)
		return true;
	else
		return false;
}

void Character::NoOut(const Dungeon* dungeon)
{
	if (IsOut_Right(dungeon))
		while (pos.x + width > dungeon->dungeon_width)
			pos.x -= 2;
	else if (IsOut_Left(dungeon))
		while (pos.x < 0)
			pos.x += 2;
}

void Character::MovePos(Direction direction, const int px)
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

void Character::Render(HDC scene_dc, const RECT& bit_rect) const
{
	if (looking_direction) {
		image.Draw(scene_dc, pos.x, pos.y, width, height, 0, 0, image.GetWidth(), image.GetHeight());
	}
	else {
		FlipImage(scene_dc, bit_rect, image, pos.x, pos.y, width, height);
	}
}

void Character::Look(const POINT& target)
{
	if (pos.x < target.x)
		looking_direction = TRUE;
	else
		looking_direction = FALSE;
}

void Character::Look(const Character& target)
{
	if (pos.x < target.pos.x)
		looking_direction = TRUE;
	else
		looking_direction = FALSE;
}

void Character::UpdateAnimation(AnimationManager* animation_manager)
{
	if (old_animation_name != cur_animation_name) {
		animation.LoadAnimation(animation_manager, cur_animation_name);
		old_animation_name = cur_animation_name;
	}

	animation.Play();
	animation.Update();
	image = animation.GetImage(animation_manager);
}