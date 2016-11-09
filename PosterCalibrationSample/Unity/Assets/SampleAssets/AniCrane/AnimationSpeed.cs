using UnityEngine;
using System.Collections;

public class AnimationSpeed : MonoBehaviour
{
    public float speed = 1.0f;

    // Use this for initialization
    void Start()
    {
        foreach (AnimationState state in this.gameObject.GetComponent<Animation>())
        {
            state.speed = speed;
        }
    }
}
