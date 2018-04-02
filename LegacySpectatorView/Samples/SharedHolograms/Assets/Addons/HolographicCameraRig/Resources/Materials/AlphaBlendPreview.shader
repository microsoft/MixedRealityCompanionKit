// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

Shader "Hidden/AlphaBlendPreview"
{
    Properties
    {
        _MainTex("Texture", 2D) = "white" {}
        _FrontTex("Texture", 2D) = "white" {}
        _Alpha("Alpha", float) = 0.9
        _YUV("YUV", int) = 0
        _Width("Width", int) = 0
        _Height("Height", int) = 0
        _SwapBackgroundRB("SwapBackgroundRB", int) = 0
        _FlipVideo("FlipVideo", int) = 0
        // Flip R and B
        _RGBA("RGBA", int) = 0
        _Brightness("Brightness", float) = 1.0
    }
    SubShader
    {
        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            // Target shader model 5 to use bit shifting.
            #pragma target 5.0

            #include "UnityCG.cginc"
            #include "AlphaBlendHelper.cginc"
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

            v2f vert(appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = v.uv;
                return o;
            }

            sampler2D _MainTex;
            sampler2D _FrontTex;
            float _Alpha;
            int _YUV;
            int _Width;
            int _Height;
            int _SwapBackgroundRB;
            int _FlipVideo;
            int _RGBA;
            float _Brightness;

            fixed4 fragYUV(v2f i)
            {
                fixed4 frontCol = tex2D(_FrontTex, i.uv);

                if (_FlipVideo == 1)
                {
                    i.uv.y = 1 - i.uv.y;
                }

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
            fixed4 backCol = GetRGBA(yuvPixel, val);
            backCol.rgba *= _Brightness;

                if (_SwapBackgroundRB == 0)
                {
                    backCol.rgba = backCol.bgra;
                }

                fixed4 composite = AlphaBlend(backCol, frontCol, _Alpha);

                if (_RGBA == 1)
                {
                    composite.rgba = composite.bgra;
                }

                return composite;
            }

            fixed4 fragRGBA(v2f i)
            {
                fixed4 frontCol = tex2D(_FrontTex, i.uv);

                if (_FlipVideo == 1)
                {
                    i.uv.y = 1 - i.uv.y;
                }

                fixed4 backCol = tex2D(_MainTex, i.uv);
                backCol.rgba *= _Brightness;

                if (_SwapBackgroundRB == 0)
                {
                    backCol.rgba = backCol.bgra;
                }

                fixed4 composite = AlphaBlend(backCol, frontCol, _Alpha);

                if (_RGBA == 1)
                {
                    composite.rgba = composite.bgra;
                }

                return composite;
            }

            fixed4 frag(v2f i) : SV_Target
            {
                if (_YUV == 0)
                {
                    return fragRGBA(i);
                }

                return fragYUV(i);
            }
            ENDCG
        }
    }
}
