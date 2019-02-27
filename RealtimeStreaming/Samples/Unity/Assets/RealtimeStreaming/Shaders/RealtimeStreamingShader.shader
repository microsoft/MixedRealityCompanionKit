Shader "Unlit/RealtimeStreamingShader"
{
    Properties
    {
        _MainTex_Luma ("Texture", 2D) = "white" {}
        _MainTex_Chroma("Texture", 2D) = "white" {}
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            // make fog work
            #pragma multi_compile_fog

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                UNITY_FOG_COORDS(1)
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex_Luma;
            sampler2D _MainTex_Chroma;
            float4 _MainTex_ST;

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                UNITY_TRANSFER_FOG(o,o.vertex);
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // sample the texture YUV
                float4 tex2 = tex2D(_MainTex_Luma, i.uv);
                float4 tex1 = tex2D(_MainTex_Chroma, i.uv);

                float y = tex1.r;
                float u = tex2.r - 0.5;
                float v = tex2.g - 0.5;

                // The numbers are just YUV to RGB conversion constants
                float r = y + 1.13983*v;
                float g = y - 0.39465*u - 0.58060*v;
                float b = y + 2.03211*u;

                float4 col = float4(r, g, b, 1.0);

                // apply fog
                UNITY_APPLY_FOG(i.fogCoord, col);
                return col;
            }
            ENDCG
        }
    }
}
