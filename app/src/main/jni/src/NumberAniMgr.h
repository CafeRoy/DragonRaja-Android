// NumberAniMgr_New.h: Refactored interface for the CNumberAniMgr class.
#ifndef NUMBERANIMGR_NEW_H
#define NUMBERANIMGR_NEW_H

#include <memory>  // For std::unique_ptr
#include <vector>  // For a more modern approach if needed, but we'll stick to arrays for now.

// 常數定義可以保留或移入類別中作為靜態常數
#define MAX_NUMBER_ANIMATIONS 1000 // 內存池的大小，可以根據需求調整
#define MAX_HPUPDOWN 512

const int NUMBER_ANIMATION_TYPE_UP_ = 0;

const int NUMBER_ANIMATION_TYPE_DOWN_ = 1; // Health Down.,..


const int NUMBER_ANIMATION_ST_EMPTY_ = 0;

const int NUMBER_ANIMATION_ST_ANIMATING_ = 1;

const int NUMBER_ANIMATION_ST_END_ = 2;
// 重新設計的動畫節點結構
struct AnimationNode {
    // union 讓我們在節點活躍和空閒時共用記憶體
    union {
        // 狀態 1: 節點是活躍的
        struct {
            short x, y;
            short d, delay;
            int offset;
            char st;
            char type;
            int number;
            int direction;
			BYTE nDamageType;
        } data;

        // 狀態 2: 節點是空閒的, 用於指向下一個空閒節點
        AnimationNode* next_free;
    };
    // 這兩個指標用於維護"活躍"物件的雙向鏈結串列
    AnimationNode* prev_active, * next_active;
};

// 用於延遲處理HP變化的結構
struct HpUpdateJob {
    bool flag;
    int id;
    int hp;
    int attackedresult;
    int hpupdown_timer; // 更明確的命名
    unsigned char damage_type;
};

// 主要的管理類別
class CNumberAniMgr {
public:
    CNumberAniMgr();
    virtual ~CNumberAniMgr();

    CNumberAniMgr(const CNumberAniMgr&) = delete;
    CNumberAniMgr& operator=(const CNumberAniMgr&) = delete;

	void InitHpUpdateQueue();
    void CreateAnimation(BYTE DamageType, int type, int off, int x, int y, int num);
    void QueueHpUpdate(BYTE DamageType, int id, int hp, int attackedresult);
    void Update();

private:
    AnimationNode* AllocateNode();
    void FreeNode(AnimationNode* node);

    std::unique_ptr<AnimationNode[]> m_pool;
    AnimationNode* m_freeListHead;
    AnimationNode* m_activeListHead;
    HpUpdateJob m_hpUpdateQueue[MAX_HPUPDOWN];
};

// 新的全域繪製函式宣告 (用來取代舊的 DisplayChatAndNumber)
extern void DisplayChatAndNumber(void);

extern CNumberAniMgr g_pNumberAniMgr;

// 外部函式宣告
extern LPCHARACTER ReturnCharacterPoint(int id);
extern void SendReqHpOfParty(char* name);
extern void CharDoAni(LPCHARACTER ch, int dir, int ani);
extern void SkillSettingClear();
extern int  BoxAndDotCrash(int mapX, int mapY, int screenXSize, int screenYSize, int targetX, int targetY);
extern void DisplayChatMessage(LPCHARACTER ch);
extern void Getcolor(int* r, int* g, int* b);
extern void Hcolor(int r, int g, int b);
extern void Hprint(int x, int y, char* dest, const char* format);
extern void Hprint2(int x, int y, char* dest, char* format, ...);
extern void HprintBold(int x, int y, const int color, const int bcolor, char* format, ...);
#endif // NUMBERANIMGR_NEW_H