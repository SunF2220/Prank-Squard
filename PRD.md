# Prank Squad — 产品需求文档 (PRD)

## 1. 项目概述

### 1.1 简介
Prank Squad 是一款致敬《小朋友齐打交》(Little Fighter 2) 的 2.5D 火柴人格斗游戏，使用 **Godot 4.3+ 引擎 + C++ GDExtension** 开发。

### 1.2 核心理念
1. **数据驱动帧系统** — 所有角色行为由 `FrameData` 定义，核心引擎逐帧解释执行
2. **C++ 高性能引擎** — 帧解释器、战斗系统、AI 等性能关键部分用 C++ 实现
3. **文本生成角色** — 用户输入文字描述（如"一个会扔火球的魔法师"），系统自动生成角色形象、属性和招式

### 1.3 目标平台
- Windows (首发)
- 后续可扩展至 Linux / macOS

---

## 2. 游戏设计

### 2.1 核心玩法循环

```
主菜单 → 角色/模式选择 → 战斗开始 → 回合结算 → 下一回合/结束
```

### 2.2 操作方式

| 动作 | P1 (键盘) | P2 (键盘) |
|------|-----------|-----------|
| 左移 | A | ← |
| 右移 | D | → |
| 上移 (纵深) | W | ↑ |
| 下移 (纵深) | S | ↓ |
| 攻击 | J | 1 (小键盘) |
| 跳跃 | K | 2 (小键盘) |
| 防御 | L | 3 (小键盘) |

### 2.3 基础操作
- **移动**：方向键控制角色在 XZ 平面移动
- **攻击**：按攻击键发动普通攻击
- **连击**：连续按攻击键触发连段
- **跳跃**：跳跃，空中可控制方向和攻击
- **防御**：按住防御减少伤害
- **翻滚/闪避**：防御 + 方向键，短暂无敌帧
- **必杀技**：特定方向组合 + 攻击键（如 →↓→+攻击=升龙拳）

### 2.4 属性系统
- **HP (生命值)**：归零即死亡
- **MP (魔力值)**：使用必杀技消耗，随时间自动回复
- **重量**：影响受击击退距离

### 2.5 游戏模式

| 模式 | 说明 | 优先级 |
|------|------|--------|
| VS Mode | 1v1 / 2v2 本地对战 | P0 |
| Stage Mode | 闯关模式，波次击败 AI 敌人 | P1 |
| Championship | 淘汰赛制，AI 对手 | P2 |
| Training | 练习模式，木桩 | P2 |

---

## 3. 特色功能：文字生成角色

这是 Prank Squad 区别于传统 LF2 复刻版的**最大特色**。

### 3.1 用户流程

```
1. 用户在界面输入文字描述:
   "一个冰系忍者，身法灵活，能扔出冰冻手里剑，大招是召唤暴风雪"

2. 系统解析文字，生成角色预览:
   ┌──────────────────────────────┐
   │  角色: 冰忍                   │
   │  HP: 350  MP: 600  速度: 高   │
   │  招式:                        │
   │  · 普攻: 快速拳击 (3段连击)    │
   │  · 必杀1: 冰冻手里剑 (↓→+攻)  │
   │  · 必杀2: 冰影瞬移 (→↓→+攻)   │
   │  · 大招: 暴风雪 (↓↑+攻)       │
   │                              │
   │  [确认生成]  [重新描述]        │
   └──────────────────────────────┘

3. 用户确认 → 角色资源自动创建
4. 该角色立即可用于对战
```

### 3.2 文本解析引擎

```
输入 → 分词 → 关键词提取 → 模板匹配 → 参数填充 → 角色数据输出

输入文本: "一个会扔火球的魔法师"
  ↓
分词: [一个, 会, 扔, 火球, 的, 魔法师]
  ↓
关键词匹配:
  职业:  魔法师 → archetype = mage
  元素:  火球   → element = fire
  武器:  (无)   → weapon = none
  特征:  远程   → range_type = ranged
  ↓
模板匹配 → MageTemplate (魔法师模板)
  ↓
参数填充:
  stats:    HP=350, MP=700, speed=4
  moves:    火球术 (→↓→+攻击),  火焰爆发 (↓→+攻击)
  visuals:  红色袍子, 火焰特效
  ↓
生成 CharacterData + FrameData + 程序化精灵
```

