extends Area2D

var speed = 600  # Bullet movement speed

@onready var sprite = $Sprite2D
@onready var collision_shape = $CollisionShape2D
@onready var explosion = $ExplosionParticles  # Reference to CPUParticles2D

func _ready():
	# Connect collision detection signal
	connect("area_entered", Callable(self, "_on_Bullet_area_entered"))
	
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
		area.queue_free()  # Remove the alien
		explode()

func explode():
	sprite.visible = false  # Hide bullet
	collision_shape.set_deferred("disabled", true)  # Disable collision
	explosion.emitting = true  # Play explosion effect

	# Wait for explosion effect to finish, then remove bullet
	await get_tree().create_timer(0.5).timeout
	queue_free()
