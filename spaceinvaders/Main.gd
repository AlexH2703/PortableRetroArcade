extends Node2D

@onready var alien_scene = preload("res://Alien.tscn")
@onready var alien_container = $AlienContainer

@export var rows := 4
@export var columns := 10  # Number of columns of aliens
@export var drop_timer_interval := 1.0  # How often aliens drop
@export var alien_scale := Vector2(3, 3)  # Scale to make aliens bigger
@export var speed := 5  # Slow down the horizontal movement speed

var drop_timer := Timer.new()
var alien_spacing := Vector2()  # Will be calculated dynamically
var direction := 1  # 1 for moving right, -1 for moving left
var move_timer := 0.0  # Timer to control horizontal movement

func _ready():
	# Make sure to add the drop timer to the scene
	add_child(drop_timer)
	drop_timer.wait_time = drop_timer_interval
	drop_timer.timeout.connect(_on_drop_timer_timeout)
	#drop_timer.start()

	# Spawn the alien grid
	spawn_alien_grid()

func spawn_alien_grid():
	# Get the current screen size (viewport size)
	var screen_size = get_viewport_rect().size
	var screen_width = screen_size.x
	var screen_height = screen_size.y
	
	# Instantiate an alien to check its size when scaled (no need for get_rect)
	var alien_instance = alien_scene.instantiate()
	var alien_animated_sprite = alien_instance.get_node("Alien1")  # Get the AnimatedSprite2D node ("Alien1")
	
	# Ensure the AnimatedSprite2D node is valid
	if alien_animated_sprite == null:
		print("Error: Alien1 node not found in Alien scene.")
		return
	
	# Apply scaling to the alien (adjust its size)
	alien_animated_sprite.scale = alien_scale
	
	# Assume a fixed width for the alien sprite, just use a default width based on scale
	var alien_width = 32 * alien_scale.x  # Adjust this as necessary
	
	# Manually reduce the horizontal and vertical spacing to make aliens closer together
	alien_spacing.x = 1  # Tighter horizontal spacing
	alien_spacing.y = 50  # Tighter vertical spacing
	
	# Calculate the width and height of the entire alien grid
	var grid_width = columns * alien_width + (columns - 1) * alien_spacing.x  # Adjusted for tight spacing
	var grid_height = rows * alien_spacing.y
	
	# Dynamically calculate the start position (center the grid horizontally and position from the top)
	var start_position = Vector2((screen_width - grid_width) / 2 + 100, 50)  # Push it a little to the right and center

	# Spawn the aliens
	for row in range(rows):
		for col in range(columns):
			var alien = alien_scene.instantiate()
			alien.position = start_position + Vector2(col * (alien_width + alien_spacing.x), row * alien_spacing.y)
			alien.scale = alien_scale  # Scale each alien
			alien_container.add_child(alien)
			
			# Start the animation for each alien
			var alien_sprite = alien.get_node("Alien1")  # Get the AnimatedSprite2D node
			if alien_sprite != null:
				alien_sprite.play("default")  # Play the "default" animation

func _process(delta):
	# Move the aliens left or right depending on direction
	move_timer += delta
	if move_timer >= 0.1:  # Update position every 0.1 seconds
		move_aliens()

func move_aliens():
	# Check if aliens have reached the edge of the screen
	var screen_width = get_viewport_rect().size.x
	var alien_width = 32 * alien_scale.x  # Adjust width based on scale

	# Move all aliens in the container
	for alien in alien_container.get_children():
		alien.position.x += direction * speed * 0.1  # Move horizontally

	# Check if any alien reaches the edge
	var left_edge = alien_container.get_child(0).position.x
	var right_edge = alien_container.get_child(columns - 1).position.x

	if left_edge <= 0 or right_edge >= screen_width:  # If the edge is reached, reverse direction and move down
		direction *= -1  # Reverse direction
		# Move aliens down
		for alien in alien_container.get_children():
			alien.position.y += alien_spacing.y  # Move down a little bit

		move_timer = 0  # Reset move timer to delay next movement

func _on_drop_timer_timeout():
	# Move all aliens down every time the timer triggers
	for alien in alien_container.get_children():
		alien.position.y += alien_spacing.y  # Move all aliens down
