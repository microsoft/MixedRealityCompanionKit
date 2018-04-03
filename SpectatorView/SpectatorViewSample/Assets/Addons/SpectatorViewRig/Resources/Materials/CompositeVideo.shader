// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

Shader "Hidden/CompositeVideo"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _Width("Width", int) = 0
        _Height("Height", int) = 0
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
            int _Height;

            fixed4 fragNV12(v2f i)
            {
                // An NV12 frame is composed of 
                // 1. A Width * Height block of data with every y value
                // 2. Alternating u and v data from every other row in the original yuv image.

                uint2 xy = i.vertex.xy;

                uint p = xy.x + (xy.y * _Width);
                uint pixloc = p * 4;
                uint pixloc2 = pixloc + 1;
                uint pixloc3 = pixloc + 2;
                uint pixloc4 = pixloc + 3;

                bool renderY = true;

                if (p >= (uint)(_Width * _Height * 1.5f))
                {
                    return fixed4(0, 0, 0, 0);
                }

                // Find UV color values in original width * height range.
                if (pixloc >= (uint)(_Width * _Height))
                {
                    renderY = false;
                    int offset = _Width * _Height;

                    // Find every other row's UV color values for NV12 frame.
                    uint row = (pixloc - offset) / _Width;
                    offset -= row * _Width;

                    pixloc -= offset;
                    pixloc2 -= offset;
                    pixloc3 -= offset;
                    pixloc4 -= offset;
                }

                float2 uv, uv2, uv3, uv4;
                uv.x = pixloc % _Width;
                uv.y = _Height - (pixloc / _Width);
                uv2.x = pixloc2 % _Width;
                uv2.y = _Height - (pixloc2 / _Width);
                uv3.x = pixloc3 % _Width;
                uv3.y = _Height - (pixloc3 / _Width);
                uv4.x = pixloc4 % _Width;
                uv4.y = _Height - (pixloc4 / _Width);

                // Use Texture2D.Load so we do not contaminate any of the selected pixels from filtering.
                fixed4 col1 = _MainTex.Load(int3(uv, 0));
                fixed4 col2 = _MainTex.Load(int3(uv2, 0));
                fixed4 col3 = _MainTex.Load(int3(uv3, 0));
                fixed4 col4 = _MainTex.Load(int3(uv4, 0));

                // yuv is packed uyvy
                fixed4 yuv = GetYUV(col1, col2);
                fixed4 yuv2 = GetYUV(col3, col4);

                if (renderY)
                {
                    return fixed4(yuv.ga, yuv2.ga);
                }

                // Render UV
                return fixed4(yuv.rb, yuv2.rb);
            }

            fixed4 frag (v2f i) : SV_Target
            {
                return fragNV12(i);
            }
            ENDCG
        }
    }
}
