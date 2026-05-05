extends Control

# Text-to-character creator UI

@onready var input_field: LineEdit = $Panel/VBoxContainer/InputField
@onready var generate_btn: Button = $Panel/VBoxContainer/GenerateBtn
@onready var preview_panel: Panel = $Panel/VBoxContainer/PreviewPanel
@onready var preview_text: RichTextLabel = $Panel/VBoxContainer/PreviewPanel/PreviewText
@onready var confirm_btn: Button = $Panel/VBoxContainer/PreviewPanel/ConfirmBtn
@onready var back_btn: Button = $Panel/VBoxContainer/BackBtn

# Template data (mirrors C++ archetype_templates.h)
var templates = {
	"brawler": {
		"name": "Brawler",
		"max_hp": 500, "max_mp": 400, "mp_regen": 2,
		"speed": 5, "jump_power": 12, "weight": 100,
		"color": Color(0.9, 0.3, 0.1),
		"moves": ["Combo Punch", "Uppercut", "Dash Punch", "Flurry"]
	},
	"mage": {
		"name": "Mage",
		"max_hp": 350, "max_mp": 700, "mp_regen": 5,
		"speed": 4, "jump_power": 10, "weight": 80,
		"color": Color(0.2, 0.1, 0.8),
		"moves": ["Magic Bolt", "Elemental Blast", "Teleport", "Cataclysm"]
	},
	"ninja": {
		"name": "Ninja",
		"max_hp": 380, "max_mp": 450, "mp_regen": 4,
		"speed": 7, "jump_power": 14, "weight": 70,
		"color": Color(0.1, 0.1, 0.1),
		"moves": ["Quick Strike", "Shuriken Throw", "Shadow Dash", "Assassinate"]
	},
	"swordsman": {
		"name": "Swordsman",
		"max_hp": 450, "max_mp": 400, "mp_regen": 3,
		"speed": 5, "jump_power": 11, "weight": 95,
		"color": Color(0.8, 0.6, 0.2),
		"moves": ["Sword Slash", "Blade Wave", "Thrust", "Whirlwind"]
	},
	"archer": {
		"name": "Archer",
		"max_hp": 380, "max_mp": 450, "mp_regen": 3,
		"speed": 5, "jump_power": 11, "weight": 85,
		"color": Color(0.1, 0.5, 0.1),
		"moves": ["Rapid Shot", "Piercing Arrow", "Multi Shot", "Arrow Rain"]
	},
	"tank": {
		"name": "Tank",
		"max_hp": 700, "max_mp": 250, "mp_regen": 1,
		"speed": 3, "jump_power": 7, "weight": 150,
		"color": Color(0.5, 0.5, 0.5),
		"moves": ["Heavy Punch", "Ground Slam", "Charge", "Earthquake"]
	}
}

var elements = {
	"fire": {"name": "Fire", "effect": 1, "color": Color(1.0, 0.3, 0.0)},
	"ice": {"name": "Ice", "effect": 2, "color": Color(0.3, 0.8, 1.0)},
	"thunder": {"name": "Thunder", "effect": 3, "color": Color(1.0, 0.8, 0.0)},
	"dark": {"name": "Dark", "effect": 4, "color": Color(0.3, 0.0, 0.5)},
	"light": {"name": "Light", "effect": 5, "color": Color(1.0, 1.0, 0.5)},
}

var generated_data: Dictionary = {}

func _ready():
	generate_btn.pressed.connect(_on_generate)
	confirm_btn.pressed.connect(_on_confirm)
	back_btn.pressed.connect(_on_back)
	preview_panel.visible = false

func _on_generate():
	var text = input_field.text.strip_edges()
	if text.is_empty():
		preview_text.text = "[color=red]Please enter a character description![/color]"
		preview_panel.visible = true
		return

	generated_data = parse_text(text)
	show_preview(generated_data)

