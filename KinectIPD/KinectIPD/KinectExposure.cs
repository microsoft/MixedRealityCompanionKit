// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Xbox.Kinect;

namespace KinectIPD
{
    class KinectExposure
    {
        NuiSensor sensor = null;

        /// <summary>
        /// Value between 0 and 100
        /// </summary>
        /// <param name="val"></param>
        public void SetExposure(float val)
        {
            if (sensor == null)
            {
                Connect();
                //return;
            }
            SetCommand(NUISENSOR_RGB_COMMAND_TYPE.NUISENSOR_RGB_COMMAND_SET_EXPOSURE_TIME_MS, 640.0f * val / 100.0f);
        }

        public void Connect()
        {
            sensor = new NuiSensor();
            SetCommand(NUISENSOR_RGB_COMMAND_TYPE.NUISENSOR_RGB_COMMAND_SET_ACS, 0u);
            Task.Delay(500).Wait();
            SetCommand(NUISENSOR_RGB_COMMAND_TYPE.NUISENSOR_RGB_COMMAND_SET_EXPOSURE_MODE, 3u);

        }

        private void Disconnect()
        {
            if (sensor != null)
            {
                sensor.Dispose();
                sensor = null;
            }
        }

        static float ToSingle(uint value)
        {
            return BitConverter.ToSingle(BitConverter.GetBytes(value), 0);
        }

        static uint ToUInt32(float value)
        {
            return BitConverter.ToUInt32(BitConverter.GetBytes(value), 0);
        }

        private Dictionary<NUISENSOR_RGB_COMMAND_TYPE, uint> GetCommands(NUISENSOR_RGB_COMMAND_TYPE[] commands)
        {
            NUISENSOR_RGB_CHANGE_STREAM_SETTING setting;
            NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY reply;

            setting.SequenceId = 0;
            setting.Commands = commands.Select(command => new NUISENSOR_RGB_CHANGE_SETTING_CMD { Cmd = (uint)command, Arg = 0 }).ToArray();

            lock (sensor)
            {
                sensor.ColorChangeCameraSettings(ref setting, out reply);
            }

            return commands.Zip(reply.Status, (c, r) => { return Tuple.Create(c, r.Data); }).ToDictionary(t => t.Item1, t => t.Item2);
        }

        private uint GetCommand(NUISENSOR_RGB_COMMAND_TYPE command)
        {
            return SetCommand(command, 0);
        }

        private uint SetCommand(NUISENSOR_RGB_COMMAND_TYPE command, uint argument)
        {
            NUISENSOR_RGB_CHANGE_STREAM_SETTING setting;
            NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY reply;

            setting.SequenceId = 0;
            setting.Commands = new NUISENSOR_RGB_CHANGE_SETTING_CMD[1];
            setting.Commands[0].Cmd = (uint)command;
            setting.Commands[0].Arg = argument;

            lock (sensor)
            {
                sensor.ColorChangeCameraSettings(ref setting, out reply);
            }

            return reply.Status[0].Data;
        }

        private void SetCommand(NUISENSOR_RGB_COMMAND_TYPE command, float argument)
        {
            SetCommand(command, ToUInt32(argument));
        }
    }
}
