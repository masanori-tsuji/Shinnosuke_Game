//====================================================================
//
// エネミーの処理 (enemy.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================

#define DONT_CHANGE_MOVE 1000               // 引数で使う、移動を変えないための数値
#define ENEMY_COUNTERATTACK_COOL_TIME 5     // 反撃のクールタイム
#define MAX_ENEMY_COLLISION 3               // 敵一体に対する、当たり判定の最大数
#define WEAK_HIT_DAMAGE_UP_RATE 1.15f        // 弱点部位で上がるダメージ割合

// ハエの各設定
#define ENEMY_FLY_SIZE D3DXVECTOR3(80.0f, 80.0f, 0.0f)
#define ENEMY_FLY_COLLISION_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)
#define ENEMY_FLY_LIFE 70
#define ENEMY_FLY_SPEED 6.0f
#define ENEMY_FLY_SCORE 500

// 骸骨の各設定
#define ENEMY_SKELETON_SIZE D3DXVECTOR3(75.0f, 90.0f, 0.0f)
#define ENEMY_SKELETON_COLLISION_POS D3DXVECTOR3(0.0f, 7.5f, 0.0f)
#define ENEMY_SKELETON_COLLISION_SIZE D3DXVECTOR3(45.0f, 45.0f, 0.0f)
#define ENEMY_SKELETON_LIFE 150
#define ENEMY_SKELETON_SPEED 3.0f
#define ENEMY_SKELETON_SCORE 1000

// 緑鳥の各設定
#define ENEMY_GREEN_BIRD_SIZE D3DXVECTOR3(120.0f, 120.0f, 0.0f)
#define ENEMY_GREEN_BIRD_COLLISION_SIZE D3DXVECTOR3(45.0f, 50.0f, 0.0f)
#define ENEMY_GREEN_BIRD_LIFE 350
#define ENEMY_GREEN_BIRD_SPEED 1.5f
#define ENEMY_GREEN_BIRD_SCORE 3000
#define ENEMY_GREEN_BIRD_STOP 300                                       // 止まるまでのカウンタ
#define ENEMY_GREEN_BIRD_DANGER 510                                     // デンジャー警告のカウンタ
#define ENEMY_GREEN_BIRD_GOD 600                                        // ゴッドバードするまでのカウンタ
#define ENEMY_GREEN_BIRD_GOD_SPEED 15.0f                                // ゴッドバード中の速さ
#define ENEMY_GREEN_BIRD_ATTACK_COUNTER 61                              // 弾を撃つカウンタ
#define ENEMY_GREEN_BIRD_BULLET_SIZE D3DXVECTOR3(35.0f, 35.0f, 0.0f)    // 弾の大きさ
#define ENEMY_GREEN_BIRD_BULLET_SPEED 7.5f                              // 弾の速さ
#define ENEMY_GREEN_BIRD_SET_EFFECT_COUNT 3                             // 炎をセットするカウンタ

// 白カラスの設定
#define ENEMY_CROW_SIZE D3DXVECTOR3(85.0f, 85.0f, 0.0f)
#define ENEMY_CROW_COLLISION_SIZE D3DXVECTOR3(55.0f, 55.0f, 0.0f)
#define ENEMY_CROW_LIFE 1
#define ENEMY_CROW_SPEED 9.0f
#define ENEMY_CROW_SCORE 10000

