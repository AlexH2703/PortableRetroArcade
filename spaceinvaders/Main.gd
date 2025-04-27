extends Node2D

@onready var alien_container = $AlienContainer
@onready var alien_scene1 = preload("res://Alien.tscn")
@onready var alien_scene2 = preload("res://Alien2.tscn")
@onready var alien_scene3 = preload("res://Alien3.tscn")
@onready var shield_scene = preload("res://Shield.tscn")
@onready var score_label = $CanvasLayer/ScoreLabel
@onready var level_label = $CanvasLayer/LevelLabel
@onready var lives_label = $CanvasLayer/LivesLabel
@onready var game_over_overlay = $GameOverOverlay
@onready var leaderboard_label = $GameOverOverlay/VBoxContainer/Label2
@onready var restart_button = $GameOverOverlay/CenterContainer/Panel/RestartButton
@onready var main_menu_button = $GameOverOverlay/CenterContainer/Panel/MainMenuButton
@onready var http_request = $GameOverOverlay/VBoxContainer2/HTTPRequest
@onready var spaceship = $SpaceShipArea

@export var rows := 4
@export var columns := 11
@export var drop_timer_interval := 1.0
@export var alien_scale := Vector2(2.5, 2.5)
@export var speed := 2

var score = 0
var level = 1
var lives = 1
var level_in_progress = false
var drop_timer := Timer.new()
var alien_spacing := Vector2()
var direction := 1
var move_timer = 0.0
var invincible = false
var blink_count = 0
var ufo_scene = preload("res://Alien4.tscn")
var ufo_timer = 0.0
var ufo_spawn_delay = randf_range(10, 20)
var shields = []
var screen_size: Vector2

######################################################
#			Initialization functions
######################################################
func _ready():
	add_child(drop_timer)
	drop_timer.wait_time = drop_timer_interval
	drop_timer.timeout.connect(_on_drop_timer_timeout)
	spaceship.ship_hit.connect(_on_ship_hit)

	spawn_alien_grid()
	spawn_shields()
	restart_button.pressed.connect(_on_restart_pressed)
	main_menu_button.pressed.connect(_on_main_menu_pressed)
	screen_size = get_viewport_rect().size
	var ship_size = spaceship.get_node("SpaceShip").texture.get_size() if spaceship.has_node("SpaceShip") else Vector2(32, 32)
	print("Spaceship start position: ", spaceship.position)

	#spaceship.position = Vector2(screen_size.x / 2, screen_size.y - ship_size.y - 20)
	print("Spaceship Updated position: ", spaceship.position)


# Input function to detect Escape key press
func _input(event):
	if event.is_action_pressed("ui_cancel"): 
		get_tree().quit()  # Close the application

func _process(delta):
	update_label_positions()

	move_timer += delta
	if move_timer >= 0.1:
		move_aliens()
	if alien_container.get_child_count() == 0 and not level_in_progress:
		level_in_progress = true
		next_level()

	ufo_timer += delta
	if ufo_timer >= ufo_spawn_delay:
		spawn_ufo()
		ufo_timer = 0
		ufo_spawn_delay = randf_range(10, 20)


######################################################
#			Alien functions
######################################################
func spawn_alien_grid():
	var screen_size = get_viewport_rect().size
	var screen_width = screen_size.x

	var temp_alien = alien_scene1.instantiate()
	var alien_sprite = temp_alien.get_node("Alien1")
	if alien_sprite == null:
		return
	
	alien_sprite.scale = alien_scale
	var alien_width = 32 * alien_scale.x
	
	alien_spacing.x = 1
	alien_spacing.y = 50
	
	var grid_width = columns * alien_width + (columns - 1) * alien_spacing.x
	var start_position = Vector2((screen_width - grid_width) / 2 + 100, 50)

	for row in range(rows):
		for col in range(columns):
			var alien
			if row == 0:
				alien = alien_scene2.instantiate()
			elif row == 1:
				alien = alien_scene3.instantiate()
			else:
				alien = alien_scene1.instantiate()

			var position = start_position + Vector2(col * (alien_width + alien_spacing.x), row * alien_spacing.y)

			if row == 1:
				position.y += 15
			elif row > 1:
				position.y += 10

			alien.position = position
			alien.scale = alien_scale
			alien_container.add_child(alien)

			var sprite_node = alien.get_node("Alien1")
			if sprite_node != null:
				sprite_node.play("default")

func move_aliens():
	var screen_width = get_viewport_rect().size.x
	var alien_width = 32 * alien_scale.x
	var spaceship_y = spaceship.position.y  # Get the spaceship's Y position

	# Move aliens
	for alien in alien_container.get_children():
		alien.position.x += direction * speed * 0.1

	# If no aliens are left, return
	if alien_container.get_child_count() == 0:
		return

	var left_edge = INF
	var right_edge = -INF
	var bottom_edge = -INF

	# Check if any alien has hit the edge and reverse direction
	for alien in alien_container.get_children():
		if alien.is_in_group("enemy"):
			left_edge = min(left_edge, alien.position.x)
			right_edge = max(right_edge, alien.position.x)
			bottom_edge = max(bottom_edge, alien.position.y)
	
	# Reverse direction and move down when edges are hit
	if (left_edge <= alien_width / 2 and direction == -1) or (right_edge + alien_width / 2 >= screen_width and direction == 1):
		direction *= -1
		for alien in alien_container.get_children():
			alien.position.y += alien_spacing.y
		move_timer = 0
	if bottom_edge >= spaceship.position.y - 75:
			print("Alien reached game over threshold at Y: ", bottom_edge)
			game_over()
			return

