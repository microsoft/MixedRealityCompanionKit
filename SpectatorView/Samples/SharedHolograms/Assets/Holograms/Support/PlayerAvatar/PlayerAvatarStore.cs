using UnityEngine;
using System.Collections.Generic;
using HoloToolkit.Unity;

/// <summary>
/// Keeps a list of possible avatars that the user can pick from.
/// </summary>
public class PlayerAvatarStore : SpectatorView.SV_Singleton<PlayerAvatarStore>
{

    /// <summary>
    /// List of possible avatars.
    /// </summary>
    public GameObject[] PlayerAvatars;

    /// <summary>
    /// Audio clip to play when 'picker' UI is active.
    /// </summary>
    public AudioClip PickerActiveSound;

    /// <summary>
    /// List of spawned versions of player avatars.  
    /// Used for the 'picker' UI.
    /// </summary>
    private List<GameObject> spawnedPlayerAvatars = new List<GameObject>();

    /// <summary>
    /// Tracks if the picker is open.
    /// </summary>
    public bool PickerActive { get; set; }

    /// <summary>
    /// Distance to display the avatar picker in front of the user.
    /// </summary>
    public float PickerDistanceFromUser = 2.0f;

    // Distance between avatars.
    private const float avatarPositionOffset = 0.3f;

    // Time when audioSource.Play() was called.
    private float audioStartTime = 0.0f;

    // Time to wait before calling audioSource.Play() again.
    private float audioDelayTime = 2.0f;

    // The spatialized audioSource component.
    private AudioSource audioSource;

    /// <summary>
    /// Spawns the avatar picker.
    /// </summary>
    public void SpawnAvatarPicker()
    {
        // Clear the previous instance of the avatar picker.
        DismissAvatarPicker();

        PickerActive = true;

        // Show the possible avatars in front of user and evenly spaced from right to left.
        Vector3 startPosition = Camera.main.transform.position + Camera.main.transform.forward * PickerDistanceFromUser;
        startPosition.y = Camera.main.transform.position.y;
        transform.position = startPosition;

        int halfAvatarLength = PlayerAvatars.Length / 2;
        for (int index = 0; index < PlayerAvatars.Length; index++)
        {
            Vector3 offset = Camera.main.transform.right * (index - halfAvatarLength) * avatarPositionOffset;

            GameObject nextAvatar = (GameObject)Instantiate(PlayerAvatars[index]);
            nextAvatar.transform.position = startPosition + offset;
            spawnedPlayerAvatars.Add(nextAvatar);

            // Add AvatarSelector component to handle avatar selection by user.
            AvatarSelector avatarSelector = nextAvatar.GetComponent<AvatarSelector>();
            if (avatarSelector == null)
            {
                avatarSelector = nextAvatar.AddComponent<AvatarSelector>();
            }

            avatarSelector.AvatarIndex = index;
        }

        // If this object does not already have an AudioSource component, then add one.
        audioSource = this.gameObject.GetComponent<AudioSource>();
        if (audioSource == null)
        {
            // Add an AudioSource and spatialize it.
            audioSource = this.gameObject.AddComponent<AudioSource>() as AudioSource;
            audioSource.playOnAwake = false;
            audioSource.spatialize = true;
            audioSource.spatialBlend = 1.0f;
            audioSource.rolloffMode = AudioRolloffMode.Custom;
        }

        audioSource.loop = false;
        audioSource.clip = PickerActiveSound;
        audioSource.Play();
        audioStartTime = Time.time;
    }

    // Called every frame.
    void Update()
    {
        if (PickerActive)
        {
            // Our sound does not have built-in looping, so we'll control playback frequency here.
            if ((Time.time - audioStartTime) >= audioDelayTime)
            {
                audioSource.Play();
                audioStartTime = Time.time;
            }
        }
    }

    /// <summary>
    /// Makes the avatar picker go away.
    /// </summary>
    public void DismissAvatarPicker()
    {
        if (audioSource != null && audioSource.isPlaying)
        {
            audioSource.Stop();
        }

        for (int index = 0; index < spawnedPlayerAvatars.Count; index++)
        {
            Destroy(spawnedPlayerAvatars[index]);
        }

        spawnedPlayerAvatars.Clear();

        PickerActive = false;
    }
}