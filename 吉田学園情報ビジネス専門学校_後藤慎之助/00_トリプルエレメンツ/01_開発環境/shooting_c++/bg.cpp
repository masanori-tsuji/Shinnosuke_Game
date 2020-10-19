//===============================================
//
// 背景処理 (bg.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "bg.h"
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "enemy.h"

//================================================
// マクロ定義
//================================================

// ステージ1の背景 
#define STAGE1_SKY_FLOW_SPEED 1000           // 空の流れる速度
#define STAGE1_SKY_FLOW_SPEED_BOSS 1000      // ボス戦時の、空の流れる速度
#define STAGE1_SKY_RED_RATE 0.001f           // だんだん空を赤くする
#define STAGE1_MOUNTAIN_FLOW_SPEED 500       // 山の流れる速度
#define STAGE1_MOUNTAIN_FLOW_SPEED_BOSS 1000 // 山の流れる速度(ボス戦に入るとき)
#define STAGE1_MOUNTAIN_FLOW_UP_RATE 0.175f  // 山がだんだん上がっていく割合
#define STAGE1_WOODS_FLOW_SPEED 250          // 林の流れる速度
#define STAGE1_WOODS_FLOW_SPEED_BOSS 500     // 林の流れる速度(ボス戦に入るとき)
#define STAGE1_WOODS_STOP_POS 400.0f         // 林の流れが止まる位置
#define STAGE1_WOODS_FLOW_UP_RATE 0.35f      // 林がだんだん上がっていく割合
#define STAGE1_FOREST_FIRST_POS D3DXVECTOR3(SCREEN_WIDTH + 825.0f, SCREEN_HEIGHT / 2 + 80.0f, 0.0f)    // 森の最初の位置
#define STAGE1_FOREST_SIZE  (D3DXVECTOR3(625.0f, 625.0f,0.0f))                                 // 森の大きさ
#define STAGE1_FOREST_SPEED_X 2.75f                                                            // 森の横移動の速さ
#define STAGE1_FOREST_STOP_COUNT 330                                                           // 森が止まるまでのカウンタ
#define STAGE1_FOREST_SHAKE_POS_X 4.5f                                                         // 森が揺れる値

// ステージ2の背景
#define STAGE2_SAND_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 80.0f,0.0f))   // 砂の位置
#define STAGE2_SAND_SIZE (D3DXVECTOR3(SCREEN_WIDTH, 180.0f,0.0f))                     // 砂の大きさ
#define STAGE2_SAND_FLOW_SPEED 400                                                    // 砂の流れる速度
#define STAGE2_ROCKS_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 150.0f,0.0f)) // 岩の位置
#define STAGE2_ROCKS_SIZE (D3DXVECTOR3(SCREEN_WIDTH + 60.0f, 360.0f,0.0f))            // 岩の大きさ
#define STAGE2_ROCKS_FLOW_SPEED 600                                                   // 岩の流れる速度
#define STAGE2_FISHES_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 400.0f,0.0f))// 魚群の位置
#define STAGE2_FISHES_SIZE (D3DXVECTOR3(SCREEN_WIDTH + 720.0f, 360.0f,0.0f))          // 魚群の大きさ
#define STAGE2_FISHES_FLOW_SPEED 2000                                                 // 魚群の流れる速度
#define STAGE2_OCEAN_FLOW_SPEED 800                                                   // 海の流れる速度
#define STAGE2_OCEAN_FLOW_SPEED_BOSS 800                                              // 海の流れる速度(ボス戦)
#define STAGE2_RED_FADE_RATE 0.003f                                                   // 赤フェード割合
#define STAGE2_RED_FADE_MAX 0.78f                                                     // 赤フェードの最大値

