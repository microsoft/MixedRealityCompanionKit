namespace TestApp
{
    partial class theForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.bInitialize = new System.Windows.Forms.Button();
            this.bConnect = new System.Windows.Forms.Button();
            this.tATR = new System.Windows.Forms.TextBox();
            this.tUID = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.tData = new System.Windows.Forms.TextBox();
            this.bRead = new System.Windows.Forms.Button();
            this.bDisconnect = new System.Windows.Forms.Button();
            this.bRelease = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.tType = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.bWrite = new System.Windows.Forms.Button();
            this.bRaw = new System.Windows.Forms.Button();
            this.dReaders = new System.Windows.Forms.ComboBox();
            this.btTogglePoll = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // bInitialize
            // 
            this.bInitialize.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bInitialize.Location = new System.Drawing.Point(12, 12);
            this.bInitialize.Name = "bInitialize";
            this.bInitialize.Size = new System.Drawing.Size(89, 23);
            this.bInitialize.TabIndex = 0;
            this.bInitialize.Text = "Intialize";
            this.bInitialize.UseVisualStyleBackColor = true;
            this.bInitialize.Click += new System.EventHandler(this.bInitialize_Click);
            // 
            // bConnect
            // 
            this.bConnect.Location = new System.Drawing.Point(12, 54);
            this.bConnect.Name = "bConnect";
            this.bConnect.Size = new System.Drawing.Size(89, 23);
            this.bConnect.TabIndex = 1;
            this.bConnect.Text = "Connect";
            this.bConnect.UseVisualStyleBackColor = true;
            this.bConnect.Click += new System.EventHandler(this.bConnect_Click);
            // 
            // tATR
            // 
            this.tATR.Location = new System.Drawing.Point(151, 56);
            this.tATR.Name = "tATR";
            this.tATR.Size = new System.Drawing.Size(366, 20);
            this.tATR.TabIndex = 2;
            // 
            // tUID
            // 
            this.tUID.Location = new System.Drawing.Point(151, 82);
            this.tUID.Name = "tUID";
            this.tUID.Size = new System.Drawing.Size(366, 20);
            this.tUID.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(116, 59);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "ATR";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(119, 85);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(26, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "UID";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(116, 137);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Guid";
            // 
            // tData
            // 
            this.tData.Location = new System.Drawing.Point(151, 134);
            this.tData.Multiline = true;
            this.tData.Name = "tData";
            this.tData.Size = new System.Drawing.Size(366, 126);
            this.tData.TabIndex = 7;
            // 
            // bRead
            // 
            this.bRead.Enabled = false;
            this.bRead.Location = new System.Drawing.Point(12, 117);
            this.bRead.Name = "bRead";
            this.bRead.Size = new System.Drawing.Size(89, 23);
            this.bRead.TabIndex = 8;
            this.bRead.Text = "Read GUID";
            this.bRead.UseVisualStyleBackColor = true;
            this.bRead.Click += new System.EventHandler(this.bRead_Click);
            // 
            // bDisconnect
            // 
            this.bDisconnect.Location = new System.Drawing.Point(12, 181);
            this.bDisconnect.Name = "bDisconnect";
            this.bDisconnect.Size = new System.Drawing.Size(89, 23);
            this.bDisconnect.TabIndex = 9;
            this.bDisconnect.Text = "Disconnect";
            this.bDisconnect.UseVisualStyleBackColor = true;
            this.bDisconnect.Click += new System.EventHandler(this.bDisconnect_Click);
            // 
            // bRelease
            // 
            this.bRelease.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bRelease.Location = new System.Drawing.Point(428, 12);
            this.bRelease.Name = "bRelease";
            this.bRelease.Size = new System.Drawing.Size(89, 23);
            this.bRelease.TabIndex = 10;
            this.bRelease.Text = "Release";
            this.bRelease.UseVisualStyleBackColor = true;
            this.bRelease.Click += new System.EventHandler(this.bRelease_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(114, 111);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(31, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "Type";
            // 
            // tType
            // 
            this.tType.Location = new System.Drawing.Point(151, 108);
            this.tType.Name = "tType";
            this.tType.Size = new System.Drawing.Size(366, 20);
            this.tType.TabIndex = 12;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(121, 17);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(24, 13);
            this.label5.TabIndex = 14;
            this.label5.Text = "Rdr";
            // 
            // bWrite
            // 
            this.bWrite.Enabled = false;
            this.bWrite.Location = new System.Drawing.Point(12, 146);
            this.bWrite.Name = "bWrite";
            this.bWrite.Size = new System.Drawing.Size(89, 23);
            this.bWrite.TabIndex = 15;
            this.bWrite.Text = "Write GUID";
            this.bWrite.UseVisualStyleBackColor = true;
            this.bWrite.Click += new System.EventHandler(this.bWrite_Click);
            // 
            // bRaw
            // 
            this.bRaw.Location = new System.Drawing.Point(12, 88);
            this.bRaw.Name = "bRaw";
            this.bRaw.Size = new System.Drawing.Size(89, 23);
            this.bRaw.TabIndex = 16;
            this.bRaw.Text = "Read Raw";
            this.bRaw.UseVisualStyleBackColor = true;
            this.bRaw.Click += new System.EventHandler(this.bRaw_Click);
            // 
            // dReaders
            // 
            this.dReaders.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.dReaders.FormattingEnabled = true;
            this.dReaders.Location = new System.Drawing.Point(151, 14);
            this.dReaders.Name = "dReaders";
            this.dReaders.Size = new System.Drawing.Size(255, 21);
            this.dReaders.TabIndex = 17;
            // 
            // btTogglePoll
            // 
            this.btTogglePoll.Location = new System.Drawing.Point(12, 237);
            this.btTogglePoll.Name = "btTogglePoll";
            this.btTogglePoll.Size = new System.Drawing.Size(89, 23);
            this.btTogglePoll.TabIndex = 18;
            this.btTogglePoll.Tag = "";
            this.btTogglePoll.Text = "Start Polling";
            this.btTogglePoll.UseVisualStyleBackColor = true;
            this.btTogglePoll.Click += new System.EventHandler(this.btTogglePoll_Click);
            // 
            // theForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(531, 272);
            this.Controls.Add(this.btTogglePoll);
            this.Controls.Add(this.dReaders);
            this.Controls.Add(this.bRaw);
            this.Controls.Add(this.bWrite);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.tType);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.bRelease);
            this.Controls.Add(this.bDisconnect);
            this.Controls.Add(this.bRead);
            this.Controls.Add(this.tData);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tUID);
            this.Controls.Add(this.tATR);
            this.Controls.Add(this.bConnect);
            this.Controls.Add(this.bInitialize);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "theForm";
            this.Text = "NFCH Test App";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.theForm_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button bInitialize;
        private System.Windows.Forms.Button bConnect;
        private System.Windows.Forms.TextBox tATR;
        private System.Windows.Forms.TextBox tUID;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tData;
        private System.Windows.Forms.Button bRead;
        private System.Windows.Forms.Button bDisconnect;
        private System.Windows.Forms.Button bRelease;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tType;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button bWrite;
        private System.Windows.Forms.Button bRaw;
        private System.Windows.Forms.ComboBox dReaders;
        private System.Windows.Forms.Button btTogglePoll;
    }
}

