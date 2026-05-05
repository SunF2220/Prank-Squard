extends CharacterBody2D
class_name FighterNode

# Fighter state enum
enum State {
	IDLE,
	WALK_FORWARD,
	WALK_BACKWARD,
	ATTACK,
	ATTACK2,
	ATTACK3,
	JUMP,
	DASH,
	HURT,
	KNOCKDOWN,
	DEFEATED,
	DEFEND,
	SPECIAL,
	SPECIAL2,
	SPECIAL3
}

# Player assignment
@export var player_id: int = 1
@export var is_ai: bool = false

# Character stats
@export var max_hp: int = 500
@export var max_mp: int = 500
@export var mp_regen: float = 5.0  # per second
@export var move_speed: float = 300.0
@export var jump_velocity: float = -500.0
@export var weight: int = 100
@export var character_name: String = "Fighter"
@export var primary_color: Color = Color(0.9, 0.3, 0.1)
@export var secondary_color: Color = Color(0.2, 0.1, 0.05)
@export var element_type: int = 0  # 0=normal, 1=fire, 2=ice, 3=thunder

# Runtime state
var hp: int
var mp: int
var current_state: int = State.IDLE
var previous_state: int = State.IDLE
var state_timer: float = 0.0
var facing: int = 1  # 1=right, -1=left
var is_grounded: bool = true
var is_blocking: bool = false
var is_hurtable: bool = true
var combo_count: int = 0
var alive: bool = true
var hitstop_timer: float = 0.0
var invincible_timer: float = 0.0

# Attack data
var active_attack_box: Rect2
var attack_damage: int = 0
var attack_knockback: Vector2 = Vector2.ZERO
var attack_effect: int = 0

# Input buffer
var input_buffer: Array[Dictionary] = []
var input_buffer_max: int = 20

# Reference to opponent
var opponent: FighterNode = null

# Signals
signal fighter_hurt(damage: int)
signal fighter_defeated()
signal attack_landed(target: FighterNode, damage: int)

func _ready():
	hp = max_hp
	mp = max_mp
	alive = true

func _physics_process(delta):
	if not alive:
		return

	if hitstop_timer > 0:
		hitstop_timer -= delta
		return

	if invincible_timer > 0:
		invincible_timer -= delta

	# MP regen
	mp = min(mp + int(mp_regen * delta), max_mp)

	# Read input
	if not is_ai:
		read_input(delta)

	# Update facing based on opponent
	if opponent and opponent.alive:
		facing = 1 if opponent.global_position.x > global_position.x else -1

	# State update
	update_state(delta)

	# Apply gravity
	if not is_grounded:
		velocity.y += 980 * delta  # gravity

	# Apply velocity
	move_and_slide()

	# Ground check
	is_grounded = is_on_floor()
	if is_grounded and velocity.y > 0:
		velocity.y = 0

	# Arena bounds
	global_position.x = clamp(global_position.x, 100, 1180)
	global_position.y = clamp(global_position.y, 0, 720)
	if global_position.y >= 620:
		global_position.y = 620
		velocity.y = 0
		is_grounded = true

	# Auto-return to idle if state ended
	state_timer += delta

	# Combo timer decay
	if combo_count > 0:
		combo_count = 0

func read_input(_delta: float):
	var prefix = "p%d_" % player_id

	var left = Input.is_action_pressed(prefix + "left")
	var right = Input.is_action_pressed(prefix + "right")
	var up = Input.is_action_pressed(prefix + "up")
	var down = Input.is_action_pressed(prefix + "down")
	var attack = Input.is_action_just_pressed(prefix + "attack")
	var jump = Input.is_action_just_pressed(prefix + "jump")
	var defend = Input.is_action_pressed(prefix + "defend")

	var snap = {
		"left": left, "right": right, "up": up, "down": down,
		"attack": attack, "jump": jump, "defend": defend,
		"time": Time.get_ticks_msec()
	}
	input_buffer.append(snap)
	while input_buffer.size() > input_buffer_max:
		input_buffer.pop_front()

	# Process state transitions from input
	process_input_transitions(left, right, up, down, attack, jump, defend)

