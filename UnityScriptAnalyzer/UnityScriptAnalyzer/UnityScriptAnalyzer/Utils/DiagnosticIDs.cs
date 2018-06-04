using System;
using System.Collections.Generic;
using System.Text;

namespace UnityScriptAnalyzer
{
    public class DiagnosticIDs
    {
        public const string EmptyUnityCallback              = "USA0001";
        public const string GetComponentString              = "USA0002";
        public const string LINQUsage                       = "USA0003";
        public const string UnityMessaging                  = "USA0004";
        public const string DateTimeNow                     = "USA0005";
        public const string VectorMagnitude                 = "USA0006";
        public const string VectorDistance                  = "USA0007";
        public const string ShaderPropertyID                = "USA0008";
        public const string AnimatorPropertyID              = "USA0009";
        public const string RaycastLayerMask                = "USA0010";
        public const string LambdaUsage                     = "USA0011";
        public const string ExpensiveRepeatedUnityFuncs     = "USA0012";
        public const string CacheMainCamera                 = "USA0013";
        public const string CompareTag                      = "USA0014";
        public const string MeshVertices                    = "USA0015";
    }
}
