// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

Shader "Holographic/WireFrame"
{
    Properties
    {
        _DispTex("Disp Texture", 2D) = "gray" {}
        _Displacement("Displacement", Range(0, 1.0)) = 0.3
        _Offset("Offset", Vector) = (0,0,0,0)
        _WireColor("WireColor", Color) = (1,0,0,1)
        _Color("Color", Color) = (1,1,1,1)
		[HDR]_RimColor("Rim Color", Color) = (1,1,1,1)
		_RimPow("Rim Power", Float) = .7
    }
    SubShader
    {
        Tags{"RenderType" = "Opaque"  "PerformanceChecks" = "False" }
        //Blend SrcAlpha OneMinusSrcAlpha
        //Cull Off

        Pass
        {
            ZWrite Off
            CGPROGRAM
            #include "UnityCG.cginc"
			#include "HoloToolkitCommon.cginc"
			#pragma target 5.0
			#pragma only_renderers d3d11
            #pragma vertex vert
            #pragma geometry geom
            #pragma fragment frag

            half4 _WireColor, _Color;
			float4 _RimColor;
			float _RimPow;

            struct appdata {
                float4 vertex : POSITION;
                float4 tangent : TANGENT;
                float3 normal : NORMAL;
                float2 texcoord : TEXCOORD0;
                float2 texcoord1 : TEXCOORD1;
				float4 color : COLOR;
				UNITY_VERTEX_INPUT_INSTANCE_ID
            };

            struct v2g
            {
                float4 pos : SV_POSITION;
                float2 uv : TEXCOORD0;
				float4 color : COLOR;
				UNITY_VERTEX_OUTPUT_STEREO
            };

            struct g2f
            {
                float4 pos : SV_POSITION;
                float2 uv : TEXCOORD0;
                float3 dist : TEXCOORD1;
				float4 color : COLOR;
				UNITY_VERTEX_OUTPUT_STEREO
            };

            sampler2D _DispTex;
            float _Displacement;
            float4 _Offset;

            v2g vert(appdata v)
            {
				UNITY_SETUP_INSTANCE_ID(v);
				v2g OUT;	

                float d = tex2Dlod(_DispTex, float4(v.texcoord1.x + _Offset.x, v.texcoord1.y + _Offset.y, 0, 0)).r * _Displacement;
                v.vertex.xyz += v.normal * d;
                OUT.pos = UnityObjectToClipPos(v.vertex);
                OUT.uv = v.texcoord; // The UVs aren't used in this shader but are included in case you want to use them.
				float3 viewDir = normalize(ObjSpaceViewDir(v.vertex));
				float dotProduct = 1 - dot(v.normal, viewDir);
				OUT.color = smoothstep(1 - _RimPow, 1.0, dotProduct);
				OUT.color *= _RimColor;
				UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(OUT);
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
				OUT.color = IN[0].color;
                OUT.pos = IN[0].pos;
                OUT.uv = IN[0].uv;
                OUT.dist = float3(area / length(v0),0,0);
				UNITY_TRANSFER_VERTEX_OUTPUT_STEREO(IN[0], OUT);
                triStream.Append(OUT);

				OUT.color = IN[1].color;
                OUT.pos = IN[1].pos;
                OUT.uv = IN[1].uv;
                OUT.dist = float3(0,area / length(v1),0);
				UNITY_TRANSFER_VERTEX_OUTPUT_STEREO(IN[1], OUT);
                triStream.Append(OUT);

				OUT.color = IN[2].color;
                OUT.pos = IN[2].pos;
                OUT.uv = IN[2].uv;
                OUT.dist = float3(0,0,area / length(v2));
				UNITY_TRANSFER_VERTEX_OUTPUT_STEREO(IN[2], OUT);
                triStream.Append(OUT);
            }

            half4 frag(g2f IN) : COLOR
            {
                // Distance of frag from triangles center
                float d = min(IN.dist.x, min(IN.dist.y, IN.dist.z));
                // Fade based on dist from center
                float I = exp2(-1 * d*d);

				half4 output = _Color;
				half4 emission = IN.color;
				output.rgb += emission.rgb;

				return lerp(output, _WireColor, I);
            }

            ENDCG
        }
    }
    FallBack "Unlit"
}