func parse_text(text: String) -> Dictionary:
	var result = {}
	var lower = text.to_lower()

	# Find archetype
	var best_arch = "brawler"
	var best_score = 0
	var arch_keywords = {
		"brawler": ["格斗", "武术", "拳", "打架", "brawler", "fighter", "拳师"],
		"mage": ["魔法", "法师", "巫师", "术士", "mage", "wizard", "sorcerer"],
		"ninja": ["忍者", "刺客", "ninja", "assassin", "忍", "影"],
		"swordsman": ["剑", "战士", "sword", "warrior", "骑士", "刀"],
		"archer": ["弓", "射手", "猎人", "archer", "bow", "箭"],
		"tank": ["重装", "坦克", "tank", "heavy", "防御", "盾"],
	}

	for arch in arch_keywords:
		for kw in arch_keywords[arch]:
			if kw in lower:
				var score = kw.length()
				if score > best_score:
					best_score = score
					best_arch = arch

	result["archetype"] = best_arch
	var t = templates[best_arch]

	# Find element
	var best_elem = "none"
	best_score = 0
	var elem_keywords = {
		"fire": ["火", "炎", "fire", "flame", "灼烧", "燃烧"],
		"ice": ["冰", "霜", "ice", "frost", "冻", "寒"],
		"thunder": ["雷", "电", "thunder", "lightning", "闪", "霹雳"],
		"dark": ["暗", "影", "dark", "shadow", "黑暗", "冥"],
		"light": ["光", "圣", "light", "holy", "光明", "辉"],
	}

	for elem in elem_keywords:
		for kw in elem_keywords[elem]:
			if kw in lower:
				var score = kw.length()
				if score > best_score:
					best_score = score
					best_elem = elem

	result["element"] = best_elem
	var elem_data = elements.get(best_elem, {"name": "None", "effect": 0, "color": Color.WHITE})

	# Build character data
	result["display_name"] = elem_data["name"] + " " + t["name"] if best_elem != "none" else t["name"]
	result["max_hp"] = t["max_hp"]
	result["max_mp"] = t["max_mp"]
	result["mp_regen"] = t["mp_regen"]
	result["speed"] = t["speed"]
	result["jump_power"] = t["jump_power"]
	result["weight"] = t["weight"]
	result["primary_color"] = t["color"]
	result["secondary_color"] = elem_data["color"] if best_elem != "none" else t["color"].darkened(0.3)
	result["effect_type"] = elem_data["effect"]
	result["moves"] = t["moves"]

	return result

func show_preview(data: Dictionary):
	var text = "[center][font_size=24][b]%s[/b][/font_size][/center]\n\n" % data.get("display_name", "Unknown")
	text += "[b]Stats:[/b]\n"
	text += "  HP: %d  |  MP: %d  |  Speed: %d\n" % [data.get("max_hp", 0), data.get("max_mp", 0), data.get("speed", 0)]
	text += "  Jump: %d  |  Weight: %d\n\n" % [data.get("jump_power", 0), data.get("weight", 0)]
	text += "[b]Moves:[/b]\n"

	var moves = data.get("moves", [])
	var move_names = ["Basic", "Special 1", "Special 2", "Ultimate"]
	for i in range(moves.size()):
		var label = move_names[i] if i < move_names.size() else "Move %d" % (i + 1)
		text += "  %s: %s\n" % [label, moves[i]]

	text += "\n[b]Element:[/b] %s\n" % data.get("element", "None")
	text += "[b]Archetype:[/b] %s\n" % data.get("archetype", "Unknown")

	preview_text.text = text
	preview_panel.visible = true

func _on_confirm():
	if generated_data.is_empty():
		return

	# Save generated character data
	var save_path = "user://generated/"
	DirAccess.make_dir_recursive_absolute(save_path)

	var file_name = generated_data.get("display_name", "character").replace(" ", "_").to_lower() + ".tres"
	# In a full implementation, save as a CharacterData resource
	# For now, store in a global autoload
	if has_node("/root/CharacterStorage"):
		get_node("/root/CharacterStorage").add_character(generated_data)

	# Go to character select
	get_tree().change_scene_to_file("res://src/gdscript/ui/character_select.tscn")

func _on_back():
	get_tree().change_scene_to_file("res://src/gdscript/ui/main_menu.tscn")