func _on_drop_timer_timeout():
	for alien in alien_container.get_children():
		alien.position.y += alien_spacing.y
		
func spawn_ufo():
	var ufo = ufo_scene.instantiate()

	ufo.scale = alien_scale

	if randi() % 2 == 0:
		ufo.position = Vector2(-50, 32)
		ufo.speed = 150
	else:
		ufo.position = Vector2(get_viewport_rect().size.x + 50, 32)
		ufo.speed = -150

	var sprite_node = ufo.get_node("Alien1")
	if sprite_node != null:
		sprite_node.play("default")

	ufo.connect("ufo_destroyed", Callable(self, "_on_UFO_destroyed"))
	add_child(ufo)

func _on_UFO_destroyed(points):
	print("UFO hit! Awarded points: ", points)
	add_score(points)

######################################################
#			Shield functions
######################################################
func spawn_shields():
	var screen_size = get_viewport_rect().size
	var shield_count = 4
	var extra_spacing = 100
	var total_spacing = (screen_size.x - (shield_count * extra_spacing)) / (shield_count + 1)
	var y_position = screen_size.y - 150

	for i in range(shield_count):
		var shield = shield_scene.instantiate()
		var x_position = total_spacing * (i + 1) + extra_spacing * i + extra_spacing / 2
		shield.position = Vector2(x_position, y_position)
		add_child(shield)
		
		# Add each shield to the shields list
		shields.append(shield)

######################################################
#			Label functions
######################################################
func add_score(points):
	score += points
	score_label.text = "Score:" + str(score)

func next_level() -> void:
	level += 1
	speed += 1
	level_label.text = "Level:" + str(level)
	direction = 1

	move_timer = 0.0
	ufo_timer = 0.0
	ufo_spawn_delay = randf_range(10, 20)

	await get_tree().create_timer(1.0).timeout

	spawn_alien_grid()
	level_in_progress = false
	
func update_label_positions():
	var padding = 40
	var horizontal_spacing = 300  # space between each label

	screen_size = get_viewport_rect().size

	# Start near bottom left and spread horizontally
	lives_label.position = Vector2(padding, screen_size.y - padding)
	level_label.position = Vector2(padding + horizontal_spacing, screen_size.y - padding)
	score_label.position = Vector2(padding + horizontal_spacing * 2, screen_size.y - padding)



######################################################
#			Ship functions
######################################################
func _on_ship_hit(lives_left):
	lives_label.text = "Lives:" + str(lives_left)
	
	if lives_left <= 0:
		game_over()

######################################################
#			Game Over functions
######################################################
func game_over():
	print("Game Over!")

	if spaceship != null:
		spaceship.queue_free()

	drop_timer.stop()

	# Call the function to handle centering and setup
	show_game_over_overlay()

	restart_button.grab_focus()

	get_tree().paused = true

func show_game_over_overlay():
	var center_container = game_over_overlay.get_node("CenterContainer")
	
	# Set the anchors of the CenterContainer to stretch it across the whole screen
	center_container.anchor_left = 0
	center_container.anchor_top = 0
	center_container.anchor_right = 1
	center_container.anchor_bottom = 1
	center_container.offset_left = 0
	center_container.offset_top = 0
	#
	#var panel = game_over_overlay.get_node("Panel")
	#var vbox = game_over_overlay.get_node("VBoxContainer")
#
	## Set the VBoxContainer's anchors to fill the entire Panel
	#vbox.anchor_left = 0
	#vbox.anchor_top = 0
	#vbox.anchor_right = 1
	#vbox.anchor_bottom = 1
#
	## Set VBoxContainer's offsets to ensure it's correctly positioned
	#vbox.offset_left = 0
	#vbox.offset_top = 0
	#
	## Center the GameOver label inside the VBoxContainer
	#var game_over_label = vbox.get_node("GameOver")
	#game_over_label.anchor_left = 0.5
	#game_over_label.anchor_top = 0.5
	#game_over_label.anchor_right = 0.5
	#game_over_label.anchor_bottom = 0.5
	#game_over_label.offset_left = 0
	#game_over_label.offset_top = 0
	#
	#var restart_rect = restart_button.get_rect()
	## Center the Restart button inside the VBoxContainer without resizing
	#restart_button.anchor_left = 0.5
	#restart_button.anchor_top = 0.5
	#restart_button.anchor_right = 0.5
	#restart_button.anchor_bottom = 0.5
	#restart_button.offset_left = -restart_rect.size.x / 2
	#restart_button.offset_top = -restart_rect.size.y / 2
	#
	## Get the current rect of the Main Menu button
	#var main_menu_rect = main_menu_button.get_rect()
	## Center the Main Menu button inside the VBoxContainer without resizing
	#main_menu_button.anchor_left = 0.5
	#main_menu_button.anchor_top = 0.5
	#main_menu_button.anchor_right = 0.5
	#main_menu_button.anchor_bottom = 0.5
	#main_menu_button.offset_left = -main_menu_rect.size.x / 2
	#main_menu_button.offset_top = -main_menu_rect.size.y / 2

	# Make sure the game over overlay is visible
	game_over_overlay.visible = true

	# Pause the game (if needed)
	get_tree().paused = true



func _on_restart_pressed():
	get_tree().paused = false
	get_tree().reload_current_scene()

func _on_main_menu_pressed():
	get_tree().quit()
