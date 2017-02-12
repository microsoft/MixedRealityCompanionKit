Shader "Holographic/SrCollisionEffect"
{
    Properties
    {
        _EffectHalfTime("EffectHalfTime", Range(1, 10)) = 1
        _EffectRadius("EffectRadius", Range(0.1, 2.0)) = 0.75
        _GloabalEffectMaxTime("GlobalEffectMaxTime", Range(1, 600)) = 30
        _LineWidth("LineWidth", Range(0.01, 0.1)) = 0.05
    }

    SubShader
    {
        Tags{ "RenderType" = "Opaque" }

        LOD 100

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"
            #pragma target 5.0

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

            // Line width controls how wide the lines should be.
            float _LineWidth;

            // The only input we need for the vertex shader is position.
            struct appdata
            {
                float4 vertex : POSITION;
            };

            // And we output view space and world space.
            struct v2f
            {
                float4 vertex : SV_POSITION;
                float4 worldPos: TEXCOORD1;
            };

            // Vertex program.  Calculates view space and world space for
            // frag program.
            v2f vert(appdata v)
            {
                v2f o;
                o.vertex = mul(UNITY_MATRIX_MVP, v.vertex);
                o.worldPos = mul(unity_ObjectToWorld, v.vertex);
                return o;
            }

            // Calculates how bright a pixel should be based on its distance from
            // a hit position and a timer.
            float CalculateHitRatio(float HitTime, float3 HitPos, float3 WorldPos)
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
                        return hitRat;
                    }
                }

                // Here we just return 0.  Either no time left or this pixel is too far away.
                return 0;
            }

            // Calculates how bright a pixel should be based on all of the hit positions.
            fixed4 CalculateHitColors(v2f i)
            {
                // If 'global time' is active, we'll just return grey.
                if (_GlobalHitTime > 0)
                {
                    return fixed4(fixed3(1, 1, 1) * _GlobalHitTime / 30.0f, 1);
                }

                // Otherwise, we'll add up all of the four hit positions.
                fixed4 hitColor = fixed4(0, 0, 0, 0);

                hitColor = saturate(hitColor + fixed4(1, 0, 0, 1) * CalculateHitRatio(_HitTimes0123.x, _HitPosition0, i.worldPos.xyz));
                hitColor = saturate(hitColor + fixed4(0, 1, 0, 1) * CalculateHitRatio(_HitTimes0123.y, _HitPosition1, i.worldPos.xyz));
                hitColor = saturate(hitColor + fixed4(0, 0, 1, 1) * CalculateHitRatio(_HitTimes0123.z, _HitPosition2, i.worldPos.xyz));
                hitColor = saturate(hitColor + fixed4(1, 1, 0, 1) * CalculateHitRatio(_HitTimes0123.w, _HitPosition3, i.worldPos.xyz));

                return fixed4(hitColor.xyz,1);
            }

            // Fragment program. Makes a color. :)
            fixed4 frag(v2f i) : SV_Target
            {
                // We only want to draw lines every so often through the world.
                float4 wpmodip;
                float4 wpmod = abs(modf(i.worldPos * 4, wpmodip));

                float halfWidth = _LineWidth * 0.5;
                wpmod = wpmod - float4(halfWidth, halfWidth, halfWidth, 0);

                float closest = abs(wpmod.x);
                closest = min(closest, abs(wpmod.y));
                closest = min(closest, abs(wpmod.z));

                if (closest < halfWidth)
                {
                    closest = 1 - (closest / (halfWidth));

                    // If we are inside of one of the areas in the world we want to draw a line, return
                    // CalculateHitColors (which may still return black).
                    return fixed4(CalculateHitColors(i).xyz*closest, 1);
                }
                else
                {
                    // Here we aren't inside of one of the areas we want a line, so we always return
                    // black.
                    return fixed4(0, 0, 0, 1);
                }
            }
            ENDCG
        }
    }
}