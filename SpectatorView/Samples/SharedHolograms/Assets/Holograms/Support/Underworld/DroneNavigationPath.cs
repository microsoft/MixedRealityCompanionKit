using UnityEngine;
using System.Collections.Generic;

public class DroneNavigationPath : MonoBehaviour
{
    [Tooltip("List of navigation points for the drone to move between.")]
    public List<Transform> NavigationPoints;

    [Tooltip("Indicates if the navigation path includes a point outside of the underworld.")]
    public bool HasOutPoint = false;
}