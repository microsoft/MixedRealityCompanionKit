using UnityEngine;
using HoloToolkit.Unity;
using HoloToolkit.Sharing;
using HoloToolkit.Unity.InputModule;
using HoloToolkit.Unity.SpatialMapping;

public class UnderworldBase : SpectatorView.SV_Singleton<UnderworldBase>
{
    [Tooltip("Ambient clip to play when the underworld is visible.")]
    public AudioClip Ambient;

    [Tooltip("Explosion clip to play when the explosion particles are active.")]
    public AudioClip explosion;

    AudioSource source;

    // Use this for initialization.
    void Start()
    {
        source = GetComponent<AudioSource>();

        if (GazeManager.Instance)
        {
            // Exclude the NavPath layer (used for UnderDrone navigation) from GazeManager raycasts.
            var navLayer = LayerMask.NameToLayer("NavPath");
            var ignoreNavLayerMask = ~(1 << navLayer);
            GazeManager.Instance.RaycastLayerMasks = new LayerMask[1] { ignoreNavLayerMask };
        }
    }

    // Called every frame.
    void Update()
    {
        if (gameObject.activeSelf)
        {
            // If the Underworld is visible, play Ambient sound.
            if (source && !source.isPlaying)
            {
                source.clip = Ambient;
                source.loop = true;
                source.Play();
            }
        }
    }

    // Called every frame after Update().
    void LateUpdate()
    {
        if (gameObject.activeSelf && IsTargetVisible())
        {
#if !UNITY_EDITOR
            UnityEngine.VR.WSA.HolographicSettings.SetFocusPointForFrame(GazeManager.Instance.HitInfo.point, -Camera.main.transform.forward);
#endif
        }
    }

    // Called whenever the underworld is enabled.
    void OnEnable()
    {
        if (source)
        {
            source.clip = explosion;
            source.loop = false;
            source.Play();
        }
    }

    /// <summary>
    /// Enables the target and disables the underworld.
    /// </summary>
    public void ResetUnderworld()
    {
        if (gameObject.activeSelf)
        {
            // Grab all components from this GameObject's parent and enable them.
            MonoBehaviour[] behaviours = transform.parent.gameObject.GetComponentsInChildren<MonoBehaviour>(true);
            foreach (var behaviour in behaviours)
            {
                // If the component's GameObject is disabled, enable it
                if (!behaviour.gameObject.activeSelf)
                {
                    behaviour.gameObject.SetActive(true);
                }
            }

            // Disable the underworld.
            gameObject.SetActive(false);

            // Turn spatial mapping meshes back on.
            SpatialMappingManager.Instance.gameObject.SetActive(true);
        }
    }

    /// <summary>
    /// Checks to see if the target's mesh is visible within the Main Camera's view frustum.
    /// </summary>
    /// <returns>True, if the target's mesh is visible.</returns>
    bool IsTargetVisible()
    {
        Vector3 targetViewportPosition = Camera.main.WorldToViewportPoint(gameObject.transform.position);
        return (targetViewportPosition.x > 0.0 && targetViewportPosition.x < 1 &&
            targetViewportPosition.y > 0.0 && targetViewportPosition.y < 1 &&
            targetViewportPosition.z > 0);
    }
}