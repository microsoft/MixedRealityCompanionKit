using UnityEngine;

public class ExplodingBlob : MonoBehaviour
{
    /// <summary>
    /// TargetDangerLevel tracks what 'danger level'  we are heading towards.
    /// Once we hit max danger level we'll do a fun effect.
    /// </summary>
    float TargetDangerLevel = 0;
    float SetDangerLevel = 0 ;

    /// <summary>
    /// To prevent the effects from changing too quickly we'll clamp the 
    /// max per frame change to this value.
    /// </summary>
    public float MaxDangerLevelChangePerFrame = 0.1f;

    /// <summary>
    /// When the blob is hit we'll increase the target danger level by this value.
    /// </summary>
    public float DangerLevelIncreaseFactor = 10;

    /// <summary>
    /// This is the largest danger level. 
    /// </summary>
    public float MaxDangerLevel = 128;

    /// <summary>
    /// Variables for caching the start state of the material on the blob.
    /// </summary>
    Color StartWireColor;
    Color StartColor;
    float StartDisplacement;

    /// <summary>
    /// Property Id's for the shader variables we will be changing.
    /// </summary>
    int WireColorPropertyId;
    int ColorPropertyId;
    int DisplacementPropertyId;

    // Use this for initialization
    void Start ()
    {
        // Get the shader IDs for these properties. 
        WireColorPropertyId = Shader.PropertyToID("_WireColor");
        ColorPropertyId = Shader.PropertyToID("_Color");
        DisplacementPropertyId = Shader.PropertyToID("_Displacement");

        // Get the initial state of the blob.
        Material BlobMaterial = GetComponent<MeshRenderer>().material;
        StartWireColor = BlobMaterial.GetColor(WireColorPropertyId);
        StartColor = BlobMaterial.GetColor(ColorPropertyId);
        StartDisplacement = BlobMaterial.GetFloat(DisplacementPropertyId);
    }

    // Called whenever the gameObject is enabled.
    void OnEnable()
    {
        TargetDangerLevel = 0;
        SetDangerLevel = 0;
    }

    // Update is called once per frame.
    void Update ()
    {
        // Keep the TargetDangerLevel within tolerance.
        TargetDangerLevel = Mathf.Clamp(TargetDangerLevel, 0, MaxDangerLevel);

        if (TargetDangerLevel == MaxDangerLevel)
        {
            TargetDangerLevel = 0;
            SetDangerLevel = 0;
            ExplodeTarget.Instance.OnExplode();
            CustomMessages.Instance.SendExplodeTarget();            
            Debug.Log("BOOOOM");
        }

        // Adjust the set danger level up by the MaxDangerLevelChangePerFrame.
        if (SetDangerLevel < TargetDangerLevel)
        {
            SetDangerLevel += MaxDangerLevelChangePerFrame;
        }
        else
        {
            // DangerLevel already decays in the else case at the MaxDangerLevelChangePerFrame rate.
            SetDangerLevel = TargetDangerLevel;
        }

        Material mat = GetComponent<MeshRenderer>().material;

        // If we are at 0 we should restore defaults.
        if (SetDangerLevel == 0)
        {
            mat.SetColor(ColorPropertyId, StartColor);
            mat.SetFloat(DisplacementPropertyId, StartDisplacement);
            mat.SetColor(WireColorPropertyId, StartWireColor);
        }
        else
        {
            // Otherwise, we want to get more red and less and green as the danger level increases.
            float r = SetDangerLevel / MaxDangerLevel;
            float g = (MaxDangerLevel - SetDangerLevel) / MaxDangerLevel;

            float rootR = Mathf.Sqrt(r);
            float b = 1 - rootR;

            float outerAlpha = Mathf.Max(r, StartWireColor.a);

            // Color.a needs to be lower than WireColor.a for our shader to be happy.
            mat.SetColor(ColorPropertyId, new Color(r, g, b, outerAlpha * 0.3f));
            mat.SetFloat(DisplacementPropertyId, Mathf.Max(r*0.2f, StartDisplacement));
            mat.SetColor(WireColorPropertyId, new Color(rootR, g * 0.4f, b * 0.8f, outerAlpha));

            // Decay the danger.
            TargetDangerLevel -= MaxDangerLevelChangePerFrame;
        }      
    }

    // Called whenever this object is involved in a collision.
    void OnCollisionEnter(Collision collision)
    {
        // We only care if the collision is with a projectile.
        ProjectileBehavior pb = collision.contacts[0].otherCollider.gameObject.GetComponent<ProjectileBehavior>();
        if (pb != null)
        {
            TargetDangerLevel += DangerLevelIncreaseFactor;
        }
    }
}