// ステージ3の背景
#define STAGE3_TAKE_OFF_TO_THE_SKY 2850                                                     // 空に飛び立つカウンタ(game.cppのカウンタと調整)
#define STAGE3_END_SKY_MODE 4850                                                            // 空モードの終了
#define STAGE3_RED_SKY_FLOW_SPEED 2850                                                      // 赤い空の流れる速度
#define STAGE3_IN_THE_RED_SKY_FLOW_SPEED 400                                                // 赤い空の中の時の、流れる速度
#define STAGE3_RED_SKY_SPEED_BOSS 600                                                       // 赤い空のボス戦時の流れる速度
#define STAGE3_DEAD_END_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100.0f,0.0f))    // 行き止まりの位置
#define STAGE3_DEAD_END_SIZE (D3DXVECTOR3(SCREEN_WIDTH + 70.0f, 400.0f,0.0f))               // 行き止まりの大きさ
#define STAGE3_RED_GROUND_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 150.0f,0.0f))  // 赤い地面の位置
#define STAGE3_RED_GROUND_SIZE (D3DXVECTOR3(SCREEN_WIDTH + 60.0f, 360.0f,0.0f))             // 赤い地面の大きさ
#define STAGE3_RED_GROUND_FLOW_SPEED 600                                                    // 赤い地面の流れる速度
#define STAGE3_VOLCANO_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 400.0f,0.0f))     // 火山の位置
#define STAGE3_VOLCANO_SIZE (D3DXVECTOR3(SCREEN_WIDTH + 720.0f, 275.0f,0.0f))               // 火山の大きさ
#define STAGE3_VOLCANO_FLOW_SPEED 2000                                                      // 火山の流れる速度
#define STAGE3_THE_TOP_POS (D3DXVECTOR3(SCREEN_WIDTH + (SCREEN_WIDTH / 2) + 300.0f, SCREEN_HEIGHT - 135.0f,0.0f))// 頂上の位置
#define STAGE3_THE_TOP_SIZE (D3DXVECTOR3(SCREEN_WIDTH + 100.0f, 300.0f,0.0f))                                    // 頂上の大きさ
#define STAGE3_THE_TOP_FLOAT_IN_SPEED 3.6f                                                  // 頂上の流れる速度
#define STAGE3_STOP_POS_X -2000.0f                                                          // 行き止まりを止める位置
#define STAGE3_STOP_POS_Y 1440.0f                                                           // 空以外を止める位置
#define STAGE3_RED_RATE 0.001f                                                              // だんだん赤くする
#define STAGE3_RED_MAX 0.5f                                                                 // 赤みの最大
#define STAGE3_RED_MIN 0.9f                                                                 // 赤みの最小

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CBg::m_apTexture[CBg::TYPE_MAX] = {};
bool CBg::m_bShake = false;
int CBg::m_nCntState = 0;
bool CBg::m_bEraseForest = false;
bool CBg::m_bInTheSky = false;

//========================================
// 背景のコンストラクタ
//========================================
CBg::CBg() :CScene(OBJTYPE::OBJTYPE_BG)
{
    for (int nCntBg = 0; nCntBg < CBg::TYPE_MAX; nCntBg++)
    {
        m_apScene2D[nCntBg] = NULL;
    }

    m_col = DEFAULT_COLOR;
    m_set = SET_NONE;
    m_nCntTime = 0;
    m_nCntState = 0;
    m_bShake = false;
    m_bEraseForest = false;
    m_bInTheSky = false;
}

//========================================
// 背景のデストラクタ
//========================================
CBg::~CBg()
{

}

//========================================
// 背景の画像ロード処理
//========================================
HRESULT CBg::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000.jpg", &m_apTexture[CBg::TYPE_SKY]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg003.png", &m_apTexture[CBg::TYPE_MOUNTAIN]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg001.png", &m_apTexture[CBg::TYPE_WOODS]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/forest000.png", &m_apTexture[CBg::TYPE_FOREST]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/sand000.png", &m_apTexture[CBg::TYPE_SAND]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg002.png", &m_apTexture[CBg::TYPE_OCEAN]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg004.png", &m_apTexture[CBg::TYPE_ROCKS]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg005.png", &m_apTexture[CBg::TYPE_FISHES]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg006.png", &m_apTexture[CBg::TYPE_DEAD_END]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000.jpg", &m_apTexture[CBg::TYPE_RED_SKY]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg007.png", &m_apTexture[CBg::TYPE_RED_GROUND]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg008.png", &m_apTexture[CBg::TYPE_VOLCANO]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg009.png", &m_apTexture[CBg::TYPE_THE_TOP]);

    return S_OK;
}

