using System.Collections;
using System.Collections.Generic;
using MixedRemoteViewCompositor;
using UnityEngine;

public class CameraMatricesTest : MonoBehaviour {

	[SerializeField] MrvcManager _manager;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		CameraMatrices matrices = new CameraMatrices();
		if(_manager.TryGetCaptureCameraMatrices(ref matrices))
		{
			Debug.LogFormat(this, "[CameraMatricesTest] - We should have matrices, no idea about validity");
		}
		//else
		//{
		//	Debug.LogErrorFormat(this, "[CameraMatricesTest] - We were not delivered matrices");
		//}
	}
}
