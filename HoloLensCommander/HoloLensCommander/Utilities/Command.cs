// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Windows.Input;

namespace HoloLensCommander
{
    /// <summary>
    /// Simple implementation of the System.Windows.Input.ICommand interface.
    /// </summary>
    public sealed class Command : ICommand
    {
        /// <summary>
        /// The action to be performed when Execute() is called.
        /// </summary>
        private readonly Action<object> commandAction;

        /// <summary>
        /// Event that is notified when the ability of the command to be executed has changed.
        /// </summary>
        public event EventHandler CanExecuteChanged;

        /// <summary>
        /// Creates a new command that can always execute.
        /// </summary>
        /// <param name="action">The action associated with this command.</param>
        public Command(Action<object> action)
        {
            if (action == null)
            {
                throw new ArgumentNullException();
            }

            //  Set the action.
            this.commandAction = action;
        }

        /// <summary>
        /// Determines whether this command can execute in its current state.
        /// </summary>
        /// <param name="parameter">Data used by the command. If the command does not require data to be passed, 
        /// this object can be set to null.
        /// </param>
        /// <returns>In this simple implementation, CanExecute always returns true and the parameter is always ignored.</returns>
        public bool CanExecute(object parameter)
        {
            return true;
        }

        /// <summary>
        /// Executes the command.
        /// </summary>
        /// <param name="parameter">Data used by the command. If the command does not require data to be passed, 
        /// this object can be set to null.
        /// </param>
        /// <remarks>In this simple implementation the parameter is always ignored.</remarks>
        public void Execute(object parameter)
        {
            this.commandAction?.Invoke(parameter);
        }

        /// <summary>
        /// Sends the CanExecuteChanged event to registered listeners.
        /// </summary>
        public void NotifyCanExecuteChanged()
        {
            this.CanExecuteChanged?.Invoke(this, EventArgs.Empty);
        }
    }
}