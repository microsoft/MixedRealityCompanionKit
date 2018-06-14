// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

Shader "Hidden/FlipHorizontally"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _YUV("YUV", int) = 0
        _Width("Width", int) = 0
        _Height("Height", int) = 0
        // Flip R and B
        _RGBA("RGBA", int) = 0
        _Brightness("Brightness", float) = 1.0
    }
    SubShader
    {
        Pass
        {
            CGPROGRAM
            #pragma target 5.0
            #pragma vertex vert
            #pragma fragment frag
            
            #include "UnityCG.cginc"
            #include "YUVHelper.cginc"

            struct appdata
            {
                fixed4 vertex : POSITION;
                fixed2 uv : TEXCOORD0;
            };

            struct v2f
            {
                fixed2 uv : TEXCOORD0;
                fixed4 vertex : SV_POSITION;
            };

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = v.uv;
                return o;
            }
            
            sampler2D _MainTex;
            int _YUV;
            int _Width;
            int _Height;
            int _RGBA;
            float _Brightness;

            fixed4 fragYUV(v2f i)
            {
                i.uv.y = 1 - i.uv.y;

                uint index = (int)floor(((int)floor(_Height * i.uv.y) * _Width) + floor(_Width * i.uv.x));
                uint yuvIndex = index / 2;

                fixed2 uv;
                uv.x = ((yuvIndex / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv.y = ((yuvIndex / 4.0f) / (_Width / 4.0f)) / _Height;

                fixed4 yuvPixel = tex2D(_MainTex, uv);
                int val = 0;
                if (index % 2 != 0)
                {
                    val = 1;
                }
                fixed4 color = GetRGBA(yuvPixel, val);
                color *= _Brightness;
                return color;
            }

            fixed4 fragRGBA(v2f i)
            {
                i.uv.y = 1 - i.uv.y;
                fixed4 color = tex2D(_MainTex, i.uv);
                color *= _Brightness;
                return color;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                fixed4 color;
                if (_YUV == 0)
                {
                    color = fragRGBA(i);
                }
                else
                {
                    color = fragYUV(i);
                }

                if (_RGBA == 1)
                {
                    color.rgba = color.bgra;
                }

                color *= _Brightness;

                return color;
            }
            ENDCG
        }
    }
}