// ボス森の各設定
#define ENEMY_BOSS_FOREST_SIZE D3DXVECTOR3(600.0f, 600.0f, 0.0f)
#define ENEMY_BOSS_FOREST_COLLISION_POS_1 D3DXVECTOR3(0.0f, 50.0f, 0.0f)        // 幹
#define ENEMY_BOSS_FOREST_COLLISION_SIZE_1 D3DXVECTOR3(125.0f, 380.0f, 0.0f)    // 幹
#define ENEMY_BOSS_FOREST_COLLISION_POS_2 D3DXVECTOR3(0.0f, -75.0f, 0.0f)       // 葉っぱ
#define ENEMY_BOSS_FOREST_COLLISION_SIZE_2 D3DXVECTOR3(300.0f, 150.0f, 0.0f)    // 葉っぱ
#define ENEMY_BOSS_FOREST_LIFE 6000
#define ENEMY_BOSS_FOREST_SPEED_X 2.75f
#define ENEMY_BOSS_FOREST_SPEED_Y 1.75f
#define ENEMY_BOSS_FOREST_SCORE 50000
#define ENEMY_BOSS_FOREST_STOP_X 330                                            // X軸が止まるまでのカウンタ
#define ENEMY_BOSS_FOREST_STOP_Y 150                                            // Y軸が止まるまでのカウンタ
#define ENEMY_BOSS_FOREST_ATTACK_COUNTER 100                                    // 攻撃のカウンタ
#define ENEMY_BOSS_FOREST_SET_ATTACK_POS_Y 135.0f                               // 攻撃の設置位置Y
#define ENEMY_BOSS_FOREST_CHANGE_ATTACK 2400                                    // 攻撃方法を変えるカウンタ
#define ENEMY_BOSS_FOREST_SCATTER 12                                            // 葉っぱや枝をバラまくカウンタ
#define ENEMY_BOSS_FOREST_SCATTER_LEAF_POS_Y -150.0f                            // 葉っぱをバラまく際、中心からずらす値
#define ENEMY_BOSS_FOREST_SHAKE_POS_X 3.5f                                      // 横に揺れる値
#define ENEMY_BOSS_FOREST_WEEKNESS 0                                            // 弱点部位は幹
#define ENEMY_BOSS_FOREST_WEEKNESS_ADD_SCORE 10000                              // 弱点部位で倒した時の加算スコア

// 青鳥の各設定
#define ENEMY_BLUE_BIRD_SIZE D3DXVECTOR3(120.0f, 120.0f, 0.0f)
#define ENEMY_BLUE_BIRD_COLLISION_SIZE D3DXVECTOR3(45.0f, 50.0f, 0.0f)
#define ENEMY_BLUE_BIRD_LIFE 150                                        // 1面のボス戦時のライフ
#define ENEMY_BLUE_BIRD_WAY_LIFE 400                                    // 道中のライフ
#define ENEMY_BLUE_BIRD_SPEED 2.0f
#define ENEMY_BLUE_BIRD_SCORE 4000
#define ENEMY_BLUE_BIRD_STOP 280                                        // 止まるまでのカウンタ
#define ENEMY_BLUE_BIRD_LEAVE 2400                                      // 去るまでのカウンタ
#define ENEMY_BLUE_BIRD_WAY_LEAVE (720)                                 // 道中で去るまでのカウンタ
#define ENEMY_BLUE_BIRD_ATTACK_COUNTER 180                              // 弾を撃つカウンタ
#define ENEMY_BLUE_BIRD_BULLET_SIZE D3DXVECTOR3(40.0f, 40.0f, 0.0f)     // 弾の大きさ
#define ENEMY_BLUE_BIRD_BULLET_SPEED 5.0f                               // 弾の速さ

// イモムシの各設定
#define ENEMY_CATERPILLAR_SIZE D3DXVECTOR3(90.0f, 80.0f, 0.0f)
#define ENEMY_CATERPILLAR_COLLISION_SIZE D3DXVECTOR3(60.0f, 50.0f, 0.0f)
#define ENEMY_CATERPILLAR_LIFE 60
#define ENEMY_CATERPILLAR_SCORE 250
#define ENEMY_CATERPILLAR_ROT_SPEED 0.2f                                // 回転スピード
#define ENEMY_CATERPILLAR_FALL_COUNT 90                                 // 落下するまでのカウンタ
#define ENEMY_CATERPILLAR_RIGHT_BOUND D3DXVECTOR3(3.0f, -7.5f, 0.0f)    // 右にバウンド
#define ENEMY_CATERPILLAR_LEFT_BOUND D3DXVECTOR3(-3.0f, -7.5f, 0.0f)    // 左にバウンド

// グソクムシの各設定
#define ENEMY_GUSOKU_SIZE D3DXVECTOR3(85.0f, 85.0f, 0.0f)
#define ENEMY_GUSOKU_COLLISION_SIZE D3DXVECTOR3(55.0f, 55.0f, 0.0f)
#define ENEMY_GUSOKU_LIFE 1
#define ENEMY_GUSOKU_SPEED 9.0f
#define ENEMY_GUSOKU_SCORE 10000

