#include "MonsterAI.h"

void MonsterAI::Stand()
{
	monster->cur_animation_name = monster->stand_animation_name;
}

void MonsterAI::MoveToPlayer(const Player* player)
{
	if (!monster->move_animation_name.empty())
		monster->cur_animation_name = monster->move_animation_name;
	else
		monster->cur_animation_name = monster->stand_animation_name;

	// 플레이어가 위쪽, 바로 위에 올라갈 수 있는 블록이 있다면 점프

	// 이동하려는데 벽으로 막혀있으면 점프
}

void MonsterAI::MoveFromPlayer(const Player* player)
{
	if (!monster->move_animation_name.empty())
		monster->cur_animation_name = monster->move_animation_name;
	else
		monster->cur_animation_name = monster->stand_animation_name;

	// 이동하려는데 벽으로 막혀있으면 점프
}

void MonsterAI::Attack()
{
	if (!monster->attack_animation_name.empty())
		monster->cur_animation_name = monster->attack_animation_name;
	else
		monster->cur_animation_name = monster->stand_animation_name;
}