### 3.3 模板系统架构

每种"职业原型"是一个模板：

| 模板 | 描述 | 典型属性倾向 | 示例招式 |
|------|------|-------------|---------|
| Brawler | 格斗家 | 高HP, 中MP, 高速 | 拳击连段, 升龙拳 |
| Mage | 魔法师 | 低HP, 高MP, 低速 | 元素弹, 范围法术 |
| Ninja | 忍者 | 低HP, 中MP, 极速 | 投掷武器, 瞬移 |
| Swordsman | 剑士 | 中HP, 中MP, 中速 | 剑气斩, 突进刺 |
| Archer | 弓箭手 | 低HP, 中MP, 中速 | 多重箭, 穿透箭 |
| Tank | 重装战士 | 高HP, 低MP, 低速 | 冲击波, 大地震 |

**元素修饰**：原型 × 元素 = 多种变体
- 火 + Mage = 火法师（火焰弹, 爆炸, 灼烧效果）
- 冰 + Mage = 冰法师（冰冻弹, 减速, 冰墙）
- 雷 + Ninja = 雷忍者（高速, 电击, 麻痹效果）

### 3.4 程序化精灵生成

根据解析出的角色数据，**自动绘制火柴人精灵**：

```
角色描述 → 参数化火柴人绘制

可参数化维度:
  体型:  HeadSize / BodyWidth / LimbLength / Height
  配色:  PrimaryColor / SecondaryColor / AccentColor
  装备:  WeaponType / ArmorStyle / Headgear
  特效:  ElementalAura / TrailEffect
  姿态:  IdlePose / WalkStyle / AttackStyle

绘制流程:
  1. 根据 archetype 选择基础骨架模板
  2. 根据 stats 调整体型 (tank=粗壮, ninja=瘦小)
  3. 根据 color 方案着色
  4. 根据武器类型添加装备图形
  5. 根据招式数据自动生成动作帧的骨骼姿势
```

### 3.5 LLM 集成（可选增强）

```
┌──────────┐    ┌──────────────┐    ┌────────────┐
│ 用户输入  │───▶│   LLM API    │───▶│  结构化    │
│ 自然语言  │    │ (Ollama/OpenAI) │    │ JSON 输出  │
└──────────┘    └──────────────┘    └──────┬─────┘
                                           ▼
                                    ┌──────────────┐
                                    │   C++ 引擎    │
                                    │  → 生成角色数据 │
                                    │  → 平衡性校验  │
                                    └──────────────┘
```

LLM 负责：自然语言 → 结构化 JSON（角色属性、招式列表）
C++ 引擎负责：JSON → 实际的 `CharacterData` / `FrameData` / 精灵资源

本地运行 (Ollama) 或云端 API 均可，通过插件化接口切换。

---

## 4. 技术架构

### 4.1 语言分层策略

```
┌──────────────────────────────────────────────┐
│             GDScript 层 (UI/场景/流程)         │
│  MainMenu  CharSelect  HUD  GameOver  Stage  │
├──────────────────────────────────────────────┤
│           GDScript ↔ GDExtension 桥接         │
├──────────────────────────────────────────────┤
│              C++ GDExtension 核心层            │
│  FrameInterpreter  CombatSystem  InputBuffer  │
│  CharacterGenerator  AIEngine  SpriteGen     │
├──────────────────────────────────────────────┤
│              C++ 数据结构层                    │
│  FrameData  ItrData  BdyData  CharacterData   │
│  PoseData  AnimationData  PatternData         │
└──────────────────────────────────────────────┘
```

**分层理由：**

| 层次 | 语言 | 原因 |
|------|------|------|
| 数据结构 | C++ | 每帧访问百万次，内存布局需紧凑 |
| 帧解释器 | C++ | 60fps 每角色每帧调用，性能关键 |
| 战斗系统 | C++ | 双重循环 (攻击者×目标×itr×bdy) |
| 文本生成 | C++ | 文本解析 + 模板匹配 + 资源生成 |
| AI 决策 | C++ | 每帧决策循环，需要与帧解释器紧耦合 |
| UI / 场景管理 | GDScript | 迭代快，与引擎场景系统集成好 |
| 游戏流程控制 | GDScript | 模式切换、回合管理、状态编排 |

### 4.2 总体架构

