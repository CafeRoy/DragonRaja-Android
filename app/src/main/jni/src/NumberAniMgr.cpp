// NumberAniMgr_New.cpp: Implementation of the refactored CNumberAniMgr class.
#include "stdafx.h" // 根據你的專案包含必要的標頭檔
#include "NumberAniMgr.h"
#include "Menu.h"
#include "dragon.h"
#include "gameproc.h"
#include "MenuSet.h"
#include "Chat.h"
#include "Skill.h"
#include "CurrentMsgMgr.h"
#include "Hong_Sub.h"
#include "char.h"
#include "Tool.h"
#include "Hangul.h"
#include "CharDataTable.h"
#include "Battle.h"
#include "smallmenu.h"

// ... 包含其他需要的標頭檔，如繪圖相關函式等
CNumberAniMgr g_pNumberAniMgr; // 全局指標，方便在其他地方使用
// =======================================================================
// CNumberAniMgr 類別實作
// =======================================================================

// 簡化顏色判斷的查表法
struct ColorRange {
    int maxNumber;
    int r, g, b;
};

// 傷害數字顏色表
static const std::vector<ColorRange> damageColors = {
    {50, 0, 255, 95}, {75, 0, 255, 135}, {100, 0, 255, 175},
    {180, 0, 255, 215}, {250, 0, 248, 255}, {300, 0, 208, 255},
    {400, 0, 168, 255}, {500, 0, 128, 255}, {600, 0, 88, 255},
    {730, 20, 8, 255}, {870, 60, 8, 255}, {1000, 100, 8, 255},
    {1150, 140, 8, 255}, {1350, 180, 8, 255}, {1500, 225, 8, 255},
    {1700, 255, 0, 160}, {1900, 255, 0, 120}, {2100, 255, 5, 80},
    {2330, 255, 5, 40}, {2560, 255, 40, 8}, {2800, 255, 80, 8},
    {3200, 255, 120, 8}, {3600, 255, 160, 8}, {4000, 255, 200, 8},
    {99999, 255, 255, 8}
};

// 治療數字顏色表
struct BoldColorRange {
    int maxNumber;
    DWORD color;
    DWORD bcolor;
};

static const std::vector<BoldColorRange> healColors = {
    {50, RGB(0, 255, 95), RGB(0, 185, 5)}, {75, RGB(0, 255, 135), RGB(0, 185, 55)},
    {100, RGB(0, 255, 175), RGB(0, 185, 95)}, {180, RGB(0, 255, 215), RGB(0, 185, 125)},
    {250, RGB(0, 248, 255), RGB(0, 188, 125)}, {300, RGB(0, 208, 255), RGB(0, 148, 125)},
    {400, RGB(0, 168, 255), RGB(0, 88, 125)}, {500, RGB(0, 128, 255), RGB(0, 48, 155)},
    {600, RGB(0, 88, 255), RGB(0, 8, 125)}, {730, RGB(20, 8, 255), RGB(0, 8, 155)},
    {870, RGB(60, 8, 255), RGB(0, 8, 155)}, {1000, RGB(100, 8, 255), RGB(40, 8, 155)},
    {1150, RGB(140, 8, 255), RGB(0, 8, 155)}, {1350, RGB(180, 8, 255), RGB(40, 8, 155)},
    {1500, RGB(225, 8, 255), RGB(185, 8, 155)}, {1700, RGB(255, 0, 160), RGB(185, 0, 60)},
    {1900, RGB(255, 0, 120), RGB(185, 0, 20)}, {2100, RGB(255, 5, 80), RGB(185, 5, 0)},
    {2330, RGB(255, 5, 40), RGB(185, 5, 0)}, {2560, RGB(255, 40, 8), RGB(185, 0, 8)},
    {2800, RGB(255, 80, 8), RGB(185, 20, 8)}, {3200, RGB(255, 120, 8), RGB(185, 60, 8)},
    {3600, RGB(255, 160, 8), RGB(185, 40, 8)}, {4000, RGB(255, 200, 8), RGB(185, 140, 8)},
    {99999, RGB(255, 255, 8), RGB(185, 180, 8)}
};

