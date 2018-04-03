// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

Shader "Hidden/YUV2RGB"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _Width("Width", int) = 0
        _YUV("YUV", int) = 1 
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
            // Target shader model 5 to use bit shifting.
            #pragma target 5.0

            #include "UnityCG.cginc"
            #include "YUVHelper.cginc"

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
            
            Texture2D _MainTex;
            int _Width;
            int _YUV;

            fixed4 fragYUV(v2f i)
            {
                uint2 xy = i.vertex.xy;
                uint p = xy.x + (xy.y * _Width);
                uint pixloc = p / 2;

                float2 uv;
                uv.x = pixloc % _Width;
                uv.y = pixloc / _Width;

                // Use Texture2D.Load so we do not contaminate any of the selected pixels from filtering.
                float4 yuvPixel = _MainTex.Load(int3(uv, 0));
                return GetRGBA(yuvPixel, p % 2);
            }

            fixed4 frag(v2f i) : SV_Target
            {    
                fixed4 col;
                if (_YUV == 1)
                {
                    col = fragYUV(i);
                }
                else
                {
                    // Input texture is already rgba.
                    col = _MainTex.Load(int3(i.vertex.xy, 0));
                }

                // Convert to Unity color space.
                col.rgba = col.bgra;

                // Background should always be fully opaque.
                col.a = 1;
                return col;
            }
            ENDCG
        }
    }
}