//========================================
// 背景の画像破棄処理
//========================================
void CBg::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CBg::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// 背景の作成
//================================
CBg *CBg::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, SET set)
{
    CBg *pBg = NULL;

    // メモリを確保
    pBg = new CBg;

    // 引数とメンバ変数を結びつける
    pBg->m_set = set;

    // 初期化
    pBg->Init(pos, size);

    return pBg;
}

//========================================
// 背景の初期化処理
//========================================
HRESULT CBg::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    switch (m_set)
    {
    case SET_α:

        // 生成と初期化
        m_apScene2D[CBg::TYPE_SKY] = new CScene2D;
        m_apScene2D[CBg::TYPE_SKY]->CScene2D::Init(pos, size);
        m_apScene2D[CBg::TYPE_MOUNTAIN] = new CScene2D;
        m_apScene2D[CBg::TYPE_MOUNTAIN]->CScene2D::Init(pos + D3DXVECTOR3(0.0f, 25.0f, 0.0f), size);
        m_apScene2D[CBg::TYPE_WOODS] = new CScene2D;
        m_apScene2D[CBg::TYPE_WOODS]->CScene2D::Init(pos + D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 4 - 25.0f, 0.0f), size);
        m_apScene2D[CBg::TYPE_FOREST] = new CScene2D;
        m_apScene2D[CBg::TYPE_FOREST]->CScene2D::Init(STAGE1_FOREST_FIRST_POS, STAGE1_FOREST_SIZE);

        // テクスチャを割り当て
        m_apScene2D[CBg::TYPE_SKY]->BindTexture(m_apTexture[CBg::TYPE_SKY]);
        m_apScene2D[CBg::TYPE_WOODS]->BindTexture(m_apTexture[CBg::TYPE_WOODS]);
        m_apScene2D[CBg::TYPE_MOUNTAIN]->BindTexture(m_apTexture[CBg::TYPE_MOUNTAIN]);
        m_apScene2D[CBg::TYPE_FOREST]->BindTexture(m_apTexture[CBg::TYPE_FOREST]);

        break;

    case SET_β:

        // 海
        // 生成と初期化
        m_apScene2D[CBg::TYPE_OCEAN] = new CScene2D;
        m_apScene2D[CBg::TYPE_OCEAN]->CScene2D::Init(pos, size);
        // テクスチャを割り当て
        m_apScene2D[CBg::TYPE_OCEAN]->BindTexture(m_apTexture[CBg::TYPE_OCEAN]);

        // 魚群
        // 生成と初期化
        m_apScene2D[CBg::TYPE_FISHES] = new CScene2D;
        m_apScene2D[CBg::TYPE_FISHES]->CScene2D::Init(STAGE2_FISHES_POS, STAGE2_FISHES_SIZE);
        // テクスチャを割り当て
        m_apScene2D[CBg::TYPE_FISHES]->BindTexture(m_apTexture[CBg::TYPE_FISHES]);

        // 岩
        // 生成と初期化
        m_apScene2D[CBg::TYPE_ROCKS] = new CScene2D;
        m_apScene2D[CBg::TYPE_ROCKS]->CScene2D::Init(STAGE2_ROCKS_POS, STAGE2_ROCKS_SIZE);
        // テクスチャを割り当て
        m_apScene2D[CBg::TYPE_ROCKS]->BindTexture(m_apTexture[CBg::TYPE_ROCKS]);

        // 砂
        // 生成と初期化
        m_apScene2D[CBg::TYPE_SAND] = new CScene2D;
        m_apScene2D[CBg::TYPE_SAND]->CScene2D::Init(STAGE2_SAND_POS, STAGE2_SAND_SIZE);
        // テクスチャを割り当て
        m_apScene2D[CBg::TYPE_SAND]->BindTexture(m_apTexture[CBg::TYPE_SAND]);

        break;

    case SET_γ:

        // 赤い空
        // 生成と初期化
        m_apScene2D[CBg::TYPE_RED_SKY] = new CScene2D;
        m_apScene2D[CBg::TYPE_RED_SKY]->CScene2D::Init(pos, size);
        // テクスチャを割り当て
        m_apScene2D[CBg::TYPE_RED_SKY]->BindTexture(m_apTexture[CBg::TYPE_RED_SKY]);

        // 赤い地面
        // 生成と初期化
        m_apScene2D[CBg::TYPE_RED_GROUND] = new CScene2D;
        m_apScene2D[CBg::TYPE_RED_GROUND]->CScene2D::Init(STAGE3_RED_GROUND_POS, STAGE3_RED_GROUND_SIZE);
        // テクスチャを割り当て
        m_apScene2D[CBg::TYPE_RED_GROUND]->BindTexture(m_apTexture[CBg::TYPE_RED_GROUND]);

        // 火山
        // 生成と初期化
        m_apScene2D[CBg::TYPE_VOLCANO] = new CScene2D;
        m_apScene2D[CBg::TYPE_VOLCANO]->CScene2D::Init(STAGE3_VOLCANO_POS, STAGE3_VOLCANO_SIZE);
        // テクスチャを割り当て
        m_apScene2D[CBg::TYPE_VOLCANO]->BindTexture(m_apTexture[CBg::TYPE_VOLCANO]);

        // 行き止まり
        // 生成と初期化
        m_apScene2D[CBg::TYPE_DEAD_END] = new CScene2D;
        m_apScene2D[CBg::TYPE_DEAD_END]->CScene2D::Init(STAGE3_DEAD_END_POS, STAGE3_DEAD_END_SIZE);
        // テクスチャを割り当て
        m_apScene2D[CBg::TYPE_DEAD_END]->BindTexture(m_apTexture[CBg::TYPE_DEAD_END]);

        // 頂上
        // 生成と初期化
        m_apScene2D[CBg::TYPE_THE_TOP] = new CScene2D;
        m_apScene2D[CBg::TYPE_THE_TOP]->CScene2D::Init(STAGE3_THE_TOP_POS, STAGE3_THE_TOP_SIZE);
        // テクスチャを割り当て
        m_apScene2D[CBg::TYPE_THE_TOP]->BindTexture(m_apTexture[CBg::TYPE_THE_TOP]);

        break;
    }

    return S_OK;
}