```
                    GDScript Layer
┌───────────────────────────────────────────────────────┐
│                     Game Controller                    │
│              (回合管理 / 模式切换 / 场景编排)            │
└──────────┬────────────┬──────────────┬────────────────┘
           │            │              │
     ┌─────▼──────┐ ┌──▼────────┐ ┌──▼──────────────┐
     │ HUD (GDS)  │ │  Stages   │ │  CharacterSelect │
     └────────────┘ │  (GDS)    │ │  (GDS)           │
                    └───────────┘ └──────────────────┘

                    C++ GDExtension Layer
┌───────────────────────────────────────────────────────┐
│                    World Manager                       │
├──────────┬──────────┬──────────┬──────────────────────┤
│ Fighter  │ Combat   │  Camera  │  ProjectileManager   │
│ (C++)    │ (C++)    │  (C++)   │  (C++)               │
├──────────┴──────────┴──────────┴──────────────────────┤
│               FrameInterpreter (C++)                   │
│   ┌─────────┐  ┌──────────┐  ┌───────────────────┐   │
│   │FrameData│  │InputBuf  │  │StateMachine       │   │
│   │ (C++ )  │  │ (C++)    │  │ (C++)             │   │
│   └─────────┘  └──────────┘  └───────────────────┘   │
├───────────────────────────────────────────────────────┤
│              CharacterGenerator (C++)                  │
│   ┌──────────┐  ┌──────────┐  ┌──────────────────┐   │
│   │TextParser│  │Template  │  │ProceduralSprite  │   │
│   │  (C++)   │  │Matcher   │  │Generator (C++)   │   │
│   └──────────┘  └──────────┘  └──────────────────┘   │
└───────────────────────────────────────────────────────┘
```

### 4.3 GDExtension 架构

```
Prank Squad/
├── godot-cpp/              # Godot C++ 绑定 (git submodule)
├── src/cpp/gdextension/    # GDExtension 入口
│   ├── register_types.cpp  # 注册所有 C++ 类到 Godot
│   └── register_types.h
├── src/cpp/core/           # C++ 核心实现
│   ├── frame_interpreter.h/.cpp
│   ├── input_buffer.h/.cpp
│   ├── combat_system.h/.cpp
│   ├── fighter.h/.cpp
│   ├── state_machine.h/.cpp
│   └── data_types.h       # FrameData, ItrData, BdyData 等 struct
├── src/cpp/generator/      # 文本生成角色引擎
│   ├── text_parser.h/.cpp
│   ├── template_matcher.h/.cpp
│   ├── character_generator.h/.cpp
│   ├── procedural_sprite.h/.cpp
│   ├── pose_engine.h/.cpp
│   └── balance_validator.h/.cpp
├── src/cpp/ai/             # AI 系统
│   └── ai_controller.h/.cpp
├── SConstruct              # SCons 构建脚本
└── src/gdscript/           # GDScript 脚本 (组织在子目录)
    ├── core/
    │   ├── game.gd
    │   ├── game.tscn
    │   └── camera.gd
    ├── fighter/
    │   └── fighter_node.gd     # Fighter 的 Godot 节点封装
    ├── ui/
    │   ├── main_menu.gd/tscn
    │   ├── character_select.gd/tscn
    │   ├── character_creator.gd/tscn   # 文字创建角色界面
    │   └── hud.gd
    └── stages/
        ├── stage.gd
        └── stage_data.gd
```

### 4.4 C++ 数据类设计

