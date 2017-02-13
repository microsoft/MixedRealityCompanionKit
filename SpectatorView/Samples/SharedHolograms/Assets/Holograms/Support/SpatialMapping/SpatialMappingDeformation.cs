using UnityEngine;
using System.Collections.Generic;
using HoloToolkit;
using System.Linq;
using System.Collections;
using HoloToolkit.Unity;

public class SpatialMappingDeformation : SpectatorView.SV_Singleton<SpatialMappingDeformation>
{

    /// <summary>
    /// When we want to create an explosion effect, we use this particleSystem.
    /// </summary>
    ParticleSystem bounceParticles;

    /// <summary>
    /// And we draw the particle system with this renderer. :)
    /// </summary>
    ParticleSystemRenderer particleRenderer;

    /// <summary>
    /// Keeps track of which 'hit position' we will use for the next
    /// collision that triggers the radial animation.
    /// </summary>
    int HitIndex = 0;
   
    /// <summary>
    /// Keeps track of the time remaining for the hit positions.
    /// x = index 0 y = index 1 and so on.
    /// </summary>
    Vector4 hitTimes = Vector4.zero;

    /// <summary>
    /// The shader variable id for the hit times.
    /// </summary>
    int hitTimesId;

    /// <summary>
    /// Temporary hit positions.  
    /// </summary>
    Vector4[] hitPositions =
    {
        Vector4.zero,
        Vector4.zero,
        Vector4.zero,
        Vector4.zero
    };

    /// <summary>
    /// The shader variable IDs for the hit positions.
    /// </summary>
    int[] HitPositionIds;

    /// <summary>
    /// The amount of global hit time left.  Global hit time allows for
    /// the entire spatial mapping mesh to be rendered.
    /// </summary>
    float globalHitTimeLeft;

    /// <summary>
    /// The shader variable ID for the global hit time.
    /// </summary>
    int globalHitTimeId;

    /// <summary>
    /// How long to set the full spatial mapping rendering to run
    /// Needs to be in sync with the SrCollisionEffect shader
    /// </summary>
    const float MaxGlobalHitTime = 30;

    /// <summary>
    /// How long the hit animation should take to play.
    /// Needs to be in sync with the SrCollisionEffect shader
    /// (2x the EffectHalfTime)
    /// </summary>
    const float MaxEffectTime = 2;

    // Use this for initialization
    void Start ()
    {
        bounceParticles = GetComponent<ParticleSystem>();
        particleRenderer = GetComponent<ParticleSystemRenderer>();
        particleRenderer.mesh = ParticleTriangle();

        HitPositionIds = new int[hitPositions.Length];
        for(int index=0;index<HitPositionIds.Length;index++)
        {
            string shaderVarName = "_HitPosition" + index.ToString();
            HitPositionIds[index] = Shader.PropertyToID(shaderVarName);
        }

        hitTimesId = Shader.PropertyToID("_HitTimes0123");

        globalHitTimeId = Shader.PropertyToID("_GlobalHitTime");
        ResetGlobalRendering();
    }

    void Update()
    {
        // Decrement hit times and set the shader variables appropriately.
        hitTimes -= Vector4.one * Time.deltaTime;
        Shader.SetGlobalVector(hitTimesId, hitTimes);

        globalHitTimeLeft -= Time.deltaTime;
        Shader.SetGlobalFloat(globalHitTimeId, globalHitTimeLeft);
    }

    /// <summary>
    /// Generates a custom mesh for our particles.  It's a 3d-ish triangle
    /// </summary>
    /// <returns>A 3d-ish triangle mesh.</returns>
    Mesh ParticleTriangle()
    {
        Mesh mesh = new Mesh();
        List<Vector3> verts = new List<Vector3>();
        verts.Add(new Vector3(-0.5f, -0.5f, 0.1f));
        verts.Add(new Vector3(0, 0.5f, 0.1f));
        verts.Add(new Vector3(0.5f, -0.5f, 0.1f));

        verts.Add(new Vector3(-0.5f, -0.5f, -0.1f));
        verts.Add(new Vector3(0, 0.5f, -0.1f));
        verts.Add(new Vector3(0.5f, -0.5f, -0.1f));

        mesh.SetVertices(verts);

        mesh.SetIndices(
            new int[] {
                0, 2, 1,
                3, 4, 5,
                0,5,2, 0,3,5,
                1,3,0, 1,4,3,
                2,4,1, 2,5,4
            }, 
            MeshTopology.Triangles, 
            0);

        List<Vector2> uvs = new List<Vector2>();
        for(int index=0;index< verts.Count;index++)
        {
            uvs.Add(new Vector2(0.5f+Random.value*0.5f, 0.5f + Random.value * 0.5f));
        }

        mesh.SetUVs(0,uvs);
        return mesh;
    }
    
    /// <summary>
    /// Spawns particles at the specified positions.
    /// </summary>
    /// <param name="Positions">Where to emit the particles.</param>
    public void EmitParticles(List<Vector3> Positions)
    {
        for (int index = 0; index < Positions.Count; index++)
        {
            ParticleSystem.EmitParams pse = new ParticleSystem.EmitParams();
            pse.position = Positions[index];
            pse.velocity = Random.onUnitSphere*2;
            pse.startSize = 0.05f + Random.value * 0.08f;
            pse.axisOfRotation = Random.onUnitSphere.normalized;
            pse.rotation3D = Random.onUnitSphere;// * 0.1f;
            pse.startColor = new Color(Random.value, Random.value, Random.value, 0.35f);

            pse.angularVelocity3D = Random.onUnitSphere*10;
            bounceParticles.Emit(pse, 1);
        }
    }

    /// <summary>
    /// Enables the hit spatial mapping animation at the specified position.
    /// </summary>
    /// <param name="position">Where to set the hit point.</param>
    public void SetHitPosition(Vector3 position)
    {
        // Need to set the hit time for the next hit index.
        // Don't need to actually set the hitTimes variable in the
        // shader since update will do this.
        switch (HitIndex)
        {
            case 0:
                hitTimes.x = MaxEffectTime;
                break;
            case 1:
                hitTimes.y = MaxEffectTime;
                break;
            case 2:
                hitTimes.z = MaxEffectTime;
                break;
            case 3:
                hitTimes.w = MaxEffectTime;
                break;

        }

        // Then set the HitPosition with the specified position.
        int shaderVarId = HitPositionIds[HitIndex];
        Shader.SetGlobalVector(shaderVarId, new Vector4(position.x, position.y, position.z, 1));

        // Move to the next hit index.
        HitIndex = (HitIndex + 1) % hitPositions.Length;
    }

    // Resets the shader to draw all of spatial mapping for MaxGlobalHitTime seconds.
    public void ResetGlobalRendering()
    {
        globalHitTimeLeft = MaxGlobalHitTime;
    }
}
