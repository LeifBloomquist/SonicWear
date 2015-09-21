namespace SomoTestApp
{
    partial class MainForm
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
            this.InfoBox1 = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.RawDataStreamText = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.InfoBox2 = new System.Windows.Forms.TextBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.InfoBox3 = new System.Windows.Forms.TextBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.InfoBox4 = new System.Windows.Forms.TextBox();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.FakeDataButton = new System.Windows.Forms.Button();
            this.ApplyOffsetCheckBox = new System.Windows.Forms.CheckBox();
            this.ClearStatsButton = new System.Windows.Forms.Button();
            this.DisconnectButton = new System.Windows.Forms.Button();
            this.BaudRate = new System.Windows.Forms.ComboBox();
            this.ConnectButton = new System.Windows.Forms.Button();
            this.COMPorts = new System.Windows.Forms.ComboBox();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.ErrorMessages = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.groupBox7.SuspendLayout();
            this.SuspendLayout();
            // 
            // InfoBox1
            // 
            this.InfoBox1.BackColor = System.Drawing.SystemColors.Window;
            this.InfoBox1.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.InfoBox1.Location = new System.Drawing.Point(8, 21);
            this.InfoBox1.Margin = new System.Windows.Forms.Padding(4);
            this.InfoBox1.Multiline = true;
            this.InfoBox1.Name = "InfoBox1";
            this.InfoBox1.ReadOnly = true;
            this.InfoBox1.Size = new System.Drawing.Size(205, 206);
            this.InfoBox1.TabIndex = 0;
            this.InfoBox1.Text = "No Data";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.RawDataStreamText);
            this.groupBox1.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(7, 85);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox1.Size = new System.Drawing.Size(981, 231);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Raw Data Stream";
            // 
            // RawDataStreamText
            // 
            this.RawDataStreamText.BackColor = System.Drawing.SystemColors.Window;
            this.RawDataStreamText.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RawDataStreamText.Location = new System.Drawing.Point(8, 16);
            this.RawDataStreamText.Margin = new System.Windows.Forms.Padding(4);
            this.RawDataStreamText.Multiline = true;
            this.RawDataStreamText.Name = "RawDataStreamText";
            this.RawDataStreamText.ReadOnly = true;
            this.RawDataStreamText.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.RawDataStreamText.Size = new System.Drawing.Size(961, 207);
            this.RawDataStreamText.TabIndex = 2;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.InfoBox1);
            this.groupBox2.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox2.Location = new System.Drawing.Point(7, 322);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox2.Size = new System.Drawing.Size(225, 235);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Unit #1";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.InfoBox2);
            this.groupBox3.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox3.Location = new System.Drawing.Point(259, 322);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox3.Size = new System.Drawing.Size(225, 235);
            this.groupBox3.TabIndex = 4;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Unit #2";
            // 
            // InfoBox2
            // 
            this.InfoBox2.BackColor = System.Drawing.SystemColors.Window;
            this.InfoBox2.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.InfoBox2.Location = new System.Drawing.Point(8, 21);
            this.InfoBox2.Margin = new System.Windows.Forms.Padding(4);
            this.InfoBox2.Multiline = true;
            this.InfoBox2.Name = "InfoBox2";
            this.InfoBox2.ReadOnly = true;
            this.InfoBox2.Size = new System.Drawing.Size(205, 206);
            this.InfoBox2.TabIndex = 1;
            this.InfoBox2.Text = "No Data";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.InfoBox3);
            this.groupBox4.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox4.Location = new System.Drawing.Point(511, 322);
            this.groupBox4.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox4.Size = new System.Drawing.Size(225, 235);
            this.groupBox4.TabIndex = 5;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Unit #3";
            // 
            // InfoBox3
            // 
            this.InfoBox3.BackColor = System.Drawing.SystemColors.Window;
            this.InfoBox3.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.InfoBox3.Location = new System.Drawing.Point(8, 21);
            this.InfoBox3.Margin = new System.Windows.Forms.Padding(4);
            this.InfoBox3.Multiline = true;
            this.InfoBox3.Name = "InfoBox3";
            this.InfoBox3.ReadOnly = true;
            this.InfoBox3.Size = new System.Drawing.Size(205, 206);
            this.InfoBox3.TabIndex = 1;
            this.InfoBox3.Text = "No Data";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.InfoBox4);
            this.groupBox5.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox5.Location = new System.Drawing.Point(763, 322);
            this.groupBox5.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox5.Size = new System.Drawing.Size(225, 235);
            this.groupBox5.TabIndex = 6;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Unit #4";
            // 
            // InfoBox4
            // 
            this.InfoBox4.BackColor = System.Drawing.SystemColors.Window;
            this.InfoBox4.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.InfoBox4.Location = new System.Drawing.Point(0, 21);
            this.InfoBox4.Margin = new System.Windows.Forms.Padding(4);
            this.InfoBox4.Multiline = true;
            this.InfoBox4.Name = "InfoBox4";
            this.InfoBox4.ReadOnly = true;
            this.InfoBox4.Size = new System.Drawing.Size(205, 206);
            this.InfoBox4.TabIndex = 1;
            this.InfoBox4.Text = "No Data";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.FakeDataButton);
            this.groupBox6.Controls.Add(this.ApplyOffsetCheckBox);
            this.groupBox6.Controls.Add(this.ClearStatsButton);
            this.groupBox6.Controls.Add(this.DisconnectButton);
            this.groupBox6.Controls.Add(this.BaudRate);
            this.groupBox6.Controls.Add(this.ConnectButton);
            this.groupBox6.Controls.Add(this.COMPorts);
            this.groupBox6.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox6.Location = new System.Drawing.Point(7, 4);
            this.groupBox6.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox6.Size = new System.Drawing.Size(981, 78);
            this.groupBox6.TabIndex = 4;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Controls";
            // 
            // FakeDataButton
            // 
            this.FakeDataButton.Location = new System.Drawing.Point(555, 34);
            this.FakeDataButton.Name = "FakeDataButton";
            this.FakeDataButton.Size = new System.Drawing.Size(93, 23);
            this.FakeDataButton.TabIndex = 6;
            this.FakeDataButton.Text = "Fake Data";
            this.FakeDataButton.UseVisualStyleBackColor = true;
            this.FakeDataButton.Visible = false;
            this.FakeDataButton.Click += new System.EventHandler(this.FakeDataButton_Click);
            // 
            // ApplyOffsetCheckBox
            // 
            this.ApplyOffsetCheckBox.AutoSize = true;
            this.ApplyOffsetCheckBox.Location = new System.Drawing.Point(739, 36);
            this.ApplyOffsetCheckBox.Name = "ApplyOffsetCheckBox";
            this.ApplyOffsetCheckBox.Size = new System.Drawing.Size(110, 19);
            this.ApplyOffsetCheckBox.TabIndex = 5;
            this.ApplyOffsetCheckBox.Text = "Apply Offset";
            this.ApplyOffsetCheckBox.UseVisualStyleBackColor = true;
            // 
            // ClearStatsButton
            // 
            this.ClearStatsButton.Location = new System.Drawing.Point(870, 34);
            this.ClearStatsButton.Name = "ClearStatsButton";
            this.ClearStatsButton.Size = new System.Drawing.Size(93, 23);
            this.ClearStatsButton.TabIndex = 4;
            this.ClearStatsButton.Text = "Clear Stats";
            this.ClearStatsButton.UseVisualStyleBackColor = true;
            this.ClearStatsButton.Click += new System.EventHandler(this.ClearStatsButton_Click);
            // 
            // DisconnectButton
            // 
            this.DisconnectButton.Location = new System.Drawing.Point(358, 34);
            this.DisconnectButton.Name = "DisconnectButton";
            this.DisconnectButton.Size = new System.Drawing.Size(93, 23);
            this.DisconnectButton.TabIndex = 3;
            this.DisconnectButton.Text = "Disconnect";
            this.DisconnectButton.UseVisualStyleBackColor = true;
            this.DisconnectButton.Click += new System.EventHandler(this.DisconnectButton_Click);
            // 
            // BaudRate
            // 
            this.BaudRate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.BaudRate.FormattingEnabled = true;
            this.BaudRate.Items.AddRange(new object[] {
            "9600",
            "38400",
            "57600",
            "115200"});
            this.BaudRate.Location = new System.Drawing.Point(135, 34);
            this.BaudRate.Name = "BaudRate";
            this.BaudRate.Size = new System.Drawing.Size(121, 23);
            this.BaudRate.TabIndex = 2;
            // 
            // ConnectButton
            // 
            this.ConnectButton.Location = new System.Drawing.Point(277, 34);
            this.ConnectButton.Name = "ConnectButton";
            this.ConnectButton.Size = new System.Drawing.Size(75, 23);
            this.ConnectButton.TabIndex = 1;
            this.ConnectButton.Text = "Connect";
            this.ConnectButton.UseVisualStyleBackColor = true;
            this.ConnectButton.Click += new System.EventHandler(this.ConnectButton_Click);
            // 
            // COMPorts
            // 
            this.COMPorts.DisplayMember = "dfdf";
            this.COMPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.COMPorts.FormattingEnabled = true;
            this.COMPorts.Items.AddRange(new object[] {
            "COM Port"});
            this.COMPorts.Location = new System.Drawing.Point(8, 34);
            this.COMPorts.MaxDropDownItems = 20;
            this.COMPorts.Name = "COMPorts";
            this.COMPorts.Size = new System.Drawing.Size(121, 23);
            this.COMPorts.Sorted = true;
            this.COMPorts.TabIndex = 0;
            this.COMPorts.ValueMember = "Items";
            this.COMPorts.Click += new System.EventHandler(this.COMPorts_Click);
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.ErrorMessages);
            this.groupBox7.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox7.Location = new System.Drawing.Point(7, 565);
            this.groupBox7.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox7.Size = new System.Drawing.Size(981, 122);
            this.groupBox7.TabIndex = 3;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Messages";
            // 
            // ErrorMessages
            // 
            this.ErrorMessages.BackColor = System.Drawing.SystemColors.Window;
            this.ErrorMessages.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ErrorMessages.Location = new System.Drawing.Point(8, 16);
            this.ErrorMessages.Margin = new System.Windows.Forms.Padding(4);
            this.ErrorMessages.Multiline = true;
            this.ErrorMessages.Name = "ErrorMessages";
            this.ErrorMessages.ReadOnly = true;
            this.ErrorMessages.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.ErrorMessages.Size = new System.Drawing.Size(961, 103);
            this.ErrorMessages.TabIndex = 2;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1002, 700);
            this.Controls.Add(this.groupBox7);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("Consolas", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MainForm";
            this.Text = "SOMO Data Analyzer 0.1";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.groupBox7.ResumeLayout(false);
            this.groupBox7.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox InfoBox1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox RawDataStreamText;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.ComboBox COMPorts;
        private System.Windows.Forms.Button ConnectButton;
        private System.Windows.Forms.ComboBox BaudRate;
        private System.Windows.Forms.Button DisconnectButton;
        private System.Windows.Forms.Button ClearStatsButton;
        private System.Windows.Forms.CheckBox ApplyOffsetCheckBox;
        private System.Windows.Forms.TextBox InfoBox2;
        private System.Windows.Forms.TextBox InfoBox3;
        private System.Windows.Forms.TextBox InfoBox4;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.TextBox ErrorMessages;
        private System.Windows.Forms.Button FakeDataButton;
    }
}