```cpp
// data_types.h — 核心数据结构 (POD 类型, 紧凑内存布局)

struct ItrData {
    int16_t x, y, z;           // 相对位置 (2字节有符号)
    int16_t width, height, depth; // 判定框大小
    int16_t damage;             // 伤害值
    int16_t dvx, dvy, dvz;      // 击退力
    uint8_t effect_type;        // 元素类型 (0=普通,1=火,2=冰...)
    uint8_t fall;               // 击倒方向
    bool blockable;
    bool ground_only;
    bool aerial_only;
};

struct BdyData {
    int16_t x, y, z;
    int16_t width, height, depth;
};

struct FrameData {
    int16_t pic;                // 精灵帧索引
    int16_t state;              // 状态ID
    int16_t wait;               // 停留帧数
    int16_t next;               // 跳转目标
    int16_t dvx, dvy, dvz;      // 速度增量
    int16_t mp;                 // 魔力变化
    int16_t shake;              // 屏幕震动
    int32_t sound_id;           // 音效ID

    // 输入跳转
    int16_t hit_a, hit_j, hit_d;
    int16_t hit_Fa, hit_Ua, hit_Da;
    int16_t hit_Fj, hit_Uj, hit_Dj;

    // 判定框 (内联数组, 避免动态分配)
    static const int MAX_ITRS = 4;
    static const int MAX_BDYS = 4;
    ItrData itrs[MAX_ITRS];
    BdyData bdys[MAX_BDYS];
    int itr_count;
    int bdy_count;

    // 特殊对象
    int16_t opoint_id;          // 生成对象ID (-1=无)
    int16_t opoint_x, opoint_y, opoint_z;
    int16_t opoint_dvx, opoint_dvy, opoint_dvz;

    bool hurtable;
    bool is_return_to_idle() const { return next == 999; }
};
```

### 4.5 帧解释器核心 (C++)

```cpp
class FrameInterpreter : public Node {
    GDCLASS(FrameInterpreter, Node);

    int current_frame_index;
    const FrameData* current_frame;
    int frame_timer;
    Vector<const FrameData*> frame_table;  // 快速帧查找表

    // 每帧执行
    void update(Fighter* fighter, float delta) {
        // 1. 应用速度增量
        fighter->apply_velocity(current_frame->dvx, current_frame->dvy, current_frame->dvz);

        // 2. 检测输入跳转
        int transition = check_input_transitions(fighter);
        if (transition >= 0) {
            transition_to(transition);
            return;
        }

        // 3. 自动过渡
        frame_timer++;
        if (current_frame->wait > 0 && frame_timer >= current_frame->wait) {
            if (current_frame->is_return_to_idle()) {
                fighter->change_state("idle");
            } else {
                transition_to(current_frame->next);
            }
        }

        // 4. 激活判定框
        fighter->set_active_itrs(current_frame->itrs, current_frame->itr_count);
        fighter->set_active_bdys(current_frame->bdys, current_frame->bdy_count);
    }

    // 输入匹配 (C++ 指针遍历, 无虚函数调用)
    int check_input_transitions(Fighter* fighter) {
        InputBuffer* input = fighter->get_input_buffer();

        // 优先检测必杀技指令
        int special = input->detect_special();
        if (special >= 0) return special;

        // 检测方向+按键组合
        if (current_frame->hit_Fa >= 0 && input->is_dir("FRONT") && input->attack_just_pressed())
            return current_frame->hit_Fa;
        // ... 更多组合
    }
};
```

### 4.6 文本生成引擎 (C++)

```cpp
class CharacterGenerator : public RefCounted {
    GDCLASS(CharacterGenerator, RefCounted);

    // 关键词 → 参数映射表
    HashMap<String, ArchetypeParams> archetype_map;
    HashMap<String, ElementParams> element_map;
    HashMap<String, WeaponParams> weapon_map;

    // 主入口
    Ref<CharacterData> generate_from_text(const String& description);

    // 解析步骤
    ParsedIntent parse_text(const String& text);
    ArchetypeParams match_archetype(const ParsedIntent& intent);
    Ref<CharacterData> apply_template(const ArchetypeParams& archetype,
                                       const ParsedIntent& intent);
    void generate_frame_data(CharacterData* data, const MoveTemplate& moves);
    void generate_sprites(CharacterData* data, const VisualParams& visuals);
    void validate_balance(CharacterData* data);
};
```

### 4.7 GDScript ↔ C++ 桥接

```gdscript
# fight_node.gd — Fighter 的 Godot 节点封装
extends CharacterBody2D

var cpp_fighter: FighterCpp            # C++ Fighter 对象
var interpreter: FrameInterpreterCpp   # C++ 帧解释器
var input_buffer: InputBufferCpp       # C++ 输入缓冲

func _ready():
    # 创建 C++ 对象
    cpp_fighter = FighterCpp.new()
    interpreter = FrameInterpreterCpp.new()
    input_buffer = InputBufferCpp.new()

    # 加载角色数据 (C++ 资源)
    var data: CharacterDataCpp = load("user://generated/my_ninja.tres")
    cpp_fighter.set_character_data(data)
    interpreter.set_frame_table(data.get_frames())

func _process(delta):
    # 收集 GDScript 输入
    input_buffer.record(direction, attack, jump, defend)

    # 委托 C++ 引擎执行
    interpreter.update(cpp_fighter)
    cpp_fighter.apply_physics(delta)
    update_display(cpp_fighter.get_sprite_frame())
```

