using UnityEngine;

public class GrenadeRotate : MonoBehaviour
{
    public Color EmissiveColor = new Vector4(0f, .87f, 1f, 1f);

    void Start()
    {
        var materials = GetComponent<Renderer>().materials;
        materials[1].SetColor("_EmissionColor", EmissiveColor);
        GameObject childParticle = transform.Find("ParticleTrail").gameObject;
        if (childParticle != null)
        {
            childParticle.GetComponent<ParticleSystem>().startColor = EmissiveColor;
        }
    }
}