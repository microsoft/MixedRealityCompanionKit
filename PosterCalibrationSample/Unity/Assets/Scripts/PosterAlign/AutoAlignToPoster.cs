using UnityEngine;
using System.Linq;

[RequireComponent(typeof(ImagePosterLocator))]
public class AutoAlignToPoster : MonoBehaviour
{
    [Tooltip("GameObject (Quad) in scene with the poster texture (texture must be read/write enabled).")]
    public GameObject Poster = null;

    [Tooltip("Shows rays/spheres/cubes denoting the various positions of alignment.")]
    public bool ShowDebugObjects = false;

    private GameObject[] cornerObjects = new GameObject[4];
    private GameObject[] debugRayObjects = new GameObject[4];
    private GameObject[] debugPositionObjects = new GameObject[4];

    private ImagePosterLocator posterLocator;
    private int updateVersion = -1;

    private bool isInitialized = false;
    void Initialize()
    {
        if (isInitialized)
            return;

        posterLocator = gameObject.GetComponent<ImagePosterLocator>();

        if (Poster != null &&
            Poster.GetComponent<Renderer>() != null &&
            Poster.GetComponent<Renderer>().material.mainTexture != null &&
            Poster.GetComponent<MeshFilter>() != null &&
            Poster.GetComponent<MeshFilter>().mesh.vertexCount == 4)
        {
            Texture2D posterTexture = (Texture2D)Poster.GetComponent<Renderer>().material.mainTexture;
            posterLocator.SetPosterTexture(posterTexture);

            Mesh mesh = Poster.GetComponent<MeshFilter>().mesh;
            for (int i = 0; i < 4; i++)
            {
                Vector3 vertexPosW = Poster.transform.TransformPoint(mesh.vertices[i]);
                var posterVert = GameObject.CreatePrimitive(PrimitiveType.Cube);
                posterVert.transform.localScale = new Vector3(.005f, .005f, .005f);
                posterVert.transform.position = vertexPosW;
                posterVert.transform.SetParent(Poster.transform);
                cornerObjects[i] = posterVert;
            }
            isInitialized = true;
        }
        else
        {
            Debug.LogError("Poster not valid - should be a quad with a read/write enabled texture.");
        }
    }

    void OnDisable()
    {
        StopProcessing();
    }

    public void StartProcessing()
    {
        Initialize();
        posterLocator.enabled = true;
        posterLocator.StartProcessing();
        ShowCorners();
    }

    public void StopProcessing()
    {
        foreach (var ob in debugRayObjects)
        {
            GameObject.Destroy(ob);
        }

        foreach (var ob in debugPositionObjects)
        {
            GameObject.Destroy(ob);
        }

        HideCorners();
        if (posterLocator != null)
        {
            posterLocator.StopProcessing();
            posterLocator.enabled = false;
        }
    }

    private void ShowCorners()
    {
        foreach (var ob in cornerObjects)
        {
            if (ob != null)
                ob.SetActive(true);
        }
    }

    private void HideCorners()
    {
        foreach (var ob in cornerObjects)
        {
            if (ob != null)
                ob.SetActive(false);
        }
    }

    private void Update()
    {
        if (this.updateVersion != this.posterLocator.UpdateVersion)
        {
            this.updateVersion = this.posterLocator.UpdateVersion;
            Debug.Log("Update version updated.");

            UpdateLocationFromPoster();

            if (ShowDebugObjects)
            {
                UpdateDebugObjects();
            }
        }
    }

    private void AlignCorners(GameObject[] objectsToMove, Vector3 newCenter)
    {
        var oldCenter = objectsToMove.Select(k => k.transform.position)
            .Aggregate((a, b) => (a + b)) / objectsToMove.Length;
        var delta = newCenter - oldCenter;
        transform.position += delta;
    }

    public void UpdateLocationFromPoster()
    {
        if (posterLocator.DetectedPositions == null)
            return;

        var corners = posterLocator.DetectedPositions.Select(k => k.EstimatedWorldPos).ToList();

        Debug.Log("Applying placement...");

        // First recenter the object:
        var toCenter = corners.Aggregate((a, b) => (a + b))
            / corners.Count;
        AlignCorners(cornerObjects, toCenter);

        // Now for rotation:
        Quaternion avgRot = Quaternion.identity;
        float oneOverI = 1.0f / ((float)corners.Count);
        for (int i = 0; i < corners.Count; i++)
        {
            var from = cornerObjects[i].transform.position - toCenter;
            var to = corners[i] - toCenter;

            from.y = 0.0f;
            to.y = 0.0f;

            var rot = Quaternion.FromToRotation(from, to);
            avgRot = avgRot * Quaternion.Slerp(Quaternion.identity, rot, oneOverI);
            //avgRot = rot;
        }
        transform.rotation *= avgRot;

        // And do a final re-center:
        AlignCorners(cornerObjects, toCenter);
    }

    private void UpdateDebugObjects()
    {
        if (this.posterLocator == null || this.posterLocator.DetectedPositions == null)
            return;
        for (var i = 0; i < 4; i++)
        {
            var corner = this.posterLocator.DetectedPositions[i];
            if (corner != null && corner.Rays != null)
            {
                if (debugRayObjects[i] == null)
                {
                    var rayObj = GameObject.CreatePrimitive(PrimitiveType.Cube);
                    var sphereCol = rayObj.GetComponent<Collider>();
                    if (sphereCol != null)
                    {
                        rayObj.GetComponent<Collider>().enabled = false;
                    }
                    rayObj.transform.forward = corner.LatestRay.direction;
                    rayObj.transform.localScale = new Vector3(0.0005f, 0.0005f, 2.0f);
                    debugRayObjects[i] = rayObj;
                }

                debugRayObjects[i].transform.position = corner.LatestRay.origin + (corner.LatestRay.direction * 1.5f);
                debugRayObjects[i].transform.forward = corner.LatestRay.direction;

                if (corner.HasWorldPos)
                {
                    if (debugPositionObjects[i] == null)
                    {
                        var posObj = GameObject.CreatePrimitive(PrimitiveType.Sphere);
                        posObj.transform.localScale = Vector3.one * 0.01f;

                        var posObjCol = posObj.GetComponent<Collider>();
                        if (posObjCol != null)
                        {
                            //sphereCol.enabled = false;
                            posObjCol.bounds.Expand(.1f);
                        }

                        debugPositionObjects[i] = posObj;
                    }
                    debugPositionObjects[i].transform.position = corner.EstimatedWorldPos;
                }
                else
                {
                    // no world location, make sure there isn't an object for it:
                    if (debugPositionObjects[i] != null)
                    {
                        GameObject.Destroy(debugPositionObjects[i]);
                        debugPositionObjects[i] = null;
                    }
                }
            }
        }
    }
}