//===============================================
//
// 様々な処理をまとめたファイル (library.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "library.h"

//===========================================
// 画面外かどうか(二次元)
//===========================================
bool OutScreen2D(const D3DXVECTOR3 *pPos, const D3DXVECTOR3 *pSize)
{
    // 変数宣言
    bool bOutScreen = false;    // 画面外かどうか

    // 画面の端から出ているかどうか
    if (pPos->x < -(pSize->x / 2) ||
        pPos->x > SCREEN_WIDTH + pSize->x / 2 ||
        pPos->y < -(pSize->y / 2) ||
        pPos->y > SCREEN_HEIGHT + pSize->y / 2)
    {
        // 画面外のフラグをtrueに
        bOutScreen = true;
    }

    return bOutScreen;
}

//===========================================
// ゲーム外かどうか(二次元)
//===========================================
bool OutGame2D(const D3DXVECTOR3 * pPos, const D3DXVECTOR3 * pSize)
{
    // 変数宣言
    bool bOutGame = false;    // ゲーム外かどうか

    // 画面の端から2倍のところから出ているかどうか
    if (pPos->x < -SCREEN_WIDTH -(pSize->x / 2) ||
        pPos->x > (SCREEN_WIDTH * 2) + pSize->x / 2 ||
        pPos->y < -SCREEN_HEIGHT -(pSize->y / 2) ||
        pPos->y > (SCREEN_HEIGHT * 2) + pSize->y / 2)
    {
        // ゲーム外のフラグをtrueに
        bOutGame = true;
    }

    return bOutGame;
}

//===========================================
// 画面内にとどまらせる(二次元)
//===========================================
bool SteyInScreen2D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pSize)
{
    // 変数宣言
    bool bScreenEdge = false;    // 画面端かどうか

    // 画面の左
    if (pPos->x < pSize->x / 2)
    {
        // とどまらせる
        pPos->x = pSize->x / 2;

        // 画面端のフラグをtrueに
        bScreenEdge = true;
    }

    // 画面の右
    if (pPos->x > SCREEN_WIDTH - pSize->x / 2)
    {
        // とどまらせる
        pPos->x = SCREEN_WIDTH - pSize->x / 2;

        // 画面端のフラグをtrueに
        bScreenEdge = true;
    }

    // 画面の上
    if (pPos->y < pSize->y / 2)
    {
        // とどまらせる
        pPos->y = pSize->y / 2;

        // 画面端のフラグをtrueに
        bScreenEdge = true;
    }

    // 画面の下
    if (pPos->y > SCREEN_HEIGHT - pSize->y / 2)
    {
        // とどまらせる
        pPos->y = SCREEN_HEIGHT - pSize->y / 2;

        // 画面端のフラグをtrueに
        bScreenEdge = true;
    }

    return bScreenEdge;
}

//===========================================
// 矩形の当たり判定2D
//===========================================
bool RectangleCollision2D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2)
{
    // 変数宣言
    bool bHit = false;    // 当たったかどうか

    // 二つのものの当たり判定を計算
    if (pPos1->x - pSize1->x / 2 <= pPos2->x + pSize2->x / 2 &&
        pPos1->x + pSize1->x / 2 >= pPos2->x - pSize2->x / 2 &&
        pPos1->y - pSize1->y / 2 <= pPos2->y + pSize2->y / 2 &&
        pPos1->y + pSize1->y / 2 >= pPos2->y - pSize2->y / 2)
    {
        // 当たったフラグをtrueに
        bHit = true;
    }

    return bHit;
}

//===========================================
// 矩形の当たり判定3D
//===========================================
bool RectangleCollision3D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2)
{
    // 変数宣言
    bool bHit = false;    // 当たったかどうか

    // 二つのものの当たり判定を計算
    if (pPos1->x - pSize1->x / 2 <= pPos2->x + pSize2->x / 2 &&
        pPos1->x + pSize1->x / 2 >= pPos2->x - pSize2->x / 2 &&
        pPos1->y - pSize1->y / 2 >= pPos2->y + pSize2->y / 2 &&
        pPos1->y + pSize1->y / 2 <= pPos2->y - pSize2->y / 2 &&
        pPos1->z - pSize1->z / 2 <= pPos2->z + pSize2->z / 2 &&
        pPos1->z + pSize1->z / 2 >= pPos2->z - pSize2->z / 2)
    {
        // 当たったフラグをtrueに
        bHit = true;
    }

    return bHit;
}

//===========================================
// 範囲内で、ランダムな数字を得る
//===========================================
int GetRandNum(int nMax, int nMin)
{
    int nRandNum = rand() % (nMax - nMin + 1) + nMin;

    return nRandNum;
}