// サーディンの各設定
#define ENEMY_SARDINE_SIZE D3DXVECTOR3(120.0f, 55.0f, 0.0f)
#define ENEMY_SARDINE_COLLISION_SIZE D3DXVECTOR3(90.0f, 25.0f, 0.0f)
#define ENEMY_SARDINE_LIFE 100
#define ENEMY_SARDINE_MOVE D3DXVECTOR3(5.0f, 2.5f, 0.0f)
#define ENEMY_SARDINE_SCORE 1800
#define ENEMY_SARDINE_ATTACK_COUNTER 90                             // 弾を撃つカウンタ
#define ENEMY_SARDINE_BULLET_SIZE D3DXVECTOR3(40.0f, 40.0f, 0.0f)   // 弾の大きさ
#define ENEMY_SARDINE_BULLET_SPEED 7.0f                             // 弾の速さ
#define ENEMY_SARDINE_DONT_SHOT_POS_X 475.0f                        // 一定のX軸で、弾を撃たなくする
#define ENEMY_SARDINE_STRAIGHT_SPEED 5.5f                           // 直線で進むときの速さ
// サーディンの移動領域
#define ENEMY_SARDINE_INVERSIO_POS_Y_1 140.0f
#define ENEMY_SARDINE_INVERSIO_POS_Y_2 250.0f
#define ENEMY_SARDINE_INVERSIO_POS_Y_3 340.0f
#define ENEMY_SARDINE_INVERSIO_POS_Y_4 450.0f
#define ENEMY_SARDINE_INVERSIO_POS_Y_5 540.0f
#define ENEMY_SARDINE_INVERSIO_POS_Y_6 650.0f

// キャノンの各設定
#define ENEMY_CANNON_SIZE D3DXVECTOR3(120.0f, 120.0f, 0.0f)
#define ENEMY_CANNON_COLLISION_SIZE D3DXVECTOR3(60.0f, 60.0f, 0.0f)
#define ENEMY_CANNON_LIFE 75
#define ENEMY_CANNON_SCORE 5000
#define ENEMY_CANNON_ATTACK_COUNTER 100                             // 弾を撃つカウンタ
#define ENEMY_CANNON_BULLET_SIZE D3DXVECTOR3(40.0f, 40.0f, 0.0f)    // 弾の大きさ
#define ENEMY_CANNON_BULLET_SPEED 6.0f                              // 弾の速さ
#define ENEMY_CANNON_TURN_SPEED 0.035f                              // 振り向き速度

// トーピードの各設定
#define ENEMY_TORPEDO_SIZE D3DXVECTOR3(100.0f, 100.0f, 0.0f)
#define ENEMY_TORPEDO_COLLISION_SIZE D3DXVECTOR3(60.0f, 60.0f, 0.0f)
#define ENEMY_TORPEDO_LIFE 30
#define ENEMY_TORPEDO_SCORE 2800
#define ENEMY_TORPEDO_SCORE_IN_BOSS 1200                            // ボス戦時のスコア
#define ENEMY_TORPEDO_FALLING_SPEED 1.5f                            // 落下速度
#define ENEMY_TORPEDO_FALLING_SPEED_IN_BOSS 4.5f                    // ボス戦時の落下速度
#define ENEMY_TORPEDO_BULLET_SIZE D3DXVECTOR3(25.0f, 25.0f, 0.0f)   // 弾の大きさ
#define ENEMY_TORPEDO_BULLET_SPEED 3.5f                             // 弾の速さ

