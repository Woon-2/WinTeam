# WinTeam
윈도우프로그래밍 팀프로젝트

#Dash 변경사항

Player::Update() 변경
Player::Dash() 추가
class Player에 dash_power, dash_radian 추가

Degree() 함수 위치 Weapon -> Fileutility

map4_terrain 색 수정함

#soundManager 추가

bgm 바꾸고 싶을때 or 처음 bgm 재생할때 PlayBgm("파일경로\\파일이름");
Effect_sound 사용하고 싶을때 InsertEffectSound("파일경로\\파일이름); 딱 한번 해주고
Effect_sound 사용할 타이밍에 Play("파일경로\\파일이름"); 해주면 됨

#RedImage 추가

키보드 C 누르면 빨간 반투명 이미지 그려줌 // Character.cpp Render 부분에서 테스트함
현재 RedImage를 flip시키면 투명처리가 안됨
