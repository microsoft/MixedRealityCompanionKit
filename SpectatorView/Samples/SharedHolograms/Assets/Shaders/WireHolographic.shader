Shader "Holographic/PlacingWireFrame"
{
    Properties
    {
        _WireColor("WireColor", Color) = (1,0,0,1)
        _Color("Color", Color) = (1,1,1,1)
    }
    SubShader
    {
        Tags{ "Queue" = "Transparent"  "IgnoreProjector" = "True" "RenderType" = "Transparent" }
        Blend SrcAlpha OneMinusSrcAlpha

        Pass
        {
            CGPROGRAM
            #include "UnityCG.cginc"
            #pragma target 5.0
            #pragma vertex vert
            #pragma geometry geom
            #pragma fragment frag

            half4 _WireColor, _Color;

            struct appdata {
                float4 vertex : POSITION;
                float4 tangent : TANGENT;
                float3 normal : NORMAL;
                float2 texcoord : TEXCOORD0;
                float2 texcoord1 : TEXCOORD1;
            };

            struct v2g
            {
                float4 pos : SV_POSITION;
                float2 uv : TEXCOORD0;
            };

            struct g2f
            {
                float4 pos : SV_POSITION;
                float2 uv : TEXCOORD0;
                float3 dist : TEXCOORD1;
            };

            v2g vert(appdata v)
            {
                v2g OUT;
                OUT.pos = mul(UNITY_MATRIX_MVP, v.vertex);
                OUT.uv = v.texcoord; // The UVs aren't used in this shader but are included in case you want to use them.
                return OUT;
            }

            [maxvertexcount(3)]
            void geom(triangle v2g IN[3], inout TriangleStream<g2f> triStream)
            {
                float2 WIN_SCALE = float2(_ScreenParams.x / 1.5, _ScreenParams.y / 1.5);
                // Frag position
                float2 p0 = WIN_SCALE * IN[0].pos.xy / IN[0].pos.w;
                float2 p1 = WIN_SCALE * IN[1].pos.xy / IN[1].pos.w;
                float2 p2 = WIN_SCALE * IN[2].pos.xy / IN[2].pos.w;

                // Barycentric position
                float2 v0 = p2 - p1;
                float2 v1 = p2 - p0;
                float2 v2 = p1 - p0;

                // Triangles area
                float area = abs(v1.x*v2.y - v1.y * v2.x);

                g2f OUT;
                OUT.pos = IN[0].pos;
                OUT.uv = IN[0].uv;
                OUT.dist = float3(area / length(v0),0,0);
                triStream.Append(OUT);

                OUT.pos = IN[1].pos;
                OUT.uv = IN[1].uv;
                OUT.dist = float3(0,area / length(v1),0);
                triStream.Append(OUT);

                OUT.pos = IN[2].pos;
                OUT.uv = IN[2].uv;
                OUT.dist = float3(0,0,area / length(v2));
                triStream.Append(OUT);
            }

            half4 frag(g2f IN) : COLOR
            {
                // Distance of frag from triangles center
                float d = min(IN.dist.x, min(IN.dist.y, IN.dist.z));
                // Fade based on dist from center
                float I = exp2(-1 * d*d);

                return lerp(_Color, _WireColor, I);
            }
            ENDCG
        }
    }
    FallBack "Transparent/VertexLit"
}