// ドラゴンの各設定
#define ENEMY_BOSS_DRAGON_SIZE D3DXVECTOR3(500.0f, 400.0f, 0.0f)
#define ENEMY_BOSS_DRAGON_COLLISION_POS_1 D3DXVECTOR3(-67.5f, -8.75f, 0.0f)     // 顔
#define ENEMY_BOSS_DRAGON_COLLISION_SIZE_1 D3DXVECTOR3(200.0f, 22.5f, 0.0f)     // 顔
#define ENEMY_BOSS_DRAGON_COLLISION_POS_2 D3DXVECTOR3(50.0f, 70.0f, 0.0f)       // 胴体
#define ENEMY_BOSS_DRAGON_COLLISION_SIZE_2 D3DXVECTOR3(200.0f, 150.0f, 0.0f)    // 胴体
#define ENEMY_BOSS_DRAGON_COLLISION_POS_3 D3DXVECTOR3(50.0f, 0.0f, 0.0f)        // 翼
#define ENEMY_BOSS_DRAGON_COLLISION_SIZE_3 D3DXVECTOR3(60.0f, 210.0f, 0.0f)     // 翼
#define ENEMY_BOSS_DRAGON_LIFE 8500
#define ENEMY_BOSS_DRAGON_SCORE 60000
#define ENEMY_BOSS_DRAGON_STOP_POS_X 1188.0f                                    // 止まるX座標
#define ENEMY_BOSS_DRAGON_STOP_POS_Y 490.0f                                     // 止まるY座標
#define ENEMY_BOSS_DRAGON_AURA_COLOR D3DXCOLOR(1.0f,0.99f,0.0f,1.0f)            // オーラのエフェクトの色
#define ENEMY_BOSS_DRAGON_AURA_SET_POS_Y 150.0f                                 // オーラの設置位置Y
#define ENEMY_BOSS_DRAGON_WEEKNESS 0                                            // 弱点部位は顔
#define ENEMY_BOSS_DRAGON_WEEKNESS_ADD_SCORE 20000                              // 弱点部位で倒した時の加算スコア
#define ENEMY_BOSS_DRAGON_FIRST_ATTACK_COUNTER 330                              // 最初の攻撃をするカウンタ

// マインスイープ
#define MINE_SWEEP_COUNTER 300 // 攻撃の全体フレーム
#define MINE_SWEEP_CREATE 240  // クリエイトフレーム
#define MINE_SWEEP_CHANCE 160  // 隙フレーム

// レイニーバレット
#define RAINY_BULLET_SET_POS_Y -14.0f                              // 攻撃の設置位置Y
#define RAINY_BULLET_SIZE (D3DXVECTOR3(30.0f,30.0f,0.0f))          // レイニーバレットの大きさ
#define RAINY_BULLET_LIGHT_SIZE (D3DXVECTOR3(45.0f,45.0f,0.0f))    // 雷のレイニーバレットの大きさ
#define RAINY_BULLET_COUNTER 240                                   // 攻撃の全体フレーム
#define RAINY_BULLET_CREATE 180                                    // クリエイトフレーム
#define RAINY_BULLET_CHANCE 160                                    // 隙フレーム

// ホーミングショット
#define HOMING_SHOT_SIZE (D3DXVECTOR3(50.0f,50.0f,0.0f))            // ホーミングショットの大きさ
#define HOMING_SHOT_LIGHT_SIZE (D3DXVECTOR3(100.0f,100.0f,0.0f))    // 雷のホーミングショットの大きさ
#define HOMING_SHOT_SHIFT_POS 75.0f                                 // ホーミングショットを口から出しているようにずらす
#define HOMING_SHOT_COUNTER 120                                     // 攻撃の全体フレーム
#define HOMING_SHOT_CREATE 45                                       // クリエイトフレーム
#define HOMING_SHOT_CHANCE 90                                       // 隙フレーム

// 岩石落とし
#define ROCK_SLIDE_SET_POS_Y -200.0f // 攻撃の設置位置Y
#define ROCK_SLIDE_COUNTER 180       // 攻撃の全体フレーム
#define ROCK_SLIDE_CREATE 60         // クリエイトフレーム
#define ROCK_SLIDE_CHANCE 180        // 隙フレーム

