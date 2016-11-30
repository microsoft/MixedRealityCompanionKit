// Very fast lambertian/blinn-phong lit shader that uses the Unity lights.
// Compiles down to only performing the operations you're actually using.
// Uses material property drawers rather than a custom editor for ease of maintenance.

Shader "HoloToolkit/Fast Configurable"
{
    Properties
    {
        [Toggle] _UseColor("Main Color Enabled?", Float) = 0
        _MainColor("Main Color", Color) = (1,1,1,1)
	
		[Space(20)]

        [Toggle] _UseMainTex("Main Texture Enabled?", Float) = 0
        _MainTex("Main Texture", 2D) = "red" {}
        
		[Space(20)]
		
        // Uses UV scale, etc from main texture		
        [Toggle] _UseBumpMap("Normal Map Enabled? (also enables per-pixel lighting)", Float) = 0
        [NoScaleOffset][Normal] _BumpMap("Normalmap", 2D) = "bump" {}
        
		[Space(20)]

		[Toggle] _UseDiffuse("Diffuse Lighting Enabled?", Float) = 1

		[Space(20)]

        [Toggle] _UseSpecular("Specular Lighting Enabled?", Float) = 0
        _SpecularColor("Specular Color", Color) = (0.5, 0.5, 0.5, 1)
        [PowerSlider(2.0)]_SpecularAmount("Specular", Range(0.0, 1.0)) = 0.05
        [PowerSlider(3.0)]_GlossAmount("Gloss", Range(0.0, 10.0)) = 0.1

		[Space(20)]				

		[Toggle] _UseEmissionColor("Emission Color Enabled?", Float) = 0
		_EmissionColor("Emission Color", Color) = (1,1,1,1)

		[Space(20)]

        // Uses UV scale, etc from main texture
        [Toggle] _UseEmissionTex("Emission Texture Enabled?", Float) = 0
        [NoScaleOffset] _EmissionTex("Emission Texture", 2D) = "blue" {}

		[Space(20)]

        [Enum(UnityEngine.Rendering.BlendMode)] _SrcBlend("SrcBlend", Float) = 1 //"One"
        [Enum(UnityEngine.Rendering.BlendMode)] _DstBlend("DestBlend", Float) = 0 //"Zero"

		[Space(20)]

        [Enum(UnityEngine.Rendering.CullMode)] _Cull("Cull", Float) = 2 //"Back"
        [Enum(UnityEngine.Rendering.CompareFunction)] _ZTest("ZTest", Float) = 4 //"LessEqual"
        [Enum(Off,0,On,1)] _ZWrite("ZWrite", Float) = 1.0 //"On"
        [Enum(UnityEngine.Rendering.ColorWriteMask)] _ColorWriteMask("ColorWriteMask", Float) = 15 //"All"
    }

    SubShader
    {
        Tags { "RenderType"="Opaque" "PerformanceChecks"="False" }
        LOD 100
        Blend[_SrcBlend][_DstBlend]
        ZTest[_ZTest]
        ZWrite[_ZWrite]
        Cull[_Cull]
        ColorMask[_ColorWriteMask]

        Pass
        {
            Name "FORWARD"
            Tags{ "LightMode" = "ForwardBase" }

            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #pragma multi_compile_fwdbase
            #pragma multi_compile_fog

            // We only target the HoloLens (and the Unity editor), so take advantage of shader model 5.
            #pragma target 5.0
            #pragma only_renderers d3d11

            #pragma shader_feature _USEMAINCOLOR_ON
            #pragma shader_feature _USEMAINTEX_ON
            #pragma shader_feature _USEBUMPMAP_ON
			#pragma shader_feature _USEDIFFUSE_ON
            #pragma shader_feature _USESPECULAR_ON
			#pragma shader_feature _USEEMISSIONCOLOR_ON
            #pragma shader_feature _USEEMISSIONTEX_ON
            #pragma shader_feature _NEAR_PLANE_FADE_ON

            #include "HoloToolkitCommon.cginc"
			#include "HLSLSupport.cginc"
			#include "UnityCG.cginc"
			#include "Lighting.cginc"
			#include "AutoLight.cginc"

			#if _USEMAINCOLOR_ON
				float4 _MainColor;
			#endif

			#if _USEMAINTEX_ON
				UNITY_DECLARE_TEX2D(_MainTex);
			#endif
		
			#if _USEBUMPMAP_ON
				UNITY_DECLARE_TEX2D(_BumpMap);
			#endif
			
			#if _USESPECULAR_ON
				float3 _SpecularColor;
				float _SpecularAmount;
				float _GlossAmount;
			#endif

			#if _USEEMISSIONCOLOR_ON
				float4 _EmissionColor;
			#endif

			#if _USEEMISSIONTEX_ON
				UNITY_DECLARE_TEX2D(_EmissionTex);
			#endif

			#if _USEMAINTEX_ON || _USEEMISSIONTEX_ON || _USEBUMPMAP_ON
				float4 _MainTex_ST;
			#endif

			struct appdata_t
			{
				float4 vertex : POSITION;
				#if _USEMAINTEX_ON || _USEEMISSIONTEX_ON || _USEBUMPMAP_ON
					float2 texcoord : TEXCOORD0;
				#endif
				float3 normal : NORMAL;
			};

			struct v2f
			{
				float4 pos : SV_POSITION;
				#if _USEMAINTEX_ON || _USEEMISSIONTEX_ON || _USEBUMPMAP_ON
					float2 tex : TEXCOORD0;
				#endif
				#ifndef LIGHTMAP_OFF
					float2 lmap : TEXCOORD1;
				#else
					#ifndef _USEBUMPMAP_ON
						float3 vlight : TEXCOORD1;
					#endif
				#endif
				#if _USEBUMPMAP_ON
					float4 worldPos: TEXCOORD2;
				#endif
				#if _NEAR_PLANE_FADE_ON
					float fade : TEXCOORD3;
				#endif
				LIGHTING_COORDS(4, 5)
				UNITY_FOG_COORDS(6)
				UNITY_VERTEX_OUTPUT_STEREO
			};

			v2f vert(appdata_full IN)
			{
				v2f o;
				UNITY_INITIALIZE_OUTPUT(v2f, o);
				UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(o);

				o.pos = UnityObjectToClipPos(IN.vertex);

				#if _USEBUMPMAP_ON
					o.worldPos = mul(unity_ObjectToWorld, IN.vertex);
				#endif

				#if _USEMAINTEX_ON || _USEEMISSIONTEX_ON || _USEBUMPMAP_ON
					o.tex.xy = TRANSFORM_TEX(IN.texcoord, _MainTex);
				#endif

				#ifndef LIGHTMAP_OFF
					o.lmap.xy = IN.texcoord1.xy * unity_LightmapST.xy + unity_LightmapST.zw;
				#else
					#ifndef _USEBUMPMAP_ON
						float3 worldN = UnityObjectToWorldNormal(IN.normal);
						#if _USEDIFFUSE_ON
							o.vlight =  ShadeSHPerVertex(worldN, o.vlight.rgb);
							o.vlight += HoloTKLightingLambertian(worldN, _WorldSpaceLightPos0.xyz, _LightColor0);
						#endif
						#if _USESPECULAR_ON
							o.vlight += HoloTKLightingBlinnPhong(worldN, _WorldSpaceLightPos0.xyz, _LightColor0, UnityWorldSpaceViewDir(mul(unity_ObjectToWorld, IN.vertex)), _SpecularAmount, _GlossAmount, _SpecularColor);
						#endif						
					#endif
				#endif
				
				#if _NEAR_PLANE_FADE_ON
					o.fade = ComputeNearPlaneFadeLinear(IN.vertex);
				#endif

				TRANSFER_VERTEX_TO_FRAGMENT(o);
				UNITY_TRANSFER_FOG(o, o.pos);
				return o;
			}

			float4 frag(v2f IN) : SV_Target
			{
				#if _USEMAINTEX_ON
				    float4 color = UNITY_SAMPLE_TEX2D(_MainTex, IN.tex);
				#else
				    float4 color = 1;
				#endif

				#if _USEMAINCOLOR_ON
					color *= _MainColor;
				#endif

				float atten = LIGHT_ATTENUATION(IN);

				#ifdef LIGHTMAP_OFF
					float3 colorLight = float3(0,0,0);
					#ifndef _USEBUMPMAP_ON
						colorLight = IN.vlight;
					#else
						float3 normalObject = UnpackNormal(UNITY_SAMPLE_TEX2D(_BumpMap, IN.tex));
						float3 normalWorld = UnityObjectToWorldNormal(normalObject);

						#if _USEDIFFUSE_ON
							colorLight += ShadeSH9(float4(normalWorld, 1.0));
							colorLight += HoloTKLightingLambertian(normalWorld, _WorldSpaceLightPos0.xyz, _LightColor0);
						#endif
						#if _USESPECULAR_ON
							colorLight += HoloTKLightingBlinnPhong(normalWorld, _WorldSpaceLightPos0.xyz, _LightColor0, UnityWorldSpaceViewDir(IN.worldPos), _SpecularAmount, _GlossAmount, _SpecularColor);
						#endif
					#endif					
					color.rgb *= (colorLight * atten);
				#endif

				#ifdef LIGHTMAP_ON
					float3 lm = DecodeLightmap(UNITY_SAMPLE_TEX2D(unity_Lightmap, IN.lmap.xy));
					#ifdef SHADOWS_SCREEN
						color.rgb *= min(lm, atten * 2);
					#else
						color.rgb *= lm;
					#endif	
				#endif

				#ifdef _USEEMISSIONTEX_ON
					color.rgb += UNITY_SAMPLE_TEX2D(_EmissionTex, IN.tex);
				#endif

				#ifdef _USEEMISSIONCOLOR_ON
					color.rgb += _EmissionColor;
				#endif

				#if _NEAR_PLANE_FADE_ON
					color.rgb *= IN.fade;
				#endif

				UNITY_APPLY_FOG(IN.fogCoord, color);

				return color;
			}

            ENDCG
        }
    } 
	Fallback "VertexLit" //for shadows
}