### 4.8 程序化精灵生成

```
火柴人绘制管线 (C++ → Godot Image → Texture2D):

1. 根据角色 archetype 生成骨架:
   - 头: 圆形 (半径 = head_size)
   - 躯干: 线段 (shoulder → hip)
   - 四肢: 带关节的折线 (shoulder → elbow → hand)

2. 每帧动作插值:
   - idle: 轻微呼吸起伏
   - walk: 四肢交替摆动 (正弦波)
   - punch: 手臂前伸 + 躯干旋转
   - kick: 腿部前踢 + 身体后倾
   - special: 根据元素不同 (火焰环绕/冰晶聚集)

3. 渲染到 Godot Image:
   - 描边: 3px 黑色线条
   - 填充: primary_color
   - 装备图形: 叠加绘制 (武器/盔甲/头饰)
   - 特效粒子: 在攻击帧叠加元素粒子

4. 生成 SpriteFrames 资源:
   - 所有帧合并为 AtlasTexture
   - 存入 CharacterData 的 sprite_sheet 字段
```

---

## 5. 核心系统设计

### 5.1 帧解释器 (FrameInterpreter)

```
每帧执行:
  1. 应用 dvx/dvy/dvz 到角色速度 (C++ 整数运算)
  2. 检查输入跳转条件 (hit_* 表查找)
  3. wait 计数 +1 → 判断自动过渡
  4. 激活 itr/bdy 判定框 (memcpy 到 Fighter 的激活列表)
  5. 触发 opoint 生成 / 音效 / 特效
```

### 5.2 输入缓冲 + 必杀技识别

```
InputBuffer (C++ 环形缓冲区 64 帧):

struct InputSnapshot {
    uint8_t directions;    // 位掩码: UP|DOWN|LEFT|RIGHT
    uint8_t buttons;       // 位掩码: ATTACK|JUMP|DEFEND
    uint8_t just_pressed;  // 位掩码: 刚按下
};

环形缓冲区 + 滑动窗口模式匹配:
  - 注册指令模式 (PatternData 数组)
  - 每帧从最新帧开始回溯匹配
  - 匹配成功 → 返回模式 ID → 帧解释器跳转
```

### 5.3 2.5D 坐标系统

```cpp
struct WorldPosition {
    int32_t x;      // 水平轴 (-2000 ~ 2000)
    int32_t y;      // 跳跃高度 (-500 ~ 500)
    int32_t z;      // 纵深 (-200 ~ 200)

    // 2D 投影
    Vector2 to_screen() const {
        return Vector2(x, z * Z_SCALE - y);
    }

    // Z 轴排序
    int32_t get_sort_order() const { return -z; }
};
```

### 5.4 碰撞检测系统

```
每帧 (CombatSystem C++):
  1. 遍历所有存活 Fighter
  2. 对于每个有激活 itr 的攻击者:
     遍历其他 Fighter 的激活 bdy:
       if (AABB overlap):
         if (Y轴高度范围重叠):
           应用伤害/击退/元素效果
           触发命中停顿 (全局帧冻结)
           生成命中特效
           更新连击计数器
```

### 5.5 状态机架构

```cpp
class FighterStateMachine {
    State states[MAX_STATES];    // 状态数组 (固定大小, 无动态分配)
    int current_state;

    void transition(int new_state) {
        states[current_state].exit(fighter);
        current_state = new_state;
        states[current_state].enter(fighter);
    }

    void update() {
        states[current_state].update(fighter, interpreter);
    }
};
```

C++ 状态机 vs GDScript 状态机的优势：
- 状态切换是 O(1) 数组索引，不是 HashMap 查找
- 状态更新是直接函数指针调用，不是虚函数
- 数据局部性好，缓存友好

---

## 6. 目录结构

