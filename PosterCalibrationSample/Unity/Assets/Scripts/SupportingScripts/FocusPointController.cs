using Showcases.Input;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.VR.WSA;

public class FocusPointController : MonoBehaviour
{
    public float DefaultStartDistance = 2.0f;

    public float MinDistance = 0.1f;

    public bool Snap = false;

    public float FocusPointDistance { get; private set; }

    public Vector3 FocusPointPosition { get; private set; }

    public Vector3 FocusPointNormal { get { return -Camera.main.transform.forward; } }

    private const float LerpPowerCloser = 7.0f;
    private const float LerpPowerFarther = 10.0f;

    void Awake()
    {
        // uses the main camera for focus plane adjustmensts
        if(Camera.main == null)
        {
            Debug.LogError("You need to choose a main camera that will be used for the scene");
            return;
        }

        this.MinDistance = Camera.main.nearClipPlane + this.MinDistance;
        this.FocusPointDistance = this.DefaultStartDistance;
        this.FocusPointPosition = Camera.main.transform.position + (Camera.main.transform.forward * this.FocusPointDistance);
    }

    void LateUpdate()
    {
        // use the last location
        var newFocusPos = Camera.main.transform.position + (Camera.main.transform.forward * this.FocusPointDistance);

        // if we hit an object, set this as the new position
        RaycastResult raycast = CustomHoloLensInputModule.GetRaycastResult();
        if (raycast.gameObject != null)
        {
            newFocusPos = raycast.worldPosition;
        }

        // determine its distance to that point
        var newFocusPointDistance = (Camera.main.transform.position - newFocusPos).magnitude;

        // if we want to set it immediately, set snap mode
        if (this.Snap)
        {
            this.FocusPointDistance = newFocusPointDistance;
        }
        else
        {
            // otherwise, lerp based on whether we are going away from the camera
            if (newFocusPointDistance > this.FocusPointDistance)
            {
                this.FocusPointDistance = Mathf.Lerp(this.FocusPointDistance, newFocusPointDistance, LerpPowerFarther * Time.deltaTime);
            }
            else
            {
                this.FocusPointDistance = Mathf.Lerp(newFocusPointDistance, this.FocusPointDistance, LerpPowerCloser * Time.deltaTime);
            }
        }

        if (this.FocusPointDistance <= this.MinDistance)
        {
            this.FocusPointDistance = this.MinDistance;
        }

        // set the position
        this.FocusPointPosition
            = Camera.main.transform.position + (Camera.main.transform.forward * this.FocusPointDistance);

        HolographicSettings.SetFocusPointForFrame(this.FocusPointPosition, this.FocusPointNormal);
    }
}
