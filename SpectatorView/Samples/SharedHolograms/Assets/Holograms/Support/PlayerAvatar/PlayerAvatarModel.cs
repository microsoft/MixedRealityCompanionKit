using UnityEngine;
using System.Collections;

/// <summary>
/// Controls the model being used to represent a user.
/// </summary>
public class PlayerAvatarModel : MonoBehaviour
{
    /// <summary>
    /// Changes the model used to represent a user.
    /// </summary>    
    public void SetAvatar(GameObject avatar)
    {
        PlayerAvatarParameters playerParams = avatar.GetComponent<PlayerAvatarParameters>();
        
        GetComponent<MeshFilter>().mesh = avatar.GetComponent<MeshFilter>().sharedMesh;
        GetComponent<MeshRenderer>().material = avatar.GetComponent<MeshRenderer>().sharedMaterial;

        Transform avatarTransform = avatar.GetComponent<Transform>();
        transform.localPosition = avatarTransform.localPosition;
        transform.localRotation = avatarTransform.localRotation;
        transform.localScale = avatarTransform.localScale;

        MeshCollider mc = GetComponent<MeshCollider>();
        mc.sharedMesh = null;
        mc.sharedMesh = GetComponent<MeshFilter>().mesh;

        AudioSource audioSource = GetComponent<AudioSource>();
        audioSource.clip = playerParams.PlayerJoinedClip;
        audioSource.Play();
    }
}