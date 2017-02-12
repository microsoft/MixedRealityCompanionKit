using UnityEngine;
using HoloToolkit.Sharing;

public class ProjectileLauncher : MonoBehaviour
{
    /// <summary>
    /// Keep Track of the last shot time to throttle users' shots
    /// </summary>
    float LastShotTime = 0;

    void Start()
    {
        CustomMessages.Instance.MessageHandlers[CustomMessages.TestMessageID.ShootProjectile] = this.ProcessRemoteProjectile;

        // We will use the camera's audio source to play sounds whenever a projectile hits the user's avatar
        // or when the user hits another player's avatar with a projectile.
        if (Camera.main.gameObject.GetComponent<AudioSource>() == null)
        {
            // Add an AudioSource and spatialize it.
            AudioSource audio = Camera.main.gameObject.AddComponent<AudioSource>() as AudioSource;
            audio.playOnAwake = false;
            audio.spatialize = true;
            audio.spatialBlend = 1.0f;
            audio.rolloffMode = AudioRolloffMode.Custom;
        }
    }

    /// <summary>
    /// OnSelect is sent by gesture manager.
    /// </summary>
    void OnSelect()
    {
        if (Time.realtimeSinceStartup - LastShotTime > 1)
        {
            LastShotTime = Time.realtimeSinceStartup;
            SpawnProjectile(0);
        }
    }

    /// <summary>
    /// Spawns a new projectile in the world if the user
    /// doesn't already have one.
    /// </summary>
    void SpawnProjectile(long UserId)
    {
        Vector3 ProjectilePosition = Camera.main.transform.position + Camera.main.transform.forward * 0.85f;
        Vector3 ProjectileDirection = Camera.main.transform.forward;

        ShootProjectile(ProjectilePosition,
                    ProjectileDirection, UserId);

        Transform anchor = SpectatorView.SV_ImportExportAnchorManager.Instance.gameObject.transform;
        CustomMessages.Instance.SendShootProjectile(anchor.InverseTransformPoint(ProjectilePosition), anchor.InverseTransformDirection(ProjectileDirection));
    }


    /// <summary>
    /// Adds a new projectile to the world.
    /// </summary>
    /// <param name="start">Position to shoot from</param>
    /// <param name="direction">Position to shoot toward</param>
    /// <param name="radius">Size of destruction when colliding.</param>
    void ShootProjectile(Vector3 start, Vector3 direction, long OwningUser)
    {
        // Need to know the index in the PlayerAvatarStore to grab for this projectile's behavior.
        int AvatarIndex = 0;

        // Special case ID 0 to mean the local user.  
        if (OwningUser == 0)
        {
            AvatarIndex = LocalPlayerManager.Instance.AvatarIndex;
        }
        else
        {
            RemotePlayerManager.RemoteHeadInfo headInfo = RemotePlayerManager.Instance.GetRemoteHeadInfo(OwningUser);
            AvatarIndex = headInfo.PlayerAvatarIndex;
        }

        PlayerAvatarParameters ownerAvatarParameters = PlayerAvatarStore.Instance.PlayerAvatars[AvatarIndex].GetComponent<PlayerAvatarParameters>();

        GameObject spawnedProjectile = (GameObject)Instantiate(ownerAvatarParameters.PlayerShotObject);
        spawnedProjectile.transform.position = start;

        // Set projectile color to be the same as the avatar color.
        FriendlyDrone drone = PlayerAvatarStore.Instance.PlayerAvatars[AvatarIndex].GetComponentInChildren<FriendlyDrone>();
        if (drone != null)
        {
            spawnedProjectile.GetComponentInChildren<Renderer>().materials[1].SetColor("_EmissionColor", drone.EmissiveColor);
            foreach(ParticleSystem particleSystem in spawnedProjectile.transform.GetComponentsInChildren<ParticleSystem>())
            {
                particleSystem.startColor = drone.EmissiveColor;
            }
        }

        ProjectileBehavior pc = spawnedProjectile.GetComponentInChildren<ProjectileBehavior>();
        pc.startDir = direction;
        pc.OwningUserId = OwningUser;
    }

    /// <summary>
    /// Process user hit.
    /// </summary>
    /// <param name="msg"></param>
    void ProcessRemoteProjectile(NetworkInMessage msg)
    {
        // Parse the message
        long userID = msg.ReadInt64();
        Vector3 remoteProjectilePosition = CustomMessages.Instance.ReadVector3(msg);

        Vector3 remoteProjectileDirection = CustomMessages.Instance.ReadVector3(msg);

        Transform anchor = SpectatorView.SV_ImportExportAnchorManager.Instance.gameObject.transform;
        ShootProjectile(anchor.TransformPoint(remoteProjectilePosition), anchor.TransformDirection(remoteProjectileDirection), userID);
    }
}