func process_input_transitions(left: bool, right: bool, up: bool, down: bool,
		attack: bool, jump: bool, defend: bool):
	# Can't act during certain states
	if current_state in [State.HURT, State.KNOCKDOWN, State.DEFEATED]:
		return

	# Defend
	if defend and is_grounded and current_state not in [State.ATTACK, State.ATTACK2, State.ATTACK3, State.SPECIAL, State.SPECIAL2, State.SPECIAL3]:
		change_state(State.DEFEND)
		return

	# Jump
	if jump and is_grounded and current_state != State.DEFEND:
		change_state(State.JUMP)
		return

	# Attack
	if attack and is_grounded and current_state not in [State.DEFEND]:
		# Detect special: D, DR, R + A
		if detect_special_input():
			change_state(State.SPECIAL)
			return
		# Normal attack (combo chain)
		if current_state == State.ATTACK:
			change_state(State.ATTACK2)
		elif current_state == State.ATTACK2:
			change_state(State.ATTACK3)
		else:
			change_state(State.ATTACK)
		return

	# Movement
	var moving = false
	if left or right:
		moving = true
		if left:
			facing = -1
			velocity.x = -move_speed
		if right:
			facing = 1
			velocity.x = move_speed

		if current_state not in [State.ATTACK, State.ATTACK2, State.ATTACK3, State.SPECIAL, State.SPECIAL2, State.SPECIAL3]:
			if left and facing == -1:
				change_state(State.WALK_FORWARD)
			elif right and facing == 1:
				change_state(State.WALK_FORWARD)
			else:
				change_state(State.WALK_BACKWARD)

	if not moving and current_state in [State.WALK_FORWARD, State.WALK_BACKWARD]:
		velocity.x = 0
		change_state(State.IDLE)

	# Z-axis (depth) movement
	var z_input = 0
	if up: z_input = -1
	elif down: z_input = 1

	# Not defending -> go idle
	if current_state == State.DEFEND and not defend:
		change_state(State.IDLE)

func detect_special_input() -> bool:
	# Check for D, DR, R pattern in input buffer
	if mp < 100:
		return false
	if input_buffer.size() < 4:
		return false

	# Simple check: down then right then attack within last ~10 frames
	var recent = input_buffer.slice(max(0, input_buffer.size() - 10))
	var has_down = false
	var has_right = false
	for snap in recent:
		if snap.down and not snap.right:
			has_down = true
		if snap.right and has_down:
			has_right = true
	return has_down and has_right

func change_state(new_state: int):
	if new_state == current_state:
		return

	exit_state(current_state)
	previous_state = current_state
	current_state = new_state
	state_timer = 0.0
	enter_state(new_state)

func enter_state(state: int):
	match state:
		State.IDLE:
			attack_damage = 0
			active_attack_box = Rect2()
		State.WALK_FORWARD:
			velocity.x = move_speed * facing
		State.WALK_BACKWARD:
			velocity.x = -move_speed * 0.6 * facing
		State.ATTACK:
			set_attack(15, Vector2(10, 0), 0.3)
		State.ATTACK2:
			set_attack(20, Vector2(12, 0), 0.3)
		State.ATTACK3:
			set_attack(30, Vector2(18, -3), 0.4)
		State.JUMP:
			velocity.y = jump_velocity
			is_grounded = false
		State.DEFEND:
			is_blocking = true
			velocity.x = 0
		State.SPECIAL:
			set_attack(40, Vector2(20, -5), 0.5)
			mp -= 100
		State.HURT:
			is_blocking = false
			invincible_timer = 0.3
			velocity.x = 0
		State.DEFEATED:
			alive = false
			is_hurtable = false

func exit_state(state: int):
	match state:
		State.DEFEND:
			is_blocking = false
		State.ATTACK, State.ATTACK2, State.ATTACK3, State.SPECIAL:
			attack_damage = 0
			active_attack_box = Rect2()

func set_attack(damage: int, knockback: Vector2, duration: float):
	attack_damage = damage
	attack_knockback = knockback
	attack_effect = element_type
	# Attack box in front of fighter
	var box_x = 40 * facing
	var box_y = -40
	active_attack_box = Rect2(box_x, box_y, 50, 60 if current_state != State.JUMP else 40)

