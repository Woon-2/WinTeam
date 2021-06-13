# WinTeam
윈도우프로그래밍 팀프로젝트

수정 사항:  
map_png/map1-terrain.png 수정  

던전 로드할 때 던전에 출현하는 몬스터 정보도 같이 로드하도록 수정 (몬스터 id, 그 id의 몬스터가 출현할 숫자)  
MonsterManager에 던전을 주면 그 던전에 필요한 몬스터들을 모두 "생성"  
Scene의 Update 수가 일정 숫자로 나누어 떨어질 때 몬스터 출현 -> "생성"된 몬스터들 중 일부를 "출현"시킴  

// --- 이 부분 아직 안 넣었지만 곧 추가할 것  
//bool Character::IsDied()는 hp가 0이면 true를 반환하는데 이를 통해 죽음 체크 가능 
//MonsterManager 안에는 living_monster_cnt가 있는데 MonsterManager은 죽은 몬스터가 발생할 때마다 living_monster_cnt를 줄여줌  
// ----------------------------------------

bool MonsterManager::AreMonstersAllDied() 는 living_monster_cnt가 0이어야 true를 반환  
생성된 모든 몬스터가 죽어야 다음 던전이나 이전 던전으로 넘어갈 수 있음  
  
몬스터는 캐릭터 정보 외에  
is_floating : 공중에 떠다니는 몬스터인가  
melee_attack : 근접 공격을 하는가  
missile_attack : 원거리 공격을 하는가  
animation_id0 : 정지 애니메이션  
animation_id1 : 공격 애니메이션  
animation_id2 : 이동 애니메이션  
animation_id3 : 특수 애니메이션 -> 사용하지 않는 애니메이션들은 그 id를 0으로 만들어준다.  
를 가짐  
