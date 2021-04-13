
namespace App_Industry_comu
{
    partial class Board_comunication
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
            this.label1 = new System.Windows.Forms.Label();
            this.combox_portt = new System.Windows.Forms.ComboBox();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.button_open = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.button_exit_APP = new System.Windows.Forms.Button();
            this.panel2 = new System.Windows.Forms.Panel();
            this.modbus_finish_task = new System.Windows.Forms.Button();
            this.modbus_create_task = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.button_finish_cclink = new System.Windows.Forms.Button();
            this.button_cclink = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.button_exit_ethercat = new System.Windows.Forms.Button();
            this.button_ethercat = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.button_exit_profinet = new System.Windows.Forms.Button();
            this.button_profinet = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.button_exit_profibus = new System.Windows.Forms.Button();
            this.button_profibus = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.button_exit_canopen = new System.Windows.Forms.Button();
            this.button_canopen = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.button_exit_ethernetip = new System.Windows.Forms.Button();
            this.button_ethernetip = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F);
            this.label1.Location = new System.Drawing.Point(24, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(70, 16);
            this.label1.TabIndex = 2;
            this.label1.Text = "Serial Port";
            // 
            // combox_portt
            // 
            this.combox_portt.FormattingEnabled = true;
            this.combox_portt.Items.AddRange(new object[] {
            "ModbusTCP (Ethernet)",
            "ModbusRTU (Serial)"});
            this.combox_portt.Location = new System.Drawing.Point(100, 28);
            this.combox_portt.Name = "combox_portt";
            this.combox_portt.Size = new System.Drawing.Size(126, 21);
            this.combox_portt.TabIndex = 4;
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(496, 20);
            this.richTextBox1.Margin = new System.Windows.Forms.Padding(2);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(515, 401);
            this.richTextBox1.TabIndex = 5;
            this.richTextBox1.Text = "";
            // 
            // button_open
            // 
            this.button_open.Location = new System.Drawing.Point(242, 21);
            this.button_open.Margin = new System.Windows.Forms.Padding(2);
            this.button_open.Name = "button_open";
            this.button_open.Size = new System.Drawing.Size(112, 32);
            this.button_open.TabIndex = 6;
            this.button_open.Text = "Open";
            this.button_open.UseVisualStyleBackColor = true;
            this.button_open.UseWaitCursor = true;
            this.button_open.Click += new System.EventHandler(this.button_open_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.button_exit_APP);
            this.panel1.Controls.Add(this.panel2);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.button_open);
            this.panel1.Controls.Add(this.combox_portt);
            this.panel1.Location = new System.Drawing.Point(18, 20);
            this.panel1.Margin = new System.Windows.Forms.Padding(2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(473, 401);
            this.panel1.TabIndex = 7;
            // 
            // button_exit_APP
            // 
            this.button_exit_APP.Location = new System.Drawing.Point(358, 22);
            this.button_exit_APP.Margin = new System.Windows.Forms.Padding(2);
            this.button_exit_APP.Name = "button_exit_APP";
            this.button_exit_APP.Size = new System.Drawing.Size(112, 31);
            this.button_exit_APP.TabIndex = 8;
            this.button_exit_APP.Text = "Exit App";
            this.button_exit_APP.UseVisualStyleBackColor = true;
            this.button_exit_APP.UseWaitCursor = true;
            this.button_exit_APP.Click += new System.EventHandler(this.button_exit_APP_Click);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.button_exit_ethernetip);
            this.panel2.Controls.Add(this.button_ethernetip);
            this.panel2.Controls.Add(this.label8);
            this.panel2.Controls.Add(this.button_exit_canopen);
            this.panel2.Controls.Add(this.button_canopen);
            this.panel2.Controls.Add(this.label7);
            this.panel2.Controls.Add(this.button_exit_profibus);
            this.panel2.Controls.Add(this.button_profibus);
            this.panel2.Controls.Add(this.label6);
            this.panel2.Controls.Add(this.button_exit_profinet);
            this.panel2.Controls.Add(this.button_profinet);
            this.panel2.Controls.Add(this.label5);
            this.panel2.Controls.Add(this.button_exit_ethercat);
            this.panel2.Controls.Add(this.button_ethercat);
            this.panel2.Controls.Add(this.label4);
            this.panel2.Controls.Add(this.modbus_finish_task);
            this.panel2.Controls.Add(this.modbus_create_task);
            this.panel2.Controls.Add(this.label3);
            this.panel2.Controls.Add(this.button_finish_cclink);
            this.panel2.Controls.Add(this.button_cclink);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Location = new System.Drawing.Point(14, 64);
            this.panel2.Margin = new System.Windows.Forms.Padding(2);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(446, 317);
            this.panel2.TabIndex = 7;
            // 
            // modbus_finish_task
            // 
            this.modbus_finish_task.Location = new System.Drawing.Point(266, 283);
            this.modbus_finish_task.Margin = new System.Windows.Forms.Padding(2);
            this.modbus_finish_task.Name = "modbus_finish_task";
            this.modbus_finish_task.Size = new System.Drawing.Size(112, 19);
            this.modbus_finish_task.TabIndex = 12;
            this.modbus_finish_task.Text = "finish";
            this.modbus_finish_task.UseVisualStyleBackColor = true;
            this.modbus_finish_task.UseWaitCursor = true;
            this.modbus_finish_task.Click += new System.EventHandler(this.modbus_finish_task_Click);
            // 
            // modbus_create_task
            // 
            this.modbus_create_task.Location = new System.Drawing.Point(132, 282);
            this.modbus_create_task.Margin = new System.Windows.Forms.Padding(2);
            this.modbus_create_task.Name = "modbus_create_task";
            this.modbus_create_task.Size = new System.Drawing.Size(112, 19);
            this.modbus_create_task.TabIndex = 11;
            this.modbus_create_task.Text = "Create ";
            this.modbus_create_task.UseVisualStyleBackColor = true;
            this.modbus_create_task.UseWaitCursor = true;
            this.modbus_create_task.Click += new System.EventHandler(this.modbus_create_task_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F);
            this.label3.Location = new System.Drawing.Point(59, 283);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 16);
            this.label3.TabIndex = 10;
            this.label3.Text = "Modbus";
            // 
            // button_finish_cclink
            // 
            this.button_finish_cclink.Location = new System.Drawing.Point(266, 233);
            this.button_finish_cclink.Margin = new System.Windows.Forms.Padding(2);
            this.button_finish_cclink.Name = "button_finish_cclink";
            this.button_finish_cclink.Size = new System.Drawing.Size(112, 19);
            this.button_finish_cclink.TabIndex = 9;
            this.button_finish_cclink.Text = "finish";
            this.button_finish_cclink.UseVisualStyleBackColor = true;
            this.button_finish_cclink.UseWaitCursor = true;
            this.button_finish_cclink.Click += new System.EventHandler(this.button_finish_cclink_Click);
            // 
            // button_cclink
            // 
            this.button_cclink.Location = new System.Drawing.Point(132, 233);
            this.button_cclink.Margin = new System.Windows.Forms.Padding(2);
            this.button_cclink.Name = "button_cclink";
            this.button_cclink.Size = new System.Drawing.Size(112, 19);
            this.button_cclink.TabIndex = 8;
            this.button_cclink.Text = "Create ";
            this.button_cclink.UseVisualStyleBackColor = true;
            this.button_cclink.UseWaitCursor = true;
            this.button_cclink.Click += new System.EventHandler(this.button_cclink_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F);
            this.label2.Location = new System.Drawing.Point(63, 233);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 16);
            this.label2.TabIndex = 7;
            this.label2.Text = "CC Link";
            // 
            // button_exit_ethercat
            // 
            this.button_exit_ethercat.Location = new System.Drawing.Point(266, 27);
            this.button_exit_ethercat.Margin = new System.Windows.Forms.Padding(2);
            this.button_exit_ethercat.Name = "button_exit_ethercat";
            this.button_exit_ethercat.Size = new System.Drawing.Size(112, 19);
            this.button_exit_ethercat.TabIndex = 15;
            this.button_exit_ethercat.Text = "finish";
            this.button_exit_ethercat.UseVisualStyleBackColor = true;
            this.button_exit_ethercat.UseWaitCursor = true;
            // 
            // button_ethercat
            // 
            this.button_ethercat.Location = new System.Drawing.Point(132, 27);
            this.button_ethercat.Margin = new System.Windows.Forms.Padding(2);
            this.button_ethercat.Name = "button_ethercat";
            this.button_ethercat.Size = new System.Drawing.Size(112, 19);
            this.button_ethercat.TabIndex = 14;
            this.button_ethercat.Text = "Create ";
            this.button_ethercat.UseVisualStyleBackColor = true;
            this.button_ethercat.UseWaitCursor = true;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F);
            this.label4.Location = new System.Drawing.Point(59, 30);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(57, 16);
            this.label4.TabIndex = 13;
            this.label4.Text = "Ethercat";
            // 
            // button_exit_profinet
            // 
            this.button_exit_profinet.Location = new System.Drawing.Point(266, 67);
            this.button_exit_profinet.Margin = new System.Windows.Forms.Padding(2);
            this.button_exit_profinet.Name = "button_exit_profinet";
            this.button_exit_profinet.Size = new System.Drawing.Size(112, 19);
            this.button_exit_profinet.TabIndex = 18;
            this.button_exit_profinet.Text = "finish";
            this.button_exit_profinet.UseVisualStyleBackColor = true;
            this.button_exit_profinet.UseWaitCursor = true;
            // 
            // button_profinet
            // 
            this.button_profinet.Location = new System.Drawing.Point(132, 67);
            this.button_profinet.Margin = new System.Windows.Forms.Padding(2);
            this.button_profinet.Name = "button_profinet";
            this.button_profinet.Size = new System.Drawing.Size(112, 19);
            this.button_profinet.TabIndex = 17;
            this.button_profinet.Text = "Create ";
            this.button_profinet.UseVisualStyleBackColor = true;
            this.button_profinet.UseWaitCursor = true;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F);
            this.label5.Location = new System.Drawing.Point(59, 70);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 16);
            this.label5.TabIndex = 16;
            this.label5.Text = "Profinet";
            // 
            // button_exit_profibus
            // 
            this.button_exit_profibus.Location = new System.Drawing.Point(266, 108);
            this.button_exit_profibus.Margin = new System.Windows.Forms.Padding(2);
            this.button_exit_profibus.Name = "button_exit_profibus";
            this.button_exit_profibus.Size = new System.Drawing.Size(112, 19);
            this.button_exit_profibus.TabIndex = 21;
            this.button_exit_profibus.Text = "finish";
            this.button_exit_profibus.UseVisualStyleBackColor = true;
            this.button_exit_profibus.UseWaitCursor = true;
            // 
            // button_profibus
            // 
            this.button_profibus.Location = new System.Drawing.Point(132, 108);
            this.button_profibus.Margin = new System.Windows.Forms.Padding(2);
            this.button_profibus.Name = "button_profibus";
            this.button_profibus.Size = new System.Drawing.Size(112, 19);
            this.button_profibus.TabIndex = 20;
            this.button_profibus.Text = "Create ";
            this.button_profibus.UseVisualStyleBackColor = true;
            this.button_profibus.UseWaitCursor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F);
            this.label6.Location = new System.Drawing.Point(59, 111);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(57, 16);
            this.label6.TabIndex = 19;
            this.label6.Text = "Profibus";
            // 
            // button_exit_canopen
            // 
            this.button_exit_canopen.Location = new System.Drawing.Point(266, 149);
            this.button_exit_canopen.Margin = new System.Windows.Forms.Padding(2);
            this.button_exit_canopen.Name = "button_exit_canopen";
            this.button_exit_canopen.Size = new System.Drawing.Size(112, 19);
            this.button_exit_canopen.TabIndex = 24;
            this.button_exit_canopen.Text = "finish";
            this.button_exit_canopen.UseVisualStyleBackColor = true;
            this.button_exit_canopen.UseWaitCursor = true;
            // 
            // button_canopen
            // 
            this.button_canopen.Location = new System.Drawing.Point(132, 149);
            this.button_canopen.Margin = new System.Windows.Forms.Padding(2);
            this.button_canopen.Name = "button_canopen";
            this.button_canopen.Size = new System.Drawing.Size(112, 19);
            this.button_canopen.TabIndex = 23;
            this.button_canopen.Text = "Create ";
            this.button_canopen.UseVisualStyleBackColor = true;
            this.button_canopen.UseWaitCursor = true;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F);
            this.label7.Location = new System.Drawing.Point(59, 152);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(65, 16);
            this.label7.TabIndex = 22;
            this.label7.Text = "CanOpen";
            // 
            // button_exit_ethernetip
            // 
            this.button_exit_ethernetip.Location = new System.Drawing.Point(266, 191);
            this.button_exit_ethernetip.Margin = new System.Windows.Forms.Padding(2);
            this.button_exit_ethernetip.Name = "button_exit_ethernetip";
            this.button_exit_ethernetip.Size = new System.Drawing.Size(112, 19);
            this.button_exit_ethernetip.TabIndex = 27;
            this.button_exit_ethernetip.Text = "finish";
            this.button_exit_ethernetip.UseVisualStyleBackColor = true;
            this.button_exit_ethernetip.UseWaitCursor = true;
            // 
            // button_ethernetip
            // 
            this.button_ethernetip.Location = new System.Drawing.Point(132, 191);
            this.button_ethernetip.Margin = new System.Windows.Forms.Padding(2);
            this.button_ethernetip.Name = "button_ethernetip";
            this.button_ethernetip.Size = new System.Drawing.Size(112, 19);
            this.button_ethernetip.TabIndex = 26;
            this.button_ethernetip.Text = "Create ";
            this.button_ethernetip.UseVisualStyleBackColor = true;
            this.button_ethernetip.UseWaitCursor = true;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F);
            this.label8.Location = new System.Drawing.Point(52, 192);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(72, 16);
            this.label8.TabIndex = 25;
            this.label8.Text = "Ethernet/ip";
            // 
            // Board_comunication
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1018, 465);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.richTextBox1);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "Board_comunication";
            this.Text = "Board_comunication";
            this.Load += new System.EventHandler(this.Board_comunication_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox combox_portt;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button button_open;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Button button_cclink;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button_finish_cclink;
        private System.Windows.Forms.Button button_exit_APP;
        private System.Windows.Forms.Button modbus_finish_task;
        private System.Windows.Forms.Button modbus_create_task;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button button_exit_ethernetip;
        private System.Windows.Forms.Button button_ethernetip;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button button_exit_canopen;
        private System.Windows.Forms.Button button_canopen;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button button_exit_profibus;
        private System.Windows.Forms.Button button_profibus;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button button_exit_profinet;
        private System.Windows.Forms.Button button_profinet;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button button_exit_ethercat;
        private System.Windows.Forms.Button button_ethercat;
        private System.Windows.Forms.Label label4;
    }
}