// ドラゴンチャージ
#define DRAGON_CHARGE_SIZE (D3DXVECTOR3(75.0f,75.0f,0.0f))    // ドラゴンチャージの大きさ
#define DRAGON_CHARGE_STAGE_1 25.0f                           // チャージの進行度1
#define DRAGON_CHARGE_STAGE_2 50.0f                           // チャージの進行度2
#define DRAGON_CHARGE_STAGE_3 75.0f                           // チャージの進行度3
#define DRAGON_CHARGE_FULL 100.0f                             // チャージの進行度4
#define DRAGON_CHARGE_TIME_1 1500.0f                          // チャージ時間1
#define DRAGON_CHARGE_TIME_2 3000.0f                          // チャージ時間2
#define DRAGON_CHARGE_TIME_3 4500.0f                          // チャージ時間3
#define DRAGON_CHARGE_TIME_4 6000.0f                          // チャージ時間4
#define DRAGON_CHARGE_STOP 10000.0f                           // チャージ時間ストップ
#define DRAGON_CHARGE_HEAL1 2000                              // チャージでHP回復1
#define DRAGON_CHARGE_HEAL2 1000                              // チャージでHP回復2
#define DRAGON_CHARGE_HEAL3 500                               // チャージでHP回復3
#define DRAGON_CHARGE_HEAL4 250                               // チャージでHP回復4
#define DRAGON_CHARGE_COUNTER 300                             // 攻撃の全体フレーム
#define DRAGON_CHARGE_CREATE 30                               // クリエイトフレーム
#define DRAGON_CHARGE_CHANCE 90                               // 隙フレーム

// 炎鳥の各設定
#define ENEMY_RED_BIRD_SIZE D3DXVECTOR3(120.0f, 120.0f, 0.0f)
#define ENEMY_RED_BIRD_COLLISION_SIZE D3DXVECTOR3(45.0f, 50.0f, 0.0f)
#define ENEMY_RED_BIRD_LIFE 800
#define ENEMY_RED_BIRD_SPEED 1.5f
#define ENEMY_RED_BIRD_SCORE 5000
#define ENEMY_RED_BIRD_STOP 500                                       // 止まるまでのカウンタ
#define ENEMY_RED_BIRD_DANGER 770                                     // デンジャー警告のカウンタ
#define ENEMY_RED_BIRD_GOD 860                                        // ゴッドバードするまでのカウンタ
#define ENEMY_RED_BIRD_GOD_SPEED 15.0f                                // ゴッドバード中の速さ
#define ENEMY_RED_BIRD_ATTACK_COUNTER 149                             // 弾を撃つカウンタ
#define ENEMY_RED_BIRD_BULLET_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)    // 弾の大きさ
#define ENEMY_RED_BIRD_BULLET_SPEED 8.75f                             // 弾の速さ
#define ENEMY_RED_BIRD_SET_EFFECT_COUNT 3                             // 炎をセットするカウンタ

// コウモリの各設定
#define ENEMY_BAT_SIZE D3DXVECTOR3(80.0f, 80.0f, 0.0f)
#define ENEMY_BAT_COLLISION_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)
#define ENEMY_BAT_LIFE 50
#define ENEMY_BAT_SPEED 7.0f
#define ENEMY_BAT_SCORE 2500
#define ENEMY_BAT_STOP_HOMING 270                                     // ホーミングをやめるカウンタ
#define ENEMY_BAT_SET_EFFECT_COUNT 5                                  // 炎をセットするカウンタ

// マルチバードの各設定
#define ENEMY_MULTI_BIRD_SIZE D3DXVECTOR3(180.0f, 180.0f, 0.0f)
#define ENEMY_MULTI_BIRD_COLLISION_SIZE D3DXVECTOR3(67.5f, 65.0f, 0.0f)
#define ENEMY_MULTI_BIRD_LIFE 1950
#define ENEMY_MULTI_BIRD_SPEED 2.0f
#define ENEMY_MULTI_BIRD_SCORE 77770
#define ENEMY_MULTI_BIRD_CHANGE_MOVE 275                                 // 移動を変えるまでのカウンタ
#define ENEMY_MULTI_BIRD_RETURN_X 100                                    // 横移動を折り返すカウンタ
#define ENEMY_MULTI_BIRD_LEAVE (1850)                                    // 去るまでのカウンタ
#define ENEMY_MULTI_BIRD_ATTACK_COUNTER 130                              // 弾を撃つカウンタ
#define ENEMY_MULTI_BIRD_BULLET_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)     // 弾の大きさ
#define ENEMY_MULTI_BIRD_BULLET_FIRE_SPEED 7.0f                          // マルチバードの炎弾速
#define ENEMY_MULTI_BIRD_BULLET_WATER_SPEED 7.75f                        // マルチバードの水弾速
#define ENEMY_MULTI_BIRD_BULLET_GRASS_SPEED 6.5f                         // マルチバードの草弾速