func update_state(_delta: float):
	match current_state:
		State.ATTACK:
			if state_timer > 0.3:
				change_state(State.IDLE)
		State.ATTACK2:
			if state_timer > 0.3:
				change_state(State.IDLE)
		State.ATTACK3:
			if state_timer > 0.45:
				change_state(State.IDLE)
		State.SPECIAL:
			if state_timer > 0.5:
				change_state(State.IDLE)
		State.HURT:
			if state_timer > 0.3 and is_grounded:
				change_state(State.IDLE)
		State.JUMP:
			if is_grounded and state_timer > 0.1:
				change_state(State.IDLE)

func take_hit(from_node: FighterNode, damage: int, knockback: Vector2):
	if not alive:
		return
	if invincible_timer > 0:
		return
	if not is_hurtable:
		return

	var actual_damage = damage
	var actual_kb = knockback * (100.0 / float(weight))

	if is_blocking:
		actual_damage = int(damage * 0.25)
		actual_kb *= 0.3
		is_blocking = false

	hp -= actual_damage
	fighter_hurt.emit(actual_damage)

	# Apply knockback (away from attacker)
	var kb_dir = 1 if from_node.global_position.x > global_position.x else -1
	velocity.x = actual_kb.x * -kb_dir * 50
	velocity.y = actual_kb.y * 50 - 100  # slight upward

	if hp <= 0:
		hp = 0
		change_state(State.DEFEATED)
		fighter_defeated.emit()
	else:
		change_state(State.HURT)

	# Hitstop
	hitstop_timer = 0.1
	if from_node:
		from_node.hitstop_timer = 0.1

	from_node.attack_landed.emit(self, actual_damage)

func check_attack_hit(target: FighterNode):
	if attack_damage <= 0:
		return
	if not target.alive:
		return

	# Calculate world-space attack box
	var atk_rect = Rect2(
		global_position.x + active_attack_box.position.x,
		global_position.y + active_attack_box.position.y,
		active_attack_box.size.x,
		active_attack_box.size.y
	)

	# Target hurtbox (simplified)
	var target_rect = Rect2(
		target.global_position.x - 25,
		target.global_position.y - 80,
		50, 85
	)

	if atk_rect.intersects(target_rect):
		target.take_hit(self, attack_damage, attack_knockback)
		# Reset attack so it only hits once
		attack_damage = 0

func _draw():
	if not alive:
		return
	draw_fighter()

