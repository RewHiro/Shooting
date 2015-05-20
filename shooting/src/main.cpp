//
// アプリ雛形
//

//基本的な読み込み
#include "lib/defines.hpp"
#include "lib/appEnv.hpp"

//ランダム関連の読み込み
#include "lib/random.hpp"
//現時間を使える用にする
#include <ctime>

//画面サイズ
enum Window {
	WIDTH = 500,		//横幅
	HEIGHT = 800		//縦幅
};


// 
// メインプログラム
// 
int main() {
	//（横幅、縦幅、フルスクリーン、サイズを変更時に描画してるものもサイズに合わせる）
	AppEnv env(WIDTH, HEIGHT, false, true);

	//ランダム
	Random random;							//定義
	random.setSeed(u_int(time(nullptr)));	//起動するたびに使うランダムパターンを変更

	//敵のデータ
	Vec2f enemy_pos[5];				//座標
	float enemy_size[5];			//サイズ
	float enemy_speed[5];			//速度
	bool is_hit_myship = false;		//自機に当たったかどうか？
	bool is_dead[5] =				//敵	
	{ false, false, false, false, false };

	//自機のデータ
	Vec2f my_pos = { 0, -HEIGHT / 2 };		//座標
	float my_size = 100;					//サイズ
	float my_speed = 5;						//速度
	int my_hp = 3;							//体力
	float hp_draw = WIDTH / my_hp;			//体力の描画

	//自機の体力を制御する変数
	int hp_minus = 0;						//体力の減らし方を制御する為の変数

	//弾
	Vec2f shot_pos = { my_pos };			//座標
	Vec2f shot_size = { 10, 60 };			//サイズ
	float shot_speed = 5.0f;				//速度
	bool is_shot = false;					//撃ったかどうか？
	bool is_hit_enemy = false;				//敵に当たったか？

	//敵の初期化
	//----------------------------------------------------------------------
	for (int i = 0; i < 5; ++i){
		//サイズ
		enemy_size[i] =
			random.fromFirstToLast(30, 70);
		//位置
		enemy_pos[i] = Vec2f(
			random.fromFirstToLast(-WIDTH / 2, WIDTH / 2), HEIGHT / 2);
		//速度
		enemy_speed[i] =
			random.fromFirstToLast(2, 5);
	}

	//本体
	//------------------------------------------------------------------------------------------
	while (env.isOpen()) {
		//個人的に[Esc]で画面消せると楽だから消せるように
		if (env.isPushKey(GLFW_KEY_ESCAPE))return 0;

		//描画準備
		env.setupDraw();

		//敵
		//------------------------------------------------------------------------------------------------------
		for (int i = 0; i < 5; ++i){
			//描画
			if (!is_dead[i]){
				drawFillBox(enemy_pos[i].x(), enemy_pos[i].y(),
					enemy_size[i], enemy_size[i], Color(1, 1, 1));
			}
			//移動
			enemy_pos[i].y() -= enemy_speed[i];

			//敵が画面から消えたら
			if (enemy_pos[i].y() < -HEIGHT / 2){
				//敵のサイズをランダムに
				enemy_size[i] =
					random.fromFirstToLast(20, 70);
				//敵の位置をランダムに
				enemy_pos[i] = {
					random.fromFirstToLast(-WIDTH / 2.0f, (WIDTH / 2 - enemy_size[i])),
					HEIGHT / 2
				};
				//敵の速度をランダムに
				enemy_speed[i] =
					random.fromFirstToLast(5, 10);
			}

			//敵が生きているなら
			if (!is_dead[i]){
				//敵と自機の当たり判定
				if (my_pos.x() + my_size > enemy_pos[i].x() &&
					my_pos.x() < enemy_pos[i].x() + enemy_size[i] &&
					my_pos.y() + my_size > enemy_pos[i].y() &&
					my_pos.y() < enemy_pos[i].y() + enemy_size[i]){
					//当たったので判定をtrueに
					is_hit_myship = true;
				}

				//弾を発射していたら
				if (is_shot){
					//敵と弾の当たり判定
					if (shot_pos.x() + shot_size.x() > enemy_pos[i].x() &&
						shot_pos.x() < enemy_pos[i].x() + enemy_size[i] &&
						shot_pos.y() + shot_size.y() > enemy_pos[i].y() &&
						shot_pos.y() < enemy_pos[i].y() + enemy_size[i]){
						//当たったので判定をtrueに
						is_hit_enemy = true;
						//当たったので敵を生存判定をfalseに
						is_dead[i] = true;
					}
				}
			}

		}

		//自機
		//-----------------------------------------------------------------------------------------------
		//敵に当たってない時
		if (!is_hit_myship){
			//自機の体力が１以上なら
			if (my_hp >= 0){
				//自機を表示
				drawFillBox(my_pos.x(), my_pos.y(),
					my_size, my_size, Color(1, 0, 0));
			}
			//体力を正確に減らすための変数を0に
			hp_minus = 0;
		}
		//敵に当たってる時
		else {
			//体力を正確に減らすための変数を++
			++hp_minus;
			//敵に当たってない状態に戻す
			is_hit_myship = false;
		}

		//体力の描画
		drawFillBox(-WIDTH / 2, -HEIGHT / 2,
			hp_draw * my_hp, 20, Color(1, 0, 0));

		//1になった瞬間だけ
		if (hp_minus == 1)
			//自機の体力を減らす
			my_hp--;

		//Aを押しているとき
		if (env.isPressKey('A')){
			//左移動
			my_pos.x() -= my_speed;
			//if文の要らない壁判定
			my_pos.x() = std::max(my_pos.x(), -WIDTH / 2.0f);
		}
		//Dを押しているとき
		if (env.isPressKey('D')){
			//右移動
			my_pos.x() += my_speed;
			//if文の要らない壁判定
			my_pos.x() = std::min(my_pos.x(), (WIDTH / 2.0f - my_size));
		}

		//壁判定(通常)
		/*
		if (my_pos.x() < -WIDTH / 2)
		my_pos.x() = -WIDTH / 2;
		if (my_pos.x() + my_size > WIDTH / 2)
		my_pos.x() = WIDTH / 2 - my_size;
		*/

		//弾
		//--------------------------------------------------------------------------------
		//弾の描画
		//自機の体力が１以上なら
		if (my_hp >= 0){
			drawFillBox(shot_pos.x(), shot_pos.y(),
				shot_size.x(), shot_size.y(), Color(1, 1, 0));
		}

		//弾の位置
		if (!is_shot){
			//Aを押しているとき
			if (env.isPressKey('A')){
				//自機と一緒に動かす
				shot_pos.x() -= my_speed;
				//if文の要らない壁判定
				shot_pos.x() = std::max(shot_pos.x(), -WIDTH / 2.0f);
			}
			//Dを押しているとき
			if (env.isPressKey('D')){
				//自機と一緒に動かす
				shot_pos.x() += my_speed;
				//if文の要らない壁判定
				shot_pos.x() = std::min(shot_pos.x(), (WIDTH / 2.0f - my_size));
			}
		}
		//画面外に出たら
		if (shot_pos.y() > HEIGHT){
			//弾を撃てる状態に
			is_shot = false;
			//弾を自機の位置に
			shot_pos = {
				my_pos.x(),
				my_pos.y()
			};
		}

		//スペースが押された時
		if (env.isPushKey(GLFW_KEY_SPACE))
			//発射のスイッチオフなら
			if (!is_shot)
				//発射のスイッチオンに
				is_shot = true;

		//発射されたなら
		if (is_shot)
			//位置を変更(発射)
			shot_pos.y() += shot_speed;

		//敵に当たったら
		if (is_hit_enemy){
			//弾を撃てる状態に戻し、敵に当たってない状態に
			is_shot = false;
			is_hit_enemy = false;
			//弾を自機の位置に
			shot_pos = {
				my_pos.x(),
				my_pos.y()
			};
		}

		//更新
		env.update();
	}
}
