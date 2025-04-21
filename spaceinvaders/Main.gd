extends Node2D

@onready var alien_container = $AlienContainer

# Preload each alien type
@onready var alien_scene1 = preload("res://Alien.tscn")   # Alien1 for bottom 2 rows
@onready var alien_scene2 = preload("res://Alien2.tscn")  # Alien2 for top row
@onready var alien_scene3 = preload("res://Alien3.tscn")  # Alien3 for second row

# Preload the Shield scene
@onready var shield_scene = preload("res://Shield.tscn")

@export var rows := 4
@export var columns := 11  # Number of columns of aliens
@export var drop_timer_interval := 1.0  # How often aliens drop
@export var alien_scale := Vector2(2.5, 2.5)  # Scale to make aliens bigger
@export var speed := 2  # Slow down the horizontal movement speed

@onready var score_label = $CanvasLayer/ScoreLabel
@onready var level_label = $CanvasLayer/LevelLabel
@onready var lives_label = $CanvasLayer/LivesLabel
@onready var game_over_overlay = $GameOverOverlay
@onready var leaderboard_label = $GameOverOverlay/VBoxContainer/Label2  # Adjust if you name it differently
@onready var restart_button = $GameOverOverlay/VBoxContainer/RestartButton
@onready var main_menu_button = $GameOverOverlay/VBoxContainer/MainMenuButton

var score = 0
var level = 1
var lives = 3
var level_in_progress = false

var drop_timer := Timer.new()
var alien_spacing := Vector2()  # Will be calculated dynamically
var direction := 1  # 1 for moving right, -1 for moving left
var move_timer = 0.0  # Timer to control horizontal movement

var ship: Node2D
var invincible = false
var blink_count = 0

var ufo_scene = preload("res://Alien4.tscn")
var ufo_timer = 0.0
var ufo_spawn_delay = randf_range(10, 20)  # Random time between spawns

func _ready():
	add_child(drop_timer)
	drop_timer.wait_time = drop_timer_interval
	drop_timer.timeout.connect(_on_drop_timer_timeout)
	#drop_timer.start()
	ship = get_node("SpaceShip")  # Make sure your SpaceShip node is named this
	var spaceship = $SpaceShipArea  # Adjust this path if your SpaceShip is deeper
	spaceship.ship_hit.connect(_on_ship_hit)

	spawn_alien_grid()
	spawn_shields()  # <- Call the new shield spawner here!
	restart_button.pressed.connect(_on_restart_pressed)
	main_menu_button.pressed.connect(_on_main_menu_pressed)


func spawn_alien_grid():
	var screen_size = get_viewport_rect().size
	var screen_width = screen_size.x

	var temp_alien = alien_scene1.instantiate()
	var alien_sprite = temp_alien.get_node("Alien1")
	if alien_sprite == null:
		print("Error: Alien1 node not found in Alien1 scene.")
		return
	
	alien_sprite.scale = alien_scale
	var alien_width = 32 * alien_scale.x  # Adjust as necessary
	
	alien_spacing.x = 1
	alien_spacing.y = 50
	
	var grid_width = columns * alien_width + (columns - 1) * alien_spacing.x
	var start_position = Vector2((screen_width - grid_width) / 2 + 100, 50)

	for row in range(rows):
		for col in range(columns):
			var alien
			if row == 0:
				alien = alien_scene2.instantiate()  # Top row uses Alien2.tscn
			elif row == 1:
				alien = alien_scene3.instantiate()  # Second row uses Alien3.tscn
			else:
				alien = alien_scene1.instantiate()  # Bottom two rows use Alien.tscn

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


func spawn_shields():
	var screen_size = get_viewport_rect().size
	var shield_count = 4
	var extra_spacing = 100  # Increase this number to space them even mor
	var total_spacing = (screen_size.x - (shield_count * extra_spacing)) / (shield_count + 1)
	var y_position = screen_size.y - 150  # Adjust this height as needed

	for i in range(shield_count):
		var shield = shield_scene.instantiate()
		var x_position = total_spacing * (i + 1) + extra_spacing * i + extra_spacing / 2
		shield.position = Vector2(x_position, y_position)
		add_child(shield)



func _process(delta):
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


func move_aliens():
	var screen_width = get_viewport_rect().size.x
	var alien_width = 32 * alien_scale.x

	for alien in alien_container.get_children():
		alien.position.x += direction * speed * 0.1

	if alien_container.get_child_count() == 0:
		return

	var left_edge = INF
	var right_edge = -INF

	for alien in alien_container.get_children():
		left_edge = min(left_edge, alien.position.x)
		right_edge = max(right_edge, alien.position.x)

	if (left_edge <= alien_width / 2 and direction == -1) or (right_edge + alien_width / 2 >= screen_width and direction == 1):
		direction *= -1
		for alien in alien_container.get_children():
			alien.position.y += alien_spacing.y
		move_timer = 0


func _on_drop_timer_timeout():
	for alien in alien_container.get_children():
		alien.position.y += alien_spacing.y
		

func add_score(points):
	score += points
	score_label.text = "Score:" + str(score)


func next_level() -> void:
	level += 1
	speed += 0.5
	level_label.text = "Level:" + str(level)
	direction = 1

	# Reset timers when the level starts
	move_timer = 0.0
	ufo_timer = 0.0
	ufo_spawn_delay = randf_range(10, 20)

	await get_tree().create_timer(1.0).timeout

	spawn_alien_grid()
	level_in_progress = false



func _on_ship_hit(lives_left):
	lives_label.text = "Lives:" + str(lives_left)
	
	if lives_left <= 0:
		game_over()


func game_over():
	print("Game Over!")
	if ship != null:
		ship.queue_free()

	# Stop all aliens by disconnecting timers or clearing move logic
	drop_timer.stop()
	
	# Show overlay
	game_over_overlay.visible = true

	# Load and display leaderboard (dummy example)
	#var scores = load_top_scores()
	#var leaderboard_text = "Top 5 Scores:\n"
	#for i in range(scores.size()):
		#leaderboard_text += str(i + 1) + ". " + str(scores[i]) + "\n"
	#leaderboard_label.text = leaderboard_text

	# Optionally stop the entire tree
	get_tree().paused = true

		
func spawn_ufo():
	var ufo = ufo_scene.instantiate()

	# Apply the same scale as other aliens
	ufo.scale = alien_scale

	# Random start side
	if randi() % 2 == 0:
		ufo.position = Vector2(-50, 32)  # Left offscreen
		ufo.speed = 150  # Move right
	else:
		ufo.position = Vector2(get_viewport_rect().size.x + 50, 32)  # Right offscreen
		ufo.speed = -150  # Move left

	# Play the animation just like the normal aliens
	var sprite_node = ufo.get_node("Alien1")
	if sprite_node != null:
		sprite_node.play("default")

	ufo.connect("ufo_destroyed", Callable(self, "_on_UFO_destroyed"))
	add_child(ufo)

func _on_UFO_destroyed(points):
	print("UFO hit! Awarded points: ", points)
	add_score(points)

func _on_restart_pressed():
	get_tree().paused = false
	get_tree().reload_current_scene()

func _on_main_menu_pressed():
	get_tree().quit()