CNumberAniMgr::CNumberAniMgr()
    : m_pool(std::make_unique<AnimationNode[]>(MAX_NUMBER_ANIMATIONS)),
    m_freeListHead(nullptr),
    m_activeListHead(nullptr)
{
    m_freeListHead = &m_pool[0];
    for (size_t i = 0; i < MAX_NUMBER_ANIMATIONS - 1; ++i) {
        m_pool[i].next_free = &m_pool[i + 1];
    }
    m_pool[MAX_NUMBER_ANIMATIONS - 1].next_free = nullptr;

    for (int i = 0; i < MAX_HPUPDOWN; ++i) {
        m_hpUpdateQueue[i].flag = false;
    }
}

CNumberAniMgr::~CNumberAniMgr() {}

AnimationNode* CNumberAniMgr::AllocateNode() {
    if (!m_freeListHead) return nullptr;
    AnimationNode* node = m_freeListHead;
    m_freeListHead = node->next_free;
    return node;
}

void CNumberAniMgr::FreeNode(AnimationNode* node) {
    if (!node) return;
    node->next_free = m_freeListHead;
    m_freeListHead = node;
}

void CNumberAniMgr::InitHpUpdateQueue()
{
    for (int i = 0; i < MAX_HPUPDOWN; ++i) {
        m_hpUpdateQueue[i].flag = false;
    }
}

void CNumberAniMgr::CreateAnimation(BYTE DamageType, int type, int off, int x, int y, int num) {
    //return;

    AnimationNode* newNode = AllocateNode();
    if (!newNode) return;

    static int s_animation_counter = 0;
    int direction = (s_animation_counter % 2 == 0) ? 1 : -1; // 偶數向右，奇數向左
    s_animation_counter++;

    // 遍歷所有活躍中的動畫節點，檢查它們的位置
    int stack_offset = 0;
    AnimationNode* current = m_activeListHead;
    while (current != nullptr) {
        // 如果這個活躍中的動畫節點與我們即將創建的新動畫在X座標上非常接近
        if (std::abs(current->data.x - (x + 20)) < 16) { // 假設X座標相差小於30像素視為重疊
            // 則增加堆疊偏移量，讓新數字從更高處開始
            stack_offset += 3; // 每個數字向上偏移20像素
        }
        current = current->next_active;
    }

    newNode->data.st = NUMBER_ANIMATION_ST_ANIMATING_;
    newNode->data.delay = 40;
    newNode->data.number = num;
    newNode->data.offset = off;
    newNode->data.type = type;
    newNode->data.x = x;
    newNode->data.y = y + 10 + stack_offset;
    newNode->data.d = 20;
    newNode->data.direction = direction; // 儲存移動方向
	newNode->data.nDamageType = static_cast<BYTE>(DamageType);

    newNode->next_active = m_activeListHead;
    newNode->prev_active = nullptr;
    if (m_activeListHead) {
        m_activeListHead->prev_active = newNode;
    }
    m_activeListHead = newNode;
}

void CNumberAniMgr::QueueHpUpdate(BYTE DamageType, int id, int hp, int attackedresult) {
    for (int i = 0; i < MAX_HPUPDOWN; ++i) {
        if (!m_hpUpdateQueue[i].flag) {
            m_hpUpdateQueue[i].id = id;
            m_hpUpdateQueue[i].hp = hp;
            m_hpUpdateQueue[i].attackedresult = attackedresult;
            m_hpUpdateQueue[i].hpupdown_timer = 8;
            m_hpUpdateQueue[i].flag = true;
            m_hpUpdateQueue[i].damage_type = DamageType;
            return;
        }
    }
}

