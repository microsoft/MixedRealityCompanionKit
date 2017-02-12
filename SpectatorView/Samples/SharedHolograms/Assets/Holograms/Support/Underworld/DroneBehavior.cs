using UnityEngine;
using System.Collections.Generic;

[RequireComponent(typeof(Rigidbody))]
public class DroneBehavior : MonoBehaviour
{
    [Tooltip("Determines how fast the drone will move.")]
    public float Speed = 0.2f;

    [Tooltip("GameObject which contains the list of navigation points for the drone to move between.")]
    public GameObject NavPath;

    List<Transform> NavPositions;
    Rigidbody droneRigidBody;
    GameObject explosion;
    bool moving = false;
    bool animating = false;
    bool doneAnimating = false;
    bool exploding = false;
    bool moveForwards = true;
    int navIndex = 1;
    float animStartTime = 0.0f;
    float animTime = 4.0f;
    float explodeStartTime = 0.0f;
    float explodeTime = 1.0f;
    bool hasOutPoint = false;

    public int NavPathIndex { get; set; }
    public int Life { get; set; }

    // Use this for initialization.
    void Start()
    {
        Life = 3;
        SetNavPath(NavPath);
        explosion = gameObject.GetComponentInChildren<AudioSource>().gameObject;
        droneRigidBody = gameObject.GetComponent<Rigidbody>();
        droneRigidBody.useGravity = false;
        droneRigidBody.isKinematic = false;
        gameObject.transform.position = NavPositions[0].position;
        transform.LookAt(NavPositions[navIndex]);
    }

    void Update()
    {
#if UNITY_EDITOR
        //Keyboard hack to test life drain and explosion.
        if (Input.GetKeyDown("e"))
        {
            Debug.Log("Drone hit!");
            Life--;
        }
#endif
    }

    // Sets the navigation path for this drone.
    void SetNavPath(GameObject path)
    {
        NavPositions = new List<Transform>();
        DroneNavigationPath navPoints = path.GetComponent<DroneNavigationPath>();
        NavPositions.AddRange(navPoints.NavigationPoints);
        hasOutPoint = navPoints.HasOutPoint;
    }

    // Called whenever the drone is enabled.
    void OnEnable()
    {
        moving = true;
        animating = false;
    }

    // Called whenever the drone is disabled.
    void OnDisable()
    {
        // Reset the drone to its starting position.
        navIndex = 1;
        gameObject.transform.position = NavPositions[0].position;
        transform.LookAt(NavPositions[navIndex]);
        moveForwards = true;
    }

    // Uses physics to push the drone in the direction of its next navigation point.
    void FixedUpdate()
    {
        CheckDroneLife();

        if (animating && (Time.time - animStartTime) >= animTime)
        {
            animating = false;
            droneRigidBody.isKinematic = false;
        }

        if (moving && !animating)
        {
            // Move and rotate the drone towards the next navigation point.
            Vector3 targetPosition = NavPositions[navIndex].position;
            RotateDrone(targetPosition);
            Vector3 direction = targetPosition - transform.position;

            // If the drone has moved outside of the underworld, rotate to look at user.
            if (hasOutPoint && NavPositions[navIndex].gameObject.name.EndsWith("_out"))
            {
                RotateDrone(Camera.main.transform.position);
                direction = Vector3.up;
            }

            droneRigidBody.velocity = Speed * direction;

            // If the drone is close to its destination, snap position and change rotation/direction.
            if (Vector3.Distance(transform.position, targetPosition) < 0.1f)
            {
                transform.position = targetPosition;
                transform.LookAt(NavPositions[navIndex]);
                SetNextNavPoint();
            }

            // If the drone got off of its path, reset its rotation to look at targetPosition.
            if(transform.position.y > targetPosition.y)
            {
                transform.LookAt(NavPositions[navIndex]);
            }
        }
    }

    // Checks to see if the drone has enough life to survive.
    void CheckDroneLife()
    {
        if (Life <= 0)
        {
            if (!exploding)
            {
                // Explosion!
                droneRigidBody.isKinematic = true;
                gameObject.GetComponentInChildren<FriendlyDrone>().gameObject.SetActive(false);
                explosion.GetComponent<ParticleSystem>().Play();
                explosion.GetComponent<AudioSource>().Play();
                explodeStartTime = Time.time;
                exploding = true;
            }

            if (exploding && (Time.time - explodeStartTime) >= explodeTime)
            {
                // After explosion is done, replace the drone with a new one.
                exploding = false;
                DroneSpawner.Instance.SpawnDrone(NavPathIndex);
                Destroy(gameObject);
            }
        }
    }

    // Called whenever the drone enters a trigger collider.
    void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.name.Equals("AnimationTrigger") && !doneAnimating)
        {
            TriggerAnimation();
        }
        else
        {
            DroneSpawner.Instance.SpawnDrone(NavPathIndex);
            Destroy(gameObject);
        }
    }

    // Called whenever the drone collides with another object.
    void OnCollisionEnter(Collision other)
    {
        if(other.gameObject.GetComponent<ProjectileBehavior>())
        {
            // If the drone was hit by a projectile, decrease its life.
            Life--;
        }
    }

    /// <summary>
    /// Smoothly rotates the drone to face its target.
    /// </summary>
    /// <param name="targetPoint"></param>
    void RotateDrone(Vector3 targetPoint)
    {
        var targetRotation = Quaternion.LookRotation(targetPoint - transform.position, Vector3.up);
        transform.rotation = Quaternion.Slerp(transform.rotation, targetRotation, Time.deltaTime * 2.0f);
    }

    /// <summary>
    /// Plays the 'Happy' animation when the drone reaches the AnimationTrigger.
    /// </summary>
    void TriggerAnimation()
    {
        droneRigidBody.isKinematic = true;
        transform.LookAt(Camera.main.transform);
        gameObject.GetComponentInChildren<FriendlyDrone>().Happy();
        animating = true;
        animStartTime = Time.time;
        SetNextNavPoint();
    }

    /// <summary>
    /// Sets the next navigation point based on where the current index is in the NavPositions list.
    /// </summary>
    void SetNextNavPoint()
    {
        if (moveForwards)
        {
            navIndex++;
            if (navIndex >= NavPositions.Count)
            {
                navIndex--;
                moveForwards = false;
            }
        }
        else
        {
            // Moves drone backwards along the NavPositions list.
            navIndex--;
            if (navIndex < 0)
            {
                navIndex++;
                moveForwards = true;
            }
        }
    }
}