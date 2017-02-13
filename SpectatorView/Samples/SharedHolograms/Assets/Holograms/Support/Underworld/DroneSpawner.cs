using HoloToolkit.Unity;
using System.Collections.Generic;
using UnityEngine;

public class DroneSpawner : SpectatorView.SV_Singleton<DroneSpawner>
{
    [Tooltip("Prefab to use when creating underworld drones.")]
    public GameObject DronePrefab;

    [Tooltip("Collection of navigation paths for underworld drones to use.")]
    public List<GameObject> NavigationPaths;

    [Tooltip("Selection of colors for underworld drones to appear with.")]
    public List<Color> DroneColors;

    [Tooltip("Speed to use for underworld drone movement.")]
    [Range(0.2f, 0.5f)]
    public List<float> DroneSpeeds;

    void Start()
    {
        if (NavigationPaths.Count != DroneColors.Count && NavigationPaths.Count != DroneSpeeds.Count)
        {
            Debug.Log("NavigationPaths, DroneColors, and DroneSpeeds should have the same number of elements!");
        }

        // Spawn a drone for each navigation path.
        for (int i = 0; i < NavigationPaths.Count; i++)
        {
            SpawnDrone(i);
        }
    }

    /// <summary>
    /// Spawns a drone at the first position in the NavigationPath.
    /// </summary>
    /// <param name="navIndex">Index of NavigationPath, Color, and Speed to use for the underdrone.</param>
    public void SpawnDrone(int index)
    {
        // Create drone and set initial position, rotation, scale.
        DroneNavigationPath path = NavigationPaths[index].gameObject.GetComponent<DroneNavigationPath>();
        GameObject drone = (GameObject)Instantiate(DronePrefab);
        drone.transform.parent = this.transform;
        drone.transform.position = path.NavigationPoints[0].position;
        drone.transform.LookAt(path.NavigationPoints[1].transform);
        drone.transform.localScale = new Vector3(0.72f, 0.72f, 0.72f);

        // Set drone's speed and navigation path.
        DroneBehavior behavior = drone.AddComponent<DroneBehavior>();
        behavior.NavPath = NavigationPaths[index].gameObject;
        behavior.Speed = DroneSpeeds[index];
        behavior.NavPathIndex = index;

        // Set drone's color.
        FriendlyDrone friendly = drone.GetComponentInChildren<Animator>().gameObject.AddComponent<FriendlyDrone>();
        friendly.EmissiveColor = DroneColors[index];
    }
}