#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init()
{
	//로딩클래스 초기화
	_loading = new loading;
	_loading->init();

	//로딩 이미지 및 사운드 초기화
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release()
{
	//로딩클래스 해제
	_loading->release();
	SAFE_DELETE(_loading);
}

void loadingScene::update()
{
	//로딩클래스 업데이트
	_loading->update();

	//로딩완료후 화면 변경
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("시작화면");
	}
}

void loadingScene::render()
{
	//로딩클래스 렌더
	_loading->render();
}


//로딩이미지 함수(이곳에 이미지를 전부 넣어라)
void loadingScene::loadingImage()
{
	/*타이틀*/
	_loading->loadImage("title_bg","title_bg2.bmp", 1280, 720);
	_loading->loadImage("btn_new","btn_new.bmp", 129, 48);
	_loading->loadImage("btn_new_clk","btn_new_clk.bmp", 129, 48);
	_loading->loadImage("btn_con","btn_continue.bmp", 172, 48);
	_loading->loadImage("btn_con_clk","btn_con_clk.bmp", 172, 48);
	
	/*UI*/
	_loading->loadImage("hud_collapse", "hud_collapse.bmp", 1280, 46);


	/*플레이어*/
	_loading->loadFrameImage("player_idle", "images/player/player_idle_11x2.bmp", 770, 140, 11, 2);
	_loading->loadFrameImage("player_run", "images/player/player_run_10x2.bmp", 880, 128, 10, 2);
	_loading->loadFrameImage("player_jump", "images/player/player_jump_4x2.bmp", 240, 168, 4, 2);
	_loading->loadFrameImage("player_fall", "images/player/player_fall_4x2.bmp", 328, 192, 4, 2);
	_loading->loadFrameImage("player_fall_shadow", "images/player/player_fall_shadow_4x2.bmp", 328, 192, 4, 2);
	_loading->loadFrameImage("player_roll", "images/player/player_roll_7x2.bmp", 658, 128, 7, 2);
	_loading->loadFrameImage("player_roll_shadow", "images/player/player_roll_shadow_7x2.bmp", 658, 128, 7, 2);
	_loading->loadFrameImage("player_attack", "images/player/player_attack_7x2.bmp", 840, 164, 7, 2);
	_loading->loadFrameImage("player_attack_shadow", "images/player/player_attack_shadow_7x2.bmp", 840, 164, 7, 2);
	_loading->loadFrameImage("player_slash_basic", "images/player/player_slash_basic_6x2.bmp", 1260, 120, 6, 2);
	_loading->loadFrameImage("player_hurtfly", "images/player/player_hurtfly_4x2.bmp", 400, 132, 4, 2);
	_loading->loadFrameImage("player_hurtground", "images/player/player_hurtground_6x2.bmp", 684, 92, 6, 2);
	_loading->loadFrameImage("player_grab", "images/player/player_grab_wall_4x2.bmp", 312, 200, 4, 2);
	_loading->loadFrameImage("player_flip", "images/player/player_flip_11x2.bmp", 1056, 176, 11, 2);
	_loading->loadFrameImage("player_flip_shadow", "images/player/player_flip_shadow_11x2.bmp", 1056, 176, 11, 2);
	_loading->loadFrameImage("player_doorbreak", "images/player/player_doorbreak_10x2.bmp", 1000, 176, 10, 2);
	_loading->loadFrameImage("player_run_to_idle","images/player/player_run_to_idle_5x2.bmp", 400, 140, 5, 2);
	_loading->loadFrameImage("player_idle_to_run","images/player/player_idle_to_run_4x2.bmp", 336, 128, 5, 2);


	/*에너미*/
	_loading->loadImage("enemy_bullet", "images/enemy/enemy_bullet.bmp", 48, 2);

	//빡빡이
	_loading->loadFrameImage("enemy_bold_idle", "images/enemy/enemy_bold_Idle_8x2.bmp", 576, 140, 8, 2);
	_loading->loadFrameImage("enemy_bold_run", "images/enemy/enemy_bold_run_10x2.bmp", 900, 156, 10, 2);
	_loading->loadFrameImage("enemy_bold_walk", "images/enemy/enemy_bold_walk_8x2.bmp", 544, 152, 8, 2);
	_loading->loadFrameImage("enemy_bold_hurt", "images/enemy/enemy_bold_hurt_14x2.bmp", 1036, 128, 14, 2);

	_loading->loadImage("enemy_bold_aim_L", "images/enemy/enemy_bold_aim_L.bmp", 42, 70);
	_loading->loadImage("enemy_bold_aim_R", "images/enemy/enemy_bold_aim_R.bmp", 42, 70);
	_loading->loadImage("enemy_bold_leftarm_R", "images/enemy/enemy_bold_leftarm_R.bmp", 28, 16);
	_loading->loadImage("enemy_bold_rightarm_R", "images/enemy/enemy_bold_rightarm_R.bmp", 30, 18);
	_loading->loadImage("enemy_bold_leftarm_L", "images/enemy/enemy_bold_leftarm_L.bmp", 28, 16);
	_loading->loadImage("enemy_bold_rightarm_L", "images/enemy/enemy_bold_rightarm_L.bmp", 30, 18);
	
	//경비병
	_loading->loadFrameImage("enemy_cop_idle", "images/enemy/enemy_cop_idle_8x2.bmp", 576, 148, 8, 2);
	_loading->loadFrameImage("enemy_cop_run", "images/enemy/enemy_cop_run_10x2.bmp", 900, 160, 10, 2);
	_loading->loadFrameImage("enemy_cop_walk", "images/enemy/enemy_cop_walk_8x2.bmp", 432, 152, 8, 2);
	_loading->loadFrameImage("enemy_cop_hurt", "images/enemy/enemy_cop_hurt_14x2.bmp", 1148, 136, 14, 2);

	_loading->loadImage("enemy_cop_aim_L", "images/enemy/enemy_cop_aim_L.bmp", 46, 72);
	_loading->loadImage("enemy_cop_aim_R", "images/enemy/enemy_cop_aim_R.bmp", 46, 72);
	_loading->loadImage("enemy_cop_leftarm_R", "images/enemy/enemy_cop_leftarm_R.bmp", 28, 16);
	_loading->loadImage("enemy_cop_rightarm_R", "images/enemy/enemy_cop_rightarm_R.bmp", 30, 18);
	_loading->loadImage("enemy_cop_leftarm_L", "images/enemy/enemy_cop_leftarm_L.bmp", 28, 16);
	_loading->loadImage("enemy_cop_rightarm_L", "images/enemy/enemy_cop_rightarm_L.bmp", 30, 18);
	_loading->loadImage("enemy_gun_L", "images/enemy/enemy_gun_L.bmp", 54, 12);
	_loading->loadImage("enemy_gun_R", "images/enemy/enemy_gun_R.bmp", 54, 12);

	//그런트
	_loading->loadFrameImage("enemy_grunt_attack", "images/enemy/enemy_grunt_attack_7x2.bmp", 616, 168, 7, 2);
	_loading->loadFrameImage("enemy_grunt_hurt", "images/enemy/enemy_grunt_hurt_15x2.bmp", 1680, 164, 15, 2);
	_loading->loadFrameImage("enemy_grunt_idle", "images/enemy/enemy_grunt_idle_8x2.bmp", 480, 144, 8, 2);
	_loading->loadFrameImage("enemy_grunt_run", "images/enemy/enemy_grunt_run_10x2.bmp", 720, 156, 10, 2);
	_loading->loadFrameImage("enemy_grunt_walk", "images/enemy/enemy_grunt_walk_10x2.bmp", 640, 160, 10, 2);
	_loading->loadFrameImage("enemy_grunt_slash", "images/enemy/enemy_grunt_slash_5x2.bmp", 600, 148, 5, 2);

	//폼프
	_loading->loadFrameImage("enemy_pomp_attack", "images/enemy/enemy_pomp_attack_6x2.bmp", 552, 172, 6, 2);
	_loading->loadFrameImage("enemy_pomp_run", "images/enemy/enemy_pomp_run_10x2.bmp", 660, 160, 10, 2);
	_loading->loadFrameImage("enemy_pomp_walk", "images/enemy/enemy_pomp_walk_10x2.bmp", 620, 172, 10, 2);
	_loading->loadFrameImage("enemy_pomp_hurt", "images/enemy/enemy_pomp_hurt_15x2.bmp", 1530, 164, 15, 2);
	_loading->loadFrameImage("enemy_pomp_idle", "images/enemy/enemy_pomp_idle_8x2.bmp", 528, 168, 8, 2);

	//터렛_플로어
	_loading->loadFrameImage("enemy_turret_idle", "images/enemy/enemy_turret_idle_1x2.bmp", 100, 128, 1, 2);
	_loading->loadFrameImage("enemy_turret_die", "images/enemy/enemy_turret_die_14x2.bmp", 2380, 136, 14, 2);

	/*이펙트*/
	_loading->loadFrameImage("effect_blood", "images/effect/effect_blood_11x2.bmp", 880, 158, 11, 2);
	_loading->loadImage("effect_blood_remain1_L", "images/effect/effect_blood_remain1_L.bmp", 118, 113);
	_loading->loadImage("effect_blood_remain1_R", "images/effect/effect_blood_remain1_R.bmp", 118, 113);
	_loading->loadImage("effect_blood_remain2_L", "images/effect/effect_blood_remain2_L.bmp", 120, 116);
	_loading->loadImage("effect_blood_remain2_R", "images/effect/effect_blood_remain2_R.bmp", 120, 116);
	_loading->loadImage("effect_blood_remain3_L", "images/effect/effect_blood_remain3_L.bmp", 128, 123);
	_loading->loadImage("effect_blood_remain3_R", "images/effect/effect_blood_remain3_R.bmp", 128, 123);
	_loading->loadImage("effect_blood_remain4_L", "images/effect/effect_blood_remain4_L.bmp", 144, 144);
	_loading->loadImage("effect_blood_remain4_R", "images/effect/effect_blood_remain4_R.bmp", 144, 144);
	_loading->loadFrameImage("effect_slashhit", "images/effect/effect_slashhit3_6x2.bmp", 650, 217,6,2);
	_loading->loadFrameImage("effect_reflect", "images/effect/effect_reflect_6x2.bmp", 684, 217, 6, 2);
	_loading->loadFrameImage("effect_reflect2", "images/effect/effect_reflect_6x2.bmp", 684, 217, 6, 2);
	_loading->loadFrameImage("effect_gunspark", "images/effect/effect_gunspark_4x2.bmp", 392,156, 4, 2);
	_loading->loadImage("effect_gunspark_big", "images/effect/effect_gunspark_big.bmp", 68,76);
	_loading->loadFrameImage("effect_explosion", "images/effect/effect_explosion_12x1.bmp", 2112,224, 11, 1);
	_loading->loadFrameImage("effect_floor", "images/effect/effect_floor_2x1.bmp", 128,64, 2, 1);
	_loading->loadImage("effect_hit_lazer", "images/effect/effect_hit_lazer.bmp", 1000, 4);
	_loading->loadImage("effect_battery", "images/effect/effect_battery.bmp", 110, 20);
	_loading->loadImage("effect_timer", "images/effect/effect_timer.bmp", 188, 22);
	_loading->loadImage("effect_red", "images/effect/effect_red.bmp", 110, 20);

	/*오브젝트*/
	_loading->loadFrameImage("object_door_break", "images/object/object_door_break_20x2.bmp", 3200, 256, 20, 2);

	/*아이템*/
	_loading->loadImage("item_bottle", "images/item/item_bottle.bmp", 12, 30);
	_loading->loadImage("item_bust", "images/item/item_bust.bmp", 38, 56);
	_loading->loadImage("item_bust_small", "images/item/item_bust_small.bmp", 24, 35);
	_loading->loadImage("item_knife", "images/item/item_knife.bmp", 34, 34);
	_loading->loadFrameImage("item_arrow", "images/item/item_pickuparrow_8x1.bmp", 136, 21,8,1);
	_loading->loadImage("item_bust_get", "images/item/item_bust_get.bmp", 46, 64);
	_loading->loadImage("item_bottle_get", "images/item/item_beer_get.bmp", 20, 38);
	_loading->loadImage("item_knife_get", "images/item/item_knife_get.bmp", 42, 42);
	
	//커서
	_loading->loadImage("cursor", "images/effect/cursor.bmp", 50, 50);

	/*스테이지1*/
	_loading->loadImage("stage1_bg_collision", "images/stage1_bg_collision.bmp", 2176, 3500);
	_loading->loadImage("stage1_bg_render", "images/stage1_bg_render.bmp", 2176, 3500);
	_loading->loadImage("stage1_bg_render_slow", "images/stage1_bg_render_slow.bmp", 2176, 3500);
	
	/*스테이지2*/
	_loading->loadImage("stage2_bg_collision", "images/stage2_bg_collision.bmp", 4288, 1200);
	_loading->loadImage("stage2_bg_render", "images/stage2_bg_render.bmp", 4288, 1200);
	_loading->loadImage("stage2_bg_render_slow", "images/stage2_bg_render_slow.bmp", 4288, 1200);

	/*스테이지3*/
	_loading->loadImage("stage3_bg_collision", "images/stage3_bg_collision.bmp", 4000, 1492);
	_loading->loadImage("stage3_bg_render", "images/stage3_bg_render.bmp", 4000, 1492);
	_loading->loadImage("stage3_bg_render_slow", "images/stage3_bg_render_slow.bmp", 4000, 1492);

	/*스테이지4*/
	_loading->loadImage("stage4_bg_collision", "images/stage4_bg_collision.bmp", 1344, 1424);
	_loading->loadImage("stage4_bg_collision_explosion", "images/stage4_bg_collision_explosion.bmp", 1344, 1424);
	_loading->loadImage("stage4_bg_render", "images/stage4_bg_render.bmp", 1344, 1424);
	_loading->loadImage("stage4_bg_render_explosion", "images/stage4_bg_render_explosion.bmp", 1344, 1424);
	_loading->loadImage("stage4_bg_render_slow", "images/stage4_bg_render_slow.bmp", 1344, 1424);

	/*스테이지5*/
	_loading->loadImage("stage5_bg_collision", "images/stage5_bg_collision.bmp", 1344, 784);
	_loading->loadImage("stage5_bg_render", "images/stage5_bg_render.bmp", 1344, 784);
	_loading->loadImage("stage5_bg_render_slow", "images/stage5_bg_render_slow.bmp", 1344, 784);

	/*보스*/
	//_loading->loadFrameImage("boss_headhunter_aim", "images/boss/boss_headhunter_aim_19x2.bmp", 1482, 208, 19, 2);
	_loading->loadFrameImage("boss_aim", "images/boss/boss_headhunter_aim_1x2.bmp", 78, 208, 1, 2);
	_loading->loadFrameImage("boss_idle", "images/boss/boss_headhunter_idle_12x2.bmp", 528, 176, 12, 2);
	_loading->loadFrameImage("boss_recover", "images/boss/boss_headhunter_hurt_recover_13x2.bmp", 1820, 212, 13, 2);
	_loading->loadFrameImage("boss_drawgun", "images/boss/boss_headhunter_drawgun_8x2.bmp",608, 172, 8, 2);
	_loading->loadImage("boss_aimlazer", "images/boss/boss_aimlazer.bmp", 2000,3);
	_loading->loadFrameImage("boss_lazer", "images/boss/boss_lazer_1x5.bmp", 2000, 250, 1, 5);
	_loading->loadFrameImage("boss_lazer_top", "images/boss/boss_lazer_top_10x1.bmp", 360, 1500, 10, 1);
	_loading->loadFrameImage("boss_lazer_ground", "images/boss/boss_lazer_ground_1x10.bmp", 1500, 360, 1, 10);
	_loading->loadFrameImage("boss_lazer_ground_small", "images/boss/boss_lazer_ground_small_1x10.bmp", 2500, 360, 1, 10);
	_loading->loadFrameImage("boss_jump", "images/boss/boss_jump_1x2.bmp", 54, 176, 1, 2);
	_loading->loadFrameImage("boss_jump_shadow", "images/boss/boss_jump_shadow_1x2.bmp", 54, 176, 1, 2);
	_loading->loadFrameImage("boss_prejump", "images/boss/boss_prejump_3x2.bmp", 198, 172, 3, 2);
	_loading->loadFrameImage("boss_wallgrab", "images/boss/boss_wallgrab_3x2.bmp", 180, 172, 3, 2);
	_loading->loadFrameImage("boss_walljump", "images/boss/boss_walljump_7x2.bmp", 756, 248, 7, 2);
	_loading->loadFrameImage("boss_walljump_shadow", "images/boss/boss_walljump_shadow_7x2.bmp", 756, 248, 7, 2);
	_loading->loadFrameImage("boss_land", "images/boss/boss_land_4x2.bmp", 264, 172, 4, 2);
	_loading->loadFrameImage("boss_dash_end", "images/boss/boss_dash_end_10x2.bmp", 1000, 184, 10, 2);
	_loading->loadFrameImage("boss_sweep", "images/boss/boss_sweep_26x2.bmp", 3068, 184, 26, 2);
	_loading->loadFrameImage("boss_predash", "images/boss/boss_predash_8x2.bmp", 912, 172, 8, 2);
	_loading->loadFrameImage("boss_out_sweep", "images/boss/boss_out_sweep_3x2.bmp", 204, 164, 3, 2);
	_loading->loadFrameImage("boss_takeoutgun", "images/boss/boss_takeoutgun_7x2.bmp", 658, 172, 7, 2);
	_loading->loadFrameImage("boss_teleport_out", "images/boss/boss_teleport_out_4x2.bmp", 224, 180, 4, 2);
	_loading->loadFrameImage("boss_teleport", "images/boss/boss_teleport_8x2.bmp", 448, 180, 8, 2);
	_loading->loadFrameImage("boss_teleport_ground", "images/boss/boss_teleport_ground_8x2.bmp", 656, 140, 8, 2);
	_loading->loadFrameImage("boss_shoot", "images/boss/boss_shoot_8x2.bmp", 1088, 184, 8, 2);
	_loading->loadFrameImage("boss_in_sweep", "images/boss/boss_in_sweep_4x2.bmp", 432, 168, 4, 2);
	_loading->loadFrameImage("boss_teleport_in", "images/boss/boss_teleport_in_4x2.bmp", 224, 180, 4, 2);
	_loading->loadFrameImage("boss_dash", "images/boss/boss_dash_1x2.bmp", 102, 100, 1, 2);
	_loading->loadFrameImage("boss_dash_shadow", "images/boss/boss_dash_shadow_1x2.bmp", 102, 100, 1, 2);
	_loading->loadFrameImage("boss_mine", "images/boss/boss_mine_1x2.bmp", 40, 20, 2, 1);
	_loading->loadFrameImage("boss_in", "images/boss/boss_in_pattern_4x2.bmp", 288, 124, 4, 2);
	_loading->loadFrameImage("boss_out", "images/boss/boss_out_pattern_4x2.bmp", 288, 124, 4, 2);
	_loading->loadImage("boss_circle", "images/boss/boss_circle.bmp", 274,274);
	_loading->loadFrameImage("boss_dieground", "images/boss/boss_dieground_8x2.bmp", 928, 164, 8, 2);
	_loading->loadFrameImage("boss_headground", "images/boss/boss_headground_8x2.bmp", 720, 56, 8, 2);
	_loading->loadFrameImage("boss_headfly", "images/boss/boss_headfly_8x2.bmp", 320, 84, 8, 2);
	_loading->loadFrameImage("boss_diefly", "images/boss/boss_diefly_4x2.bmp", 328, 104, 4, 2);
	_loading->loadFrameImage("boss_nohead", "images/boss/boss_nohead_6x2.bmp", 720, 88, 6, 2);
	_loading->loadFrameImage("boss_dead", "images/boss/boss_dead_19x2.bmp", 2128, 84, 19, 2);

}

//로딩사운드 함수(이곳에 사운드를 전부 넣어라)
void loadingScene::loadingSound()
{
}