func draw_fighter():
	var c = primary_color
	var outline = Color(0.1, 0.1, 0.1)

	# Body origin (feet on ground)
	var cx = 0.0
	var base_y = 0.0

	# Head
	var head_r = 10.0
	var head_y = -90.0
	# Breathing animation
	if current_state == State.IDLE:
		head_y += sin(Time.get_ticks_msec() * 0.002) * 2.0

	draw_circle(Vector2(cx, head_y), head_r, c, outline)

	# Body (torso line)
	var neck_y = head_y + head_r
	var hip_y = -35.0
	draw_line(Vector2(cx, neck_y), Vector2(cx, hip_y), outline, 3)

	# Arms
	var elbow_y = neck_y + 20
	var hand_y = elbow_y + 18

	var swing = 0.0
	if current_state in [State.ATTACK, State.ATTACK2, State.ATTACK3, State.SPECIAL]:
		swing = 30.0 if state_timer > 0.15 else 0.0
	elif current_state == State.JUMP:
		swing = -15.0
	elif current_state == State.DEFEND:
		swing = -20.0

	var l_shoulder_x = -10.0
	var r_shoulder_x = 10.0
	var l_elbow_x = l_shoulder_x - 8.0 + swing * 0.5
	var r_elbow_x = r_shoulder_x + 8.0 + swing * 1.0
	var l_hand_x = l_elbow_x - 5.0 + swing * 0.3
	var r_hand_x = r_elbow_x + 5.0 + swing * 1.5

	if current_state == State.DEFEND:
		l_elbow_y = neck_y + 5
		l_hand_y = neck_y - 5
		r_elbow_y = neck_y + 5
		r_hand_y = neck_y - 5
		draw_line(Vector2(l_shoulder_x, neck_y + 3), Vector2(l_elbow_x, l_elbow_y), outline, 2)
		draw_line(Vector2(l_elbow_x, l_elbow_y), Vector2(l_hand_x, l_hand_y), outline, 2)
		draw_line(Vector2(r_shoulder_x, neck_y + 3), Vector2(r_elbow_x, r_elbow_y), outline, 2)
		draw_line(Vector2(r_elbow_x, r_elbow_y), Vector2(r_hand_x, r_hand_y), outline, 2)
	else:
		draw_line(Vector2(l_shoulder_x, neck_y + 3), Vector2(l_elbow_x, elbow_y), outline, 2)
		draw_line(Vector2(l_elbow_x, elbow_y), Vector2(l_hand_x, hand_y), outline, 2)
		draw_line(Vector2(r_shoulder_x, neck_y + 3), Vector2(r_elbow_x, elbow_y), outline, 2)
		draw_line(Vector2(r_elbow_x, elbow_y), Vector2(r_hand_x, hand_y), outline, 2)

	# Legs
	var knee_y = hip_y + 22
	var foot_y = base_y
	var l_knee_x = -5.0
	var r_knee_x = 5.0
	var l_foot_x = l_knee_x - 3
	var r_foot_x = r_knee_x + 3

	# Walk animation
	if current_state in [State.WALK_FORWARD, State.WALK_BACKWARD]:
		var walk_phase = sin(Time.get_ticks_msec() * 0.008)
		l_knee_x += walk_phase * 8
		r_knee_x -= walk_phase * 8
		l_foot_x += walk_phase * 8
		r_foot_x -= walk_phase * 8
	elif current_state == State.JUMP:
		l_knee_x -= 5; l_foot_x -= 8
		r_knee_x += 5; r_foot_x += 8

	draw_line(Vector2(-5, hip_y), Vector2(l_knee_x, knee_y), outline, 2)
	draw_line(Vector2(l_knee_x, knee_y), Vector2(l_foot_x, foot_y), outline, 2)
	draw_line(Vector2(5, hip_y), Vector2(r_knee_x, knee_y), outline, 2)
	draw_line(Vector2(r_knee_x, knee_y), Vector2(r_foot_x, foot_y), outline, 2)

	# Element effect during attack
	if current_state in [State.ATTACK, State.ATTACK2, State.ATTACK3, State.SPECIAL] and element_type > 0:
		var effect_color = get_element_color()
		for i in range(4):
			var px = r_hand_x + randi_range(-12, 12)
			var py = hand_y + randi_range(-12, 12)
			draw_circle(Vector2(px, py), 3, effect_color, effect_color)

func get_element_color() -> Color:
	match element_type:
		1: return Color(1.0, 0.4, 0.0, 0.8)  # Fire
		2: return Color(0.4, 0.7, 1.0, 0.8)  # Ice
		3: return Color(1.0, 0.9, 0.1, 0.8)  # Thunder
	return Color.WHITE

func draw_circle(pos: Vector2, radius: float, fill: Color, outline: Color):
	draw_circle(pos, radius, fill)
	draw_arc(pos, radius, 0, TAU, 32, outline, 1.5)

func draw_line_custom(from_pos: Vector2, to_pos: Vector2, color: Color, width: float):
	draw_line(from_pos, to_pos, color, width)

func ai_update(delta: float):
	if not opponent or not opponent.alive:
		return

	var dx = opponent.global_position.x - global_position.x

	# Simple AI: move toward opponent, attack when close
	var prefix = "p%d_" % player_id

	var abs_dx = abs(dx)
	if abs_dx > 60:
		if dx > 0:
			Input.action_press(prefix + "right")
			Input.action_release(prefix + "left")
		else:
			Input.action_press(prefix + "left")
			Input.action_release(prefix + "right")
	else:
		Input.action_release(prefix + "left")
		Input.action_release(prefix + "right")

	# Attack when in range
	if abs_dx < 80 and randi() % 60 == 0:
		var ev = InputEventAction.new()
		ev.action = prefix + "attack"
		ev.pressed = true
		Input.parse_input_event(ev)

	# Jump sometimes
	if randi() % 200 == 0:
		var ev = InputEventAction.new()
		ev.action = prefix + "jump"
		ev.pressed = true
		Input.parse_input_event(ev)

	# Defend guard
	if abs_dx < 50 and randi() % 100 < 20:
		Input.action_press(prefix + "defend")
	else:
		Input.action_release(prefix + "defend")
