using UnityEngine;

public class Blob : MonoBehaviour
{
    public float scrollSpeed = 0.05F;
    public Renderer rend;

    void Start()
    {
        rend = GetComponent<Renderer>();
        float boundwidth = 5.0f;
        Mesh mesh = GetComponent<MeshFilter>().mesh;
        mesh.bounds = new Bounds(new Vector3(0, 0, 0), Vector3.one * boundwidth);
    }

    void Update()
    {
        float offset = Mathf.Repeat(Time.time * scrollSpeed, 4);
        rend.sharedMaterial.SetVector("_Offset", new Vector4(offset, offset * 0.05f, 0, 0));
    }
}