//========================================
// 背景の終了処理
//========================================
void CBg::Uninit(void)
{
    // 終了処理
    for (int nCntBg = 0; nCntBg < CBg::TYPE_MAX; nCntBg++)
    {
        if (m_apScene2D[nCntBg] != NULL)
        {
            m_apScene2D[nCntBg]->Uninit();
            m_apScene2D[nCntBg] = NULL;
        }
    }

    Release();
}
//========================================
// 背景の更新処理
//========================================
void CBg::Update(void)
{
    // 変数宣言
    bool bBossBattle = false;           // ボスバトル中かどうか
    D3DXVECTOR3 pos = DEFAULT_VECTOR;   // 位置

    // ボスバトル中かどうかを取得
    bBossBattle = CGame::GetBossBattle();

    switch (m_set)
    {
    case SET_α:

        if (bBossBattle == false)
        {// 通常時

            m_apScene2D[CBg::TYPE_SKY]->SetFlowingAnimation(1, STAGE1_SKY_FLOW_SPEED, true, CScene2D::DIRECT_HORIZON);
            m_apScene2D[CBg::TYPE_MOUNTAIN]->SetFlowingAnimation(1, STAGE1_MOUNTAIN_FLOW_SPEED, true, CScene2D::DIRECT_HORIZON);
            m_apScene2D[CBg::TYPE_WOODS]->SetFlowingAnimation(1, STAGE1_WOODS_FLOW_SPEED, true, CScene2D::DIRECT_HORIZON);
        }
        else
        {// ボスバトル中

            // タイム加算
            m_nCntTime++;

            // 上限で、タイムを止める
            if (m_nCntTime > STAGE1_FOREST_STOP_COUNT)
            {
                m_nCntTime = STAGE1_FOREST_STOP_COUNT;
            }

            // 空をゆっくりにする
            m_apScene2D[CBg::TYPE_SKY]->SetFlowingAnimation(1, STAGE1_SKY_FLOW_SPEED_BOSS, true, CScene2D::DIRECT_HORIZON);

            //====================================================================================================
            // 林の位置を取得
            pos = m_apScene2D[CBg::TYPE_WOODS]->GetPosition();

            // 林の位置をだんだん上へ(一定の位置に達するまで)
            if (pos.y >= STAGE1_WOODS_STOP_POS)
            {
                // 林をだんだん上へ
                pos.y -= STAGE1_WOODS_FLOW_UP_RATE;

                // 林の位置を反映
                m_apScene2D[CBg::TYPE_WOODS]->SetPosition(pos);

                // 林をゆっくりにする（一定の位置で止める）
                m_apScene2D[CBg::TYPE_WOODS]->SetFlowingAnimation(1, STAGE1_WOODS_FLOW_SPEED_BOSS, true, CScene2D::DIRECT_HORIZON);

                // 背景を赤くしていく
                m_col.g -= STAGE1_SKY_RED_RATE;
                m_col.b -= STAGE1_SKY_RED_RATE;
                m_apScene2D[CBg::TYPE_SKY]->SetColor(m_col);
                m_apScene2D[CBg::TYPE_MOUNTAIN]->SetColor(m_col);
                m_apScene2D[CBg::TYPE_WOODS]->SetColor(m_col);

                // 山の位置を取得
                pos = m_apScene2D[CBg::TYPE_MOUNTAIN]->GetPosition();

                // 山をだんだん上へ
                pos.y -= STAGE1_MOUNTAIN_FLOW_UP_RATE;

                // 山の位置を反映
                m_apScene2D[CBg::TYPE_MOUNTAIN]->SetPosition(pos);

                // 山をゆっくりにする（一定の位置で止める）
                m_apScene2D[CBg::TYPE_MOUNTAIN]->SetFlowingAnimation(1, STAGE1_MOUNTAIN_FLOW_SPEED_BOSS, true, CScene2D::DIRECT_HORIZON);
            }
            //======================================================================================================


            //=========================================================
            // 森の位置を取得
            pos = m_apScene2D[CBg::TYPE_FOREST]->GetPosition();

            // タイムが一定以下なら移動
            if (m_nCntTime < STAGE1_FOREST_STOP_COUNT)
            {
                // 左へ移動
                pos.x -= STAGE1_FOREST_SPEED_X;
            }

            // 森の位置を反映
            m_apScene2D[CBg::TYPE_FOREST]->SetPosition(pos);
            //=========================================================
        }

        // 頂点座標の更新
        m_apScene2D[CBg::TYPE_SKY]->SetVertex();
        m_apScene2D[CBg::TYPE_MOUNTAIN]->SetVertex();
        m_apScene2D[CBg::TYPE_WOODS]->SetVertex();

        // 森は揺らすかどうかで場合分け
        if (m_bShake == true)
        {
            // 偶数で割れるなら、右に揺れる
            if (m_nCntState % 2 == 0)
            {
                // 見かけ上の位置を設定
                m_apScene2D[CBg::TYPE_FOREST]->SetVisualVertex(D3DXVECTOR3(pos.x + STAGE1_FOREST_SHAKE_POS_X, pos.y, 0.0f),
                    STAGE1_FOREST_SIZE);
            }
            // 奇数なら左
            else
            {
                // 見かけ上の位置を設定
                m_apScene2D[CBg::TYPE_FOREST]->SetVisualVertex(D3DXVECTOR3(pos.x - STAGE1_FOREST_SHAKE_POS_X, pos.y, 0.0f),
                    STAGE1_FOREST_SIZE);
            }

            // 色を変える(赤)
            m_apScene2D[CBg::TYPE_FOREST]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
        }
        else
        {
            // 通常
            m_apScene2D[CBg::TYPE_FOREST]->SetVertex();
        }

        break;

    case SET_β:

        if (bBossBattle == false)
        {// 通常時
            m_apScene2D[CBg::TYPE_OCEAN]->SetFlowingAnimation(1, STAGE2_OCEAN_FLOW_SPEED, true, CScene2D::DIRECT_HORIZON);
            m_apScene2D[CBg::TYPE_FISHES]->SetFlowingAnimation(1, STAGE2_FISHES_FLOW_SPEED, true, CScene2D::DIRECT_HORIZON);
            m_apScene2D[CBg::TYPE_ROCKS]->SetFlowingAnimation(1, STAGE2_ROCKS_FLOW_SPEED, true, CScene2D::DIRECT_HORIZON);
            m_apScene2D[CBg::TYPE_SAND]->SetFlowingAnimation(1, STAGE2_SAND_FLOW_SPEED, true, CScene2D::DIRECT_HORIZON);
        }
        else
        {// ボスバトル中

            // 背景を赤くしていく
            m_col.g -= STAGE2_RED_FADE_RATE;
            m_col.b -= STAGE2_RED_FADE_RATE;

            // 最大まで赤くする
            if (m_col.g <= STAGE2_RED_FADE_MAX)
            {
                m_col.g = STAGE2_RED_FADE_MAX;
                m_col.b = STAGE2_RED_FADE_MAX;
            }

            // 色を反映
            m_apScene2D[CBg::TYPE_OCEAN]->SetColor(m_col);
            m_apScene2D[CBg::TYPE_SAND]->SetColor(m_col);

            // 魚群は常に流れる
            m_apScene2D[CBg::TYPE_FISHES]->SetFlowingAnimation(1, STAGE2_FISHES_FLOW_SPEED, true, CScene2D::DIRECT_HORIZON);

            // 海はゆっくり流れる
            m_apScene2D[CBg::TYPE_OCEAN]->SetFlowingAnimation(1, STAGE2_OCEAN_FLOW_SPEED_BOSS, true, CScene2D::DIRECT_HORIZON);
        }

        // 頂点座標の更新
        m_apScene2D[CBg::TYPE_OCEAN]->SetVertex();
        m_apScene2D[CBg::TYPE_FISHES]->SetVertex();
        m_apScene2D[CBg::TYPE_ROCKS]->SetVertex();
        m_apScene2D[CBg::TYPE_SAND]->SetVertex();

        break;

    case SET_γ:

        // 時間を数える
        m_nCntTime++;

        // 飛び立つまで
        if (m_nCntTime < STAGE3_TAKE_OFF_TO_THE_SKY)
        {
            m_apScene2D[CBg::TYPE_RED_SKY]->SetFlowingAnimation(1, STAGE3_RED_SKY_FLOW_SPEED, true, CScene2D::DIRECT_HORIZON);

            m_apScene2D[CBg::TYPE_RED_GROUND]->SetFlowingAnimation(1, STAGE3_RED_GROUND_FLOW_SPEED, true, CScene2D::DIRECT_HORIZON);

            m_apScene2D[CBg::TYPE_VOLCANO]->SetFlowingAnimation(1, STAGE3_VOLCANO_FLOW_SPEED, true, CScene2D::DIRECT_HORIZON);

            // だんだん赤く
            if (m_col.g > STAGE3_RED_MAX)
            {
                m_col.g -= STAGE3_RED_RATE;
                m_col.b -= STAGE3_RED_RATE;
            }
        }
        // 飛び立った後
        else if(m_nCntTime >= STAGE3_TAKE_OFF_TO_THE_SKY && m_nCntTime < STAGE3_END_SKY_MODE)
        {
            // 空のカウンタをリセット
            if (m_nCntTime == STAGE3_TAKE_OFF_TO_THE_SKY)
            {
                m_apScene2D[CBg::TYPE_RED_SKY]->ResetCountAnim();
            }

            // 空の中
            m_bInTheSky = true;

            // 空の流れる速度を上げる
            m_apScene2D[CBg::TYPE_RED_SKY]->SetFlowingAnimation(1, STAGE3_IN_THE_RED_SKY_FLOW_SPEED, true, CScene2D::DIRECT_RIGHT_UP);

            // 空以外を下げる処理
            //===========================================================================
            // 位置を取得
            pos = m_apScene2D[CBg::TYPE_RED_GROUND]->GetPosition();

            // 下へ移動
            pos.y += FLOWING_STAGE;

            // 完全に見えない位置で止めておく
            if (pos.y > STAGE3_STOP_POS_Y)
            {
                pos.y = STAGE3_STOP_POS_Y;
            }

            // 位置を反映
            m_apScene2D[CBg::TYPE_RED_GROUND]->SetPosition(pos);
            //============================================================================
            // 位置を取得
            pos = m_apScene2D[CBg::TYPE_VOLCANO]->GetPosition();

            // 下へ移動
            pos.y += FLOWING_STAGE;

            // 完全に見えない位置で止めておく
            if (pos.y > STAGE3_STOP_POS_Y)
            {
                pos.y = STAGE3_STOP_POS_Y;
            }

            // 位置を反映
            m_apScene2D[CBg::TYPE_VOLCANO]->SetPosition(pos);
            //===========================================================================

            // だんだん赤みを解除
            if (m_col.g < STAGE3_RED_MIN)
            {
                m_col.g += STAGE3_RED_RATE;
                m_col.b += STAGE3_RED_RATE;
            }
        }
        else if(m_nCntTime >= STAGE3_END_SKY_MODE)
        {
            // カウンタ固定
            m_nCntTime = STAGE3_END_SKY_MODE;

            // 空モード終了
            m_bInTheSky = false;

            // 空の流れる速度を遅く
            m_apScene2D[CBg::TYPE_RED_SKY]->SetFlowingAnimation(1, STAGE3_RED_SKY_SPEED_BOSS, true, CScene2D::DIRECT_HORIZON);

            // ボス戦時
            if (bBossBattle == true)
            {
                // だんだん赤く
                if (m_col.g > STAGE3_RED_MAX)
                {
                    m_col.g -= STAGE3_RED_RATE;
                    m_col.b -= STAGE3_RED_RATE;
                }

                // 頂上の処理
                //===========================================================================
                // 位置を取得
                pos = m_apScene2D[CBg::TYPE_THE_TOP]->GetPosition();

                // 真ん中より右側なら
                if (pos.x > SCREEN_WIDTH / 2)
                {
                    // 左へ移動
                    pos.x -= STAGE3_THE_TOP_FLOAT_IN_SPEED;
                }

                // 位置を反映
                m_apScene2D[CBg::TYPE_THE_TOP]->SetPosition(pos);
                //===========================================================================
            }
        }

        // 行き止まりの処理
        //===========================================================================
        // 位置を取得
        pos = m_apScene2D[CBg::TYPE_DEAD_END]->GetPosition();

        // 左へ移動
        pos.x -= FLOWING_STAGE;

        // 完全に見えない位置で止めておく
        if (pos.x < STAGE3_STOP_POS_X)
        {
            pos.x = STAGE3_STOP_POS_X;
        }

        // 位置を反映
        m_apScene2D[CBg::TYPE_DEAD_END]->SetPosition(pos);
        //===========================================================================

        // 色の更新
        m_apScene2D[CBg::TYPE_RED_SKY]->SetColor(m_col);
        m_apScene2D[CBg::TYPE_RED_GROUND]->SetColor(m_col);
        m_apScene2D[CBg::TYPE_VOLCANO]->SetColor(m_col);
        m_apScene2D[CBg::TYPE_DEAD_END]->SetColor(m_col);
        m_apScene2D[CBg::TYPE_THE_TOP]->SetColor(m_col);

        // 頂点座標の更新
        m_apScene2D[CBg::TYPE_DEAD_END]->SetVertex();
        m_apScene2D[CBg::TYPE_RED_SKY]->SetVertex();
        m_apScene2D[CBg::TYPE_RED_GROUND]->SetVertex();
        m_apScene2D[CBg::TYPE_VOLCANO]->SetVertex();
        m_apScene2D[CBg::TYPE_THE_TOP]->SetVertex();

        break;
    }  

    // 状態カウンタを減算
    if (m_nCntState > 0)
    {
        m_nCntState--;
    }
    // 状態カウンタがないなら
    else
    {
        // 揺れを止める
        m_bShake = false;

        // ステージαなら
        if (m_set == SET_α)
        {
            // 森を消すフラグがfalseなら
            if (m_bEraseForest == false)
            {
                // 色を戻す
                m_apScene2D[CBg::TYPE_FOREST]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
            }
        }
    }

    // 森を消すフラグがtrueなら
    if (m_bEraseForest == true)
    {
        // ステージαなら
        if (m_set == SET_α)
        {
            // 森を消す(透明に)
            m_apScene2D[CBg::TYPE_FOREST]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
        }
    }
}

//========================================
// 背景の描画処理
//========================================
void CBg::Draw(void)
{
    // DrawAllで呼び出されている
}