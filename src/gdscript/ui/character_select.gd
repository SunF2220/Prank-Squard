extends Control

# Character selection screen

@export var default_characters: Array[Dictionary] = []

var p1_selection: int = 0
var p2_selection: int = 0
var p1_locked: bool = false
var p2_locked: bool = false

@onready var p1_name_label: Label = $P1Panel/P1Name
@onready var p1_stats_label: Label = $P1Panel/P1Stats
@onready var p1_prev_btn: Button = $P1Panel/P1Prev
@onready var p1_next_btn: Button = $P1Panel/P1Next
@onready var p1_lock_btn: Button = $P1Panel/P1Lock

@onready var p2_name_label: Label = $P2Panel/P2Name
@onready var p2_stats_label: Label = $P2Panel/P2Stats
@onready var p2_prev_btn: Button = $P2Panel/P2Prev
@onready var p2_next_btn: Button = $P2Panel/P2Next
@onready var p2_lock_btn: Button = $P2Panel/P2Lock

@onready var fight_btn: Button = $FightBtn
@onready var back_btn: Button = $BackBtn

var all_characters: Array[Dictionary] = []

func _ready():
	_load_characters()

	p1_prev_btn.pressed.connect(func(): _cycle_p1(-1))
	p1_next_btn.pressed.connect(func(): _cycle_p1(1))
	p1_lock_btn.pressed.connect(func(): _lock_p1())
	p2_prev_btn.pressed.connect(func(): _cycle_p2(-1))
	p2_next_btn.pressed.connect(func(): _cycle_p2(1))
	p2_lock_btn.pressed.connect(func(): _lock_p2())
	fight_btn.pressed.connect(_on_fight)
	back_btn.pressed.connect(_on_back)

	_update_display()

func _load_characters():
	# Default Davis-style character
	all_characters.append({
		"name": "Davis",
		"max_hp": 500, "max_mp": 500, "mp_regen": 3,
		"speed": 5, "jump_power": 12, "weight": 100,
		"primary_color": Color(0.9, 0.3, 0.1),
		"secondary_color": Color(0.2, 0.1, 0.05),
		"effect_type": 0,
		"moves": ["Combo Punch", "Uppercut", "Dragon Punch", "Super Flurry"]
	})

	# Default characters
	all_characters.append({
		"name": "Flame Mage",
		"max_hp": 350, "max_mp": 700, "mp_regen": 5,
		"speed": 4, "jump_power": 10, "weight": 80,
		"primary_color": Color(0.8, 0.2, 0.1),
		"secondary_color": Color(1.0, 0.4, 0.0),
		"effect_type": 1,
		"moves": ["Magic Bolt", "Fireball", "Flame Wave", "Inferno"]
	})

	all_characters.append({
		"name": "Ice Ninja",
		"max_hp": 380, "max_mp": 450, "mp_regen": 4,
		"speed": 7, "jump_power": 14, "weight": 70,
		"primary_color": Color(0.1, 0.2, 0.3),
		"secondary_color": Color(0.4, 0.7, 1.0),
		"effect_type": 2,
		"moves": ["Quick Strike", "Ice Shuriken", "Shadow Dash", "Blizzard"]
	})

	# Load any generated characters
	var storage = get_node_or_null("/root/CharacterStorage")
	if storage and storage.characters.size() > 0:
		all_characters.append_array(storage.characters)

func _update_display():
	# P1
	if p1_selection < all_characters.size():
		var c = all_characters[p1_selection]
		p1_name_label.text = c.get("name", "???")
		p1_stats_label.text = "HP:%d MP:%d SPD:%d" % [c.get("max_hp", 0), c.get("max_mp", 0), c.get("speed", 0)]
		if p1_locked:
			p1_lock_btn.text = "Locked!"
			p1_lock_btn.modulate = Color.GREEN
		else:
			p1_lock_btn.text = "Lock In"
			p1_lock_btn.modulate = Color.WHITE

	# P2
	if p2_selection < all_characters.size():
		var c = all_characters[p2_selection]
		p2_name_label.text = c.get("name", "???")
		p2_stats_label.text = "HP:%d MP:%d SPD:%d" % [c.get("max_hp", 0), c.get("max_mp", 0), c.get("speed", 0)]
		if p2_locked:
			p2_lock_btn.text = "Locked!"
			p2_lock_btn.modulate = Color.GREEN
		else:
			p2_lock_btn.text = "Lock In"
			p2_lock_btn.modulate = Color.WHITE

	fight_btn.visible = p1_locked and p2_locked

func _cycle_p1(dir: int):
	if p1_locked: return
	p1_selection = wrapi(p1_selection + dir, 0, all_characters.size())
	_update_display()

func _cycle_p2(dir: int):
	if p2_locked: return
	p2_selection = wrapi(p2_selection + dir, 0, all_characters.size())
	_update_display()

func _lock_p1():
	p1_locked = !p1_locked
	_update_display()

func _lock_p2():
	p2_locked = !p2_locked
	_update_display()

func _on_fight():
	if not p1_locked or not p2_locked:
		return

	# Store selections for game scene
	var sel = {
		"p1": all_characters[p1_selection],
		"p2": all_characters[p2_selection]
	}
	if has_node("/root/MatchData"):
		get_node("/root/MatchData").set_selections(sel)

	get_tree().change_scene_to_file("res://src/gdscript/core/game.tscn")

func _on_back():
	get_tree().change_scene_to_file("res://src/gdscript/ui/main_menu.tscn")