void CNumberAniMgr::Update() {
    // 1. 處理HP更新佇列 (移植自 CheckHpUpDown)
    for (int i = 0; i < MAX_HPUPDOWN; ++i) {
        if (!m_hpUpdateQueue[i].flag) continue;
        m_hpUpdateQueue[i].hpupdown_timer--;
        if (m_hpUpdateQueue[i].hpupdown_timer <= 0) {
            HpUpdateJob& job = m_hpUpdateQueue[i];
            job.flag = false;
            LPCHARACTER ch = ReturnCharacterPoint(job.id);
            if (!ch) continue;

            SendReqHpOfParty(ch->name);
            this->CreateAnimation(m_hpUpdateQueue[i].damage_type, NUMBER_ANIMATION_TYPE_DOWN_, ch->y + 1, ch->x, ch->y - 44, job.hp);

            switch (job.attackedresult) {
            case 1: {
                if (ch == Hero) {
                    SCharacterData.nCharacterHP = 0;
                    SCharacterData.nCharacterMP = 0;
                    SCharacterData.condition = CON_DEATH;
                    Hero->hp = SCharacterData.nCharacterHP;
                }
                ch->hp = 0; ch->mp = 0; ch->condition = CON_DEATH;
                int ani = 0;
                switch (ch->animationType) {
                case ANIMATION_TYPE_MAN: case ANIMATION_TYPE_WOMAN: ani = ACTION_DIE; break;
                case ANIMATION_TYPE_MON1: ani = MON1_DIE; break;
                }
                if (ani != ch->nCurrentAction) CharDoAni(ch, ch->direction, ani);
                SkillSettingClear();
                ch->dummy_pk = 0;
            } break;
            case 0: ch->NumberAnimationValue.PutEncrypted(0); break;
            case 2: {
                if (ch == Hero) {
                    ch->hp += job.hp;
                    SCharacterData.nCharacterHP += job.hp;
                    if (SCharacterData.nCharacterHP < 0) {
                        SCharacterData.nCharacterHP = 1;
                        Hero->hp = SCharacterData.nCharacterHP;
                    }
                }
            } break;
            }
            ch->NumberAnimationValue.PutEncrypted(0);
        }
    }

    // 2. 更新所有活躍的動畫 (移植自 OutputNumberAni)
    AnimationNode* current = m_activeListHead;
    while (current != nullptr) {
        AnimationNode* next = current->next_active;
        bool animationEnded = false;

        switch (current->data.type) {
        case NUMBER_ANIMATION_TYPE_DOWN_:
            // 根據方向變數，調整 x 座標的移動
            current->data.x += current->data.direction;
            current->data.y -= current->data.d;
            current->data.d -= 15;
            if (current->data.d < 2) current->data.d = 1;
            break;
        case NUMBER_ANIMATION_TYPE_UP_:
            // 治療數字也一樣，根據方向移動 x 座標
            //current->data.x += current->data.direction;
            if (current->data.d > 1) current->data.y -= current->data.d;
            current->data.d -= 6;
            if (current->data.d < 2) current->data.d = 1;
            break;
        }

        current->data.delay--;
        if (current->data.delay <= 0) {
            animationEnded = true;
        }

        if (animationEnded) {
            if (current->prev_active) current->prev_active->next_active = current->next_active;
            else m_activeListHead = current->next_active;
            if (current->next_active) current->next_active->prev_active = current->prev_active;
            FreeNode(current);
        }
        else {
            if (BoxAndDotCrash(Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, current->data.x, current->data.y)) {
                if (g_OrderInfo.count < MAX_ORDERTABLE) {
                    LPORDERTABLE o = &g_OrderInfo.order[g_OrderInfo.count++];
                    o->wType = SPRITETYPE_NUMBER;
                    o->x = current->data.x; o->y = current->data.y; o->height = 0;
                    o->offset = current->data.offset;
                    o->lpvData = (LPVOID)current;
                    o->show = 1; o->sort = 1;
                }
            }
        }
        current = next;
    }
}

