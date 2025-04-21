extends Area2D

var speed = 600  # Bullet movement speed

@onready var sprite = $Bullet
@onready var collision_shape = $BulletCollision
@onready var explosion = $ExplosionParticles  # Reference to CPUParticles2D

# Define a variable for the score
var score = 0

# Optional: Reference to a score label (you can connect this to the UI)
@onready var score_label = $ScoreLabel  # Assuming there's a label for the score

func _ready():
	# Connect collision detection signal
	connect("area_entered", Callable(self, "_on_Bullet_area_entered"))
	connect("body_entered", Callable(self, "_on_bullet_body_entered"))
	
	# Make sure the explosion is not visible at the start
	explosion.emitting = false

func _process(delta):
	# Move the bullet upwards
	position.y -= speed * delta
	
	# If the bullet goes off-screen, queue for deletion
	if position.y < -10:
		queue_free()

func _on_Bullet_area_entered(area):
	if area.is_in_group("back_edge"):  # If bullet hits screen edge
		explode()
	elif area.is_in_group("enemy"):  # If bullet hits an enemy
		area._on_Hit()  # Call the enemy's hit logic
		update_score_display()
		area.queue_free()  # Remove the enemy
		explode()
	elif area.is_in_group("ShieldBlock"):  # If bullet hits a shield block
		area._on_hit()  # Let the shield block handle damage
		explode()
		
func _on_bullet_body_entered(body):
	if body.is_in_group("ShieldBlock"):
		body._on_hit()
		queue_free()

func explode():
	sprite.visible = false  # Hide bullet
	collision_shape.set_deferred("disabled", true)  # Disable collision
	explosion.emitting = true  # Play explosion effect

	# Wait for explosion effect to finish (based on particle lifetime), then remove bullet
	await get_tree().create_timer(10.5).timeout  # Match the explosion's lifetime
	queue_free()  # Delete bullet after explosion

func update_score_display():
	# Update the score label in the UI
	if score_label:
		score_label.text = str(score)  # Update the score label with the new score value
