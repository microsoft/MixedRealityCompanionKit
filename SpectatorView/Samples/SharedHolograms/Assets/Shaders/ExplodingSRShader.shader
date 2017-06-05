// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Unlit/ExplodingSRShader"
{
    /*Properties
    {
        _GazePosition("GazePosition", Vector) = (0,0,0,0)
    }*/
    SubShader
    {
        Tags{ "RenderType" = "Transparent"
        "LightMode" = "ForwardBase" }

        LOD 100
        Blend SrcAlpha OneMinusSrcAlpha // Alpha blending
        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"
            #pragma target 5.0

            float4 _GazePosition;
            struct appdata
            {
                float4 vertex : POSITION;
                float4 color : COLOR0;
                float3 Normal: NORMAL0;
            };

            struct v2f
            {
                float4 color : COLOR0;
                float4 vertex : SV_POSITION;
                float4 worldPos: TEXCOORD1;
            };

            v2f vert(appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.color = v.color;//saturate(float4(ShadeVertexLights(o.vertex, v.Normal), 1) * v.color);;
                o.worldPos = mul(unity_ObjectToWorld, v.vertex);
                return o;
            }

            fixed4 frag(v2f i) : SV_Target
            {
                float d2gp = length(_GazePosition.xyz - i.worldPos.xyz);
                if (length(i.color.xyz) > 0.1f)
                {
                    return i.color;
                }
                else if (d2gp < 0.2f && d2gp > 0.1f)
                {
                    return fixed4(0.5f*(_CosTime.xyz + fixed3(1,1,1)), 1);
                }

                return fixed4(0,0,0,1);
            }
            ENDCG
        }
    }
}