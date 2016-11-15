// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using System.Collections.Generic;
using UnityEngine;

namespace PosterAlignment
{
    public class WorldPosFromRays : IComparable<WorldPosFromRays>
    {
        public int PositionId;
        public List<Ray> Rays = new List<Ray>();
        public Vector3 EstimatedWorldPos = Vector3.zero;
        public bool HasWorldPos = false;

        private const float RayAngleClosenessRejectionThreshold = 0.99f;//97f;

        public Ray LatestRay { get; private set; }

        public int CompareTo(WorldPosFromRays other)
        {
            return this.PositionId.CompareTo(other.PositionId);
        }

        public WorldPosFromRays(int posId)
        {
            this.PositionId = posId;
        }

        public float RayWeight(Ray ray)
        {
            if (this.HasWorldPos)
            {
                return 1.0f / ((ray.origin - this.EstimatedWorldPos).magnitude);
            }
            else
            {
                return 1.0f;
            }
        }
        public float ApproxDistanceToNewRay(Ray ray)
        {
            if (!this.HasWorldPos) return 0.0f;

            var worldPos = this.EstimatedWorldPos;
            var dir = ray.direction.normalized;
            var amtAlong = Vector3.Dot(dir, worldPos - ray.origin);
            var closest = ray.origin + (dir * amtAlong);
            var dist = (worldPos - closest).magnitude;

            return dist;
        }

        public void Reset()
        {
            this.HasWorldPos = false;
            this.Rays.Clear();
        }

        public void AddRay(Ray ray, Vector3 cameraPos, Matrix4x4 projMatrix)
        {
            this.Rays.Add(ray);
            this.LatestRay = ray;

            // Update the estimate
            Vector3 sumAvg = Vector3.zero;
            float totalWeight = 0.0f;
            int sampleCount = 0;
            for (int i = 0; i < this.Rays.Count; ++i)
            {
                for (int j = (i + 1); j < this.Rays.Count; ++j)
                {
                    var ray_0 = this.Rays[i];
                    var ray_1 = this.Rays[j];

                    // Reject any with angles too close...
                    var delta = Vector3.Dot(ray_1.direction.normalized, ray_0.direction.normalized);
                    if (delta < WorldPosFromRays.RayAngleClosenessRejectionThreshold)
                    {
                        // if the rays are sufficiently pointing in different directions:
                        Vector3 avg = GetClosestPointBetweenLines(
                            ray_1.origin, ray_1.direction,
                            ray_0.origin, ray_0.direction);

                        var weight = 1;// HasWorldPos ? (0.01f / Mathf.Max((avg - EstimatedWorldPos).magnitude, 0.01f)) : 1.0f; // Weight it based on the distance from the estimated center...
                        sumAvg += (avg * weight);
                        totalWeight += weight;
                        ++sampleCount;
                    }
                }
            }


            if (totalWeight > 0)
            {
                this.EstimatedWorldPos = sumAvg / totalWeight;
                this.HasWorldPos = true;
            }
        }

        public static Vector3 GetClosestPointBetweenLines(Vector3 point1, Vector3 direction1,
            Vector3 point2, Vector3 direction2)
        {
            var normalizedDirection1 = direction1.normalized; // just to be sure
            var normalizedDirection2 = direction2.normalized; // just to be sure

            var directionProjection = Vector3.Dot(normalizedDirection1, normalizedDirection2);
            if (directionProjection == 1)
            {
                // parallel, do something better than returning v3.zero
                return point1;
            }

            var projection1 = Vector3.Dot(point2 - point1, normalizedDirection1);
            var projection2 = Vector3.Dot(point2 - point1, normalizedDirection2);
            var distanceAlongLine1 = (projection1 - directionProjection * projection2) / (1 - directionProjection * directionProjection);
            var distanceAlongLine2 = (projection2 - directionProjection * projection1) / (directionProjection * directionProjection - 1);
            var pointOnLine1 = point1 + distanceAlongLine1 * normalizedDirection1;
            var pointOnLine2 = point2 + distanceAlongLine2 * normalizedDirection2;

            var closestPoint = Vector3.Lerp(pointOnLine2, pointOnLine1, 0.5f);


            return closestPoint;
        }
    }
}