// ブレイズの各設定
#define ENEMY_BOSS_BLAZE_SIZE D3DXVECTOR3(400.0f, 400.0f, 0.0f)
#define ENEMY_BOSS_BLAZE_COLLISION_POS_1 D3DXVECTOR3(-50.0f, -10.0f, 0.0f)     // 顔
#define ENEMY_BOSS_BLAZE_COLLISION_SIZE_1 D3DXVECTOR3(25.0f, 60.0f, 0.0f)      // 顔
#define ENEMY_BOSS_BLAZE_COLLISION_POS_2 D3DXVECTOR3(-10.0f, 82.5f, 0.0f)      // 胴体
#define ENEMY_BOSS_BLAZE_COLLISION_SIZE_2 D3DXVECTOR3(102.5f, 85.0f, 0.0f)     // 胴体
#define ENEMY_BOSS_BLAZE_LIFE 13000
#define ENEMY_BOSS_BLAZE_SCORE 50000
#define ENEMY_BOSS_BLAZE_SCORE_ADD_STOCK_RATE 100000                           // 1ストックに対して、スコアを加算する割合
#define ENEMY_BOSS_BLAZE_SCORE_ADD_BARRIER_RATE 50000                          // 1バリアに対して、スコアを加算する割合
#define ENEMY_BOSS_BLAZE_STOP_POS_X 1160.0f                                    // 止まるX座標
#define ENEMY_BOSS_BLAZE_STOP_POS_Y 330.0f                                     // 止まるY座標
#define ENEMY_BOSS_BLAZE_WEEKNESS 0                                            // 弱点部位は顔
#define ENEMY_BOSS_BLAZE_WEEKNESS_ADD_SCORE 25000                              // 弱点部位で倒した時の加算スコア
#define ENEMY_BOSS_BLAZE_FIRST_ATTACK_COUNTER 150                              // 最初の攻撃をするカウンタ
#define ENEMY_BOSS_BLAZE_SPEED 2.77f                                           // 移動速度
#define ENEMY_BOSS_BLAZE_AURA_SIZE D3DXVECTOR3(30.0f, 30.0f, 0.0f)             // オーラの大きさ
#define ENEMY_BOSS_BLAZE_AURA_SET_POS_Y 100.0f                                 // オーラの設置位置Y
#define ENEMY_BOSS_BLAZE_AURA_SET_COUNT 2                                      // オーラをセットするカウンタ

// バーニングバレット
#define BURNING_BULLET_SET_POS_Y -13.0f                              // 攻撃の設置位置Y
#define BURNING_BULLET_SET_POS_LEFT -13.0f                           // 攻撃の設置位置左
#define BURNING_BULLET_SET_POS_RIGHT (SCREEN_WIDTH + 13.0f)          // 攻撃の設置位置右
#define BURNING_BULLET_SIZE (D3DXVECTOR3(27.5f,27.5f,0.0f))          // バレットの大きさ
#define BURNING_BULLET_COUNTER 210                                   // 攻撃の全体フレーム
#define BURNING_BULLET_CREATE 150                                    // クリエイトフレーム
#define BURNING_BULLET_CHANCE 140                                    // 隙フレーム

// ウィンドバレット
#define WIND_BULLET_SET_POS_RIGHT (SCREEN_WIDTH + 13.0f)          // 攻撃の設置位置右
#define WIND_BULLET_SIZE (D3DXVECTOR3(27.5f,27.5f,0.0f))          // バレットの大きさ
#define WIND_BULLET_COUNTER 180                                   // 攻撃の全体フレーム
#define WIND_BULLET_CREATE 150                                    // クリエイトフレーム
#define WIND_BULLET_CHANCE 140                                    // 隙フレーム

// ゴッドバード
#define GOD_BIRD_COUNTER 210                                                        // 攻撃の全体フレーム
#define GOD_BIRD_DANGER 150                                                         // デンジャー警告のカウンタ
#define GOD_BIRD_DANGER_SIZE D3DXVECTOR3(375.0f,187.5f,0.0f)                        // デンジャー警告の大きさ
#define GOD_BIRD_CREATE 60                                                          // クリエイトフレーム
#define GOD_BIRD_CHANCE 150                                                         // 隙フレーム
#define GOD_BIRD_FIRST_SPEED 6.0f                                                   // 初速
#define GOD_BIRD_ACCEL_SPEED 27.5f                                                  // 超加速
#define GOD_BIRD_WARP_POS D3DXVECTOR3(ENEMY_BOSS_BLAZE_STOP_POS_X, -225.0f, 0.0f)   // ワープして戻ってくる位置