```
Prank Squad/
├── project.godot                       # 项目配置
├── .editorconfig                       # 代码风格
├── .gitignore
├── PRD.md                              # 本文档
├── SConstruct                          # C++ 构建脚本
│
├── godot-cpp/                          # Godot C++ 绑定 (submodule)
│
├── src/
│   ├── cpp/                            # C++ 源码
│   │   ├── gdextension/
│   │   │   ├── register_types.cpp      # GDExtension 入口
│   │   │   ├── register_types.h
│   │   │   └── PrankSquad.gdextension   # GDExtension 配置文件
│   │   │
│   │   ├── core/                       # 核心引擎
│   │   │   ├── data_types.h            # FrameData/ItrData/BdyData struct
│   │   │   ├── frame_interpreter.h/.cpp
│   │   │   ├── input_buffer.h/.cpp
│   │   │   ├── combat_system.h/.cpp
│   │   │   ├── fighter.h/.cpp          # Fighter 逻辑 (无节点依赖)
│   │   │   ├── state_machine.h/.cpp
│   │   │   └── world_position.h        # 2.5D 坐标
│   │   │
│   │   ├── generator/                  # 文本生成角色
│   │   │   ├── text_parser.h/.cpp      # NLP 分词/关键词提取
│   │   │   ├── template_matcher.h/.cpp # 模板匹配引擎
│   │   │   ├── archetype_templates.h   # 职业模板数据
│   │   │   ├── character_generator.h/.cpp  # 主生成器
│   │   │   ├── procedural_sprite.h/.cpp    # 程序化精灵生成
│   │   │   ├── pose_engine.h/.cpp      # 骨骼动作引擎
│   │   │   └── balance_validator.h/.cpp    # 平衡性校验
│   │   │
│   │   ├── ai/
│   │   │   ├── ai_controller.h/.cpp
│   │   │   └── ai_behaviors.h
│   │   │
│   │   └── audio/
│   │       └── audio_manager.h/.cpp    # 音效管理
│   │
│   └── gdscript/                       # GDScript 脚本
│       ├── core/
│       │   ├── game.gd                 # 主游戏控制器
│       │   ├── game.tscn
│       │   └── camera_2d.gd
│       │
│       ├── fighter/
│       │   ├── fighter_node.gd         # Fighter 的 Godot 节点封装
│       │   └── fighter_node.tscn
│       │
│       ├── systems/
│       │   └── projectile_manager.gd   # 飞行道具有限状态
│       │
│       ├── ui/
│       │   ├── main_menu.gd
│       │   ├── main_menu.tscn
│       │   ├── character_select.gd
│       │   ├── character_select.tscn
│       │   ├── character_creator.gd    # 文字创建角色界面
│       │   ├── character_creator.tscn
│       │   ├── hud.gd
│       │   └── game_over.gd
│       │
│       └── stages/
│           ├── stage.gd
│           ├── stage.tscn
│           └── stage_data.gd
│
├── assets/
│   ├── sprites/
│   │   ├── default/                    # 默认火柴人部件
│   │   ├── effects/
│   │   ├── items/
│   │   └── ui/
│   ├── sounds/
│   ├── fonts/
│   └── generated/                      # 文本生成的角色资源目录
│       └── .gitkeep
│
└── addons/
```

---

## 7. 数据模型

### 7.1 C++ 数据类 (暴露给 GDScript)

```cpp
class CharacterDataCpp : public Resource {
    GDCLASS(CharacterDataCpp, Resource);

    String character_id;
    String display_name;

    // 属性
    int max_hp;
    int max_mp;
    int mp_regen;
    int speed;
    int jump_power;
    int weight;

    // 帧数据 (连续内存)
    PackedByteArray frame_data_raw;  // FrameData[] 序列化
    int frame_count;

    // 精灵
    Ref<Texture2D> sprite_sheet;
    Color primary_color;
    Color secondary_color;

    // 获取帧 (O(1) 索引)
    const FrameData* get_frame(int index) const {
        return &reinterpret_cast<const FrameData*>(frame_data_raw.ptr())[index];
    }
};
```

### 7.2 帧数据

