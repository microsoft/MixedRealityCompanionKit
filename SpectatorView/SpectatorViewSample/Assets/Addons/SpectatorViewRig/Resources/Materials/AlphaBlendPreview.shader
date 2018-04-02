// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

Shader "Hidden/AlphaBlendPreview"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
		_HoloTex ("Texture", 2D) = "white" {}
		_Alpha("Alpha", float) = 0.95
	}
	SubShader
	{
		// No culling or depth
		Cull Off ZWrite Off ZTest Always
		Tags{ "PreviewType" = "Plane" }

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"
			#include "AlphaBlendHelper.cginc"

			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
			};

			struct v2f
			{
				float2 uv : TEXCOORD0;
				float4 vertex : SV_POSITION;
			};

			v2f vert (appdata v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.uv = v.uv;
				return o;
			}
			
			sampler2D _MainTex;
			sampler2D _HoloTex;
			float _Alpha;

			fixed4 frag (v2f i) : SV_Target
			{
				fixed2 backUV = i.uv;
				// Image arrives upside down from capture pipeline.
				backUV.y = 1 - backUV.y;

				// Background color image has already been converted to RGBA
				fixed4 backCol = tex2D(_MainTex, backUV);
				fixed4 frontCol = tex2D(_HoloTex, i.uv);

				return AlphaBlend(backCol, frontCol, _Alpha);
			}
			ENDCG
		}
	}
}