// メテオストライク
#define METEO_STRIKE_SET_POS_X 1480.0f // 攻撃の設置位置X
#define METEO_STRIKE_SET_POS_Y -200.0f // 攻撃の設置位置Y
#define METEO_STRIKE_COUNTER 180       // 攻撃の全体フレーム
#define METEO_STRIKE_CREATE 60         // クリエイトフレーム
#define METEO_STRIKE_CHANCE 140        // 隙フレーム

// 神炎モード
#define GOD_FIRE_STOP_COUNTERATTACK 1000                 // 反撃をやめる
#define GOD_FIRE_SIZE (D3DXVECTOR3(70.0f,70.0f,0.0f))    // 神炎の大きさ
#define GOD_FIRE_STAGE_1 25.0f                           // チャージの進行度1
#define GOD_FIRE_STAGE_2 50.0f                           // チャージの進行度2
#define GOD_FIRE_STAGE_3 75.0f                           // チャージの進行度3
#define GOD_FIRE_FULL 100.0f                             // チャージの進行度4
#define GOD_FIRE_VALUE_FOR_ONE_CHARGE 45.0f              // 一段階チャージするのに必要な値
#define GOD_FIRE_BLAZE_ATTACK_VALUE 15.0f                // ブレイズの攻撃による、属性値の上昇量
#define GOD_FIRE_PLAYER_FIRE_ATTACK_VALUE 1.0f           // プレイヤーの炎攻撃による、属性値の上昇量

//================================================
// クラス宣言
//================================================

// エネミークラス
class CEnemy : public CScene2D
{
public:

    // エネミーの種類
    typedef enum
    {
        TYPE_FLY,           // ハエ(草)
        TYPE_SKELETON,      // 骸骨(炎)
        TYPE_GREEN_BIRD,    // 緑鳥(草)
        TYPE_CROW,          // 白カラス(マルチ)
        TYPE_BLUE_BIRD,     // 青鳥(水)
        TYPE_BOSS_FOREST,   // ボス森(草)
        TYPE_CATERPILLAR,   // イモムシ(草)
        TYPE_GUSOKU,        // グソクムシ(マルチ)
        TYPE_SARDINE,       // サーディン(水)
        TYPE_CANNON,        // キャノン(雷)
        TYPE_TORPEDO,       // トーピード(雷)
        TYPE_BOSS_DRAGON,   // ドラゴン(水)
        TYPE_RED_BIRD,      // 赤鳥(炎)
        TYPE_BAT,           // コウモリ(炎)
        TYPE_MULTI_BIRD,    // マルチバード(マルチ)
        TYPE_BOSS_BLAZE,    // ブレイズ(炎)
        TYPE_MAX            // 種類の最大数
    }TYPE;

    // 当たり判定の情報
    typedef struct
    {
        D3DXVECTOR3 posToShift;     // 中心からずらす位置
        D3DXVECTOR3 size;           // 矩形のサイズ
    }COLLISION;

    // サーディンの移動領域
    typedef enum
    {
        SARDINE_MOVE_UPPER,         // 上段
        SARDINE_MOVE_MIDDLE,        // 中段
        SARDINE_MOVE_LOWER,         // 下段
        SARDINE_MOVE_STRAIGHT,      // 真っ直ぐ
        SARDINE_MOVE_MAX            // 種類の最大数
    }SARDINE_MOVE;

    // ドラゴンの攻撃
    typedef enum
    {
        DRAGON_ATTACK_NONE = 0,       // 待機中
        DRAGON_ATTACK_MINE_SWEEP,     // マインスイープ
        DRAGON_ATTACK_RAINY_BULLET,   // レイニーバレット
        DRAGON_ATTACK_HOMING_SHOT,    // ホーミングショット
        DRAGON_ATTACK_ROCK_SLIDE,     // 岩石落とし
        DRAGON_ATTACK_CHARGE,         // チャージ
        DRAGON_ATTACK_MAX             // 種類の最大数
    }DRAGON_ATTACK;