| 字段 | 类型 | 大小 | 说明 |
|------|------|------|------|
| pic | int16_t | 2B | 精灵帧索引 |
| state | int16_t | 2B | 状态ID |
| wait | int16_t | 2B | 停留帧数 |
| next | int16_t | 2B | 跳转目标 (999=idle) |
| dvx, dvy, dvz | int16_t | 6B | 速度增量 |
| mp | int16_t | 2B | 魔力变化 |
| shake | int16_t | 2B | 屏幕震动强度 |
| sound_id | int16_t | 2B | 音效ID |
| hit_a, hit_j, hit_d | int16_t×3 | 6B | 按键跳转 |
| hit_Fa/Ua/Da/Fj/Uj/Dj | int16_t×6 | 12B | 方向+按键跳转 |
| itr_count, bdy_count | uint8_t×2 | 2B | 判定框数量 |
| itrs[MAX_ITRS] | ItrData×4 | 4×16=64B | 攻击框内联数组 |
| bdys[MAX_BDYS] | BdyData×4 | 4×12=48B | 受击框内联数组 |
| opoint fields | int16_t×7 | 14B | 对象生成数据 |
| hurtable | bool | 1B | 是否可受击 |
| padding | - | ~5B | 对齐填充 |

**单帧 ≈ 192 字节**。50 帧/角色 × 20 角色 = **~192KB 帧数据**，可全部驻留 L2 缓存。

---

## 8. 构建与开发环境

### 8.1 依赖
- Godot 4.3+ (官方分发版)
- C++17 编译器 (MSVC / GCC / Clang)
- SCons (构建系统)
- godot-cpp (Git submodule: `git submodule add https://github.com/godotengine/godot-cpp`)

### 8.2 构建流程

```bash
# 1. 克隆 + 初始化子模块
git clone <repo>
git submodule update --init

# 2. 编译 C++ GDExtension
cd godot-cpp
scons platform=windows
cd ..
scons platform=windows

# 3. 在 Godot 中打开 project.godot
# 4. 运行 (GDExtension 自动加载)
```

### 8.3 开发迭代循环

```
修改 C++ 代码 → scons 重新编译 → 在 Godot 编辑器中按 F5
                  ↓
        GDExtension DLL 热重载 (Godot 4.3+)
```

---

## 9. 实现优先级

### P0 (MVP — 核心可玩)
- [ ] 项目搭建: SCons + godot-cpp + GDExtension 注册
- [ ] C++ 数据结构: data_types.h (FrameData/ItrData/BdyData)
- [ ] C++ 帧解释器 (FrameInterpreter + InputBuffer)
- [ ] C++ 战斗系统 (CombatSystem)
- [ ] GDScript: Fighter 节点封装 + 基础状态机
- [ ] GDScript: VS Mode 本地双人对战
- [ ] GDScript: 基础 UI (主菜单 + HUD)
- [ ] 手写一个默认角色 (Davis) 的帧数据用于测试

### P1 (特色功能)
- [ ] C++ 文本解析引擎 (TextParser + TemplateMatcher)
- [ ] C++ 程序化精灵生成 (ProceduralSprite + PoseEngine)
- [ ] GDScript: 角色创建界面 (文字输入 → 预览 → 确认)
- [ ] C++ 平衡性校验器 (BalanceValidator)
- [ ] C++ AI 控制器
- [ ] GDScript: Stage Mode 闯关模式
- [ ] 3 个职业模板 (Brawler / Mage / Ninja)

### P2 (完善)
- [ ] LLM 集成接口 (本地 Ollama / OpenAI API)
- [ ] 更多职业模板 (Swordsman / Archer / Tank)
- [ ] 元素系统 (火/冰/雷/暗/光)
- [ ] 物品/武器生成
- [ ] 音效系统
- [ ] Championship 模式
- [ ] 程序化背景生成

---

## 10. 设计原则

1. **C++ 性能优先** — 核心循环使用连续内存、内联数组、整数运算，避免 GC 和动态分配
2. **数据驱动** — 所有角色行为由 FrameData 定义，C++ 引擎只是解释器
3. **文本即代码** — 用户的文字描述是最高抽象层，自动降级为 FrameData
4. **可扩展** — 新增职业模板 = 一份数据文件，不涉及核心代码修改
5. **零美术依赖** — 所有视觉资产可程序化生成，初始版本不需要美术资源

---

## 11. 成功标准

1. VS Mode 中两个玩家可以选择角色正常对战
2. 输入 "一个火系格斗家，拳脚带火焰" → 系统生成一个可玩的角色
3. 生成的角色拥有合理的属性、招式和动画
4. 程序在 60fps 下稳定运行 (8 角色同屏)
5. 至少 3 种职业模板 + 3 种元素可组合出 9 种不同角色
