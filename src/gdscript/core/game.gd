extends Node2D
class_name GameController

# The main game controller - manages rounds, fighters, and game flow

enum GameState {
	INTRO,
	FIGHTING,
	ROUND_END,
	GAME_OVER
}

enum GameMode {
	VS_MODE,
	STAGE_MODE,
	TRAINING
}

@export var game_mode: int = GameMode.VS_MODE
@export var round_time: float = 99.0
@export var rounds_to_win: int = 2

var current_state: int = GameState.INTRO
var timer: float = 0.0
var intro_timer: float = 0.0

# Fighters
var fighter1: FighterNode
var fighter2: FighterNode

# Round tracking
var p1_wins: int = 0
var p2_wins: int = 0
var current_round: int = 1

# References
@onready var hud = $HUD
@onready var intro_label = $IntroLabel

signal round_start()
signal round_end(winner: int)
signal game_over(winner: int)

func _ready():
	timer = round_time
	start_intro()

func start_intro():
	current_state = GameState.INTRO
	intro_timer = 2.0

	# Find fighters
	fighter1 = $Fighter1
	fighter2 = $Fighter2

	if fighter1 and fighter2:
		fighter1.opponent = fighter2
		fighter2.opponent = fighter1

	if intro_label:
		intro_label.text = "Round " + str(current_round)
		intro_label.visible = true

	# Reset fighters
	if fighter1:
		fighter1.hp = fighter1.max_hp
		fighter1.mp = fighter1.max_mp
		fighter1.alive = true
		fighter1.global_position = Vector2(300, 500)
		fighter1.facing = 1
		fighter1.change_state(FighterNode.State.IDLE)
		fighter1.current_state = FighterNode.State.IDLE

	if fighter2:
		fighter2.hp = fighter2.max_hp
		fighter2.mp = fighter2.max_mp
		fighter2.alive = true
		fighter2.global_position = Vector2(980, 500)
		fighter2.facing = -1
		fighter2.change_state(FighterNode.State.IDLE)
		fighter2.current_state = FighterNode.State.IDLE

func _process(delta):
	match current_state:
		GameState.INTRO:
			update_intro(delta)
		GameState.FIGHTING:
			update_fighting(delta)
		GameState.ROUND_END:
			update_round_end(delta)
		GameState.GAME_OVER:
			update_game_over(delta)

	# Always update HUD
	if hud:
		update_hud()

func update_intro(delta):
	intro_timer -= delta
	if intro_label:
		var scale_val = 1.0 + abs(sin(intro_timer * 5.0)) * 0.3
		intro_label.scale = Vector2(scale_val, scale_val)

	if intro_timer <= 0:
		current_state = GameState.FIGHTING
		if intro_label:
			intro_label.visible = false
		round_start.emit()

func update_fighting(delta):
	timer -= delta

	# AI update
	if fighter2 and fighter2.is_ai:
		fighter2.ai_update(delta)
	if fighter1 and fighter1.is_ai:
		fighter1.ai_update(delta)

	# Check attack collisions
	if fighter1 and fighter2:
		fighter1.check_attack_hit(fighter2)
		fighter2.check_attack_hit(fighter1)

	# Check round end
	if timer <= 0:
		end_round_by_timeout()
	elif fighter1 and not fighter1.alive:
		end_round(2)
	elif fighter2 and not fighter2.alive:
		end_round(1)

func update_round_end(delta):
	intro_timer -= delta
	if intro_timer <= 0:
		if p1_wins >= rounds_to_win or p2_wins >= rounds_to_win:
			current_state = GameState.GAME_OVER
			var winner = 1 if p1_wins >= rounds_to_win else 2
			game_over.emit(winner)
			if intro_label:
				intro_label.text = "Player " + str(winner) + " Wins!"
				intro_label.visible = true
		else:
			current_round += 1
			timer = round_time
			start_intro()

func update_game_over(_delta):
	if Input.is_action_just_pressed("p1_attack") or Input.is_action_just_pressed("p2_attack"):
		get_tree().change_scene_to_file("res://src/gdscript/ui/main_menu.tscn")

func end_round(winner: int):
	current_state = GameState.ROUND_END
	intro_timer = 2.0

	if winner == 1:
		p1_wins += 1
	else:
		p2_wins += 1

	if intro_label:
		intro_label.text = "Player " + str(winner) + " Wins!"
		intro_label.visible = true

	round_end.emit(winner)

func end_round_by_timeout():
	# Winner is whoever has more HP
	if fighter1 and fighter2:
		if fighter1.hp >= fighter2.hp:
			end_round(1)
		else:
			end_round(2)

func update_hud():
	if not hud:
		return

	if fighter1:
		hud.set_p1_hp(fighter1.hp, fighter1.max_hp)
		hud.set_p1_mp(fighter1.mp, fighter1.max_mp)
		hud.set_p1_name(fighter1.character_name)
	if fighter2:
		hud.set_p2_hp(fighter2.hp, fighter2.max_hp)
		hud.set_p2_mp(fighter2.mp, fighter2.max_mp)
		hud.set_p2_name(fighter2.character_name)

	hud.set_timer(timer)