    // ブレイズの攻撃
    typedef enum
    {
        BLAZE_ATTACK_NONE = 0,       // 待機中
        BLAZE_ATTACK_BURNING_BULLET, // バーニングバレット
        BLAZE_ATTACK_WIND_BULLET,    // ウィンドバレット
        BLAZE_ATTACK_GOD_BIRD,       // ゴッドバード
        BLAZE_ATTACK_METEO_STRIKE,   // メテオストライク
        BLAZE_ATTACK_MAX             // 種類の最大数
    }BLAZE_ATTACK;

    CEnemy();
    ~CEnemy();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nCntChangeMove, bool bMoveUp, TYPE type);
    void Hit(D3DXVECTOR3 posBullet, const int nCounterState, int nDamage, const int nWhereHit, const ELEMENT element);
    void DamageCalculation(D3DXVECTOR3 pos, const ELEMENT element, int nDamage);
    bool DefeatEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void CounterAttack(void);
    float GetAngleToPlayer(void);
    int GetCountHitWater(void) { return m_nCounterHitWater; }           // 水のヒットカウンタを返す
    int GetCountHitLight(void) { return m_nCounterHitLight; }           // 雷のヒットカウンタを返す
    COLLISION GetCollision(int nCnt) { return m_aCollision[nCnt]; }     // 衝突に関する情報を返す
    int GetCntCollision(void) { return m_nCntCollision; }               // 当たり判定の数を返す
    D3DXVECTOR3 GetPosOld(void) { return m_posOld; }                    // 1F前の位置を返す

    D3DXVECTOR3 Bound(D3DXVECTOR3 pos, D3DXVECTOR3 size);               // 床に当たった際のバウンド
    void ForestMovement(D3DXVECTOR3 playerPos);                         // ボス森の動き
    bool TorpedoExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 size);           // トーピードの爆発
    D3DXVECTOR3 DragonMovement(D3DXVECTOR3 pos, D3DXVECTOR3 size);      // ドラゴンの行動
    void DragonSetChance(int nChanceFrame);                             // ドラゴンの隙フレーム
    void DragonSetAura(D3DXVECTOR3 pos);                                // ドラゴンオーラ
    void MultiBird(D3DXVECTOR3 pos);                                    // マルチバード
    D3DXVECTOR3 BlazeMovement(D3DXVECTOR3 pos, D3DXVECTOR3 size);       // ブレイズの行動
    void BlazeSetChance(int nChanceFrame);                              // ブレイズの隙フレーム
    void BlazeSetAura(D3DXVECTOR3 pos, ELEMENT element);                // ブレイズオーラ
    void BurningBullet(void);                                           // バーニングバレット
    void WindBullet(void);                                              // ウィンドバレット

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];      // 読み込んだテクスチャを保存する
    D3DXVECTOR3 m_posOld;                                 // 1F前の位置
    D3DXVECTOR3 m_move;                                   // 移動量
    COLLISION m_aCollision[MAX_ENEMY_COLLISION];          // 当たり判定の情報
    int m_nCntCollision;                                  // 当たり判定の数
    int m_nWhereHit;                                      // どこに当たったか

    float m_fMoveAngle;     // 移動の角度
    int m_nCntChangeMove;   // 移動量が変わりだすまでのカウント
    bool m_bMoveUp;         // 上に行くかどうか

    int m_nBound;           // バウンド回数

    float m_fDestAngle;     // 目的の角度
    float m_fMinAngle;      // 最小の振り向き角度

    int m_nLife;                    // ライフ
    int m_nMaxLife;                 // 最大ライフ
    int m_nCounterState;            // 状態カウンタ
    int m_nCounterHitWater;         // 水のヒットカウンタ
    int m_nCounterHitLight;         // 雷のヒットカウンタ
    int m_nCounterAttackCoolTime;   // 反撃までのカウンタ
    bool m_bDefeatWithWeak;         // 弱点で倒したかどうか
    bool m_bDefeatOneHit;           // 一発で倒したかどうか
    TYPE m_type;                    // エネミーの種類
    ELEMENT m_element;              // エネミーの属性
};

#endif