// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

Shader "Hidden/AlphaBlendVideo"
{
    Properties
    {
        _MainTex("Texture", 2D) = "white" {}
        _FrontTex("Texture", 2D) = "white" {}
        _Alpha("Alpha", float) = 0.9
        _FlipVideo("FlipVideo", int) = 0
        _RGBA("RGBA", int) = 0
        _Width("Width", int) = 0
        _Height("Height", int) = 0
        _YUV("YUV", int) = 0
        _SwapBackgroundRB("SwapBackgroundRB", int) = 0
        _NV12("NV12", int) = 0
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
            int _FlipVideo;
            int _RGBA;
            int _Width;
            int _Height;
            int _YUV;
            int _NV12;
            int _SwapBackgroundRB;
            float _Brightness;

            fixed4 fragRGBA(v2f i)
            {
                fixed4 col;
                fixed4 backCol;
                fixed4 frontCol;

                fixed2 backColUV;

                if (_FlipVideo == 0)
                {
                    backColUV = fixed2(i.uv.x, 1 - i.uv.y);
                    frontCol = tex2D(_FrontTex, i.uv);
                }
                else
                {
                    backColUV = i.uv;
                    frontCol = tex2D(_FrontTex, fixed2(i.uv.x, 1 - i.uv.y));
                }

                backCol = tex2D(_MainTex, backColUV);
                backCol.rgba *= _Brightness;

                if (_SwapBackgroundRB == 0)
                {
                    backCol.rgba = backCol.bgra;
                }
                col.rgba = AlphaBlend(backCol, frontCol, _Alpha);

                if (_RGBA == 1)
                {
                    col.rgba = col.bgra;
                }
                return col;
            }

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

                fixed4 yuv = tex2D(_MainTex, uv);
                int val = 0;
                if (index % 2 != 0)
                {
                    val = 1;
                }
                fixed4 backCol = GetRGBA(yuv, val);
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

            fixed4 fragYUV_NV12(v2f i)
            {
                float w = _Width;
                uint index = (int)floor(((int)floor(_Height * i.uv.y) * w) + floor(w * i.uv.x));
                if (index > (1.5f * _Width * _Height) / 4.0f)
                {
                    return fixed4(0, 0, 0, 1);
                }

                int yuvIndex = index * 2;
                int frontYuvIndex = index * 4;
                
                fixed2 yuvUV1, yuvUV2;
                yuvUV1.x = ((yuvIndex / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                yuvUV1.y = ((yuvIndex / 4.0f) / (_Width / 4.0f)) / _Height;
                yuvUV2.x = (((yuvIndex + 1) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                yuvUV2.y = (((yuvIndex + 1) / 4.0f) / (_Width / 4.0f)) / _Height;

                fixed4 yuvPixel = tex2D(_MainTex, yuvUV1);
                fixed4 yuvPixel2 = tex2D(_MainTex, yuvUV2);

                fixed4 backCol = GetRGBA(yuvPixel, 0);
                fixed4 backCol2 = GetRGBA(yuvPixel, 1);
                fixed4 backCol3 = GetRGBA(yuvPixel2, 0);
                fixed4 backCol4 = GetRGBA(yuvPixel2, 1);
                backCol.rgba *= _Brightness;
                backCol2.rgba *= _Brightness;
                backCol3.rgba *= _Brightness;
                backCol4.rgba *= _Brightness;

                // Alpha Blend
                fixed2 uv1, uv2, uv3, uv4;
                uv1.x = ((frontYuvIndex / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv1.y = 1 - ((frontYuvIndex / 4.0f) / (_Width / 4.0f)) / _Height;
                uv2.x = (((frontYuvIndex + 1) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv2.y = 1 - (((frontYuvIndex + 1) / 4.0f) / (_Width / 4.0f)) / _Height;
                uv3.x = (((frontYuvIndex + 2) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv3.y = 1 - (((frontYuvIndex + 2) / 4.0f) / (_Width / 4.0f)) / _Height;
                uv4.x = (((frontYuvIndex + 3) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv4.y = 1 - (((frontYuvIndex + 3) / 4.0f) / (_Width / 4.0f)) / _Height;

                fixed4 frontCol = tex2D(_FrontTex, uv1);
                fixed4 frontCol2 = tex2D(_FrontTex, uv2);
                fixed4 frontCol3 = tex2D(_FrontTex, uv3);
                fixed4 frontCol4 = tex2D(_FrontTex, uv4);

                fixed4 composite1 = AlphaBlend(backCol, frontCol, _Alpha);
                fixed4 composite2 = AlphaBlend(backCol2, frontCol2, _Alpha);
                fixed4 composite3 = AlphaBlend(backCol3, frontCol3, _Alpha);
                fixed4 composite4 = AlphaBlend(backCol4, frontCol4, _Alpha);

                fixed4 yuv1 = GetYUV(composite1, composite2);
                fixed4 yuv2 = GetYUV(composite3, composite4);

                if (yuvIndex < (_Width * _Height) / 2.0f)
                {
                    // Y values
                    return fixed4(yuv1.g, yuv1.a, yuv2.g, yuv2.a);
                }

                // UV values
                yuvIndex -= (_Width * _Height) / 2.0f;
                int row = (int)(yuvIndex / _Width);
                yuvIndex += row * _Width;

                frontYuvIndex -= (_Width * _Height);
                int frontRow = (int)(yuvIndex / _Width);
                frontYuvIndex += frontRow * _Width;

                yuvUV1.x = ((yuvIndex / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                yuvUV1.y = ((yuvIndex / 4.0f) / (_Width / 4.0f)) / _Height;
                yuvUV2.x = (((yuvIndex + 1) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                yuvUV2.y = (((yuvIndex + 1) / 4.0f) / (_Width / 4.0f)) / _Height;

                yuvPixel = tex2D(_MainTex, yuvUV1);
                yuvPixel2 = tex2D(_MainTex, yuvUV2);

                backCol = GetRGBA(yuvPixel, 0);
                backCol2 = GetRGBA(yuvPixel, 1);
                backCol3 = GetRGBA(yuvPixel2, 0);
                backCol4 = GetRGBA(yuvPixel2, 1);
                backCol.rgba *= _Brightness;
                backCol2.rgba *= _Brightness;
                backCol3.rgba *= _Brightness;
                backCol4.rgba *= _Brightness;

                if (_SwapBackgroundRB == 1)
                {
                    backCol.rgba = backCol.bgra;
                    backCol2.rgba = backCol2.bgra;
                    backCol3.rgba = backCol3.bgra;
                    backCol4.rgba = backCol4.bgra;
                }

                // Alpha blend
                uv1.x = ((frontYuvIndex / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv1.y = 1 - ((frontYuvIndex / 4.0f) / (_Width / 4.0f)) / _Height;
                uv2.x = (((frontYuvIndex + 1) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv2.y = 1 - ((frontYuvIndex + 1 / 4.0f) / (_Width / 4.0f)) / _Height;
                uv3.x = (((frontYuvIndex + 2) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv3.y = 1 - (((frontYuvIndex + 2) / 4.0f) / (_Width / 4.0f)) / _Height;
                uv4.x = (((frontYuvIndex + 3) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv4.y = 1 - (((frontYuvIndex + 3) / 4.0f) / (_Width / 4.0f)) / _Height;

                frontCol = tex2D(_FrontTex, uv1);
                frontCol2 = tex2D(_FrontTex, uv2);
                frontCol3 = tex2D(_FrontTex, uv3);
                frontCol4 = tex2D(_FrontTex, uv4);

                composite1 = AlphaBlend(backCol, frontCol, _Alpha);
                composite2 = AlphaBlend(backCol2, frontCol2, _Alpha);
                composite3 = AlphaBlend(backCol3, frontCol3, _Alpha);
                composite4 = AlphaBlend(backCol4, frontCol4, _Alpha);

                yuv1 = GetYUV(composite1, composite2);
                yuv2 = GetYUV(composite3, composite4);

                return fixed4(yuv1.r, yuv1.b, yuv2.r, yuv2.b);
            }

            fixed4 fragRGBA_NV12(v2f i)
            {
                float w = _Width;
                uint index = (int)floor(((int)floor(_Height * i.uv.y) * w) + floor(w * i.uv.x));
                if (index > (1.5f * _Width * _Height) / 4.0f)
                {
                    return fixed4(0, 0, 0, 1);
                }

                int rgbaIndex = index * 4;

                // Alpha Blend
                fixed2 uv1, uv2, uv3, uv4;
                uv1.x = ((rgbaIndex / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv1.y = ((rgbaIndex / 4.0f) / (_Width / 4.0f)) / _Height;
                uv2.x = (((rgbaIndex + 1) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv2.y = (((rgbaIndex + 1) / 4.0f) / (_Width / 4.0f)) / _Height;
                uv3.x = (((rgbaIndex + 2) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv3.y = (((rgbaIndex + 2) / 4.0f) / (_Width / 4.0f)) / _Height;
                uv4.x = (((rgbaIndex + 3) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv4.y = (((rgbaIndex + 3) / 4.0f) / (_Width / 4.0f)) / _Height;

                fixed4 backCol = tex2D(_MainTex, uv1);
                fixed4 backCol2 = tex2D(_MainTex, uv2);
                fixed4 backCol3 = tex2D(_MainTex, uv3);
                fixed4 backCol4 = tex2D(_MainTex, uv4);
                backCol.rgba *= _Brightness;
                backCol2.rgba *= _Brightness;
                backCol3.rgba *= _Brightness;
                backCol4.rgba *= _Brightness;

                fixed4 frontCol = tex2D(_FrontTex, fixed2(uv1.x, 1 - uv1.y));
                fixed4 frontCol2 = tex2D(_FrontTex, fixed2(uv2.x, 1 - uv2.y));
                fixed4 frontCol3 = tex2D(_FrontTex, fixed2(uv3.x, 1 - uv3.y));
                fixed4 frontCol4 = tex2D(_FrontTex, fixed2(uv4.x, 1 - uv4.y));

                fixed4 composite1 = AlphaBlend(backCol, frontCol, _Alpha);
                fixed4 composite2 = AlphaBlend(backCol2, frontCol2, _Alpha);
                fixed4 composite3 = AlphaBlend(backCol3, frontCol3, _Alpha);
                fixed4 composite4 = AlphaBlend(backCol4, frontCol4, _Alpha);

                fixed4 yuv1 = GetYUV(composite1, composite2);
                fixed4 yuv2 = GetYUV(composite3, composite4);

                if (rgbaIndex < (_Width * _Height))
                {
                    // Y values
                    return fixed4(yuv1.g, yuv1.a, yuv2.g, yuv2.a);
                }
                
                // UV values
                rgbaIndex -= (_Width * _Height);
                int row = (int)(rgbaIndex / _Width);
                rgbaIndex += row * _Width;

                // Alpha blend
                uv1.x = ((rgbaIndex / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv1.y = ((rgbaIndex / 4.0f) / (_Width / 4.0f)) / _Height;
                uv2.x = (((rgbaIndex + 1) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv2.y = ((rgbaIndex + 1 / 4.0f) / (_Width / 4.0f)) / _Height;
                uv3.x = (((rgbaIndex + 2) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv3.y = (((rgbaIndex + 2) / 4.0f) / (_Width / 4.0f)) / _Height;
                uv4.x = (((rgbaIndex + 3) / 4.0f) % (_Width / 4.0f)) / (_Width / 4.0f);
                uv4.y = (((rgbaIndex + 3) / 4.0f) / (_Width / 4.0f)) / _Height;

                backCol = tex2D(_MainTex, uv1);
                backCol2 = tex2D(_MainTex, uv2);
                backCol3 = tex2D(_MainTex, uv3);
                backCol4 = tex2D(_MainTex, uv4);
                backCol.rgba *= _Brightness;
                backCol2.rgba *= _Brightness;
                backCol3.rgba *= _Brightness;
                backCol4.rgba *= _Brightness;

                if (_SwapBackgroundRB == 1)
                {
                    backCol.rgba = backCol.bgra;
                    backCol2.rgba = backCol2.bgra;
                    backCol3.rgba = backCol3.bgra;
                    backCol4.rgba = backCol4.bgra;
                }

                frontCol = tex2D(_FrontTex, fixed2(uv1.x, 1 - uv1.y));
                frontCol2 = tex2D(_FrontTex, fixed2(uv2.x, 1 - uv2.y));
                frontCol3 = tex2D(_FrontTex, fixed2(uv3.x, 1 - uv3.y));
                frontCol4 = tex2D(_FrontTex, fixed2(uv4.x, 1 - uv4.y));

                composite1 = AlphaBlend(backCol, frontCol, _Alpha);
                composite2 = AlphaBlend(backCol2, frontCol2, _Alpha);
                composite3 = AlphaBlend(backCol3, frontCol3, _Alpha);
                composite4 = AlphaBlend(backCol4, frontCol4, _Alpha);

                yuv1 = GetYUV(composite1, composite2);
                yuv2 = GetYUV(composite3, composite4);

                return fixed4(yuv1.r, yuv1.b, yuv2.r, yuv2.b);
            }

            fixed4 frag(v2f i) : SV_Target
            {
                if (_YUV == 0 && _NV12 == 0)
                {
                    return fragRGBA(i);
                }
                else if (_NV12 == 1)
                {
                    if (_YUV == 1)
                    {
                        return fragYUV_NV12(i);
                    }
                    
                    return fragRGBA_NV12(i);
                }

                return fragYUV(i);
            }
            ENDCG
        }
    }
}
