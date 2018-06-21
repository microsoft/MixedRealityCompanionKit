using System;
using System.Collections.Generic;
using System.Text;

namespace UnityScriptAnalyzer
{
    public class Consts
    {
        public const string DiagnosticId = "UnityScriptAnalyzer";
        public const string Category = "Performance";

        public static string[] UnityCallbacksFunctions = 
        {
            "Awake",
            "FixedUpdate",
            "LateUpdate",
            "OnAnimatorIK",
            "OnAnimatorMove",
            "OnApplicationFocus",
            "OnApplicationPause",
            "OnApplicationQuit",
            "OnAudioFilterRead",
            "OnBecameInvisible",
            "OnBecameVisible",
            "OnCollisionEnter",
            "OnCollisionEnter2D",
            "OnCollisionExit",
            "OnCollisionExit2D",
            "OnCollisionStay",
            "OnCollisionStay2D",
            "OnConnectedToServer",
            "OnControllerColliderHit",
            "OnDestroy",
            "OnDisable",
            "OnDisconnectedFromServer",
            "OnDrawGizmos",
            "OnDrawGizmosSelected",
            "OnEnable",
            "OnFailedToConnect",
            "OnFailedToConnectToMasterServer",
            "OnGUI",
            "OnJointBreak",
            "OnJointBreak2D",
            "OnMasterServerEvent",
            "OnMouseDown",
            "OnMouseDrag",
            "OnMouseEnter",
            "OnMouseExit",
            "OnMouseOver",
            "OnMouseUp",
            "OnMouseUpAsButton",
            "OnNetworkInstantiate",
            "OnParticleCollision",
            "OnParticleTrigger",
            "OnPlayerConnected",
            "OnPlayerDisconnected",
            "OnPostRender",
            "OnPreCull",
            "OnPreRender",
            "OnRenderImage",
            "OnRenderObject",
            "OnSerializeNetworkView",
            "OnServerInitialized",
            "OnTransformChildrenChanged",
            "OnTransformParentChanged",
            "OnTriggerEnter",
            "OnTriggerEnter2D",
            "OnTriggerExit",
            "OnTriggerExit2D",
            "OnTriggerStay",
            "OnTriggerStay2D",
            "OnValidate",
            "OnWillRenderObject",
            "Reset",
            "Start",
            "Update",
        };

        public static string[] RepeatingUnityCallbacksFunctions = 
        {
            "FixedUpdate",
            "LateUpdate",
            "OnPostRender",
            "OnPreCull",
            "OnPreRender",
            "OnRenderImage",
            "OnRenderObject",
            "OnWillRenderObject",
            "Update",
        };

        public static string[] ShaderPropertyIDFunctions =
        {
            "UnityEngine.Material.GetColor(string",
            "UnityEngine.Material.GetColorArray(string",
            "UnityEngine.Material.GetFloat(string",
            "UnityEngine.Material.GetFloatArray(string",
            "UnityEngine.Material.GetInt(string",
            "UnityEngine.Material.GetMatrix(string",
            "UnityEngine.Material.GetMatrixArray(string",
            "UnityEngine.Material.GetPassName(string",
            "UnityEngine.Material.GetTexture(string",
            "UnityEngine.Material.GetTextureOffset(string",
            "UnityEngine.Material.GetTextureScale(string",
            "UnityEngine.Material.GetVector(string",
            "UnityEngine.Material.GetVectorArray(string",
            "UnityEngine.Material.HasProperty(string",
            "UnityEngine.Material.SetBuffer(string",
            "UnityEngine.Material.SetColor(string",
            "UnityEngine.Material.SetColorArray(string",
            "UnityEngine.Material.SetFloat(string",
            "UnityEngine.Material.SetFloatArray(string",
            "UnityEngine.Material.SetInt(string",
            "UnityEngine.Material.SetMatrix(string",
            "UnityEngine.Material.SetMatrixArray(string",
            "UnityEngine.Material.SetTexture(string",
            "UnityEngine.Material.SetTextureOffset(string",
            "UnityEngine.Material.SetTextureScale(string",
            "UnityEngine.Material.SetVector(string",
            "UnityEngine.Material.SetVectorArray(string",
        };

        public static string[] AnimatorPropertyIDFucntions =
        {
            "UnityEngine.Animator.GetBool(string",
            "UnityEngine.Animator.GetFloat(string",
            "UnityEngine.Animator.GetInteger(string",
            "UnityEngine.Animator.SetBool(string",
            "UnityEngine.Animator.SetFloat(string",
            "UnityEngine.Animator.SetInteger(string",
            "UnityEngine.Animator.SetTrigger(string",
        };

        public static string[] ExpensiveUnityFunctions =
        {
            "UnityEngine.Component.GetComponent",
            "UnityEngine.Component.GetComponentInChildren",
            "UnityEngine.Component.GetComponentInParent",
            "UnityEngine.Component.GetComponents",
            "UnityEngine.Component.GetComponentsInChildren",
            "UnityEngine.Component.GetComponentsInParent",
            "UnityEngine.Object.Instantiate(",
            "UnityEngine.Object.FindObjectOfType(",
            "UnityEngine.Object.FindObjectsOfType(",
            "UnityEngine.GameObject.FindGameObjectsWithTag(",
            "UnityEngine.GameObject.Find(",
            "UnityEngine.GameObject.FindWithTag(",
            "UnityEngine.Physics.Raycast(",
            "UnityEngine.Physics.RaycastAll(",
        };

    }
}
