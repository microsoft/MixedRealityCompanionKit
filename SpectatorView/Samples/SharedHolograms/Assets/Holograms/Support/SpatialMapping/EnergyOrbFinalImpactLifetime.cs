using UnityEngine;

public class EnergyOrbFinalImpactLifetime : MonoBehaviour
{
    void Start()
    {
        Invoke("DestroyGameObject", 0.5f);
    }

    void DestroyGameObject()
    {
        Destroy(gameObject);
    }
}