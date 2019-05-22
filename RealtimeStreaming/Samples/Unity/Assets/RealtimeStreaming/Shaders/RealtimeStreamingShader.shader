Shader "Unlit/RealtimeStreamingShader"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "green" {}
        _MainTex_Luma ("Texture", 2D) = "green" {}
        _MainTex_Chroma("Texture", 2D) = "green" {}
        [Toggle(_DISCARD_TEX_WRAP_MODE)] _EnableDiscardWrapMode("Enable Discard Texture Wrap Mode", Float) = 0.0
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
            
            #pragma shader_feature _DISCARD_TEX_WRAP_MODE

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;

                UNITY_VERTEX_INPUT_INSTANCE_ID
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                UNITY_FOG_COORDS(1)
                float4 vertex : SV_POSITION;

                UNITY_VERTEX_OUTPUT_STEREO
            };

            float4 _MainTex_ST;
            sampler2D _MainTex;
            sampler2D _MainTex_Luma;
            sampler2D _MainTex_Chroma;

            v2f vert (appdata v)
            {
                v2f o;

                UNITY_SETUP_INSTANCE_ID(v);
                UNITY_INITIALIZE_OUTPUT(v2f, o);
                UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(o);

                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);

                UNITY_TRANSFER_FOG(o, o.vertex);

                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {

#if defined(_DISCARD_TEX_WRAP_MODE)
                // If uv is out of bounds of [0,1] then clip pixel
                clip(1.0 - i.uv); 
                clip(i.uv);
#endif
                // sample the texture YUV
                float4 lumaTex = tex2D(_MainTex_Luma, i.uv);
                float4 chromaTex = tex2D(_MainTex_Chroma, i.uv);

                float y = chromaTex.r;
                float u = lumaTex.r - 0.5;
                float v = lumaTex.g - 0.5;

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
