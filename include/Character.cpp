#include "Character.h"

void Character::ForceGravity(const Dungeon* dungeon)	// ĳ���� ����(LANDING, DOWN, DOWNJUMP, UP)�� ���� ������
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });

	dungeon->dungeon_terrain_image->Draw(dc_set.buf_dc, dc_set.bit_rect);

	// ĳ���� �� ��ġ�� ����� ��� DOWN���·� �ٲٱ�
	if ((state == State::STANDING || state == State::MOVING || state == State::DOWNJUMP) && MapPixelCollision(dc_set.buf_dc, RGB(255, 0, 255), POINT{ pos.x + width / 2, pos.y + height })) {
		state = State::DOWN;
		MovePos(Direction::DOWN, 5);
	}
	// ���� �� �� ��� �Ǹ� STANDING ���·� �ٲٱ�
	else if (state == State::DOWN && (MapPixelCollision(dc_set.buf_dc, RGB(255, 0, 0), POINT{ pos.x + width / 2, pos.y + height }) || MapPixelCollision(dc_set.buf_dc, RGB(0, 255, 0), POINT{ pos.x + width / 2, pos.y + height }))) {
		state = State::STANDING;
		jump_power = 0;
	}

	if (state == State::STANDING || state == State::MOVING) {	// ������ or DOWN���� �������� �� ĳ���Ͱ� �� �ٷ� ���� ���� �Ϸ� ����
		while (MapPixelCollision(dc_set.buf_dc, RGB(255, 0, 0), POINT{ pos.x + width / 2, pos.y + height - 1 })
			|| MapPixelCollision(dc_set.buf_dc, RGB(0, 255, 0), POINT{ pos.x + width / 2, pos.y + height - 1 })) {
			MovePos(Direction::UP, 1);
		}
	}

	if (state == State::UP) {	// JUMP���� �ö󰡰� �ִ� ���� jump_power�� 0�� �Ǹ� ���°� DOWN���� �ٲ��
		MovePos(Direction::UP, jump_power);
		// jump_power�� ������ �и� Ű��� �߷��� �� �ް� Ƣ������� ������ ��������.
		// �ڿ������� �������� ������� �� �и��� ���� �ִ��� �����Ѵ�. ex) 50.0f : 2000.0f, 25.0f : 4000.0f
		jump_power -= jump_power / 25.0f + dungeon->camera_y_half_range / 4000.0f;
		if (jump_power <= 0) {
			state = State::DOWN;
		}
	}
	else if (state == State::DOWN || state == State::DOWNJUMP) {
		MovePos(Direction::DOWN, jump_power);
		if (jump_power < dungeon->camera_y_half_range / 80.0f) {
			// jump_power�� ������ �и� Ű��� �߷��� ���� ������ ��������.
			// �ڿ������� �������� ���ؼ� ������ �и� 500~1500 ���̷� �����Ѵ�. ���ڴ� �������Դ� �ϱ� ���� �ּ����� ��ġ��.
			// �� ������ �߷� ���� ������ �����ϸ� �� ���� �ٸ� �߷� ������ ������ ���߸ʵ� ������ ������ ���̴�.
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
	// �и� Ŀ���� �������� ��������.
	jump_power = jump_start_power;
}

bool Character::CanGoToPos(const HDC terrain_dc, const POINT pos)
{
	if (!MapPixelCollision(terrain_dc, RGB(255, 0, 0), pos))
		return true;
	return false;
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

bool Character::MapPixelCollision(const HDC terrain_dc, const COLORREF& val, const POINT& pt)	// ���� ǥ�� �̹����� ����� �浹 Ȯ��, ���� ����
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
	if (looking_direction) {
		image.Draw(scene_dc, pos.x, pos.y, width, height, 0, 0, image.GetWidth(), image.GetHeight());
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