// =======================================================================
// 新的全域繪製函式 (移植自 DisplayChatAndNumber)
// =======================================================================
void DisplayChatAndNumber(void) {
    DWORD color, bcolor;
    int number;
    int r, g, b;
    for (int i = 0; i < g_OrderInfo.count; i++) {
        switch (g_OrderInfo.order[i].wType) {
        case SPRITETYPE_CHARACTER:
        case SPRITETYPE_MONSTER:
        case SPRITETYPE_NPC:
        {
            LPCHARACTER ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
            ::DisplayChatMessage(ch);
        }
        break;
        case SPRITETYPE_NUMBER:
        {
            if (!system_info.counter) continue;

            LPORDERTABLE o = &g_OrderInfo.order[i];
            //o->show = 0;
            if (tool_ViewNumber) { break; }

            // *** 修改點 1: 轉型為新的 AnimationNode* ***
            AnimationNode* n = (AnimationNode*)o->lpvData;
            number = abs(n->data.number);
            // *** 修改點 2: 所有成員存取都透過 n->data.xxx ***
            switch (n->data.type) {
            case NUMBER_ANIMATION_TYPE_DOWN_:
            {
                if (number == 0) {
                    Getcolor(&r, &g, &b);
                    Hcolor(0, 0xaa, 0);

                    switch (n->data.nDamageType)
                    {
                    case battle::DAMAGE_TYPE_BLOCKED: // 格擋
                    {
                        Hprint2(n->data.x - Mapx - 30, n->data.y - Mapy,
                            g_DestBackBuf, "🛡️ Miss");
                        break;
                    }
                    case battle::DAMAGE_TYPE_IMMUNE: // 無效
                    {
                        Hprint2(n->data.x - Mapx - 30, n->data.y - Mapy,
                            g_DestBackBuf, "🚫 Miss");
                        break;
                    }
                    default:
                        Hprint(n->data.x - Mapx - 30, n->data.y - Mapy, g_DestBackBuf, "Miss");
                        break;
                    }
                    Hcolor(r, g, b); // 恢復顏色
                }
                else {
					// 根據傷害數字大小選擇顏色
                    const ColorRange* selectedColor = &damageColors.back();
                    for (const auto& color : damageColors) {
                        if (number <= color.maxNumber) {
                            selectedColor = &color;
                            break;
                        }
                    }
                    Hcolor(selectedColor->r, selectedColor->g, selectedColor->b);
                    // 在 switch 語句上方或類別中定義一個 char* 變數來儲存格式字串
                    char* formatString = nullptr;

                    switch (n->data.nDamageType)
                    {
                    case battle::DAMAGE_TYPE_CRITICAL: // 會心一擊
                        formatString = "🔥 %d";
                        break;

                    case battle::DAMAGE_TYPE_PHYSICAL_PIERCING: // 物理穿刺
                        formatString = "💥 %d";
                        break;

                    case battle::DAMAGE_TYPE_MAGIC_PIERCING: // 魔法穿刺
                        formatString = "✨ %d";
                        break;

                    case battle::DAMAGE_TYPE_BLOCKED: // 格擋
                        formatString = "🛡️ %d";
                        break;

                    case battle::DAMAGE_TYPE_RESIST: // 抗性
                        formatString = "🛡️ %d";
                        break;

                    default:
                    case battle::DAMAGE_TYPE_NORMAL: // 普通傷害
                        // 如果是普通傷害，直接使用 "%d"
                        formatString = "%d";
                        break;
                    }
                    Hprint2(n->data.x - Mapx - 30, n->data.y - Mapy-16, g_DestBackBuf, formatString, number);
                    
                }
            }
            break;
            case NUMBER_ANIMATION_TYPE_UP_:
            {
                const BoldColorRange* selectedColor = &healColors.back();
                for (const auto& color : healColors) {
                    if (number <= color.maxNumber) {
                        selectedColor = &color;
                        break;
                    }
                }
                HprintBold(n->data.x - Mapx - 30, n->data.y - Mapy, 
                    selectedColor->color, selectedColor->bcolor, "%d", number);
            }
            break;
            }
        }
        break;
        }
    }
}