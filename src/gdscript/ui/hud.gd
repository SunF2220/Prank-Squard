extends CanvasLayer
class_name BattleHUD

# P1 HUD elements
@onready var p1_hp_bar: TextureProgressBar = $P1Panel/P1HPBar
@onready var p1_mp_bar: TextureProgressBar = $P1Panel/P1MPBar
@onready var p1_name_label: Label = $P1Panel/P1NameLabel
@onready var p1_hp_text: Label = $P1Panel/P1HPText

# P2 HUD elements
@onready var p2_hp_bar: TextureProgressBar = $P2Panel/P2HPBar
@onready var p2_mp_bar: TextureProgressBar = $P2Panel/P2MPBar
@onready var p2_name_label: Label = $P2Panel/P2NameLabel
@onready var p2_hp_text: Label = $P2Panel/P2HPText

# Timer
@onready var timer_label: Label = $TimerLabel

func _ready():
	pass

func set_p1_hp(current: int, maximum: int):
	if p1_hp_bar:
		p1_hp_bar.max_value = maximum
		p1_hp_bar.value = current
	if p1_hp_text:
		p1_hp_text.text = str(current) + " / " + str(maximum)

func set_p1_mp(current: int, maximum: int):
	if p1_mp_bar:
		p1_mp_bar.max_value = maximum
		p1_mp_bar.value = current

func set_p2_hp(current: int, maximum: int):
	if p2_hp_bar:
		p2_hp_bar.max_value = maximum
		p2_hp_bar.value = current
	if p2_hp_text:
		p2_hp_text.text = str(current) + " / " + str(maximum)

func set_p2_mp(current: int, maximum: int):
	if p2_mp_bar:
		p2_mp_bar.max_value = maximum
		p2_mp_bar.value = current

func set_timer(time_left: float):
	if timer_label:
		var mins = int(time_left / 60)
		var secs = int(time_left) % 60
		timer_label.text = "%02d:%02d" % [mins, secs]
		if time_left < 10:
			timer_label.modulate = Color.RED
		else:
			timer_label.modulate = Color.WHITE

func set_p1_name(name_str: String):
	if p1_name_label:
		p1_name_label.text = name_str

func set_p2_name(name_str: String):
	if p2_name_label:
		p2_name_label.text = name_str
