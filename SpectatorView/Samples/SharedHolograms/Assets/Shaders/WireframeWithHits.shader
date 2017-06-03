// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

///
/// Basic wireframe shader that can be used for rendering spatial mapping meshes.
///
Shader "Holographic/WireframeWithHits"
{
    Properties
    {
        _BaseColor("Base color", Color) = (0.0, 0.0, 0.0, 1.0)
        _WireColor("Wire color", Color) = (1.0, 1.0, 1.0, 1.0)
        _WireThickness("Wire thickness", Range(0, 800)) = 100
        _EffectHalfTime("EffectHalfTime", Range(1, 10)) = 1
        _EffectRadius("EffectRadius", Range(0.1, 2.0)) = 0.75
        _GloabalEffectMaxTime("GlobalEffectMaxTime", Range(1, 600)) = 30
    }
    SubShader
    {
        Tags { "RenderType" = "Opaque" }

        Pass
        {
            Offset 50, 100

            CGPROGRAM

            #pragma vertex vert
            #pragma geometry geom
            #pragma fragment frag

            // We only target the HoloLens (and the Unity editor), so take advantage of shader model 5.
            #pragma target 5.0
            #pragma only_renderers d3d11

            #include "UnityCG.cginc"

            float4 _BaseColor;
            float4 _WireColor;
            float _WireThickness;

            // _HitPosition0...3 control where we will animate an SR collision
            float4 _HitPosition0;
            float4 _HitPosition1;
            float4 _HitPosition2;
            float4 _HitPosition3;

            // _HitTimes0123 stores the time left for _HitPosition0...3 to be relevant.
            // _HitTimes0123.x corresponds to _HitPosition0, .y to Position1, and so on.
            float4 _HitTimes0123;

            // Sometimes its nice to just see the whole spatial map.  _GlobalHitTime > 0 will cause
            // the whole map to be rendered.
            float _GlobalHitTime;

            // This is what the _GlobalHitTime should be reset to.  Lets us fade out the SR.
            float _GloabalEffectMaxTime;

            // EffectHalfTime controls how long the effect animation should take
            float _EffectHalfTime;

            // EffectRadius controls how large the effect should be.
            float _EffectRadius;

            // Based on approach described in "Shader-Based Wireframe Drawing", http://cgg-journal.com/2008-2/06/index.html

            struct v2g
            {
                float4 viewPos : SV_POSITION;
                float4 worldPos: TEXCOORD1;
            };

            v2g vert(appdata_base v)
            {
                v2g o;
                o.viewPos = UnityObjectToClipPos(v.vertex);
                o.worldPos = mul(unity_ObjectToWorld, v.vertex);
                return o;
            }

            // inverseW is to counter-act the effect of perspective-correct interpolation so that the lines look the same thickness
            // regardless of their depth in the scene.
            struct g2f
            {
                float4 viewPos : SV_POSITION;
                float inverseW : TEXCOORD0;
                float3 dist : TEXCOORD1;
            };

            bool IsPointInHit(float HitTime, float3 HitPos, float3 WorldPos)
            {
                // If the timer is below zero, nothing to do.  Otherwise...
                if (HitTime > 0)
                {
                    // The animation expands then contracts
                    float hitPhase = abs(HitTime - _EffectHalfTime);

                    // We need to see the % we are through the expansion or contraction
                    float hitRat = 1.0f - (hitPhase / _EffectHalfTime);

                    // The closer we are to the center of the effect time, the larger the radius should be.
                    float hitRadius = hitRat * _EffectRadius;

                    // Check to see if we are within the radius.
                    float d2gp = length(HitPos - WorldPos);
                    if (d2gp < hitRadius)
                    {
                        // If we are return how far along we are through the animation.
                        return true;
                    }
                }

                // Here we just return false.  Either no time left or this pixel is too far away.
                return false;
            }

            bool ArePointsInHit(float3 worldPos)
            {
                if (_GlobalHitTime > 0)
                {
                    return true;
                }

                return (
                    IsPointInHit(_HitTimes0123.x, _HitPosition0, worldPos) ||
                    IsPointInHit(_HitTimes0123.y, _HitPosition1, worldPos) ||
                    IsPointInHit(_HitTimes0123.z, _HitPosition2, worldPos) ||
                    IsPointInHit(_HitTimes0123.w, _HitPosition3, worldPos));
            }

            [maxvertexcount(3)]
            void geom(triangle v2g i[3], inout TriangleStream<g2f> triStream)
            {
                if (ArePointsInHit(i[0].worldPos.xyz) ||
                    ArePointsInHit(i[1].worldPos.xyz) ||
                    ArePointsInHit(i[2].worldPos.xyz))
                {
                    // Calculate the vectors that define the triangle from the input points.
                    float2 point0 = i[0].viewPos.xy / i[0].viewPos.w;
                    float2 point1 = i[1].viewPos.xy / i[1].viewPos.w;
                    float2 point2 = i[2].viewPos.xy / i[2].viewPos.w;

                    // Calculate the area of the triangle.
                    float2 vector0 = point2 - point1;
                    float2 vector1 = point2 - point0;
                    float2 vector2 = point1 - point0;
                    float area = abs(vector1.x * vector2.y - vector1.y * vector2.x);

                    float wireScale = 800 - _WireThickness;

                    // Output each original vertex with its distance to the opposing line defined
                    // by the other two vertices.

                    g2f o;

                    o.viewPos = i[0].viewPos;
                    o.inverseW = 1.0 / o.viewPos.w;
                    o.dist = float3(area / length(vector0), 0, 0) * o.viewPos.w * wireScale;
                    triStream.Append(o);

                    o.viewPos = i[1].viewPos;
                    o.inverseW = 1.0 / o.viewPos.w;
                    o.dist = float3(0, area / length(vector1), 0) * o.viewPos.w * wireScale;
                    triStream.Append(o);

                    o.viewPos = i[2].viewPos;
                    o.inverseW = 1.0 / o.viewPos.w;
                    o.dist = float3(0, 0, area / length(vector2)) * o.viewPos.w * wireScale;
                    triStream.Append(o);
                }
            }

            float4 frag(g2f i) : COLOR
            {
                // Calculate minimum distance to one of the triangle lines, making sure to correct
                // for perspective-correct interpolation.
                float dist = min(i.dist[0], min(i.dist[1], i.dist[2])) * i.inverseW;

                // Make the intensity of the line very bright along the triangle edges but fall-off very
                // quickly.
                float I = exp2(-2 * dist * dist);

                // Fade out the alpha but not the color so we don't get any weird halo effects from
                // a fade to a different color.
                float4 color = I * _WireColor + (1 - I) * _BaseColor;
                color.a = I;
                return color;
            }
            ENDCG
        }
    }
    FallBack "Diffuse"
}